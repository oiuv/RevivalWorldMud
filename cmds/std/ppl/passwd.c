/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : passwd.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-30
 * Note   : 
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <secure.h>

inherit COMMAND;

string help = @HELP
密碼修改指令

這個指令可以修改使用者密碼

請各玩家務必記住更改過後的密碼並留下正確的使用者資訊
以便當密碼遺失時，可以透過正確的使用者資訊向巫師取回角色
如欲修改使用者資訊，詳情請 help chfn 以取得更多的資訊

相關指令:
HELP;

private void redo_passwd(object me);

private void confirm_new_passwd(object me, string new_passwd, string arg)
{
	/* 避免被繼承 */
	if( !is_command() ) return;
	
	tell(me, repeat_string("*", strlen(arg))+"\n");
	
	if( !arg || crypt(arg, new_passwd) != new_passwd )
	{
		tell(me, "密碼確認失敗。\n");
		return redo_passwd(me);
	}
	
	if( !PASSWORD_D->set_password(me->query_id(1), new_passwd) )
	{
		tell(me, "密碼設定失敗，請通知巫師處理。\n");
		return me->finish_input();
	}
	
	tell(me, "進行 MD5 編碼系統編碼:"+new_passwd+"\n密碼編碼儲存完畢。\n"+me->query_idname()+"的密碼已更新，新密碼立即生效。\n");
	me->finish_input();
}
	
private void input_new_passwd(object me, string arg)
{
	if( !is_command() ) return;
	
	tell(me, repeat_string("*", strlen(arg))+"\n");
	
	tell(me, "請再輸入一次確認：\n>");
	input_to( (: confirm_new_passwd, me, crypt(arg, 0) :), 3 );
}

private void check_old_passwd(object me, string arg)
{
	string old_passwd = PASSWORD_D->query_password(me->query_id(1));
	
	tell(me, repeat_string("*", strlen(arg))+"\n");
	
	if( !old_passwd ) 
	{
		tell(me, "舊密碼之搜尋結果發生錯誤，請通知巫師處理。\n");
		return me->finish_input();
	}
	
	if( !arg || crypt(arg, old_passwd) != old_passwd )
	{
		tell(me, "密碼輸入錯誤。\n");
		return redo_passwd(me);
	}
	
	tell(me, "請輸入新的密碼：\n>");
	input_to( (: input_new_passwd, me :), 3 );
}

private void confirm_redo_passwd(object me, string arg)
{
	if( !arg ) return redo_passwd(me);
	arg = lower_case(arg);
	
	if( arg[0] == 'y' )
	{
		tell(me, "\n請先輸入您原本的密碼：\n>");
		input_to( (: check_old_passwd, me :), 3 );
	}
	else 
	{
		tell(me, "\n變更密碼取消，繼續使用原密碼。\n");
		me->finish_input();
	}
}
	
private void redo_passwd(object me)
{
	tell(me, "\n修改密碼失敗，是否重新設定(Yes/No)？");
	input_to( (: confirm_redo_passwd, me :) );
}

private void do_command(object me, string arg)
{
	string id, newpasswd;

	if( call_stack(1)[<1] != me || this_player(1) != me ) return;
	
	if( arg && sscanf(arg, "%s %s",id, newpasswd) == 2 && SECURE_D->level_num(me->query_id(1)) >= ADVISOR )
	{
		newpasswd = remove_ansi(newpasswd);
		
		if( strlen(newpasswd) < 4 )
			return tell(me, "密碼字元數必須大於 4。\n");

		if( user_exists(id) )
		{
			if( !PASSWORD_D->set_password(id, crypt(newpasswd, 0)) )
				return tell(me, id+" 密碼設定失敗。\n");
			else
				return tell(me, id+" 新密碼設定為 "+newpasswd+"。\n");
		}
	}
		
	tell(me, "請先輸入您原本的密碼：\n>");
	input_to( (: check_old_passwd, me :), 3 );
}