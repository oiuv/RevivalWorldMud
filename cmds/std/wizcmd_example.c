/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wizcmd_example.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-00-00
 * Note   : �Ův���O�зǮ榡
 * Update :
 *  o 2005-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>

inherit COMMAND;

string help = @HELP
	�з� wizcmd_example ���O�C
HELP;

private void command(object me, string arg)
{
       	if( !is_command() ) return;
       	

}
