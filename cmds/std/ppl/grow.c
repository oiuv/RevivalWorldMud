/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : grow.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-04
 * Note   : ��~�ش�/�b��/�i�ޫ��O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <map.h>
#include <material.h>
#include <skill.h>

inherit COMMAND;

#define GROW_STR_COST		20
#define HARVEST_STR_COST	400
#define MAINTAIN_STR_COST	300
#define DEFAULT_GROW		10

string help = @HELP
    �ΨӥͲ��A�� / ���� / �i�޳������O

    grow [�ƶq] [�ؤl�κدb�γ��]]	- �i�漽��/�t��/���񳽭]���ʧ@
    grow [�ƶq] [���ι}��]		- �i����@�����i
    grow harvest			- �������O
    grow maintain			- ��a���O�A�ΨӴ��ɤg�a���šA��֤ѵM�a�`�l�`�P�W�[���q
    grow maintain '����'		- �@����a�h��
    grow -d				- �����ثe���Ͳ�
    
HELP;

private void maintain_levelup(object me, array loc)
{
	int level = 0;

	switch( CITY_D->add_coor_data(loc, "maintain_time", 1) )
	{
		case 1:
			level = 1; break;
		case 50:
			level = 2; break;
		case 1000:
			level = 3; break;
		case 10000:
			level = 4; break;
		case 50000:
			level = 5; break;
		case 100000:
			level = 6; break;
		case 200000:
			level = 7; break;
		case 500000:
			level = 8; break;
		case 1000000:
			level = 9; break;
		case 2000000:
			level = 10; break;
	}
	
	if( level )
	{
		CITY_D->set_coor_data(loc, "growth_level", level);
		msg("���a���g�a�����H��$ME�����ԷӮƴ��ɬ��� "HIY+level+NOR" �šC\n", me, 0, 1);
		
		if( !SECURE_D->is_wizard(me->query_id(1)) )
		TOP_D->update_top("maintain", save_variable(loc), CITY_D->query_coor_data(loc, "growth_level"), me->query_idname(), CITY_D->query_coor_data(loc, TYPE));
	}
}

private void do_command(object me, string arg)
{
	int amount, growth_level, multiplicand;
	array loc = query_temp("location", me);
	object ob;
	string msg="", file;
	mapping coor_data, growth_data;
	
	if( (MAP_D->query_map_system(loc)) != CITY_D )
		return tell(me, pnoun(2, me)+"�L�k�b���i��Ͳ��C\n");
	
	coor_data = CITY_D->query_coor_data(loc);
	
	if( !mapp(coor_data) )
		return tell(me, pnoun(2, me)+"�L�k�b���i��Ͳ��C\n");

	growth_data = coor_data["growth"];
	
	if( coor_data["owner"] != me->query_id(1) )
		return tell(me, "�o���g�a���O"+pnoun(2, me)+"���C\n");
	
	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n������˪��Ͳ��ʧ@�H�� help grow�C\n");

	if( arg == "-d" )
	{
		if( !mapp(growth_data) )
			return tell(me, "�o�̨S���Ͳ����󲣫~�C\n");
			
		CITY_D->delete_coor_data(loc, "growth");
		GROWTH_D->set_status(loc);
		return tell(me, pnoun(2, me)+"�����F���a�����~�Ͳ��C\n");
	}
		
	if( arg == "maintain" || sscanf(arg, "maintain %d", amount) == 1 )
	{
		int cost_str;

		switch(coor_data[TYPE])
		{
			case FARM:
			case PASTURE:
			case FISHFARM:
				break;
			default:
				return tell(me, pnoun(2, me)+"�L�k��o���a�C\n");
		}

		if( amount < 1 )
			amount = 1;

		if( mapp(growth_data) )
			return tell(me, "�o�̥��b�Ͳ����~�A�L�k�i���a�ʧ@�C\n");
			
		cost_str = to_int(MAINTAIN_STR_COST * pow(amount, 0.9));

		if( !me->cost_stamina(cost_str) )
			return tell(me, pnoun(2, me)+"����O���� "+cost_str+"�A�L�k�i���a���ʧ@�C\n");

		msg("$ME�ӶO "+cost_str+" ����O���Ԫ��O�i��a�A�Ϥg�a�~���ܱo�����u�}�C("WHT"+"HIW+amount+NOR")\n", me, 0, 1);
		
		// �ɯ��ˬd
		for(int i = 0; i<amount ; ++i)
			maintain_levelup(me, loc);
		
		me->add_social_exp(amount * (10+random(30)));
		GROWTH_D->set_status(loc);

		return;
	}
	
	// ����
	if( arg == "harvest" )
	{
		if( !mapp(growth_data) )
			return tell(me, "�o�̨S���Ͳ����󲣫~�A�L�k�i�榬���ʧ@�C\n");
			
		if( !growth_data["wait_for_harvesting"] )
			return tell(me, "�ثe�ٵL�k�i�榬�����ʧ@�C\n");

		if( !me->cost_stamina(HARVEST_STR_COST) )
			return tell(me, pnoun(2, me)+"����O�������i�榬�����ʧ@�C\n");

		msg += growth_data["harvest_msg"];
		
		growth_level = CITY_D->query_coor_data(loc, "growth_level");

		foreach(file, amount in growth_data["harvest"])
		{
			ob = new(file);
			
			if( !objectp(ob) ) continue;
			
			multiplicand = growth_data["input_amount"] * growth_data["harvest_percent"] * (growth_level+DEFAULT_GROW) / 1000.;
			
			amount *= multiplicand;

			set_temp("amount", amount, ob);
		
			msg += "�D"+ob->short(1)+"\n";
	
			me->add_social_exp(random(amount * 30));

			ob->move_to_environment(me);
		}
		
		msg(msg, me, 0, 1);
		
		// �ɯ��ˬd
		maintain_levelup(me, loc);

		TREASURE_D->get_treasure_growth(me, growth_level, coor_data[TYPE]);

		CITY_D->delete_coor_data(loc, "growth");
		CITY_D->delete_coor_data(loc, "status");
		GROWTH_D->set_status(loc);
		return;
	}
	
	if( sscanf(arg, "%d %s", amount, arg) != 2 )
		amount = 1;
	else if( amount < 1 )
		return tell(me, "�п�J���T���ƶq�C\n");
	
	if( !objectp(ob = present(arg)) )
		return tell(me, pnoun(2, me)+"���W�èS�� "+arg+" �o�تF��C\n");

	if( count(amount, ">", query_temp("amount", ob)||1) )
		return tell(me, pnoun(2, me)+"���W�S������h��"+ob->query_idname()+"�C\n");

	// �Y�����~�O�ؤl/���]/�ذ��ؤ�/���]
	if( query("growth", ob) )
	{
		if( !mapp(growth_data) )
			growth_data = copy(query("growth", ob));
		else
			return tell(me, "�o���g�a�w�g�b�Ͳ�"+growth_data["idname"]+"���C\n");

		if( growth_data["type"] != coor_data[TYPE] )
			return tell(me, ob->query_idname()+"�L�k�b���i��Ͳ��C\n");

		if( growth_data["type"] == PASTURE && amount == 1 )
			return tell(me, "�u���@��"+ob->query_idname()+"�n����t...�H\n");

		if( amount > DEFAULT_GROW + coor_data["growth_level"] )
		{
			switch(growth_data["type"])
			{
				case FARM:
					return tell(me, "�o���A�a�̦h�@���u��ѧ@ "+(DEFAULT_GROW+coor_data["growth_level"])+" �ӧ@���C\n");
					break;
				case PASTURE:
					return tell(me, "�o�������̦h�@���u��}�i "+(DEFAULT_GROW+coor_data["growth_level"])+" ���ʪ��C\n");
					break;
				case FISHFARM:
					return tell(me, "�o�B�i�޳��̦h�@���u��i�� "+(DEFAULT_GROW+coor_data["growth_level"])+" �Ӥ����C\n");
					break;
			}
		}
	
		foreach(string skill, int num in growth_data["skill"])
			if( me->query_skill_level(skill) < num )
				return tell(me, pnoun(2, me)+"��"+(SKILL(skill))->query_idname()+"�ޯण���H�Ͳ�"+ob->query_idname()+"�C(�ݨD���� Lv"+num+")\n");
		
		if( !me->cost_stamina(GROW_STR_COST * amount) )
			return tell(me, pnoun(2, me)+"����O�����F�C\n");

		growth_data["input_amount"] = amount;
		growth_data["harvest_percent"] = 100;
		
		foreach(file, int num in growth_data["material"])
			growth_data["material"][file] *= amount;
		
		CITY_D->set_coor_data(loc, "growth", growth_data);
		GROWTH_D->set_status(loc, growth_data);

		switch(growth_data["type"])
		{
			case FARM:
				msg("$ME�}�l�b�o���Цa�W�ѧ@"+QUANTITY_D->obj(ob, amount)+"�C\n"+growth_data["start_msg"], me, ob, 1);
				break;
			case PASTURE:
				msg("$ME�}�l�b�o�������W�b�i"+QUANTITY_D->obj(ob, amount)+"�C\n"+growth_data["start_msg"], me, ob, 1);
				break;
			case FISHFARM:
				msg("$ME�}�l�b�o�B�i�޳��W�i��"+QUANTITY_D->obj(ob, amount)+"�C\n"+growth_data["start_msg"], me, ob, 1);
				break;
		}
		
		
		destruct(ob, amount);
		
		return;
	}
	
	if( !mapp(growth_data) )
		return tell(me, ob->query_idname()+"�L�k�ΨӶi��Ͳ��C\n");

	if( mapp(growth_data["material"]) )
	switch(coor_data[TYPE])
	{
		case FARM:
		case PASTURE:
		case FISHFARM:
		{
			string basename = base_name(ob);
			
			if( !undefinedp(growth_data["material"][basename]) )
			{
				if( !me->cost_stamina(GROW_STR_COST * amount) )
					return tell(me, pnoun(2, me)+"����O�����F�C\n");
				
				me->add_social_exp(random(amount * 50));
				
				growth_data["material"][basename] -= amount;
				
				if( growth_data["material"][basename] <= 0 )
					map_delete(growth_data["material"], basename);
					
				switch(growth_data["type"])
				{
					case FARM:
						msg("$ME�Q��"+QUANTITY_D->obj(ob, amount)+"��o���A�a�i����@�C\n", me, ob, 1);
						break;
					case PASTURE:
						msg("$ME�Q��"+QUANTITY_D->obj(ob, amount)+"��o�������i��}�i�C\n", me, ob, 1);
						break;
					case FISHFARM:
						msg("$ME�Q��"+QUANTITY_D->obj(ob, amount)+"��o�B�i�޳��i�����i�C\n", me, ob, 1);
						break;
				}

				// ���ƿ�J����, �}�l�ͪ�
				if( !sizeof(growth_data["material"]) )
				{
					map_delete(growth_data, "material");
					growth_data["current_growing"] = growth_data["start_msg"];
					msg(growth_data["start_msg"], me, 0, 1);
					growth_data["started"] = 1;
				}
			
				CITY_D->set_coor_data(loc, "growth", growth_data);
				GROWTH_D->set_status(loc, growth_data);
				destruct(ob, amount);
				
				return;
			}
			break;
		}
	}
	
	tell(me, ob->query_idname()+"�L�k�ΨӶi��Ͳ��C\n");
}
