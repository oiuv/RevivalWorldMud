/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : bug.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : bug ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

#define PER_REPORT_TIME_LIMIT	60

#define LIST_OPT_ALL	(1<<0)

string help = @HELP

bug -a				�s�W bug �^��
bug -l				�C�X bug ���
bug [�s��]			Ū�� bug �ԲӤ��e
bug -r [�s��]			�^�� bug �B�z����	�Ův�M��
bug -d [�s��]   		�R�� bug �Ҧ����	�Ův�M��
bug -d [�s��] reply [�s��]	�R�� bug ���Y�@�g�^��	�Ův�M��
bug				Ū�����~�^��		�Ův�M��
bug [ID]			Ū���Y�H�����~�^��	�Ův�M��

HELP;

void edit_bug_content(object me, string title, string content)
{
	string number;
	
	if( time() - query_temp("bug_report", me) < PER_REPORT_TIME_LIMIT )
		return tell(me, pnoun(2, me)+"�����A�j "+(PER_REPORT_TIME_LIMIT + query_temp("bug_report", me) - time())+" ���~��A�^���@�� Bug�C\n"NOR);
	
	number = BUG_D->add_bug(me, title, content);
	
	tell(me, HIY"�P��"+pnoun(2, me)+"�� Bug �^���ϱo�o�ӥ@�ɧ�[�����C\n"NOR);
	tell(me, HIY"\n�w�N"+pnoun(2, me)+"�Ҧ^���� Bug �s�J��Ʈw���A�s���� "+number+"�A�ЧQ�� bug -l �H�ɪ`�N�B�z���p�C\n"NOR);
	
	set_temp("bug_report", time(), me);
	
	me->finish_input();
}

void input_bug_title(object me, string title)
{
	if( !title || !title[0] )
	{
		tell(me, "���� Bug �^����J�C\n");
		return me->finish_input();
	}
	
	if( noansi_strlen(title) > 30 )
	{
		tell(me, "�^���D�D���o�W�L 30 �Ӧr���C\n");
		return me->finish_input();
	}
	
	if( query("encode/gb", me) )
		title = G2B(title);

	tell(me, pnoun(2, me)+"�ҿ�J�� Bug �^���D�D���G�u"+title+NOR+"�v\n");
	tell(me, HIY"�п�J���^���� Bug �ԲӤ��e�C\n"NOR);
	me->edit( (: edit_bug_content, me, title :) );
}	

void edit_reply_content(object me, string number, string status, string content)
{
	BUG_D->reply_bug(me, number, status, content);
	
	tell(me, "�^���s���� "+number+" �� Bug �����C\n");
	
	me->finish_input();
}

void confirm_input_reply(object me, string number, string status, string confirm)
{
	if( stringp(confirm) )
		confirm = lower_case(confirm);
		
	switch(confirm)
	{
		case "yes":
		case "y":
			tell(me, HIY"�п�J�B�z�ԲӤ��e�C\n"NOR);
			me->edit( (: edit_reply_content, me, number, status :) );
			return; 
		default:
			tell(me, HIY"����J�B�z�ԲӤ��e�C\n"NOR);
			BUG_D->reply_bug(me, number, status);
			me->finish_input();
			break;
	}
}
	

void input_reply_status(object me, string number, string arg)
{
	string status;

	switch(arg)
	{
		case "1":	status = HIR"���B�z"NOR;	break;
		case "2":	status = HIY"�B�z��"NOR;	break;
		case "3":	status = HIC"�w�״_"NOR;	break;
		case "4":	status = HIR"�L�k�״_"NOR;	break;
		case "5":	status = HIW"���ݳB�z"NOR;	break;
		case "6":	status = HIM"�ݦA�^��"NOR;	break;
		default:	status = HIR"���B�z"NOR;	break;
	}
			
	tell(me, "�O�_��J�B�z�ԲӤ��e�H(Yes/No):");
	input_to( (: confirm_input_reply, me, number, status :) );
}

private void command(object me, string arg)
{
	string number;

	if( !is_command() ) return;
	
	if( wizardp(me) )
	{
		if( !arg || !arg[0] )
			return me->more(query_temp("bug_msg", me) || pnoun(2, me)+"���W�S�����~�^����ơC\n");
		
		else if( find_player(arg) )
			return me->more(query_temp("bug_msg", find_player(arg)) || find_player(arg)->query_idname()+"���W�S�����~�^����ơC\n");
		
		else if( sscanf(arg, "-r %s", number) )
		{
			if( !BUG_D->bug_exists(number) )
				return tell(me, "�èS���s�� "+number+" �� Bug �s�b�C\n"NOR);
				
			tell(me, HIY"�п�J�ثe�B�z���p�s���G\n[1]"HIR"���B�z"NOR" [2]"HIY"�B�z��"NOR" [3]"HIC"�w�״_"NOR" [4]"HIR"�L�k�״_"NOR" [5]"HIW"���ݳB�z"NOR" [6]"HIM"�ݦA�^��"NOR"\n:");
			input_to( (: input_reply_status, me, number :) );
			return;
		}
		else if( sscanf(arg, "-d %s", number) )
		{
			int reply;
	
			sscanf(number, "%s reply %d", number, reply);
			
			if( !BUG_D->bug_exists(number) )
				return tell(me, "�èS���s�� "+number+" �� Bug �s�b�C\n"NOR);

			if( reply > 0 )
			{
				BUG_D->remove_bug(me, number, reply);
				tell(me, "�R���s���� "+number+" �� Bug ���� "+reply+" �g�^���C\n"NOR);
			}
			else
			{
				BUG_D->remove_bug(me, number);
				tell(me, "�R���s���� "+number+" �� Bug ���Ҧ���ơC\n"NOR);
			}

			return;
		}
	}
	
	if( !arg )
		return tell(me, help);
	
	else if( arg == "-l" )
		return me->more(BUG_D->list_bug(1));
		
	else if( arg == "-a" )
	{
		tell(me, HIW"�w��ϥ�"WHT"����(Bug)�^���t��\n"NOR HIG"�^�� Bug �ɽиԭz�o�ͮɶ��B�a�Ϯy�Цa�I�B�U�F�L�����O�P�t����ܪ��T��\n�ԲӪ���ƱN�i�H���U�Ův�ֳt�ѨM"+pnoun(2, me)+"�J�쪺 Bug�C\n"NOR);
		tell(me, HIY"�п�J���^���� Bug �D�D(�ȥD�D�A�D�ԲӤ��e)�A�Ϊ����� Enter ������J�C\n:"NOR);
		input_to( (: input_bug_title, me :) );
		return;
	}
	else if( big_number_check(arg) )
	{
		if( !BUG_D->bug_exists(arg) )
			return tell(me, "�èS���s�� "+arg+" �� Bug �s�b�C\n"NOR);

		tell(me, BUG_D->query_bug(arg));
	}
	else return tell(me, help);
}