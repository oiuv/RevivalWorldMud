/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : to.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-6
 * Note   : to ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>

inherit COMMAND;

string help = @HELP
�h���J���O

�o�ӫ��O�i�H���z��J�h��T���������w���O���Ѽ�
�b�s���@�椤

��J '~q' �i�H�����h���J 
��J '.'  �i�H�����h���J

���O�榡:
to <���O>

�������O:
HELP;

private void do_to(object me, string arg, string str)
{
	if( query("encode/gb", me) )
		str = B2G(str);

	me->force_me(arg ? arg+" "+str : str);
	me->finish_input();
}

private void do_command(object me, string arg)
{
	me->edit( (: do_to, me, arg :) );
}
