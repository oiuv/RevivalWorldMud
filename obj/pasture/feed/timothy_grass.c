/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : timothy_grass.c
 * Author : Clode@RevivalWorld
 * Date	  : 2003-09-03
 * Note	  : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname("timothy grass", NOR GRN"�����說��"NOR );

	set_temp("status", HIG"��"NOR GRN"��"NOR);

	if( this_object()->set_shadow_ob() ) return;
	
	set("unit", "��");
	set("value", 30);
	set("mass", 10);
	set("badsell", 1);
}
