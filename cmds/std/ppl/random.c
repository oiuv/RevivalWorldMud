/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pplcmd_example.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-00-00
 * Note   : ���a���O�d��
 * Update :
 *  o 2003-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>

inherit COMMAND;

string help = @HELP
	�зǪ��a���O�C
HELP;

private void do_command(object me, string arg)
{
	int i;
	
	if( arg ) sscanf(arg, "%d", i);
	if( i <= 0 ) i = 6;
	msg("$ME�q��̥�X�F�@�� 1 ~ " + i + " �I����l ...\n", me, 1);
	msg("��l�̧r�̪����F�U�� ... ���G�X�{���O " + (random(i) + 1) + " �I�C\n", me, 1);
	return;
}