/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : beep.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-06-10
 * Note   : beep ���O
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
�I�s���O

�o�ӫ��O�i�H�ΨөI�s��L�ϥΪ̡A�Y�ӨϥΪ̪� Client �䴩�h�|���񴣥��n

���O�榡:
beep <�ϥΪ̥N��>

�������O: waitback
HELP;

private void do_command(object me, string arg)
{
	object ob;
    	
    	if( !arg )
    		return tell(me, pnoun(2, me)+"�Q�I�s����u�W���a�H\n", CMDMSG);
    	if( !(ob = find_player(arg)) ) 
    		return tell(me, "�S���o���a�C\n", CMDMSG);
    	
        msg("$ME���۵P�ؤ��_���V����$YOU���j�Y�A���G�O�Q����$YOU���֤@�I�C\n", me, ob, 1, ENVMSG);
    	tell(ob,  "\a", 0);
}
