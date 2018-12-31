/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : callouts.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-6
 * Note   : callouts ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */
 
#include <feature.h>
#include <ansi.h>

inherit COMMAND;

string help = @HELP
	�з� callouts ���O
HELP;

private void command(object me, string arg)
{
	array info;
	string msg, oarg;

	if( !is_command() ) return;
	
	msg  = sprintf("%6s %-20s %-10s %-40s\n", "����", "�I�s���", "�Ѽ�", "����");
	msg += "  �w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
	info = call_out_info();
	info = sort_array(info, (: ($1[2] < $2[2] ? -1:1) :));
	foreach(mixed cout in info) 
	{
		if( sizeof(cout) > 3 )
			oarg = cout[3];
		else
			oarg = "�L";

		msg += sprintf("%6d %-20s %-10s %-40O\n", cout[2], cout[1], oarg, cout[0]);
	}
	
	if( !sizeof(info) ) 
		msg += "�ثe�S���{�ǩI�s���C\n";
	
	msg += "  �w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
	msg += NOR WHT" �禡�I�s�w "NOR"�ثe�� " + sizeof(info) + " �ӵ{�ǩ���I�s���C\n";
	
	me->more(msg);
}
