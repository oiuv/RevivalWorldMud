/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : gold.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-06
 * Note   : ���q
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <material.h>
#include <ansi.h>
#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("gold",HIY"���q"NOR);

	if( this_object()->set_shadow_ob() ) return;

	set("material/"+GOLD, 1);
	set("unit","���");
	set("mass", 100);
	set("value", 10);
}
