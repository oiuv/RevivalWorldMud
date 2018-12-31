/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sell.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-8-22
 * Note   : sell ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <location.h>
#include <roommodule.h>

inherit COMMAND;

string help = @HELP
�c��g�a�����O

sell here		- �N�ثe�Ҧb���o���g�a��X

HELP;

#define SELL_PERCENT	20

#define BUILDING_WALL_TABLE	4
#define BUILDING_ROOM_TABLE	5

void sell_land(object me, string owner, array loc, mapping data, string moneyunit, string value, string enterprise, string arg)
{
	int percent;
	mapping building_table = BUILDING_D->query_building_table();
	mapping estdata = ESTATE_D->query_loc_estate(loc);
	
	if( arg != "y" )
	{
		tell(me, pnoun(2, me)+"�M�w����X���a�C\n");
		return me->finish_input();
	}
	
        if(!estdata) return tell(me, "�g�a��ƿ��~�A�гq���Ův�B�z�C\n");

	value = ESTATE_D->query_estate_value(loc);
	
	percent = SELL_PERCENT + me->query_skill_level("estaterebate")/2;

	value = count(count(value, "*", percent), "/", 100);

	if( belong_to_enterprise(owner) )
	{
		ENTERPRISE_D->change_assets(enterprise, value);
		
		if( sizeof(estdata["roomtable"]) )
			CHANNEL_D->channel_broadcast("news", "���~���Ρu"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"�v�����ð�X�b"+CITY_D->query_city_idname(loc[CITY], loc[NUM])+"���@��"+building_table[estdata["type"]][ROOMMODULE_SHORT]+"�C\n");
		
		tell(me, pnoun(2, me)+"�H��y�Цa���� "+percent+"% �����汼�F�o�B�Цa���A���^ "HIY+money(moneyunit, value)+NOR" ��ú�^���~����C\n");
	}
	else
	{
		me->earn_money(moneyunit, value);
		
		if( sizeof(estdata["roomtable"]) )
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"�����ð�X�b"+CITY_D->query_city_idname(loc[CITY], loc[NUM])+"���@��"+building_table[estdata["type"]][ROOMMODULE_SHORT]+"�C\n", me);
		
		tell(me, pnoun(2, me)+"�H��y�Цa���� "+percent+"% �����汼�F�o�B�Цa���A���^ "HIY+money(moneyunit, value)+NOR" ���C\n");
	}
	
	ESTATE_D->remove_estate(loc);
	me->finish_input();
}

private void do_command(object me, string arg)
{
	int percent;
	mapping data;
	string value, moneyunit, enterprise;
	array loc = query_temp("location", me);
	object env = environment(me);
	
	if( !arg ) return tell(me, pnoun(2, me)+"�Q�椰��H\n");

	if( arg == "here" )
	{
		if( !arrayp(loc) || !env || !env->is_maproom() || !env->query_city() ) 
			return tell(me, "�o���g�a�L�k�R��C\n");
	
		if( !mapp(data = CITY_D->query_coor_data(loc)) )
			return tell(me, "�g�a��ƿ��~�A�гq���Ův�B�z�C\n");
		
		moneyunit = MONEY_D->city_to_money_unit(loc[CITY]);

		if( belong_to_enterprise(data["owner"]) )
		{
			enterprise = query("enterprise", me);
			
			if( enterprise != data["owner"][11..] )
				return tell(me, pnoun(2, me)+"���O"+ENTERPRISE_D->query_enterprise_color_id(data["owner"][11..])+"���~���Ϊ������A�L�v�c�榹���g�a�C\n");
			
			moneyunit = MONEY_D->query_default_money_unit();
		}
		else if( data["owner"] != me->query_id(1) )
			return tell(me, "�o���a�ä��O"+pnoun(2, me)+"���C\n");
		
		if( data["claimed"] && ( !me->query_highest_skill() || me->query_skill_level(me->query_highest_skill()) < 10) )
			return tell(me, "�o���g�a�O"+pnoun(2, me)+"�{�⪺�A"+pnoun(2, me)+"���������@�ޯ�W�L 10 �Ť~���X�����g�a�C\n");

		value = ESTATE_D->query_estate_value(loc);

		percent = SELL_PERCENT + me->query_skill_level("estaterebate")/2;

		tell(me, "�o�B�Цa������ "HIY+money(moneyunit, value)+NOR"�A�Y���ؿv���b�W���N�|�H "+percent+"% ���������X�A"+pnoun(2, me)+"�T�w�n��X���a�H(y/n)");
		input_to( (: sell_land, me, data["owner"], loc, data, moneyunit, value, enterprise :) );
	}
	else
		return tell(me, pnoun(2, me)+"�Q�n�椰��H\n");
}
