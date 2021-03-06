/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : build
 * Author : Cookys@RevivalWorld
 * Date   : 2001-06-19
 * Note   : (一般玩家)地圖編輯指令, 只能建造牆壁與一些小景觀。
 * Update :
 *  o 2001-07-09 Clode 改寫指令 edit_map 成為 build
 *
 -----------------------------------------
 */
 
/*
├                                                            ┤
｜景  8. 花叢   ζ       11. 大樹   ¥        14. 水池   ☉   ｜
｜    9. 草皮   ω       12. 柳樹   η        15. 路燈   Γ   ｜
｜觀  10. 矮叢   φ      13. 涼亭   ☆        16. 雕像   〥   ｜
*/

#include <map.h>
#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <material.h>
#include <message.h>
#include <location.h>

inherit COMMAND;

string help = @HELP
        標準 build 指令。
HELP;

string list = @LIST
┌                                                            ┐
｜牆  ＱＷＥ  ╭─╮ �翦��� �鵂w�� ┌─┐ �歈���  6. 直門  ∥ ｜
｜    Ａ  Ｄ  │１│ │２│ �齰莓� │４│ �齰渺�  	      ｜
｜壁  ＺＸＣ  ╰─╯ �檛��� �鶔w�� └─┘ �裺���  7. 橫門  ＝ ｜
├                                                            ┤
｜8. 農田   9. 清除   0.離開                                  ｜
└                                                            ┘
 ─取消請輸入"quit"─移平整棟建築請輸入"clean"─
 
LIST;

mapping wall_type = ([
"q"	:	0,
"e"	:	1,
"z"	:	2,
"c"	:	3,
"w"	:	4,
"x"	:	4,
"a"	:	5,
"d"	:	5,
]);

mapping obj = ([
1	:	({ "╭","╮","╰","╯","─","│" }),
2	:	({ "��","��","��","��","��","│" }),
3	:	({ "��","��","��","��","─","��" }),
4	:	({ "┌","┐","└","┘","─","│" }),
5	:	({ "��","��","��","��","��","��" }),
6	:	"∥",
7	:	"＝",
8	:	ansi("$HIG$$BGRN$〃$NOR$"),
]);

void build_wall(string pic, object me)
{
	array loc = query_temp("location", me);
	(MAP_D->query_map_system(loc))->set_coor_data(loc, TYPE, WALL);
	(MAP_D->query_map_system(loc))->set_coor_icon(loc, pic);
	tell(me, pnoun(2, me)+"在這塊地上造起了一面'"+pic+"'牆壁。\n", CMDMSG);
}

/*
void build_scenery(string pic, object me)
{
	array loc = query_temp("location", me);
	(MAP_D->query_map_system(loc))->set_coor_data(loc[X], loc[Y], loc[CITY], loc[NUM], TYPE, SCENERY);
	(MAP_D->query_map_system(loc))->set_coor_icon(loc[X], loc[Y], loc[CITY], loc[NUM], pic);
	tell(me, pnoun(2, me)+"在這塊地上建造了'"+pic+"'景觀。\n", CMDMSG);
}
*/

void build_farm(string pic, object me)
{
	array loc = query_temp("location", me);
	(MAP_D->query_map_system(loc))->set_coor_data(loc, TYPE, FARM);
	(MAP_D->query_map_system(loc))->set_coor_icon(loc, pic);
	msg("$ME在這塊地上犁土，準備作為農用田地。\n",me,0,1);
	tell(me, pnoun(2, me)+"可以 help here 學習農田的灌概方法。\n", CMDMSG);
}
	
void build_door(string pic, object me)
{
	array loc = query_temp("location", me);
	(MAP_D->query_map_system(loc))->set_coor_data(loc, TYPE, DOOR);
	(MAP_D->query_map_system(loc))->set_coor_icon(loc, pic);
	tell(me, pnoun(2, me)+"在這塊地上建造一扇門'"+pic+"'。\n", CMDMSG);
}

void clean_build(object me)
{
	array loc = query_temp("location", me);
	(MAP_D->query_map_system(loc))->delete_coor_data(loc, TYPE);
	(MAP_D->query_map_system(loc))->set_coor_icon(loc, "．");
	tell(me, "清除建築完畢。\n", CMDMSG);
}

void select_icon(object me, string arg)
{
	int choice, type;
	mixed value;
	arg = lower_case(arg);
	
	if( sscanf( arg, "%d%s", choice, arg ) != 2 )
	{
		tell(me, list+"請輸入列表上的英文或數字。<建造牆壁請輸出數字加英文的組合。例：2a>\n:", CMDMSG);
		input_to((: select_icon, me :));
		return;
	}
	
	if( (value = obj[choice]) )
	{
		switch(choice)
		{
			case 1..5:
				if( arg=="" || !intp(type = wall_type[arg]) )
				{
					tell(me, list+"建造牆壁請輸出數字加英文的組合。例：2a。\n:", CMDMSG);
					input_to((: select_icon, me :));
					return;
				}
				build_wall(value[type], me);
				break;
			case 6..7:
				build_door(value, me);
				break;
			case 8:
				build_farm(value, me);
				break;
			case 9:
				clean_build(me);
				break;
			case 0:
				return tell(me, "取消建築指令。\n", CMDMSG);
				break;
			default:
				tell(me, list+"\n請輸入數字選擇"+pnoun(2, me)+"要建造的東西。< 建造牆壁請輸出數字加英文。例：2a >\n:", CMDMSG);
				input_to((: select_icon, me :));
		}
	}
}
	
private void command(object me, string arg)
{
	string owner;
	array loc = query_temp("location", me);

	if( !is_command() ) return;
	
	if( !arrayp(loc) || !environment(me)->is_maproom() ) 
		return tell(me, pnoun(2, me)+"不能在這裡建造牆壁或景觀。\n", CMDMSG);
	
	owner = (MAP_D->query_map_system(loc))->query_coor_data(loc, "owner");
	
	/*
	if( owner != me->query_id(1) )
	{
		if( owner == "GOVERNMENT" )
		{
			if( CITY_D->query_city_info(environment(me)->query_city(), "official/mayor") != me->query_id(1) )
				return tell(me, "這塊地是市府用地，"+pnoun(2, me)+"沒有權利在這上面建造任何東西。\n", CMDMSG);
		}
		else if( owner )
			return tell(me, "這塊地是 "+owner+" 的，"+pnoun(2, me)+"沒有權利在這上面建造任何東西。\n", CMDMSG);
		//else if( CITY_D->city_exist(loc[CITY]) )
		//	return tell(me, "無法在城市中任意建造建築，必須依照正常的購買土地程序進行。\n", CMDMSG);
	}
	*/
	if( arg )
		return select_icon(me, arg);
		
	tell(me, list+"\n請輸入數字選擇"+pnoun(2, me)+"要建造的東西。< 建造牆壁請輸出數字加英文。例：2a >\n:", CMDMSG);
	input_to((: select_icon, me :));
}
