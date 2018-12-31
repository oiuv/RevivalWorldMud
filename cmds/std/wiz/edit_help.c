/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : edit_help.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-9-15
 * Note   : edit_help ���O
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
	�з� edit_help ���O�C
HELP;

private void modify_help(object me, string filename, string help)
{
	int i, start_line, end_line, size;
	string *file = explode(read_file(filename), "\n");
	
	size = sizeof(file);
	
	for(i=0;i<size;i++)
	{
		if( file[i][0..18] == "string help = @HELP" )
			start_line = i;
		else if( file[i][0..4] == "HELP;" )
		{
			end_line = i;
			break;
		}
	}

	file = file[0..start_line] + ({ kill_repeat_ansi(help+NOR) }) + file[end_line..];

	if( write_file(filename, implode(file, "\n"), 1) )
	{
		CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�s�� "+filename+" �����ɡC");
		tell(me, "�ק� "+filename+" �����ɧ����C\n", CMDMSG);
	}
	else
		tell(me, "�g�J�v�������C\n", CMDMSG);
}


private void command(object me, string arg)
{
	string current_help;
	
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "�п�J���s�軡���ɤ��ɮצW�١C\n", CMDMSG);

	arg = resolve_path(me, arg);
	
	intact_file_name(ref arg);
	
	if( !file_exist(arg) )
		return tell(me, "�S�� "+arg+" �o���ɮסC\n", CMDMSG);
		
	if( !(current_help = fetch_variable("help", load_object(arg))) )
		return tell(me, arg+" ���ݭn�s�軡���ɡC\n", CMDMSG);
	
	tell(me, arg+" ���쥻�����p�U�G\n"+current_help+"\n", CMDMSG);
	me->edit( (:modify_help, me, arg:) );
}