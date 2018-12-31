/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mapob_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-3-18
 * Note   : �a�Ϩt�ΤW������w��P����(���ȥ� NPC / �԰��� NPC / ��L�S��a�Ϫ��~)
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <daemon.h>
#include <location.h>

#define LOC_MIN		0
#define LOC_MAX		1
#define MAP_ID		0
#define MAP_NUM		1

array mapob =
({

});

int is_mapob(object ob)
{
	int i, size;
	
	size = sizeof(mapob);

	for(i=0;i<size;i++)
		if( member_array(ob, mapob[i]["objects"]) != -1 )
			return 1;

	return 0;
}

array query_mapob()
{
	return mapob;
}

void heart_beat()
{
	int i, j, size;
	object ob;
	array loc;
	string *valid_direction;
	string direction;
	string filename;
	mapping current_objects_count;
	int amount;

	size = sizeof(mapob);

	for(i=0;i<size;i++)
	{
		// �M�����s�b������
		mapob[i]["objects"] -= ({ 0 });

		// ��Ĥ@�Ӫ���ӧP�_�ثe�y��	
		if( sizeof(mapob[i]["objects"]) )
			loc = query_temp("location", mapob[i]["objects"][0]);
		else
			loc = 0;

		current_objects_count = allocate_mapping(0);
		// �p��{���ƶq, ��X�ثe�y��
		foreach(ob in mapob[i]["objects"])
			current_objects_count[base_name(ob)]++;

		
		// �ˬd��ӹζ�������O�_�٦s�b
		foreach(filename, amount in mapob[i]["group"])
		{
			// ��ڼƶq�֩�]�w�ƶq(�����i�歫��)
			if( current_objects_count[filename] < amount )
			{
				if( undefinedp(mapob[i]["reborn_filename"]) )
					mapob[i]["reborn_filename"] = allocate_mapping(0);
					
				if( undefinedp(mapob[i]["reborn_filename"][filename]) )
					mapob[i]["reborn_filename"][filename] = allocate(0);
			
				// �Y�ثe�ƧǤ������͸�Ƥ����ʤ֪������ -> �s�W���͸��
				if( sizeof(mapob[i]["reborn_filename"][filename]) < amount - current_objects_count[filename] )
					mapob[i]["reborn_filename"][filename] += allocate(amount - current_objects_count[filename], mapob[i]["reborn_time"]);
					
				// ���˥ثe���͸�� -> �˼Ƨ����i�歫��
				for(j=0;j<sizeof(mapob[i]["reborn_filename"][filename]);j++)
				{
					// �˼Ƨ���, �i�歫��
					if( !--mapob[i]["reborn_filename"][filename][j] )
					{
						// �S���y��, �إ߷s�y��
						if( !arrayp(loc) )
						{
							loc = arrange_location(
								range_random(mapob[i]["x"][LOC_MIN], mapob[i]["x"][LOC_MAX]), 
								range_random(mapob[i]["y"][LOC_MIN], mapob[i]["y"][LOC_MIN]), 
								0, 
								mapob[i]["map"][MAP_ID], 
								mapob[i]["map"][MAP_NUM], 
								0
							);
						}
						
						ob = new(filename);
						ob->move(loc);
						ob->do_command("wahaha �ڭ��ͤF�I�I�I");
						mapob[i]["objects"] += ({ ob });
					}
				}
				mapob[i]["reborn_filename"][filename] -= ({ 0 });
				
				if( !sizeof(mapob[i]["reborn_filename"][filename]) )
					map_delete(mapob[i]["reborn_filename"], filename);
			}
		}

		// �Y�q�q���٨S����, �N���L		
		if( !sizeof(mapob[i]["objects"]) ) continue;

		// �i�沾�ʳB�z
		if( --mapob[i]["move_speed_count"] <= 0 )
		{
			mapob[i]["move_speed_count"] = mapob[i]["move_speed"];
			
			// �����ʽd�򪺸ܴN�i�沾��
			if( mapob[i]["x"][LOC_MIN] != mapob[i]["x"][LOC_MAX] || mapob[i]["y"][LOC_MIN] != mapob[i]["y"][LOC_MAX] )
			{				
				valid_direction = allocate(0);
				
				if( loc[X] > mapob[i]["x"][LOC_MIN] ) valid_direction += ({ "west" });
				if( loc[X] < mapob[i]["x"][LOC_MAX] ) valid_direction += ({ "east" });
				if( loc[Y] > mapob[i]["y"][LOC_MIN] ) valid_direction += ({ "north" });
				if( loc[Y] < mapob[i]["y"][LOC_MAX] ) valid_direction += ({ "south" });
				
				direction = valid_direction[random(sizeof(valid_direction))];
				
				foreach(ob in mapob[i]["objects"])
					ob->do_command("go "+direction);
			}
		}
	}	
}

void create()
{
	int i, size;
	mapping data;
	string filename;
	int amount;
	array loc;
	object ob;

	size = sizeof(mapob);

	for(i=0;i<size;i++)
	{
		data = mapob[i];

		loc = arrange_location(
			range_random(data["x"][LOC_MIN], data["x"][LOC_MAX]), 
			range_random(data["y"][LOC_MIN], data["y"][LOC_MIN]), 
			0, 
			data["map"][MAP_ID], 
			data["map"][MAP_NUM], 
			0
		);
		
		mapob[i]["objects"] = allocate(0);

		foreach(filename, amount in data["group"])
		while(amount--)
		{
			ob = new(filename);
			ob->move(loc);
			ob->do_command("wahaha �ڭ��ͤF�I�I�I");
			mapob[i]["objects"] += ({ ob });
		}
	}
	
	set_heart_beat(10);
}

int remove()
{
	object ob;
	mapping data;

	foreach(data in mapob)
	{
		if( arrayp(data["objects"]) )
			foreach(ob in data["objects"])
				if( objectp(ob) )
					destruct(ob);
	}
	
	mapob = allocate(0);
}

string query_name()
{
	return "�a�Ϫ��󱱨�t��(MAPOB_D)";
}
