/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _living.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-12-17
 * Note   : 生物繼承檔
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <buff.h>

// 啟動生命特性
nomask void startup_living()
{
	if( !userp(this_object()) )
		CHAR_D->create_npc(this_object());

	set_heart_beat(1);
}

nomask int is_living()
{
	return 1;
}

int query_inventory_slot()
{
	return this_object()->query_str()*5;
}


nomask int query_loading()
{
	int loading, mass;

	foreach(object ob in all_inventory(this_object()))
	{
		mass = query("mass", ob);

		if( mass > 0 )
			loading += to_int(count(mass, "*", query_temp("amount", ob)||1));
	}

	return loading;
}

nomask int query_max_loading()
{
	return 1000*(5*(this_object()->query_str()) + this_object()->query_condition_buff(BUFF_LOADING_MAX) + this_object()->query_equipment_buff(BUFF_LOADING_MAX));
}

varargs nomask int get_object(object ob, mixed amount)
{
	int mass = query("mass", ob);

	// 若超過負重則 return 0
	if( mass == -1 || count(count(amount||1,"*",mass),">", query_max_loading()-query_loading()) )
		return 0;

	// 若物品種類過多 return 0
	if( count(count(sizeof(all_inventory()), "+", query("flag/no_amount", ob) ? amount : 1), ">", query_inventory_slot()) )
		return 0;

	ob->move(this_object(), amount);
	return 1;
}

nomask int available_get(object ob, mixed amount)
{
	int mass = query("mass", ob);

	// 若超過負重則 return 0
	if( mass == -1 || count(count(amount||1,"*",mass),">", query_max_loading()-query_loading()) )
		return 0;

	return 1;
}


// 暈倒
nomask void faint()
{
	// 不管如何一暈倒就將所有能力值降為 0
	this_object()->set_all_ability_zero();

	COMBAT_D->stop_fight(this_object());

	msg("\e[1;31m$ME\e[1;31m突然眼前一堆小星星，昏倒了。\n\e[m", this_object(), 0, 1);

	set("faint", 1);
	set_temp("disable/msg", 1);
	set_temp("disable/cmd", 1);
}

// 甦醒
nomask void wakeup()
{
	delete("faint");
	delete_temp("disable/msg");
	delete_temp("disable/cmd");

	msg("$ME逐漸地恢復了意識。\n", this_object(), 0, 1);
	this_object()->show_msg_buffer();
	this_object()->show_prompt();
}

// 是否暈倒
nomask int is_faint()
{
	return query("faint");
}

// 死亡
nomask void die()
{
	this_object()->set_all_ability_zero();

	set("die", 1);
	set_temp("disable/msg", 1);
	set_temp("disable/cmd", 1);
}

// 是否死亡
nomask int is_dead()
{
	return query("die");
}
