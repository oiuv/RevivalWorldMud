/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : version.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-4-20
 * Note   : version ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <mudlib.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�d�ߨt�Ϊ���

�o�ӫ��O�i�H�d�ߥثe���ͪ��@�ɩҨϥΪ��{���w���X�ʵ{������
HELP;

private void do_command(object me, string arg)
{
	string str;
	
	str =  "�C���W�� : "+MUD_FULL_NAME+"\n";
	str += "�{���w   : "+MUDLIB_NAME+" "+MUDLIB_VERSION_NUMBER+"\n";
	str += "�X�ʵ{�� : "+__VERSION__+" on "+__ARCH__+"\n";
	tell(me, str, CMDMSG);
}