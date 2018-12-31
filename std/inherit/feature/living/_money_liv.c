/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _money.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-01
 * Note   : �����B�z�~����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <bank.h>
#include <daemon.h>

varargs int spend_money(string unit, mixed money, int autotransfer)
{
	if( this_object()->is_npc() ) return 0;

	return MONEY_D->spend_money(this_object()->query_id(1), unit, money, autotransfer);
}

varargs string earn_money(string unit, mixed money, int autotransfer)
{
	if( this_object()->is_npc() ) return 0;

	return MONEY_D->earn_money(this_object()->query_id(1), unit, money, autotransfer);
}

int drop_money(string unit, mixed money)
{
	string myid = this_object()->query_id(1);
	mapping moneydata = MONEY_D->query_moneydata(myid);
	
	if( !money || !unit || !mapp(moneydata) ) return 0;
	
	money = big_number_check(money);
	unit = upper_case(unit);
	
	if( count( money, "<=", 0 ) ) return 0;

	if( MONEY_D->spend_money(myid, unit, money) )
	{
		new("/obj/etc/money", unit, money)->move_to_environment(this_object());
	
		return 1;
	}
	return 0;
}

string current_money_unit()
{
	object env = environment(this_object());

	if( !env ) return 0;

	return MONEY_D->city_to_money_unit(query("city", env) || env->query_city() || query("city"));
}