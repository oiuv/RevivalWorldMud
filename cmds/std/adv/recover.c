/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : recover.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-26
 * Note   : recover ���O
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

#define LOG	"command/recover"

string help = @HELP
	�з� recover ���O�C
HELP;

private void command(object me, string arg)
{
	string path, option, msg;
	int overwrite, stat_time;
	int target_level_num, my_level_num;
	object target;
	
	if( !is_command() ) return;
	
	if( !arg ) 
		return tell(me, pnoun(2,me)+"�Q�n��_����ϥΪ̪���ơH\n", CMDMSG);
	
	sscanf( arg, "%s -%s", arg, option );
	
	if( !option )
		return tell(me, "�п�J -backup �� -deleted ���ѼơC\n", CMDMSG);
	
	if( sscanf( option, "%s -overwrite", option ) == 1 ) overwrite = 1;

	target_level_num = SECURE_D->level_num(arg);
	my_level_num = SECURE_D->level_num(me->query_id(1));
	
	switch( option )
	{
		case "backup":
			if( file_size(path = user_backup_path(arg)) != -2 )
				return tell(me, "�S�� "+arg+" �o��ϥΪ̪��ƥ���ơC\n", CMDMSG);
			
			msg = me->query_idname()+"�H "+TIME_D->replace_ctime(stat(path+"/data.o")[1])+" ���ƥ��ɮר��N�F "+capitalize(arg)+" �{������ơC\n";
			
			if( file_size(user_path(arg)) == -2 )
			{
				if( overwrite )
				{
					if( my_level_num < target_level_num )
						return tell(me, pnoun(2,me)+"���v�������H�л\ "+arg+" ���{���ϥΪ̸�ơC\n", CMDMSG);
					else if( !sizeof(rmtree(user_path(arg))[0]) )
						return tell(me, pnoun(2,me)+"���v���L�k�R�� "+arg+" ���{���ϥΪ̸�ơC\n", CMDMSG);
					
				}
				else return tell(me, "�Y�n�л\�{����ƥ����[�W -overwrite �ѼơC\n", CMDMSG);
			}
			
			if( file_size(user_path(arg)) == -2 || !sizeof(cptree(path, user_path(arg))[0]) )
				return tell(me, "��_ "+arg+" �s�ɥ��ѡC\n", CMDMSG);
			
			stat_time = stat(path+"/data.o")[1];
			
			break;
		case "deleted":
			if( file_size(path = user_deleted_path(arg)) != -2 )
				return tell(me, "�S�� "+arg+" �o��ϥΪ̪��R����ƥ���ơC\n", CMDMSG);
			
			
			
			msg = me->query_idname()+"�N��ӧG���ǹЪ��ɮ׫�½�F�@�M�A�ש�q���t����������x�s "+capitalize(arg)+" ��ƪ��Ϥ��A�æw�˦^�t�Τ��C\n";
			
			if( file_size(user_path(arg)) == -2 )
			{
				if( overwrite )
				{
					if( my_level_num < target_level_num )
						return tell(me, pnoun(2,me)+"���v�������H�л\ "+arg+" ���{���ϥΪ̸�ơC\n", CMDMSG);
					else if( !sizeof(rmtree(user_path(arg))[0]) )
						return tell(me, pnoun(2,me)+"���v���L�k�R�� "+arg+" ���{���ϥΪ̸�ơC\n", CMDMSG);
						
					rmtree(user_backup_path(arg));
				}
				else return tell(me, "�Y�n�л\�{����ƥ����[�W -overwrite �ѼơC\n", CMDMSG);
			}
			
			if( file_size(user_backup_path(arg)) == -2 || !sizeof(cptree(path, user_backup_path(arg))[0]) )
				return tell(me, "��_ "+arg+" �s�ɥ��ѡC\n", CMDMSG);
				
			if( file_size(user_path(arg)) == -2 || !sizeof(cptree(path, user_path(arg))[0]) )
				return tell(me, "��_ "+arg+" �s�ɥ��ѡC\n", CMDMSG);
			
			stat_time = stat(path+"/data.o")[1];
			
			rmtree(path);
			
			target = load_user(arg);
			
			PASSWORD_D->set_password(arg, copy(query("password", target)));
			
			delete("password", target);
			
			target->save();
			
			destruct(target);
			
			break;
		default:
			return tell(me, "�п�J -backup �� -deleted ���ѼơC\n", CMDMSG);
	}
	
	CHANNEL_D->channel_broadcast("news", msg);
	log_file(LOG, me->query_idname()+"��_ "+arg+" �b "+TIME_D->replace_ctime(stat_time)+" ���s�ɡC");
	
	if( objectp(target = find_player(arg)) ) 
	{
		tell(target, "\n"+me->query_idname()+"��_�F"+pnoun(2, me)+"�b "+TIME_D->replace_ctime(stat_time)+" ���s�ɡC\n"HIY"�Э��s�n�J�s�u�C\n"NOR, CMDMSG);
		destruct(target);
	}
}	