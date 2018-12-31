/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : buddha_wood.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : �s����
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
	set_idname(HIY"buddha "NOR YEL"wood"NOR, HIY"�s"NOR YEL"��"NOR HIY"��"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�@�ӥ��j����W���H���C�v\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 100000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 65);
	set(BUFF_SOCIAL_EXP_BONUS, 50);
	set("buff/status", HIY"��"NOR YEL"��"NOR HIY"��"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"��"NOR GRN"�w"NOR);
}
