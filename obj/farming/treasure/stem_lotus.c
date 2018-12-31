/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : stem_lotus.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : �ø���
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
	set_idname(HIG"stem "NOR GRN"lotus"NOR, HIG"�ø�"NOR GRN"��"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "���֤H��l�֤��G�u�~�Ǽ`�S���n�f�A�ѥx�e�z�⯫�P�C��ɺɦ����y�L�A�֦V�H���@�罬�C�v\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 12000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 45);
	set(BUFF_SOCIAL_EXP_BONUS, 25);
	set("buff/status", HIG"�ø�"NOR GRN"��"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"��"NOR GRN"�w"NOR);
}
