/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : night_lotus.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : �]�β�
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
	set_idname(HIB"night "NOR BLU"lotus"NOR, HIB"�]��"NOR BLU"��"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�ʤH���šu�B��O�v���G�u�~�F�Ҧ��뤤�ӽ��A�j�p�\�A\n���@�V�A�中�]�ήѨ��A�W��]�β��C�v\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 6000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 30);
	set(BUFF_SOCIAL_EXP_BONUS, 5);
	set("buff/status", HIB"�]��"NOR BLU"��"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"��"NOR GRN"�w"NOR);
}
