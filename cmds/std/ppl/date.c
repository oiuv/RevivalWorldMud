/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : realtime.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-7-4
 * Note   : realtime ���O
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
�d�ߤ��

�o�ӫ��O�i�H�d�߯u��@�ɪ�����P�ɶ�

�������O: time
HELP;

private void do_command(object me, string arg)
{
	tell(me, TIME_D->real_time_description()+" ( "+TIME_D->replace_ctime(time())+" )�C\n", CMDMSG);
}
