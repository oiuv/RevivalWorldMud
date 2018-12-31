/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : open.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-08-24
 * Note   : �ؿv���}�i���O�A���n���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <location.h>
#include <map.h>
#include <message.h>
#include <roommodule.h>

inherit COMMAND;

string help = @HELP
    ��A�N����զX���@�ӱK�X���a�ϮɡA�K�i�H�ϥΦ����O�Ӷ}�i�һݭn���ؿv��
���O�ϥήɥ����n���b�ؿv�������W�A�L�׬O�����ξ�����i�H�C�ثe�}�񪺫ؿv��
���G�p���ө��B��v... ���C

    �b�ؿv�������ɫh�i�Q�� open up ���O�ө��W�[�\�Ӽh�A�C�[�\�@�h�K�i�h�X
�@���ж��A�P�ɫؿv���U���a�Ӫ��H��P�Ӿ��]�U���A�ña�ʫ����c�a�P���O��O�C

    �b�[�\�Ӽh������i�H�Q�ΥH�U���O�Ӷ}�q�Ӽh�����q�D�A�C�}�P�@�ӳq�D�ݪ�
�O 10,000 �����
      
    open north		- �}�P���_�䪺�q�D
    open south		- �}�P���n�䪺�q�D
    open west		- �}�P�����䪺�q�D
    open east		- �}�P���F�䪺�q�D
      
�۹��}�i���O�A�]���ҿת��������O�C

�������O: blist, close
HELP;

#define WALL_TABLE	0
#define ROOM_TABLE	1

#define OPEN_DOOR_COST	10000

#define CORRESPONDING_DIRECTION	([ "north":"south", "south":"north", "west":"east", "east":"west" ])

void confirm_build_up(object me, object env, string owner, string nfile, string ofile, string bfile, int newfloor, string cost, string yn)
{
	string city;
	object room, broom;
	string moneyunit;

	if( lower_case(yn) != "y" )
	{
		tell(me, pnoun(2, me)+"��󩹤W�[�\�Ӽh�C\n");
		me->finish_input();
		return;
	}

	city = env->query_city();
	moneyunit = MONEY_D->city_to_money_unit(city);

	if( belong_to_enterprise(owner) )
	{
		moneyunit = MONEY_D->query_default_money_unit();
		
		if( !ENTERPRISE_D->change_assets(query("enterprise", me), "-"+cost) )
		{
			tell(me, "���~����w�g�����[�\�Ӽh�F�C\n");
			return me->finish_input();
		}
	}
	else if( !me->spend_money(moneyunit, cost) )
	{
		tell(me, pnoun(2,me)+"���W�� $"+moneyunit+" �������F�I�I\n");
		return me->finish_input();
	}

	room = load_object(nfile);
	broom = load_object(bfile);
	
	addn("floor", 1, broom);
	broom->save();
	
	set("exits/down", ofile, room);
	set("firstfloor", bfile, room);
	set("owner", owner, room);
	set("short", copy(query("short", broom)), room);
	room->save();

	set("exits/up", nfile, env);
	env->save();
	
	if( !(newfloor%5) )	
		CHANNEL_D->channel_broadcast("city", me->query_idname()+"��O "HIY"$"+moneyunit+" "+NUMBER_D->number_symbol(cost)+NOR" �N"+query("short", broom)+"���W�[�\�ܲ� "HIW+newfloor+NOR" �ӡC", me);
	
	if( !(newfloor%20) )
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"�b"+CITY_D->query_city_name(city)+"��"+query("short", broom)+"�ؿv��} "HIW+newfloor+NOR" �ӡA������a "HIY+(newfloor/20)+NOR" �Ŧa�СC");

	if( newfloor == 160 )
		me->add_title( HIR"��"NOR RED"�Q"NOR HIR"��"NOR RED"�J"NOR );
	else if( newfloor == 120 )
		me->add_title( HIR"�ؿv��"NOR RED"�Q�D"NOR );
	else if( newfloor == 100 )
		me->add_title( HIR"���u"NOR RED"�ؿv�v"NOR );
	else if( newfloor == 50 )
		me->add_title( HIR"�ؿv��"NOR RED"�s�q"NOR );
		
	tell(me, pnoun(2, me)+"��O "HIY"$"+moneyunit+" "+NUMBER_D->number_symbol(cost)+NOR" �N"+query("short", broom)+"���W�[�\�ܲ� "+newfloor+" �ӡC\n");

	if( !wizardp(me) )
		TOP_D->update_top("building", base_name(broom), newfloor, owner, broom->query_room_name(), city);

	me->finish_input();
}

void open_building(object me, string arg)
{
	int num;
	string owner, city, enterprise, cost, moneyunit;
	mapping table;
	array loc = query_temp("location",me);
	array building_info;

	if( !arrayp(loc) || !environment(me)->is_maproom() || !CITY_D->is_city_location(loc) )
		return tell(me, pnoun(2, me)+"�����b�a�ϤW�}�i�ؿv���C\n");
		
	owner = CITY_D->query_coor_data(loc, "owner");
	
	city = loc[CITY];
	num = loc[NUM];

	moneyunit = MONEY_D->city_to_money_unit(city);
	
	if( !owner )
		return tell(me, "�o���g�a���ݩ����H�A"+pnoun(2, me)+"�L�k�b���}�i�ؿv���C\n");

	arg = lower_case(arg);

	if( !BUILDING_D->has_building(arg) )
		return tell(me, "�S�� "+arg+" �o�ثؿv�������A�d�߫ؿv���C��п�J blist�C\n");
	
	building_info = BUILDING_D->query_building(arg);

	if( building_info[ROOMMODULE_TESTING] && !wizardp(me) )
		return tell(me, "���ؿv�����b�ʳ����դ��A�|���}��سy�C\n");

	// �F���g�a
	if( belong_to_government(owner) )
	{
		if( !wizardp(me) )
		{
			if( !CITY_D->is_mayor_or_officer(loc[CITY], me) )
				return tell(me, pnoun(2, me)+"���O�o�y�����������Ωx���A�L�k�ϥάF���g�a�سy�ؿv���C\n");
		
			if( !(building_info[ROOMMODULE_BUILDINGTYPE] & GOVERNMENT) )
				return tell(me, "�F���g�a�W�u��سy�F���ؿv�C\n");
		}
	}
	// ���~�g�a
	else if( belong_to_enterprise(owner) )
	{
		enterprise = query("enterprise", me);
		moneyunit = MONEY_D->query_default_money_unit();

		if( !ENTERPRISE_D->is_member(enterprise, me->query_id(1)) )
			return tell(me, pnoun(2, me)+"���O�o�ӥ��~���Ϊ������A�L�k�ϥΥ��~�g�a�سy�ؿv���C\n");
		
		if( enterprise != owner[11..] )
			return tell(me, "�o���g�a�ä��ݩ�"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"���~���ΡC\n");
			
		if( !(building_info[ROOMMODULE_BUILDINGTYPE] & ENTERPRISE) )
			return tell(me, "���~�g�a�W�u��سy���~�ؿv�C\n");
	}
	// �ӤH�g�a
	else if( belong_to_individual(owner) )
	{
		if( owner != me->query_id(1) )
			return tell(me, "�o���g�a�O�ݩ� "+owner+" ���A�ä��O"+pnoun(2, me)+"���g�a�C\n");
		
		if( !(building_info[ROOMMODULE_BUILDINGTYPE] & INDIVIDUAL) )
			return tell(me, "�ӤH�g�a�W�u��سy�ӤH�ؿv�C\n");
	}
	
	// �سy����
	if( arg == "fence" )
	{
		int land_type = CITY_D->query_coor_data(loc, TYPE);
		string estate_type = ESTATE_D->query_loc_estate(loc)["type"];
		
		if( land_type != WALL && land_type != DOOR )
			return tell(me, pnoun(2, me)+"�������b���z�Ϊ��W�ӱҰʳ���\��C\n");

		if( estate_type == "fence" )
			return tell(me, "�o�̤w�g�O����F�C\n");

		if( estate_type != "land" )
			return tell(me, "�o�̤���Ұʳ���\��C\n");			
		
		if( CITY_D->query_coor_data(loc, TYPE) == WALL )
			CITY_D->set_coor_data(loc, FLAGS, NO_MOVE);

		table = allocate_mapping(0);
		table["type"] = "fence";
		table["regtime"] = time();
		table["walltable"] = ({ save_variable(loc) });
		table["roomtable"] = allocate(0);
		
		CITY_D->set_coor_icon(loc, WHT+remove_ansi(CITY_D->query_coor_icon(loc))+NOR);

		ESTATE_D->set_estate(owner, table, "fence", city, num);
		
		msg("$ME�b���a�ҰʤF����\��C\n", me, 0, 1);
		CHANNEL_D->channel_broadcast("city", me->query_idname()+"�b"+loc_short(loc)+NOR"�سy�F�u"HIM"����"NOR"�v", me);
		return;
	}
	
	if( building_info[ROOMMODULE_MAXLIMIT] > 0 && ESTATE_D->query_owner_amount(owner, arg) >= building_info[ROOMMODULE_MAXLIMIT] )
		return tell(me, "���ثؿv�C�ӬF��/���~/���a���o�سy�W�L "+building_info[ROOMMODULE_MAXLIMIT]+" �ɡC\n");
	
	if( CITY_D->query_city_info(city, "age") < building_info[ROOMMODULE_AGE] )
		return tell(me, "�ثe�o�y����������ɥN���u"+CITY_D->query_age_name(city)+"�v�A�L�k�سy�������ؿv���C\n");

	// �W�L�C�y�������ؿv�ƶq����
	if( building_info[ROOMMODULE_MAXBUILDINGLIMIT] && ESTATE_D->query_city_amount(loc[CITY], loc[NUM], arg) >= building_info[ROOMMODULE_MAXBUILDINGLIMIT] )
		return tell(me, "�o�y������"+building_info[ROOMMODULE_SHORT]+"�ؿv���ƶq�w�g��F "+building_info[ROOMMODULE_MAXBUILDINGLIMIT]+" �ɪ�����A�L�k�A�W�[�F�C\n");


	// �W�����ˬd
	if( !(CITY_D->query_coor_data(loc, "region") & building_info[ROOMMODULE_REGION]) )
		return tell(me, building_info[ROOMMODULE_SHORT]+"����سy�b���ؤg�a�W���ϤW�C\n");

	// �}�i�a�I�ˬd
	if( CITY_D->query_coor_data(loc, TYPE) != DOOR )
		return tell(me, pnoun(2, me)+"�������b�j���f�Ӷi��ؿv�����}�i�����C\n");

	// ����޿�
	if( !mapp(table = BUILDING_D->analyze_building_logic(loc)) )
		return tell(me, "���ɫؿv�����سy���c�����D�A�L�k�}�i�A�бN���~�������s�سy�C\n");

	foreach( string coor, mixed data in CITY_D->query_coor_range(loc, ROOM, 1) )
		if( data )
			return tell(me, pnoun(2, me)+"�����������o�ɫؿv���~�୫�s�}�i�C\n");

	if( sizeof(table["roomtable"]) < building_info[ROOMMODULE_ROOMLIMIT] )
		return tell(me, building_info[ROOMMODULE_SHORT]+"�ֻ̤ݭn"+CHINESE_D->chinese_number(building_info[ROOMMODULE_ROOMLIMIT])+"���ж��A�o�ɫؿv�u��"+CHINESE_D->chinese_number(sizeof(table["roomtable"]))+"���ж��C\n");

	if( !building_info[ROOMMODULE_OPENCOST] )
		return tell(me, "�L�k�}�i�o�ثؿv���C\n");

	// �p��}�i��O
	cost = count(building_info[ROOMMODULE_OPENCOST], "*", sizeof(table["roomtable"]));

	if( belong_to_government(owner) )
	{
		CHANNEL_D->channel_broadcast("city", me->query_idname()+"�ʥΥ����겣 "+HIY+money(moneyunit, cost)+NOR" �b"+CITY_D->query_city_idname(city, num)+"��"+CITY_D->position(loc[X], loc[Y])+"����سy�F�@��"+building_info[ROOMMODULE_SHORT]+"�C\n", me);
		CITY_D->change_assets(city, "-"+cost);
	}
	else if( belong_to_enterprise(owner) )
	{
		if( ENTERPRISE_D->change_assets(enterprise, "-"+cost) )
			CHANNEL_D->channel_broadcast("news", "���~���Ρu"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"�v��O���~�겣 "+HIY+money(moneyunit, cost)+NOR" �b"+CITY_D->query_city_idname(city, num)+"�سy�F�@��"+building_info[ROOMMODULE_SHORT]+"�C\n");		
		else if( me->spend_money(moneyunit, cost) )
		{
			ENTERPRISE_D->change_invest(enterprise, me->query_id(1), cost);
			CHANNEL_D->channel_broadcast("news", "���~���Ρu"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"�v��O�ӤH��� "+HIY+money(moneyunit, cost)+NOR" �b"+CITY_D->query_city_idname(city, num)+"�سy�F�@��"+building_info[ROOMMODULE_SHORT]+"�C\n");
			tell(me, "���~���Ϊ��{����������H�}�i�ؿv�F�A�]����O"+pnoun(2, me)+"���p�H��� "+money(moneyunit, cost)+" �i��}�i�C\n");
		}
		else
			return tell(me, "���~�겣��"+pnoun(2, me)+"���p�H�{�����w�g�����H��I "+HIY+money(moneyunit, cost)+NOR+"�C\n");
	}
	else if( me->spend_money(moneyunit , cost) )
		CHANNEL_D->channel_broadcast("city", me->query_idname()+"��O "+HIY+money(moneyunit, cost)+NOR" �b"+CITY_D->query_city_idname(city, num)+"��"+CITY_D->position(loc[X], loc[Y])+"����سy�F�@��"+building_info[ROOMMODULE_SHORT]+"�C\n", me);
	else
		return tell(me, "�}�i"+building_info[ROOMMODULE_SHORT]+"�ݭn�򥻸��C�P�}�i�����O��"+HIY+money(moneyunit, cost)+NOR"�C\n");


	BUILDING_D->materialize_city_building(me, owner, table, city, num, arg);
	
}


void open_up(object me)
{
	int x, y, z, newfloor;
	object env = environment(me);
	string owner, fhead, file = base_name(env), nfile;
	string type, bfile, cost, money_unit = MONEY_D->city_to_money_unit(env->query_city());

	owner = query("owner", env);

	if( env->is_maproom() )
		return tell(me, pnoun(2, me)+"�����b�ؿv���������W�[�\�C\n");
	
	if( belong_to_enterprise(owner) )
	{
		if( query("enterprise", me) != owner[11..] )
			return tell(me, pnoun(2, me)+"���O�����~���Ϊ������C\n");
	}
	else if( owner != me->query_id(1) )
		return tell(me, pnoun(2, me)+"���O�o�ɫؿv�����֦��̡C\n");

	if( query("exits/up", env) )
		return tell(me, "�W�@�h���Ӽh�w�g�[�\�����C\n");

	if( sscanf(file, "%s/room/%d_%d_%d_%s", fhead, x, y, z, type) == 5 || sscanf(file, "%s/room/%d_%d_%s", fhead, x, y, type) == 4 )
	{
		nfile = fhead+"/room/"+x+"_"+y+"_"+(z+1)+"_"+type;
		bfile = fhead+"/room/"+x+"_"+y+"_"+type;
		
		newfloor = z+2;
		
		if( newfloor > BUILDING_D->query_building_table()[type][ROOMMODULE_MAXFLOOR] )
			return tell(me, "���ثؿv���̦h�u��[�\�ܦ��ӡC\n");

		if( newfloor <= 100 )
		{	
			if( me->query_skill_level("architectonic") < newfloor )
				return tell(me, pnoun(2, me)+"���ؿv�޳N�����H�A���W�[�\�Ӽh�C\n");
		}
		else
		{
			if( me->query_skill_level("architectonic") < 100 )
				return tell(me, pnoun(2, me)+"�������N�ؿv�޳N���ɨ�̰����šC\n");
			
			if( me->query_skill_level("architectonic-adv")+100 < newfloor )
				return tell(me, pnoun(2, me)+"�������ؿv�ޯ൥�Ť����H�A���W�[�\�Ӽh�C\n");
		}
		
		cost = BUILDING_D->query_floor_value(newfloor);
			
		tell(me, "�[�\�� "+newfloor+" �Ӽh�ݭn��� "HIY"$"+money_unit+" "+NUMBER_D->number_symbol(cost)+HIG"\n�O�_�T�w�[�\�H(Y/N)"NOR);
		input_to((: confirm_build_up, me, env, owner, nfile, file, bfile, newfloor, cost:));
		
		return;
	}
	
	tell(me, "�o�ɫؿv���L�k���W�[�\�Ӽh�C\n");
}

void open_door(object me, string arg)
{
	int x, y, z;
	object nextroom, env = environment(me);
	string type, fhead, file = base_name(env);
	string owner, money_unit = MONEY_D->city_to_money_unit(env->query_city());

	owner = query("owner", env);

	if( env->is_maproom() )
		return tell(me, pnoun(2, me)+"�����b�ؿv���������W�[�\�C\n");
	
	if( belong_to_enterprise(owner) )
	{
		if( query("enterprise", me) != owner[11..] )
			return tell(me, pnoun(2, me)+"���O�����~���Ϊ������C\n");
	}
	else if( owner != me->query_id(1) )
		return tell(me, pnoun(2, me)+"���O�o�ɫؿv�����֦��̡C\n");

	if( query("exits/"+arg, env) )
		return tell(me, "���Ӥ�V���q�D�w�g���}�C\n");

	if( sscanf(file, "%s/room/%d_%d_%d_%s", fhead, x, y, z, type) != 5 )
		return tell(me, pnoun(2, me)+"�u��b 2 �ӥH�W���ж��}�P�q�D�C\n");
		
	if( arg == "south" ) y++;
	else if( arg == "north" ) y--;
	else if( arg == "west" ) x--;
	else if( arg == "east" ) x++;
	
	if( !objectp(nextroom = find_object(fhead+"/room/"+x+"_"+y+"_"+z+"_"+type)) )
		return tell(me, "���Ӥ�V������᭱�èS���ж��C\n");

	if( belong_to_enterprise(owner) )
	{
		if( !ENTERPRISE_D->change_assets(query("enterprise", me), "-"+OPEN_DOOR_COST) )
			return tell(me, "���~����w�g�����}�P�q�D�F�C\n");
	}
	else if( !me->spend_money(money_unit, OPEN_DOOR_COST) )
		return tell(me, pnoun(2,me)+"���W�� $"+money_unit+" �������F�I�I\n");

	set("exits/"+arg, base_name(nextroom), env);
	set("exits/"+CORRESPONDING_DIRECTION[arg], file, nextroom);
		
	msg("$ME��O $"+money_unit+" 10,000 �N�s��"+query("short", env)+env->query_coor_short()+"�P"+query("short", nextroom)+nextroom->query_coor_short()+"���q�D���}�C\n", me, 0, 1);
	
	env->save();
	nextroom->save();
}

private void do_command(object me, string arg)
{
	if( !arg || !arg[0] ) 
		return tell(me, "�п��"+pnoun(2, me)+"�Q�n�ϥΪ��ؿv�������A�d�߫ؿv���C���J blist�C\n");

	arg = lower_case(arg);
	
	if( arg == "up" )
		open_up(me);
	else if( !undefinedp(CORRESPONDING_DIRECTION[arg]) )
		open_door(me, arg);
	else 
		open_building(me, arg);
}
