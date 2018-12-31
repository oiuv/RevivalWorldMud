/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : keep.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-04-12
 * Note   : keep ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <object.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�o�ӫ��O�ΨӫO�d���W���n�����~�A�H�����p�߳Q��X�Ϊ̬O���C

keep '���~'		- �Ұʨ��W�Y�Ӫ��~���O�d���A
keep -d '���~'    	- �����Y�Ӫ��~���O�d���A
keep all            	- �Ұʨ��W�Ҧ��F�誺�O�d���A

HELP;

private void do_command(object me, string arg)
{
	int cancel;
	object ob;
	
	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say ����Q�n�N�ڨ��W���@�Ӫ��~�]�w���O�d���A�H\n");
		else
			return tell(me, pnoun(2, me)+"�Q�n�N���@�Ӫ��~�]�w���O�d���A�H\n");
	}
	
	if( sscanf(arg, "-d %s", arg) )
		cancel = 1;
	
	if( arg == "all" )
	{
		if( !cancel )
		{
			foreach( ob in all_inventory(me) )
				ob->set_keep();

			return tell(me, "�ҰʩҦ����W���~���O�d���A�C\n");
		}
		else
		{
			foreach( ob in all_inventory(me) )
			{
				if( ob->is_keeping() == 2 || ob->is_equipping() ) 
					continue;
					
				ob->delete_keep();
			}
		
			return tell(me, "�����Ҧ����W���~���O�d���A�C\n");
		}
	}
	else if( objectp(ob = present(arg, me)) )
	{
		if( !cancel )
		{
			ob->set_keep();
			
			return tell(me, "�Ұ�"+ob->query_idname()+"���O�d���A�C\n");
		}
		else
		{
			if( ob->is_keeping() == 2 )
				return tell(me, "�L�k����"+ob->query_idname()+"���O�d���A�C\n");
			
			if( ob->is_equipping() )
				return tell(me, pnoun(2, me)+"���������U"+ob->query_idname()+"���˳ƪ��A�C\n");

			ob->delete_keep();	
			return tell(me, "����"+ob->query_idname()+"���O�d���A�C\n");
		}
	}
	else
		return tell(me, pnoun(2, me)+"���W�èS�� "+arg+" �o�󪫫~�C\n");
}
