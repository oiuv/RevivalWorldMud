/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : warning.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-29
 * Note   : warning 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <ansi.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	warning [玩家id] [multi]
HELP;

private void command(object me, string arg)
{
	object target;
	string option;

	if( !is_command() ) return;

	if( !arg || sscanf(arg, "%s %s", arg, option) != 2 )
		return tell(me, pnoun(2, me)+"想要對誰警告何事？\n", CMDMSG);
	
	if( !objectp(target = find_player(arg)) )
		return tell(me, "沒有 "+arg+" 這個人。\n", CMDMSG);

	switch( option )
	{
		case "multi":
		{
			int indeed_multi;
			string target_ip = query_ip_number(target);

			foreach( object user in users() - ({ target }) )
				if( query_ip_number(user) == target_ip )
				{
					indeed_multi++;
					tell(target, BEEP+HIR+"\n系統警告："+user->query_idname()+HIR"與"+pnoun(2, target)+"重覆 IP 登入遊戲，請立即在公開頻道提出解釋(例:使用IP分享器)，否則將處以罰則。\n\n"NOR, CMDMSG);
					tell(user, BEEP+HIR+"\n系統警告："+target->query_idname()+HIR"與"+pnoun(2, user)+"重覆 IP 登入遊戲，請立即在公開頻道提出解釋(例:使用IP分享器)，否則將處以罰則。\n\n"NOR, CMDMSG);
				}
			
			if( indeed_multi )
			{
				tell(me, "已送出對"+target->query_idname()+"的相同 IP 重覆登入警告，共有 "+(indeed_multi+1)+" 個相同 IP 重覆登入。\n", CMDMSG);
				CHANNEL_D->channel_broadcast("nch", me->query_idname()+"已送出對"+target->query_idname()+"的相同 IP 重覆登入警告，共有 "+(indeed_multi+1)+" 個相同 IP 重覆登入。");
			}
			else
				tell(me, target->query_idname()+"並無重覆登入情形。\n", CMDMSG);
			break;
		}
		default:
			return tell(me, "請輸入正確的警告選項(multi)。\n", CMDMSG);
	}
}
