/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cost.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-28
 * Note   : cost ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

#define COST_TRIM       0
#define EXPT_TRIM       -40
inherit COMMAND;

string help = @HELP
        �з� cost ���O�C
HELP;

private void command(object me, string arg)
{
	mapping sinfo, einfo;
	int i, cost, time_exp, count = 1;
	float etime, utime, stime;
	string msg;

	if( !is_command() ) return;
	if( !arg ) return tell(me, "���O�榡�Rcost <���O>\n");
	sscanf(arg, "%d %s", count, arg);

	me = this_player(1);
	sinfo = rusage();
	cost = eval_cost();
	while(i < count) {
		time_exp += time_expression
		{
			me->force_me(arg);
		};
		// �� 40 �� force_me()���T�w������
		time_exp += EXPT_TRIM;
		cost += COST_TRIM;
		i++;
	}       
	cost -= eval_cost();
	einfo = rusage();
	utime = to_float(einfo["utime"]-sinfo["utime"])/1000000;
	stime = to_float(einfo["stime"]-sinfo["stime"])/1000000;
	etime = to_float(time_exp + EXPT_TRIM)/1000000;
	msg  = sprintf("���� %d �������ȡG\n", count);
	msg += sprintf("�Ĳv����: %.2f\n", to_float(cost)/count);
	msg += sprintf("�t�ήɶ�: %.6f s\n", stime/count);
	msg += sprintf("�ϥήɶ�: %.6f s\n", utime/count);
	msg += sprintf("�B��ɶ�: %.6f s\n", etime/count);
	tell(me, msg);
}

