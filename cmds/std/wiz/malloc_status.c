/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : malloc_status.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-01
 * Note   : malloc_status ���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        �з� malloc_status ���O�C
HELP;

private void command(object me, string arg)
{
	if( !is_command() ) return;
	
	tell(me, "\n"+malloc_status()+"\n", CMDMSG);
}
