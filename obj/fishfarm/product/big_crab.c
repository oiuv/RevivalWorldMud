/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : big_crab.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-30
 * Note   : �j�h��
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("big crab","�j�h��");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�κ��h�Ī��j�h�ɡC\n");
	set("unit", "��");
	set("mass", 400);
	set("value", 12000);
}
