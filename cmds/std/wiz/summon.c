/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : summon.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-24
 * Note   : summon ���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        �з� summon ���O�C
HELP;

private void command(object me, string arg)
{
	object you;
	
	if( !is_command() ) return;
		
	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n��ֶǰe�L�ӡH\n", CMDMSG);
		
	if( !objectp(you = find_player(arg)) )
		return tell(me, "�S�� "+arg+" �o�쪱�a�C\n", CMDMSG);
		
	if( !objectp(environment(me)) )
		return tell(me, pnoun(2, me)+"���B���ۮɪŤ����A�L�k�N�O�H�ǰe��"+pnoun(2, me)+"���B�����ҡC\n", CMDMSG);
	
	msg("$ME�����Ǭ�M�U�_�@�}�Ť��A������$ME������U�ӷU�ҽk�A�̫�����Ϯ������C\n", you, 0, 1);
	
	you->move_to_environment(me);
	
	msg("$ME���_����Aíí�a�N�q���䭸�Ӫ�"+you->query_idname()+"����C\n", me, 0, 1, ENVMSG);
}