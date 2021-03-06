/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : fight.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-26
 * Note   : fight 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>

inherit COMMAND;

string help = @HELP
與人戰鬥的指令

fight '目標'		- 和某人開始戰鬥

HELP;

private void do_command(object me, string arg)
{
	object enemy;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"想要和誰戰鬥？\n");
		
	if( arg == "-all" )
	{
		object enemies = filter_array(present_objects(me), (: $1 != $(me) && $1->is_living() && !$1->is_faint() && !query("no_fight", $1) :));
			
		if( sizeof(enemies) )
		foreach(enemy in  enemies)
		{
			msg(HIY"$ME"HIY"開始對$YOU"HIY"進行攻擊。\n"NOR, me, enemy, 1);
			
			// 嗶嗶警告
			//if( userp(enemy) )
			//	tell(enemy, BEEP);
		
			COMBAT_D->start_fight(me, enemy);
		}
		else
			return tell(me, "這裡沒有任何可以進行攻擊的目標。\n");

		return;
	}
	else if( arg == "-stop" )
	{
		if( !COMBAT_D->in_fight(me) )
			return tell(me, pnoun(2, me)+"並沒有在戰鬥中。\n");
		
		msg("$ME停止戰鬥。\n", me, 0, 1);
		COMBAT_D->stop_fight(me);
		return;
	}
	
	if( !objectp(enemy = present(arg, environment(me))) )
		return tell(me, "這裡沒有 "+arg+" 這個人。\n");

	if( enemy == me )
		return tell(me, pnoun(2, me)+"不能和自己戰鬥。\n");

	if( !enemy->is_living() )
		return tell(me, enemy->query_idname()+"不是生物，"+pnoun(2, me)+"沒辦法和"+pnoun(3, enemy)+"戰鬥。\n");

	if( enemy->is_faint() )
		return tell(me, enemy->query_idname()+"已經沒有能力跟"+pnoun(2, me)+"戰鬥了。\n");

	if( COMBAT_D->is_fighting(me, enemy) )
		return tell(me, pnoun(2, me)+"和"+enemy->query_idname()+"已經在戰鬥中。\n");
		
	if( !wizardp(me) )
		return tell(me, "戰鬥功能測試中。\n");

	if( query("no_fight", enemy) )
		return tell(me, pnoun(2, me)+"無法對"+enemy->query_idname()+"進行攻擊。\n");
	
	msg(HIY"$ME"HIY"開始向$YOU"HIY"進行攻擊。\n"NOR, me, enemy, 1);
	
	// 嗶嗶警告
	//if( userp(enemy) )
	//	tell(enemy, BEEP);

	COMBAT_D->start_fight(me, enemy);
}
