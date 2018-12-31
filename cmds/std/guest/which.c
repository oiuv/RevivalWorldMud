/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : which.c
 * Author : Ekac@RevivalWorld
 * Date   : 2003-03-04
 * Note   : �M��Y���O�Ҧb��m�C
 * Update :
 *  o 2003-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
        �з� which ���O�C
HELP;

private void command(object me, string arg)
{
	function fp;
	object cmd_ob;
	string cmd_file;
	if( !is_command() ) return;

	if( !arg )
		return tell(me, "���O�榡�Gwhich ���O\n");

	if( functionp(fp = COMMAND_D->find_wiz_command(arg)) &&
	    objectp(cmd_ob = function_owner(fp)) &&
	    strlen( cmd_file = base_name(cmd_ob) ) &&
	    cmd_file[0..5] == "/cmds/" )
		return tell(me, sprintf("���O %s ��� %s\n", arg, cmd_file) );

	return tell(me, "���|���S���� "+arg+" �����O�C\n");
}
