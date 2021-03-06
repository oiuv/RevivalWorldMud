/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_2.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-18
 * Note   : 標準房間
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_ROOM;

string help = @HELP
在這個平凡無奇的房間內並沒有任何特殊的資訊。
HELP;

void create()
{
	set("short", WHT"良凡環的房間"NOR);
	set("long",@TEXT
    這裡是良凡環的房間，到處擺滿了各種的盆栽與布廉，從房間裡各種細心優雅
的擺設看來，似乎不是一個農夫該有的擺設，可以看出這裡應該有經過女孩子的巧
手仔細裝潢過。
TEXT);

	set("exits", ([
		"east" : "/quest/yin_ling_yu/room_1",
	]));
	
	::reset_objects();
}


void do_search(object me, string arg)
{
	if( !arg || arg=="" )
		return tell(me, pnoun(2, me)+"想要搜尋什麼？\n");

	if( strsrch(arg, "布廉") != -1 )
	{
		msg("$ME試圖在布廉上找出一些線索，但被長布絆了一跤，狠狠地摔在地上。\n", me, 0, 1);
		me->faint();
	}
	else if( strsrch(arg, "盆栽") != -1 )
	{
		msg("$ME試圖在盆栽上找出一些線索，但不小心碰了一下將盆栽摔落，狠狠地砸在$ME的腳上。\n");
		me->faint();
	}
	else if( strsrch(arg, "擺設") != -1 || strsrch(arg, "裝潢") != -1 )
		tell(me, "四處放置著各種可愛又純樸的小飾物，讓人感到無比的溫馨。\n");
	else if( strsrch(arg, "女") != -1 )
		tell(me, "雖然不知道這位女孩是誰，但她一定是位溫柔顧家的女子。\n");
	else
		tell(me, pnoun(2, me)+"想要搜尋什麼？\n");	
}

mapping actions = 
([
	"search" : (: do_search :),
]);