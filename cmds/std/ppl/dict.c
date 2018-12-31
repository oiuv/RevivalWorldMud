/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : dict.c
 * Author : Msr@RevivalWorld
 * Date   : 2005-07-21
 * Note   : dict ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP

�i�H���z�Y��½Ķ
�� <-> �^


HELP;

nosave string *deer = ({ "deer", "elk", "wapiti" });

private void do_command(object me, string arg)
{
	if( !arg ) return tell( me, "�Х[�J�Q�n½Ķ���r�C\n", CMDMSG);

	if( member_array(arg, deer) != -1) {
		me->faint();
		return;
	}

	DICT_D->find_word( me, arg);
	return;
}
