/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _save.c
 * Author : Clode@RevivalWorld
 * Date   : 2000-12-30
 * Note   : �x�s�PŪ�����|�{��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

// �u���\�H�U���|�U������i�H�i���x�s
#define VALID_SAVE_PATH	\
({			\
"/area",		\
"/city",		\
"/data",		\
"/product",		\
"/npc",			\
})

varargs nomask int save(string file)
{
	if( !stringp(file) )
		file = this_object()->query_save_file() || base_name(this_object());

	// �u���\�S�w���|���x�s�ʧ@
	if( member_array(file[0..strsrch(file[1..], "/")], VALID_SAVE_PATH) == -1 )
		return 0;

	// �u����l�����v�Q�x�s���
	if( file_name(this_object()) != base_name(this_object()) )
		return 0;

	// �Y�� nosave() �h���x�s
	if( this_object()->no_save(file) )
		return 0;

	// �إߧ�����|
	intact_path(file);

	if( userp(this_object()) || this_object()->is_module_npc() )
		this_object()->save_inventory();
	
	if( !save_object(file) )
		log_file("save_failed", file);
	else
		return 1;
}

varargs nomask varargs int restore(string file, int i)
{
	if( undefinedp(file) )
		file = this_object()->query_save_file() || base_name(this_object());
		
	if( !stringp(file) )
		return 0;

	if( restore_object(file, i) )
	{
		this_object()->reset_objects();
		return 1;
	}
	else
		return 0;
}
