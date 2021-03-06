/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : history.c
 * Author : Cwlin
 * Date   : 2002-8-20
 * Note   : history 指令
 * Update :
 *  o 2003-04-22 Clode rewrite
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <ansi.h>
#include <feature.h>
#include <message.h>

#define HCMD		0
#define HTIME		1

#define DEFAULT_SIZE	25

inherit COMMAND;

string help = @HELP
指令歷程

這個指令用來檢視您曾使用過的指令紀錄，您可以透過下列指令來使用您的指令紀錄

!!		重複上一個指令
!<指令編號>	重複 <指令編號> 的指令

指令格式:
history <指令數>		顯示最近 <指令數> 筆的指令歷程
history <使用者代號>		顯示 <使用者> 的指令歷程 (巫師專用)
history <指令數> <使用者代號>	顯示 <使用者> 最近 <指令數> 筆的指令歷程 (巫師專用)
history <使用者代號> <指令數>	同上

相關指令: 
HELP;

private void do_command(object me, string arg)
{
	int i, ttl_size, size, start;
	string msg;
	array cmd_history;
	object ob;

	if( wizardp(me) && arg == "-l" )
	{
		object *sortusers;
		
		msg = "排名 玩家名稱                   指令數   速度(每秒)   五百平均 系統負荷\n";
		msg += WHT"───────────────────────────────────────\n"NOR;
		sortusers = filter_array(users(), (: $1->is_user_ob() :));
		sortusers = sort_array(sortusers, (: to_float($1->total_command_count())/(time() - query_temp("login/time", $1)) < to_float($2->total_command_count())/(time() - query_temp("login/time", $2)) ? 1 : -1 :));

		foreach(ob in sortusers)
		{
			cmd_history = ob->query_history();
			
			msg += sprintf("%-3d. %-27s%-9d%-13.2f%-8.2f %-8s %s\n", 
				++i, 
				ob->query_idname(), 
				ob->total_command_count(), 
				to_float(ob->total_command_count())/(time() - query_temp("login/time", ob)),
				sizeof(cmd_history) ? to_float(sizeof(cmd_history))/((cmd_history[<1][1] - cmd_history[0][1])||1) : 0.,
				count(ob->query_timecost_record(), "/", ob->total_command_count()||1),
				query("cmdlimit", ob) ? HIR"(Limited)"NOR:"",
			);

		}
		msg += WHT"───────────────────────────────────────\n"NOR;
		me->more(msg);
		return;
	}
	if( !arg || !wizardp(me) ) ob = me;
	else if( sscanf(arg, "%d %s", size, arg) == 2 ) ob = find_player(arg);
	else if( sscanf(arg, "%s %d", arg, size) == 2 ) ob = find_player(arg);
	else if( sscanf(arg, "%d", size) == 1 ) ob = me;
	else ob = find_player(arg);

	if( !objectp(ob) )
		return tell(me, "沒有 " + arg + " 這位玩家。\n");

	if( ob != me && SECURE_D->level_num(me->query_id(1)) < SECURE_D->level_num(ob->query_id(1)) )
		return tell(me, pnoun(2, me) + "沒有權限查閱" + ob->query_idname() + "的歷史指令紀錄。\n");

	cmd_history = ob->query_history();

	if( !(ttl_size = sizeof(cmd_history)) )
		return tell(me, (me==ob ? pnoun(2, me):ob->query_idname()) + "沒有任何歷史指令紀錄。\n");

	if( size <= 0 ) size = DEFAULT_SIZE;
	if( size > ttl_size ) size = ttl_size;
	
        msg = (me==ob ? pnoun(2, me):ob->query_idname())+"總共有 "+ttl_size+" 筆指紀錄，指令平均速度 "+(ob->total_command_count() > 0 ? sprintf("%.2f", to_float(ob->total_command_count())/((time() - query_temp("login/time", ob))||1)) : 0)+" (指令數/秒)，下列為最近的 "+size+" 筆: \n"; 

	start = ttl_size - size;

	i = ob->total_command_count() - size;
	foreach(array cmd in cmd_history[start..])
		msg += sprintf(HIW" [%4d]"NOR + WHT" %s"NOR" %s\n"NOR, (++i), ctime(cmd[HTIME])[11..18], cmd[HCMD]);

	me->more(msg);
}
