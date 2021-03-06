/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cd.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : cd 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	標準 cd 指令。
HELP;

private void command(object me, string arg)
{
	if( !is_command() ) return;
	
	if( !arg ) arg = "~";
	
	arg = resolve_path(me, arg);
	
	add_slash(ref arg);
	
	if( file_size(arg) == -2 )
	{
		set("current_work/directory", arg, me);
		tell(me, "目錄："+arg+"\n", CMDMSG);
	}
	else
		tell(me, "沒有 "+arg+" 這個目錄。\n", CMDMSG);
}