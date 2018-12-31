/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : egg.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : ���J
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("egg","���J");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "���L�z�����ն������J�C\n");
	set("unit", "��");
	set("mass", 50);
	set("value", 100);
}
