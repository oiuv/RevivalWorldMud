/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rm.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-6
 * Note   : rm ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� rm ���O
HELP;

private void command(object me, string arg)
{
	string *parse;
	
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "�п�J���R�����ɦW�C\n", CMDMSG);
	
	parse = single_quote_parse(arg);
	
	arg = resolve_path(me, parse[0]);
	
	if( file_size(arg) < 0 )
		return tell(me, "�S�� "+arg+" �o���ɮסC\n", CMDMSG);
	
	if( rm( arg ) )
		tell(me, "�R�� "+arg+" �����C\n", CMDMSG);
	else
		tell(me, "�v�������A�L�k�R�� "+arg+"�C\n", CMDMSG);
}