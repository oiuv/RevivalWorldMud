/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : waitback.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-9-9
 * Note   : waitback ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
���ݫ��O

�o�ӫ��O�i�H���z���ݶ��m�����ϥΪ̡A����ӨϥΪ̶}�l���ʮɳq���z�C

���O�榡:
waitback <�ϥΪ̥N��> <�d��>

�������O: beep
HELP;

private void do_command(object me, string arg)
{
	object target;
	mapping waitback_list, info;
	string msg;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n���֦^���H\n", CMDMSG);

	sscanf(arg, "%s %s", arg, msg);
	
	if( !objectp(target = find_player(arg)) )
		return tell(me, "�S�� "+arg+" �o�쪱�a�C\n", CMDMSG);

	if( target == me )
		return tell(me, "��"+pnoun(2, me)+"�ۤv�^���H�H\n", CMDMSG);

	info = allocate_mapping(2);
	info["time"] = time();
	info["msg"] = msg;
	
	if( mapp(waitback_list = query_temp("waitback_list", target)) ) 
		set_temp("waitback_list", waitback_list + ([ me : info ]), target);
	else
		set_temp("waitback_list", ([ me : info ]), target);	
	
	msg(HIG"$ME"HIG"�}�l���ݵ�"HIG"$YOU�^���C\n"NOR, me, target, 1);
	tell(target, "\a", 0);
}