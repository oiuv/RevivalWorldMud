/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sky_sword.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-20
 * Note   : �_�ѼC
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
#include <weapon.h>
#include <daemon.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname("sky sword", HIW"�_"NOR WHT"��"HIW"�C"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@��i�H�}�B�b�Ť������C�C\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 100000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 5);
	set(BUFF_STAMINA_REGEN, 5);
	set("weapon_type", WEAPON_TYPE_SLASH);
	set("buff/status", HIW"�_"NOR WHT"��"HIW"�C"NOR);
	
	::setup_equipment(EQ_HAND, HIY"��"NOR YEL"��"NOR);
}


//
// �Z���۰ʯS�����
//
void special_attack(object attacker, object defender)
{
	int damage = range_random(80,100);

	if( !random(5) )
	{
		msg("$ME��W��"+this_object()->query_idname()+"��M�o�X�@�D�ե��g�V�ѻڨë����U�A�A�y��$YOU���Y����ˡC\n", attacker, defender, 1);
		COMBAT_D->damage_message(attacker, defender, damage);
		defender->cost_health(damage);
	}
}
