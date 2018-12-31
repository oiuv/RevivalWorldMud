/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : location.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-24
 * Note   : included by Simul_Efun Object
 * Update :
 *
 *
 -----------------------------------------
 */

array arrange_location(int x, int y, int z, string place, int num, mixed extra)
{
	if( intp(extra) || stringp(extra) )
		return ({ x, y, z, place, num, extra, x+"/"+y+"/"+z+"/"+place+"/"+num+"/"+extra });
	else
		return ({ x, y, z, place, num, extra, x+"/"+y+"/"+z+"/"+place+"/"+num+"/"+typeof(extra) });
}
