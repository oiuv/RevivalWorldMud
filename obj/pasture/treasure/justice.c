/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : justice.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-20
 * Note   : �f�P��
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
	set_idname(HIW"jus"NOR WHT"tice"NOR, HIW"�f"NOR WHT"�P"HIW"��"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�֦����P�v�O���Ҥ��y�M�C\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 100000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 60);
	set(BUFF_STAMINA_REGEN, 10);
	set("buff/status", HIW"�f"NOR WHT"�P"HIW"��"NOR);
	
	::setup_equipment(EQ_MOUNT, HIG"�y"NOR GRN"�M"NOR);
}
