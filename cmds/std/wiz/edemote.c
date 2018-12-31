/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : edemote.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-9-13
 * Note   : ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <emotion.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�����O�s�边

edemote '�����O'	- �s������O
edemote -d '�����O'	- �R�������O

HELP;

private int valid_emote_name(string emotion)
{
	return strsrch(emotion, " ") == -1;
}

private void choose_option(object me, string emotion, mapping modifying_emotion, string arg);
private void setup_emotion(object me, string emotion, mapping modifying_info, int option, string contiune_arg, string arg);

private string replace_keys(string description)
{
	if( !description ) return "�|���]�w";
	
	description = replace_string(description, "$ME", 	HIC"�A"NOR);
	description = replace_string(description, "$YOU", 	HIG"�Y�H"NOR);
	description = replace_string(description, "$T", 	HIM"�ƪ�"NOR);
	
	return description;
}
private string emotion_detail_description(string emotion, mixed info)
{
	string msg = "�w�w �� "HIY+emotion+NOR+" ���Բӱԭz�p�U�G"+repeat_string("�w", 13-(strlen(emotion)/2))+"\n";
	
	msg += NOR WHT"1.�L��H�L���[��r    �G"NOR+replace_keys(info[NONE_ALL])+"\n";
	msg += NOR WHT"2.�L��H�����[��r    �G"NOR+replace_keys(info[NONE_ALL_ARG])+"\n";
	msg += NOR WHT"3.��H���ۤv�L���[��r�G"NOR+replace_keys(info[ME_ALL])+"\n";
	msg += NOR WHT"4.��H���ۤv�����[��r�G"NOR+replace_keys(info[ME_ALL_ARG])+"\n";
	msg += NOR WHT"5.��H���Y�H�L���[��r�G"NOR+replace_keys(info[YOU_ALL])+"\n";
	msg += NOR WHT"6.��H���Y�H�����[��r�G"NOR+replace_keys(info[YOU_ALL_ARG])+"\n";
	msg += NOR WHT"7.������            �G"NOR+(info[NOTE] || "�|���]�w")+"\n";
	msg += NOR WHT"8.�ϥΦ���            �G"NOR+info[USE]+" ��\n";
	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
	return msg;
}

private void confirm_delete_emotion(object me, string emotion, string arg)
{
	if( lower_case(arg) == "y" )
	{
		if( EMOTE_D->delete_emotion(emotion) )
			tell(me, "�R�������O "+emotion+" ���\�C\n");
		else
			tell(me, "�R�����ѡA�гq���Ův�B�z�C\n");
		
		COMMAND_D->do_reset();
		return me->finish_input();
	}
	else
	{
		tell(me, "�����R���C\n");
		return me->finish_input();
	}
}

private void setup_emotion(object me, string emotion, mapping modifying_info, int option, string multi_line_arg, string arg)
{
	if( !arg || !arg[0] )
	{
		multi_line_arg += "\n";
		tell(me, ">");
		input_to( (: setup_emotion, me, emotion, modifying_info, option, multi_line_arg :) );
	}
	
	switch(arg)
	{
		case "cancel":
			tell(me, "������J�ԭz�C\n"+emotion_detail_description(emotion, modifying_info)+"�п�J���]�w������(�]�w���� save, �����]�w exit)�G\n>");
			input_to( (: choose_option, me, emotion, modifying_info :) );
			break;
		
		case ".":
			// �̨ΤƱԭz
			multi_line_arg = kill_repeat_ansi(remove_fringe_blanks(multi_line_arg) + NOR);
			modifying_info[option-1] = multi_line_arg;
			
			tell(me, "�]�w�����C\n"+emotion_detail_description(emotion, modifying_info)+"�п�J���]�w������(�]�w���� save, �����]�w exit)�G\n>");
			input_to( (: choose_option, me, emotion, modifying_info :) );
			break;
		default:
			multi_line_arg += arg+"\n";
			tell(me, ">");
			input_to( (: setup_emotion, me, emotion, modifying_info, option, multi_line_arg :) );
	}
}
	
private void choose_option(object me, string emotion, mapping modifying_info, string arg)
{
	int option;
	
	switch(arg)
	{
		case "save":
			if( sizeof(modifying_info) != 8 )
			{
				tell(me, "�����]�w���Ҧ����ԭz�~���x�s�]�w�C\n�п�J���]�w������(�]�w���� save, �����]�w exit)�G\n>");
				input_to( (: choose_option, me, emotion, modifying_info :) );
				return;
			}
			else
			{
				array info = allocate(0);
				
				for(int i=0;i<8;i++)
					info += ({ modifying_info[i] });
					
				if( EMOTE_D->set_emotion(emotion, info) )
				{
					tell(me, "�����O "+emotion+" �]�w�x�s���\�C\n");
					COMMAND_D->do_reset();
				}
				else
					tell(me, "�����O "+emotion+" �]�w�x�s���ѡC\n");
					
				return me->finish_input();
			}
			break;
			
		case "exit":
			tell(me, "������ "+emotion+" �����O���]�w�C\n");
			return me->finish_input();
			
		default:break;
	}
	
	option = to_int(arg);
	
	if( option > 8 || option < 1 )
	{
		tell(me, "�S���o�ӿﶵ�C\n�п�J���]�w������(�]�w���� save, �����]�w exit)�G\n>");
		input_to( (: choose_option, me, emotion, modifying_info :) );
	}
	else if( option == USE+1 ) 
	{
		tell(me, "�o�ӿﶵ�L�k�ϥΤ�ʳ]�w�C\n�п�J���]�w������(�]�w���� save, �����]�w exit)�G\n>");
		input_to( (: choose_option, me, emotion, modifying_info :) );
	}
	else
	{
		tell(me, "\n�� ENTER �i�����J�h��A��J '.' �����A��J 'cancel' �����C\n�п�J�зǮ榡�ԭz "HIC"$ME(�N��ۤv) "HIG"$YOU(�N���H) "HIY"$T(�N����[��r)"NOR"�G\n>");
		input_to( (: setup_emotion, me, emotion, modifying_info, option, "" :) );
	}
}
	
private void command(object me, string arg)
{
	array info;
	mapping modifying_info = allocate_mapping(8);
	
	if( !is_command() ) return;

	if( !arg ) return tell(me, pnoun(2, me)+"�Q�n�W�[�έק���Ӫ����O�H\n");
	
	arg = remove_ansi(arg);
	
	if( sscanf(arg, "-d %s", arg) == 1 )
	{
		if( !arrayp(info = EMOTE_D->query_emotion(arg)) )
			return tell(me, "�S�� "+arg+" �o�Ӫ����O�C\n");
		
		tell(me, emotion_detail_description(arg, info)+"�T�w�O�_�R�� "+arg+" �����O(Yes/No)�H");
		input_to( (: confirm_delete_emotion, me, arg :) );
		return;
	}
		
	if( arrayp(info = EMOTE_D->query_emotion(arg)) )
	{
		for(int i=0;i<8;i++)
			modifying_info[i] = info[i];
	}
	else
	{
		if( !valid_emote_name(arg) )
			return tell(me, "���X�k�������O�W�١C\n");
			
		modifying_info[NOTE] = 0;
		modifying_info[USE] = 0;
	}
		
	tell(me, emotion_detail_description(arg, modifying_info)+"�п�J���]�w������(�]�w���� save, �����]�w exit)�G\n>");
	input_to( (: choose_option, me, arg, modifying_info :) );
}