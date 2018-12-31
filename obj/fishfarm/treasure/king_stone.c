/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : king_stone.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : �Ҥ���
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
	set_idname(HIC"king "NOR CYN"stone"NOR, HIC"��"NOR CYN"��"HIC"��"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@���C�Ŧ⪺���Y�A���Y�����`�B���G���@�ǲ`�㪺�C��b�����a�y�ʵۡC\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 100000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 65);
	set(BUFF_ENERGY_REGEN, 10);
	set("buff/status", HIC"��"NOR CYN"��"HIC"��"NOR);
	
	::setup_equipment(EQ_EYE, HIG"��"NOR GRN"��"NOR);
}
