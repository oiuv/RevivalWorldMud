/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_5.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : 標準房間
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <quest.h>

inherit STANDARD_ROOM;

string help = @HELP
在這個平凡無奇的房間內並沒有任何特殊的資訊。
HELP;

void create()
{
	set("short", HIW"隱密"NOR WHT"山洞"NOR);
	set("long",@TEXT
    這裡是一個極為隱密的山洞，裡面一片漆黑，往手邊摸去都是冷冰冰且潮濕的
石璧，頭頂上也不時地滴下水來，不過往裡頭一看，似乎格局還挺方正的，地上也
可以偶爾見到幾株正在生長的野山人參，而在這個方正的山洞正中央則有一個很顯
著的[1;33m黃玉石柱[m直直插在地上，完美無暇的玉石讓人看得目瞪口呆。
TEXT);

	set("exits", ([
		"south" 	: "/quest/yin_ling_yu/room_6",
		"north" 	: "/quest/yin_ling_yu/room_4",
	]));
	
	::reset_objects();
}

void do_push(object me, string arg)
{
	if( arg != "左" && arg != "右" )
		return tell(me, pnoun(2, me)+"想要把石柱往左右哪個方向推(左, 右)？\n");
		
	if( !query_temp("/quest/yin_ling_yu/stone/green", me) )
		return tell(me, pnoun(2, me)+"使出吃奶的力量也推不動這個石柱。\n");

	if( lower_case(arg) == "左" )
	{
		set_temp("/quest/yin_ling_yu/stone/yellow", 1, me);
		msg("$ME用力將石柱往左邊推，只感覺到地板下似乎有東西在震動。\n", me, 0, 1);
		return;
	}
	else if( lower_case(arg) == "右" )
	{
		set_temp("/quest/yin_ling_yu/stone/yellow", 2, me);
		msg("$ME用力將石柱往右邊推，只感覺到地板下似乎有東西在震動...。\n", me, 0, 1);
		return;
	}
}

mapping actions = 
([
	"push" : (: do_push :),
]);