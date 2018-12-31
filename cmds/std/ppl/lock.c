/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : Lock.c
 * Author : 
 * Date   : 2003-05-02
 * Note   : ������O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <lock.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <location.h>

inherit COMMAND;

string help = @HELP
�o���O�i���A��W�ݩ�A�ؿv�������A���~�H�L�k�i�X�Q�A��W�����C

lock '��V'		- �N�Y�Ӥ�V������W
(north, south, east, west, northeast, northwest, southeast, southwest, up, down)
HELP;

private void do_command(object me, string arg)
{
	int locktype;
	string option;
        mapping exits;
        mapping a = ([
        "west"  : "east",
        "east"  : "west",
        "north" : "south",
        "south" : "north",
        "northeast" : "southwest",
        "northwest" : "southeast",
        "southeast" : "northwest",
        "southwest" : "northeast",
        "up"    : "down",
        "down"  : "up"
        ]);
        object env, to;

        env = environment(me);

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�N���@�Ӥ�V�����W��H\n");

	sscanf(arg, "%s %s", arg, option);

	switch(option)
	{
		case "wiz":
			if( wizardp(me) )
				locktype = WIZLOCKED;
			else
				locktype = LOCKED;
			break;
		default:
			locktype = LOCKED;
	}

	if( !env->is_maproom() )
	{
	        if( !wizardp(me) && query("owner", env) != me->query_id(1) )
	                return tell(me, "�o�̨ä��O�A���ؿv���A�L�k�W��C\n");

	        if( !mapp(exits = query("exits", env)) )
	                return tell(me, "�o�̨èS������X�f�A�n���W��O�H\n");
	
	        if( !exits[arg] )
	                return tell(me, "�o�̨èS���o�Ӥ�V�A�L�k���o��V�W��C\n");

	        if( arrayp(exits[arg]) )
	        {
	        	set("lock/"+arg, query("lock/"+arg, env) | locktype, env);
	        	CITY_D->set_coor_data(exits[arg], "lock", CITY_D->query_coor_data(exits[arg], "lock") | locktype);
	        } 
	        else 
	        {
	                to = load_object(exits[arg]);
	                set("lock/"+arg, query("lock/"+arg, env) | locktype, env);
	                set("lock/"+a[arg], query("lock/"+a[arg], to) | locktype, to);
	                broadcast(to, "���G���H�� "+a[arg] +"��V������F�_�ӡC\n");
	        }
	        
	        tell(me, pnoun(2, me)+"�⩹ "+capitalize(arg)+" ����V������F�_�ӡC\n");
	}
	else
	{
		int x, y, num;
		string city;
		array loc = query_temp("location", me);
		
		x = loc[X];
		y = loc[Y];
		city = loc[CITY];
		num = loc[NUM];

		if( !CITY_D->is_city_location(loc) )
			return tell(me, "�o����S�����i�H��"+pnoun(2, me)+"�W��C\n");

		switch(arg)
		{
			case "north":
				loc = arrange_city_location(x, --y, city, num);
				break;
			case "south":
				loc = arrange_city_location(x, ++y, city, num);
				break;
			case "east":
				loc = arrange_city_location(++x, y, city, num);
				break;
			case "west":
				loc = arrange_city_location(--x, y, city, num);
				break;
			case "northeast":
				loc = arrange_city_location(++x, --y, city, num);
				break;
			case "northwest":
				loc = arrange_city_location(--x, --y, city, num);
				break;
			case "southeast":
				loc = arrange_city_location(++x, ++y, city, num);
				break;
			case "southwest":
				loc = arrange_city_location(--x, ++y, city, num);
				break;
			default:
				return tell(me, pnoun(2, me)+"�Q�n�N���@�Ӥ�V�����W��H\n");
		}

		if( !CITY_D->valid_coordinate(x, y, city, num) )
			return tell(me, "���Ӥ�V�O���X�z���y�СC\n");

		if( !wizardp(me) && CITY_D->query_coor_data(loc, "owner") != me->query_id(1) )
			return tell(me, "�o�̨ä��O�A���ؿv���A�L�k�W��C\n");
		
		if( CITY_D->query_coor_data(loc, TYPE) != DOOR )
			return tell(me, "���̨èS�����A�L�k�W��C\n");
		
		if( ESTATE_D->query_loc_estate(loc)["type"] == "land" )
			return tell(me, "�u���@�����A�n��֡H\n");

		CITY_D->set_coor_data(loc, "lock", CITY_D->query_coor_data(loc, "lock") | locktype);
		tell(me, pnoun(2, me)+"�⩹ "+capitalize(arg)+" ����V������F�_�ӡC\n");
	}
}


