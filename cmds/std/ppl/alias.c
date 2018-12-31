/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : alias.c
 * Author : Sinji@RevivalWorld
 * Date   : 2001-07-10
 * Note   : ���O�O�W���N
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

#define MAX_ALIASES	50

inherit COMMAND;

string help = @HELP
�O�W���O

�ϥΪ̥i�۩w���O���O�W�A��N�@���ꪺ���O�Y�u��²�u���O����r
�b�]�w�����O���A�i�H�[�J $* �� $# �������O�O�W���Ѽ�

 # �N�� 1 �� N �ӫ��O�O�W�Ѽƪ��Ӽ�
 * �N��Ҧ��Ѽ�
 
�p $1, $2, $3 ���O�N����O�O�W���Ĥ@�B�ĤG�B�ĤT�ѼƥH�ťհϹj

�d�ҤΥΪk�p�U:

> alias da date
�]�w���O date ���O�W�� da�Ada �P date

> alias ch chat $1 and $2
�]�w chat ���O�W�� ch�Ach hello world �P chat hello and world 

> alias ch chat $*
�]�w chat ���O�W�� ch�Ach hello world �P chat hello world 

> alias -d ch
�R�����O�O�W ch 

���O�榡:
alias			��ܩҦ��ۭq�����O�O�W
alias -d		��ܩҦ��t�ιw�]�����O�O�W
alias -d <���O�O�W>	�R���w�ۭq�����O�O�W
alias <�O�W> <���O>	�]�w���O <���O> �O�W�� <�O�W>
alias <�ϥΪ̥N��>	�d�ݨϥΪ̪����O�O�W (�Ův�M��)

�������O: chfn, set, 
HELP;

private void do_command(object me, string arg)
{
	object target;
	string verb, cmd;
	int detail;

	if( arg )
	{
		if( arg == "-d" ) // ��ܸԲӪ����O�O�W
		{
			detail = 1;
			target = me;
		}
		else if( wizardp(me) && objectp(find_player(arg)) )
			target = find_player(arg);
	}
	else target = me;

	// ��ܥؼЪ����O�O�W
	if( objectp(target) )
	{
		mapping alias;
		string msg, pnoun;
		int i;

		if( !detail )
			alias = target->query_alias();
		else
			alias = COMMAND_D->query_default_alias();

		pnoun = target == me ? pnoun(2, me) : target->query_idname();

		if( !(i = sizeof(alias)) ) 
			return tell(me, pnoun + "�å��]�w������O�O�W�C\n");

		msg = pnoun + "�ثe�@�� " + i + " �ի��O�O�W�p�U: \n";

		foreach(verb in sort_array(keys(alias), 1))
			msg += sprintf(WHT" ["NOR HIW"%12|s"NOR WHT"]"NOR" %s\n", verb, alias[verb]);

		return me->more(msg);
	}
	// �]�w/�R�� ���O�O�W
	if( sscanf(arg, "%s %s", verb, cmd) != 2 || verb == "alias" )
		return tell(me, "���O�榡���~�A�аѦ� help alias �H���o��h���ϥθ�T�C\n");

	// �R��
	if( verb == "-d" )
	{
		if( undefinedp(me->query_alias()[cmd]) )
			return tell(me, pnoun(2, me) + "�èS���]�w "+ cmd +" �o�ӫ��O�O�W�C\n");

		me->del_alias(cmd);
		return tell(me, "���O�O�W " + cmd + " �R�������C\n");
	}
	// �]�w
	if( sizeof(me->query_alias()) > MAX_ALIASES )
		return tell(me, "�z�����O�O�W�ƶq�v�F�W���C");

	if( !undefinedp(COMMAND_D->query_default_alias()[verb]) )
		return tell(me, "���O " + verb + " �w�s�b��w�]�����O�O�W���A�аѦ� alias -d�C\n");

	if( me->set_alias(verb, cmd) ) 
		tell(me, "�]�w���O " + verb + " �O�W�� " + cmd + " �����C\n");
}