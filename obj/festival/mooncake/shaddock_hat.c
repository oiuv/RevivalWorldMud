/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : shaddock_hat.c
 * Author : Kyo@RevivalWorld
 * Date   : 2006-10-07
 * Note   : ����`§�� - �c�l�ִU
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
	set_idname("shaddock hat", HIG"�c�l"NOR+GRN"��"HIW"�U"NOR);
	set_temp("status", HIB"����`"NOR BLU"§��");

	if( this_object()->set_shadow_database() ) return;

	set("long", "���ͪ��@�ɤ���`��§���C\n");
	
	set("unit", "��");
	set("mass", 0);
	set("value", 10);
	set("badsell", 1);
	set("flag/no_amount", 1);
	set("flag/no_drop", 1);
	set("flag/no_give", 1);
	set("flag/no_import", 1);

	set(BUFF_CHA, 1);
	//set("buff/status", HIB"��"NOR+BLU"��"NOR);
	::setup_equipment(EQ_HEAD, HIG"�Y"NOR GRN"��"NOR);
}
