/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : editnews.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-00-00
 * Note   : 編輯新聞
 * Update :
 *  o 2003-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

#define TIME		0
#define MESSAGE		1

inherit COMMAND;

string help = @HELP
	標準 editnews 指令。
HELP;

private void command(object me, string arg)
{
	string option;
       	
       	if( !is_command() ) return;
       	
       	if( arg == "-l" )
       	{
       		int i;
       		string msg = "目前的新聞資訊如下：\n";
       		
       		foreach(array news in NEWS_D->query_news())
       			msg += (++i)+". ("+TIME_D->replace_ctime(news[TIME])[0..4]+") "+news[MESSAGE]+NOR+"\n";
       			
       		return tell(me, msg+"\n");
	}
      	
       	if( !arg || sscanf(arg, "-%s %s", option, arg) != 2 )
                return tell(me, "editnews [-l | -a 訊息 | -d 編號]\n");
       	
       	switch( option )
       	{
       		case "a":
			if( NEWS_D->add_news(arg) )
			{
				tell(me, "新增新聞完成。\n");
				NEWS_D->broadcast_latest_news();
			}
			else
				tell(me, "新增新聞失敗。\n");
			break;
		case "1":
			CHANNEL_D->channel_broadcast("news", HIG"單次公告 "+NOR GRN"("+TIME_D->replace_ctime(time())[0..4]+") "+arg+NOR);
			break;
		case "d":
		{
			int i = to_int(arg);
			
			if( !i )
				return tell(me, "請輸入要刪除的新聞編號。\n");
				
			if( !NEWS_D->remove_news(i-1) )
				tell(me, "請輸入正確的新聞編號。\n");
			else
				tell(me, "刪除新聞完成。\n");
			break;
		}
	}
}
