/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : master.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-24
 * Note   : �s����ɭ��O
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
	set_idname(HIB"master"NOR BLU" board"NOR, HIB"�s��"NOR BLU"����"NOR);
	
	// �Q�� shadow_ob �޳N�Ӱ����������G��, �Y�ɸ�Ƭ��x�s�� shadow_ob �W
	if( clonep() ) 
		set("shadow_ob", find_object(base_name(this_object())));
	
	else
	{
		set("boardid", "master");
		set("capacity", 2000);
		
		initialize_board();
	}
	startup_action();
}
