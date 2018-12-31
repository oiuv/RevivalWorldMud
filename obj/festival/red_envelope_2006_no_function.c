/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : red_envelope_2006_no_function.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-28
 * Note   : �������~���]
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname(HIR"red envelope 2006"NOR, HIY"����"NOR YEL"���~"HIC"��"NOR CYN"��"HIR"��"NOR RED"�]"NOR);
	
	if( this_object()->set_shadow_database() ) return;
	
	set("unit", "��");
	set("long", "2006 �������~�������]");
	set("mass", 100);
	set("value", 100);
	set("flag/no_amount", 1);
}
