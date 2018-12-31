/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : area.c
 * Author : Cookys@RevivalWorld
 * Date   : 2002-02-20
 * Note   : included by Simul_Efun Object
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <object.h>

/* Ū�J area_room  */
object load_zoneroom(string zone, int num, int z)
{
	object zoneroom;
	
	if( !stringp(zone) || !stringp(zone) || !intp(z) ) return 0;
		
	foreach( object child in children(ZONEROOM) )
		if( query("zone", child) == zone  && query("z", child) == z &&  query("num", child) == num   )
			return child;

	zoneroom = new(ZONEROOM);
	
	set("zone", zone, zoneroom);
	set("num", num, zoneroom);
	set("z", z, zoneroom);
	
        return zoneroom;
}
