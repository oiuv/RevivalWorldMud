/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : buildroad.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-05
 * Note   : �سy�D��
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <location.h>

inherit COMMAND;

string help = @HELP
    �y�����O�A�y���O�ΨC�q�� 500,000
buildroad		- �b���a�سy�D��
buildroad -d		- ����a���D��
HELP;

#define BUILDROAD_COST	500000

private void do_command(object me, string arg)
{
	string owner;
	array loc;
	object env = environment(me);
	
	if( !env->is_maproom() )
		return tell(me, "�u���b�a�Ϥ��~����سy�D���C\n", CMDMSG);

	loc = query_temp("location", me);

	if( !CITY_D->is_mayor_or_officer(loc[CITY], me) )
		return tell(me, pnoun(2, me)+"���O�o�y�����������Ωx���C\n", CMDMSG);

	owner = (MAP_D->query_map_system(loc))->query_coor_data(loc, "owner");

	if( owner != "GOVERNMENT/"+loc[CITY] )
		return tell(me, "�o���g�a�ä��O�F���g�a�A�L�k�Ψӫس]�D���C\n", CMDMSG);

	switch( (MAP_D->query_map_system(loc))->query_coor_data(loc, TYPE) )
	{
		case 0:
		case ROAD:
			break;
		default:
			return tell(me, "�o�̤w�g����L�a�ΩάO�ؿv���A�L�k�b���سy�D���C\n", CMDMSG);
	}

	if( arg )
	{
		if( arg == "-d" )
		{
			(MAP_D->query_map_system(loc))->delete_coor_data(loc);
			(MAP_D->query_map_system(loc))->delete_coor_icon(loc);
			
			return tell(me, pnoun(2, me)+"����F���B���D���C\n", CMDMSG);
		}
		
		if( noansi_strlen(arg) > 14 )
			return tell(me, "���W�̪�����W�L�C�Ӥ���r�C\n", CMDMSG);

		arg = kill_repeat_ansi(arg+NOR);
		
		if( (MAP_D->query_map_system(loc))->query_coor_data(loc, TYPE) == ROAD && CITY_D->query_coor_data(loc, "roadname") == arg )
			return tell(me, "�o���g�a�W�w�g�سy�F�D���u"+arg+"�v�C\n", CMDMSG);
		
		if( CITY_D->city_exist(loc[CITY]) )
		{
			CITY_D->change_assets(loc[CITY], -BUILDROAD_COST);
		
			msg("$ME��O�F�����겣 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+NUMBER_D->number_symbol(BUILDROAD_COST)+"�A�b���}�P�F�@���s�D���A�W�s�u"+arg+"�v�C\n", me, 0, 1);
		}
		else
			msg("$ME�b���}�P�F�@���s�D���A�W�s�u"+arg+"�v�C\n", me, 0, 1);
			
		(MAP_D->query_map_system(loc))->set_coor_data(loc, "roadname", arg);
	}
	else
	{
		if( (MAP_D->query_map_system(loc))->query_coor_data(loc, TYPE) == ROAD && !(MAP_D->query_map_system(loc))->query_coor_data(loc, "roadname") )
			return tell(me, "�o���g�a�W�w�g�سy�F�D���C\n", CMDMSG);
		
		if( CITY_D->city_exist(loc[CITY]) )
		{
			CITY_D->change_assets(loc[CITY], -BUILDROAD_COST);
		
			msg("$ME��O�F�����겣 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+NUMBER_D->number_symbol(BUILDROAD_COST)+"�A�b���}�P�F�@���s�D���C\n", me, 0, 1);
		}
		else
			msg("$ME�b���}�P�F�@���s�D���C\n", me, 0, 1);
	}

	(MAP_D->query_map_system(loc))->set_coor_data(loc, TYPE, ROAD); 
	(MAP_D->query_map_system(loc))->set_coor_icon(loc, "�@");
	
}
