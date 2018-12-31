/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wool.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �Ϥ�
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("wool","�Ϥ�");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�¥զ⪺�Ϥ�C\n");
	set("unit", "��");
	set("mass", 100);
	set("value", 15000);
}
