/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : login_ob.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-30
 * Note   : 登錄物件主檔
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#define LOGIN_TIME_LIMIT	600

#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <login.h>

/* Ident 檢查 */
inherit IDENT_USR;

nosave mapping database;

nomask int is_login_ob()
{
	return 1;
}

nomask int get_ident();

private nomask object logon()
{
	string daemon;

	switch(query_ip_port())
	{

		case PPL_PORT:
		case CLIENT_PORT:
			daemon = PPL_LOGIN_D;
			break;
		case WIZ_PORT:
			daemon = WIZ_LOGIN_D;
			break;
		default:
			return 0;
	}

	if( catch( daemon->logon( this_object() ) ) )
	{
		tell(this_object(), @MSG
	連線程式正在修改中﹐請稍後再嘗試連線。 
	The program of connection is being modified now, 
	please try later.
	
MSG, CONNECTFAILED);
		destruct(this_object());
		return 0;
	}

	get_ident();

	return this_object();
}

nomask void catch_tell(string msg, string classes)		
{
	if( query("encode/gb") ) msg = B2G(msg);

	while(strlen(msg) > 0) {
		receive(msg[0..__LARGEST_PRINTABLE_STRING__ - 1]);
		msg = msg[__LARGEST_PRINTABLE_STRING__..];
	}
}

nomask void directly_receive(string msg)
{
	receive(msg);
}

private nomask void terminal_type(string term_type)
{
	set("terminaltype", term_type);
}

private nomask void window_size(int width, int height)
{
	set("windowsize", ([ 
		"width":width, 
		"height":height,
	    ]));
}

nomask void reset_database()
{
	database = allocate_mapping(0);
}

private nomask void dest() 
{ 
	destruct(this_object()); 
}

private nomask void net_dead() 
{ 
	call_out( (: dest :), 1 ); 
}

private nomask void time_out()
{
	catch_tell("\n你登入所花的時間已超過 "+LOGIN_TIME_LIMIT/60+" 分鐘，請重新登入。\n", CONNECTFAILED);
	evaluate( (: dest :) );
}

nomask void reset_time_out()
{
	remove_call_out(query("callout"));
	set("callout", call_out((: time_out :), LOGIN_TIME_LIMIT) );
}

private void create()
{
	reset_database();
	if( clonep(this_object()) )
		set("callout", call_out((: time_out :), LOGIN_TIME_LIMIT) );
}

int remove()
{
	//catch_tell("\n登入系統正在修改中，請重新登入。\n", CONNECTFAILED);
}