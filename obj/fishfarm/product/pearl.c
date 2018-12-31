/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pearl.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-10
 * Note   : �ï]
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("pearl","�ï]");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�¥յL�媺�ï]�C\n");
	set("unit", "��");
	set("mass", 50);
	set("value", 60000);
}
