/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ff.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-6-7
 * Note   : ff 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	標準 ff 指令。
HELP;

private void command(object me, string arg)
{

	object ob;
	string dest, func, file;

	if( !is_command() ) return;

	if( !arg || sscanf(arg, "%s %s", dest, func)!=2 )
		return tell(me, "指令格式﹕ff <物件> <函數名稱>\n", CMDMSG);	
		
	if( dest=="me" ) ob = me;
	else if( dest=="here" ) ob = environment(me);
	else ob = present(dest, me);
	
	if( !ob ) ob = present(dest, environment(me));
	if( !ob ) ob = find_object(resolve_path(me, dest));
	if( !ob ) return tell(me, "這裡沒有「" + dest + "」。\n", CMDMSG);
	
        file = function_exists(func, ob, 1);
	
	if(!file)
		tell(me, sprintf("物件 %O 並沒有定義 %s 這個函數。\n", ob, func), CMDMSG);
	else
		tell(me, sprintf("物件 %O 的 %s 函數定義在 %s.c。\n", ob, func, file ), CMDMSG);
}	
