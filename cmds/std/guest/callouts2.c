/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : callouts2.c
 * Author : Msr@RevivalWorld
 * Date   : 2001-2-6
 * Note   : callouts ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <ansi.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
        �o�ӫ��O�i�d�ߥثe mud �����ϥ� SCHEDULE_D ��Ʃ����I�s�����ءC
HELP;

private void command(object me, string arg)
{
	mapping info;
        string msg;

	msg  = sprintf("%6s %-20s %-25s %-40s\n", "����", "�I�s���", "����", "�Ѽ�");
	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";

	info = SCHEDULE_D->query_events();

	foreach(int key in keys(info)) 
	{
		msg += sprintf("%6d %-20s %-25O %-40s\n", 
		    info[key][2] - info[key][1],
		    info[key][4], 
		    info[key][3],
		    (sizeof(info[key])>5 ? info[key][5]: "�L")              
		);
	}

	if( !sizeof(info) ) 
		msg += "�ثe�S���{�ǩI�s���C\n";

	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
	msg += NOR WHT" �禡�I�s�w "NOR"�ثe�� " + sizeof(info) + " �ӵ{�ǩ���I�s���C\n";

	me->more(msg);
}
