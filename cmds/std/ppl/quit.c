/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : quit.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : quit ���O
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
���u���O

���}���ͪ��@��
HELP;

private void do_command(object me, string arg)
{
	if( me->is_fighting() )
		return tell(me,"�A���b�԰����A�L�k���u�I\n");
	if( me != this_player(1) ) 
		tell(me, this_player(1)->query_idname()+"�j���A���}�C���C\n", CMDMSG);

	if( me->is_delaying() )
		return tell(me, me->query_delay_msg(), CMDMSG);

     	catch{"/cmds/std/wiz/snoop.c"->remove_user(me);};

	LOGOUT_D->quit(me, arg);
}
