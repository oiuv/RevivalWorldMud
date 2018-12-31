/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : unlock.c
 * Author : 
 * Date   : 2003-05-03
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
�o���O�i���A�Ѱ�����C

unlock '��V'		- �Ѱ��Y�Ӥ�V������
(north, south, east, west, northeast, northwest, southeast, southwest, up, down)
HELP;

private void do_command(object me, string arg)
{
	int unlocktype;
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
		return tell(me, pnoun(2, me)+"�Q�n�ѭ��@�Ӥ�V����H\n");

	switch(option)
	{
		case "wiz":
			if( wizardp(me) )
				unlocktype = WIZLOCKED;
			else
				unlocktype = LOCKED;
			break;
		default:
			unlocktype = LOCKED;
	}
	
	if( !env->is_maproom() )
	{
	        if( !wizardp(me) && query("owner", env) != me->query_id(1) )
	                return tell(me, "�o�̨ä��O�A���ؿv���A�L�k����C\n");
	
	        if( !mapp(exits = query("exits", env)) )
	                return tell(me, "�o�̨èS������������A�n������O�H\n");
	
	        if( !exits[arg] )
	                return tell(me, "���Ӥ�k�èS������A�L�k����C\n");

	        if( arrayp(exits[arg]) )
	        {
	        	delete("lock/"+arg, env);
	        	CITY_D->delete_coor_data(exits[arg], "lock");
	        		
	        	env->save();
	        }
	        else
	        {
	                to = load_object(exits[arg]);
	                
	        	delete("lock/"+arg, env);
	                delete("lock/"+a[arg], to);

	                broadcast(to, "���G���H�� "+a[arg] +"��V�����ꥴ�}�F�C\n");
	                
	                env->save();
	                to->save();
	        }
	        
	        tell(me, pnoun(2, me)+"�⩹ "+capitalize(arg)+" ����V�����ꥴ�}�C\n");
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
				return tell(me, pnoun(2, me)+"�Q�n�ѭ��@�Ӥ�V����H\n");
		}

		if( !CITY_D->valid_coordinate(x, y, city, num) )
			return tell(me, "���Ӥ�V�O���X�z���y�СC\n");

		if( !wizardp(me) && CITY_D->query_coor_data(loc, "owner") != me->query_id(1) )
			return tell(me, "�o�̨ä��O�A���ؿv���A�L�k����C\n");
		
		if( CITY_D->query_coor_data(loc, TYPE) != DOOR )
			return tell(me, "���̨èS�����A�L�k����C\n");

        	CITY_D->delete_coor_data(loc, "lock");	        		
		tell(me, pnoun(2, me)+"�⩹ "+capitalize(arg)+" ����V�����ꥴ�}�C\n");
	}
}
