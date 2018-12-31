/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : buildbridge.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-03
 * Note   : �سy����
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
    �y�����O�A�y���O�ΨC�q�� 2,000,000
buildbridge north	- ���_��سy����
buildbridge south	- ���n��سy����
buildbridge east	- ���F��سy����
buildbridge west	- �����سy����
buildbridge west -d	- ��b��誺����
HELP;

#define BUILDBRIDGE_COST	2000000

private void do_command(object me, string arg)
{
	string name;
	array loc;
	object env = environment(me);
	
	if( !env->is_maproom() )
		return tell(me, "�u���b�a�Ϥ��~����سy���١C\n");

	loc = query_temp("location", me);

	if( !CITY_D->is_mayor_or_officer(loc[CITY], me) )
		return tell(me, pnoun(2, me)+"���O�o�y�����������Ωx���C\n");

	if( !arg )
		return tell(me, "�п�J���T�����٫سy��V(north, south, west, east)�C\n");

	sscanf(arg, "%s %s", arg, name);
	
	switch(arg)
	{
		case "north":
			loc = arrange_city_location(loc[X], loc[Y]-1, loc[CITY], loc[NUM]);
			break;
		case "south":
			loc = arrange_city_location(loc[X], loc[Y]+1, loc[CITY], loc[NUM]);
			break;
		case "west":
			loc = arrange_city_location(loc[X]-1, loc[Y], loc[CITY], loc[NUM]);
			break;
		case "east":
			loc = arrange_city_location(loc[X]+1, loc[Y], loc[CITY], loc[NUM]);
			break;
		default:
			return tell(me, "�п�J���T�����٫سy��V(north, south, west, east)�C\n");
	}

	if( name == "-d" )
	{
		if( (MAP_D->query_map_system(loc))->query_coor_data(loc, TYPE) != BRIDGE )
			return tell(me, "���Ӥ�V�èS�����١C\n");

		(MAP_D->query_map_system(loc))->set_coor_data(loc, TYPE, RIVER); 
		(MAP_D->query_map_system(loc))->set_coor_icon(loc, BBLU HIB"��"NOR);
		(MAP_D->query_map_system(loc))->delete_coor_data(loc, "owner");
		msg("$ME����F�b "+arg+" ��V�����١C\n", me, 0, 1);
		return;
	}

	if( (MAP_D->query_map_system(loc))->query_coor_data(loc, TYPE) != RIVER )
		return tell(me, "���Ӥ�V�èS���e�y�C\n");

	if( name )
	{
		if( noansi_strlen(name) > 14 )
			return tell(me, "���٦W�ٳ̪�����W�L�C�Ӥ���r�C\n");

		name = kill_repeat_ansi(name+NOR);
		
		if( (MAP_D->query_map_system(loc))->query_coor_data(loc, TYPE) == BRIDGE && (MAP_D->query_map_system(loc))->query_coor_data(loc, "bridgename") == name )
			return tell(me, "�o���g�a�W�w�g�سy�F���١u"+arg+"�v�C\n");
		
		if( CITY_D->city_exist(loc[CITY]) )
		{
			CITY_D->change_assets(loc[CITY], -BUILDBRIDGE_COST);
			
			msg("$ME��O�F�����겣 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+NUMBER_D->number_symbol(BUILDBRIDGE_COST)+"�A�b "+arg+" ��V�}�P�F�@�y�s���١A�W�s�u"+name+"�v�C\n", me, 0, 1);
		
			CITY_D->set_coor_data(loc, "owner", "GOVERNMENT/"+loc[CITY]);
		}
		else
			msg("$ME�b "+arg+" ��V�}�P�F�@���s���١A�W�s�u"+name+"�v�C\n", me, 0, 1);
		(MAP_D->query_map_system(loc))->set_coor_data(loc, "bridgename", name);
	}
	else
	{
		if( CITY_D->query_coor_data(loc, TYPE) == BRIDGE && !CITY_D->query_coor_data(loc, "roadname") )
			return tell(me, "�o���g�a�W�w�g�سy�F���١C\n");
		
		if( CITY_D->city_exist(loc[CITY]) )
		{
			CITY_D->change_assets(loc[CITY], -BUILDBRIDGE_COST);
			
			msg("$ME��O�F�����겣 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+NUMBER_D->number_symbol(BUILDBRIDGE_COST)+"�A�b "+arg+" ��V�}�P�F�@�y�s���١C\n", me, 0, 1);
		
			CITY_D->set_coor_data(loc, "owner", "GOVERNMENT/"+loc[CITY]);
		}
		else
			msg("$ME�b "+arg+" ��V�}�P�F�@���s���١C\n", me, 0, 1);
	}

	(MAP_D->query_map_system(loc))->set_coor_data(loc, TYPE, BRIDGE); 
	(MAP_D->query_map_system(loc))->set_coor_icon(loc, BRED HIR"��"NOR);
	
}
