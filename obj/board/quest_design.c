/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : quest_design.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-10
 * Note   : ���ȳ]�p
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit BULLETIN_BOARD;

void create()
{
	set_idname("quest degisn", "���ȳ]�p");
	
	// �Q�� shadow_ob �޳N�Ӱ����������G��, �Y�ɸ�Ƭ��x�s�� shadow_ob �W
	if( clonep() ) 
		set("shadow_ob", find_object(base_name(this_object())));
	else
	{
		set("boardid", "quest_design");
		set("capacity", 2000);
		
		initialize_board();
	}
	startup_action();
}
