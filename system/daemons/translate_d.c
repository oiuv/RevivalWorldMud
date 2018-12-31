/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : translate_d.c
 * Author : Clode@Revival World
 * Date   : 2006-09-08
 * Note   : ½Ķ���F
 * Update :
 *  o 2006-00-00 �Q�� Google Translate �s�@
 *
 -----------------------------------------
 */
#include <ansi.h>
#include <socket.h>
#include <socket_err.h>
#include <message.h>

string strs="";

mapping connects = allocate_mapping(0);
private void get_translated_message(int);

void create() { }

void translate(object user, string sentence, string language)
{
	int err, fd;
	string replaced_sentence;
	//string color;

	if( !objectp(user) || !stringp(sentence) ) return ;

	/* �}�Һ����s�� */
	fd = socket_create(STREAM, "read_callback", "close_socket");

	if ( fd < 0 )
		return user->directly_receive(HIR"½Ķ����, �еy��b�աC\n"NOR);

	err = socket_connect(fd, "64.233.189.104 80", "read_callback", "write_callback" );

	if( err != EESUCCESS )
		return user->directly_receive(HIR"½Ķ����, �еy��b�աC\n"NOR);     

	replaced_sentence = replace_string(sentence, "\n", "_LINE_");
	
	//replaced_sentence = replace_string(replaced_sentence, "�C", ".");
	//replaced_sentence = replace_string(replaced_sentence, "�G", ":");
	//replaced_sentence = replace_string(replaced_sentence, "�I", "!");
	//replaced_sentence = replace_string(replaced_sentence, "�A", ",");
	//replaced_sentence = replace_string(replaced_sentence, "�u", "\"");
	//replaced_sentence = replace_string(replaced_sentence, "�v", "\"");

	replaced_sentence = replace_string(replaced_sentence, " ", "%20");
	
	//while( sscanf(replaced_sentence, "\e[%sm", color) > 0 )
	//	replaced_sentence = replace_string(replaced_sentence, "\e["+color+"m", "_COLOR"+color+"_");

	connects[fd] = allocate_mapping(5);
	connects[fd]["translated_message"] = "";
	connects[fd]["user"] = user;
	connects[fd]["sentence"] = sentence;
	connects[fd]["replaced_sentence"] = replaced_sentence;
	connects[fd]["language"] = language;

	return ;
}

int close_socket(int fd)
{
	socket_close(fd);

	get_translated_message(fd);

	map_delete(connects, fd);
}

void read_callback(int fd, mixed message)
{
	/* ��C�����o����Ʀs�i strs �� */
	connects[fd]["translated_message"] += message;

	socket_write(fd, "\n");

	return ;
}

void write_callback(int fd)
{
	/* ��J���O����ӭ����e */
	socket_write(fd, sprintf("GET /translate_t?ie=Big5&oe=Big5&langpair=zh-CN|%s&text=%s HTTP/1.1\n\n", connects[fd]["language"], remove_ansi(connects[fd]["replaced_sentence"])));

	return ;
}

private void get_translated_message(int fd)
{
	string translated_message;
	//string color;

	if( sscanf(connects[fd]["translated_message"],"%*s<textarea %*s>%s</textarea>%*s", translated_message) != 4 )
	{
		connects[fd]["user"]->directly_receive(HIR"½Ķ���ѡG\n"NOR+connects[fd]["sentence"]);
		return ;
	}

	translated_message = replace_string(translated_message, "_LINE_", "\n");

	//while( sscanf(translated_message, "_COLOR%s_", color) > 0 )
	//	translated_message = replace_string(translated_message, "_COLOR"+color+"_", "\e["+color+"m");

	connects[fd]["user"]->directly_receive(translated_message+"\n");

	return ;
}

mapping query_connects()
{
	return connects;
}

string query_name()
{
	return "½Ķ�t��(TRANSLATE_D)";
}

