/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : follow.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-10
 * Note   : follow ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�o�ӫ��O�i�H���A�۰ʸ�ۥt�@�ӤH���ʡC

follow [�Y�H]		- �}�l���H�Y�H
follow -d		- ������H����H

HELP;

private void do_command(object me, string arg)
{
	string target_id;
	object target_ob;
	object *followers;
	object following_ob;
	
	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say ����Ʊ�ڸ��H�֡H\n");
		else
			return tell(me, pnoun(2, me)+"�Q�n���H�֡H\n");
	}
	
	if( arg == "-d" )
	{
		target_ob = query_temp("following", me);
		
		if( !objectp(target_ob) )
		{
			delete_temp("following", me);

			if( me->is_npc() )
				return me->do_command("say �ڭ쥻�K�S���b���H����H�C\n");
			else
				return tell(me, pnoun(2, me)+"�쥻�K�S���b���H����H�C\n");
		}

		followers = query_temp("follower", target_ob);		

		if( arrayp(followers) && sizeof(followers) )
		{
			followers -= ({ me });
			
			if( !sizeof(followers) )
				delete_temp("follower", target_ob);
			else
				set_temp("follower", followers, target_ob);
		}
		
		delete_temp("following", me);

		msg("$ME�����~����H$YOU���欰�C\n", me, target_ob, 1);
		
		return;
	}

	target_id = arg;
	target_ob = present(target_id);

	if( !objectp(target_ob) )
	{
		if( me->is_npc() )
		{
			if( target_id == "me" )
				target_ob = this_player(1);
			else
				return me->do_command("say �o�̨S�� "+arg+" �o�ӪF��C\n");
		}
		else
			return tell(me, "�o�̨S�� "+arg+" �o�ӪF��C\n");
	}

	if( !target_ob->is_living() )
	{
		if( me->is_npc() )
			return me->do_command("say "+target_ob->query_idname()+"���O�ͪ��A�L�k���H�C\n");
		else
			return tell(me, target_ob->query_idname()+"���O�ͪ��A�L�k���H�C\n");
	}
		
	if( target_ob == me )
	{
		if( me->is_npc() )
			return me->do_command("say �ڵL�k���H�ڦۤv�C\n");
		else
			return tell(me, pnoun(2, me)+"�L�k���H"+pnoun(2, me)+"�ۤv�C\n");
	}

	following_ob = query_temp("following", me);

	if( objectp(following_ob) )
	{
		followers = query_temp("follower", following_ob) || allocate(0);
		followers -= ({ me });
		
		if( !sizeof(followers) )
			delete_temp("follower", following_ob);
		else
			set_temp("follower", followers, following_ob);
			
		msg("$ME������H$YOU�C\n", me, following_ob, 1);	
	}
		
	followers = query_temp("follower", target_ob) || allocate(0);
	
	followers |= ({ me });
	
	set_temp("follower", followers, target_ob);
	set_temp("following", target_ob, me);

	msg("$ME�}�l�μv�����a��b$YOU������C\n", me, target_ob, 1);	
}
