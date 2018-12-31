/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _behavior_walking_npc.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-10
 * Note   : NPC ���ʦ欰
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <location.h>
#include <daemon.h>

private nomask int process_command(string verb, string arg);

private nosave array directions = ({"northwest","north","northeast","west","east","southwest","south","southeast"});

//   1  2   4
//   8      16
//   32 64  128
// ���ʦ欰
void behavior_walking()
{
	int x, y;
	// �㰣�A�ݭn���ʡC
	string *exits = allocate(0);
	object env = environment();

	if( !objectp(env) ) return;

	// �S��a�Ϩt�β���
	if( env->is_maproom() )
	{
		array loc = query_temp("location");

		// �N�ӻݭn��g map_d.c �[�J query_exits �\��
		// �{�b�u�䴩 CITY_D
		exits = trans_bit_to_array(CITY_D->direction_check(loc[X], loc[Y], loc[CITY], loc[NUM]),directions);
		
		foreach( string coor, string roomtype in CITY_D->query_coor_range(loc, ROOM, 3) )
		{
			if( roomtype != "store" ) continue;
			
			sscanf(coor, "({%d,%d,%*s", x, y);
			
			if( loc[X] > x && random(3) )
				exits -= ({ "northeast", "east", "southeast" });
			if( loc[Y] > y && random(3) )
				exits -= ({ "northwest", "west", "southwest" });
			
			break;
		} 	
	} else {
		exits = keys(query("exits", env));
	}

	if( !sizeof(exits) )
	{
		CHANNEL_D->channel_broadcast("sys", sprintf("%O cant find any exits. terminal random run.\n",this_object()));
		delete("random_run");
		return;
	}

	process_command("go", exits[random(sizeof(exits))]);
}
