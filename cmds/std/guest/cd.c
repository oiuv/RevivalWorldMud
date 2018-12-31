/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cd.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : cd ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� cd ���O�C
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
		tell(me, "�ؿ��G"+arg+"\n", CMDMSG);
	}
	else
		tell(me, "�S�� "+arg+" �o�ӥؿ��C\n", CMDMSG);
}