/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : reset_coor.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-16
 * Note   : reset_coor ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� reset_coor ���O�C
HELP;

private void command(object me, string arg)
{	
	array loc;
	object env = environment(me);
	
	if( !is_command() ) return;
	
	if( !env->is_maproom() )
		return tell(me, "�����b�a�Ϩt�ΤW�~������]�y�и�ơC\n", CMDMSG);
	
	loc = query_temp("location", me);

	switch( CITY_D->query_coor_data(loc, TYPE) )
	{
		case MOUNTAIN:
		case FOREST:
		case RIVER:
			return tell(me, "�o���a�O�۵M�a�ΡA�L�k���]�y�и�ơC\n", CMDMSG);
	}
		
	CITY_D->delete_coor_data(loc);
	CITY_D->delete_coor_icon(loc);
	ESTATE_D->remove_estate(loc);

	tell(me, "���]�y�и�Ƨ����C\n", CMDMSG);
}
