/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : idea.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-15
 * Note   : �c�Q�ب��O
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
	set_idname(HIM"idea"NOR MAG" board"NOR, HIM"�c�Q"NOR MAG"�ب�"NOR);
	
	// �Q�� shadow_ob �޳N�Ӱ����������G��, �Y�ɸ�Ƭ��x�s�� shadow_ob �W
	if( clonep() ) 
		set("shadow_ob", find_object(base_name(this_object())));
	else
	{
		set("boardid", "idea");
		set("capacity", 2000);
		
		initialize_board();
	}
	startup_action();
}
