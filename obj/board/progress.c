/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : progress.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-3-15
 * Note   : �o�i�W���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit BULLETIN_BOARD;

void create()
{
	set_idname("progress board", "�o�i�W���O");
	
	// �Q�� shadow_ob �޳N�Ӱ����������G��, �Y�ɸ�Ƭ��x�s�� shadow_ob �W
	if( clonep() ) 
		set("shadow_ob", find_object(base_name(this_object())));
	
	else
	{
		set("boardid", "progress");
		set("capacity", 2000);
		
		initialize_board();
	}
	startup_action();
}
