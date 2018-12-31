/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : channel_status.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-4-23
 * Note   : channel_status ���O
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
	�з� channel_status ���O�C
HELP;

private void command(object me, string arg)
{
	string msg = "";

	if( !is_command() ) return;

        foreach( object user in filter_array(users(), (: environment($1) :)) )
//        foreach( object user in sort_array( filter_array(users(), (: environment($1) :)), (: SECURE_D->level_num($1->query_id(1)) > SECURE_D->level_num($2->query_id(1)) ? -1 : 1 :) ) )
	msg += sprintf("%-20s:%s\n", user->query_idname(), CHANNEL_D->query_using_channel(user));

	tell(me, "�Ҧ����a�ϥ��W�D����T�p�U�G\n"+msg+"\n", CMDMSG);
}
