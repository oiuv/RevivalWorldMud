/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chfn.c
 * Author : Sinji@RevivalWorld
 * Date   : 2006-09-07
 * Note   : chfn ���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
#include <ansi.h>

#define CH_ID		(1<<0)
#define CH_NAME		(1<<1)
#define CH_EMAIL	(1<<2)

#define DEFAULT		CH_ID | CH_NAME | CH_EMAIL
#define HELP		"/doc/help/color"

inherit COMMAND;

string help = @HELP
���ϥΪ̸�T

�����O�i�Ψӧ��ϥΪ̸�T�p�U�G

-i	�ק缾�a�^��N����m
-n	�ק缾�a�W�٤Φ�m
-e	�ק缾�a�q�l�l��H�c

�p���[�Ѽƫh��ܥ����ק�

���O�榡:
chfn [-i] [-n] [-e]

�������O: finger, nick
HELP;

private void change_prompt(object me, int flag);
private void change_input(object me, int flag, string arg);
private void do_command(object me, string arg)
{
	int flag;
	if( arg )
	{
		if( sscanf(arg, "%*s-i%*s", arg) == 2 )	flag |= CH_ID;
		if( sscanf(arg, "%*s-n%*s", arg) == 2 )	flag |= CH_NAME;
		if( sscanf(arg, "%*s-e%*s", arg) == 2 ) flag |= CH_EMAIL;
	}
	if( !flag ) flag = DEFAULT;
	tell(me, "���L�п�J [ENTER]�A�����п�J '~q'�A��m��U�п�J '~h'\n");
	tell(me, WHT"["HIW"Change Information"NOR + WHT"]�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR);
	change_prompt(me, flag);
}
private void change_prompt(object me, int flag)
{
	if( flag & CH_ID )
	{
		tell(me, "\nID - [" + me->query_id() + "]\n");
		tell(me, "�п�J" + pnoun(2, me) + "���m��^��N���G");
		input_to( (: change_input, me, flag :), 2 );
	}
	else if( flag & CH_NAME )
	{
		tell(me, "\nNAME - [" + me->query_name() + "]\n");
		tell(me, "�п�J" + pnoun(2, me) + "���m��W�١G");
		input_to( (: change_input, me, flag :), 2 );
	}
	else if( flag & CH_EMAIL )
	{
		tell(me, HIR"\n�`�N�G�H�c��}�N�@������������ƥ��ɮפ��̾ڡC\n"NOR);
		tell(me, "EMAIL - [" + query("email", me) + "]\n");
		tell(me, "�п�J" + pnoun(2, me) + "���q�l�l��H�c�G");
		input_to( (: change_input, me, flag :), 2 );
	}
	return;
}
private void change_input(object me, int flag, string arg)
{
	string *res;

	arg = remove_ansi(arg);

	if( arg && query("encode/gb", me) ) arg = G2B(arg);

	if( !arg ) arg = "";

	if( arg == "~q" )
	{
		tell(me, "������J�C\n");
		return me->finish_input();
	}
	if( arg == "~h" )
	{
		tell(me, read_file(HELP));
		return change_prompt(me, flag);
	}
	if( flag & CH_ID )
	{
		if( arg == "" )
			tell(me, "���L�m��^��N���]�w�C\n");
		else
		{
			if( !(res = me->set_idname(ansi(arg), 0)) )
			{
				tell(me, "�s�]�w���^��N���P�즳�^��N�����P�A�Э��s�]�w�C\n");
				return change_prompt(me, flag);
			}
			tell(me, "�m��^��N�� '"+ res[0] +"' �]�w�����C\n");
		}
		flag ^= CH_ID;
	}
	else if( flag & CH_NAME )
	{
		int len;

		if( arg == "" )
			tell(me, "���L�m��W�ٳ]�w�C\n");
		else
		{
			arg = ansi(arg);
			len = noansi_strlen(arg);
			if( len > 12 || len < 2 )
			{
				tell(me, "�W�٥����b 1 �� 12 �Ӧr�������A�Э��s�]�w�C\n");
				return change_prompt(me, flag);
			}
			foreach(int i in remove_ansi(arg))
			{
				if( i <= ' ' )
				{
					tell(me, "�Фũl�α���X�ΪťաA�Э��s�]�w�C\n");
					return change_prompt(me, flag);
				}
			}
			res = me->set_idname(0, ansi(arg));
			tell(me, "�m��W�� '" + res[1] + "' �]�w�����C\n");
		}
		flag ^= CH_NAME;
	}
	else if( flag & CH_EMAIL )
	{
		if( arg == "" )
			tell(me, "���L�q�l�l��H�c�]�w�C\n");
		else
		{
			if( sscanf(arg, "%*s@%*s.%*s") != 3 )
			{
				tell(me, "���H�c��}�榡�����D�C\n");
				return change_prompt(me, flag);
			}
			if( !set("email", arg, me) )
				tell(me, "�q�l�l��H�c '" + arg + "' �]�w���ѡC\n");
			else
				tell(me, "�q�l�l��H�c '" + arg + "' �]�w�����C\n");
		}
		flag ^= CH_EMAIL;
	}
	if( flag ) return change_prompt(me, flag);
	return me->finish_input();
}
