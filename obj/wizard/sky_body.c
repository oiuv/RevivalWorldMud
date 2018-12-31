/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sky_body.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-20
 * Note   : �_�ѥ�
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
#include <daemon.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname("sky body", HIW"�_"NOR WHT"��"HIW"��"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�_�ѥҡC\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 100000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 5);
	set(BUFF_STAMINA_REGEN, 5);
	set(BUFF_ARMOR_INJURY, 5);
	set(BUFF_ARMOR_SLASH, 5);
	set("buff/status", HIW"�_"NOR WHT"��"HIW"��"NOR);
	
	::setup_equipment(EQ_BODY, HIY"��"NOR YEL"��"NOR);
}

//
// ����۰ʯS���m
//
void special_defend(object attacker, object defender)
{
	int damage = range_random(10,20);

	if( !random(2) )
	{
		msg("$ME�Q$YOU���W��"+this_object()->query_name()+"�y�U���W�빺�F�@�U�A�y�����L���ˮ`�C\n", attacker, defender, 1);
		COMBAT_D->damage_message(defender, attacker, damage);
		attacker->cost_health(damage);
	}
}