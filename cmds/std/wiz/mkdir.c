/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mkdir.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-8
 * Note   : mkdir ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� mkdir ���O�C
HELP;

private void command(object me, string arg)
{
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "�п�J���|�W�١C\n", CMDMSG);
	
	arg = resolve_path(me, arg);
		
	if( mkdir(arg) )
		tell(me, "�إߥؿ� "+arg+" �����C\n", CMDMSG);
	else
		tell(me, pnoun(2, me)+"�S���v�Q�إߥؿ� "+arg+"�C\n", CMDMSG);
}