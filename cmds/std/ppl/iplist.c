/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : iplist.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-12
 * Note   : iplist ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ip.h>
#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�ˬd�ثe���C���� IP ����]�w
HELP;

private void do_command(object me, string arg)
{
	string ip, msg, option;
	mapping ipdata = IP_D->query_ipdata();
	mapping data; 
	msg  = HIW"IP                  �]�w           �]�w�ɶ�         �����ɶ�\n"NOR;
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

	if( sizeof(ipdata) )
		foreach(ip in sort_array(keys(ipdata), 1))
	{
		data = ipdata[ip];
		switch(data["status"])
		{
		case BAN: 		option = HIR"�T��n�J"NOR; break;
		case CMDLIMIT:		option = HIY"���O�t�׭���"NOR; break;
		case OK:		continue;
		}
		msg += sprintf("%-20s%-15s"HIM"%-17s"NOR MAG"%s\n"NOR, ip, option, TIME_D->replace_ctime(data["recordtime"]), data["releasetime"] ? TIME_D->replace_ctime(data["releasetime"]) : "�L��");
	}
	else
		msg += "�S������ IP ����]�w�C\n";

	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += "�Y"+pnoun(2, me)+"���H�ϥ���C���W�h�� IP �o�D��t�έ���A�i�H�ɦV�Ův����\n";
	//return tell(me, msg);
	return me->more(msg);
}
