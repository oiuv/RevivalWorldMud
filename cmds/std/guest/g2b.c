/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : gb2.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-1-8
 * Note   : GB to Big5 ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
 
inherit COMMAND;

string help = @HELP
	�з� g2b ���O
HELP;


private void command(object me, string arg)
{
	if( !is_command() ) return;
	
	if(!arg) return;
	
	me->more(G2B(arg));
}

