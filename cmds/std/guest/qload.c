/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : qload.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-10
 * Note   : qload ���O
 * Update :
 *  o 2003-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>

inherit COMMAND;

string help = @HELP
	�з� qload ���O�C
HELP;

private void command(object me, string arg)
{
       	if( !is_command() ) return;
       	
	tell(me, SYSTEM_D->query_total_system_info()+"\n");
}
