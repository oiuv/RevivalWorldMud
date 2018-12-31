/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rmtree.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-06-03
 * Note   : rmtree ���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        �з� rmtree ���O�C
HELP;

private void confirm_rmtree(object me, string dir, string arg)
{
	string msg = "";
	array res;
	
	if( !arg || lower_case(arg) != "y" )
		return tell(me, "�����R�� "+dir+" �ؿ��C\n", CMDMSG);
	
	res = rmtree(dir);
	
	foreach( string file in res[0] )
		msg += "rm "+file+" Done...�C\n";
	
	foreach( string file in res[1] )
		msg += "rm "+file+" Failed...�C\n";
		
	if( msg == "" )	
		msg = "rmtree "+dir+" Failed...�C\n";

	tell(me, msg, CMDMSG);
	me->finish_input();	
}
private void command(object me, string arg)
{
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "�п�J�ؿ����|�C\n", CMDMSG);

	arg = resolve_path(me, arg);
		
	if( file_size(arg) != -2 )
		return tell(me, arg+" �ä��O�@�ӥؿ��C\n", CMDMSG);

	tell(me, "�T�w�R���ؿ��G"+arg+" (Yes/No)�H", CMDMSG);
	input_to( (: confirm_rmtree, me, arg :) );
}
