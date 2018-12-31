/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wild ginseng.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-13
 * Note   : ���s�H�x
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("wild ginseng", HIY"���s"NOR YEL"�H��"NOR);
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long","���G�O�@�ػ�}�����ħ��C\n");
	set("unit", "��");
	set("mass", 300);
	set("value", 1000);
}
