/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : finger.c
 * Author : Clode && Cookys
 * Date   : 2001-6-10
 * Note   : finger ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>

inherit COMMAND;

string help = @HELP
�ϥΪ̬d�߫��O

�o�ӫ��O�i�H�d�ߨϥΪ̸�T�A�p�ʺ١B�N���B�v���B�s�u�ɶ��B���A ... ���C

���O�榡:
finger <�ϥΪ̥N��>
	
�������O: chfn, who
HELP;

private void do_command(object me, string arg)
{
	if( !arg )
	{
		if( wizardp(me) ) 
			me->more(FINGER_D->finger_all()+"\n");
		else
			tell(me, pnoun(2, me)+"�Q�n�d�\�֪���ơH\n");
	}
	else
	{
		arg = replace_string(arg, "#", "");
		tell(me, FINGER_D->finger_user(arg));
	}
}
