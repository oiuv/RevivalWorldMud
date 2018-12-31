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
	set_idname(HIY"G"NOR""YEL"olden "HIC"rod"NOR, HIY"����"NOR""YEL"�p����"NOR );
	
	set_temp("endurance", 2147483647); 

	if( this_object()->set_shadow_ob() ) return;

	set("unit", "��");
	set("value", 999999999);
	set("mass", 1);
	set("rod_level", 100);
	set("flag/no_amount", 1);
	set("badsell", 1);
}
