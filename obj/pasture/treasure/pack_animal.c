/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pack_animal.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : 馱獸
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname("pack animal", HIY"馱"NOR YEL"獸"NOR);
	set_temp("status", HIC"稀"NOR CYN"有");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一匹可以用來載重物品的動物。\n");
	set("unit", "匹");
	set("mass", 0);
	set("value", 3000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 20);
	set("buff/status", HIY"馱"NOR YEL"獸"NOR);

	::setup_equipment(EQ_MOUNT, HIG"座"NOR GRN"騎"NOR);
}
