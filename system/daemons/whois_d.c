/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : whois_d.c
 * Author : sinji@Revival World
 * Date   : 2000-00-00
 * Note   : whois ���F
 *
 -----------------------------------------
 */
#include <ansi.h>
#include <socket.h>
#include <socket_err.h>
#include <message.h>

string strs="";

// string simplex; /* �d�ߪ���r */
// object tu;/* ��T���ǵ�user�ݨ� */
mapping connects = allocate_mapping(0);
private void get_data(int);

void create() { }

void find_ip(object user, string word)
{
	int err,fd;

	if(!user) return ;
	if(!word) return ;        

	/* �}�Һ����s�� */
	fd = socket_create(STREAM, "read_callback","close_socket");

	if ( fd < 0 )
	{
		return tell( user, HIR"�d�ߥ���, �еy��b�աC\n"NOR, CMDMSG);             
	}

	err = socket_connect(fd, "210.17.9.228 80","read_callback","write_callback" );

	if( err!=EESUCCESS )
	{
		return tell( user, HIR"�d�ߥ���, �еy��b�աC\n"NOR, CMDMSG);     
	}

	connects[fd] = allocate_mapping(3);
	connects[fd]["data"] = "";
	connects[fd]["user"] = user;
	connects[fd]["simplex"] = word;

	return ;
}

int close_socket(int fd)
{
	socket_close(fd);
	get_data(fd);
}

void read_callback(int fd, mixed message)
{
	/* ��C�����o����Ʀs�i strs �� */
	connects[fd]["data"] += message;
	return ;
}

void write_callback(int fd)
{
	string code="";
	code = connects[fd]["simplex"];
	/* ��J���O����ӭ����e */
	socket_write(fd,
	    sprintf("GET /whois.cgi?sld=IP&query=%s\n\n", code));
	return ;
}

private void get_data(int fd)
{
        connects[fd]["user"]->more(connects[fd]["data"] + "\n");
	return ;
}

string query_name()
{
	return "WHOIS �t��(WHOIS_D)";
}
