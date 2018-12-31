/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _message_usr.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-6
 * Note   : �ϥΪ̰T���~����
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <function.h>
#include <message.h>

#define MAX_MSG_BUFFER		1000
#define MAX_LAST_MSG		1000

#define BEGIN	NOR WHT"\n�w�w["HIW"�H�U�O�Ȧs�T��"NOR WHT"]�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR
#define END	NOR WHT"�w�w["HIW"�H�W���Ȧs�T��"NOR WHT"]�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n\n"NOR

nosave string *msg_buffer = allocate(0);
nosave array msg_log = allocate(0);

void show_msg_buffer()
{
	if( sizeof(msg_buffer) )
	{
		tell(this_object(), BEGIN + implode(msg_buffer, "") + END);

		msg_buffer = allocate(0);
	}
}

void clear_msg_buffer()
{
	msg_buffer = allocate(0);
}

array query_msg_log()
{
	return msg_log;
}
	
void add_msg_log(string type, string msg)
{
	if( !query("msg/"+type) ) return;

	if( !wizardp(this_object()) )
	{
		if( sizeof(msg_log)/3 >= (query("msg/size") || MAX_LAST_MSG) )
			msg_log = msg_log[3..] + ({ time(), type, msg });
		else
			msg_log += ({ time(), type, msg });
	}
	else
	{
		if( sizeof(msg_log)/3 >= (query("msg/size") || 20000 ) )
			msg_log = msg_log[3..] + ({ time(), type, msg });
		else
			msg_log += ({ time(), type, msg });
	}
}

void clear_msg_log()
{
	msg_log = allocate(0);
}

//
// �Ҧ��T����X���ߤ@�q�D, �䴩 client ���T�����O����
//
nomask varargs void catch_tell(string msg, string classes)
{
	if( !msg || !msg[0] ) return;

	if( undefinedp(classes) || (stringp(classes) && classes != "snoop") )
		catch(COMMAND_D->find_command_object("snoop")->notify_snooper_catch(this_object(), msg));

	if(wizardp(this_object()) && query_temp("secondary_message_handler")) {
		foreach(function f in query_temp("secondary_message_handler")) {
			if(!(functionp(f) & FP_OWNER_DESTED))
				(*f)(msg, classes);
		}
	}

	// �Ȧs�T��
	if( !interactive() || in_input() || (in_edit() && msg != ":" && msg != sprintf("%c%c",42,8)) )
	{
		if( !noansi_strlen(msg) )
			return;
		
		if( sizeof(msg_buffer) > MAX_MSG_BUFFER )
			msg_buffer = msg_buffer[1..];

		msg_buffer += ({ msg });
		return;
	}


	// �_���@���T��
	if( query_temp("disable/msg") && !wizardp(this_object()) ) return;

	if( query("encode/gb") ) msg = B2G(msg);

	if( query("translate/language") )
		TRANSLATE_D->translate(this_object(), msg, query("translate/language"));
	else
	while(strlen(msg) > 0) {
		receive(msg[0..__LARGEST_PRINTABLE_STRING__ - 1]);
		msg = msg[__LARGEST_PRINTABLE_STRING__..];
	}
}

nomask varargs void directly_receive(string msg)
{
	while(strlen(msg) > 0) {
		receive(msg[0..__LARGEST_PRINTABLE_STRING__ - 1]);
		msg = msg[__LARGEST_PRINTABLE_STRING__..];
	}
}


//
// ��z snoop �T��
//
nomask void receive_snoop(string message)
{
	string msgs = "";
	foreach( string msg in explode(message,"\n") )
	msgs += NOR WHT"-"+msg+"\n"NOR;

	catch_tell(msgs);
}

nomask void show_prompt()
{
	string prompt;

	if( !objectp(this_object()) )
		return;

	if( wizardp(this_object()) )
		prompt = (query("current_work/directory") || "")+"> ";
	else
		prompt = "> ";

	if( query("encode/gb") ) prompt = B2G(prompt);

	receive(prompt);
}

nomask void finish_input()
{
	show_msg_buffer();
	show_prompt();
}

private nomask void window_size(int width, int height)
{
        set("windowsize", ([ 
		"width":width, 
		"height":height,
	]));
}
