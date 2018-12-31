/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : check_memory.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-01
 * Note   : check_memory ���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        �з� check_memory ���O�C
HELP;

private void command(object me, string arg)
{
	if( !is_command() ) return;
	
#ifdef CHECK_MEMORY
	me->more(remove_ansi(check_memory(to_int(arg))));
#else
	tell(me, "�ثe Driver ���䴩���\��C\n", CMDMSG);
#endif
}