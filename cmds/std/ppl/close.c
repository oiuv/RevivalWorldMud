/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : close.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-04
 * Note   : �ؿv���������O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <map.h>
#include <location.h>
#include <building.h>

inherit COMMAND;

string help = @HELP
�����g�a�\��B�ؿv���B�Ω�W�h�ж������O�C

close		- �����z�ثe�Ҧb�a���ؿv���Τg�a�\��(�������b�ؿv�����f�θӤg�a�W)
close north	- �����_�䪺�ؿv���Τg�a�\��
close south	- �����n�䪺�ؿv���Τg�a�\��
close east	- �������䪺�ؿv���Τg�a�\��
close west	- �����F�䪺�ؿv���Τg�a�\��

close up	- ��W�h���ж�

HELP;

#define SELL_PERCENT	20

#define BUILDING_TYPE		2

private void confirm_close(object me, array loc, string owner, string enterprise, string arg)
{
	int percent;
	string value;
	mapping building_table;
	mapping estdata;
	string moneyunit = MONEY_D->city_to_money_unit(loc[CITY]);
	
	if( lower_case(arg) != "y" )
		tell(me, "���������ؿv���C\n");
	else 
	{
		building_table = BUILDING_D->query_building_table();
		estdata = ESTATE_D->query_loc_estate(loc);
		
		percent = SELL_PERCENT + me->query_skill_level("estaterebate")/2;

		if( estdata["roomtable"] )
		{
			value = ESTATE_D->query_estate_value(loc, 1);
			value = count(count(value, "*", percent), "/", 100);
		}

		ESTATE_D->unlink_estate(loc);
		
		if( belong_to_enterprise(owner) )
		{
			if( estdata["roomtable"] )
			{	
				CHANNEL_D->channel_broadcast("news", "���~���Ρu"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"�v�����F�b"+CITY_D->query_city_idname(loc[CITY], loc[NUM])+"���@��"+building_table[estdata["type"]][BUILDING_SHORT]+"�C\n");
				tell(me, pnoun(2, me)+"�H "+percent+"% �����^�������F�o�B�Цa���A���^ "HIY+money(moneyunit, value)+NOR" ��ú�^���~����C\n");
				ENTERPRISE_D->change_assets(enterprise, value);
			}
			else
				tell(me, pnoun(2, me)+"�����F���B���ؿv���δ��[�C\n");
		}
		else if( belong_to_individual(owner) )
		{
			if( estdata["roomtable"] )
			{
				CHANNEL_D->channel_broadcast("city", me->query_idname()+"�����F�b"+loc_short(loc)+NOR"����"+building_table[estdata["type"]][BUILDING_SHORT]+"�C\n", me);
				tell(me, pnoun(2, me)+"�H "+percent+"% �����^�������F�o�B�Цa���A���^ "HIY+money(moneyunit, value)+NOR" ���C\n");
				me->earn_money(moneyunit, value);
			}
			else
				tell(me, pnoun(2, me)+"�����F���B���ؿv���δ��[�C\n");
		}
		else
			tell(me, pnoun(2, me)+"�����F���B���ؿv���δ��[�C\n");
	}
	
	me->finish_input();
}

private void confirm_close_up(object me, object env, object up_room, string moneyunit, string value, string arg)
{
	object first_floor_room;
	object connect_room;
	object master = up_room->query_master();
	string exit, exit2;
	mixed direction, direction2;
	string up_room_basename = base_name(up_room);

	if( arg != "yes" && arg != "y" )
	{
		tell(me, "������u�@�C\n");
		return me->finish_input();
	}
	
	if( !objectp(first_floor_room = load_object(query("firstfloor", up_room))) )
		error("�Ӽh��Ƶo�Ϳ��~�C");
		
	addn("floor", -1, first_floor_room);
	first_floor_room->save();
	
	up_room->remove_master();
	master->remove_slave(up_room_basename);
	master->save();

	foreach(exit, direction in query("exits", up_room))
	{
		if( !stringp(direction) || !file_exists(direction) ) continue;
			
		connect_room = load_object(direction);
		
		if( mapp(query("exits", connect_room)) )
		foreach(exit2, direction2 in query("exits", connect_room))
		{
			if( direction2 == up_room_basename )
				delete("exits/"+exit2, connect_room);
				
			connect_room->save();
		}
	}
	
	destruct(up_room);
	rm(up_room_basename+__SAVE_EXTENSION__);

	me->earn_money(moneyunit, value);
	tell(me, pnoun(2, me)+"�N�W�h�ж�����F�A���^ "HIY+money(moneyunit, value)+NOR" ���C\n");
	me->finish_input();
}


private void do_command(object me, string arg)
{
	string owner, enterprise;
	array loc = copy(query_temp("location",me));
	mapping estdata;
	object env = environment(me);

	if( arg == "up" )
	{
		object up_room;
		string value; 
		string moneyunit = env->query_money_unit();
		int percent;

		if( query("owner", env) != me->query_id(1) )
			return tell(me, pnoun(2, me)+"���O�o�ɫؿv�����֦��̡C\n");
		
		if( !query("exits/up", env) )
			return tell(me, "�o�̼ӤW�S���ж��C\n");
		
		if( !objectp(up_room = load_object(query("exits/up", env))) )
			error("�L�k���J�W�h�ж��C");
			
		if( query("exits/up", up_room) )
			return tell(me, "�W�h���ж��ä��O���ӡA"+pnoun(2, me)+"�����q�̳��Ӥ@�h�@�h��U�ӡC\n");
		
		value = BUILDING_D->query_floor_value(up_room->query_floor());
		
		percent = SELL_PERCENT + me->query_skill_level("estaterebate")/2;

		value = count(count(value, "*", percent), "/", 100);
		
		tell(me, "��W�h�ж��i�^�� "+percent+"% �����B�@ "HIY+money(moneyunit, value)+NOR"�A�T�w��W���o�@�h�ж���(y/n)�H");
		
		input_to((: confirm_close_up, me, env, up_room, moneyunit, value :));
		
		return;
	}
	
        if( !arrayp(loc) || !env || !env->is_maproom() || !CITY_D->is_city_location(loc) )
	{
		if( query("owner", env) != me->query_id(1) )
			return tell(me, pnoun(2, me)+"�L�k�����o�ӫؿv���C\n");
		else
			return tell(me, pnoun(2, me)+"������ؿv�����f�i���������ʧ@�C\n");
	}
	
	if( arg == "north" )
		loc = arrange_city_location(loc[X], loc[Y]-1, loc[CITY], loc[NUM]);
	else if( arg == "south" )
		loc = arrange_city_location(loc[X], loc[Y]+1, loc[CITY], loc[NUM]);
	else if( arg == "west" )
		loc = arrange_city_location(loc[X]-1, loc[Y], loc[CITY], loc[NUM]);
	else if( arg == "east" )
		loc = arrange_city_location(loc[X]+1, loc[Y], loc[CITY], loc[NUM]);
	
	if( !CITY_D->valid_coordinate(loc[X], loc[Y], loc[CITY], loc[NUM]) )
		return tell(me, "���Ӥ�V���y�п��~�C\n");

	owner = CITY_D->query_coor_data(loc, "owner");

	if( !owner )
		return tell(me, "�o���g�a���ݩ�"+pnoun(2, me)+"�C\n");
	else if( belong_to_government(owner) )
	{
		if( !CITY_D->is_mayor_or_officer(loc[CITY], me) )
			return tell(me, "�o���a�O���F���겣�A"+pnoun(2, me)+"�S���v�Q�����o�̪��ؿv���δ��[�C\n");
	}
	else if( belong_to_enterprise(owner) )
	{
		enterprise = query("enterprise", me);
		
		if( enterprise != owner[11..] )
			return tell(me, pnoun(2, me)+"�ä��O"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"���~���Ϊ������A�S���v�Q�����o�̪��ؿv���δ��[�C\n");
	}
	else if( owner != me->query_id(1) )
		return tell(me, "�o���a�O "+capitalize(owner)+" ���p�H�g�a�A"+pnoun(2, me)+"�S���v�Q�����o�̪��ؿv���δ��[�C\n");

	estdata = ESTATE_D->query_loc_estate(loc);

	if( !mapp(estdata) )
		return tell(me, "�o���g�a�W�èS�ؿv���δ��[�i�H�����C\n");
		
	if( estdata["type"] == "land" )
		return tell(me, "��a���ݭn�����C\n");

	tell(me, HIR"�`�N�G"HIY"�ؿv��������A�N�|�M���Ҧ����e��ƻP�g�a���šC\n"NOR HIR"�нT�{�O�_���s��D���b�䤤�A�s�f��Ƭҷ|�Q�@�֧R���C\n"NOR"�T�w�O�_�������ؿv���H(y/n):");
	input_to( (: confirm_close, me, loc, owner, enterprise :) );
}
