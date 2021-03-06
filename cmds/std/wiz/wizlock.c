/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wizlock.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-06-21
 * Note   : wizlock 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <daemon.h>
#include <secure.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	標準 wizlock 指令。
HELP;

private void command(object me, string arg)
{
	int level_num, my_level_num;
	
	if( !is_command() ) return;
	
	if( !arg ) 
		return tell(me, "目前允許權限在 "+SECURE_D->level_num_to_level_name(LOGIN_D->query_wiz_lock())+" 以上的使用者連線。\n", CMDMSG);
	
	my_level_num = SECURE_D->level_num(me->query_id(1));
	
	if( !(level_num = SECURE_D->level_name_to_level_num(arg)) )
		return tell(me, "沒有 "+arg+" 這個等級。\n", CMDMSG);
	
	if( my_level_num < level_num )
		return tell(me, pnoun(2,me)+"的權限不足以設定這樣的登入權限。\n", CMDMSG);
	
	if( level_num == LOGIN_D->query_wiz_lock() )
		return tell(me, "目前的登入權限已經設定為 "+arg+"。\n", CMDMSG);
	
	LOGIN_D->set_wiz_lock(level_num);
	
	if( level_num > PLAYER )
		CHANNEL_D->announce(me, "系統更新維護中，目前僅允許權限在 "+arg+" 以上的使用者連線。\n");
	else
		CHANNEL_D->announce(me, "系統維護完畢，解除登入權限限制。\n");
		
}	