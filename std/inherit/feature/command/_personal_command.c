/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _personal_command.c
 * Author : Abcd@RevivalWorld
 * Date   : 2005-1-12
 * Note   : �ӤH���O�~���ɮ�
 * Update :
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
        // ���ϥξ������v���ˬd -.-
        return (: do_command($1, $2) :);
}
