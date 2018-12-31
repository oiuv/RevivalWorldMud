/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : userid.c
 * Author : Sinji@RevivalWorld
 * Date   : 2002-8-23
 * Note   : sinji ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <socket.h>
#include <message.h>

#define AUTH_PORT		113
#define IDENT_CALL_HANDLE       0
#define IDENT_COMMANDER         1
#define IDENT_USER              2
#define IDENT_PORT		3
#define IDENT_RPORT		4

inherit COMMAND;

string help = @HELP
���O�榡�Gident <�ϥΪ�>

�o�ӫ��O�i�H���o�ϥΪ̦b��D���W�ҨϥΪ�
�b���ΥD����T�C
HELP;

private nosave object receiver;
private nosave mapping idents = allocate_mapping(0);
private void time_out(int fd);

private void command(object me, string arg)
{
	object ob;
	int fd, res, address, port, rport, call_handle;
	string socket;

	if( !is_command() ) return;

	if( !arg ) ob = me;
	if( !ob ) ob = find_player(arg);
	if( !ob )
		return tell(me, "�S�� " + arg + " �o�ӨϥΪ̡C\n", CMDMSG);
	if( (socket = socket_address(ob)) == "" )
		return tell(me, "�L�k���o���ݳs�u��}�C\n", CMDMSG);
	if( sscanf(socket, "%s %d", address, rport) != 2 )
		return tell(me, "��}�榡���~�C\n", CMDMSG);
	if( !(port = query_ip_port(ob)) )
	 	return tell(me, "�L�k���o�s�u�𸹡C\n", CMDMSG);
	
	fd = socket_create(STREAM, "create_read_callback", "create_close_callback");

	if( fd < 0 )
		return tell(me, "�L�k�إ߳s�u�G" + socket_error(fd), CMDMSG);

	res = socket_connect(fd, address + " " + AUTH_PORT, "read_callback", "write_callback");

	if( res < 0 )
	{
		tell(me, "�L�k�s�u�G" + socket_error(fd), CMDMSG);
		socket_close(fd);
		return;
	}
	tell(me, "[FD: " + fd + "] �P�ϥΪ̥D���s�u��...\n", CMDMSG);
	call_handle = call_out((: time_out :), 30, fd);
	idents[fd] = ({ call_handle, me, ob, port, rport });
	return;

}
void create_read_callback(int fd, mixed message) {}
void create_close_callback(int fd)
{
	int call_handle;
	object ob;

	call_handle = idents[fd][IDENT_CALL_HANDLE];

	if( ob = idents[fd][IDENT_COMMANDER] )
		tell(ob, "\n[FD: " + fd + "] �ϥΪ̥D���ڵ��s�u�C\n", CMDMSG);
	if( find_call_out(call_handle) != -1 ) remove_call_out(call_handle);
	socket_close(fd);
	return;
}
void write_callback(int fd)
{
	int res;
	object ob, user;

	if( !(ob = idents[fd][IDENT_COMMANDER]) )
	{
		socket_close(fd);
		return;
	}
	if( !(user = idents[fd][IDENT_USER]) )
	{
		tell(ob, "\n[FD: " + fd + "] �ϥΪ̤w���u�C\n", CMDMSG);
		socket_close(fd);
		return;
	}
	tell(ob, "\n[FD: " + fd + "] ���o�ϥΪ̸�T ...\n", CMDMSG);
	res = socket_write(fd, idents[fd][IDENT_RPORT] + "," + idents[fd][IDENT_PORT] + "\n");
	if( res < 0 )
	{
		tell(ob, "[FD: " + fd + "] �T���ǰe���~�G" + socket_error(res), CMDMSG);
		socket_close(fd);
	}
}
/*
 * receive ident response
 *
 * format of response (string):
 *     their_port, our_port : USERID : operating_system : user_id
 *     their_port, our_port : USERID : OTHER : user_id_octet_string
 *     their_port, our_port : ERROR : INVALID-PORT
 *     their_port, our_port : ERROR : NO-USER
 *     their_port, our_port : ERROR : HIDDEN-USER
 *     their_port, our_port : ERROR : UNKNOWN-ERROR
 *
 */
void read_callback(int fd, mixed info)
{
	int rport, port;
	string type, arg, os, uid;
	object ob, user;

	remove_call_out(idents[fd][IDENT_CALL_HANDLE]);

	if( !(ob = idents[fd][IDENT_COMMANDER]) )
	{
		socket_close(fd);
		return;
	}
	if( !(user = idents[fd][IDENT_USER]) )
	{
		tell(ob, "\n[FD: " + fd + "] �ϥΪ̤w���u�C\n", CMDMSG);
		socket_close(fd);
		return;
	}
	if( !info || sscanf(info, "%d,%d : %s : %s", rport, port, type, arg) != 4 )
	{
		tell(ob, "\n[FD: " + fd + "] �Ǧ^�T���榡���~�C\n", CMDMSG);
		socket_close(fd);
		return;
	}
	if( type == "USERID" && (sscanf(arg, "%s :%s", os, uid) == 2) )
	{
		if( uid[<1] == '\n' ) uid = uid[0..<2];
		if( uid[<1] == '\r' ) uid = uid[0..<2];
		tell(ob, "\n[FD: " + fd + "]\n", CMDMSG);
		tell(ob, user->query_idname() + "���D����T�p�U�G\n", CMDMSG);
		tell(ob, "�@�~�t�ΡG" + os + "\n", CMDMSG);
		tell(ob, "�ϥΪ̱b���G" + uid + "\n", CMDMSG);
	}
	else if( type == "ERROR" )
	{
		tell(ob, "\n[FD: " + fd + "]\n", CMDMSG);
		tell(ob, user->query_idname() + " ���D����T�p�U�G\n", CMDMSG);
		tell(ob, "���~�G" + arg + "\n", CMDMSG);
	}
	socket_close(fd);
	return;
}
private void time_out(int fd)
{
	object ob;

	if( !(ob = idents[fd][IDENT_COMMANDER]) )
	{
		socket_close(fd);
		return;
	}
	tell(ob, "\n[FD: " + fd + "] �s�u�U�ɡC\n", CMDMSG);
	socket_close(fd);
	return;
}
