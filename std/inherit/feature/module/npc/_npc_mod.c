/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _npc_mod.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-18
 * Note   : npc module
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>

int is_module_npc()
{
	return 1;
}

int remove()
{
	return this_object()->save();
}

// 由 Virtual_D 呼叫並送入虛擬物件名稱 file
varargs void create(string file)
{
	if( !stringp(file) ) return;

	// 若是第一次創立物件, 則設定基本資料
	if( !this_object()->restore(file) ) 
	{
		this_object()->set_idname("npc", "剛出生的 NPC");
		CHAR_D->create_npc(this_object());
		this_object()->save(file);
	}
	
	this_object()->restore_inventory();

	//set_heart_beat(1);
}
