/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : growth_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-04
 * Note   : �������F - �A�� / ���� / �i�޳�
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <ansi.h>
#include <daemon.h>
#include <location.h>
#include <message.h>
#include <nature.h>
#include <nature_data.h>

int tick;
mapping growth;

varargs void set_status(array loc, mapping data)
{
	object ob;
	string status="";
	int growth_level = CITY_D->query_coor_data(loc, "growth_level");
	int maintain_time = CITY_D->query_coor_data(loc, "maintain_time");

	if( undefinedp(data) )
	{
		switch(CITY_D->query_coor_data(loc, TYPE))
		{
			case FARM:
				status =
					HIG"  �ش�"NOR GRN"�@���G"NOR+"�L\n"
					HIG"  �g�a"NOR GRN"���šG"HIG+repeat_string("*", growth_level)+NOR GRN"/"+maintain_time+"\n"NOR;
				break;
			case PASTURE:
				status =
					HIY"  �b�i"NOR YEL"�ʪ��G"NOR+"�L\n"
					HIY"  �g�a"NOR YEL"���šG"HIY+repeat_string("*", growth_level)+NOR YEL"/"+maintain_time+"\n"NOR;
				break;
			case FISHFARM:
				status =
					HIB"  �i��"NOR BLU"�����G"NOR+"�L\n"
					HIB"  �g�a"NOR BLU"���šG"HIB+repeat_string("*", growth_level)+NOR BLU"/"+maintain_time+"\n"NOR;
				break;
		}
		status += "\n";
		CITY_D->set_coor_data(loc, "status", status);
		return;
	}

	switch(CITY_D->query_coor_data(loc, TYPE))
	{
		case FARM:
			status =
				HIG"  �ش�"NOR GRN"�@���G"NOR+data["idname"]+"\n"
				HIG"  �ش�"NOR GRN"�ƶq�G"NOR+data["input_amount"]+"/"+(10+growth_level)+"\n"
				HIG"  ����"NOR GRN"��ҡG"NOR+data["harvest_percent"]+" %\n"
				HIG"  �g�a"NOR GRN"���šG"HIG+repeat_string("*", growth_level)+NOR GRN"/"+maintain_time+"\n"NOR
				HIG"  �ش�"NOR GRN"�ɶ��G"NOR+(data["tick"]/60)+" �� "+(data["tick"]%60)+" �� "+(data["wait_for_harvesting"] ? HIG"(�w�i����)"NOR : "" )+"\n"
				HIG"  �ش�"NOR GRN"���A�G"NOR+(data["current_growing"]||"�|����@����\n");
			
			if( data["material"] )
				status += HIG"  �ݭn"NOR GRN"���ơG"NOR;
			break;
		case PASTURE:
			status =
				HIY"  �b�i"NOR YEL"�ʪ��G"NOR+data["idname"]+"\n"
				HIY"  �b�i"NOR YEL"�ƶq�G"NOR+data["input_amount"]+"/"+(10+growth_level)+"\n"
				HIY"  ����"NOR YEL"��ҡG"NOR+data["harvest_percent"]+" %\n"
				HIY"  �g�a"NOR YEL"���šG"HIY+repeat_string("*", growth_level)+NOR YEL"/"+maintain_time+"\n"NOR
				HIY"  �b�i"NOR YEL"�ɶ��G"NOR+(data["tick"]/60)+" �� "+(data["tick"]%60)+" �� "+(data["wait_for_harvesting"] ? HIY"(�w�i����)"NOR : "" )+"\n"
				HIY"  �b�i"NOR YEL"���A�G"NOR+(data["current_growing"]||"�|����������\n");
			
			if( data["material"] )
				status += HIY"  �ݭn"NOR YEL"���ơG"NOR;
			break;
		case FISHFARM:
			status =
				HIB"  �i��"NOR BLU"�����G"NOR+data["idname"]+"\n"
				HIB"  �i��"NOR BLU"�ƶq�G"NOR+data["input_amount"]+"/"+(10+growth_level)+"\n"
				HIB"  ����"NOR BLU"��ҡG"NOR+data["harvest_percent"]+" %\n"
				HIB"  �g�a"NOR BLU"���šG"HIB+repeat_string("*", growth_level)+NOR BLU"/"+maintain_time+"\n"NOR
				HIB"  �i��"NOR BLU"�ɶ��G"NOR+(data["tick"]/60)+" �� "+(data["tick"]%60)+" �� "+(data["wait_for_harvesting"] ? HIB"(�w�i����)"NOR : "" )+"\n"
				HIB"  �i��"NOR BLU"���A�G"NOR+(data["current_growing"]||"�|����������\n");

			if( data["material"] )
				status += HIB"  �ݭn"NOR BLU"���ơG"NOR;
			break;
	}	

	if( mapp(data["material"]) )
	{
		foreach(string file, int num in data["material"] )
		{
			ob = load_object(file);
			
			if( !objectp(ob) ) continue;
			
			status += num + " "+(query("unit", ob)||"���") + ob->query_idname()+", ";
		}
		
		status = status[0..<3]+"\n";
	}
		
	status += "\n";
	CITY_D->set_coor_data(loc, "status", status);
}

void grow_up(string sloc)
{
	int localtick;
	string weather;
	array loc = restore_variable(sloc);	
	mapping coor_data = CITY_D->query_coor_data(loc);
	mapping data = coor_data["growth"];

	switch(coor_data[TYPE])
	{
		case FARM:
		case PASTURE:
		case FISHFARM:
			break;
		default:
			this_object()->remove_growth(loc);
			return;
			break;
	}

	if( !mapp(data) || !data["started"] || data["wait_for_harvesting"] ) return;

	weather = NATURE_D->query_nature(MAP_D->query_maproom(loc))[NATURE_WEATHER][WEATHER_ID];

	switch( weather )
	{
		case SHOWER:
		case FOEHN:
		case SNOWY:
		case TYPHOON:
			if( random(4) ) break;
			
			if( random(10) && member_array(STATUE, values(CITY_D->query_coor_range(loc, TYPE, 3))) != -1 )
			{
				switch(weather)
				{
					case SHOWER:
						broadcast(loc, "������Y�¿p���J���o�X����"HIG"��"NOR GRN"��"NOR"�A�ѤW���U�����b�j�B���G��p�F�\�h�C\n");
						break;
					case FOEHN:
						broadcast(loc, "������Y�¿p���J���o�X����"HIG"��"NOR GRN"��"NOR"�A�b�ɵI�����G��z�F�\�h�C\n");
						break;
					case SNOWY:
						broadcast(loc, "������Y�¿p���J���o�X����"HIG"��"NOR GRN"��"NOR"�A�n�����G�C�C�a�ĤƤF�C\n");
						break;
					case TYPHOON:
						broadcast(loc, "������Y�¿p���J���o�X����"HIG"��"NOR GRN"��"NOR"�A���զ��G�v���ܮz�F�C\n");
						break;
				}
			}
			else
			{
				broadcast(loc, HIR+data["weather"][weather]+NOR);
				data["harvest_percent"] -= random(5);
				
				if( data["harvest_percent"] < 0 )
					data["harvest_percent"] = 0;
			}	
				

			break;
	}
			
	data["tick"] += 5;
	localtick = data["tick"];

	if( data["growing"][localtick] )
	{
		// �s�������T��
		broadcast(loc, data["growing"][localtick]);
		data["current_growing"] = copy(data["growing"][localtick]);
		map_delete(data["growing"], localtick);
				
		// ��������
		if( !sizeof(data["growing"]) )
			data["wait_for_harvesting"] = 1;
	}
	
	set_status(loc, data);
	CITY_D->set_coor_data(loc, "growth", data);
}

void heart_beat()
{
	string sloc;

	++tick;
	
	// �����B�z - �A��
	if( !((tick+1)%5) )
	{
		foreach(sloc in growth[FARM])
			grow_up(sloc);
	}
	// �����B�z - ����
	else if( !((tick+3)%5) )
	{	
		foreach(sloc in growth[PASTURE])
			grow_up(sloc);
	}	
	// �����B�z - �i�޳�	
	else if( !((tick+4)%5) )
	{
		foreach(sloc in growth[FISHFARM])
			grow_up(sloc);
	}
}

void add_growth(array loc)
{
	string sloc;
	int type = CITY_D->query_coor_data(loc, TYPE);

	switch(type)
	{
		case FARM:
		case PASTURE:
		case FISHFARM:
			sloc = save_variable(loc);
			growth[type] |= ({ sloc });
			break;
		default:
			break;
	}
}

void remove_growth(array loc)
{
	string sloc = save_variable(loc);

	foreach(int type, string *slocs in growth)
		growth[type] -= ({ sloc });

	CITY_D->delete_coor_data(loc, "growth");
}

void create()
{
	int num, x, y, type;
	array coordata;
	string sloc;

	growth = allocate_mapping(0);	
	growth[FARM] = allocate(0);
	growth[PASTURE] = allocate(0);
	growth[FISHFARM] = allocate(0);
	
	// ���s���J���
	foreach(string city in CITY_D->query_cities())
	{
		for(num=0;CITY_D->city_exist(city, num);++num)
		{
			coordata = CITY_D->query_all_coor_data(city, num);

			for(y=0;y<100;y++)
			for(x=0;x<100;x++)
			{
				type = coordata[y][x][TYPE];
				
				switch(type)
				{
					case FARM:
					case PASTURE:
					case FISHFARM:
						sloc = save_variable(arrange_city_location(x, y, city, num));
						growth[type] |= ({ sloc });
						break;
					default:
						break;
				}
			}
		}
	}
	set_heart_beat(10);
}

mapping query_growth_data()
{
	return growth;
}

string query_name()
{
	return "�����t��(GROWTH_D)";
}
