/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rice.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-2-20
 * Note   : �_��
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("rice","�_��");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("unit", "�U");
	set("mass", 400);
	set("value", 100);
}