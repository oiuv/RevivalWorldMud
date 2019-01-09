/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : flatbed_tricycle.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-08
 * Note   : ���O��
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
	set_idname(NOR YEL"flatbed tricycle", NOR YEL"���O��");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�@���A�X�ΨӸ�������y���O���C\n");

	set("unit", "�x");
	set("mass", 8000);
	set("value", 50000);
	set("flag/no_amount", 1);
	set("badsell", 1);

	set(BUFF_LOADING_MAX, 2000);
	set(BUFF_MOVING_STAMINA, -50);

	::setup_equipment(EQ_MOUNT, HIG"��"NOR GRN"��"NOR);
}
