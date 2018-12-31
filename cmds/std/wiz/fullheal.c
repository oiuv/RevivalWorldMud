/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : fullheal.c
 * Author : Sinji@RevivalWorld
 * Date   : 2001-6-10
 * Note   : fullheal ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� fullheal ���O
HELP;

private void fullheal(object ob)
{
	ob->set_all_ability_full();
	
	if( ob->is_faint() )
		ob->wakeup();
}
	
private void command(object me, string arg)
{
    	object ob;

	if( !is_command() ) return;
	
    	if( !arg )
    		ob = me;
    	else if( !objectp(ob = find_player(arg)) )
    		ob = present(arg);
 
    	if( !objectp(ob) )
    		return tell(me, "�S�� "+arg+" �o�إͪ��C\n");
    	
    	if( !ob->is_living() )
    		return tell(me, ob->query_idname()+"���O�ͪ��C\n");

    	if( ob == me )
    		msg("$ME���x�x�ߨ��t���E�F�@��"HIR"��"NOR RED"��"NOR"�x���A�S�ֳt�a�l�J�ۤv������C\n", me, 0, 1);
    	else		
		msg("$ME���x�x�ߨ��t���E�F�@��"HIR"��"NOR RED"��"NOR"�x���A���x���i$YOU������C\n", me, ob, 1);
	
	msg("$ME���y��v����F�_��...�C\n", ob, 0, 1);
	
	fullheal(ob);
}
