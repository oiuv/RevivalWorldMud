/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : time.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-29
 * Note   : time ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�d�߮ɶ�

�o�ӫ��O�i�H�d�߭��ͪ��@�ɤ�������P�ɶ�

�������O: date
HELP;

private void do_command(object me, string arg)
{
       	tell(me, TIME_D->game_time_description()+"�C\n", CMDMSG);
}
