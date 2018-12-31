/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : purewater.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-04
 * Note   : �M��
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>

inherit STANDARD_OBJECT;
inherit WEAPON;

void create()
{
	set_idname("wood chair", HIY"����s"NOR YEL"�鹹"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("hands", 1);
	set("unit", "��");
	set("capacity", 10);
	set("mass", 100);
	set("value", 10);
	set("badsell", 1);
	set("flag/no_amount", 1);
}

int do_drink()
{
	object me = this_player();

	addn("abi/str/cur", 10, me);
	if( query("abi/str/cur", me) > query("abi/str/max", me) )
		set("abi/str/cur", copy(query("abi/str/max", me)), me);
	msg("$ME�B�P�B�P�a�ܤU$YOU�A��_�F�ǳ\����O�C\n", me, this_object(), 1);
	destruct(this_object(), 1);
	return 1;
}
