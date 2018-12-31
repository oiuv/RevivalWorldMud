/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : bug_red_envelope.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-12
 * Note   : bug �^�����]
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <daemon.h>

inherit STANDARD_OBJECT;

mapping actions;

string *awards =
({
	"/obj/book/skill/leadership-adv",
	"/obj/book/skill/architectonic-adv",
	"/obj/book/skill/strength-adv",
	"/obj/book/skill/physique-adv",
	"/obj/book/skill/intelligence-adv",
	"/obj/book/skill/agility-adv",	
	"/obj/book/skill/charisma-adv"
});

void do_open_envelope(object me, string arg)
{
	object award;
	int money = range_random(10000, 10000000);
	string msg;
	
	msg = HIY"$RW "+NUMBER_D->number_symbol(money)+NOR;

	me->earn_money("RW", money);

	if( !random(10) )
	{
		award = new(awards[random(sizeof(awards))]);

		msg += " �M"+award->query_idname();
		
		award->set_keep();
		award->move(me);
	}
	else
		msg += " (�I�H�I�S����S����~�I)";
	
	CHANNEL_D->channel_broadcast("news", me->query_idname()+"���}"+this_object()->query_idname()+"�A�o�� "+msg+"�C");
	
	destruct(this_object());
	
	me->save();
}

void create()
{
	set_idname(HIR"bug red envelope"NOR, NOR YEL"���j "HIY"BUG "NOR YEL"�^��"HIR"���y"NOR RED"���]"NOR);
	
	if( this_object()->set_shadow_database() ) return;
	
	set("unit", "��");
	set("long", "���j BUG �^�����y���], ���Q�����@�����|�i�H��o�S����~");
	set("mass", 100);
	set("value", -1);
	set("flag/no_amount", 1);

	actions = ([ "open_envelope" : (: do_open_envelope :) ]);
}
