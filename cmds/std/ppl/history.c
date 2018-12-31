/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : history.c
 * Author : Cwlin
 * Date   : 2002-8-20
 * Note   : history ���O
 * Update :
 *  o 2003-04-22 Clode rewrite
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <ansi.h>
#include <feature.h>
#include <message.h>

#define HCMD		0
#define HTIME		1

#define DEFAULT_SIZE	25

inherit COMMAND;

string help = @HELP
���O���{

�o�ӫ��O�Ψ��˵��z���ϥιL�����O�����A�z�i�H�z�L�U�C���O�Өϥαz�����O����

!!		���ƤW�@�ӫ��O
!<���O�s��>	���� <���O�s��> �����O

���O�榡:
history <���O��>		��̪ܳ� <���O��> �������O���{
history <�ϥΪ̥N��>		��� <�ϥΪ�> �����O���{ (�Ův�M��)
history <���O��> <�ϥΪ̥N��>	��� <�ϥΪ�> �̪� <���O��> �������O���{ (�Ův�M��)
history <�ϥΪ̥N��> <���O��>	�P�W

�������O: 
HELP;

private void do_command(object me, string arg)
{
	int i, ttl_size, size, start;
	string msg;
	array cmd_history;
	object ob;

	if( wizardp(me) && arg == "-l" )
	{
		object *sortusers;
		
		msg = "�ƦW ���a�W��                   ���O��   �t��(�C��)   ���ʥ��� �t�έt��\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		sortusers = filter_array(users(), (: $1->is_user_ob() :));
		sortusers = sort_array(sortusers, (: to_float($1->total_command_count())/(time() - query_temp("login/time", $1)) < to_float($2->total_command_count())/(time() - query_temp("login/time", $2)) ? 1 : -1 :));

		foreach(ob in sortusers)
		{
			cmd_history = ob->query_history();
			
			msg += sprintf("%-3d. %-27s%-9d%-13.2f%-8.2f %-8s %s\n", 
				++i, 
				ob->query_idname(), 
				ob->total_command_count(), 
				to_float(ob->total_command_count())/(time() - query_temp("login/time", ob)),
				sizeof(cmd_history) ? to_float(sizeof(cmd_history))/((cmd_history[<1][1] - cmd_history[0][1])||1) : 0.,
				count(ob->query_timecost_record(), "/", ob->total_command_count()||1),
				query("cmdlimit", ob) ? HIR"(Limited)"NOR:"",
			);

		}
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		me->more(msg);
		return;
	}
	if( !arg || !wizardp(me) ) ob = me;
	else if( sscanf(arg, "%d %s", size, arg) == 2 ) ob = find_player(arg);
	else if( sscanf(arg, "%s %d", arg, size) == 2 ) ob = find_player(arg);
	else if( sscanf(arg, "%d", size) == 1 ) ob = me;
	else ob = find_player(arg);

	if( !objectp(ob) )
		return tell(me, "�S�� " + arg + " �o�쪱�a�C\n");

	if( ob != me && SECURE_D->level_num(me->query_id(1)) < SECURE_D->level_num(ob->query_id(1)) )
		return tell(me, pnoun(2, me) + "�S���v���d�\" + ob->query_idname() + "�����v���O�����C\n");

	cmd_history = ob->query_history();

	if( !(ttl_size = sizeof(cmd_history)) )
		return tell(me, (me==ob ? pnoun(2, me):ob->query_idname()) + "�S��������v���O�����C\n");

	if( size <= 0 ) size = DEFAULT_SIZE;
	if( size > ttl_size ) size = ttl_size;
	
        msg = (me==ob ? pnoun(2, me):ob->query_idname())+"�`�@�� "+ttl_size+" ���������A���O�����t�� "+(ob->total_command_count() > 0 ? sprintf("%.2f", to_float(ob->total_command_count())/((time() - query_temp("login/time", ob))||1)) : 0)+" (���O��/��)�A�U�C���̪� "+size+" ��: \n"; 

	start = ttl_size - size;

	i = ob->total_command_count() - size;
	foreach(array cmd in cmd_history[start..])
		msg += sprintf(HIW" [%4d]"NOR + WHT" %s"NOR" %s\n"NOR, (++i), ctime(cmd[HTIME])[11..18], cmd[HCMD]);

	me->more(msg);
}
