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
#include <message.h>
inherit COMMAND;

string help = @HELP
�ۭq�e���j�p

�t�η|�۰ʰ����ϥΪ̪� Client �����j�p�A�H���̨Τƪ��T���榡��X
�Y�z�� Client ���䴩�h�t�ιw�]���ù����P�e���O�� 64 ��P 80 �Ӧr���e
�Y�z���ߺD�ӳ]�w�ȡA�i�ۦ��ʭק�

�d�ҤΥΪk�p�U:

> windowsize h64
�]�w�ù����׬� 64 ��

> windowsize w80
�]�w�ù��e�׬� 80 �Ӧr��

> windowsize h64 w80
�P�ɳ]�w���� 80 �P�e�� 64

���O�榡:
windowsize			��ܥثe�]�w
windowsize h<���>		�]�w�ù����� (��)
windowsize w<�r����>		�]�w�ù��e�� (�r��)
windowsize h<���> w<�r����>	�P�ɳ]�w���׻P�e��

�������O:
HELP;

private void do_command(object me, string arg)
{
	int h, w;

	h = query_temp("windowsize/height", me);
	w = query_temp("windowsize/width", me);
	
	if( !arg )
		return tell(me, pnoun(2, me)+"�ثe�]�w���e���j�p���G�� "+h+" ��A�e "+w+" �r���C\n");
	
	foreach(arg in explode(arg, " "))
	{
		if( sscanf(arg, "h%d", h) )
			set_temp("windowsize/height", h, me);
		
		if( sscanf(arg, "w%d", w) )
			set_temp("windowsize/width", w, me);
	}

	return tell(me, pnoun(2, me)+"�ثe�]�w���e���j�p���G�� "+h+" ��A�e "+w+" �r���C\n");
}
