/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _command.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : ���O�~���ɮ�
 * Update :
 *  o 2001-08-01 Clode �Q�Ψ禡���а�����O
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <origin.h>

private void command(object me, string arg);

private void do_command(object me, string arg)
{
	if( call_stack(1)[<1] == me || call_stack(1)[<2] == me || call_stack(1)[4] == me && call_stack(2)[5] == "force_me" ) 
		command(me, arg);
	else
		printf("�v���ˬd:\n%O\n%O\n%O\n���O�ϥ��v�������A�Ы�ߺ޲z�̡C\n", call_stack(1), call_stack(2), call_stack(3));
}

nomask function query_fp()
{
	return is_daemon(previous_object()) ? (: do_command($1, $2) :) : 0;
}