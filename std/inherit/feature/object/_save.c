/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _save.c
 * Author : Clode@RevivalWorld
 * Date   : 2000-12-30
 * Note   : 儲存與讀取路徑程式
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

// 只允許以下路徑下的物件可以進行儲存
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

	// 只允許特定路徑的儲存動作
	if( member_array(file[0..strsrch(file[1..], "/")], VALID_SAVE_PATH) == -1 )
		return 0;

	// 只有原始物件有權利儲存資料
	if( file_name(this_object()) != base_name(this_object()) )
		return 0;

	// 若有 nosave() 則不儲存
	if( this_object()->no_save(file) )
		return 0;

	// 建立完整路徑
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
