/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : fight.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-26
 * Note   : fight ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>

inherit COMMAND;

string help = @HELP
�P�H�԰������O

fight '�ؼ�'		- �M�Y�H�}�l�԰�

HELP;

private void do_command(object me, string arg)
{
	object enemy;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�M�־԰��H\n");
		
	if( arg == "-all" )
	{
		object enemies = filter_array(present_objects(me), (: $1 != $(me) && $1->is_living() && !$1->is_faint() && !query("no_fight", $1) :));
			
		if( sizeof(enemies) )
		foreach(enemy in  enemies)
		{
			msg(HIY"$ME"HIY"�}�l��$YOU"HIY"�i������C\n"NOR, me, enemy, 1);
			
			// �͹�ĵ�i
			//if( userp(enemy) )
			//	tell(enemy, BEEP);
		
			COMBAT_D->start_fight(me, enemy);
		}
		else
			return tell(me, "�o�̨S������i�H�i��������ؼСC\n");

		return;
	}
	else if( arg == "-stop" )
	{
		if( !COMBAT_D->in_fight(me) )
			return tell(me, pnoun(2, me)+"�èS���b�԰����C\n");
		
		msg("$ME����԰��C\n", me, 0, 1);
		COMBAT_D->stop_fight(me);
		return;
	}
	
	if( !objectp(enemy = present(arg, environment(me))) )
		return tell(me, "�o�̨S�� "+arg+" �o�ӤH�C\n");

	if( enemy == me )
		return tell(me, pnoun(2, me)+"����M�ۤv�԰��C\n");

	if( !enemy->is_living() )
		return tell(me, enemy->query_idname()+"���O�ͪ��A"+pnoun(2, me)+"�S��k�M"+pnoun(3, enemy)+"�԰��C\n");

	if( enemy->is_faint() )
		return tell(me, enemy->query_idname()+"�w�g�S����O��"+pnoun(2, me)+"�԰��F�C\n");

	if( COMBAT_D->is_fighting(me, enemy) )
		return tell(me, pnoun(2, me)+"�M"+enemy->query_idname()+"�w�g�b�԰����C\n");
		
	if( !wizardp(me) )
		return tell(me, "�԰��\����դ��C\n");

	if( query("no_fight", enemy) )
		return tell(me, pnoun(2, me)+"�L�k��"+enemy->query_idname()+"�i������C\n");
	
	msg(HIY"$ME"HIY"�}�l�V$YOU"HIY"�i������C\n"NOR, me, enemy, 1);
	
	// �͹�ĵ�i
	//if( userp(enemy) )
	//	tell(enemy, BEEP);

	COMBAT_D->start_fight(me, enemy);
}
