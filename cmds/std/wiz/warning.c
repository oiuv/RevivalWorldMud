/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : warning.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-29
 * Note   : warning ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <ansi.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	warning [���aid] [multi]
HELP;

private void command(object me, string arg)
{
	object target;
	string option;

	if( !is_command() ) return;

	if( !arg || sscanf(arg, "%s %s", arg, option) != 2 )
		return tell(me, pnoun(2, me)+"�Q�n���ĵ�i��ơH\n", CMDMSG);
	
	if( !objectp(target = find_player(arg)) )
		return tell(me, "�S�� "+arg+" �o�ӤH�C\n", CMDMSG);

	switch( option )
	{
		case "multi":
		{
			int indeed_multi;
			string target_ip = query_ip_number(target);

			foreach( object user in users() - ({ target }) )
				if( query_ip_number(user) == target_ip )
				{
					indeed_multi++;
					tell(target, BEEP+HIR+"\n�t��ĵ�i�G"+user->query_idname()+HIR"�P"+pnoun(2, target)+"���� IP �n�J�C���A�ХߧY�b���}�W�D���X����(��:�ϥ�IP���ɾ�)�A�_�h�N�B�H�@�h�C\n\n"NOR, CMDMSG);
					tell(user, BEEP+HIR+"\n�t��ĵ�i�G"+target->query_idname()+HIR"�P"+pnoun(2, user)+"���� IP �n�J�C���A�ХߧY�b���}�W�D���X����(��:�ϥ�IP���ɾ�)�A�_�h�N�B�H�@�h�C\n\n"NOR, CMDMSG);
				}
			
			if( indeed_multi )
			{
				tell(me, "�w�e�X��"+target->query_idname()+"���ۦP IP ���еn�Jĵ�i�A�@�� "+(indeed_multi+1)+" �ӬۦP IP ���еn�J�C\n", CMDMSG);
				CHANNEL_D->channel_broadcast("nch", me->query_idname()+"�w�e�X��"+target->query_idname()+"���ۦP IP ���еn�Jĵ�i�A�@�� "+(indeed_multi+1)+" �ӬۦP IP ���еn�J�C");
			}
			else
				tell(me, target->query_idname()+"�õL���еn�J���ΡC\n", CMDMSG);
			break;
		}
		default:
			return tell(me, "�п�J���T��ĵ�i�ﶵ(multi)�C\n", CMDMSG);
	}
}
