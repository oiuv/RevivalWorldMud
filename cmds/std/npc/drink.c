/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : drink.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-04
 * Note   : drink ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>

inherit COMMAND;

string help = @HELP
���ζ��ƪ����O
drink '���~'		- �N�Y�Ӫ��~�ܨ�{�l��

HELP;

private void do_command(object me, string arg)
{
	int capacity;
	object ob;
	
	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say ����Q�n�ڳܨǤ���H\n");
		else
			return tell(me, pnoun(2, me)+"�Q�n�ܨǤ���H\n");
	}
	
	ob = present(arg, me) || present(arg, environment(me));
	
	if( !objectp(ob) )
	{
		if( me->is_npc() )
			return me->do_command("say ����èS�� "+arg+" �o�ض��ơC\n");
		else
			return tell(me, "����èS�� "+arg+" �o�ض��ơC\n");
	}

	if( !ob->is_drink() )
	{
		if( me->is_npc() )
			return me->do_command("say "+ob->query_idname()+"���O���ơA�L�k���ΡC\n");
		else
			return tell(me, ob->query_idname()+"���O���ơA�L�k���ΡC\n");
	}
	
	capacity = query("capacity", ob);

	if( capacity + me->query_drink_cur() > me->query_drink_max() )
	{
		if( me->is_npc() )
			return me->do_command("say �ڳܤ��U"+ob->query_idname()+"�F�I\n");
		else
			return tell(me, pnoun(2, me)+"�ܤ��U"+ob->query_idname()+"�F�I\n");
	}
	
	me->add_drink(capacity);

	msg("$ME��$YOU�j�f�j�f�a�ܤF�U�h�C\n", me, ob, 1);

	ob->drink();
	destruct(ob, 1);
}