/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : liang_fan_huan.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-13
 * Note   : 
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

/*
	�t�ιw�]�� NPC �����禡
	void reply_emote(object me, string verb, string args);
	void reply_say(object me, string msg);
	void reply_catch_tell(object me, string verb, string args);
	void reply_get_object(object me, object ob, mixed amount);
	void reply_get_money(object me, string unit, mixed money);
*/

#include <ansi.h>
#include <inherit.h>
#include <gender.h>
#include <daemon.h>

#define QUEST_1 "���¦Ф���"

inherit STANDARD_NPC;

void create()
{
	set_idname("liang fan huan","�}�Z��");

	set("long", "�@��Q������`���k�l�A�ݤf���M���`�έI���C");
	set("unit", "��");
	set("age", 25);
	set("gender", MALE_GENDER);
	set_temp("status", ([ HIR"����"NOR : -1 ]));
}
