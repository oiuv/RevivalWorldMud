/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : beauty_hibiscus.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : ���ܻT
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
	set_idname(HIR"beauty "NOR RED"hibiscus"NOR, HIR"��"NOR RED"�ܻT"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�u���b����²b�������~��ͪ������ܻT�A�X�񵴺ئ]���������_�C�v\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 8000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 36);
	set(BUFF_SOCIAL_EXP_BONUS, 15);
	set("buff/status", HIR"��"NOR RED"�ܻT"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"��"NOR GRN"�w"NOR);
}
