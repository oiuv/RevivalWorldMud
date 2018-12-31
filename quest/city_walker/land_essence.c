/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : land_essence.c
 * Author : 
 * Date   : 2005-01-05
 * Note   : �������O
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_OBJECT;

mapping actions;

void create()
{
	set_idname(HIY"land "NOR YEL"essence"NOR, HIY"�j�a"NOR YEL"��إ���"NOR);
	
	if( this_object()->set_shadow_database() ) return;
	
	set("long", "�@���i�H�l�Ǥg�a��ت��C������A���C���u��l�Ǥ@���C");
	set("unit", "��");
	set("mass", 3000);
	set("value", 100);
	set("badsell", 1);
	set("land_absorb", 1);
}
