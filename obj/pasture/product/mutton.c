/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mutton.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �Ϧ�
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("mutton","�Ϧ�");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�a�ۤ@�h�p�֪��ϦסC\n");
	set("unit", "��");
	set("mass", 600);
	set("value", 12000);
}
