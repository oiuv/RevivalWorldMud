/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : promote.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-1
 * Note   : promote ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <secure.h>

inherit COMMAND;

string help = @HELP
	�з� promote ���O�C
HELP;

private void command(object me, string arg)
{
	int level_num, my_level_num, no_online;
	object user;
	string id, level;

	if( !arg || !is_command() ) return;

	if( !sscanf(arg, "%s %s", id, level) )
		return tell(me, "���O�榡: promote �ϥΪ� ���šC\n");
	
	id = lower_case(id);
	
	if( !objectp(user = find_player(id)) )
	{
		if( !objectp(user = load_user(id)) )
			return tell(me, "�ثe�S�� "+id+" �o��ϥΪ̡C\n");
		else
			no_online = 1;
	}

	my_level_num = SECURE_D->level_num(me->query_id(1));
	level_num = SECURE_D->level_name_to_level_num(level);

	if( !level_num )
		return tell(me, "�S�� "+level+" �o�ӵ��šC\n");

	if( my_level_num < level_num || my_level_num < SECURE_D->level_num(id) )
		return tell(me, pnoun(2, me)+"�����Ť����A�L�k�N���Ŵ��ɦ� "+level+"�C\n");

	if( SECURE_D->change_wizard_level(id, level) )
	{
		string msg = me->query_idname()+"�N "HIY+user->query_idname()+" �ϥΪ̵��ŧאּ "+HIY+level+NOR"�C\n";
		
		CHANNEL_D->channel_broadcast("sys", msg);
		log_file("command/promote",msg);
		
		set("channels",CHANNEL_D->query_default_channel(id, user), user);
		
		if( SECURE_D->level_num(id) != PLAYER )
			tell(user, "�� Quit ���}�C����� port 4001 ���s�n���A�~��ɦ��Ův�v�Q�C\n");
		
		user->save();
		
		if( no_online )
			destruct(user);
	}
	else
		tell(me, "���Žվ㥢�ѡC\n");
}	
