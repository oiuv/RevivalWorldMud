/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zone_d.c
 * Author : Cookys@RevivalWorld
 * Date   : 2002-09-17
 * Note   : 
 * Update :
 *  o 2002-09-29 cookys ���s��g�H�ŦX�s�� map_d.c
 *
 -----------------------------------------
 */
 
/*           (place?)
** zone �w�w NUM �w zonedbase
**           �u�w
**           �u�w
**           �u�w NUM �w�s�w Z �w ZONE_ROOM
**                      �u�w Z
**                      �|�w Z �w�s�w Y �w�s X
**                               �u�w     �|
**
**
** zone[MAP]
** zone[RMAP][place][num][z][y][x]
**
** mapping database = allocate_mapping( ({ save_variable(({x,y,z})) }), ({ "�S��X�f" }) );
== mapping database = ([ save_variable(({x,y,z})) : "�S��X�f" ]);
*/

/*
 *          �n�Q�� MAP_D �a�Ϩt�Τ�����, �����w�q�H�U�禡
 *          o string show_map(array loc, int cover_range)      // ��ܦa��
 *          o object query_maproom(array loc)                  // �^�Ǧa�Ϫ���
 *          o object *query_all_maproom()                      // �^�ǩҦ��a�Ϫ���
 *          o int move(object ob, string dir)                  // �ϥΪ̲���
 *          o varargs array query_map(array loc, int realtime) // �^�Ǧa�ϰ}�C
 *          o void remove_player_icon(object me, array loc)    // �Y�Q���H�ιϹ��г]�p���禡
 *          o void set_player_icon(object me, array loc)       // �Y�Q���H�ιϹ��г]�p���禡
 *          �åB�����b map_system_table ���]�w�y�����ݧP�_�禡
*/
#include <daemon.h>
#include <location.h>
#include <ansi.h>
#include <gender.h>
#include <message.h>
#include <nature.h>
#include <nature_data.h>

#define MAP_WIDTH       100     //�����e
#define MAP_LENGTH      100     //������

/* �i���Ͻd��, ���P�e�ҥ������_��, �H�~���n�b�a�ϥ����� */
/* �s�W�B�׽d��]�w, �{�b�� 25:9 ��ҭ�n, �Y�L�ݭn���קK���, �_�h���B�׭p��|�����D */
#define VISION_WIDTH    25      //�i���a�Ϥj�p-�e
#define VISION_LENGTH   9       //�i���a�Ϥj�p-��


#define ZONE_GENERATOR_PATH "/zone/generator/"

#define MAP             "zonemap"
#define RMAP            "realtime_zonemap"
#define DATA            "data"

// ZONE_D ���ΨC�@�����s data
// �P�@�h(�H y �����)��@��
#define ZONE_ROOM       "zone_room"
#define SPECIAL_EXIT	"special_exit"

private mixed zone=([
                        RMAP:([]),
                        MAP:([]),
                        DATA:([]),
                   ]);
private mapping module_table=([]);                   



int is_zone_exist(string place,int num,int z,int y,int x)
{
	//return 1;
	if( !undefinedp(zone[MAP][place]) )
        {
        	if( num < sizeof(zone[MAP][place]) && !undefinedp(zone[MAP][place][num]) )
        	{
        		if( z < sizeof(zone[MAP][place][num]) && !undefinedp(zone[MAP][place][num][z]))
        		{
        			if(y<sizeof(zone[MAP][place][num][z]) && !undefinedp(zone[MAP][place][num][z][y]))
        			{
        				if( x< sizeof(zone[MAP][place][num][z][y]) && !undefinedp(zone[MAP][place][num][z][y][x]))
        					return 1;
        			}
        				
        		}
        	}
        	
        	//if( !undefinedp(zone[MAP][zone]) )
        	//	return 1;
        }
        
        return 0;
}

int is_zone_location(array loc)
{
	return is_zone_exist(loc[PLACE],loc[NUM],loc[Z],loc[Y],loc[X]);
}

int valid_coordinate(string place,int num,int z,int y,int x)
{
        return ( x >=0 && y>=0 && is_zone_exist(place,num,z,y,x)  );
}

// ��Ӯ�W���� special_exit ���ɭԡA�N�|�����ǰe�� link_target
mixed query_special_exit(array loc)
{
	mapping a;
	if( !undefinedp( zone[DATA][save_variable(loc)] ) && sizeof(zone[DATA][save_variable(loc)][SPECIAL_EXIT]) )
		return zone[DATA][save_variable(loc)][SPECIAL_EXIT];
	else return a;
}

int set_special_exit(array loc,mixed target)
{
	if( undefinedp( zone[DATA][save_variable(loc)] ))
		zone[DATA][save_variable(loc)]=([]);

	//if( undefinedp(zone[DATA][save_variable(loc)][SPECIAL_EXIT]) )
	zone[DATA][save_variable(loc)][SPECIAL_EXIT]=target;
		
	//if( !undefinedp(zone[DATA][save_variable(loc)][SPECIAL_EXIT] ))
	//	return 0;
		
	//zone[DATA][save_variable(loc)][SPECIAL_EXIT]=target;
	
	return 1;
}

int del_special_exit(array loc)
{
	if( undefinedp( zone[DATA][save_variable(loc)]) || undefinedp(zone[DATA][save_variable(loc)][SPECIAL_EXIT]))
		return 0;
	
	//if(undefinedp(zone[DATA][save_variable(loc)][SPECIAL_EXIT][idx]))
	//	return 0;
		
	map_delete(zone[DATA][save_variable(loc)],SPECIAL_EXIT);
	return 1;
}

object find_module(string place)
{
	if( undefinedp(place) )
		return 0;
		
	if(!module_table[place] )
		module_table[place]=load_object(ZONE_GENERATOR_PATH+place+".c");
		
	return module_table[place];
}

mapping chinese =
([
    "north"             :"�_",
    "south"             :"�n",
    "east"              :"�F",
    "west"              :"��",
    "northwest"         :"��_",
    "northeast"         :"�F�_",
    "southwest"         :"��n",
    "southeast"         :"�F�n",
    "down"              :"�U",
    "up"                :"�W",
]);


int valid_move(string place,int num,int z,int y,int x)
{
	object module;
	//mixed data;
	
	if( (module=find_module(place) )->special_move() )
	{
		//if( !undefinedp(data=zone[DATA][save_variable(place,num,z,y,x)]) )
		//	module->move(data);
		return module->move(zone[MAP][place][num][z][y][x]);
	}
	
	// not design yet
	return 1;	
}


//   1  2   4
//   8      16
//   32 64  128
int generate_move_dir(string place,int num,int z,int y,int x)
{
	int flag=0;
	
	if( valid_coordinate(place,num,z,y,x+1) && valid_move(place,num,z,y,x+1) )
		flag |= 16;
	if( valid_coordinate(place,num,z,y,x-1) && valid_move(place,num,z,y,x-1) )
		flag |= 8;
	if( valid_coordinate(place,num,z,y+1,x) && valid_move(place,num,z,y+1,x) )
		flag |= 64;
	if( valid_coordinate(place,num,z,y-1,x) && valid_move(place,num,z,y-1,x) )
		flag |= 2;
		
	if( valid_coordinate(place,num,z,y-1,x+1) && valid_move(place,num,z,y-1,x+1) )
		flag |= 4;	
	if( valid_coordinate(place,num,z,y+1,x+1) && valid_move(place,num,z,y+1,x+1) )
		flag |= 128;
	if( valid_coordinate(place,num,z,y-1,x-1) && valid_move(place,num,z,y-1,x-1) )
		flag |= 1;
	if( valid_coordinate(place,num,z,y+1,x-1) && valid_move(place,num,z,y+1,x-1) )
		flag |= 32;
	
	return flag;
}

mixed query_data()
{
	return zone[DATA];	
}
varargs int move(object me, string direction)
{
        //object *ppls;
        int x, y,z, n, ox, oy;
        //mapping data;
        string place;
        array loc, next_loc;

        if( !objectp(me) || !arrayp(loc = query_temp("location",me)) ) return 0;

        ox = x = loc[X];
        oy = y = loc[Y];
        z       =loc[Z];
        place = loc[CITY];
        n = loc[NUM];

        switch(direction)
        {
                case "north"    :       y--;            break;
                case "south"    :       y++;            break;
                case "west"     :       x--;            break;
                case "east"     :       x++;            break;
                case "northeast":       y--;x++;        break;
                case "southwest":       y++;x--;        break;
                case "northwest":       y--;x--;        break;
                case "southeast":       y++;x++;        break;
                default: return tell(me, "�o�Ӥ�V�S���X���C\n", CMDMSG);
        }

        if( !valid_coordinate(place,n,z,y,x) )
        	return tell(me, "�o�Ӥ�V�S���X���C\n", CMDMSG);
        	
	if( !valid_move(place,n,z,y,x)) 
		return tell(me, "�o�Ӥ�V�S���X���C\n", CMDMSG);
		
	
		
	next_loc = arrange_location(x,y,z,place,n,0,x+"/"+y+"/"+z+"/"+place+"/"+n+"/0");
	
	if( !undefinedp(query_special_exit(next_loc)) )
	{
		tell(me, "yes\n", CMDMSG);
		me->move(query_special_exit(next_loc));
		return 1;
	}
// icon change ��� ppl ���W�� move �I�s map_d �W���� icon ����	
/*		
        next_loc = arrange_location(x,y,z,place,n,x+"/"+y+"/"+z+"/"+place+"/"+n+"/0");

	
		
        ppls = filter_array(present_objects(me)-({me}), (: userp($1) :));

        broadcast(loc, me->query_idname()+"��"+chinese[direction]+"���}�C\n", 0, me);
      


        if( !sizeof( ppls ) )
                zone[RMAP][place][n][z][oy][ox] = copy(zone[MAP][place][n][z][oy][ox]);
        else if( sizeof(ppls) > 1 )
                zone[RMAP][place][n][z][oy][ox] = ansi_part(zone[MAP][place][n][z][oy][ox]) + HIC"��"NOR;
        else
                zone[RMAP][place][n][z][oy][ox] = ansi_part(zone[MAP][place][n][z][oy][ox]) + (query("gender", ppls[0]) == MALE_GENDER ? HIG + "��" : HIR + "��") + NOR;

        me->move(next_loc);

        if( sizeof(filter_array(present_objects(me), (: userp($1) :))) > 1 )
                zone[RMAP][place][n][z][y][x] = ansi_part(zone[MAP][place][n][z][y][x]) + HIC"��"NOR;
        else
                zone[RMAP][place][n][z][y][x] = ansi_part(zone[MAP][place][n][z][y][x]) + (query("gender", me) == MALE_GENDER ? HIG + "��" : HIR + "��") + NOR;
*/
	me->move(next_loc);
        return 1;
}

varargs string show_map(array loc, int cover_range)
{
        int i, j=1, dir, x, y,z, number, range,
        x_start, y_start,
        x_center, y_center,
        x_cover=1, y_cover=1;
        string return_map,place;
        mapping column = ([]);


        if( !loc ) return 0;

        x       =loc[X];
        y       =loc[Y];
        z       =loc[Z];
        place    =loc[CITY];
        number  =loc[NUM];
        
        x_center=(VISION_WIDTH+1)/2;
        y_center=(VISION_LENGTH+1)/2;

        if( cover_range > VISION_LENGTH/2 ) cover_range = VISION_LENGTH/2;
        x_center -= cover_range*3;
        y_center -= cover_range;

        // ��ܰ϶��p�⤽��
        x_start = (x>=x_center)*(x+1-x_center) - ((x+x_center)>MAP_WIDTH)*(x_center+x-MAP_WIDTH);
        y_start = (y>=y_center)*(y+1-y_center) - ((y+y_center)>MAP_LENGTH)*(y_center+y-MAP_LENGTH);

        range = y_start-cover_range;

        // �ϤW������u
        return_map =NOR WHT"\e[s\e["+j+++";1H   �z"+sprintf(HIG" %|3d "NOR+WHT"��"+HIG" %|3d "NOR+WHT+"�w"HIG" %|2d "NOR WHT,x+1,y+1,number+1)+"�w�w�w�w�w�w�w�w�w�w "+HIC+TIME_D->gametime_digital_clock()+NOR+WHT" �s�w�w�w�w�w�{";

        //dir = CITY_D->direction_check(x, y, place, number);
        //emblem = CITY_D->query_info(place, "emblem");
        
        dir = generate_move_dir(place,number,z,y,x);

        column[range+1] =       "�x"+ WHT"          �x";
        column[range+2] =       "�x"+ WHT"          �x";
        column[range+3] =       "�x"+ WHT"          �x";
        column[range+4] =       "�x"HIW"�H�f�G"HIG+sprintf("�F�I")+WHT"�x";
        column[range+5] =	"�x"HIW"�Ѯ�G"+(NATURE_D->query_weather(MAP_D->query_maproom(loc)))[NATURE_WEATHER][WEATHER_CHINESE]+WHT+"�x";
        column[range+6] =       "�u�w�w�w�w�w�t";
        column[range+7] =       "�x  "+(dir&1?HIW"��":NOR WHT"��")      +(dir&2?HIW"��":NOR WHT"�x")    +(dir&4?HIW"��":NOR WHT"��")    +NOR WHT"  �x";
        column[range+8] =       "�x  "+(dir&8?HIW"��":NOR WHT"�w")      +HIW+"��"+NOR WHT               +(dir&16?HIW"��":NOR WHT"�w")   +NOR WHT"  �x";
        column[range+9] =       "�x  "+(dir&32?HIW"��":NOR WHT"��")     +(dir&64?HIW"��":NOR WHT"�x")   +(dir&128?HIW"��":NOR WHT"��")  +NOR WHT"  �x";

        x_cover += cover_range*6;
        y_cover += cover_range*2;

        for(i=0;i<cover_range;i++)
                return_map += "\e["+j+++";1H   �x"NOR+"                                                  "+WHT+column[++range];

//shout(sprintf("%O\n",zone[RMAP][place][number]));
        // �H Y �b�c�m�X�ϧ�
        foreach( string *a in zone[RMAP][place][number][z][y_start..y_start+VISION_LENGTH-y_cover] )
        {

                if( range++ == y )
                        return_map += "\e["+j+++";1H   �x"NOR+repeat_string("  ",cover_range*3)+implode(a[x_start..x-1]+({ansi_part(a[x])+HIC"��"NOR})+a[x+1..x_start+VISION_WIDTH-x_cover],"")+NOR+repeat_string("�@",cover_range*3)+WHT+column[range];
                else
                        return_map += "\e["+j+++";1H   �x"NOR+repeat_string("  ",cover_range*3)+implode(a[x_start..x_start+VISION_WIDTH-x_cover],"")+NOR+repeat_string("�@",cover_range*3)+WHT+column[range];
        }

        for(i=0;i<cover_range;i++)
                return_map += "\e["+j+++";1H   �x"NOR+"                                                  "+WHT+column[++range];

        // �ϤU����u
        return_map += "\e["+j+++";1H   �|[ ";

        return_map += repeat_string(" ", 20);

        return_map+= NOR WHT" ]�w�w�w�w�w�w�w�w�w�w�w�w�w�r�w�w�w�w�w�}\e["+j+";1H"NOR MAG"________________________________________________________________\e[4m�a�Ϩt��_\e[u"NOR;

        return return_map;
}



object query_maproom(array loc)
{
        string place;
        int num,z;
        
        place=loc[PLACE];
        num=loc[NUM];

        if( !is_zone_location(loc) ) return 0;

       
	//shout(sprintf("%O\n",zone[DATA][save_variable( ({place,num,z}) )][ZONE_ROOM]));
        return zone[DATA][save_variable( ({place,num,z}) )][ZONE_ROOM];
        
        //return load_object("/wiz/room/room_hall1");
}

object *query_all_maproom() 
{
	object* rooms=({});
	
	foreach(string t,mapping data in zone[DATA])
	{
		foreach(string id,object zoneroom in data)
			if( !undefinedp(zoneroom))
				rooms+=({zoneroom});
	}
	
	return rooms;
	//return children(ZONE_ROOM);
}

void set_player_icon(object me, array loc)
{
	
}

void remove_player_icon(object me, array loc)
{
	
}

void handle_zone()
{
        object module;

        foreach( string filename in get_dir(ZONE_GENERATOR_PATH) )
        {
                // �� zone generator ���ͪ��a�ϧ˶i zone_d,�z�פW�|�䴩 xyz �T�b
                //printf("peer:%O\n",module->create_extra_info());
                //int
		int num_count = 0;
                if( !catch(module = load_object(ZONE_GENERATOR_PATH+filename)) )
                {
                        //shout(sprintf("%O\n",module->create_extra_info()));
                        zone[RMAP][filename[0..<3]]=fetch_variable("map",module);
                        //zone[RMAP][filename[0..<3]]=module->create_zone_map();
                        zone[MAP][filename[0..<3]]=copy(zone[RMAP][filename[0..<3]]);
                } else continue;
                // �s�y�C�@�h�� map_room (�H 'y' �����)
                foreach(mixed num in zone[RMAP][filename[0..<3]])
                {
                        int z_count=0;
                        foreach(mixed z in num)
                        {
                                
                                //if( !undefinedp(z) )
                                //{
                                // �ѧO�W�� [zone][place][num][x][y][x]
                                zone[DATA][save_variable( ({filename[0..<3],num_count,z_count}) )]=([]);
                                zone[DATA][save_variable( ({filename[0..<3],num_count,z_count}) )][ZONE_ROOM]=load_zoneroom(filename[0..<3],num_count,z_count);
                                //}
                                z_count++;
                        }
                        num_count++;
                }
                module_table[filename[0..<3]]=module;
        }
}



void create()
{
        if( clonep() )
                destruct(this_object());

        handle_zone();
}

string query_name()
{
	return "�S��ϰ�t��(ZONE_D)";
}
