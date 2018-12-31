/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : equip.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-21
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <equipment.h>

inherit COMMAND;

string help = @HELP
    �˳ƪ��~�����O�A�L�׬O�Z���B�窫�B�y�M�B�˹��~�B����Z�˳�
�O�Q�Φ����O�i��˳ưʧ@�C

equip '���~'		�˳ƬY�����~

HELP;

private void do_command(object me, string arg)
{
	int status;
	object ob;

	if( !arg )
	{
		string msg;
		object equipments = me->query_equipment_objects();
		
		if( !sizeof(equipments) )
			return tell(me, pnoun(2, me)+"�ثe���W�S������˳ơC\n");

		msg = pnoun(2, me)+"�ثe���W���˳Ʀp�U�G\n";
		
		foreach(ob in me->query_equipment_objects())
			msg += "�D"HIW"["NOR+ob->query_part_name()+HIW"] "+ob->query_idname()+"\n"NOR;
			
		return tell(me, msg+"\n");
	}
		
	if( !objectp(ob = present(arg, me)) )
		return tell(me, pnoun(2, me)+"�����W�èS�� "+arg+" �o�Ӫ��~�C\n");
	
	if( me->is_equipping_object(ob) )
		return tell(me, pnoun(2, me)+"�w�g�N"+ob->query_idname()+"�˳Ʀb�u"+ob->query_part_name()+"�v������W�F�C\n");

	if( !me->equip(ob, &status) )
	{
		switch(status)
		{
			// 1: �����󤣬O�˳�
			// 2: �L�k�˳Ʀb������W
			// 3: ����������Ӹ˳Ʀb��B
			// 4: �w�g���P�������˳�
			// 5: �w�g�˳Ʀb��L������W
			
			case 1:	return tell(me, ob->query_idname()+"�L�k�ΨӸ˳ơC\n"); break;
			case 2: return tell(me, pnoun(2, me)+"���G�S����������O�Ӹ˳Ƴo�Ӫ��~�C\n"); break;
			case 3: return tell(me, pnoun(2, me)+"���A��"+ob->query_idname()+"�Ӹ˳Ʀb��B�C\n"); break;
			case 4: return tell(me, ob->query_idname()+"�����˳Ʀb�u"+ob->query_part_name()+"�v����W�A���P"+pnoun(2, me)+"���W�Y�Ӹ˳Ƥ������~�o�ͽĬ�C\n"); break;
			case 5: return tell(me, ob->query_idname()+"�w�g�˳Ʀb��L������W�F�C\n"); break;
		}
	}
	else
		msg("$ME�N$YOU�˳Ʀb�u"+ob->query_part_name()+"�v������W�C\n", me, ob, 1);
}
