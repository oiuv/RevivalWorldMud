/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : reset_include.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-16
 * Note   : reset_include ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� reset_include ���O�C
HELP;

#define CREATOR	"/system/kernel/creator.c"

private void command(object me, string arg)
{	
	if( !is_command() ) return;
	
	CREATOR->create_all_include();
	
	tell(me, "���s��z���Y�ɧ����C\n", CMDMSG);
}