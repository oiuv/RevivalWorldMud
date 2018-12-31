/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : tune.c
 * Author : Sinji@RevivalWorld
 * Date   : 2001-4-22
 * Note   : tune ���O
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

string help = @HELP
�W�D���O

�b���ͪ��@�ɤ��W�D�i�H���z�P��L�Ҧ����a�pô�η��q
�z�i�H�z�L�o�ӫ��O�ۥѿ�ܱz�n �}��/���� ���q�T�W�D
�ثe���W�D�����p�U:

	chat	�����W�D	sys	�t���W�D (�Ův�M��)
	music	�����W�D	wiz	�Ův�W�D (�Ův�M��)
	rumor	�����W�D	twiz	�x�W�Ův�W�D (�Ův�M��)
	ad	�s�i�W�D	gwiz	���ڧŮv�W�D (�Ův�M��)
				nch	�ʴ��W�D (�Ův�M��)
				other	��L�W�D (�Ův�M��)

���O�榡:
tune			�d�ߥثe��ť���W�D
tune <�W�D�N��>		�}�ҩ����� <�W�D�N��>
tune -l <�W�D�N��>	�d�ߥثe��ť <�W�D�N��> ���Ҧ��ϥΪ� (�Ův�M��)

�������O:

��������: channels
HELP;

class channel_class
{
	int level, number;
	string msg;
	mapping extra;
}

private void do_command(object me, string arg)
{
	int my_channels = query("channels", me), flag;
	mapping channels = fetch_variable("channels",load_object(CHANNEL_D));
	class channel_class ch;
	
	if( !arg )
		return tell(me, "�A���b�ϥΤ����W�D���G"+CHANNEL_D->query_using_channel(me)+"�C\n");

	if( wizardp(me) && sscanf(arg, "-l %s", arg) ) flag = 1;

	if( undefinedp(ch = channels[arg]) || !classp(ch) )
		return tell(me, "�S���o���W�D�C\n");

	if( !(ch->number & my_channels) && (SECURE_D->level_num(me->query_id(1)) < ch->level) )
		return tell(me, "�S���o���W�D�C\n");
		
	if( flag )
	{
		string *names;
		names = CHANNEL_D->query_channel_users(arg)->query_id(1);
		names = sort_array(names, 1);
		tell(me, "�ثe��ť " + arg + " �W�D���@ " + sizeof(names) + " �H�p�U�G\n");
		tell(me, sprintf("%-=80s", implode(names, ", ")) + ".\n");
		
		names = CHANNEL_D->query_channel_users(arg, 1)->query_id(1);
		names = sort_array(names, 1);
		tell(me, "��l " + sizeof(names) + " �H�p�U�G\n");
		tell(me, sprintf("%-=80s", implode(names, ", ")) + ".\n");
		return;
	}

	if( ch->number & my_channels )
	{
		set("channels", my_channels ^ ch->number, me);
		CHANNEL_D->remove_register(me, ch->number);
		tell(me, "���� "+arg+" �W�D�C\n");
	}
	else
	{
		set("channels", my_channels | ch->number, me);
		CHANNEL_D->register_channel(me, ch->number);
		tell(me, "�}�� "+arg+" �W�D�C\n"); 
	}
	
}