/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : building_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-8-24
 * Note   : �ؿv�����F
 * Update :
 *  o 2002-09-26 Clode ���g�ؿv������޿��ˬd�禡 valid_building_logic()
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <daemon.h>
#include <location.h>
#include <roommodule.h>
#include <citydata.h>
#include <message.h>
#include <building.h>

#define WALL_TABLE	0
#define ROOM_TABLE	1

#define XX	0
#define YY	1

mapping building_table;
mapping floor_total_value;
mapping floor_value;

mapping default_building_table = ([

"land"	:	({ "��a"		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, 0, 1, 0, 1, 0, 1 }),
"farm"	: 	({ HIG"�A��"NOR		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION						, 0, 1, 0, 1, 0, 1 }),
"pasture":	({ NOR YEL"����"NOR	, 1, 0, INDIVIDUAL, AGRICULTURE_REGION						, 0, 1, 0, 1, 0, 1 }),
"fishfarm":	({ HIB"�i�޳�"NOR	, 1, 0, INDIVIDUAL, AGRICULTURE_REGION						, 0, 1, 0, 1, 0, 1 }),
"light"	:	({ HIY"���O"NOR 	, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, 0, 1, 0, 1, 0, 1 }),
"pool"	:	({ HIC"����"NOR		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, 0, 1, 0, 1, 0, 1 }),
"pavilion":	({ HIG"�D�F"NOR		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, 0, 1, 0, 1, 0, 1 }),
"tree":		({ HIG"�j��"NOR		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, 0, 1, 0, 1, 0, 1 }),
"grass":	({ HIG"���"NOR		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, 0, 1, 0, 1, 0, 1 }),
"statue":	({ HIM"�J��"NOR		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, 0, 1, 0, 1, 0, 1 }),
"fence":	({ HIM"����"NOR		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, "0", 0, 0, 0, 0, 1 }),
"wizhall":	({ HIW"�Ův�j�U"NOR	, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, 0, 1, 0, 1, 0, 1 }),
]);

varargs mapping analyze_building_logic(array loc, int no_owner_check);

int has_building(string type)
{
	return arrayp(building_table[type]);
}

mapping query_building_table()
{
	return copy(building_table);
}

mapping query_building(string building)
{
	return building_table[building];
}	
	
mapping exits_dir(array loc)
{
	string city = loc[CITY];
	int num = loc[NUM];
	int x = loc[X];
	int y = loc[Y];

	return ([ 
		arrange_city_location(x-1, y, city, num) : "west",
		arrange_city_location(x+1, y, city, num) : "east",
		arrange_city_location(x, y-1, city, num) : "north",
		arrange_city_location(x, y+1, city, num) : "south",
	]);
}


// ����ƫ����ؿv
void materialize_city_building(object me, string owner, mapping table, string city, int num, string btype)
{
	array loc, temploc;
	string file, sloc;
	mapping exits;
	object room;

	// �B�z����y�и��
	foreach( sloc in table["walltable"] )
	{
		loc = restore_variable(sloc);
		
		switch((MAP_D->query_map_system(loc))->query_coor_data(loc, TYPE))
		{
			case WALL:
				(MAP_D->query_map_system(loc))->set_coor_data(loc, FLAGS, NO_MOVE);
				break;
			case DOOR:
				break;
				
			default:
				error("�����Ƶ{�����~�A�гq���Ův�B�z�C\n");
		}
	}
	
	// �B�z�ж��y�и��
	foreach( sloc in table["roomtable"] )
	{
		loc = restore_variable(sloc);
		
		// �]�w���y�Ь� ROOM �аO
		(MAP_D->query_map_system(loc))->set_coor_data(loc, ROOM, btype);
		
		// �]�w���y�Ф��a�Ϲϥܬ�"  "
		(MAP_D->query_map_system(loc))->set_coor_icon(loc, "  ");
		
		file = CITY_ROOM_MODULE(city, num, loc[X], loc[Y], btype);
		
		// �R���쥻�b���a���ж�����
		if( find_object(file) ) destruct(file);
		// �R���쥻�b���a���ж����
		if( file_exist(file+".o") ) rm(file+".o");
			
		// ���J�����ж�����
		room = load_object(file);
		
		exits = allocate_mapping(1);
		
		// �إ߸��f�s��
		foreach( temploc, string dir in exits_dir(loc) )
		{	
			if( temploc[X] < 0 || temploc[X] > 99 || temploc[Y] < 0 || temploc[Y] > 99 ) continue;
			
			if( member_array( save_variable(temploc), table["roomtable"] ) != -1 ) 
				exits[dir] = CITY_ROOM_MODULE(city, num, temploc[X], temploc[Y], btype);
			else if( (MAP_D->query_map_system(loc))->query_coor_data(temploc, TYPE) == DOOR ) 
				exits[dir] = copy(temploc);
		}
		
		set("exits", exits, room);
		set("owner", owner, room);
		room->save();
	}

	if( find_object(MAP_D->query_map_system(loc)) == find_object(CITY_D) )
		ESTATE_D->set_estate(owner, table, btype, city, num);

	(MAP_D->query_map_system(loc))->translate_map_to_html(city, num);
	

}

// ����ưϰ�ؿv, �Ψӫإ߭��ϰϰ쪺�W�߫ؿv
int materialize_area_building(array loc)
{
	string sloc;
	mapping table = analyze_building_logic(loc, 1);
	
	if( !mapp(table) ) return 0;

	// �B�z����y�и��
	foreach( sloc in table["walltable"] )
	{
		loc = restore_variable(sloc);
		
		switch((MAP_D->query_map_system(loc))->query_coor_data(loc, TYPE))
		{
			case WALL:
				(MAP_D->query_map_system(loc))->set_coor_data(loc, FLAGS, NO_MOVE);
				break;
			case DOOR:
				break;
				
			default:
				error("�����Ƶ{�����~�A�гq���Ův�B�z�C\n");
		}
	}
	// �B�z�ж��y�и��
	foreach( sloc in table["roomtable"] )
	{
		loc = restore_variable(sloc);
		
		// �]�w���y�Ь� ROOM �аO
		(MAP_D->query_map_system(loc))->set_coor_data(loc, ROOM, "room");
		
		// �]�w���y�Ф��a�Ϲϥܬ�"  "
		(MAP_D->query_map_system(loc))->set_coor_icon(loc, "  ");
	}
	
	return 1;
}

// ����Ƥ�~�ؿv(�L�Ǥ����ؿv����)
void materialize_outdoor_building(object me, string owner, array loc, string building_icon, string building_name, int type)
{
	mapping table = allocate_mapping(0);
	
	CITY_D->set_coor_data(loc, TYPE, type);
	CITY_D->set_coor_icon(loc, building_icon);
	
	// �R�� material ��T
	CITY_D->delete_coor_data(loc, "material");
	
	table["walltable"] = ({ save_variable(loc) });
	table["roomtable"] = allocate(0);
	
	if( type != WALL && type != DOOR )
	{
		CHANNEL_D->channel_broadcast("city", me->query_idname()+"�b"+loc_short(loc)+NOR"�سy�F�u"+building_name+" "+building_icon+"�v", me);
		
		foreach(string building_english_name, array building_info in default_building_table)
		{
			if( remove_ansi(building_info[BUILDING_SHORT]) == building_name )
			{
				ESTATE_D->set_estate(owner, table, building_english_name, loc[CITY], loc[NUM]);
				break;
			}
		}
	}
	
	switch(type)
	{
		case FARM:
		case PASTURE:
		case FISHFARM:
			GROWTH_D->add_growth(loc);
			break;
	}
}


// ����Ƴ���ؿv
/*
void materialize_fence(object me, string owner, mapping table, string city, int num, string btype)
{
	array loc;
	string sloc;

	// �B�z����y�и��
	foreach( sloc in table["walltable"] )
	{
		loc = restore_variable(sloc);

		switch(CITY_D->query_coor_data(loc, TYPE))
		{
			case WALL:
				CITY_D->set_coor_data(loc, FLAGS, NO_MOVE);
				break;
			case DOOR:
				break;
				
			default:
				error("�����Ƶ{�����~�A�гq���Ův�B�z�C\n");
		}
	}

	table["roomtable"] = allocate(0);

	ESTATE_D->set_estate(owner, table, btype, city, num);
}
*/


// �B�z�ؿv���޿賡��
// --------------------------------------------------------------------------------------------------
//  1
//2 + 4
//  8
#define N	(1<<0)	//NORTH
#define W	(1<<1)	//WEST
#define E	(1<<2)	//EAST
#define S	(1<<3)	//SOUTH

#define T 	8

#define MAX	0
#define MIN	1
#define MAX_BUILDING_RANGE      500
#define MAX_ROOM_TABLE		25
mapping picdir =
([
"��":N|S,
"��":N|S,
"�x":N|S,

"��":N|W,
"��":N|W,
"��":N|W,
"�}":N|W,
"��":N|W,

"��":W|S,
"��":W|S,
"��":W|S,
"��":W|S,
"�{":W|S,

"��":N|E,
"��":N|E,
"��":N|E,
"�|":N|E,
"��":N|E,

"�~":S|E,
"��":S|E,
"��":S|E,
"�z":S|E,
"��":S|E,

"��":W|E,
"��":W|E,
"�w":W|E,
]);

string coorname(int x, int y)
{
	return "("+(x+1)+","+(y+1)+")";
}

// �ˬd�ؿv���ϧ��޿�
varargs mapping analyze_building_logic(array loc, int no_owner_check)
{
	int i, x, y, lx, ly, *ry, entry, exit, lentry, lexit, dir, ndir, evenwall, evenwallbit, num;

	object me = this_player();
	array map = MAP_D->query_map(loc);
	
	string owner, tempowner, city;
	string *inverse_roomtable = allocate(0);
	
	mapping boundary = allocate_mapping(0);
	mapping table = allocate_mapping(0);

	table["walltable"] = allocate(0);
	table["roomtable"] = allocate(0);

	city = loc[CITY];
	num = loc[NUM];
	x = loc[X];
	y = loc[Y];
	
	// �ˬd����s��P�L����餧���p
	while(++i)
	{

		if( i>MAX_BUILDING_RANGE )
		{
			tell(me, "�H�{�b���޳N�L�k�سy�o��j���ؿv���C\n", CMDMSG);
			return 0;
		}
	
		// �ëD����Ϲ�
		if( undefinedp(map[y]) || undefinedp(map[y][x]) || undefinedp(dir = picdir[remove_ansi(map[y][x])]) )
		{
			tell(me, "��������n���㤣�঳�}�H�_�����a���I\n", CMDMSG);
			return 0;
		}

		// �Ω�Ĥ@������W, �H����X�@�ӥثe���Ϲ��X�f
		if( undefinedp(lexit) ) 
		{
			for(lexit=1;lexit &~ dir;lexit*=2); 
			lexit = T / lexit;
		}
		
		// ��������J�f��V�S������W��������X�f��V
		if( T / lexit &~ dir )
		{
			tell(me, "�ؿv�������y��"+coorname(x,y)+"�P�y��"+coorname(lx,ly)+"����y����ä��s��I\n", CMDMSG);
			return 0;
		}

		entry = T / lexit;
		exit = dir ^ entry;
		
		if( !undefinedp(lentry) && lentry == exit )
		{
			tell(me, "�ؿv�������y��"+coorname(x,y)+"�P�y��"+coorname(lx,ly)+"����y������L�סI\n", CMDMSG);
			return 0;
		}
		
		// ����s����ˬd����
		if( i!=1 && x == loc[X] && y == loc[Y] ) break;
		
		// �إ߫ؿv���d���T
		if( undefinedp(boundary[y]) ) boundary[y] = allocate(2, x);
		if( x < boundary[y][MIN] ) boundary[y][MIN] = x;
		if( x > boundary[y][MAX] ) boundary[y][MAX] = x;
		
		table["walltable"] += ({ save_variable( arrange_city_location(x, y, city, num) ) });
		
		lx = x;
		ly = y;
		lexit = exit;
		lentry = entry;
			
		switch(exit)
		{
			case N: y--;break;
			case S: y++;break;
			case W: x--;break;
			case E:	x++;break;
		}
		
	}
	
	ry = sort_array(keys(boundary),1);
	
	// �q��ӫؿv���d�򱽴y�ˬd�Ӹ`��������|���p�P�������A
	for(y=ry[0];y<=ry[<1];evenwall=evenwallbit=0, y++)
	for(x=boundary[y][MIN];x<=boundary[y][MAX];x++)
	{
		owner = CITY_D->query_coor_data(arrange_location(x,y,loc[Z],loc[PLACE],loc[NUM],loc[EXTRA]), "owner");
		
		if( !tempowner )
			tempowner = owner;
		
		if( !no_owner_check && owner != tempowner )
		{
			tell(me, "�ؿv�������y��"+coorname(x,y)+"���g�a�֦��H�P���a���P�C\n", CMDMSG);
			return 0;
		}
		
		if( picdir[remove_ansi(map[y][x])] & (N|S) )
		{
			evenwall++;
			evenwallbit |= picdir[remove_ansi(map[y][x])];
		}

		// �o�{������Ϲ�, �}�l�@���|�ˬd
		if( !undefinedp(dir = picdir[remove_ansi(map[y][x])]) )
		{
			// �Y������ëD���e���ˬd���s��d��. �h�Q�λ��j�A�ˬd�������ؿv�޿�
			// �Ѧ��i���\�s�@�h�h���ؿv��
			if( member_array(save_variable( arrange_city_location(x, y, city, num) ), table["walltable"] ) == -1 )
			{
				mapping inside_building_table;
				array inside_loc = arrange_location(x, y, loc[Z], loc[PLACE], loc[NUM], loc[EXTRA]);
				
				inside_building_table = analyze_building_logic(inside_loc);
				
				if( !arrayp(inside_building_table) )
				{
					tell(me, "�h�h���ؿv�������ؿv���޿���~�C\n", CMDMSG);
					return 0;
				}
				
				table["walltable"] += inside_building_table["walltable"];
				// �Ϭۤ� roomtable
				inverse_roomtable += inside_building_table["roomtable"];
			}

			// Y �V������|�ˬd
			if( y < ry[<1] )
			{
				ndir = picdir[remove_ansi(map[y+1][x])];
				if( ndir && S &~ dir && N &~ ndir )
				{
					tell(me, "�ؿv�������y��"+coorname(x,y)+"�P�y��"+coorname(x, y+1)+"����������o���|�b�@�_�C\n", CMDMSG);
					return 0;
				}
			}
			
			// X �V������|�ˬd
			if( x < boundary[y][MAX] )
			{
				ndir = picdir[remove_ansi(map[y][x+1])];
				if( ndir && E &~ dir && W &~ ndir )
				{
					tell(me, "�ؿv�������y��"+coorname(x,y)+"�P�y��"+coorname(x+1, y)+"����������o���|�b�@�_�C\n", CMDMSG);
					return 0;
				}
			}

			// �צV������|�ˬd, �קK�צV�ʳ��Ŷ�
			if( y < ry[<1] && x < boundary[y][MAX] && (dir == (N|W)) && (picdir[remove_ansi(map[y+1][x+1])] == (S|E)) )
			{
				tell(me, "�ؿv�������y��"+coorname(x,y)+"�P�y��"+coorname(x+1, y+1)+"����������o���|�b�@�_�C\n", CMDMSG);
				return 0;
			}
			
			// �צV������|�ˬd, �קK�צV�ʳ��Ŷ�
			if( y < ry[<1] && x > boundary[y][MIN] && (dir == (N|E)) && (picdir[remove_ansi(map[y+1][x-1])] == (S|W)) )
			{
				tell(me, "�ؿv�������y��"+coorname(x,y)+"�P�y��"+coorname(x-1, y+1)+"����������o���|�b�@�_�C\n", CMDMSG);
				return 0;
			}
		}
		else if( evenwall % 2 || evenwallbit & (N|S) == (N|S)) 
			table["roomtable"] += ({ save_variable( arrange_city_location(x, y, city, num) ) });
	}
	
	table["roomtable"] -= inverse_roomtable;

	if( sizeof(table["roomtable"]) > MAX_ROOM_TABLE )
	{
		tell(me, "�ثe�ؿv�����ж����a�ƶq���o�W�L "+MAX_ROOM_TABLE+"�C\n");
		return 0;
	}
	return table;
}


varargs mixed query_floor_total_value(int floor)
{
	if( undefinedp(floor) )
		return floor_total_value;
	
	return floor_total_value[floor];
}
	
varargs mixed query_floor_value(int floor)
{
	if( undefinedp(floor) )
		return floor_value;
	
	return floor_value[floor];
}

// --------------------------------------------------------------------------------------------------


void create()
{
	int i;
	object module;
	building_table = allocate_mapping(0);
	floor_total_value = allocate_mapping(0);
	floor_value = allocate_mapping(0);

	// ��l�ƩҦ��Ҳո�T
	foreach( string file in get_dir(ROOM_MODULE_PATH) )
	{
		if( file[<2..<1] == ".c" && !catch(module = load_object(ROOM_MODULE_PATH+file)) )
			building_table[file[0..<3]] = module->query_building_info();
	}
	
	building_table += copy(default_building_table);

	for(i=1;i<=5;i++)
		floor_value[i] = count(100000, "*", i);
	for(i=6;i<=100;i++)
		floor_value[i] = count(count(100000, "*", i), "*", i-4);
	for(i=101;i<=120;i++)
		floor_value[i] = count(count(300000, "*", i), "*", i-4);
	for(i=121;i<=140;i++)
		floor_value[i] = count(count(500000, "*", i), "*", i-4);
	for(i=141;i<=160;i++)
		floor_value[i] = count(count(700000, "*", i), "*", i-4);
	for(i=161;i<=180;i++)
		floor_value[i] = count(count(900000, "*", i), "*", i-4);
	for(i=181;i<=200;i++)
		floor_value[i] = count(count(1100000, "*", i), "*", i-4);
	for(i=201;i<=1000;i++)
		floor_value[i] = count(count(5000000, "*", i), "*", i-4);

	for(i=1;i<=1000;i++)
		floor_total_value[i] = count(floor_total_value[i-1], "+", floor_value[i]);
}
	
string query_name()
{
	return "�ؿv�t��(BUILDING_D)";
}

int is_loc_closed(array loc)
{
	mixed type = CITY_D->query_coor_data(loc, TYPE);
	
	if( type == DOOR || type == WALL || CITY_D->query_coor_data(loc, ROOM) )
		return 1;
		
	return 0;
}

// �ˬd�ؿv���O�_�����Q����
varargs int is_building_locked(array loc, mapping estdata)
{
	int x, y, num;
	string city;
	int locked;
	string sloc;
	
	if( !arrayp(loc) ) return 0;
	
	if( undefinedp(estdata) )
		estdata = ESTATE_D->query_loc_estate(loc);
	
	if( !mapp(estdata) ) return 0;
		
	locked = 1;
	
	foreach(sloc in estdata["walltable"])
	{
		loc = restore_variable(sloc);	
		
		if( CITY_D->query_coor_data(loc, TYPE) == WALL || CITY_D->query_coor_data(loc, "lock") )
			continue;

		x = loc[X];
		y = loc[Y];
		city = loc[CITY];
		num = loc[NUM];
		
		if( x+1 < MAP_WIDTH && !is_loc_closed(arrange_city_location(x+1, y, city, num)) )
		{
			locked = 0;
			break;
		}
		
		if( x-1 >= 0 && !is_loc_closed(arrange_city_location(x-1, y, city, num)) )
		{
			locked = 0;
			break;
		}
			
		if( y+1 < MAP_LENGTH && !is_loc_closed(arrange_city_location(x, y+1, city, num)) )
		{
			locked = 0;
			break;
		}
		
		if( y-1 >= 0 && !is_loc_closed(arrange_city_location(x, y-1, city, num)) )
		{
			locked = 0;
			break;
		}
	}
	
	return locked;
}

// �ˬd�Y�өж��������ؿv���O�_�����Q����
int is_building_room_locked(object room)
{
	return is_building_locked(room->query_location());
}
