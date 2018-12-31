/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : thousandleaf_lotus.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : �d����
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
	set_idname(HIW"thousandleaf "NOR WHT"lotus"NOR, HIW"�d��"NOR WHT"��"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "��Ĭ��֤��G�u����ͲJ�d�A�b���Ѥk�C�{�����d���A�ּZ�ݦ�G�C\n�ũ��@�L�ǡA�Ȳ~�e��ҡC�M�u�i�ڼ�A���ڤT���C\n�y�M����ɡA�����ѥh�C�v\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 4000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 24);
	set("buff/status", HIW"�d��"NOR WHT"��"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"��"NOR GRN"�w"NOR);
}
