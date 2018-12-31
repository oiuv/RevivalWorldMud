/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ff.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-6-7
 * Note   : ff ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� ff ���O�C
HELP;

private void command(object me, string arg)
{

	object ob;
	string dest, func, file;

	if( !is_command() ) return;

	if( !arg || sscanf(arg, "%s %s", dest, func)!=2 )
		return tell(me, "���O�榡�Rff <����> <��ƦW��>\n", CMDMSG);	
		
	if( dest=="me" ) ob = me;
	else if( dest=="here" ) ob = environment(me);
	else ob = present(dest, me);
	
	if( !ob ) ob = present(dest, environment(me));
	if( !ob ) ob = find_object(resolve_path(me, dest));
	if( !ob ) return tell(me, "�o�̨S���u" + dest + "�v�C\n", CMDMSG);
	
        file = function_exists(func, ob, 1);
	
	if(!file)
		tell(me, sprintf("���� %O �èS���w�q %s �o�Ө�ơC\n", ob, func), CMDMSG);
	else
		tell(me, sprintf("���� %O �� %s ��Ʃw�q�b %s.c�C\n", ob, func, file ), CMDMSG);
}	
