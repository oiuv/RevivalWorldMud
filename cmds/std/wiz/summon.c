/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : summon.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-24
 * Note   : summon 指令。
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        標準 summon 指令。
HELP;

private void command(object me, string arg)
{
	object you;
	
	if( !is_command() ) return;
		
	if( !arg )
		return tell(me, pnoun(2, me)+"想要把誰傳送過來？\n", CMDMSG);
		
	if( !objectp(you = find_player(arg)) )
		return tell(me, "沒有 "+arg+" 這位玩家。\n", CMDMSG);
		
	if( !objectp(environment(me)) )
		return tell(me, pnoun(2, me)+"正處於虛幻時空之中，無法將別人傳送到"+pnoun(2, me)+"身處的環境。\n", CMDMSG);
	
	msg("$ME的身旁突然燃起一陣藍火，漸漸的$ME的身體愈來愈模糊，最後徹底煙消雲散。\n", you, 0, 1);
	
	you->move_to_environment(me);
	
	msg("$ME伸起雙手，穩穩地將從天邊飛來的"+you->query_idname()+"接住。\n", me, 0, 1, ENVMSG);
}