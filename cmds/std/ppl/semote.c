/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : semote.c
 * Author : Sinji@RevivalWorld
 * Date   : 2002-09-12
 * Note   : ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <emotion.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

#define BAR	"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"

string help = @HELP
���d�߫��O

�o�ӫ��O�|�C�X�ثe�Ҧ��i�H�ϥΪ������O

���O�榡:
semote
semote -t		�d�ݪ����O���ϥαƦ��
semote -l		�d�ݪ����O��²������
semote <���N��>	�d�� <���N��> ���ԲӪ��T��


�������O: command
HELP;

private string replace_keys(string description)
{
	description = replace_string(description, "$ME", HIC"�A"NOR);
	description = replace_string(description, "$YOU", HIG"�Y�H"NOR);
	description = replace_string(description, "$T", HIM"�ƪ�"NOR);

	return description;
}

private void do_command(object me, string arg)
{
	string msg;

	if( arg == "-t" )
	{
		int i;
		string *emotions = EMOTE_D->query_emotions();
		mapping emotion_sort = allocate_mapping(sizeof(emotions));

		foreach( string e in emotions )
		emotion_sort[e] = EMOTE_D->query_emotion(e)[USE];

		emotions = sort_array(emotions, (: count( $(emotion_sort)[$1],">",$(emotion_sort)[$2] )? -1:1:));

		msg = "�����O�ϥαƦW:\n";
		foreach( string e in emotions )
			msg += sprintf("%4s %-20s %-10s ��\n", HIW+(++i)+"."NOR, e, emotion_sort[e]);

		return me->more(msg+"\n");
	}
	else if( arg == "-l" )
	{
		int i;
		string *emotions = EMOTE_D->query_emotions();
		mapping emotion_sort = allocate_mapping(sizeof(emotions));

		foreach( string e in emotions )
		emotion_sort[e] = EMOTE_D->query_emotion(e)[NOTE];

		emotions = sort_array(emotions, (: strcmp($1, $2) :));

		msg = "�����O²������\n";
		foreach( string e in emotions )
		{
			msg += sprintf("%4s %-20s %-10s\n", HIW+(++i)+"."NOR, e, emotion_sort[e]||WHT"�L²��"NOR);
		}
		return me->more(msg+"\n");
	}
	else if( arg )
	{
		array emotion = EMOTE_D->query_emotion(arg);

		if( undefinedp(emotion) )
			return tell(me, "�S�� " + arg + " �o�Ӫ����O�C\n", CMDMSG);

		msg = "�� "+arg+" ���Բӱԭz�p�U�G\n"BAR;

		msg += NOR WHT"1.�L��H�L���[��r    �G"NOR+replace_keys(emotion[NONE_ALL])+"\n";
		msg += NOR WHT"2.�L��H�����[��r    �G"NOR+replace_keys(emotion[NONE_ALL_ARG])+"\n";
		msg += NOR WHT"3.��H���ۤv�L���[��r�G"NOR+replace_keys(emotion[ME_ALL])+"\n";
		msg += NOR WHT"4.��H���ۤv�����[��r�G"NOR+replace_keys(emotion[ME_ALL_ARG])+"\n";
		msg += NOR WHT"5.��H���Y�H�L���[��r�G"NOR+replace_keys(emotion[YOU_ALL])+"\n";
		msg += NOR WHT"6.��H���Y�H�����[��r�G"NOR+replace_keys(emotion[YOU_ALL_ARG])+"\n";
		msg += NOR WHT"7.������            �G"NOR+(emotion[NOTE] || "�L����")+"\n";
		msg += NOR WHT"8.�ϥΦ���            �G"NOR+emotion[USE]+" ��\n";
	}
	else
	{
		int size;
		string *emotions = sort_array(EMOTE_D->query_emotions(), 1);

		size = sizeof(emotions);
		msg = "�ثe�@�� "+size+" �ت����O�i�H�ϥΡA�C��p�U�G\n"BAR;

		msg += sprintf("%-=80s", sprintf("%@-:10s", emotions)) + "\n";
		msg += "\n�п�J semote <���N��> �[�ݸԲӱԭz�C\n";
	}
	me->more(msg + BAR);
}
