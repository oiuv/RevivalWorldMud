/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ed.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

// save_ed_setup and restore_ed_setup are called by the ed to maintain
// individual options settings. These functions are located in the master
// object so that the local admins can decide what strategy they want to use.
/* �x�s�@�ӨϥΪ̪��s��{���]�w�βպA�]�w */
int save_ed_setup(object user, int code)
{
    	if ( !intp(code) ) return 0;

	return set("edit_setup", code, user);
}

// Retrieve the ed setup. No meaning to defend this file read from
// unauthorized access.
/* ���o�ϥΪ̪��s��{���]�w (setup) �βպA�]�w */
int retrieve_ed_setup(object user)
{
    	return query("edit_setup", user);
}

/* �ϥΪ̤����`�_�u�ɡA�ƥ���s�褺�e */
string get_save_file_name( string file, object user ) 
{
	return wiz_home(user->query_id(1))+"/unfinished_edit";
}

/* �� ed() �ഫ��Ū�g�ɮת��۹���|�W�٬�������|�W�� */
/* ���|�������w by clode */
string make_path_absolute(string file)
{
	return file;
}