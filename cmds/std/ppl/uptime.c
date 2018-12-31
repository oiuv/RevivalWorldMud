/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : uptime.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-2
 * Note   : uptime ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <mudlib.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�d�߱Ұʮɶ�

�o�ӫ��O�i�H�d�߭��ͪ��@�ɶZ���Ұʪ��ɶ�

�������O:
HELP;

private void do_command(object me, string arg)
{
	tell(me, MUD_FULL_NAME"\n�Ұʤ���G" + TIME_D->time_description("����", TIME_D->analyse_time(time()-uptime())) + "�C\n", CMDMSG);
	tell(me, "�B�@�ɶ��G"+CHINESE_D->chinese_period(uptime())+"�C\n", CMDMSG);
}