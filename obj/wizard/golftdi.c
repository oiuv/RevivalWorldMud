/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : golftdi.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-09-12
 * Note   : GOLF TDI
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>
#include <daemon.h>
#include <secure.h>
#include <map.h>
#include <location.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

mapping actions;

void fire_process(object me, object target, int combo, int sec)
{
	if( sec == 0 )
	{
		int damage, faint;
		for(int i=0;i<combo;i++)
		{
			damage = range_random(1, 125);
			
			if( !target->cost_health(damage) )
				faint = 1;
			
			msg("$YOU�Q$ME�o�g�� 12 �������}�C�����A�ɭP "HIR+damage+NOR" ���ͩR�ˮ`("NOR GRN+target->query_health_cur()+NOR"/"HIG+target->query_health_max()+NOR")�C\n", me, target, 1);

			if( faint )
				target->faint();
		}
		
		delete_temp("delay/fire");
	}
	else
	{
		msg("�Z�� 12 �������}�C����$YOU�٦� "+sec+" ����...\n", me, target, 1);
		call_out((: fire_process, me, target, combo, sec-1 :), 1);
	}
}

void fire(object me, string arg)
{
	int combo = random(11)+1;
	object target = find_player(arg) || present(arg);
	
	if( !me->is_equipping_object(this_object()) )
		return tell(me, pnoun(2, me)+"�������˳�"+this_object()->query_idname()+"�C\n");

	if( query_temp("delay/fire") > time() )
		return tell(me, "���Z���L�k�s��o�g�A�е��ݪZ���t�ΧN�o�C\n");

	if( !objectp(target) )
		return tell(me, "�S�� "+arg+" �o�ӤH�C\n");
	
	for(int i=0;i<combo;i++)
	msg("$ME���a"+this_object()->query_idname()+"���$YOU�o�g 12 �������}�C("HIY+combo+NOR YEL" �s�o"NOR")�C\n", me, target, 1);
	set_temp("delay/fire", time() + 5);
	call_out((: fire_process, me, target, combo, 5 :), 1);
}

void killest_confirm(object me, string arg, array loc, string yn)
{
	if( yn == "y" )
	{
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"�[�p"+this_object()->query_idname()+"��� "+loc_short(loc)+" �o�g 12 �������}�C�A�Ӧa�a��W���Ҧ��F�������ǭ��Ϸ��C\n");
		ESTATE_D->remove_estate(loc);
		ESTATE_D->remove_unknown_estate(loc);
	}
	else
		tell(me, pnoun(2, me)+"�����F�����C\n");

	me->finish_input();
}

void killest(object me, string arg)
{
	array loc = query_temp("location", me);		

	if( SECURE_D->level_num(me->query_id(1)) < ADMIN )
		return tell(me, "�u�� Administrator �i�H�ϥγo���Z���C\n");	
	
	if( !arrayp(loc) )
		return tell(me, pnoun(2, me)+"�����b�a�ϤW�}�ҳo�ӪZ���C\n");
		
	tell(me, HIY+pnoun(2, me)+"�T�w�����R���b�o�Ӯy�ФW�� "+ESTATE_D->query_loc_estate(loc)["type"]+" �Цa��"NOR"("HIR"�`�N�I�o�O�u���I"NOR")�H(y/n)");
	input_to( (: killest_confirm, me, arg, loc :) );
}

/*
void killcity(object me, string arg)
{
	string city;
	int num;
	array loc;
	string owner;

	if( sscanf(arg, "'%s' %d", city, num) != 2 )
		sscanf(arg, "%s %d", city, num);
	
	num--;
	
	if( !CITY_D->city_exist(city, num) )
		return tell(me, "���������s�b\n");
	
	for(int i=0;i<99;i++)
	for(int j=0;j<99;j++)
	{
		loc = arrange_city_location(i, j, city, num);
		
		owner = CITY_D->query_coor_data(loc, "owner");
		
		// �S�֦���?? ����F��..����F
		if( !owner )
		{
			switch(CITY_D->query_coor_data(loc, TYPE))
			{
				case FARM:
				case PASTURE:
				case FISHFARM:
				case POOL:
				case LIGHT:
				case PAVILION:
				case TREE:
				case GRASS:
				case STATUE:
					killest_confirm(me, arg, loc, "y");
					break;
				default:
					break;
			}
		}
		// �F�����g�a
		else if( belong_to_government(owner) )
		{
			switch(CITY_D->query_coor_data(loc, TYPE))
			{
				case FARM:
				case PASTURE:
				case FISHFARM:
				case POOL:
				case PAVILION:
				case TREE:
				case GRASS:
				case STATUE:
					killest_confirm(me, arg, loc, "y");
					break;
				default:
					break;
			}
		}
		// ���~���g�a
		else if( belong_to_enterprise(owner) )
		{
			switch(CITY_D->query_coor_data(loc, TYPE))
			{
				case FARM:
				case PASTURE:
				case FISHFARM:
				case POOL:
				case PAVILION:
				case LIGHT:
				case TREE:
				case GRASS:
				case STATUE:
					killest_confirm(me, arg, loc, "y");
					break;
				default:
					break;
			}
		}
		// �����D�O�֪��g�a
		else {
			switch(CITY_D->query_coor_data(loc, TYPE))
			{
				case LIGHT:
					killest_confirm(me, arg, loc, "y");
					break;
				default:
					break;
			}
		}
	}
}
*/

void shield(object me, string arg)
{
	if( !me->is_equipping_object(this_object()) )
		return tell(me, pnoun(2, me)+"�������˳�"+this_object()->query_idname()+"�C\n");

	if( !query_temp("shield/on") )
	{
		msg("$ME�Ұ�"+this_object()->query_idname()+"�۰ʽ��W���@�n�t�ΡC\n", me, 0, 1);
		set_temp("status", HIR"(���@�n)"NOR);
		set_temp("shield/on", 1);
	}
	else
	{
		msg("$ME����"+this_object()->query_idname()+"�۰ʽ��W���@�n�t�ΡC\n", me, 0, 1);
		delete_temp("status");
		delete_temp("shield/on");
	}
}

void antimatter_process(object me, object target, int sec)
{
	if( sec == 0 )
	{
		msg("$YOU�Q$ME�ұҰʪ��Ϫ��贲���f���������A�ɭP "HIR+1000000+NOR" ���ͩR�ˮ`("NOR RED+"-1000000"+NOR"/"HIG+target->query_health_max()+NOR")�C\n", me, target, 1);

		target->faint();
		
		if( userp(target) )
		LOGOUT_D->quit(target);
		
		delete_temp("delay/antimatter");
	}
	else
	{
		msg("�Z���Ϫ��贲���f�E����q�����٦� "+sec+" ����...\n", me, target, 1);
		call_out((: antimatter_process, me, target, sec-10 :), 10);
	}
}

// �Ϫ��贲���f
void antimatter(object me, string arg)
{
	object target = find_player(arg) || present(arg);
	
	if( !me->is_equipping_object(this_object()) )
		return tell(me, pnoun(2, me)+"�������˳�"+this_object()->query_idname()+"�C\n");

	if( query_temp("delay/antimatter") > time() )
		return tell(me, "���Z���L�k�s��o�g�A�е��ݪZ���t�ΧN�o�C\n");

	if( !objectp(target) )
		return tell(me, "�S�� "+arg+" �o�ӤH�C\n");
		
	msg("$ME���a"+this_object()->query_idname()+"���$YOU�}�ҤϪ��贲���f�C\n", me, target, 1);
	set_temp("delay/antimatter", time() + 60);
	call_out((: antimatter_process, me, target, 60 :), 1);
}

void create()
{
	set_idname(HIW"golf"WHT" tdi"NOR, HIW"GOLF"NOR"-"WHT"TDI");
	set_temp("status", HIY"�p��"NOR YEL"��");

	actions = ([
		"fire" : (: fire :),
		"shield" : (: shield :),
		"antimatter": (: antimatter :),
		"killest": (: killest :),
		//"killcity":(:killcity:),
	]);

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", @LONG
�@�@�Ȧ⪺�p�����A���ӥi�R�����j����...
LONG
	);

	set("unit", "��");
	set("mass", 0);
	set("value", 0);
	set("flag/no_amount", 1);

	set(BUFF_STR, 200);
	set(BUFF_PHY, 200);
	set(BUFF_INT, 200);
	set(BUFF_AGI, 200);
	set(BUFF_CHA, 200);

	set(BUFF_STAMINA_MAX, 200);
	set(BUFF_HEALTH_MAX, 200);
	set(BUFF_ENERGY_MAX, 200);
	
	set(BUFF_STAMINA_REGEN, 200);
	set(BUFF_HEALTH_REGEN, 200);
	set(BUFF_ENERGY_REGEN, 200);

	set(BUFF_LOADING_MAX, 200);
	set(BUFF_FOOD_MAX, 200);
	set(BUFF_DRINK_MAX, 200);
	
	set(BUFF_SOCIAL_EXP_BONUS, 200);

	set("buff/status", HIW"GOLF"NOR"-"WHT"TDI");
	
	::setup_equipment(EQ_MOUNT, HIG"�r"NOR GRN"�p"NOR);
}
