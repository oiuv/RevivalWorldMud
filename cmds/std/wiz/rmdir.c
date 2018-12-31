/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rmdir.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-8
 * Note   : rmdir ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� rmdir ���O�C
HELP;

private void command(object me, string arg)
{

	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "�п�J���|�W�١C\n", CMDMSG);

	arg = resolve_path(me, arg);
	
	if( file_size(arg) != -2 )
		return tell(me, "�S�� "+arg+" �o�ӥؿ��C\n", CMDMSG);

	if( sizeof(get_dir(arg[<1]=='/'?arg:arg+"/")) != 0 )
		return tell(me, "�Х��N�ؿ� "+arg+" �����ؿ��ɮקR���C\n", CMDMSG);

	if( rmdir(arg) )
		tell(me, "�R���ؿ� "+arg+" �����C\n", CMDMSG);
	else
		tell(me, "�A�S���v�Q�R���ؿ� "+arg+"�C\n", CMDMSG);
}