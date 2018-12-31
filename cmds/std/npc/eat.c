/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : eat.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-04
 * Note   : eat ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>

inherit COMMAND;

string help = @HELP
���έ��������O
eat '���~'		- �N�Y�Ӫ��~�Y��{�l��

HELP;

private void do_command(object me, string arg)
{
	int capacity;
	object ob;
	
	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say ����Q�n�ڦY�Ǥ���H\n");
		else
			return tell(me, pnoun(2, me)+"�Q�n�Y�Ǥ���H\n");
	}
	
	ob = present(arg, me) || present(arg, environment(me));
	
	if( !objectp(ob) )
	{
		if( me->is_npc() )
			return me->do_command("say ����èS�� "+arg+" �o�ح����C\n");
		else
			return tell(me, "����èS�� "+arg+" �o�ح����C\n");
	}

	if( !ob->is_food() )
	{
		if( me->is_npc() )
			return me->do_command("say "+ob->query_idname()+"���O�����A�L�k���ΡC\n");
		else
			return tell(me, ob->query_idname()+"���O�����A�L�k���ΡC\n");
	}
	
	capacity = query("capacity", ob);

	if( capacity + me->query_food_cur() > me->query_food_max() )
	{
		if( me->is_npc() )
			return me->do_command("say �ڦY���U"+ob->query_idname()+"�F�I\n");
		else
			return tell(me, pnoun(2, me)+"�Y���U"+ob->query_idname()+"�F�I\n");
	}
	
	me->add_food(capacity);

	msg("$ME�N$YOU�Y�i�{�l�̡C\n", me, ob, 1);

	ob->eat();
	destruct(ob, 1);
}