/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : story.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-03
 * Note   : �ͬ��G�ƪO
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
	set_idname(HIY"story"NOR YEL" board"NOR, HIY"��"NOR YEL"�߱o"NOR);
	
	// �Q�� shadow_ob �޳N�Ӱ����������G��, �Y�ɸ�Ƭ��x�s�� shadow_ob �W
	if( clonep() ) 
		set("shadow_ob", find_object(base_name(this_object())));
	
	else
	{
		set("boardid", "story");
		set("capacity", 2000);
		
		initialize_board();
	}
	startup_action();
}
