/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : setroom.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-5-8
 * Note   : setroom ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <inherit.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� setroom ���O
HELP;

private void command(object me, string arg)
{	
	string file;
	object room;
	
	if( !is_command() ) return;
	
	if( !arg || arg == "")
		return tell(me, "�п�J�A�n�]�w�ж����ɮצW�٩θ��|�C\n", CMDMSG);
	
	if( sscanf(arg, "%s %s", file, arg) != 2 )
		return tell(me, "�п�J�A�n�]�w�ж����ɮצW�٩θ��|�P�]�w�ﶵ�C\n", CMDMSG);

	
	file = resolve_path(me, file);

	file = file[<2..<1] == ".o" ? file : file + ".o";
	
	if( file_size(file) < 0 )
		return tell(me, "�A�L�k�]�w "+file+" �o���ɮסC\n", CMDMSG);

	room = load_object(file[0..<3]);
	me->edit( (: call_other, __FILE__, "set_description", me, arg, room :) );
}

void set_description(object me, string arg, object room, string str)
{
	set(arg, str, room);
	tell(me, "�]�w�ж� "+base_name(room)+" �� "+arg+" �� "+str+"�C\n", CMDMSG);
	room->save();
}