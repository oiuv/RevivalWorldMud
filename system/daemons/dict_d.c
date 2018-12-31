/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : dict_d.c
 * Author : Whatup@Revival World
 * Date   : 2000-00-00
 * Note   : ½Ķ���F
 * Update :
 *  o 2000-00-00 �ثe�O�Q�Φ������@�d�� http://www.ee.tku.edu.tw/~rexchen/
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

void find_word(object user,string word)
{
	int err,fd;

	if(!user) return ;
	if(!word) return ;        

	/* �}�Һ����s�� */
	fd = socket_create(STREAM, "read_callback","close_socket");

	if ( fd < 0 )
	{
		return tell( user, HIR"�d�ߥ���, �еy��b�աC\n"NOR);             
	}

	err = socket_connect(fd,"163.13.132.61 80","read_callback","write_callback" );

	if( err!=EESUCCESS )
	{
		return tell( user, HIR"�d�ߥ���, �еy��b�աC\n"NOR);     
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
	    sprintf("GET /~rexchen/cdict/wwwcdict.cgi?word=%s\r\n"
		,code));
	return ;
}

private void get_data(int fd)
{
	string str;
	if(sscanf(connects[fd]["data"],"%*s<pre>%s</pre>%*s",str)!=3)
	{
		tell(connects[fd]["user"], HIR"�d�ߥ���, �еy��b�աC\n"NOR);
		return ;
	}
	str = replace_string(str,connects[fd]["simplex"],HIY+connects[fd]["simplex"]+NOR);
	str = sprintf(HIC"�z�d�ߪ���r�� : %s "NOR"\n%s\n%s", 
	    connects[fd]["simplex"],repeat_string("=",50),str);

	tell(connects[fd]["user"], str );

	return ;
}

string query_name()
{
	return "�r��t��(DICT_D)";
}
