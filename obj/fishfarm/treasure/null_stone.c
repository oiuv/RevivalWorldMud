/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : null_stone.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : �L���
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
	set_idname(HIR"null "NOR RED"stone"NOR, HIR"�L��"NOR RED"��"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@���`���⪺���Y�A���Y�����`�B���G���@�ǲ`�㪺�C��b�����a�y�ʵۡC\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 12000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 45);
	set(BUFF_ENERGY_REGEN, 5);
	set("buff/status", HIR"�L��"NOR RED"��"NOR);
	
	::setup_equipment(EQ_EYE, HIG"��"NOR GRN"��"NOR);
}
