/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : Rod.c
 * Author :
 * Date	  : 2003-5-3
 * Note	  : Rod
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>

inherit	__DIR__"standard_rod";

void create()
{
	set_idname("plastic rod", "�콦����" );

	set_temp("endurance", 3000);

	if( this_object()->set_shadow_ob() ) return;

	set("unit", "��");
	set("value", 10000);
	set("mass", 200);
	set("rod_level", 60);
	set("level", 30);
	set("flag/no_amount", 1);
	set("badsell", 1);
}
