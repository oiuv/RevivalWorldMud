/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wizlock.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-06-21
 * Note   : wizlock ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <daemon.h>
#include <secure.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� wizlock ���O�C
HELP;

private void command(object me, string arg)
{
	int level_num, my_level_num;
	
	if( !is_command() ) return;
	
	if( !arg ) 
		return tell(me, "�ثe���\�v���b "+SECURE_D->level_num_to_level_name(LOGIN_D->query_wiz_lock())+" �H�W���ϥΪ̳s�u�C\n", CMDMSG);
	
	my_level_num = SECURE_D->level_num(me->query_id(1));
	
	if( !(level_num = SECURE_D->level_name_to_level_num(arg)) )
		return tell(me, "�S�� "+arg+" �o�ӵ��šC\n", CMDMSG);
	
	if( my_level_num < level_num )
		return tell(me, pnoun(2,me)+"���v�������H�]�w�o�˪��n�J�v���C\n", CMDMSG);
	
	if( level_num == LOGIN_D->query_wiz_lock() )
		return tell(me, "�ثe���n�J�v���w�g�]�w�� "+arg+"�C\n", CMDMSG);
	
	LOGIN_D->set_wiz_lock(level_num);
	
	if( level_num > PLAYER )
		CHANNEL_D->announce(me, "�t�Χ�s���@���A�ثe�Ȥ��\�v���b "+arg+" �H�W���ϥΪ̳s�u�C\n");
	else
		CHANNEL_D->announce(me, "�t�κ��@�����A�Ѱ��n�J�v������C\n");
		
}	