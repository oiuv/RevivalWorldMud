/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : Fish.c
 * Author :
 * Date	  : 2003-5-3
 * Note	  : ��
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname( HIM"po"HIY"mp"HIG"ano"NOR,	HIC"��"NOR""CYN"��"HIR"��"NOR );

	if( this_object()->set_shadow_ob() ) return;

	set("unit", "��");
	set("value", 2100);
	set("mass", 80);
}

