/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mv.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-6
 * Note   : mv ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
 
inherit COMMAND;

#define FROM		0
#define TO		1

string help = @HELP
	�з� mv ���O
HELP;

private void command(object me, string arg)
{
	string from, to, *parse;
	
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "���O�榡: mv <���ɦW> <�ؼ��ɦW> \n", CMDMSG);
	
	parse = single_quote_parse(arg);
	
	if( sizeof(parse) != 2 )
		return tell(me, "���O�榡: mv <���ɦW> <�ؼ��ɦW> \n", CMDMSG);
	 
	from = resolve_path(me, parse[FROM]);
	to = resolve_path(me, parse[TO]);
	
	switch(file_size(from))
	{
		case -1:
			return tell(me, "�S�� "+from+" �o���ɮשΥؿ��C\n", CMDMSG);
		case -2:
			if( from[<1] != '/' ) from += "/";
			
			switch( file_size(to) )
			{
				case -1:
					if( to[<1] != '/' ) to += "/";
					break;
				case -2:
					if( to[<1] != '/' ) to += "/";
					to += from[strsrch(from[0..<2], "/",-1)+1..];
					break;
				default:
					return tell(me, "�L�k�л\�ɮ� "+to+"�C\n", CMDMSG);
			}
			break;
		default:
			switch( file_size(to) )
			{
				case -1:
					if( file_size(to[0..strsrch(to, "/",-1)]) != -2 )
						return tell(me, "�S�� "+to[0..strsrch(to, "/",-1)]+"�o�ӥؿ��C\n", CMDMSG);
					break;
				case -2:
					if( to[<1] != '/' ) to += "/";
					to += from[strsrch(from, "/",-1)+1..];
					break;
				default:
					return tell(me, "�L�k�л\�ɮ� "+to+"�C\n", CMDMSG);
			}
			break;
	}

	if( !rename(from, to) )	
		tell(me, "mv "+from+" -> "+to+" Done...�C\n", CMDMSG);
	else
		tell("mv "+from+" -> "+to+" Failed...�C\n", CMDMSG);
}
