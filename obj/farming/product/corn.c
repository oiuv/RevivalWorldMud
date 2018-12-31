/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : corn.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �ɦ�
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("corn","�ɦ�");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long","�㦳���@�f�������\���~�A�S�٬��f���B�ɸ����B�s���C\n");
	set("unit", "�U");
	set("mass", 800);
	set("value", 400);
}
