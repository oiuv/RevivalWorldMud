/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : big_shrimp.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-30
 * Note   : ��
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("big shrimp","��");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�κ��h�Ī��󽼡C\n");
	set("unit", "��");
	set("mass", 150);
	set("value", 4000);
}
