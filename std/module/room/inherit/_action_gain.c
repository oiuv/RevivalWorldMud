/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _gain_action.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-23
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>

// ��Q
void do_gain(object me, string arg)
{
	string env_city, money;
	object env;
	
	env = environment(me)->query_master();
	
	if( query("owner", environment(me)) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"�L�k�����������C\n");
	
	env_city = env->query_city();
	
	money = query("money", env);
	
	if( !money || count(money, "==", 0) )
		return msg("$ME��۪ťժ��i�b�A���Ѫ��y�U�T�w�\���C\n"NOR, me, 0, 1);

	me->earn_money(MONEY_D->city_to_money_unit(env_city), money);
	
	msg("$ME�w�ѳߦa���N��o���Q���F�_�ӡA�@�o"+HIY+QUANTITY_D->money_info(env_city, money)+NOR"�C\n"NOR, me, 0, 1);
	
	delete("money", env);
	//log_file("command/gain", me->query_idname()+"�����Q�� "+QUANTITY_D->money_info(env_city, money));
	// hmm....
	env->save();
}
