/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : array.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-11-06
 * Note   : included by Simul_Efun Object
 * Update :
 *
 *
 -----------------------------------------
 */

void reverse_array(array ref arr)
{
	if( !arrayp(arr) || !sizeof(arr) )
		return;
		
	foreach( mixed element in arr )
		arr = ({ element }) + arr[0..<2];
}