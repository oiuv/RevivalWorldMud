/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : home.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-3-27
 * Note   : home ���O
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
	�з� home ���O�C
HELP;

private void command(object me, string arg)
{
	string file;
	object home;

	if( !is_command() ) return;
	
	file = wiz_home(arg || me->query_id(1))+"/workroom";
	
	intact_file_name(ref file);
	
	if( file_size(file) == -1 )
		return tell(me, "�o�ӤH�S���u�@�ǡC\n", CMDMSG);

	home = load_object(file);
	
	if( environment(me) == home ) 
		return tell(me, pnoun(2, me)+"�w�g�b"+(arg?" "+capitalize(arg)+" ":"�ۤv")+"���u�@�ǤF�C\n", CMDMSG);
		
	msg("$ME�Ƨ@�@�}"HIG"��"NOR GRN"��"NOR"��"+(arg?" "+capitalize(arg)+" ":"�ۤv")+"���u�@�ǭ��F�L�h�C\n",me, 0, 1, ENVMSG);

	me->move(home);

	msg("$ME�Ƨ@�@�}"HIG"��"NOR GRN"��"NOR"��"+(arg?" "+capitalize(arg)+" ":"�ۤv")+"���u�@�ǭ��F�L�ӡC\n",me, 0, 1, ENVMSG, ({me}));
}