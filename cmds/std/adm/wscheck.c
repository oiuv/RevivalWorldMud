/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wscheck.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-07
 * Note   : dump ���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
#include <socket.h>

inherit COMMAND;

string help = @HELP
        �ˬd���a�O�_�ϥκæ��u�@�������O
HELP;

object *checking_user;
int *check_ip = ({ 21, 22, 23, 25, 80, 110 });
mapping data;

void show_result()
{
	string result = "�æ��u�@���� IP �p�U�G\n";

	foreach(string key in sort_array(data["result"], 1))
		result += key +"\n";
	
	foreach( object ob in checking_user )
	{
		if( objectp(ob) )
			tell(ob, result);
	}
	
	data = allocate_mapping(0);
	checking_user = allocate(0);
}

void read_callback(int socket, mixed info) 
{
	data["result"] += ({ data["socket"][socket] });
	socket_close(socket);
}

void check_ip(int port)
{
	int socket;

	foreach(object ob in checking_user)
	{
		if( objectp(ob) )
			tell(ob, sizeof(data["ip"])+". �d�� IP "+data["ip"][0]+":"+port+" ��...�C\n");
	}
			
	socket = socket_create(STREAM, "create_read_callback", "create_close_callback");

	if( socket >= 0 && socket_connect(socket, data["ip"][0] + " "+port, "read_callback", "write_callback") < 0 ) 
	{
		socket_close(socket);
		return;
	}

	data["socket"][socket] = data["ip"][0]+":"+port;
}

void heart_beat()
{
	if( sizeof(checking_user -= ({ 0 })) )
	{
		if( sizeof(data["ip"]) )
		{
			foreach(int port in check_ip)
				check_ip(port);
			
			data["ip"] = data["ip"][1..];
		}
		else
		{
			foreach(object ob in checking_user)
			{
				if( objectp(ob) )
					tell(ob, "��z���G��...�C\n");
			}
			set_heart_beat(0);
			call_out( (: show_result :), 5);
		}
	}
	else
		set_heart_beat(0);
}

private void command(object me, string arg)
{
	if( !is_command() ) return;
 
 	if( sizeof(checking_user) )
 	{
 		checking_user |= ({ me });
 		return tell(me, "���b�d�߸�Ƥ�...�еy�ԡC\n");
 	}
 	else
 		checking_user = ({ me });
 		
 	data = allocate_mapping(0);
 	data["ip"] = allocate(0);
 	data["socket"] = allocate_mapping(0);
 	data["result"] = allocate(0);
 	
	foreach(object user in users())
		data["ip"] |= ({ query_ip_number(user) });
	
	set_heart_beat(1);
}
