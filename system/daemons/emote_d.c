/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : emote_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-28
 * Note   : �����F
 * Update :
 *  o 2002-09-12 Sinji rewrite emote_d
 *  o 2002-09-13 Clode rewrite emote_d again
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <emotion.h>
#include <daemon.h>
#include <message.h>

#define MSG		1
#define RAW		2

#define DATA		"/data/emotion/emotion"+ __SAVE_EXTENSION__

private mapping emotions;

string help = @HELP
���������O�A�ШϥΫ��O semote �Ӭd�\�����O�C��ΨϥΤ�k�C
HELP;
varargs private string do_emote(object me, string verb, string arg, int option);

int is_emote(string emt)
{
	return member_array(emt, keys(emotions)) != -1;
}

private string multi_emote(object me, string arg, int option)
{
	string *args;

	if( option == RAW ) return arg;

	if( sizeof(args = explode(arg, " ")) > 1 )
		arg = do_emote(me, args[0], implode(args[1..], " "), RAW) || arg;
	else
		arg = do_emote(me, arg, 0, RAW) || arg;

	return arg;
}

varargs private string do_emote(object me, string verb, string arg, int option)
{
	object you;
	array *emotion;
	string emotion_msg, your_name, argument, ignore;

	if( !me || !verb || !(emotion = emotions[verb]) ) return 0;

	/* �L��H && �L���[��r */
	if ( !arg || !arg[0] )
		emotion_msg = emotion[NONE_ALL];

	/* �Y�N arg ��ⳡ�� */
	else if( sscanf(arg, "%s %s", your_name, argument) == 2 )
	{
		int which;

		// �ˬd�Ƽƥؼ�
		if( sscanf(argument, "%d", which) && which > 1 && objectp(you = present(your_name+" "+which, me) || present(your_name+" "+which, environment(me))) )
		{
			your_name += " "+which;
			argument = remove_fringe_blanks(argument)[strlen(which+"")+1..];
		}

		if( !objectp(you) )
			you =  present(your_name, environment(me)) || find_player(your_name);

		if( objectp(you) && interactive(you))
		{	ignore = query("deny_user", you);
			if( arrayp(ignore) )
			{
				if( member_array( me->query_id(1), ignore ) >= 0 )
				{
					tell(me, "�A�{�b�L�k��"+ you->query_idname() +"�ϥΪ����O .\n" +NOR);
					return 0;
				}
			}
		}
		/* ����H && �����[��r */
		if( objectp(you) )
		{
			/* ��H�O�ۤv */
			if( you == me )
			{
				emotion_msg = argument == "" ? emotion[ME_ALL] : emotion[ME_ALL_ARG];
				you = 0;
			}
			else
				emotion_msg = argument == "" ? emotion[YOU_ALL] : emotion[YOU_ALL_ARG];

			emotion_msg = replace_string(emotion_msg, "$T", multi_emote(me, argument, option)+NOR);
		}
		/* �L��H && �����[��r */
		else
			emotion_msg = replace_string(emotion[NONE_ALL_ARG], "$T", multi_emote(me, arg, option)+NOR);
	}
	/* ����H && �L���[��r */
	else if( objectp(you = present(arg, environment(me)) || find_player(arg)) )
	{
		if( you == me )
		{
			emotion_msg = emotion[ME_ALL];
			you = 0;
		}
		else
			emotion_msg = emotion[YOU_ALL];
	}

	/* �L��H && �����[��r */
	else
		emotion_msg = replace_string(emotion[NONE_ALL_ARG], "$T", multi_emote(me, arg, option)+NOR);

	// �p��ϥΦ���
	emotion[USE] = count(emotion[USE],"+",1);

	if( option == MSG )
	{
		emotion_msg = replace_string(emotion_msg, "$ME", me->query_idname());

		if( objectp(you) )
			emotion_msg = replace_string(emotion_msg, "$YOU", you->query_idname());

		return emotion_msg+"\n";
	}
	else if( option == RAW )
	{
		if( objectp(you) )
			emotion_msg = replace_string(emotion_msg, "$YOU", you->query_idname());

		return emotion_msg;
	}
	else
	{
		if( objectp(you) && interactive(you))
		{	ignore = query("deny_user", you);
			if( arrayp(ignore) )
			{
				if( member_array( me->query_id(1), ignore ) >= 0 )
				{
					tell(me, "�A�{�b�L�k��"+ you->query_idname() +"�ϥΪ����O .\n" +NOR);
					return 0;
				}
			}
		}

		if( !objectp(you) || member_array(you, present_objects(me) + all_inventory(me) +({me})) != -1 )
			msg(kill_repeat_ansi(emotion_msg)+"\n", me, you, 1, "EMOTE");
		else
			msg(HIB"����"NOR"-"+kill_repeat_ansi(emotion_msg)+"\n", me, you, 1, "EMOTE");
	}
}

mapping query_emotions_fpointer()
{
	mapping m = allocate_mapping(sizeof(emotions));

	if( previous_object() != load_object(COMMAND_D) )
		return 0;

	foreach( string c in keys(emotions) )
	m[c] = (: do_emote($1, $(c), $2, $3) :);

	return m;
}

int set_emotion(string emotion, array info)
{
	/*
		if( sizeof(info) != 8 || !is_command(previous_object()) ) return 0;
	*/

	emotions[emotion] = info;

	return save_object(DATA);
}

string *query_emotions()
{
	return keys(emotions);
}

array query_emotion(string m)
{
	return copy(emotions[m]);
}

int delete_emotion(string m)
{
	if( !is_command(previous_object()) ) return 0;

	map_delete(emotions, m);
	return save_object(DATA);
}

private void create()
{
	if( clonep() ) return;

	emotions = allocate_mapping(0);

	if( !restore_object(DATA) )
	{
		log_file("daemon/emote","�L�k���J�s�ɡC\n");
		destruct(this_object());
	}

	COMMAND_D->do_reset();
}

void save()
{
	save_object(DATA);
}

int remove()
{
	return save_object(DATA);
}

string query_name()
{
	return "���t��(EMOTE_D)";
}
