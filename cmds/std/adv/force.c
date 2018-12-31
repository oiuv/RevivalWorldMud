/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : force.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-02-05
 * Note   : force ���O
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
	�з� force ���O�C
HELP;

private void command(object me, string arg)
{
	string cmd;
	object you;
	if( !is_command() ) return;
	
        if( !arg || sscanf(arg,"%s to %s",arg, cmd) != 2 )
		return tell(me, "��J�榡: force �H�� to ���O�C\n", CMDMSG);
	
	you = find_player(arg) || present(arg);
	
	if( !objectp(you) || !you->is_living() )
		return tell(me, "�S�� "+arg+" �o�ӤH�C\n", CMDMSG);

	if( !userp(you) && you->is_npc() )
	{
		you->do_command(cmd);
		return;
	}

	if( SECURE_D->level_num(me->query_id(1)) < SECURE_D->level_num(you->query_id(1)) )
		return tell(me, "�A�S���v���j�� "+you->query_idname()+" ������O�C\n", CMDMSG);
		
	you->force_me(cmd);
}
