/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : horseflesh.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : 馬肉
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("horseflesh","馬肉");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "相當具有嚼勁的馬肉。\n");
	set("unit", "塊");
	set("mass", 600);
	set("value", 15000);
}
