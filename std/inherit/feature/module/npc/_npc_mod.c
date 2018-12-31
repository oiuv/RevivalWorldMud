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

// �� Virtual_D �I�s�ðe�J��������W�� file
varargs void create(string file)
{
	if( !stringp(file) ) return;

	// �Y�O�Ĥ@���Хߪ���, �h�]�w�򥻸��
	if( !this_object()->restore(file) ) 
	{
		this_object()->set_idname("npc", "��X�ͪ� NPC");
		CHAR_D->create_npc(this_object());
		this_object()->save(file);
	}
	
	this_object()->restore_inventory();

	//set_heart_beat(1);
}
