/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : map_design.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-10
 * Note   : 地圖設計
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
	set_idname("map degisn", "地圖設計");
	
	// 利用 shadow_ob 技術來做分散式公佈欄, 即時資料皆儲存於 shadow_ob 上
	if( clonep() ) 
		set("shadow_ob", find_object(base_name(this_object())));
	else
	{
		set("boardid", "map_design");
		set("capacity", 2000);
		
		initialize_board();
	}
	startup_action();
}
