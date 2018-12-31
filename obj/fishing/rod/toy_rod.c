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
	set_idname("toy rod", "���㳨��" );
	
	set_temp("endurance", 500);

	if( this_object()->set_shadow_ob() ) return;
	
	set("unit", "��");
	set("value", 100);
	set("mass", 100);
	set("rod_level", 30);
	set("level", 0);
	set("flag/no_amount", 1);
	set("badsell", 1);
}
