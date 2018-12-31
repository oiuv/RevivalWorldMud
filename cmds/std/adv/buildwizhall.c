/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : create_wizhall.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-02
 * Note   : �إ� Wizhall
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

#define WALL_TABLE	0
#define ROOM_TABLE	1

inherit COMMAND;

string help = @HELP
        �з� create_wizhall ���O�C
HELP;

private void command(object me, string arg)
{
	int x, y, num, type;
	string city;
	array room_coor, loc = query_temp("location",me);
	mapping bdata;
	
	if( !is_command() ) return;
	
	if( !arrayp(loc) ) return tell(me, pnoun(2, me)+"�����b�a�Ϩt�ΤW����o�ӫ��O�C\n");
	
	num 	= loc[NUM];
	city 	= loc[CITY];
	
	if( CITY_D->query_coor_data(loc, "owner") != "GOVERNMENT/"+city )
		return tell(me, "�o�̨ä��O���F�����g�a�C\n");

	if( !mapp(bdata = BUILDING_D->analyze_building_logic(loc)) )
		return tell(me, "���ɫؿv�����سy���c�����D�A�L�k�}�i�A�бN���~�������s�سy�C\n");


	if( sizeof(bdata["roomtable"]) != 6 )
		return tell(me, "�Ův���������O 3 X 2 ���ؿv�C\n");
		

	// ����y�и�ư}�C
	foreach( string code in bdata["walltable"] )
	{
		loc = restore_variable(code);
		
		type = CITY_D->query_coor_data(loc, TYPE);
		
		if( type == WALL )
			CITY_D->set_coor_data(loc, FLAGS, NO_MOVE);
			
		else if( type != DOOR )
			return tell(me, "(Command:open)�����ƿ��~�A�гq���Ův�B�z�C\n");
	}
	
	room_coor = allocate(6);
	
	// �ж��y�и�ư}�C
	foreach( string code in bdata["roomtable"] )
	{	
		loc = restore_variable(code);
		
		// �]�w���y�Ь� ROOM �аO
		CITY_D->set_coor_data(loc, ROOM, "wizhall");
		
		// �]�w���y�Ф��a�Ϲϥܬ�"  "
		CITY_D->set_coor_icon(loc, "  ");
		
		x += loc[X];
		if( !y || y < loc[Y] ) y = loc[Y];
	}	
	x/=6;

	// �]�w Wizhall �X�f
	CITY_D->create_wizhall_door(city, num, arrange_city_location(x, y+1, city, num));
	
	// �إߧŮv�����Цa����T
	ESTATE_D->set_estate("GOVERNMENT/"+city, bdata, "wizhall", city, num);
	
	CITY_D->set_coor_data(arrange_city_location(x, y, city, num), SPECIAL_EXIT, "/wiz/wizhall/room_wizhall_1");
	CITY_D->set_coor_data(arrange_city_location(x+1, y, city, num), SPECIAL_EXIT, "/wiz/wizhall/room_wizhall_2");
	CITY_D->set_coor_data(arrange_city_location(x-1, y, city, num), SPECIAL_EXIT, "/wiz/wizhall/room_wizhall_6");
	CITY_D->set_coor_data(arrange_city_location(x, y-1, city, num), SPECIAL_EXIT, "/wiz/wizhall/room_wizhall_4");
	CITY_D->set_coor_data(arrange_city_location(x+1, y-1, city, num), SPECIAL_EXIT, "/wiz/wizhall/room_wizhall_3");
	CITY_D->set_coor_data(arrange_city_location(x-1, y-1, city, num), SPECIAL_EXIT, "/wiz/wizhall/room_wizhall_5");
	CITY_D->set_coor_icon(arrange_city_location(x-1, y-1, city, num), HIW"��"NOR);
	CITY_D->set_coor_icon(arrange_city_location(x-1, y, city, num), NOR WHT"��"NOR);
	CITY_D->set_coor_icon(arrange_city_location(x+1, y-1, city, num), NOR HIW"�v"NOR);
	CITY_D->set_coor_icon(arrange_city_location(x+1, y, city, num), NOR WHT"��"NOR);
	CITY_D->set_section_info(city, num, "wizhall", arrange_city_location(x, y+1, city, num));

	CHANNEL_D->channel_broadcast("news", me->query_idname()+"�b����"+CITY_D->query_city_idname(city, num)+"��"+CITY_D->position(x, y)+"����سy�F�@�ɡu"HIW"�Ův"NOR WHT"����"NOR"�v�ؿv�C\n");
}
