/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : moon_cake.c
 * Author : Kyo@RevivalWorld
 * Date   : 2006-10-07
 * Note   : ����`���
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
	"/obj/festival/mooncake/cake_glass",
	"/obj/festival/mooncake/rabbit_ear",
	"/obj/festival/mooncake/wukang_axe",
	"/obj/festival/mooncake/shaddock_hat",
	"/obj/festival/mooncake/hairpin",
});

void do_eat_cake(object me, string arg)
{
	object award;
	string msg;

	award = new(awards[random(sizeof(awards))]);

	msg = award->query_idname();
	
	log_file("command/mooncake", me->query_idname()+"��o"+msg);
	
	CHANNEL_D->channel_broadcast("news", me->query_idname()+"�r�F�X�f"+this_object()->query_idname()+"��A�o�{�̭��]�ۡu"+msg+"�v�C");
	
	award->set_keep();
	award->move(me);

	me->save();

	destruct(this_object());
}

void create()
{
	set_idname("moon cake", HIC"����"NOR+CYN"�`"HIW"���"NOR);
	
	if( this_object()->set_shadow_database() ) return;
	
	set("unit", "��");
	set("long", "����`���");
	set("mass", 100);
	set("value", 100);
	set("flag/no_amount", 1);

	actions = ([ "eat_cake" : (: do_eat_cake :) ]);
}
