/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : metal.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : ����
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
	set_idname("metal",HIW"����"NOR);

	if( this_object()->set_shadow_ob() ) return;
	
	set("material/"+METAL, 1);
	set("unit","���");
	set("mass", 1000);
	set("value", 10);
}