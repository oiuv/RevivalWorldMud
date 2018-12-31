/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : venison.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : ����
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("venison","����");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�צ��A�������סC\n");
	set("unit", "��");
	set("mass", 600);
	set("value", 20000);
}
