/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : nick.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-10-14
 * Note   : nick ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
���ӤH�︹�����O

nick '�︹'

HELP;

private void do_command(object me, string arg)
{
	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say ����Q�n�ڨ�����˪��︹�H\n");
		else
			return tell(me, pnoun(2, me)+"�Q�n������˪��︹�H\n");
	}

	if( lower_case(arg) == "-d" )
	{
		delete("nickname", me);
		me->save();
		return msg("$ME�N�︹�����F�C\n", me, 0, 1);
	}
	
	if( noansi_strlen(arg) > 16 )
	{
		if( me->is_npc() )
			return me->do_command("say �︹�̦h�u�঳ 16 �Ӧr����C\n");
		else
			return tell(me, "�︹�̦h�u�঳ 16 �Ӧr����C\n");
	}
	
	arg = kill_repeat_ansi(arg+NOR);
	
	set("nickname", arg, me);
	me->save();
	msg("$ME�N�︹�]�w���u"+arg+"�v�C\n", me, 0, 1);
}