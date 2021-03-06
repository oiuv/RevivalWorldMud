/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : tune.c
 * Author : Sinji@RevivalWorld
 * Date   : 2001-4-22
 * Note   : tune 指令
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

string help = @HELP
頻道指令

在重生的世界中頻道可以讓您與其他所有玩家聯繫及溝通
您可以透過這個指令自由選擇您要 開啟/關閉 的通訊頻道
目前的頻道分類如下:

	chat	閒聊頻道	sys	系統頻道 (巫師專用)
	music	音樂頻道	wiz	巫師頻道 (巫師專用)
	rumor	謠言頻道	twiz	台灣巫師頻道 (巫師專用)
	ad	廣告頻道	gwiz	網際巫師頻道 (巫師專用)
				nch	監測頻道 (巫師專用)
				other	其他頻道 (巫師專用)

指令格式:
tune			查詢目前收聽的頻道
tune <頻道代號>		開啟或關閉 <頻道代號>
tune -l <頻道代號>	查詢目前收聽 <頻道代號> 的所有使用者 (巫師專用)

相關指令:

相關說明: channels
HELP;

class channel_class
{
	int level, number;
	string msg;
	mapping extra;
}

private void do_command(object me, string arg)
{
	int my_channels = query("channels", me), flag;
	mapping channels = fetch_variable("channels",load_object(CHANNEL_D));
	class channel_class ch;
	
	if( !arg )
		return tell(me, "你正在使用中的頻道有："+CHANNEL_D->query_using_channel(me)+"。\n");

	if( wizardp(me) && sscanf(arg, "-l %s", arg) ) flag = 1;

	if( undefinedp(ch = channels[arg]) || !classp(ch) )
		return tell(me, "沒有這個頻道。\n");

	if( !(ch->number & my_channels) && (SECURE_D->level_num(me->query_id(1)) < ch->level) )
		return tell(me, "沒有這個頻道。\n");
		
	if( flag )
	{
		string *names;
		names = CHANNEL_D->query_channel_users(arg)->query_id(1);
		names = sort_array(names, 1);
		tell(me, "目前收聽 " + arg + " 頻道的共 " + sizeof(names) + " 人如下：\n");
		tell(me, sprintf("%-=80s", implode(names, ", ")) + ".\n");
		
		names = CHANNEL_D->query_channel_users(arg, 1)->query_id(1);
		names = sort_array(names, 1);
		tell(me, "其餘 " + sizeof(names) + " 人如下：\n");
		tell(me, sprintf("%-=80s", implode(names, ", ")) + ".\n");
		return;
	}

	if( ch->number & my_channels )
	{
		set("channels", my_channels ^ ch->number, me);
		CHANNEL_D->remove_register(me, ch->number);
		tell(me, "關閉 "+arg+" 頻道。\n");
	}
	else
	{
		set("channels", my_channels | ch->number, me);
		CHANNEL_D->register_channel(me, ch->number);
		tell(me, "開啟 "+arg+" 頻道。\n"); 
	}
	
}