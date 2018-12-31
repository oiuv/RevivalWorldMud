/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : stone.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-06
 * Note   : ���
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
	set_idname("stone",WHT"���"NOR);

	if( this_object()->set_shadow_ob() ) return;

	set("material/"+STONE, 1);
	set("unit","���");
	set("mass", 1000);
	set("value", 10);
}