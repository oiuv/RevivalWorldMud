/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : blood_orchid.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-17
 * Note   : ������
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
	set_idname(HIR"blood "NOR RED"orchid"NOR, HIR"��"NOR RED"����"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�@���A�A�L�񪺨u������C\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 3000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 20);
	set("buff/status", HIR"��"NOR RED"����"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"��"NOR GRN"�w"NOR);
}
