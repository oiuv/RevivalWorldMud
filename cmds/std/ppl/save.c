/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : save.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-6
 * Note   : save ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�s�ɫ��O

�t�Ωw�ɷ|���ϥΪ̦s�ɡA���F�קK�t�εo�ͷN�~�ɵL�k�Y�ɦs��
�o�ӫ��O�i�H���ϥΪ̦ۦ�s��

�C�쪱�a���O���}�n���ߺD�H�ɦs�ɡA�H�קK��ƷN�~�y��

�������O:
HELP;

private void do_command(object me, string arg)
{
	int time = time() - query_temp("last_save_time", me);
	
	if( time < 60 )
		return tell(me, "�A "+time+" ��e�~�x�s�L�C\n", CMDMSG);
	
	set_temp("last_save_time", time(), me);
	
	if( me->save() )
		tell(me, me->query_idname()+"����x�s�����C\n", CMDMSG);
	else
		tell(me, me->query_idname()+"����x�s���ѡA�гq���Ův�B�z�C\n", CMDMSG);
}