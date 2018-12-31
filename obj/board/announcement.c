/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : announcement.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-06
 * Note   : �C�����i�O
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
	set_idname(HIC"board "NOR CYN" of announcement"NOR, HIC"�C��"NOR CYN"���i"NOR);
	
	// �Q�� shadow_ob �޳N�Ӱ����������G��, �Y�ɸ�Ƭ��x�s�� shadow_ob �W
	if( clonep() ) 
		set("shadow_ob", find_object(base_name(this_object())));
	
	else
	{
		set("boardid", "announcement");
		set("capacity", 2000);
		
		initialize_board();
	}
	startup_action();
}
