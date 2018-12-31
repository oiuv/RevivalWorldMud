/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : city.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-20
 * Note   : included by Simul_Efun Object
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <citydata.h>
#include <object.h>
#include <location.h>
#include <daemon.h>

/* Ū�J city_room  */
object load_cityroom(string city, int num)
{
	object cityroom;
	
	if( !stringp(city) || !intp(num) ) return 0;
		
	foreach( object child in children(CITYROOM) )
		if( query("city", child) == city && query("num", child) == num )
			return child;

	cityroom = new(CITYROOM);
	
	set("city", city, cityroom);
	set("num", num, cityroom);
	
        return cityroom;
}

array arrange_city_location(int x, int y, string city, int num)
{
	return ({ x, y, 0, city, num, 0, x+"/"+y+"/0/"+city+"/"+num+"/0" });
}

varargs string *city_roomfiles(array loc, string type)
{
	int x, y, num, floor;
	string city, dir, file;
	string *roomfiles = allocate(0);

	x = loc[X];
	y = loc[Y];
	num = loc[NUM];
	city = loc[CITY];

	dir = CITY_NUM_ROOM(city, num);
	
	if( undefinedp(type) )
		foreach( file in get_dir(dir)||({}) )
		{
			if( sscanf(file, (string)x+"_"+(string)y+"_%*s") == 1 )
				roomfiles += ({ dir+file });
		}
	else
	{
		file = dir+x+"_"+y+"_"+type+".o";
		
		if( file_size(file) >= 0 )
		{
			roomfiles += ({ file });
			
			while(1)
			{
				file = dir+x+"_"+y+"_"+(++floor)+"_"+type+".o";
				
				if( file_size(file) >= 0 )
					roomfiles += ({ file });
				else
					break;
			}
			
		}
	}
	
	return roomfiles;
}

varargs string city_coor_short(int x, int y, string city, int num)
{
	string str = "<";
	
	if( undefinedp(x) || undefinedp(y) )
		return 0;
	
	if( !undefinedp(city) )
		str += city + " ";
	
	if( !undefinedp(num) )
		str += (num+1) + " ";
		
	str += (x+1)+","+(y+1)+">";
	
	return str;
}

varargs string loc_short(mixed city, int num, int x, int y)
{
	if( arrayp(city) )
	{
		if( CITY_D->city_exist(city[CITY]) )
			return NOR WHT"<"NOR+CITY_D->query_city_id(city[CITY])+" "+(city[NUM]+1)+" "+(city[X]+1)+","+(city[Y]+1)+NOR WHT">"NOR;
		else if( AREA_D->area_exist(city[CITY]) )
			return NOR WHT"<"NOR+AREA_D->query_area_id(city[CITY])+" "+(city[NUM]+1)+" "+(city[X]+1)+","+(city[Y]+1)+NOR WHT">"NOR;
	}
	else if( stringp(city) )
	{
		if( CITY_D->city_exist(city) )
			return NOR WHT"<"NOR+CITY_D->query_city_id(city)+" "+(num+1)+" "+(x+1)+","+(y+1)+NOR WHT">"NOR;
		else if( AREA_D->area_exist(city) )
			return NOR WHT"<"NOR+AREA_D->query_area_id(city)+" "+(num+1)+" "+(x+1)+","+(y+1)+NOR WHT">"NOR;
	}

	return 0;
}

int belong_to_government(string owner)
{
	return stringp(owner) && owner[0..9] == "GOVERNMENT";
}

int belong_to_enterprise(string owner)
{
	return stringp(owner) && owner[0..9] == "ENTERPRISE";
}

int belong_to_individual(string owner)
{
	return stringp(owner) && owner[0..9] != "GOVERNMENT" && owner[0..9] != "ENTERPRISE";
}
