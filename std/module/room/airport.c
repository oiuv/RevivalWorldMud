/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : airport.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-21
 * Note   : 國際機場
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <npc.h>
#include <condition.h>
#include <delay.h>

inherit ROOM_ACTION_MOD;


// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"toilet"	:
	([
		"short"	: HIY"公共廁所"NOR,
		"help"	:
			([
"topics":
@HELP
    車站內的公共廁所。
read			閱讀書籍
HELP,
			]),
		"heartbeat":0,	// 實際時間 1 秒為單位
		"job": 0,
		"action":
			([
				
			]),
	
	]),	
	"platform"	:
	([
		"short"	: HIY"月台"NOR,
		"help"	:
			([
"topics":
@HELP
    旅客搭車的月台。
HELP,
			]),
		"heartbeat":0,	// 實際時間 1 秒為單位
		"job": 0,
		"action":
			([

			]),
	
	]),	
	"lobby"	:
	([
		"short"	: HIY"車站"NOR YEL"大廳"NOR,
		"help"	:
			([
"topics":
@HELP
    購買車票的大廳。
HELP,
			]),
		"heartbeat":0,	// 實際時間 1 秒為單位
		"job": 0,
		"action":
			([

			]),
	]),
]);

// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIC"國際"NOR CYN"機場"NOR

	// 開張此建築物之最少房間限制
	,12

	// 城市中最大相同建築數量限制(0 代表不限制)
	,0

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,GOVERNMENT

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,COMMERCE_REGION

	// 開張儀式費用
	,"180000000"
	
	// 建築物關閉測試標記
	,1

	// 繁榮貢獻度
	,120
	
	// 最高可加蓋樓層
	,1
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,3
});
