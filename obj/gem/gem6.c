/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : gem.c
 * Author :
 * Date	  : 2003-5-3
 * Note	  : Fish����
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>

inherit __DIR__"_gem_object.c";

void create()
{
	set_idname( "sapphire", HIB"���_��"NOR );

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�Ĥ����� �� �u�šv�C");
	set("unit", "��");
	set("value", 7777);
	set("mass", 77);
	set("gemcode", 5);
}