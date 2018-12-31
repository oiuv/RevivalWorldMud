/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : idea.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-15
* Note   : 麻將問題與建議板
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
	set_idname(HIY"mj idea"NOR YEL" board"NOR, HIY"麻將"NOR YEL"問題與建議板"NOR);

	// 利用 shadow_ob 技術來做分散式公佈欄, 即時資料皆儲存於 shadow_ob 上
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
