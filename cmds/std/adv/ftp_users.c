/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : search.c
 * Author : Sinji@RevivalWorld
 * Date   : 2003-06-21
 * Note   : 
 * Update :
 *  o 2003-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
#include <daemon.h>
inherit COMMAND;

private void command(object me, string arg)
{
	mixed ftp_sockets;
	string msg, stat ="";

	if( !find_object(FTP_D) )
		return tell(me, "FTP_D �|���ҰʡC\n");

	ftp_sockets = FTP_D->query_sockets();

	if( !ftp_sockets || !sizeof(ftp_sockets) )
                return tell(me, "FTP_D �ثe�S������ϥΪ̳s�u�C\n");
	msg = sprintf("%-3s %-12s %-17s %-8s %-s\n", "FD", "�b��", "�s�u��m", "���A", "�̫���O"); 
	foreach(string fd, mixed val in ftp_sockets)
	{
		if( !undefinedp(val["retr"]) ) stat += "<�U����>";
		if( !undefinedp(val["stor"]) ) stat += "<�W�Ǥ�>";
		msg += sprintf("%3d %-12s %-17s %-8s %s\n",
			fd,
			(val["login"] ? val["user"]:"(���n�J)"),
			val["address"],
			stat, 
			(val["lastcmd"] || "(�L)"),
		);
	}
	tell(me, msg);
	return;
}
