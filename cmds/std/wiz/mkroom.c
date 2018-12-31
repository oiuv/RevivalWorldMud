/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mkroom.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-5-8
 * Note   : mkroom ���O
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
	�з� mkroom ���O
HELP;

private void command(object me, string arg)
{	
	object room;
	
	if( !is_command() ) return;
	
	if( !arg || arg == "")
		return tell(me, "�п�J�A�n�إߩж����ɮצW�٩θ��|�C\n", CMDMSG);

	
	arg = resolve_path(me, arg);
	
	if( file_size(arg[0..strsrch(arg,"/")]) != -2 ) 
		return tell(me, "�Х��إߥؿ� "+arg[0..strsrch(arg,"/")]+" �A��سy�ж��C\n", CMDMSG);

	
	if( strsrch(arg, "/room/") == -1 )
		return tell(me, "�ж��u��سy�b�ؿ� room ���l�ؿ��U�C\n", CMDMSG);

	
	tell(me, "�إ߰򥻩ж� "+arg+" ��...�C\n", CMDMSG);
	
	if( !(room = load_object(arg)))
		tell(me, "�L�k�إ߰򥻩ж��C\n", CMDMSG);
	else
	{
		tell(me, "�ж� "+arg+" �إߦs�ɧ����C\n", CMDMSG);
		room->save();
	}
}