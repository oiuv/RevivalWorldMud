/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : integrity_stone.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �����
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
	set_idname(HIW"integrity "NOR WHT"stone"NOR, HIW"����"NOR WHT"��"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@���z�������Y�A���Y�����`�B���G���@�ǲ`�㪺�C��b�����a�y�ʵۡC\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 3000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 20);
	set("buff/status", HIW"����"NOR WHT"��"NOR);
	
	::setup_equipment(EQ_EYE, HIG"��"NOR GRN"��"NOR);
}
