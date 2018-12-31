/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : buildterrain.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-22
 * Note   : �سy�a��
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
#include <terrain.h>

inherit COMMAND;

string help = @HELP
        �з� buildterrain ���O�C
HELP;

#define BUILDROAD_COST	10000

private void do_command(object me, string arg)
{
	string owner, arg2;
	array loc;
	object env = environment(me);
	
	if( !env->is_maproom() )
		return tell(me, "�u���b�a�Ϥ��~����سy�a�ΡC\n", CMDMSG);

	loc = query_temp("location", me);
	
	return tell(me, "�ثe�Ȥ��}��g�a�a�ξ�a�\��C\n", CMDMSG);

	if( !CITY_D->is_mayor_or_officer(loc[CITY], me) )
		return tell(me, pnoun(2, me)+"���O�o�y�����������Ωx���C\n", CMDMSG);

	owner = (MAP_D->query_map_system(loc))->query_coor_data(loc, "owner");

	return tell(me, "�ثe���}��سy�a�ΡC\n", CMDMSG);

	if( owner != "GOVERNMENT/"+loc[CITY] )
		return tell(me, "�o���g�a�ä��O�F���g�a�A�L�k�Ψӫسy�a�ΡC\n", CMDMSG);

	switch( (MAP_D->query_map_system(loc))->query_coor_data(loc, TYPE) )
	{
		case DOOR:
		case WALL:
			return tell(me, "�o�̤w�g����L�a�ΩάO�ؿv���A�L�k�b���سy�a�ΡC\n", CMDMSG);
	}

	sscanf(arg, "%s %s", arg, arg2);
	
	switch(arg)
	{
		case "forest":
			(MAP_D->query_map_system(loc))->delete_coor_data(loc);
			(MAP_D->query_map_system(loc))->set_coor_data(loc, TYPE, FOREST);
			(MAP_D->query_map_system(loc))->set_coor_icon(loc, HIG"��"NOR);
			 msg("$ME�b���}�P�F�˪L�a�Ρu"HIG"��"NOR"�v�C\n", me, 0, 1);
			 break;

		case "mountain":
			if( arg2 == "right" )
			{
				(MAP_D->query_map_system(loc))->set_coor_icon(loc, YEL"��"NOR);
				msg("$ME�b���}�P�F�s�ߦa�Ρu"YEL"��"NOR"�v�C\n", me, 0, 1);
			}
			else if( arg2 == "left" )
			{
				(MAP_D->query_map_system(loc))->set_coor_icon(loc, YEL"��"NOR);
				msg("$ME�b���}�P�F�s�ߦa�Ρu"YEL"��"NOR"�v�C\n", me, 0, 1);
			}
			else
				return tell(me, "�п�J���}�P���s�ߤ�V(right, left)�C\n", CMDMSG);
				
			(MAP_D->query_map_system(loc))->delete_coor_data(loc);
			(MAP_D->query_map_system(loc))->set_coor_data(loc, TYPE, MOUNTAIN);
			break;

		case "river":
			switch(arg2)
			{
				case "north":
					(MAP_D->query_map_system(loc))->delete_coor_data(loc);
					(MAP_D->query_map_system(loc))->set_coor_data(loc, FLAGS, FLOW_NORTH);
					msg("$ME�b���}�P�F�e�y�a�Ρu"BBLU HIB"��"NOR"�v�A�y�V���_�C\n", me, 0, 1);
					break;
				case "south":
					(MAP_D->query_map_system(loc))->delete_coor_data(loc);
					(MAP_D->query_map_system(loc))->set_coor_data(loc, FLAGS, FLOW_SOUTH);
					msg("$ME�b���}�P�F�e�y�a�Ρu"BBLU HIB"��"NOR"�v�A�y�V���n�C\n", me, 0, 1);
					break;
				case "east":
					(MAP_D->query_map_system(loc))->delete_coor_data(loc);
					(MAP_D->query_map_system(loc))->set_coor_data(loc, FLAGS, FLOW_EAST);
					msg("$ME�b���}�P�F�e�y�a�Ρu"BBLU HIB"��"NOR"�v�A�y�V���F�C\n", me, 0, 1);
					break;
				case "west":
					(MAP_D->query_map_system(loc))->delete_coor_data(loc);
					(MAP_D->query_map_system(loc))->set_coor_data(loc, FLAGS, FLOW_WEST);
					msg("$ME�b���}�P�F�e�y�a�Ρu"BBLU HIB"��"NOR"�v�A�y�V����C\n", me, 0, 1);
					break;
				default:
					return tell(me, "�п�J���}�P�e�y���y�V(north, south, east, west)�C\n", CMDMSG);
			}
			
			(MAP_D->query_map_system(loc))->set_coor_data(loc, TYPE, RIVER);
			(MAP_D->query_map_system(loc))->set_coor_icon(loc, BBLU HIB"��"NOR);
			break;
		default:
			return tell(me, pnoun(2, me)+"�Q�n�سy��ئa�ΡH(forest, mountain, river)�C\n", CMDMSG);
	}
}
