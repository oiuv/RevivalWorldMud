/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : dump.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-01
 * Note   : dump ���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>

inherit COMMAND;

string help = @HELP
        �з� dump ���O�C
HELP;

private void command(object me, string arg)
{
	dumpallobj(arg || "/log/OBJ_DUMP");
	tell(me, "�ɦL���J���󤧲έp��ƦC�����C\n");
}