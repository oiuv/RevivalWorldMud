/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : say.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : say ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
    �ΨӻP�P�@�Ӧ�m�����a�̽͸�
HELP;

private void do_command(object me, string arg)
{
	if( !arg )
		return msg("$ME�ۨ��ۻy�����D�b���Ǥ���C\n", me, 0, 1);

	msg("$ME���D: "HIG+arg+NOR"\n", me, 0, 1);
	
	me->add_msg_log("say", pnoun(2, me)+"���D: "HIG+arg+NOR"\n");
	(present_objects(me)-({ me }))->add_msg_log("say", me->query_idname()+"���D: "HIG+arg+NOR"\n");
	
	//if( !wizardp(me) )
	//	log_file("command/say", me->query_idname(1)+"���D: "+arg, -1);
}