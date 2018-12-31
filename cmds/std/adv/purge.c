/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : purge.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-26
 * Note   : purge ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <secure.h>
#include <message.h>

inherit COMMAND;

#define LOG	"command/purge"

string help = @HELP
	�з� purge ���O�C
HELP;

private void command(object me, string arg)
{
	int target_level_num, my_level_num;
	object target;
	string target_idname;
	
	if( !is_command() ) return;
	
	if( !arg ) 
		return tell(me, pnoun(2,me)+"�Q�n�R������ϥΪ̪���ơH\n", CMDMSG);
	
	if( file_size(user_path(arg)) != -2 )
		return tell(me, "�S�� "+arg+" �o��ϥΪ̸�ơC\n", CMDMSG);
	
	target_level_num = SECURE_D->level_num(arg);
	my_level_num = SECURE_D->level_num(me->query_id(1));
	
	if( my_level_num < target_level_num )
		return tell(me, pnoun(2,me)+"���v�������H�R�� "+arg+" ���ϥΪ̸�ơC\n", CMDMSG);
	
	
	target = find_player(arg) || load_user(arg);
	
	set("password", PASSWORD_D->query_password(arg), target );
	
	target->save();
	
	target_idname = target->query_idname();

	if( CHAR_D->destruct_char(arg) )
	{
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"�b�_�㤧�U�N"+target_idname+"���ɮװ���F�Ϧw�˵{�ǡA���x�s��Ϥ��W�H�����ɮ׫Ǫ������C\n");
		log_file(LOG, me->query_idname()+"�R��"+target_idname+"�ɮסC");
	}
	else
		return tell(me, "�R��"+target->query_idname()+"��ƥ��ѡC\n", CMDMSG);
		
	if( objectp(target) ) 
	{
		tell(target, HIR+pnoun(2, target)+"�Q�t�κ޲z��"+me->query_idname()+"�R�����a�ɮפF�A�Y������N���i�A�إ߷s����P�Ův�̰Q�סC\n", CMDMSG);
		destruct(target);
	}	
}	