/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : area.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-20
 * Note   : included by Simul_Efun Object
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */


#include <object.h>
#include <location.h>

/* Ū�J area_room  */
object load_arearoom(string area, int num)
{
	object arearoom;
	
	if( !stringp(area) || !intp(num) ) return 0;
		
	foreach( object child in children(AREAROOM) )
		if( query("area", child) == area && query("num", child) == num )
			return child;

	arearoom = new(AREAROOM);
	
	set("area", area, arearoom);
	set("num", num, arearoom);
	
        return arearoom;
}

array arrange_area_location(int x, int y, string area, int num)
{
	return ({ x, y, 0, area, num, 0, x+"/"+y+"/0/"+area+"/"+num+"/0" });
}

array arrange_defaultmap_location(int x, int y, string map, int num)
{
	return ({ x, y, 0, map, num, 0, x+"/"+y+"/0/"+map+"/"+num+"/0" });
}
