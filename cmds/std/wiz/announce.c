/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : announce.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-9-12
 * Note   : announce ���O
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
���O�榡�Gannounce <���i�T��>

�i�H�缾�a�o�X�j����i�T���A�H�q�����a
���n�ƶ��C
HELP;

private void command(object me, string arg)
{
	if( !is_command() ) return;
	if( !arg )
		return tell(me, "���O�榡�Gannounce <���i�T��>\n", CMDMSG);
	CHANNEL_D->announce(me, arg);
	return;
}