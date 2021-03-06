/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : bug.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : bug 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

#define PER_REPORT_TIME_LIMIT	60

#define LIST_OPT_ALL	(1<<0)

string help = @HELP

bug -a				新增 bug 回報
bug -l				列出 bug 資料
bug [編號]			讀取 bug 詳細內容
bug -r [編號]			回應 bug 處理情形	巫師專用
bug -d [編號]   		刪除 bug 所有資料	巫師專用
bug -d [編號] reply [編號]	刪除 bug 中某一篇回應	巫師專用
bug				讀取錯誤回溯		巫師專用
bug [ID]			讀取某人的錯誤回溯	巫師專用

HELP;

void edit_bug_content(object me, string title, string content)
{
	string number;
	
	if( time() - query_temp("bug_report", me) < PER_REPORT_TIME_LIMIT )
		return tell(me, pnoun(2, me)+"必須再隔 "+(PER_REPORT_TIME_LIMIT + query_temp("bug_report", me) - time())+" 秒後才能再回報一次 Bug。\n"NOR);
	
	number = BUG_D->add_bug(me, title, content);
	
	tell(me, HIY"感謝"+pnoun(2, me)+"的 Bug 回報使得這個世界更加完美。\n"NOR);
	tell(me, HIY"\n已將"+pnoun(2, me)+"所回報的 Bug 存入資料庫中，編號為 "+number+"，請利用 bug -l 隨時注意處理狀況。\n"NOR);
	
	set_temp("bug_report", time(), me);
	
	me->finish_input();
}

void input_bug_title(object me, string title)
{
	if( !title || !title[0] )
	{
		tell(me, "取消 Bug 回報輸入。\n");
		return me->finish_input();
	}
	
	if( noansi_strlen(title) > 30 )
	{
		tell(me, "回報主題不得超過 30 個字元。\n");
		return me->finish_input();
	}
	
	if( query("encode/gb", me) )
		title = G2B(title);

	tell(me, pnoun(2, me)+"所輸入的 Bug 回報主題為：「"+title+NOR+"」\n");
	tell(me, HIY"請輸入欲回報的 Bug 詳細內容。\n"NOR);
	me->edit( (: edit_bug_content, me, title :) );
}	

void edit_reply_content(object me, string number, string status, string content)
{
	BUG_D->reply_bug(me, number, status, content);
	
	tell(me, "回應編號第 "+number+" 號 Bug 完畢。\n");
	
	me->finish_input();
}

void confirm_input_reply(object me, string number, string status, string confirm)
{
	if( stringp(confirm) )
		confirm = lower_case(confirm);
		
	switch(confirm)
	{
		case "yes":
		case "y":
			tell(me, HIY"請輸入處理詳細內容。\n"NOR);
			me->edit( (: edit_reply_content, me, number, status :) );
			return; 
		default:
			tell(me, HIY"不輸入處理詳細內容。\n"NOR);
			BUG_D->reply_bug(me, number, status);
			me->finish_input();
			break;
	}
}
	

void input_reply_status(object me, string number, string arg)
{
	string status;

	switch(arg)
	{
		case "1":	status = HIR"未處理"NOR;	break;
		case "2":	status = HIY"處理中"NOR;	break;
		case "3":	status = HIC"已修復"NOR;	break;
		case "4":	status = HIR"無法修復"NOR;	break;
		case "5":	status = HIW"不需處理"NOR;	break;
		case "6":	status = HIM"需再回報"NOR;	break;
		default:	status = HIR"未處理"NOR;	break;
	}
			
	tell(me, "是否輸入處理詳細內容？(Yes/No):");
	input_to( (: confirm_input_reply, me, number, status :) );
}

private void command(object me, string arg)
{
	string number;

	if( !is_command() ) return;
	
	if( wizardp(me) )
	{
		if( !arg || !arg[0] )
			return me->more(query_temp("bug_msg", me) || pnoun(2, me)+"身上沒有錯誤回溯資料。\n");
		
		else if( find_player(arg) )
			return me->more(query_temp("bug_msg", find_player(arg)) || find_player(arg)->query_idname()+"身上沒有錯誤回溯資料。\n");
		
		else if( sscanf(arg, "-r %s", number) )
		{
			if( !BUG_D->bug_exists(number) )
				return tell(me, "並沒有編號 "+number+" 的 Bug 存在。\n"NOR);
				
			tell(me, HIY"請輸入目前處理狀況編號：\n[1]"HIR"未處理"NOR" [2]"HIY"處理中"NOR" [3]"HIC"已修復"NOR" [4]"HIR"無法修復"NOR" [5]"HIW"不需處理"NOR" [6]"HIM"需再回報"NOR"\n:");
			input_to( (: input_reply_status, me, number :) );
			return;
		}
		else if( sscanf(arg, "-d %s", number) )
		{
			int reply;
	
			sscanf(number, "%s reply %d", number, reply);
			
			if( !BUG_D->bug_exists(number) )
				return tell(me, "並沒有編號 "+number+" 的 Bug 存在。\n"NOR);

			if( reply > 0 )
			{
				BUG_D->remove_bug(me, number, reply);
				tell(me, "刪除編號第 "+number+" 號 Bug 的第 "+reply+" 篇回應。\n"NOR);
			}
			else
			{
				BUG_D->remove_bug(me, number);
				tell(me, "刪除編號第 "+number+" 號 Bug 的所有資料。\n"NOR);
			}

			return;
		}
	}
	
	if( !arg )
		return tell(me, help);
	
	else if( arg == "-l" )
		return me->more(BUG_D->list_bug(1));
		
	else if( arg == "-a" )
	{
		tell(me, HIW"歡迎使用"WHT"臭蟲(Bug)回報系統\n"NOR HIG"回報 Bug 時請詳述發生時間、地圖座標地點、下達過的指令與系統顯示的訊息\n詳細的資料將可以幫助巫師快速解決"+pnoun(2, me)+"遇到的 Bug。\n"NOR);
		tell(me, HIY"請輸入欲回報的 Bug 主題(僅主題，非詳細內容)，或直接按 Enter 取消輸入。\n:"NOR);
		input_to( (: input_bug_title, me :) );
		return;
	}
	else if( big_number_check(arg) )
	{
		if( !BUG_D->bug_exists(arg) )
			return tell(me, "並沒有編號 "+arg+" 的 Bug 存在。\n"NOR);

		tell(me, BUG_D->query_bug(arg));
	}
	else return tell(me, help);
}