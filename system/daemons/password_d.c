/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : password_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-25
 * Note   : �K�X���F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>

#define DATA		"/system/kernel/data/password"+__SAVE_EXTENSION__
#define LOG		"daemon/password"

private mapping password;

private nomask int save_data()
{
	return save_object(DATA) && cp(DATA, DATA+"_backup");
}

nomask string query_password(string id)
{
	return copy(password[id]);
}

nomask int set_password(string id, string pwd)
{
	object *stack = call_stack(1);
	
	if( !stringp(id) || !stringp(pwd) || strlen(pwd) < 4 ) return 0;
	
	if( stack[<1] != load_object(PPL_LOGIN_D) && !is_command(stack[2]) ) 
		return 0;
	
	if( password[id] ) 
		log_file(LOG, sprintf("�ק� %s �K�X�� %s�C\n", id, pwd));
	else 
		log_file(LOG, sprintf("�s�W %s �K�X�� %s�C\n", id, pwd));
	
	password[id] = pwd;
	
	return save_data();
}

nomask void del_password(string id)
{
	object *stack = call_stack(1);
	
	if( !stringp(id) || undefinedp(password[id]) ) return;
	
	if( !is_command(stack[2]) ) return;
	
	log_file(LOG, sprintf("�R�� %s �K�X�C\n", id));
	
	map_delete(password, id);
	
	save_data();
}

private nomask void create()
{
  	if( clonep() ) destruct(this_object());
  	
 	if( !restore_object(DATA) )
  	{
		log_file(LOG, "�L�k���J�K�X�s�ɡC\n");
		password = allocate_mapping(0);
  	}
  	else
  		/* ��z�L�Τ��K�X��T */
  		filter(password, (: file_size(user_data($1)) < 0 && map_delete(password, $1):));
	
	save_data();
}
string query_name()
{
	return "�K�X�t��(PASSWORD_D)";
}
