/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : edit_shadow.c
 * Author : Ekac@RevivalWorld
 * Date   : 2003-06-01
 * Note   : �Ψ����ɵ��� ed() �ᤣ�|���` prompt �����D�C
 * Update :
 *  o 2003-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>

#define ME this_player()

void create()
{
	if( clonep(this_object()) && ME && ( !in_edit(ME) || !shadow(ME) ) )
		destruct(this_object());
}

void write_prompt()
{
	object usr = query_shadowing(this_object());
	if( usr )
	{
		remove_shadow(this_object());
		usr->show_msg_buffer();
		usr->show_prompt();
	}
	destruct(this_object());
}
