/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : passwd.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-30
 * Note   : 
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <secure.h>

inherit COMMAND;

string help = @HELP
�K�X�ק���O

�o�ӫ��O�i�H�ק�ϥΪ̱K�X

�ЦU���a�ȥ��O����L�᪺�K�X�ïd�U���T���ϥΪ̸�T
�H�K��K�X�򥢮ɡA�i�H�z�L���T���ϥΪ̸�T�V�Ův���^����
�p���ק�ϥΪ̸�T�A�Ա��� help chfn �H���o��h����T

�������O:
HELP;

private void redo_passwd(object me);

private void confirm_new_passwd(object me, string new_passwd, string arg)
{
	/* �קK�Q�~�� */
	if( !is_command() ) return;
	
	tell(me, repeat_string("*", strlen(arg))+"\n");
	
	if( !arg || crypt(arg, new_passwd) != new_passwd )
	{
		tell(me, "�K�X�T�{���ѡC\n");
		return redo_passwd(me);
	}
	
	if( !PASSWORD_D->set_password(me->query_id(1), new_passwd) )
	{
		tell(me, "�K�X�]�w���ѡA�гq���Ův�B�z�C\n");
		return me->finish_input();
	}
	
	tell(me, "�i�� MD5 �s�X�t�νs�X:"+new_passwd+"\n�K�X�s�X�x�s�����C\n"+me->query_idname()+"���K�X�w��s�A�s�K�X�ߧY�ͮġC\n");
	me->finish_input();
}
	
private void input_new_passwd(object me, string arg)
{
	if( !is_command() ) return;
	
	tell(me, repeat_string("*", strlen(arg))+"\n");
	
	tell(me, "�ЦA��J�@���T�{�G\n>");
	input_to( (: confirm_new_passwd, me, crypt(arg, 0) :), 3 );
}

private void check_old_passwd(object me, string arg)
{
	string old_passwd = PASSWORD_D->query_password(me->query_id(1));
	
	tell(me, repeat_string("*", strlen(arg))+"\n");
	
	if( !old_passwd ) 
	{
		tell(me, "�±K�X���j�M���G�o�Ϳ��~�A�гq���Ův�B�z�C\n");
		return me->finish_input();
	}
	
	if( !arg || crypt(arg, old_passwd) != old_passwd )
	{
		tell(me, "�K�X��J���~�C\n");
		return redo_passwd(me);
	}
	
	tell(me, "�п�J�s���K�X�G\n>");
	input_to( (: input_new_passwd, me :), 3 );
}

private void confirm_redo_passwd(object me, string arg)
{
	if( !arg ) return redo_passwd(me);
	arg = lower_case(arg);
	
	if( arg[0] == 'y' )
	{
		tell(me, "\n�Х���J�z�쥻���K�X�G\n>");
		input_to( (: check_old_passwd, me :), 3 );
	}
	else 
	{
		tell(me, "\n�ܧ�K�X�����A�~��ϥέ�K�X�C\n");
		me->finish_input();
	}
}
	
private void redo_passwd(object me)
{
	tell(me, "\n�ק�K�X���ѡA�O�_���s�]�w(Yes/No)�H");
	input_to( (: confirm_redo_passwd, me :) );
}

private void do_command(object me, string arg)
{
	string id, newpasswd;

	if( call_stack(1)[<1] != me || this_player(1) != me ) return;
	
	if( arg && sscanf(arg, "%s %s",id, newpasswd) == 2 && SECURE_D->level_num(me->query_id(1)) >= ADVISOR )
	{
		newpasswd = remove_ansi(newpasswd);
		
		if( strlen(newpasswd) < 4 )
			return tell(me, "�K�X�r���ƥ����j�� 4�C\n");

		if( user_exists(id) )
		{
			if( !PASSWORD_D->set_password(id, crypt(newpasswd, 0)) )
				return tell(me, id+" �K�X�]�w���ѡC\n");
			else
				return tell(me, id+" �s�K�X�]�w�� "+newpasswd+"�C\n");
		}
	}
		
	tell(me, "�Х���J�z�쥻���K�X�G\n>");
	input_to( (: check_old_passwd, me :), 3 );
}