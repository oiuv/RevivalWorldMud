/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : tell.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-02
 * Note   : tell ���O
 * Update :
 *  o 2000-00-00  
 *  o �� say �C��Ӫ�`�d�V�����C��
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
���q���O

�o�ӫ��O�i�H���z�P�ثe���b�u�W����L�ϥΪ̷��q�C

���O�榡:
tell				�d�ߤW�@��V�z���ܪ��ϥΪ�
tell <�ϥΪ̥N��> <�T��>	�P��L�ϥΪ̻���
tell -r <�T��>			�^�ФW�@��V�z���ܪ��ϥΪ�

tell -r ���w�]���O�O�W�� reply

�������O:
HELP;

#define WARNING_IDLE_TIME	300

private void do_command(object me, string arg)
{
	string target, msg, *ignore;
	object target_ob;
	int reply;

	if( !arg || sscanf(arg, "%s %s", target, msg) != 2 )  
	{
		if( !(target = query_temp("reply_teller", me)) )
			return tell(me, "��~�èS������H��A���L�ܡC\n", CMDMSG);

		target_ob = find_player(target);

		return tell(me, "�W���A���ܪ��ϥΪ̬�: " + (target_ob ? target_ob->query_idname():target + " [�w���u]") + "\n", CMDMSG);
	}

	if( target == "-r" )
	{
		if( !(target = query_temp("reply_teller", me)) )
			return tell(me, "��~�èS������H��A���L�ܡC\n", CMDMSG);
		reply = 1;
	}

	if( !target_ob = find_player(target) )
		return tell(me, "�ثe�u�W�S�� " + target + " �o��ϥΪ̡C\n", CMDMSG);

	ignore = query("deny_user", target_ob);

	if( arrayp(ignore) )
	{
		if( member_array( me->query_id(1), ignore ) >= 0 )
			return tell( me, HIG + target_ob->query_idname()+ "�{�b�ڵ���A���� .\n"NOR);
	}

	if( target_ob == me )
		return tell(me, pnoun(2, me)+"��ۦۤv��䪺�ۨ��ۻy�C\n", CMDMSG);

	msg("$ME�i�D$YOU�G"HIY+msg+NOR+"\n", me, target_ob, 0, TELLMSG);
	target_ob->add_last_msg(me->query_idname()+"�i�D"+pnoun(2, target_ob)+"�G"HIY+msg+NOR+"\n");
	me->add_last_msg(pnoun(2, me)+"�i�D"+target_ob->query_idname()+"�G"HIY+msg+NOR+"\n");

	if( !interactive(target_ob) ) 
		return tell(me, target_ob->query_idname()+"�w�g�_�u�F�Ať����"+pnoun(2, me)+"���ܡC\n", CMDMSG);

	if( !wizardp(me) && !wizardp(target_ob) )
	{
		log_file("command/tell", me->query_idname(1)+"�i�D"+target_ob->query_idname()+": "+msg, -1);
		//CHANNEL_D->channel_broadcast("nch", me->query_idname(1)+"�i�D"+obj->query_idname()+"�G"+msg+"\n");
	}
	if( query_idle(target_ob) > WARNING_IDLE_TIME )
		tell(me, target_ob->query_idname()+"�w�g�o�b�W�L "+(WARNING_IDLE_TIME/60)+" �����A�i��Ȯɤ��|�^��"+pnoun(2, me)+"�C\n", CMDMSG);

	set_temp("reply_teller", me->query_id(1), target_ob);
}
