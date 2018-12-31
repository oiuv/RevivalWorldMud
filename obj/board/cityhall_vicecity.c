/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cityhall_vicecity.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-21
 * Note   : ���F���d���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <daemon.h>

inherit BULLETIN_BOARD;

void create()
{
	set_idname("cityhall board", CITY_D->query_city_name("vicecity")+"�d���O");
	
	// �Q�� shadow_ob �޳N�Ӱ����������G��, �Y�ɸ�Ƭ��x�s�� shadow_ob �W
	if( clonep() ) 
		set("shadow_ob", find_object(base_name(this_object())));
	else
	{
		set("city", "vicecity");
		set("boardid", "cityhall_vicecity");
		set("capacity", 2000);
		
		initialize_board();
	}
	startup_action();
}
