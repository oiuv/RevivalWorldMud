/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : idea.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-15
* Note   : �±N���D�P��ĳ�O
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
	set_idname(HIY"mj idea"NOR YEL" board"NOR, HIY"�±N"NOR YEL"���D�P��ĳ�O"NOR);

	// �Q�� shadow_ob �޳N�Ӱ����������G��, �Y�ɸ�Ƭ��x�s�� shadow_ob �W
	if( clonep() ) 
		set("shadow_ob", find_object(base_name(this_object())));
	else
	{
		set("boardid", "mj_idea");
		set("capacity", 2000);

		initialize_board();
	}
	startup_action();
}
