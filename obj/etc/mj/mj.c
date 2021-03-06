/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mj.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-12-10
 * Note   : 臺灣十六張麻將桌
 * Update :
 *  o 2003-04-15 Clode Update v1.05 to v1.06
    o 2003-04-17 Clode Update v1.06 to v1.07
    o 2003-04-19 Clode Update v1.07 to v1.08
    o 2003-05-25 Sinji Update v1.08 to v1.09
    o 2003-06-25 Ekac 讓 onlooker 也看的到打牌訊息
    o 2004-12-28 Clode Update v1.10 to v1.11 補強 AI
    o 2004-12-31 Clode Update v1.11 to v1.12 AI 程式獨立, 可自行發展各種 AI
    o 2005-03-06 Clode Update v1.12 to v1.13 修復部份重大 bug
    o 2006-11-14 Kyo   新增指令 skyeye 天眼通，巫師不用聽牌也可以看所有人牌XD
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <inherit.h>
#include <message.h>
#include "mj.h"

inherit STANDARD_OBJECT;

#define STATUS	HIY"麻"NOR YEL"將"NOR
#define OSTATUS	HIY"旁"NOR YEL"觀"NOR

nosave string help = @HELP
□ 臺灣正宗十六張麻將指令說明

[一般指令] -
join			- 加入牌局
leave			- 離開牌局
start			- 開始新牌局
restart 		- 重新開始牌局，玩家皆不改變，重開牌局
reset			- 清除牌局，所有玩家離開牌局
redeal			- 重新洗牌，特殊狀況時使用
next			- 繼續下一局
bill			- 看玩家籌碼
top			- 排行榜，排名可依下列參數排列 [mo][hu][gun][win][lose][bonus]
redraw			- 重新啟動畫面格式，畫面錯亂或斷線重連者適用
fixmj			- 補位修復牌局，若有人斷線或是消失

[遊戲指令] -
mo			- 摸牌
da <牌> 		- 打牌
pon			- 碰牌
eat <牌1> <牌2> 	- 吃牌，僅一種吃牌組合時可不需輸入牌名
gon <牌>		- 槓牌，暗槓時需輸入暗槓牌名
tin			- 聽牌
hu			- 胡牌
au pon <牌>		- 自動碰牌
au gon <牌>		- 自動槓牌
au hu <牌>		- 自動胡牌
au tin			- 自動聽牌
.			- 切換主畫面/海底畫面
lookall 		- 觀看其他玩家的牌
substitute <玩家>	- 替補

[其他指令] -
lookmj			- 觀局
bonus			- 台數計算表
HELP;

mapping score = allocate_mapping(0);

// 即時之麻將資料
nosave mapping MJdata;

// 玩家座位排序資料 ({ OB1, OB2, OB3, OB4 })
nosave object *Psort;

// 遊戲中玩家資料 ([ PLAYER:([...]), ...])
nosave mapping Pdata;

// 洗牌後的牌子順序資料
nosave string Ctile; 

// 臺灣十六張麻將牌型資料
nosave mapping MJtile=
([
    // 萬
    "1w":({"一萬",4,HIR,RED}),
    "2w":({"二萬",4,HIR,RED}),
    "3w":({"三萬",4,HIR,RED}),
    "4w":({"四萬",4,HIR,RED}),
    "5w":({"伍萬",4,HIR,RED}),
    "6w":({"六萬",4,HIR,RED}),
    "7w":({"七萬",4,HIR,RED}),
    "8w":({"八萬",4,HIR,RED}),
    "9w":({"九萬",4,HIR,RED}),
    // 索
    "1s":({"一索",4,HIG,GRN}),
    "2s":({"二索",4,HIG,GRN}),
    "3s":({"三索",4,HIG,GRN}),
    "4s":({"四索",4,HIG,GRN}),
    "5s":({"伍索",4,HIG,GRN}),
    "6s":({"六索",4,HIG,GRN}),
    "7s":({"七索",4,HIG,GRN}),
    "8s":({"八索",4,HIG,GRN}),
    "9s":({"九索",4,HIG,GRN}),
    // 筒
    "1t":({"一筒",4,HIC,CYN}),
    "2t":({"二筒",4,HIC,CYN}),
    "3t":({"三筒",4,HIC,CYN}),
    "4t":({"四筒",4,HIC,CYN}),
    "5t":({"伍筒",4,HIC,CYN}),
    "6t":({"六筒",4,HIC,CYN}),
    "7t":({"七筒",4,HIC,CYN}),
    "8t":({"八筒",4,HIC,CYN}),
    "9t":({"九筒",4,HIC,CYN}),
    // 字牌
    "ea":({"東風",4,HIY,YEL}),
    "so":({"南風",4,HIY,YEL}),
    "we":({"西風",4,HIY,YEL}),
    "no":({"北風",4,HIY,YEL}),
    "jo":({"紅中",4,HIR,RED}),
    "fa":({"青發",4,HIG,GRN}),
    "ba":({"白板",4,HIW,WHT}),
    "9t":({"九筒",4,HIC,CYN}),
    // 花牌
    "f1":({"1 春",1,HIW,HIR}),
    "f2":({"2 夏",1,HIW,HIR}),
    "f3":({"3 秋",1,HIW,HIR}),
    "f4":({"4 冬",1,HIW,HIR}),
    "f5":({"1 梅",1,HIR,WHT}),
    "f6":({"2 蘭",1,HIR,WHT}),
    "f7":({"3 竹",1,HIR,WHT}),
    "f8":({"4 菊",1,HIR,WHT}),
]);

void seat_select();
void master_select();
void wash_mj();
void begining_select();
void deal_tile();
void initialize_player(object player);
void win_process(object winner, int cheat);
int valid_hu_check(string *tile, string *otile, string *flower);
void show_tile(object me);
void show_sea(object me);
void show_all(object me);
mapping bonus_calculate(object winner);

void do_join(object me, string arg);
void do_leave(object me, string arg);
void do_start(object me, string arg);
void do_restart(object me, string arg);
void do_reset(object me, string arg);
void do_redeal(object me, string arg);
void do_redraw(object me, string arg);
void do_next(object me, string arg);
void do_look(object me, string arg);
void do_eat(object me, string arg);
void do_da(object me, string arg);
void do_mo(object me, string arg);
void do_pon(object me, string arg);
void do_gon(object me, string arg);
void do_au(object me, string arg);
void do_tin(object me, string arg);
void do_hu(object me, string arg);
void do_lookmj(object me, string arg);
void do_lookall(object me, string arg);
void do_skyeye(object me, string arg);
void do_substitute(object me, mixed arg);
void do_top(object me, string arg);
void do_bill(object me, string arg);
void do_bonus(object me, string arg);
void do_fixmj(object me, string arg);

nosave mapping actions = 
([
    "join"	:	(:do_join:),
    "leave"	:	(:do_leave:),
    "start"	:	(:do_start:),
    "restart"	:	(:do_restart:),
    "reset"	:	(:do_reset:),
    "redeal"	:	(:do_redeal:),
    "redraw"	:	(:do_redraw:),
    "." 	:	(:do_look:),
    "eat"	:	(:do_eat:),
    "da"	:	(:do_da:),
    "mo"	:	(:do_mo:),
    "pon"	:	(:do_pon:),
    "gon"	:	(:do_gon:),
    "au"	:	(:do_au:),
    "tin"	:	(:do_tin:),
    "hu"	:	(:do_hu:),
    "lookmj"	:	(:do_lookmj:),
    "lookall"	:	(:do_lookall:),
    "next"	:	(:do_next:),
    "skyeye"    :       (:do_skyeye:),
    "substitute":	(:do_substitute:),
    "top"	:	(:do_top:),
    "bill"	:	(:do_bill:),
    "bonus"	:	(:do_bonus:),
    "fixmj"	:	(:do_fixmj:),
]);

// 牌型排序
string *tile_sort(string *tile)
{
	return sort_array(tile, (: member_array($1, Tsort) > member_array($2, Tsort) ? 1 : -1 :));
}

// 檢查是否為玩家
int is_player(object me)
{
	return !undefinedp(Pdata[me]);
}

// 完整的牌字敘述
string full_chinese(string tile)
{
	if( member_array(tile, Tsort) == -1 ) return "ERROR";
	return kill_repeat_ansi(NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+NOR);
}

// 嗶嗶聲
varargs void beep(object me)
{
	if( undefinedp(me) )
		foreach( object ppl in Psort )
		tell(ppl, "\a", 0);
	else
		tell(me, "\a", 0);
}

// 檢查自動打牌功能, 自動取消多餘的自動碰槓功能
void auto_function_check(object ppl, string tile)
{
	if( member_array(tile, Pdata[ppl][AUTOPON]) != -1 )
	{
		tell(ppl, "取消自動碰"+full_chinese(tile)+"。\n", 0);
		Pdata[ppl][AUTOPON] -= ({tile});
	}

	if( member_array(tile, Pdata[ppl][AUTOGON]) != -1 )
	{
		tell(ppl, "取消自動槓"+full_chinese(tile)+"。\n", 0);
		Pdata[ppl][AUTOGON] -= ({tile});
	}
}

void clear_last_data()
{
	Ctile			= "";
	MJdata[DUMPED]		= allocate(0);
	MJdata[DUMPING] 	= 0;
	MJdata[TILELIMIT]	= 16;
	MJdata[FINISH]		= 0;
	MJdata[HUMYSELF]	= 0;
	MJdata[HUFIRST] 	= 0;
	MJdata[NINEYOUNGS]	= 0;
	MJdata[MO]		= 0;
	MJdata[TURN]		= 0;
	MJdata[WINNER]		= 0;
	MJdata[LOSER]		= 0;
	MJdata[KEYTILE] 	= 0;
	MJdata[LISTENING]	= 0;

	foreach( object player in Psort )
	{
		if( !objectp(player) )
			Psort -= ({ player });

		Pdata[player][TILE]		= allocate(0);
		Pdata[player][OTILE]		= allocate(0);
		Pdata[player][FLOWER]		= allocate(0);
		Pdata[player][AUTOPON]		= allocate(0);
		Pdata[player][AUTOGON]		= allocate(0);
		Pdata[player][AUTOHU]		= allocate(0);
		Pdata[player][SECRETGON]	= allocate(0);
		Pdata[player][LISTEN]		= allocate(0);
		Pdata[player][PDUMPED]		= allocate(0);
		Pdata[player][PLISTEN]		= 0;
		Pdata[player][LISTENTYPE]	= 0;
		Pdata[player][AUTOOFF]		= 0;
		Pdata[player][FLOWERNUM]	= 0;
		Pdata[player][LOOKALL]		= 0;
	}
}

// 全新的牌局
void new_MJ()
{
	broadcast(environment(), HIW"\n\n█"NOR WHT"全新牌局開始\n"NOR);
	beep();

	Psort -= ({ 0 });	// 清除空物件 (離線玩家)
	clear_last_data();	// 清除遊戲資料
	MJdata[PLAYING] = 1;	// 遊戲標記
	MJdata[ROUND]	= 1;	// 起始圈數
	MJdata[ROUNDWIND] = 0;	// 起始風
	seat_select();		// 選擇座位
	master_select();	// 選擇莊家
	begining_select();	// 選擇開門
	deal_tile();		// 發牌
}

// 下一圈
void next_MJ()
{
	if( MJdata[CFINISH] )
		return tell(this_player(), "本次牌局已經結束了，重新開始牌局請按 restart。\n");

	if( !sizeof(Psort) )
		return;

	broadcast(environment(), HIW"\n\n█"NOR WHT"第"+CHINESE_D->chinese_number(++MJdata[ROUND])+"圈牌局開始\n"NOR);

	// 換莊
	if( MJdata[LOSER] == Psort[MASTER] || (MJdata[WINNER] && MJdata[WINNER] != Psort[MASTER]) )
	{
		// 重頭輪莊時檢查是否已打完四圈麻將
		if( !(++MJdata[MROUND] % sizeof(Psort)) && !(++MJdata[ROUNDWIND] % sizeof(Psort)) )
		{
			broadcast(environment(), HIG"\n已完成四圈牌局，牌局重新開始。\n"NOR);
			new_MJ();
			return;
		}

		// 換下家莊
		Psort = Psort[1..] + Psort[0..0];

		broadcast(environment(), HIG"█"NOR GRN"換莊："NOR+Pdata[Psort[MASTER]][IDNAME]+"\n");

		MJdata[CMASTER] = 0;
	}
	// 連莊
	else
	{
		MJdata[CMASTER]++;
		broadcast(environment(), HIG"█"NOR GRN"連莊連"+CHINESE_D->chinese_number(MJdata[CMASTER])+"："NOR+Pdata[Psort[MASTER]][IDNAME]+"\n");
	}

	// 清除上局資訊
	clear_last_data();

	// 開門選取
	begining_select();

	// 發牌
	deal_tile();
}

// 完全清除牌局資料
void reset_all_data()
{
	Psort			= allocate(0);
	Pdata			= allocate_mapping(0);
	MJdata			= allocate_mapping(0);
	MJdata[ONLOOKER]	= allocate(0);

	delete_temp("status", this_object());
}

// 初始玩家資料
void initialize_player(object player)
{
	Pdata[player]		= allocate_mapping(0);
	Pdata[player][PLAYER]	= player;
	Pdata[player][ID]	= player->query_id(1);
	Pdata[player][IDNAME]	= player->query_idname();
        Pdata[player][MONEY]    = 200000;

	// 初始紀錄系統資訊
	if( !mapp(score[Pdata[player][ID]]) )
		score[Pdata[player][ID]] = allocate_mapping(0);
}

// 開始牌局
void do_start(object me, string arg)
{
	int playernum = sizeof(Psort);
	int playerneed = 4-playernum;
	object AIob;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( MJdata[PLAYING] )
		return tell(me, "牌局已經開始了。\n");

	if( playernum < 4 )
	{
		while(playerneed--)
		{
			AIob = new(MJAI_FILE);
			AIob->move(environment());
			AIob->do_command("join");
		}
	}

	set_temp("status", HIY"牌"NOR YEL"局進行中"NOR, this_object());

	// 開始新牌局
	new_MJ();
}

// 顯示進遊戲畫面
void show_title()
{
	int i;
	object ppl, ppl2;
	foreach( ppl in Psort )
	{
		clear_screen(ppl);
		spec_line_msg(ppl,     HIY"					   ';;;;;;;;;;;;;;;.", 1);
		spec_line_msg(ppl,     HIY"					     ';;;;;;;;;;;;;;;;.", 2);
		spec_line_msg(ppl,     HIY"    臺 "NOR YEL"灣 正 宗 十 六 張 麻 將 ─ "HIY"v"+VERSION+"     ';;;;;;;;;;;;;;;.", 3);
		spec_line_msg(ppl,     HIY"						';;;;;;;;;;;;;;;.", 4);
		spec_line_msg(ppl, NOR CYN"   ┌────────────────────────┬─────┐", 5);
		spec_line_msg(ppl, NOR CYN"   ∣"HIC"請等待人數額滿自動開始牌局，或按 start 直接開始 "NOR CYN"∣"HIC"目前人數 "+NOR CYN+sizeof(Psort)+"∣", 6);
		spec_line_msg(ppl, NOR CYN"   └────────────────────────┴─────┘"NOR, 7);
		spec_line_msg(ppl,     HIY"						.;;;;;;;;;;;;;;;;.", 8);
		spec_line_msg(ppl,     HIY"					       .;;;;;;;;;;;;;;;'.", 9);
		spec_line_msg(ppl,     HIY"					      .;;;;;;;;;;;;;;'.", 10);
		spec_line_msg(ppl,     HIY"					    .;;;;;;;;;;;;;'.", 11);
		spec_line_msg(ppl,     HIY"					 .;;;;;;;;;;;;;'.", 12);
		spec_line_msg(ppl,     HIY"				      .;;;;;;;;;;;;;'", 13);
		spec_line_msg(ppl,     HIY"				  .;;;;;;;;;;;;'", 14);
		spec_line_msg(ppl,     HIY"			       ''''''"NOR, 15);
		i=8;
		foreach( ppl2 in Psort )
		spec_line_msg(ppl, sprintf(HIW"     ．%-20s "HIG"籌碼 $200,000"NOR, ppl2->query_idname()), i++);
	}
}

// 顯示排行榜
void show_billboard(object me)
{
	int i;

	clear_screen(me);

	spec_line_msg(me,     HIC"					    ';;;;;;;;;;;;;;;.", 1);
	spec_line_msg(me,     HIC"					     ;;;;;;;;;;;;;;;;.", 2);
	spec_line_msg(me,     HIY"    臺 "NOR YEL"灣 正 宗 十 六 張 麻 將 ─ "HIY"v"+VERSION+HIC"     ';;;;;;;;;;;;;;;.", 3);
	spec_line_msg(me,     HIC"						';;;;;;;;;;;;;;;.", 4);
	spec_line_msg(me, NOR CYN"   ┌ "HIC"玩家籌碼"NOR CYN" ─────────────────────────┐", 5);
	spec_line_msg(me, NOR CYN"   ∣ 							   "NOR CYN"∣"NOR, 6);
	spec_line_msg(me, NOR CYN"   ∣ 							   "NOR CYN"∣"NOR, 7);
	spec_line_msg(me, NOR CYN"   ∣ 							   "NOR CYN"∣"NOR, 8);
	spec_line_msg(me, NOR CYN"   ∣ 							   "NOR CYN"∣"NOR, 9);
	spec_line_msg(me, NOR CYN"   └──────────────────────────────┘", 10);
	spec_line_msg(me,     HIC"					   .;;;;;;;;;;;;;'.", 11);
	spec_line_msg(me,     HIC"					.;;;;;;;;;;;;;'.", 12);
	spec_line_msg(me,     HIC"				     .;;;;;;;;;;;;;'", 13);
	spec_line_msg(me,     HIC"				 .;;;;;;;;;;;;'", 14);
	spec_line_msg(me,     HIC"			      ''''''"NOR, 15);
	i=6;
	foreach( object ppl in Psort )
	spec_line_msg(me, sprintf(NOR CYN"   ∣"HIW"．%-20s		    "HIG"籌碼 $%s"NOR, Pdata[ppl][IDNAME], NUMBER_D->number_symbol(Pdata[ppl][MONEY])) , i++);
}

void do_bill(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	Pdata[me][LOOKSEA] = 1;

	show_billboard(me);
}

void do_top(object me, string arg)
{
	array kings = allocate(4);
	string id, str, *idsort;

	switch( arg )
	{
	case "mo"    : idsort = sort_array(keys(score), (: score[$1][HUMYSELVES] < score[$2][HUMYSELVES] ? 1 : -1 :)); break;
	case "hu"    : idsort = sort_array(keys(score), (: score[$1][WINTIMES] < score[$2][WINTIMES] ? 1 : -1 :)); break;
	case "gun"   : idsort = sort_array(keys(score), (: score[$1][GUNTIMES] < score[$2][GUNTIMES] ? 1 : -1 :)); break;
	case "win"   : idsort = sort_array(keys(score), (: (score[$1][HUMYSELVES]+score[$1][WINTIMES])*1./(score[$1][ROUNDS]||1) < (score[$2][HUMYSELVES]+score[$2][WINTIMES])*1./(score[$2][ROUNDS]||1) ? 1 : -1 :)); break;
	case "lose"  : idsort = sort_array(keys(score), (: score[$1][GUNTIMES]*1./(score[$1][ROUNDS]||1) < score[$2][GUNTIMES]*1./(score[$2][ROUNDS]||1) ? 1 : -1 :)); break;
	case "bonus" : idsort = sort_array(keys(score), (: score[$1][BONUSES] < score[$2][BONUSES] ? 1 : -1 :)); break;
	default:
		idsort = sort_array(keys(score), (: score[$1][SCORE] < score[$2][SCORE] ? 1 : -1 :)); break;
	}

	str =	   HIW"玩家	     分數  局數       自摸	 放槍 胡牌	 贏局	    輸局 贏台 輸台\n"NOR;
	str += NOR WHT"──────────────────────────────────────────\n"NOR;

	foreach( id in idsort )
	{
		//if( score[id][ROUNDS] < 10 )
		//{
		//	idsort -= ({ id });
		//	continue;
		//}

		str += sprintf(HIW"%-10s"HIG"%8d"HIW"%6d"HIY"%4d(%4.1f%%)"HIR"%4d(%4.1f%%)"HIC"%5d"HIW"%4d(%4.1f%%)"NOR WHT"%4d(%4.1f%%)"HIM"%5d"HIM"%5d\n"NOR
		    , capitalize(id)
		    , score[id][SCORE]
		    , score[id][ROUNDS]
		    , score[id][HUMYSELVES]
		    , score[id][HUMYSELVES]*100./(score[id][ROUNDS]||1)
		    , score[id][GUNTIMES]
		    , score[id][GUNTIMES]*100./(score[id][ROUNDS]||1)
		    , score[id][WINTIMES]
		    , score[id][HUMYSELVES]+score[id][WINTIMES]
		    , (score[id][HUMYSELVES]+score[id][WINTIMES])*100./(score[id][ROUNDS]||1)
		    , score[id][LOSETIMES]
		    , score[id][LOSETIMES]*100./(score[id][ROUNDS]||1)
		    , score[id][BONUSES]
		    , score[id][LOSEBONUSES]);

		if( score[id][ROUNDS] < 100 )
			idsort -= ({ id });

	};
	str += NOR WHT"──────────────────────────────────────────\n"NOR;

	foreach( id in idsort )
	{
		kings[0] = sort_array(unique_array(idsort, (: (score[$1][HUMYSELVES]+score[$1][WINTIMES])*1./(score[$1][ROUNDS]||1) :) ), (: (score[$1[0]][HUMYSELVES]+score[$1[0]][WINTIMES])*1./(score[$1[0]][ROUNDS]||1) < (score[$2[0]][HUMYSELVES]+score[$2[0]][WINTIMES])*1./(score[$2[0]][ROUNDS]||1) ? 1:-1 :) );
		kings[1] = sort_array(unique_array(idsort, (: score[$1][LOSETIMES]*1./(score[$1][ROUNDS]||1) :) ), (: score[$1[0]][LOSETIMES]*1./(score[$1[0]][ROUNDS]||1) < score[$2[0]][LOSETIMES]*1./(score[$2[0]][ROUNDS]||1) ? 1:-1 :) );
		kings[2] = sort_array(unique_array(idsort, (: score[$1][HUMYSELVES]*1./(score[$1][ROUNDS]||1) :) ), (: score[$1[0]][HUMYSELVES]*1./(score[$1[0]][ROUNDS]||1) < score[$2[0]][HUMYSELVES]*1./(score[$2[0]][ROUNDS]||1) ? 1:-1 :) );
		kings[3] = sort_array(unique_array(idsort, (: score[$1][GUNTIMES]*1./(score[$1][ROUNDS]||1) :) ), (: score[$1[0]][GUNTIMES]*1./(score[$1[0]][ROUNDS]||1) < score[$2[0]][GUNTIMES]*1./(score[$2[0]][ROUNDS]||1) ? 1:-1 :) );
	}
	str += HIW"四 大 "NOR WHT"天 王"NOR"\n";
	if( sizeof(kings[0]) )
	str += sprintf(NOR CYN"勝率最高   "HIC"麻將俠："HIW"%-10s"NOR WHT"%5.2f%%\n"NOR,capitalize(kings[0][0][0]),(score[kings[0][0][0]][HUMYSELVES]+score[kings[0][0][0]][WINTIMES])*100./(score[kings[0][0][0]][ROUNDS]));
	if( sizeof(kings[1]) )
	str += sprintf(NOR YEL"敗率最高   "HIY"槓龜聖："HIW"%-10s"NOR WHT"%5.2f%%\n"NOR,capitalize(kings[1][0][0]),score[kings[1][0][0]][LOSETIMES]*100./(score[kings[1][0][0]][ROUNDS]));
	if( sizeof(kings[2]) )
	str += sprintf(NOR GRN"自摸率最高 "HIG"自摸神："HIW"%-10s"NOR WHT"%5.2f%%\n"NOR,capitalize(kings[2][0][0]),score[kings[2][0][0]][HUMYSELVES]*100./(score[kings[2][0][0]][ROUNDS]));
	if( sizeof(kings[3]) )
	str += sprintf(NOR RED"放炮率最高 "HIR"放槍王："HIW"%-10s"NOR WHT"%5.2f%%\n"NOR,capitalize(kings[3][0][0]),score[kings[3][0][0]][GUNTIMES]*100./(score[kings[3][0][0]][ROUNDS]));
	str += NOR WHT"局數 100 局以上								      v"VERSION"\n"NOR;

	str += NOR WHT"──────────────────────────────────────────\n"NOR;
	me->more("\n"+kill_repeat_ansi(str)+"\n");
}

// 加入牌局
void do_join(object me, string arg)
{
	if( is_player(me) )
		return tell(me, pnoun(2,me)+"已經加入牌局了。\n");

	if( sizeof(Psort) == 4 )
	{
		foreach(object ob in Psort)
		{
			if( Pdata[ob][AI] )
			{
				do_substitute(ob, me);
				destruct(ob);
				return;
			}
		}

		return tell(me, "抱歉牌局已經客滿了！\n");
	}

	if( MJdata[PLAYING] && sizeof(Psort) != 4 )
		return tell(me, "目前無法加入牌局。\n");

	// 清除離線玩家
	Psort -= ({ 0 });

	Psort += ({ me });
	initialize_player(me);

	if( !userp(me) )
	{
		Pdata[me][AI] = 1;
		set_temp("mjobject", this_object(), me);
	}

	// 啟動 title screen
	startup_title_screen(me, TITLE_SCREEN_LENGTH);

	msg("$ME在麻將桌旁坐了下來。\n", me, 0, 1);
	MJdata[ONLOOKER] -= ({ me });
	me->add_status(STATUS);

	beep();

	if( sizeof(Psort) < 4 )
		msg("$ME四處招手："+HIG+"還欠"+CHINESE_D->chinese_number(4-sizeof(Psort))+"腳喔∼快來唷∼\n"NOR, me, 0, 1);

	// 四人湊齊即開始牌局
	if( sizeof(Psort) == 4 )
	{
		msg("人數已滿，牌局開始。\n");
		do_start(me, arg);
		return;
	}

	// 顯示 Title 畫面
	if( !MJdata[PLAYING] )
		show_title();

	set_temp("status", HIY"欠"+NOR YEL+CHINESE_D->chinese_number(4-sizeof(Psort))+HIY"腳"NOR, this_object());
}

// 離開牌局
void do_leave(object me, string arg)
{
	int playernum;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	foreach( object ppl in Psort )
		if( objectp(ppl) && !Pdata[ppl][AI] ) playernum++;

	if( playernum == 1 )
		return do_reset(me, arg);
	else
	{
		object AIob = new(MJAI_FILE);
		AIob->move(environment());
		do_substitute(me, AIob);
		
		msg("$ME離開麻將桌不打了。\n", me, 0, 1);
		do_mo(AIob, arg);
		return;
	}
}

// 重新開始牌局 (玩家不變)
void do_restart(object me, string arg)
{
	object player, *players = copy(Psort-({0}));

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	reset_all_data();

	Psort = players;

	foreach( player in Psort )
	initialize_player(player);

	msg(HIY"\n$ME將牌全部重洗了一遍，牌局重新開始。\n"NOR,me, 0, 1);

	do_start(me, arg);
}

// 重新設置麻將桌
void do_reset(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	foreach( object ppl in Psort )
	{
		if( !objectp(ppl) ) continue;

		ppl->remove_status(STATUS);
		ppl->remove_status(OSTATUS);
		reset_screen(ppl);

		if( !userp(ppl) )
			destruct(ppl);
	}

	foreach( object onlooker in MJdata[ONLOOKER] )
	{
		onlooker->remove_status(OSTATUS);
		reset_screen(onlooker);
	}

	reset_all_data();

	msg("$ME將麻將桌全部清空了。\n",me,0,1);
}

//重發
void do_redeal(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	if( arrayp(MJdata[NINEYOUNGS]) && member_array(me, MJdata[NINEYOUNGS]) != -1 )
	{
		msg("\n$ME慘叫一聲："HIG"九么九和啦，重洗！！\n"NOR, me, 0, 1);

		broadcast(environment(), HIG"牌局重新發牌。\n"NOR);

		next_MJ();
		return;
	}

	tell(me, "沒有特殊情形不能要求重新洗牌。\n");
}

//重畫畫面
void do_redraw(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	reset_screen(me);
	startup_title_screen(me, TITLE_SCREEN_LENGTH);

	show_tile(me);

	tell(me, "重新啟動畫面完畢。\n");
}

//進行下一局
void do_next(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	if( !MJdata[FINISH] )
		return tell(me, "本圈牌局尚未結束，不能進行下一圈牌局。\n");

	if( MJdata[FINISH] > time() )
		return tell(me, "請於 "+(MJdata[FINISH]-time())+" 秒後再進行下一局。\n");

	if( sizeof(Psort) < 2 )
		return tell(me, "至少要有兩位玩家才可以開始牌局。\n");

	next_MJ();
}

// 隨機安排座位
void seat_select()
{
	int i, j;
	object *temp = copy(Psort);

	broadcast(environment(), HIY"█"NOR YEL"取出「"+implode(Dsort[0..sizeof(temp)-1], "、")+"」隨機抽牌決定座位\n"NOR);

	Psort = allocate(0);

	while(sizeof(temp))
	{
		i = random(sizeof(temp));
		Psort += ({ temp[i] });
		broadcast(environment(), HIY"█ "NOR YEL+Dsort[j++]+"家："NOR+Pdata[temp[i]][IDNAME]+"\n");
		temp = temp[0..i-1] + temp[i+1..];
	}
}

// 莊家選擇
void master_select()
{
	int i = random(16)+3;

	msg(HIG"█"NOR GRN"東家$ME"GRN"擲骰子決定起莊，擲出骰子為 "+i+" 點\n"NOR, Psort[0], 0, 1);

	i = ((i-1) % sizeof(Pdata));

	// 直接重新依照原順序排莊家位為 index 0
	Psort = Psort[i..]+Psort[0..i-1];

	broadcast(environment(), HIG"█ "NOR GRN"莊家："NOR+Pdata[Psort[MASTER]][IDNAME]+"\n");

	MJdata[TURN] = 0;
}

// 捉牌位置選擇
void begining_select()
{
	int i = random(16)+3;
	object *temp;

	msg(HIC"█"NOR CYN"莊家$ME"CYN"擲骰子決定捉牌位置，擲出骰子為 "+i+" 點\n"NOR, Psort[MASTER], 0, 1);

	// 紀錄開門風位
	MJdata[OPENDOOR] = (i-1)%sizeof(Psort);

	broadcast(environment(), HIC"█"NOR CYN+Dsort[MJdata[OPENDOOR]]+"風開門，第 "+(++i)+" 幢開始捉牌。\n"NOR);

	// 牌型順序調整
	Ctile = Ctile[i*2..]+Ctile[0..(i*2-1)];

	temp = Psort[MJdata[OPENDOOR]..]+Psort[0..MJdata[OPENDOOR]-1];

	for(i=0;i<sizeof(Psort);i++)
		Pdata[temp[i]][FLOWERNUM] = i;
}

// 門牌牌型排列
void otile_display(string ref *arr, mapping data, int secretgon)
{
	string tile;

	arr = allocate(2, "");

	if( sizeof(data[FLOWER]) )
	{
		arr[0] += NOR WHT"∣";
		arr[1] += NOR WHT"∣";
		foreach( tile in data[FLOWER] )
		{
			arr[0] += NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+NOR WHT"∣";
			arr[1] += NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+NOR WHT"∣";
		}
	}
	if( sizeof(data[OTILE]) )
	{
		arr[0] += NOR WHT"∣";
		arr[1] += NOR WHT"∣";
		foreach( tile in data[OTILE] )
		{
			if( secretgon && member_array( tile, data[SECRETGON] ) != -1 )
			{
				arr[0] += HIW+"暗"+NOR WHT"∣";
				arr[1] += NOR WHT"槓"+NOR WHT"∣";
			}
			else
			{
				arr[0] += NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+NOR WHT"∣";
				arr[1] += NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+NOR WHT"∣";
			}
		}
	}
}


void show_dumping(object me)
{
	string tile;

	spec_line_msg(me, sprintf("	   "NOR WHT"未翻 "HIW"%d"NOR WHT" 張，海底 "HIW"%d"NOR WHT" 張，"HIW"%s"NOR WHT"風圈，"HIW"%s"NOR WHT"家開門"NOR, strlen(Ctile)/2, sizeof(MJdata[DUMPED])+stringp(MJdata[DUMPING]), Dsort[MJdata[ROUNDWIND]], Dsort[MJdata[OPENDOOR]]), 1);

	// 目前打出的牌的資訊
	if( tile = MJdata[DUMPING] )
	{
		spec_line_msg(me, HIW"┌─┐"NOR, 1);
		spec_line_msg(me, HIW"∣"+NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+NOR HIW"∣", 2);
		spec_line_msg(me, HIW"∣"+NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+NOR HIW"∣", 3);
		spec_line_msg(me, HIW"└"+NOR+MJtile[tile][COLOR1]+tile+HIW"┘"NOR, 4);
	}
}

// 顯示主畫面
void show_tile(object me)
{
	string tile, str, *arr;
	mapping mydata, lastdata, nextdata, oppodata;

	// 每次重看就清除一次畫面
	clear_screen(me);

	if( Pdata[me][LOOKALL] )
	{
		show_all(me);
		return;
	}
	if( Pdata[me][LOOKSEA] )
	{
		show_sea(me);
		return;
	}

	// 自家
	mydata = Pdata[me];

	if( sizeof(mydata[FLOWER]+mydata[OTILE]) )
	{
		otile_display(ref arr, mydata, 0);

		str = "";
		if( sizeof(mydata[FLOWER]) ) str = HIW"  ┌"+repeat_string("─┬",sizeof(mydata[FLOWER])-1)+"─┐";
		if( sizeof(mydata[OTILE]) ) str += (sizeof(mydata[FLOWER])?"":HIW"  ")+"┌"+repeat_string("─┬",sizeof(mydata[OTILE])-1)+"─┐"NOR;
		spec_line_msg(me, str, 9);
		spec_line_msg(me, NOR WHT"  "+kill_repeat_ansi(replace_string(arr[0], WHT, NOR)), 10);
		spec_line_msg(me, NOR WHT"  "+kill_repeat_ansi(arr[1])+NOR, 11);
		str = replace_string(str, "┬", "┴");
		str = replace_string(str, "┌", "└");
		str = replace_string(str, "┐", "┘");
		spec_line_msg(me, replace_string(str, HIW, NOR WHT), 12);
	}

	arr = allocate(2, "∣");

	foreach( tile in mydata[TILE] )
	{
		arr[0] += NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+HIW"∣";
		arr[1] += NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+HIW"∣";
	}

	spec_line_msg(me, HIW"┌"+repeat_string("─┬",sizeof(mydata[TILE])-1)+"─┐"NOR,12);
	spec_line_msg(me, HIW+arr[0]+" "+NOR+(me==Psort[0]?HIG"莊"+(MJdata[CMASTER]?"連"+CHINESE_D->chinese_number(MJdata[CMASTER]):"")+" "NOR:"")+(sizeof(mydata[LISTEN])?HIC"聽("+implode(mydata[LISTEN],",")+") "NOR:"")+NOR, 13);
	spec_line_msg(me, HIW+arr[1]+" "+NOR+HIM+Dsort[member_array(me, Psort)]+"家 "+(mydata[FLOWERNUM]+1)+" 花"+NOR, 14);
	spec_line_msg(me, HIW"└"+implode(mydata[TILE], "┴")+"┘"+NOR+(member_array(me, Psort)==MJdata[TURN]?HBCYN" "NOR HIREV HIC+remove_ansi(mydata[IDNAME])+NOR HBCYN" ":" "+mydata[IDNAME]+" ")+NOR" ", 15);

	// 對手資訊
	switch(sizeof(Psort))
	{
	case 2:
		oppodata = Pdata[(Psort - ({ me }))[0]];
		break;
	case 3:
		switch( member_array(me, Psort) )
		{
		case 0:
			nextdata = Pdata[Psort[1]];
			lastdata = Pdata[Psort[2]];
			break;
		case 1:
			nextdata = Pdata[Psort[2]];
			lastdata = Pdata[Psort[0]];
			break;
		case 2:
			nextdata = Pdata[Psort[0]];
			lastdata = Pdata[Psort[1]];
			break;
		}
		break;
	case 4:
		switch( member_array(me, Psort) )
		{
		case 0:
			nextdata = Pdata[Psort[1]];
			oppodata = Pdata[Psort[2]];
			lastdata = Pdata[Psort[3]];
			break;
		case 1:
			nextdata = Pdata[Psort[2]];
			oppodata = Pdata[Psort[3]];
			lastdata = Pdata[Psort[0]];
			break;
		case 2:
			nextdata = Pdata[Psort[3]];
			oppodata = Pdata[Psort[0]];
			lastdata = Pdata[Psort[1]];
			break;
		case 3:
			nextdata = Pdata[Psort[0]];
			oppodata = Pdata[Psort[1]];
			lastdata = Pdata[Psort[2]];
			break;
		}
		break;
	}

	// 下家
	arr = allocate(2, "");
	if( mapp(nextdata) )
	{
		otile_display(ref arr, nextdata, 1);

		str = " "+(nextdata[PLAYER]==Psort[MASTER]?HIG"莊"+(MJdata[CMASTER]?"連"+CHINESE_D->chinese_number(MJdata[CMASTER]):"")+" "NOR:"") + (sizeof(nextdata[LISTEN])?HIC"聽 "NOR:"");

		arr[0] = kill_repeat_ansi(arr[0]);
		arr[1] = kill_repeat_ansi(arr[1]);

		spec_line_msg(me, sprintf("%66s"NOR"%-s", repeat_string(" ",strlen(sizeof(nextdata[TILE])+""))+arr[0], str), 7);
		spec_line_msg(me, sprintf("%66s"NOR"%-s", HIW+sizeof(nextdata[TILE])+NOR+arr[1], (member_array(nextdata[PLAYER], Psort)==MJdata[TURN] ?HBCYN" "NOR HIREV HIC+remove_ansi(nextdata[IDNAME])+NOR HBCYN" ":" "+nextdata[IDNAME]+" ")+NOR" "), 8);
	}

	// 上家
	if( mapp(lastdata) )
	{
		otile_display(ref arr, lastdata, 1);

		str = " "+(lastdata[PLAYER]==Psort[MASTER]?HIG"莊"+(MJdata[CMASTER]?"連"+CHINESE_D->chinese_number(MJdata[CMASTER]):"")+" "NOR:"") + (sizeof(lastdata[LISTEN])?HIC"聽 "NOR:"");

		arr[0] = kill_repeat_ansi(arr[0]);
		arr[1] = kill_repeat_ansi(arr[1]);

		spec_line_msg(me, str+repeat_string(" ",noansi_strlen(lastdata[IDNAME])-noansi_strlen(str)+2)+arr[0]+NOR, 5);
		spec_line_msg(me, (member_array(lastdata[PLAYER], Psort)==MJdata[TURN]?HBCYN" "NOR HIREV HIC+remove_ansi(lastdata[IDNAME])+NOR HBCYN" ":" "+lastdata[IDNAME]+" ")+repeat_string(" ",noansi_strlen(str)-noansi_strlen(lastdata[IDNAME])-1)+NOR+arr[1]+HIW+sizeof(lastdata[TILE])+NOR, 6);
	}

	// 對家
	if( mapp(oppodata) )
	{
		otile_display(ref arr, oppodata, 1);

		str = " "+(oppodata[PLAYER]==Psort[MASTER]?HIG"莊"+(MJdata[CMASTER]?"連"+CHINESE_D->chinese_number(MJdata[CMASTER]):"")+" "NOR:"") + (sizeof(oppodata[LISTEN])?HIC"聽 "NOR:"");

		arr[0] = kill_repeat_ansi(arr[0]);
		arr[1] = kill_repeat_ansi(arr[1]);

		spec_line_msg(me, "	   "+str+repeat_string(" ",noansi_strlen(oppodata[IDNAME])-noansi_strlen(str)+2)+arr[0]+NOR, 2);
		spec_line_msg(me, "	   "+(member_array(oppodata[PLAYER], Psort)==MJdata[TURN]?HBCYN" "NOR HIREV HIC+remove_ansi(oppodata[IDNAME])+NOR HBCYN" ":" "+oppodata[IDNAME]+" ")+repeat_string(" ",noansi_strlen(str)-noansi_strlen(oppodata[IDNAME])-1)+NOR+arr[1]+HIW+sizeof(oppodata[TILE])+NOR, 3);
	}

	show_dumping(me);
}
// 看海底牌
void show_sea(object me)
{
	int i, j;
	string str1, str2;

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	clear_screen(me);

	spec_line_msg(me, HIW" □目前在桌面上所有的牌："NOR, 2);

	str1 = str2 = HIW"∣"NOR;

	if( arrayp(MJdata[DUMPED]) )
	{
		foreach( string tile in MJdata[DUMPED]+ (MJdata[DUMPING]?({MJdata[DUMPING]}):({})) )
		{
			if( !(++j%20) )
			{
				str1 += NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+HIW"∣"NOR;
				str2 += NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+HIW"∣"NOR;
				spec_line_msg(me, HIW"┌"+repeat_string("─┬", 19)+"─┐"NOR, i+3);
				spec_line_msg(me, kill_repeat_ansi(str1), i+4);
				spec_line_msg(me, kill_repeat_ansi(str2), i+5);
				spec_line_msg(me, HIW"└"+repeat_string("─┴", 19)+"─┘"NOR, i+6);
				i+=4;
				str1 = str2 = HIW"∣"NOR;
			}
			else
			{
				str1 += NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+HIW"∣"NOR;
				str2 += NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+HIW"∣"NOR;
			}
		}

		if( j%20 )
		{
			spec_line_msg(me, HIW"┌"+repeat_string("─┬", j%20-1)+"─┐"NOR, i+3);
			spec_line_msg(me, kill_repeat_ansi(str1), i+4);
			spec_line_msg(me, kill_repeat_ansi(str2), i+5);
			spec_line_msg(me, HIW"└"+repeat_string("─┴", j%20-1)+"─┘"NOR, i+6);
		}
	}
}

// 看自己牌
void do_look(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	if( Pdata[me][LOOKSEA] || Pdata[me][LOOKALL] )
	{
		Pdata[me][LOOKSEA] = 0;
		Pdata[me][LOOKALL] = 0;
		show_tile(me);
	}
	else
	{
		Pdata[me][LOOKSEA] = 1;
		show_sea(me);
	}
}

void show_all_tile()
{
	foreach( object ppl in Psort )
	show_tile(ppl);

	foreach( object ppl in MJdata[ONLOOKER] )
	{
		if( !objectp(ppl) )
		{
			MJdata[ONLOOKER] -= ({ ppl });
			continue;
		}
		show_all(ppl);
	}
}

// 進行補花動作
void check_flower(mapping ref data, string tile)
{
	while( sscanf(tile, "f%*d") == 1 )
	{
		msg("$ME丟出一張"+full_chinese(tile)+"同時說道："HIG"補花。\n"NOR, data[PLAYER], 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

		data[FLOWER] = tile_sort(data[FLOWER]+({ tile }));
		data[TILE] -= ({ tile });

		// 補花由牌尾補
		tile = Ctile[<2..<1];
		Ctile = Ctile[0..<3];

		msg("$ME由牌尾摸了一張新的牌。\n", data[PLAYER], 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
		tell(data[PLAYER], pnoun(2, data[PLAYER])+"摸到了一張"+full_chinese(tile)+"。\n", 0);

		data[TILE] += ({ tile });

		auto_function_check(data[PLAYER], tile);

		show_all_tile();

		// 補花胡牌
		if( valid_hu_check(tile_sort(data[TILE]), data[OTILE], data[FLOWER]) )
		{
			beep();
			msg("$ME很機車的大叫一聲："HIY"『自摸』啦！哇哈哈哈哈∼\n"NOR, data[PLAYER], 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

			// 槓上開花
			data[TAILTILE] = 1;
			MJdata[HUMYSELF] = 1;
			MJdata[KEYTILE] = tile;

			// 檢查是否天胡
			if( !sizeof(MJdata[DUMPED]) )
				MJdata[HUFIRST] = 1;

			win_process(data[PLAYER], 0);

			if( sizeof(Psort) < 4 )
				set_temp("status", HIY"欠"+NOR YEL+CHINESE_D->chinese_number(4-sizeof(Psort))+HIY"腳"NOR, this_object());
			MJdata[FINISH] = time()+20;
			return;
		}
	}
}

// 開始發牌
void deal_tile()
{
	int i, j;
	string tile, *youngs;
	object ppl;

	set_temp("status", HIY"牌"NOR YEL"局進行中"NOR, this_object());

	// 洗牌
	wash_mj();

	// 重設玩家資料
	foreach( ppl in Psort )
	Pdata[ppl][TILE] = allocate(0);

	// 每四幢輪流拿牌
	for(i=0;i<4;i++)
		foreach( ppl in Psort )
	{
		// 由牌前開始捉牌, 每四張一輪
		for(j=0;j<4;j++)
			Pdata[ppl][TILE] += ({ Ctile[j*2..j*2+1] });

		Ctile = Ctile[8..];
	}

	// 最後莊家多拿一張
	Pdata[Psort[MASTER]][TILE] += ({ Ctile[0..1] });
	Ctile = Ctile[2..];

	show_all_tile();

	broadcast(environment(), HIM"█"NOR MAG"捉牌完畢，由莊家開始補花：\n"NOR);

	foreach( ppl in Psort )
	{
		foreach( tile in Pdata[ppl][TILE] )
		check_flower(ref Pdata[ppl], tile);

		msg("$ME說道："HIG"過補啦！\n"NOR, ppl, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
		Pdata[ppl][TILE] = tile_sort(Pdata[ppl][TILE]);
	}

	//九么九和
	foreach( ppl in Psort )
	{
		i = 0;
		youngs = ({"1w","9w","1s","9s","1t","9t","ea","we","so","no","jo","fa","ba"});

		foreach( tile in Pdata[ppl][TILE] )
		{
			if( member_array(tile, youngs) != -1 ) i++;
			youngs -= ({ tile });
		}
		if( i >= 9 )
		{
			tell(ppl, "\n"HIG+pnoun(2, ppl)+"的牌為九么九和，可要求重發(redeal)。\n"NOR, 0);
			if( !arrayp(MJdata[NINEYOUNGS]) )
				MJdata[NINEYOUNGS] = ({ ppl });
			else
				MJdata[NINEYOUNGS] += ({ ppl });
		}

		// 初始自動碰槓設定
		if( Pdata[ppl][AI] )
			ppl->AI_pg();
	}

	show_all_tile();

	// 莊家天胡 ?
	if( valid_hu_check(Pdata[Psort[MASTER]][TILE], Pdata[Psort[MASTER]][OTILE], Pdata[Psort[MASTER]][FLOWER]) )
	{
		beep();
		msg("$ME很驚訝的大叫一聲："HIY"啊∼∼∼『天胡』啦！哇哈哈哈哈哈哈哈哈∼\n"NOR, ppl, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
		MJdata[HUFIRST] = 1;
		win_process(ppl, 0);

		if( sizeof(Psort) < 4 )
			set_temp("status", HIY"欠"+NOR YEL+CHINESE_D->chinese_number(4-sizeof(Psort))+HIY"腳"NOR, this_object());
		MJdata[FINISH] = time()+20;
		return;
	}

	if( Pdata[Psort[MASTER]][AI] )
	{
		Psort[MASTER]->emotion_begin();
		call_out((:do_da, Psort[MASTER], call_other(Psort[MASTER], "AI_da"):), 5);
	}
}

// 亂數洗牌
void wash_mj()
{
	int i, num;
	string str = "";
	mapping temp = copy(MJtile);

	foreach( string eng, array data in temp )
	while(data[AMOUNT]--) str += eng;

	// 狂洗十輪牌, 應該夠亂吧
	for(i=0;i<10;i++)
	{
		while(sizeof(str))
		{
			num = random(sizeof(str))/2*2;
			Ctile = random(2) ? Ctile + str[num..num+1] : str[num..num+1] + Ctile;
			str = str[0..num-1] + str[num+2..];
		}
		str = Ctile;
		Ctile = "";
	}

	Ctile = str;
}

// 打牌
void do_da(object me, string arg)
{
	int i;
	mapping data;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	if( MJdata[FINISH] )
		return tell(me, "已結束一圈牌局，若要繼續進行下一圈牌局請按 next。\n");

	if( !arg ) return tell(me, pnoun(2, me)+"想要打哪張牌？\n");

	arg = lower_case(arg);

	if( Psort[MJdata[TURN]] != me )
		return tell(me, "還沒有輪到"+pnoun(2, me)+"！\n");

	if( MJdata[MO] )
		return tell(me, pnoun(2, me)+"還沒摸牌！\n");

	data = Pdata[me];

	if( sizeof(data[LISTEN]) && !data[AUTOOFF] && previous_object() )
		return tell(me, pnoun(2, me)+"已經聽牌了，不能自己控制打牌動作。\n");

	i = member_array(arg, data[TILE]);

	if( i == -1 ) return tell(me, pnoun(2, me)+"沒有 "+arg+" 這張牌。\n");

	if( stringp(MJdata[DUMPING]) )
		MJdata[DUMPED] += ({ copy(MJdata[DUMPING]) });

	MJdata[DUMPING] = arg;

	data[TILE] = data[TILE][0..i-1] + data[TILE][i+1..];

	msg("$ME打出了一張"+full_chinese(arg)+"。\n", data[PLAYER], 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

	data[AUTOOFF] = 0;

	// check 是否有人聽牌且胡了, 且下家先胡
	foreach( object ppl in Psort[MJdata[TURN]+1..]+Psort[0..MJdata[TURN]-1] )
	{
		if( member_array(arg, Pdata[ppl][LISTEN]) != -1 || member_array(arg, Pdata[ppl][AUTOHU]) != -1 )
		{
			MJdata[LOSER] = me;
			MJdata[KEYTILE] = arg;
			do_hu(ppl, 0);
			return;
		}
	}
	MJdata[MO] = 1;

	Psort -= ({ 0 });

	if( MJdata[TURN] == sizeof(Psort) - 1 )
		MJdata[TURN] = 0;
	else
		MJdata[TURN]++;

	MJdata[NINEYOUNGS] = 0;

	// 輪到的人 beep
	beep(Psort[MJdata[TURN]]);

	show_all_tile();

	// 輪到的人 beep
	beep(Psort[MJdata[TURN]]);

	// 如果自己將自動碰槓牌打出, 則取消自動
	auto_function_check(me, arg);

	// 打完牌後 check 是否要取消 AUTOHU
	foreach( string tile in data[AUTOHU] )
	if( !valid_hu_check(tile_sort(data[TILE]+({tile})), data[OTILE], data[FLOWER]) )
	{
		tell(me, "取消自動胡"+full_chinese(tile)+"。\n", 0);
		data[AUTOHU] -= ({ tile });
	}

	// check 是否有人要自動碰槓或胡
	foreach( object ppl in Psort )
	{
		if( ppl == me ) continue;

		if( arrayp(Pdata[ppl][AUTOGON]) && member_array(arg, Pdata[ppl][AUTOGON]) != -1 )
		{
			Pdata[ppl][AUTOGON] -= ({ arg });
			do_gon(ppl, 0);
			return;
		}
		if( arrayp(Pdata[ppl][AUTOPON]) && member_array(arg, Pdata[ppl][AUTOPON]) != -1 )
		{
			Pdata[ppl][AUTOPON] -= ({ arg });
			do_pon(ppl, 0);
			return;
		}
	}

	// AI 打完牌後檢查碰槓狀態與是否聽牌, 不聽死牌, 不聽單支
	if( Pdata[me][AI] )
	{
		if( !sizeof(Pdata[me][LISTEN]) )
		{
			array tin = me->AI_tin();
	
			me->AI_pg();
	
			if( arrayp(tin) )
			{
	
				if( !intp(tin[0]) )
				{
					foreach(string tile in tin )
					if( member_array(tile, Pdata[me][AUTOHU]) != -1 )
						tin -= ({ tile });
	
					if( sizeof(tin) )
					{
						do_au(me, "hu "+implode(tin, " "));
					}
				}
				else
				{
					do_tin(me, 0);
				}
			}
		}
		
		if( MJdata[LISTENING] )
		{
			Pdata[me][AUTOPON] = allocate(0);
			Pdata[me][AUTOGON] = allocate(0);
		}
	}

	// 開始聽牌
	if( Pdata[me][PLISTEN] )
	{
		Pdata[me][PLISTEN] = 0;
		do_tin(me, 0);
	}

	Pdata[me][PDUMPED] += ({ arg });

	// 若下家是 AI
	if( Pdata[Psort[MJdata[TURN]]][AI] )
	{
		string *eat_tiles;

		// AI 檢查是否該吃
		if( arrayp(eat_tiles = Psort[MJdata[TURN]]->AI_eat()) )
			do_eat(Psort[MJdata[TURN]], implode(eat_tiles, " "));
	}

	// check 下家聽牌自動摸
	if( sizeof(Pdata[Psort[MJdata[TURN]]][LISTEN]) || Pdata[Psort[MJdata[TURN]]][AI] )
		call_out((:do_mo, Psort[MJdata[TURN]], 0:), 1);
}

void do_eat(object me, string arg)
{
	int i1, i2, *s;
	string t1, t2, b1, b2;
	mapping data;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	if( MJdata[FINISH] )
		return tell(me, "已結束一圈牌局，若要繼續進行下一圈牌局請按 next。\n");

	if( !MJdata[DUMPING] )
		return tell(me, "現在沒有牌可以吃！\n");

	if( !MJdata[MO] )
		return tell(me, "現在無法吃牌。\n");

	if( Psort[MJdata[TURN]] != me )
		return tell(me, "還沒有輪到"+pnoun(2, me)+"！\n");

	data = Pdata[me];

	// 自動分析吃牌牌型
	if( !arg )
	{
		int choices;

		if( member_array(MJdata[DUMPING][1], ({ 'w', 's', 't' })) == -1 )
			return tell(me, "沒有辦法這樣吃喔！\n");

		if( MJdata[DUMPING][0] == '1' )
		{
			t1 = "2"+MJdata[DUMPING][1..1];
			t2 = "3"+MJdata[DUMPING][1..1];
		}
		else if( MJdata[DUMPING][0] == '9' )
		{
			t1 = "7"+MJdata[DUMPING][1..1];
			t2 = "8"+MJdata[DUMPING][1..1];
		}
		else
		{
			if( MJdata[DUMPING][0] >= '3' )
			{
				b1 = ""+(MJdata[DUMPING][0]-50)+MJdata[DUMPING][1..1];
				b2 = ""+(MJdata[DUMPING][0]-49)+MJdata[DUMPING][1..1];
				if( member_array(b1, data[TILE]) != -1 && member_array(b2, data[TILE]) != -1 )
				{
					t1 = b1;
					t2 = b2;
					choices++;
				}
			}

			b1 = ""+(MJdata[DUMPING][0]-49)+MJdata[DUMPING][1..1];
			b2 = ""+(MJdata[DUMPING][0]-47)+MJdata[DUMPING][1..1];
			if( member_array(b1, data[TILE]) != -1 && member_array(b2, data[TILE]) != -1 )
			{
				t1 = b1;
				t2 = b2;
				choices++;
			}

			if( MJdata[DUMPING][0] <= '7' )
			{
				b1 = ""+(MJdata[DUMPING][0]-47)+MJdata[DUMPING][1..1];
				b2 = ""+(MJdata[DUMPING][0]-46)+MJdata[DUMPING][1..1];
				if( member_array(b1, data[TILE]) != -1 && member_array(b2, data[TILE]) != -1 )
				{
					t1 = b1;
					t2 = b2;
					choices++;
				}
			}

			if( choices > 1 )
				return tell(me, "目前有多種吃牌組合，請自行輸入其中兩張牌名。\n");

			if( !choices )
				return tell(me, "沒有牌可以吃！\n");
		}
	}
	else if( sscanf(arg, "%s %s", t1, t2) != 2 )
		return tell(me, pnoun(2, me)+"想用哪兩張牌來吃牌？\n");

	if( strlen(t1) == 1 )
		t1 += MJdata[DUMPING][1..1];
	if( strlen(t2) == 1 )
		t2 += MJdata[DUMPING][1..1];

	i1 = member_array(t1, data[TILE]);
	if( i1 == -1 ) return tell(me, pnoun(2, me)+"沒有 "+t1+" 這張牌。\n");
	i2 = member_array(t2, data[TILE]);
	if( i2 == -1 ) return tell(me, pnoun(2, me)+"沒有 "+t2+" 這張牌。\n");

	if( t1[1] == t2[1] && t2[1] == MJdata[DUMPING][1] )
	{
		s = sort_array( ({ to_int(t1[0..0]), to_int(t2[0..0]), to_int(MJdata[DUMPING][0..0]) }), 1 );

		if( s[0] != 0 && s[0] == s[1] - 1 && s[1] == s[2] - 1 )
		{
			msg("$ME說了一聲："HIG"吃！\n"NOR, me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
			msg("$ME打出"+full_chinese(t1)+"和"+full_chinese(t2)+"將"+full_chinese(MJdata[DUMPING])+"吃下。\n", me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

			data[OTILE] += ({ sort_array(({t1,t2}),1)[0], MJdata[DUMPING], sort_array(({t1,t2}),1)[1] });

			data[TILE] = data[TILE][0..i1-1] + data[TILE][i1+1..];
			i2 = member_array(t2, data[TILE]);
			data[TILE] = data[TILE][0..i2-1] + data[TILE][i2+1..];

			MJdata[DUMPING] = 0;
			MJdata[MO] = 0;

			auto_function_check(me, t1);
			auto_function_check(me, t2);
			show_all_tile();

			if( data[AI] )
			{
				me->emotion_eat();
				call_out((:do_da, me, call_other(me, "AI_da"):), 2);
			}
			return;
		}
	}

	tell(me, "沒有辦法這樣吃喔！\n");
}


void do_pon(object me, string arg)
{
	int i;
	mapping data;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	if( MJdata[FINISH] )
		return tell(me, "已結束一圈牌局，若要繼續進行下一圈牌局請按 next。\n");

	if( !MJdata[DUMPING] )
		return tell(me, "現在沒有牌可以碰！\n");

	if( !MJdata[MO] )
		return tell(me, "現在無法碰牌。\n");

	data = Pdata[me];

	i = member_array(MJdata[DUMPING], data[TILE]);

	if( i != -1 && i < sizeof(data[TILE])-1 && data[TILE][i+1] == MJdata[DUMPING] )
	{
		beep();
		msg("$ME大喊一聲："HIG"碰啦！\n"NOR, me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
		msg("$ME將三張"+full_chinese(MJdata[DUMPING])+"擺到門前。\n", me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

		data[OTILE]	+= allocate(3, MJdata[DUMPING]);
		data[TILE]	= data[TILE][0..i-1] + data[TILE][i+2..];
		data[AUTOPON]	-= ({ MJdata[DUMPING] });
		MJdata[DUMPING] = 0;
		MJdata[MO]	= 0;
		MJdata[TURN]	= member_array(me, Psort);

		show_all_tile();

		if( data[AI] )
		{
			me->emotion_pon();
			call_out((:do_da, me, call_other(me, "AI_da"):), 2);
		}
	}
	else
		return tell(me, pnoun(2,me)+"沒有牌可以碰啦！\n");
}

void do_gon(object me, string arg)
{
	int i, j;
	mapping data;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	if( MJdata[FINISH] )
		return tell(me, "已結束一圈牌局，若要繼續進行下一圈牌局請按 next。\n");

	data = Pdata[me];

	if( sizeof(data[LISTEN]) && !data[AUTOOFF] )
		return tell(me, "目前為聽牌狀態，不能隨意槓牌。\n");

	// 暗槓或碰牌槓
	if( arg )
	{
		if( Psort[MJdata[TURN]] != me )
			return tell(me, "還沒有輪到"+pnoun(2, me)+"！\n");

		if( MJdata[MO] )
			return tell(me, "現在無法暗槓。\n");

		// 自摸牌暗槓
		i = member_array(arg, data[TILE]);
		if( i != -1 && i < sizeof(data[TILE]) - 3 && data[TILE][i] == data[TILE][i+3] )
		{
			msg("$ME大喊一聲："HIG"槓啦！\n"NOR, me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
			msg("$ME不懷好意的將四張暗槓牌擺到門前。\n", me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

			data[OTILE]	+= allocate(4, arg);
			data[AUTOGON]	-= ({ arg });
			data[TILE]	= data[TILE][0..i-1] + data[TILE][i+4..];
			data[SECRETGON] += ({ arg });
			data[TAILTILE]	= 1;
			MJdata[MO]	= 1;
			MJdata[TILELIMIT]++;
			show_all_tile();

			if( data[AI] )
				call_out((:do_mo, me, 0:), 1);

			return;
		}

		// 自己的碰牌再槓
		i = member_array(arg, data[TILE]);
		j = member_array(arg, data[OTILE]);
		if( i != -1 && j != -1 && j < sizeof(data[OTILE]) - 2 && data[OTILE][j] == data[OTILE][j+2] )
		{
			msg("$ME大喊一聲："HIG"槓啦！\n"NOR, me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
			msg("$ME將"+full_chinese(arg)+"擺到門前。\n", me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

			// 被搶槓
			foreach( object ppl in Psort[MJdata[TURN]+1..]+Psort[0..MJdata[TURN]-1] )
			if( member_array(arg, Pdata[ppl][LISTEN]) != -1 )
			{
				MJdata[LOSER] = me;
				MJdata[KEYTILE] = arg;
				MJdata[DUMPING] = arg; // fixed by sinji 搶槓時會詐胡
				do_hu(ppl, 0);
				return;
			}

			data[AUTOGON]	-= ({ arg });
			data[OTILE]	= data[OTILE][0..j+2]+({arg})+data[OTILE][j+3..];
			data[TILE]	= data[TILE][0..i-1] + data[TILE][i+1..];
			data[TAILTILE]	= 1;
			MJdata[MO]	= 1;
			MJdata[TILELIMIT]++;
			show_all_tile();
			return;
		}
	}

	if( !MJdata[DUMPING] )
		return tell(me, "現在沒有牌可以槓！\n");

	if( !MJdata[MO] )
		return tell(me, "現在無法槓牌。\n");

	/* 聽說不能這樣槓 -.-
		// 槓上家碰牌
		i = member_array(arg, data[OTILE]);
		if( i != -1 && i < sizeof(data[OTILE]) - 2 && data[OTILE][i] == data[OTILE][i+2] && data[OTILE][i] == MJdata[DUMPING] )
		{
			msg("$ME大喊一聲："HIG"槓啦！\n"NOR, me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
			msg("$ME將"+full_chinese(arg)+"擺到門前。\n", me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

			data[OTILE] = data[OTILE][0..i+2]+({arg})+data[OTILE][i+3..];
			MJdata[DUMPING] = 0;
			data[TAILTILE]	= 1;
			MJdata[MO]	= 1;
			MJdata[TILELIMIT]++;
			show_all_tile();
			return;
		}
	*/
	// 明槓
	i = member_array(MJdata[DUMPING], data[TILE]);
	if( i != -1 && i < sizeof(data[TILE]) - 2 && data[TILE][i] == data[TILE][i+2] && data[TILE][i] == MJdata[DUMPING])
	{
		if( Psort[MJdata[TURN]] == me )
		{
			if( Pdata[me][AI] )
			{
				call_out((:do_mo, me, call_other(me, "AI_da"):), 1);
				return;
			}

			return tell(me, "不能槓上家的牌喔。\n");
		}

		beep();
		msg("$ME大喊一聲："HIG"槓啦！\n"NOR, me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
		msg("$ME將四張"+full_chinese(MJdata[DUMPING])+"擺到門前。\n", me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

		data[OTILE]	+= allocate(4, MJdata[DUMPING]);
		data[TILE]	= data[TILE][0..i-1] + data[TILE][i+3..];
		data[AUTOGON] -= ({ MJdata[DUMPING] });
		MJdata[DUMPING] = 0;
		MJdata[TURN]	= member_array(me, Psort);
		MJdata[TILELIMIT]++;
		data[TAILTILE]	= 1;

		show_all_tile();

		if( data[AI] )
			call_out((:do_mo, me, 0:), 1);

		return;
	}
	return tell(me, pnoun(2,me)+"沒有牌可以槓啦！\n");
}

int tile_logic_seperate_1(array ref group, int i)
{
	int j, sep;

	// sep 條件 1
	j=i+1;
	while( j < sizeof(group) && sizeof(group[j]) == 1
	    && group[i][0][1] == group[j][0][1]
	    && group[j][0][0] - group[i][0][0] == j-i ) j++;

	if( !((j-i) % 3) )
		sep = 1;

	// sep 條件 2
	if( !sep && i < sizeof(group) - 5
	    && group[i+1][0][0] == group[i+5][0][0] - 4
	    && sizeof(unique_array(group[i+1..i+5], (: $1[0][1] :))) == 1
	    && sizeof(group[i+1]) == sizeof(group[i+5]) && sizeof(group[i+1]) == 1
	    && sizeof(unique_array(group[i+2..i+4], (: sizeof($1) :))) == 1
	    && sizeof(group[i+2]) > 1 )
		sep = 1;

	// sep 條件 3
	if( !sep && i < sizeof(group) - 4
	    && group[i+1][0][0] == group[i+4][0][0] - 3
	    && sizeof(unique_array(group[i+1..i+4], (: $1[0][1] :))) == 1
	    && sizeof(group[i+1]) == 1
	    && sizeof(group[i+2]) == sizeof(group[i+3]) + 1
	    && sizeof(group[i+3]) == sizeof(group[i+4]) )
		sep = 1;

	// sep 條件 4
	if( !sep && i < sizeof(group) - 4
	    && group[i+1][0][0] == group[i+4][0][0] - 3
	    && sizeof(unique_array(group[i+1..i+4], (: $1[0][1] :))) == 1
	    && sizeof(group[i+1]) == 2
	    && sizeof(group[i+2]) == sizeof(group[i+3]) + 1
	    && sizeof(group[i+3]) == sizeof(group[i+4]) + 1)
		sep = 1;

	// sep 條件 5
	if( !sep && i < sizeof(group) - 3
	    && group[i+1][0][0] == group[i+3][0][0] - 2
	    && sizeof(unique_array(group[i+1..i+3], (: $1[0][1] :))) == 1
	    && sizeof(group[i+1]) == 2 && sizeof(group[i+2]) == 2 && sizeof(group[i+3]) == 1 )
		sep = 1;

	// sep 條件 6
	if( !sep && i < sizeof(group) - 2 && sizeof(group[i+1]) == 4)
		sep = 1;

	if( sep )
		group = group[0..i-1]+({ group[i][0..<2] }) + ({ group[i][<1..<1] })+group[i+1..];

	return sep;
}


// 牌型邏輯分析與分組
array tile_logic_check(string *tiles)
{
	int i, j;
	array group;

	group = unique_array(tiles, (:$1:));

	//--(1)
	//1 22 33 4	-> 12 2 33 4	 -> 123 2 3 4	->(1)
	//1 2 33 4 5	-> 12 33 4 5	 -> 123 3 4 5	->(1)
	//1 222 333 44	-> 12 22 333 44  -> 123 22 33 44->(1)
	//1 2222 3	-> 12 222 3	 -> 123 222	->(3)
	//1 2 3333	-> 12 3333	 -> 123 333	->(3)
	//1 2 333	-> 12 333	 -> 123 33	->(2)
	//1 222 3	-> 12 22 3	 -> 123 22	->(2)

	//--(2)
	//11 22 33	    -> 12 1 2 33	    -> 123 1 2 3	->(3)
	//11 222 333 4	    -> 12 1 22 333 4	    -> 123 1 22 33 4	->(1)
	//11 2222 3333 44   -> 12 1 222 3333 44     -> 123 1 222 333 44 ->(1)
	//11 22 33 44	    -> 12 1 2 33 44	    -> 123 1 2 3 44	->(1)
	//11 2 3 4	    -> (P)
	//11 2 33 44 5	    -> (P)
	//11 2 333 444 55   -> (P)
	//11 22 333 444 5   -> (P)
	//11 2 3333 4444 555-> (P)
	//11 22 3333 4444 55-> (P)
	//11 222 3333 4444 5-> (P)

	//--(3)
	//111 2 3		 -> 11 1 2 3		 ->(1)
	//111 2 3 4 5 6 7 8 9	 -> 11 1 2 3 4 5 6 7 8 9 ->(1)

	//111 2 3 4 5 6 	 -> 11 1 2 3 4 5 6	 ->(1)
	//111 2 33 44 55 6	 -> 11 1 2 33 44 55 6	 ->(1)
	//111 2 333 444 555 6	 -> 11 1 2 333 444 555 6 ->(1)
	//111 2 3333 4444 5555 6 -> 11 1 2 3333 4444 5555 6 ->(1)

	//111 2 33 4 5		 -> 11 1 2 33 4 5	 ->(1)
	//111 2 333 44 55	 -> 11 1 2 333 44 55	 ->(1)
	//111 2 3333 444 555	 -> 11 1 2 3333 444 555  ->(1)

	//111 22 33 4		 -> 11 1 22 33 4	 ->(1)
	//111 22 333 44 5	 -> 11 1 22 333 44 5	 ->(1)
	//111 22 3333 444 55	 -> 11 1 22 3333 444 55  ->(1)

	//111 2222 3333 444
	//111 2222 3

	//111 222 3333 444 5	 -> (P)
	//111 2 3 4		 -> (P)
	//111 2 3 4 5 6 7	 -> (P)
	//111 22 33 44		 -> (P)
	//111 222 333 444	 -> (P)
	//111 2222 3333 4444	 -> (P)
	//111 222 333		 -> (P)
	//111 222 33		 -> (P)
	//111 2 33 44 5 	 -> (P)
	//111 2 333 444 55	 -> (P)
	//111 22 333 444 5	 -> (P)
	//111 2 3333 4444 555	 -> (P)
	//111 22 3333 4444 55	 -> (P)
	//111 222 3333 4444 5	 -> (P)
	//111 2 333 4444 555 6	 -> (P)
	//111 2222 3333 4	 -> (P)
	//123			 -> (P)

	//--(4)
	// 1111 2 3		-> 111 1 2 3
	// 1111 2 3 4 5 6	->
	// 1111 2 3 4 5 6 7 8 9 ->
	// 1111 2 33 44 5	->

	// 1111 22 33		-> 11 11 22 33
	// 1111

	for(i=0;i<sizeof(group);i++)
	{
		// 不是順子牌
		if( group[i][0][0] < '0' || group[i][0][0] > '9' ) continue;

		// 第一牌的第一數字大於 8
		if( group[i][0][0] >= '8' ) continue;

		switch( sizeof(group[i]) )
		{
			// 只有一張牌, 單純往下一張牌組檢查
		case 1	:

			// 至少還要兩組才有重組可能
			if( i >= sizeof(group) - 2 ) continue;

			// 不同的順子
			if( group[i][0][1] != group[i+1][0][1] ) continue;

			// 數字不連續
			if( group[i][0][0] != group[i+1][0][0] - 1 ) continue;

			// 重組牌子
			if( sizeof(group[i+1]) == 1 )
			{
				j = 1;
				group[i] += group[i+1];
				group = group[0..i]+group[i+2..];
			}
			else
			{
				j = 2;
				group[i] += group[i+1][0..0];
				group[i+1] = group[i+1][1..];
			}

			// 重新處理目前的牌子
			i--;
			continue;
			break;
			// 有兩張牌, 需往下兩組牌組檢查
		case 2:
			// 12
			if( group[i][0][0] == group[i][1][0] - 1 )
			{
				// 至少還要有一組才有重組可能
				if( i >= sizeof(group) - 1 ) continue;

				// 不同的順子
				if( group[i][0][1] != group[i+j][0][1] ) continue;

				// 數字不連續
				if( group[i][<1][0] != group[i+j][0][0] - 1 ) continue;

				// 重組牌子
				if( sizeof(group[i+j]) == 1 )
				{
					group[i] += group[i+j];
					group = group[0..i+j-1]+group[i+j+1..];
				}
				else
				{
					group[i] += group[i+j][0..0];
					group[i+j] = group[i+j][1..];
				}

				// 組合 123 完畢, 不需 i-- 重新整理

			}
			// 11 嘗試往後配兩對順子
			else
			{
				// 至少還要有兩組才有重組可能
				if( i >= sizeof(group) - 2 ) continue;

				// 後面的牌組不足以配兩對順
				if( sizeof(group[i+1]) < 2 || sizeof(group[i+2]) < 2 ) continue;

				// 不同的順子
				if( group[i][0][1] != group[i+1][0][1] || group[i][0][1] != group[i+2][0][1] ) continue;

				// 數字不連續
				if( group[i][<1][0] != group[i+1][0][0] - 1 || group[i][<1][0] != group[i+2][0][0] - 2 ) continue;

				// 特殊 Pass 狀態
				if( i < sizeof(group) - 4
				    && sizeof(unique_array(group[i+1..i+3], (: $1[0][1] :)))==1
				    && group[i+3][0][0] - group[i+1][0][0] == 2
				    && sizeof(group[i+2]) == sizeof(group[i+3])
				    && sizeof(group[i+1])+sizeof(group[i+4]) == sizeof(group[i+2]) ) continue;

				j = 3;
				group = group[0..i-1]+({ group[i][0..0] })+({ group[i][1..1] })+group[i+1..];
				group[i] += group[i+2][0..0];
				group[i+2] = group[i+2][1..];
				i--;

			}
			continue;
			break;
		case 3:
			// 已經是順子的就略過
			if( group[i][0][0] == group[i][2][0] - 2 ) continue;

			// 至少還要有兩組才有重組可能
			if( i >= sizeof(group) - 2 ) continue;

			// 不同的順子
			if( group[i][0][1] != group[i+1][0][1] || group[i][0][1] != group[i+2][0][1] ) continue;

			// 數字不連續
			if( group[i][<1][0] != group[i+1][0][0] - 1 || group[i][<1][0] != group[i+2][0][0] - 2 ) continue;

			tile_logic_seperate_1(ref group, i);
			continue;
			break;
		case 4:
			// 至少還要有兩組才有重組可能
			if( i >= sizeof(group) - 2 ) continue;

			// 不同的順子
			if( group[i][0][1] != group[i+1][0][1] || group[i][0][1] != group[i+2][0][1] ) continue;

			// 數字不連續
			if( group[i][<1][0] != group[i+1][0][0] - 1 || group[i][<1][0] != group[i+2][0][0] - 2 ) continue;

			// 若不能分開一個, 則直接分開兩個, 讓 size 2 檢查
			if( !tile_logic_seperate_1(ref group, i) )
				group = group[0..i-1]+({ group[i][0..1] }) + ({ group[i][2..3] })+group[i+1..];
			continue;
			break;
		}
	}
	return group;
}

void do_bonus(object me, string arg)
{
	string bonus = @BONUS

□臺灣十六張麻將台數計算

屁胡	[ 0 台] 搶槓	[ 1 台] 清一色	[ 8 台]
莊家	[ 1 台] 三仙台	[ 1 台] 字一色	[ 8 台]
自摸	[ 1 台] 平胡	[ 2 台] 四槓牌	[ 8 台]
門清	[ 1 台] 三暗刻	[ 2 台] 地聽	[ 8 台]
門清自摸[ 1 台] 同組花	[ 2 台] 小四喜	[ 8 台]
風牌台	[ 1 台] 全求人	[ 2 台] 八仙過海[ 8 台]
花牌台	[ 1 台] 碰碰胡	[ 4 台] 七搶一	[ 8 台]
單吊	[ 1 台] 小三仙	[ 4 台] 大四喜	[16 台]
胡中洞牌[ 1 台] 混一色	[ 4 台] 天聽	[16 台]
胡邊張牌[ 1 台] 四暗刻	[ 5 台] 地胡	[16 台]
海底撈月[ 1 台] 五暗刻	[ 8 台] 天胡	[24 台]
槓上開花[ 1 台] 大三仙	[ 8 台]
BONUS;

	tell(me, bonus);
}

// 台數計算
mapping bonus_calculate(object winner)
{
	int i, j;
	string t;
	array group, allgroup;
	mapping bonus = allocate_mapping(0);
	mapping data = Pdata[winner];
	string *flower = data[FLOWER];
	string *tile = data[TILE];
	string *otile = data[OTILE];
	string *listen = data[LISTEN];

	// 分析聽哪幾張牌
	if( !sizeof(listen) )
	{
		string *original_tile = tile - ({ MJdata[KEYTILE] });
		
		foreach( t in Tsort )
		{
			i = member_array(t, original_tile );
	
			// 略過手上已經握住的槓牌
			if( i != -1 && i < sizeof(original_tile)-3 && original_tile[i] == original_tile[i+3] ) continue;
	
			if( valid_hu_check(tile_sort(original_tile + ({ t })), 0, 0) )
				listen += ({ t });
		}
	}
	allgroup = tile_logic_check(tile);
	if( sizeof(otile) )
	{
		for(i=0;i<sizeof(otile);i++)
		{
			// 槓子排一組
			if( i < sizeof(otile) - 3 && otile[i] == otile[i+3] )
			{
				allgroup += ({ otile[i..i+3] });
				i+=3;
				continue;
			}
			else
			{
				allgroup += ({ tile_sort(otile[i..i+2]) });
				i+=2;
				continue;
			}
		}
	}

	// 莊家
	if( Psort[MASTER] == winner || MJdata[LOSER] == Psort[MASTER] )
		bonus["莊家"] = 1;

	if( MJdata[CMASTER] )
	{
		if( Psort[MASTER] == winner || MJdata[LOSER] == Psort[MASTER] )
//			bonus["連"+CHINESE_D->chinese_number(MJdata[CMASTER])] = MJdata[CMASTER];

//		else if( MJdata[LOSER] == Psort[MASTER] )
			bonus["連"+CHINESE_D->chinese_number(MJdata[CMASTER])+"拉"+CHINESE_D->chinese_number(MJdata[CMASTER])] = MJdata[CMASTER]*2;
	}

	// 自摸
	if( MJdata[HUMYSELF] )
		bonus["自摸"] = 1;

	// 門清
	i=0;
	if( !sizeof(otile) )
		bonus["門清"] = 1;
	else
		foreach( t in otile )
	{
		if( member_array(t, data[SECRETGON]) == -1 )
			i = 1;
	}
	if( i==0 )
		bonus["門清"] = 1;

	if( bonus["門清"] && bonus["自摸"] )
	{
		map_delete(bonus, "門清");
		map_delete(bonus, "自摸");
		bonus["門清一摸三"] = 3;
	}

	// 風牌
	foreach( group in allgroup )
	{
		if( sizeof(group) >= 3 && (i = member_array(group[0], ({"ea","so","we","no"}))) != -1 )
		{

			// 圈風
			if( i == MJdata[ROUNDWIND] )
				bonus[Dsort[i]+"風"]++;

			// 莊家風
			if( i == member_array(winner, Psort) )
				bonus[Dsort[i]+"風"]++;

			if( bonus[Dsort[i]+"風"] == 2 )
			{
				map_delete(bonus, Dsort[i]+"風");
				bonus[Dsort[i]+"風"+Dsort[i]] = 2;
			}
		}
	}

	// 花牌
	foreach( t in flower )
	if( t[1]-49 == data[FLOWERNUM] || t[1]-53 == data[FLOWERNUM] ) 
	{
		string flowername = CHINESE_D->chinese_number(data[FLOWERNUM]+1)+"花";
		if( ++bonus[flowername] == 2 )
		{
			map_delete(bonus, flowername);
			bonus["雙"+flowername] = 2;
			break;
		}
	}

	// 單吊
	if( sizeof(listen) == 1 )
		bonus["單吊"] = 1;


		// 胡邊張牌
	if( sizeof(listen) && !bonus["單吊"])
		foreach( group in tile_logic_check(tile) )
		if( sizeof(group) == 3 && group[0][0] == group[2][0] - 2 )
		{
			if(( MJdata[KEYTILE] == group[2] && group[2][0] == '3' )
			    || ( MJdata[KEYTILE] == group[0] && group[0][0] == '7' ))
				bonus["胡邊張牌"] = 1;
		}

		// 胡中洞牌
	if( sizeof(listen) && !bonus["單吊"] && !bonus["胡邊張牌"] )
		foreach( group in tile_logic_check(tile) )
		if( sizeof(group) == 3 && group[0][0] == group[2][0] - 2 && MJdata[KEYTILE] == group[1] )
			bonus["胡中洞牌"] = 1;

		// 海底摸月
	if( MJdata[HUMYSELF] && (strlen(Ctile)/2) == MJdata[TILELIMIT] )
		bonus["海底撈月"] = 1;

	// 槓上開花
	if( data[TAILTILE] )
		bonus["槓上開花"] = 1;

	// 搶槓
	foreach( object ppl in Psort-({winner}) )
	foreach( group in tile_logic_check(Pdata[ppl][TILE]) )
	if( sizeof(group) == 3 && group[0] == MJdata[KEYTILE] && member_array(group[0], Pdata[ppl][AUTOGON]) != -1)
	{
		bonus["搶槓"] = 1;
		break;
	}

	// 平胡
	i=0;
	if( !sizeof(flower) && !MJdata[HUMYSELF] && !MJdata["單吊"] && !bonus["胡中洞牌"] )
	{
		foreach( group in allgroup )
		{
			if( sizeof(group) >= 3 && group[0][0] != group[2][0] - 2 )
				i = 1;
			else if( sizeof(group) == 2
			    && (member_array(group[0], ({"ea","so","we","no"})) == MJdata[ROUNDWIND]
				|| member_array(group[0], ({"ea","so","we","no"})) == member_array(winner, Psort) ))
				i = 1;
		}


		if( i == 0 )
			bonus["平胡"] = 2;
	}

	// 三,四,五暗刻
	i = sizeof(data[SECRETGON]);
	foreach( group in tile_logic_check(tile) )
	{
		if( sizeof(group) == 3 && group[0] == group[2] && member_array(group[0], listen) == -1 )
			i++;
	}
	if( i == 3 )
		bonus["三暗刻"] = 2;
	else if( i == 4 )
		bonus["四暗刻"] = 5;
	else if( i == 5 )
		bonus["五暗刻"] = 8;

	// 同組花
	if( sizeof(flower) >= 4 )
	{
		if( sizeof(flower & ({"f1","f2","f3","f4"})) == 4 )
			bonus["春夏秋冬"] += 2;
		if( sizeof(flower & ({"f5","f6","f7","f8"})) == 4 )
			bonus["梅蘭竹菊"] += 2;
	}

	// 全求人
	if( sizeof(tile) == 2 )
		bonus["全求人"] = 2;


	// 碰碰胡
	i=0;
	foreach( group in allgroup )
	{
		if( sizeof(group) >= 3 && group[0] != group[2] )
			i = 1;
	}
	if( i == 0 )
		bonus["碰碰胡"] = 4;

	// 小三仙, 大三仙, 三仙台
	i=j=0;
	foreach( group in allgroup )
	{
		if( sizeof(group) >= 3 && group[0] == group[2] && member_array(group[0], ({"jo","ba","fa"})) != -1 ) 
		{
			i++;
			bonus[MJtile[group[0]][0]] = 1;
		}
		else if( sizeof(group) == 2 && group[0] == group[1] && member_array(group[0], ({"jo","ba","fa"})) != -1 )
			j++;
	}
	
	if( i == 3 || (i == 2 && j == 1) ) 
	{
		map_delete(bonus, "青發");
		map_delete(bonus, "紅中");
		map_delete(bonus, "白板");

		if( i == 3 )
			bonus["大三仙"] = 8;
		else if( i == 2 && j == 1 )
			bonus["小三仙"] = 4;
	}

	// 清一色
	i=j=0;
	foreach( group in allgroup )
	{
		if( !i ) i = group[0][1];
		else if( i != group[0][1] ) j = 1;
	}
	if( j == 0 )
		bonus["清一色"] = 8;

	// 混一色, 或清一色(字牌)
	i=j=0;
	foreach( group in allgroup )
	{
		if( member_array(group[0], ({"jo","ba","fa","ea","so","we","no"})) != -1 ) continue;

		if( !i ) i = group[0][1];
		else if( i != group[0][1] ) j = 1;
	}
	if( i == 0 )
		bonus["字一色"] = 8;
	else if( j == 0 && !bonus["清一色"] )
		bonus["混一色"] = 4;

	// 四槓牌
	i=j=0;
	for(i=0;i<sizeof(otile)-3;i++)
		if( otile[i] == otile[i+3] )
			j++;
	if( j >= 4 )
		bonus["四槓牌"] = 8;

	// 小四喜, 大四喜
	i=j=0;
	foreach( group in allgroup )
	{
		if( sizeof(group) >= 3 && group[0] == group[2] && member_array(group[0], ({"ea","so","we","no"})) != -1 )
			i++;
		else if( sizeof(group) == 2 && group[0] == group[1] && member_array(group[0], ({"ea","so","we","no"})) != -1 )
			j++;
	}
	if( i == 4 )
		bonus["大四喜"] = 16;
	else if( i == 3 && j == 1 )
		bonus["小四喜"] = 8;

	// 地聽
	if( data[LISTENTYPE] == 2 )
		bonus["地聽"] = 8;

	// 八仙過海
	if( sizeof(flower) == 8 )
		bonus["八仙過海"] = 8;

	// 七搶一
	if( sizeof(flower) == 7 )
		foreach( object ppl in Psort )
	{
		if( ppl == winner ) continue;
		if( sizeof(Pdata[ppl][FLOWER]) )
		{
			bonus["七搶一"] = 8;
			break;
		}
	}

	// 天聽
	if( data[LISTENTYPE] == 1 )
		bonus["天聽"] = 16;

	// 天胡
	if( MJdata[HUFIRST] )
		bonus["天胡"] = 24;

	else if( sizeof(MJdata[DUMPED]) <= 8 )
	{
		i = 0;
		foreach( object ppl in Psort )
		if( sizeof(Pdata[ppl][OTILE]) )
			i = 1;

		if( i == 0 )
			bonus["地胡"] = 16;
	}

	if( !sizeof(bonus) )
		bonus["屁胡"] = 0;

	return bonus;
}

// 贏家事件處理
void win_process(object winner, int cheat)
{
	int AIs, bonus, money, loss;
	string bonus_str = "", str, *arr;
	mapping data = Pdata[winner];
	object ppl;

	foreach( string bonus_type, int bonus_number in bonus_calculate(winner) )
	{
		bonus += bonus_number;
		bonus_str = sprintf(HIC"%-12s%2d 台\n", bonus_type, bonus_number) + bonus_str;
	}
	bonus_str += NOR;

	if( cheat )
	{
		bonus = 0;
		bonus_str += HIR"詐胡 "+bonus+" 台\n"NOR;
		MJdata[LOSER] = 0;
	}

	if( file_size(DATA_PATH) > 0 )
	{
		restore_object(DATA_PATH);
		set_idname("mj table", HIY"臺"NOR YEL"灣十六張麻將桌"NOR);
	}

	foreach( ppl in Psort )
	{
		if( undefinedp(score[Pdata[ppl][ID]]) )
			score[Pdata[ppl][ID]] = allocate_mapping(0);

		clear_screen(ppl);

		if( sizeof(data[FLOWER]+data[OTILE]) )
		{
			otile_display(ref arr, data, 0);

			str = "";
			if( sizeof(data[FLOWER]) ) str = HIW"  ┌"+repeat_string("─┬",sizeof(data[FLOWER])-1)+"─┐";
			if( sizeof(data[OTILE]) ) str += (sizeof(data[FLOWER])?"":HIW"	")+"┌"+repeat_string("─┬",sizeof(data[OTILE])-1)+"─┐"NOR;
			spec_line_msg(ppl, str, 2);
			spec_line_msg(ppl, NOR WHT"  "+kill_repeat_ansi(replace_string(arr[0], WHT, NOR)), 3);
			spec_line_msg(ppl, NOR WHT"  "+kill_repeat_ansi(arr[1])+NOR, 4);
			str = replace_string(str, "┬", "┴");
			str = replace_string(str, "┌", "└");
			str = replace_string(str, "┐", "┘");
			spec_line_msg(ppl, replace_string(str, HIW, NOR WHT), 5);
		}

		arr = allocate(2, "∣");
		foreach( string tile in data[TILE] )
		{
			arr[0] += NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+HIW"∣";
			arr[1] += NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+HIW"∣";
		}
		spec_line_msg(ppl, HIW"┌"+repeat_string("─┬",sizeof(data[TILE])-1)+"─┐"NOR, 5);
		spec_line_msg(ppl, HIW+arr[0]+NOR+(data[PLAYER]==Psort[MASTER]?HIG"莊 "NOR:"")+(sizeof(data[LISTEN])?HIC"聽("+implode(data[LISTEN],",")+") "NOR:""), 6);
		spec_line_msg(ppl, HIW+arr[1]+NOR+HIM+Dsort[member_array(winner, Psort)]+"家 "+(data[FLOWERNUM]+1)+" 花"NOR, 7);
		if( MJdata[KEYTILE] )
			spec_line_msg(ppl, HIW"└"+replace_string(implode(data[TILE],"┴"),MJdata[KEYTILE],HIR+MJdata[KEYTILE]+HIW,1,1)+"┘"+NOR+data[IDNAME]+NOR, 8);
		else
			spec_line_msg(ppl, HIW"└"+implode(data[TILE],"┴")+"┘"+NOR+data[IDNAME]+NOR, 8);
		// fixed by sinji at 2003/6/25 *Bad argument 2 to replace_string() - Expected: string Got: 0.
		// 未檢查關鍵牌是否存在

		if( MJdata[KEYTILE] )
			spec_line_msg(ppl, HIG" 關鍵牌："NOR+full_chinese(MJdata[KEYTILE]), 10);

		spec_line_msg(ppl, HIC" 胡牌："NOR+Pdata[winner][IDNAME]+NOR, 12);

		if( MJdata[LOSER] )
			spec_line_msg(ppl, HIR" 放槍："NOR+Pdata[MJdata[LOSER]][IDNAME]+NOR, 13);

		spec_line_msg(ppl, HIW" 台數計算：總計 "+bonus+" 台"NOR, 15);
	}

//	broadcast(environment(), HIW"台數計算：總計 "+bonus+" 台\n"NOR+bonus_str+"\n");
		broadcast(environment(), HIW"────────────\n"+NOR+bonus_str+HIW"────────────\n"HIW"台數計算：總計 "+bonus+" 台\n\n");

	if( cheat )
	{
		msg("\n$ME被眾人發現"HIR"∼詐胡∼\n\n"NOR, winner, 0, 1);

		money = BASEMONEY;

		if( Pdata[winner][MONEY] < money*(sizeof(Psort)-1) )
		{
			money = Pdata[winner][MONEY] / (sizeof(Psort)-1);
			Pdata[winner][MONEY] = 0;
			loss = 1;
		}
		else
			Pdata[winner][MONEY] -= money;

		score[Pdata[winner][ID]][SCORE] -= money*(sizeof(Psort)-1);

		foreach( ppl in Psort-({winner}) )
		{
			msg(HIW"$ME"HIW"付給$YOU"HIG"籌碼 $"+NUMBER_D->number_symbol(money)+"。\n"NOR, winner, ppl, 1);
			Pdata[ppl][MONEY] += money;
			score[Pdata[ppl][ID]][SCORE] += money;
		}

		MJdata[LOSER] = winner;
		score[Pdata[winner][ID]][GUNTIMES]++;
		score[Pdata[winner][ID]][LOSETIMES]++;
	}
	else
	{
		MJdata[WINNER] = winner;

		money = bonus*BONUSMONEY+BASEMONEY;

		// 有放槍者
		if( MJdata[LOSER] )
		{
			score[Pdata[winner][ID]][WINTIMES]++;

			if( Pdata[MJdata[LOSER]][MONEY] < money )
			{
				money = Pdata[MJdata[LOSER]][MONEY];
				Pdata[MJdata[LOSER]][MONEY] = 0;
				loss = 1;
			}
			else
				Pdata[MJdata[LOSER]][MONEY] -= money;

			score[Pdata[MJdata[LOSER]][ID]][SCORE] -= money;

			msg(HIW"$ME"HIW"付給$YOU"HIG"籌碼 $"+NUMBER_D->number_symbol(money)+"。\n"NOR, MJdata[LOSER], winner, 1);

			Pdata[winner][MONEY] += money;
			score[Pdata[winner][ID]][SCORE] += money;
			score[Pdata[winner][ID]][BONUSES] += bonus;

			score[Pdata[MJdata[LOSER]][ID]][LOSEBONUSES] += bonus;
			score[Pdata[MJdata[LOSER]][ID]][GUNTIMES]++;
			score[Pdata[MJdata[LOSER]][ID]][LOSETIMES]++;

		}
		// 自摸全收
		else
		{
			score[Pdata[winner][ID]][HUMYSELVES]++;

			foreach( ppl in Psort-({winner}) )
			{
				money = bonus*BONUSMONEY+BASEMONEY;
				if( Pdata[ppl][MONEY] < money )
				{
					money = Pdata[ppl][MONEY];
					Pdata[ppl][MONEY] = 0;
					loss = 1;
				}
				else
					Pdata[ppl][MONEY] -= money;

				// 檢查莊家連莊
				if( ppl == Psort[MASTER] )
				{
					score[Pdata[ppl][ID]][LOSEBONUSES] += 1;
					score[Pdata[winner][ID]][BONUSES] ++;

					money += BONUSMONEY;
					msg(HIG"$ME"HIG"莊家多賠 1 台。\n"NOR, ppl, 0, 1);

					// 連 X 拉 X
					if( MJdata[CMASTER] )
					{
						score[Pdata[ppl][ID]][LOSEBONUSES] += MJdata[CMASTER]*2;
						score[Pdata[winner][ID]][BONUSES] += MJdata[CMASTER]*2;

						msg(HIG"$ME"HIG"莊家連"+CHINESE_D->chinese_number(MJdata[CMASTER])+"拉"+CHINESE_D->chinese_number(MJdata[CMASTER])+"再賠 "+(MJdata[CMASTER]*2)+" 台。\n"NOR, ppl, 0, 1);
						money += MJdata[CMASTER]*2*BONUSMONEY;
					}
				}

				score[Pdata[ppl][ID]][SCORE] -= money;
				score[Pdata[ppl][ID]][LOSEBONUSES] += bonus;
				score[Pdata[ppl][ID]][LOSETIMES]++;

				msg(HIW"$ME"HIW"付給$YOU"HIG"籌碼 $"+NUMBER_D->number_symbol(money)+"。\n"NOR, ppl, winner, 1);

				Pdata[winner][MONEY] += money;
				score[Pdata[winner][ID]][SCORE] += money;
				score[Pdata[winner][ID]][BONUSES] += bonus;
			}
		}
	}

	broadcast(environment(), "\n");

	foreach( ppl in Psort )
	{
		if( Pdata[ppl][AI] )
		{
			loss = 0;
			Pdata[ppl][MONEY] = 200000;
			AIs++;
		}

		score[Pdata[ppl][ID]][ROUNDS]++;

		if( Pdata[ppl][SCORE] == 0 )
			msg(HIR"$ME已經把 $"+NUMBER_D->number_symbol(money)+" 籌碼輸的精光，無法繼續下一局牌局了。\n"NOR, ppl, 0, 1);

		else if( !loss )
			tell(ppl, "繼續進行下一牌局請按 next。\n", 0);
	}

	if( AIs == 4 )
		call_out((:next_MJ:), 10);

	if( loss )
		MJdata[CFINISH] = 1;


	save_object(DATA_PATH);
}

// 檢查是否已經胡牌
int valid_hu_check(string *tile, string *otile, string *flower)
{
	int gon4, valid = 1;
	int i, *form = allocate(6, 0);

	foreach( array tmp in tile_logic_check(tile) )
	{
		if( sizeof(tmp) == 2 && tmp[0] != tmp[1] )
			valid = 0;

		form[sizeof(tmp)-1]++;
	}

	if( form[1] > 1 || form[0] || form[3] ) valid = 0;

	if( arrayp(flower) )
	{
		// 七搶一胡牌
		if( sizeof(flower) == 7 )
			foreach( object ppl in Psort )
			if( sizeof(Pdata[ppl][FLOWER]) == 1 )
			{
				MJdata[LOSER] = ppl;
				MJdata[KEYTILE] = Pdata[ppl][FLOWER][0];
				valid = 1;
				break;
			}

			// 八仙過海胡牌
			else if( sizeof(flower) == 8 )
				valid = 1;
	}

	// 四槓牌
	for(i=0;i<sizeof(otile)-3;i++)
		if( otile[i] == otile[i+3] )
			gon4++;

	if( gon4 >= 4 )
		valid = 1;

	return valid;
}

// 胡牌指令
void do_hu(object me, string arg)
{
	int cheat;
	mapping data;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	if( MJdata[FINISH] )
		return tell(me, "已結束一圈牌局，若要繼續進行下一圈牌局請按 next。\n");

	// 找出放槍者或檢查是否能胡
	if( MJdata[TURN] == 0 )
	{
		if( Psort[<1] == me && MJdata[MO] )
			return tell(me, pnoun(2, me)+"想詐胡嗎！？\n");

		if( !MJdata[LOSER] )
			MJdata[LOSER] = Psort[<1];
	}
	else
	{
		if( Psort[MJdata[TURN]-1] == me && MJdata[MO] )
			return tell(me, pnoun(2, me)+"想詐胡嗎！？\n");

		if( !MJdata[LOSER] )
			MJdata[LOSER] = Psort[MJdata[TURN]-1];
	}
	data = Pdata[me];

	// 詐胡
	if( !valid_hu_check(data[TILE], data[OTILE], data[FLOWER]) )
	{
		if( MJdata[DUMPING] )
		{
			if( valid_hu_check( tile_sort(data[TILE]+({MJdata[DUMPING]})), data[OTILE], data[FLOWER]) )
				data[TILE] = tile_sort(data[TILE]+({MJdata[DUMPING]}));
			else cheat = 1;
		}
		else cheat = 1;
	}

	if( cheat && Pdata[me][AI] )
		return;

	MJdata[KEYTILE] = MJdata[DUMPING];

	beep();
	msg("$ME很機車的大叫一聲："HIY"我胡了啦！哇哈哈哈哈∼\n"NOR, me, 0, 1);

	if( cheat )
		MJdata[LOSER] = me;

	win_process(me, cheat);

	if( sizeof(Psort) < 4 )
		set_temp("status", HIY"欠"+NOR YEL+CHINESE_D->chinese_number(4-sizeof(Psort))+HIY"腳"NOR, this_object());

	
	MJdata[FINISH] = time()+20;
	
	//set_idname("mj table", HIY"臺"NOR YEL"灣十六張麻將桌"NOR);
}

void do_mo(object me, string arg)
{
	string tile;
	mapping data;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	if( MJdata[FINISH] )
		return tell(me, "已結束一圈牌局，若要繼續進行下一圈牌局請按 next。\n");

	if( !MJdata[MO] )
		return tell(me, "現在不能摸牌。\n");

	if( Psort[MJdata[TURN]] != me )
		return tell(me, "還沒有輪到"+pnoun(2, me)+"！\n");

	data = Pdata[me];

	// 由尾張捉牌
	if( data[TAILTILE] )
	{
		tile = Ctile[<2..<1];
		Ctile = Ctile[0..<3];
		tell(me, pnoun(2, me)+"由牌尾摸到了一張"+full_chinese(tile)+"。\n", 0);
		broadcast(environment(), data[IDNAME]+"由牌尾伸手摸了一張牌。\n", ENVMSG, all_inventory(environment(this_object()))-Psort+({me})-MJdata[ONLOOKER]);
	}
	else
	{
		tile = Ctile[0..1];
		Ctile = Ctile[2..];
		tell(me, pnoun(2, me)+"摸到了一張"+full_chinese(tile)+"。\n", 0);
		broadcast(environment(), data[IDNAME]+"伸手摸了一張牌。\n", ENVMSG, all_inventory(environment(this_object()))-Psort+({me}));
	}

	data[TILE] += ({ tile });

	auto_function_check(me, tile);

	check_flower(ref data, tile);

	tile = data[TILE][<1];

	data[TILE] = tile_sort(data[TILE]);

	foreach( object ppl in Psort )
	{
		if( ppl == me )
		{
			show_tile(me);
			continue;
		}

		if( Pdata[ppl][LOOKALL] )
			show_all(ppl);
		else if( !Pdata[ppl][LOOKSEA] )
			show_dumping(ppl);
	}

	MJdata[MO] = 0;

	if( MJdata[FINISH] ) return;

	// 自摸
	if( valid_hu_check(data[TILE], data[OTILE], data[FLOWER]) )
	{
		beep();
		msg("$ME很機車的大叫一聲："HIY"『自摸』啦！哇哈哈哈哈∼\n"NOR, me, 0, 1);
		MJdata[HUMYSELF] = 1;
		MJdata[KEYTILE] = tile;
		win_process(me, 0);

		if( sizeof(Psort) < 4 )
			set_temp("status", HIY"欠"+NOR YEL+CHINESE_D->chinese_number(4-sizeof(Psort))+HIY"腳"NOR, this_object());
		MJdata[FINISH] = time()+20;
		return;
	}

	data[TAILTILE] = 0;

	// 流局
	if( strlen(Ctile)/2 <= (MJdata[TILELIMIT]+(4-sizeof(Psort))*18) )
	{
		int AIs;

		beep();
		broadcast(environment(), HIG"牌子只剩下 "+(strlen(Ctile)/2)+" 張，本局宣告流局，請按 next 繼續下一局。\n"NOR);

		if( sizeof(Psort) < 4 )
			set_temp("status", HIY"欠"+NOR YEL+CHINESE_D->chinese_number(4-sizeof(Psort))+HIY"腳"NOR, this_object());

		MJdata[FINISH] = time()+5;

		foreach( object ppl in Psort )
		{
			if( !score[Pdata[ppl][ID]] )
				score[Pdata[ppl][ID]] = allocate_mapping(0);
			if( Pdata[ppl][AI] ) AIs++;
			score[Pdata[ppl][ID]][ROUNDS]++;
		}

		if( AIs == 4 )
			call_out((:next_MJ:), 10);
		return;
	}

	// 聽牌摸牌後的槓牌與打牌情形
	if( sizeof(Pdata[me][LISTEN]) )
	{
		int i;

		if( !Pdata[me][AI] )
		{
			// 自己判定是否暗槓
			i = member_array(tile, data[TILE]);
			if( i != -1 && i < sizeof(data[TILE]) - 3 && data[TILE][i] == data[TILE][i+3] )
			{
				Pdata[me][AUTOOFF] = 1;
				return tell(me, "請決定是否要暗槓，gon [牌名]。\n");
			}
			i = member_array(tile, data[OTILE]);
			if( i != -1 && i < sizeof(data[OTILE]) - 2 && data[OTILE][i] == data[OTILE][i+2] )
			{
				Pdata[me][AUTOOFF] = 1;
				return tell(me, "請決定是否要槓牌，gon [牌名]。\n");
			}
		}

		call_out((:do_da, me, tile:), 2);
		return;
	}

	if( Pdata[me][AI] )
	{
		// AI 暗槓
		int i = member_array(tile, data[TILE]);
		
		me->emotion_mo();
		
		if( i != -1 && i < sizeof(data[TILE]) - 3 && data[TILE][i] == data[TILE][i+3] )
			call_out( (:do_gon, me, tile:), 2);
		else
			call_out((:do_da, me, call_other(me, "AI_da"):), 2);
	}

	foreach( object ppl in MJdata[ONLOOKER] )
	{
		if( !objectp(ppl) )
		{
			MJdata[ONLOOKER] -= ({ ppl });
			continue;
		}
		show_all(ppl);
	}

}

void do_au(object me, string arg)
{
	int i;
	string str, arg1, arg2, *args;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	if( MJdata[FINISH] )
		return tell(me, "已結束一圈牌局，若要繼續進行下一圈牌局請按 next。\n");

	if( !arg )
	{
		if( !sizeof(Pdata[me][AUTOPON]+Pdata[me][AUTOGON]+Pdata[me][AUTOHU]) )
			return tell(me, pnoun(2, me)+"目前沒有設定任何自動功能。\n");

		if( sizeof(Pdata[me][AUTOPON]) )
		{
			str = "";
			foreach( string tile in Pdata[me][AUTOPON] )
			str += full_chinese(tile)+"、";

			tell(me, "自動碰的牌有："+str[0..<3]+"。\n");
		}
		if( sizeof(Pdata[me][AUTOGON]) )
		{
			str = "";
			foreach( string tile in Pdata[me][AUTOGON] )
			str += full_chinese(tile)+"、";
			tell(me, "自動槓的牌有："+str[0..<3]+"。\n");
		}
		if( sizeof(Pdata[me][AUTOHU]) )
		{
			str = "";
			foreach( string tile in Pdata[me][AUTOHU] )
			str += full_chinese(tile)+"、";
			tell(me, "自動胡的牌有："+str[0..<3]+"。\n");
		}
		return;
	}

	if( arg == "tin" || arg == "t" )
	{
		tell(me, "設定自動聽牌完成。\n");
		Pdata[me][PLISTEN] = 1;
		return;
	}
	if( sscanf(arg, "%s %s", arg1, arg2) != 2 )
		return tell(me, pnoun(2, me)+"想要將什麼動作自動化？\n");

	switch( arg1 )
	{
	case "h":
	case "hu":
		args = explode(arg2, " ");
		foreach( arg in args )
		{
			if( member_array(arg, Tsort) == -1 )
				return tell(me, "沒有 "+arg+" 這張牌。\n");

			if( member_array(arg, Pdata[me][AUTOHU]) != -1 )
			{
				tell(me, "取消自動胡"+full_chinese(arg)+"。\n");
				Pdata[me][AUTOHU] -= ({ arg });
			}
			else if( !valid_hu_check(tile_sort(Pdata[me][TILE]+({ arg })), 0, 0) )
				return tell(me, pnoun(2, me)+"沒有辦法靠"+full_chinese(arg)+"胡牌。\n");
			else
			{
				auto_function_check(me, arg);
				tell(me, "設定自動胡"+full_chinese(arg)+"完成。\n");
				Pdata[me][AUTOHU] += ({ arg });
			}
		}
		break;
	case "p":
	case "pon":

		if( member_array(arg2, Tsort) == -1 )
			return tell(me, "沒有 "+arg2+" 這種牌。\n");

		i = member_array(arg2, Pdata[me][TILE]);
		if( i == -1 ) return tell(me, pnoun(2, me)+"手上沒有"+full_chinese(arg2)+"。\n");

		if( i < sizeof(Pdata[me][TILE]) - 1 && Pdata[me][TILE][i+1] == arg2 )
		{
			if( member_array(arg2, Pdata[me][AUTOPON]) != -1 )
			{
				Pdata[me][AUTOPON] -= ({ arg2 });
				return tell(me, pnoun(2, me)+"取消自動碰"+full_chinese(arg2)+"。\n");
			}

			Pdata[me][AUTOPON] += ({ arg2 });
			Pdata[me][AUTOGON] -= ({ arg2 });

			tell(me, "設定自動碰"+full_chinese(arg2)+"完成。\n", 0);
		}
		else
			tell(me, pnoun(2, me)+"手上的"+full_chinese(arg2)+"不夠碰啦。\n");
		break;
	case "g":
	case "gon":
		if( member_array(arg2, Tsort) == -1 )
			return tell(me, "沒有 "+arg2+" 這種牌。\n");

		i = member_array(arg2, Pdata[me][TILE]);
		if( i == -1 ) return tell(me, pnoun(2, me)+"手上沒有"+full_chinese(arg2)+"。\n");

		if( i < sizeof(Pdata[me][TILE]) - 2 && Pdata[me][TILE][i+2] == arg2 )
		{
			if( member_array(arg2, Pdata[me][AUTOGON]) != -1 )
			{
				Pdata[me][AUTOGON] -= ({ arg2 });
				return tell(me, pnoun(2, me)+"取消自動槓"+full_chinese(arg2)+"。\n");
			}

			Pdata[me][AUTOGON] += ({ arg2 });
			Pdata[me][AUTOPON] -= ({ arg2 });
			tell(me, "設定自動槓"+full_chinese(arg2)+"完成。\n", 0);
		}
		else
			tell(me, pnoun(2, me)+"手上的"+full_chinese(arg2)+"不夠槓啦。\n");
		break;
	default:
		return tell(me, "指令格式：au [hu|pon|gon]\n");
	}
}

void do_tin(object me, string arg)
{
	int i;
	string str = "";
	string *listening, *mytiles;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	if( MJdata[FINISH] )
		return tell(me, "已結束一圈牌局，若要繼續進行下一圈牌局請按 next。\n");

	if( sizeof(Pdata[me][LISTEN]) )
	{
		foreach( string tile in Pdata[me][LISTEN] )
		str += full_chinese(tile)+"、";
		tell(me, "目前所聽的牌為："+str[0..<3]+"。\n");
		return;
	}

	if( Psort[MJdata[TURN]] == me && !MJdata[MO] )
		return tell(me, "現在輪到"+pnoun(2, me)+"打牌，不能喊聽牌。\n");

	mytiles = Pdata[me][TILE];

	listening = allocate(0);

	// 自動分析要聽哪幾張牌
	foreach( string tile in Tsort )
	{
		i = member_array(tile, mytiles);

		// 略過手上已經握住的槓牌
		if( i != -1 && i < sizeof(mytiles)-3 && mytiles[i] == mytiles[i+3] ) continue;

		if( valid_hu_check(tile_sort(mytiles+({ tile })), 0, 0) )
			listening += ({ tile });
	}

	if( sizeof(listening) )
	{
		beep();

		msg("$ME「嘿、嘿、嘿」地奸笑了幾聲，冷冷地說："HIG"我聽牌了喔。\n"NOR, me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

		if( Psort[MASTER] == me && sizeof(MJdata[DUMPED]) <= 1 )
		{
			msg(HIG"\n$ME"HIG"目前為天聽狀態。\n\n"NOR, me, 0, 1);
			Pdata[me][LISTENTYPE] = 1;
		}

		else if( sizeof(MJdata[DUMPED]) <= 8 )
		{
			i = 0;
			foreach( object ppl in Psort )
			if( sizeof(Pdata[ppl][OTILE]) )
				i = 1;
			if( i==0 )
			{
				msg(HIG"\n$ME"HIG"目前為地聽狀態。\n\n"NOR, me, 0, 1);
				Pdata[me][LISTENTYPE] = 2;
			}
		}

		Pdata[me][AUTOPON] = allocate(0);
		Pdata[me][AUTOGON] = allocate(0);
		Pdata[me][AUTOHU]  = allocate(0);
		Pdata[me][LISTEN]  = listening;

		show_all_tile();

		MJdata[LISTENING] = 1;

		if( Psort[MJdata[TURN]] == me )
			do_mo(me, 0);
	}
	else
		return tell(me, pnoun(2, me)+"這種牌不能夠聽牌啦！\n");
}

void show_all(object me)
{
	int i=1;
	string arr, tile, str="";

	clear_screen(me);

	foreach( object ppl in Psort )
	{
		otile_display(ref arr, Pdata[ppl], 0);

		arr[0] += HIW"∣";
		arr[1] += HIW"∣";

		foreach( tile in Pdata[ppl][TILE] )
		{
			arr[0] += NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+HIW"∣";
			arr[1] += NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+HIW"∣";
		}

		str = "";
		str += sizeof(Pdata[ppl][FLOWER])?NOR WHT"┌"+repeat_string("─┬", sizeof(Pdata[ppl][FLOWER])-1)+"─┐"NOR:"";
		str += sizeof(Pdata[ppl][OTILE])?NOR WHT"┌"+repeat_string("─┬", sizeof(Pdata[ppl][OTILE])-1)+"─┐"NOR:"";
		str += HIW"┌"+repeat_string("─┬", sizeof(Pdata[ppl][TILE])-1)+"─┐"NOR;
		spec_line_msg(me, str, i++);
		spec_line_msg(me, arr[0]+" "+NOR+(ppl==Psort[0]?HIG"莊"+(MJdata[CMASTER]?"連"+CHINESE_D->chinese_number(MJdata[CMASTER]):"")+" "NOR:"")+(sizeof(Pdata[ppl][LISTEN])?HIC"聽("+implode(Pdata[ppl][LISTEN],",")+") "NOR:"")+NOR, i++);
		spec_line_msg(me, arr[1]+NOR+(member_array(ppl, Psort)==MJdata[TURN]?HBCYN" "NOR HIREV HIC+remove_ansi(Pdata[ppl][IDNAME])+NOR HBCYN" ":" "+Pdata[ppl][IDNAME]+" ")+NOR" ", i++);

		str = replace_string(str, "┌","└");
		str = replace_string(str, "┐","┘");
		str = replace_string(str, "┬","┴");

		spec_line_msg(me, str, i++);
	}
}

void do_lookall(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	if( !sizeof(Pdata[me][LISTEN]) && !MJdata[FINISH] )
		return tell(me, "聽牌後才可以看所有人的牌。\n");

	Pdata[me][LOOKALL] = 1;
	Pdata[me][LOOKSEA] = 0;

	show_all(me);
}

/* 特殊作弊指令天眼通 */
void do_skyeye(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	if( !wizardp(me) )
		return tell(me, "此指令為巫師專用指令。\n");

	Pdata[me][LOOKALL] = 1;
	Pdata[me][LOOKSEA] = 0;

	show_all(me);
}


void do_lookmj(object me, string arg)
{
	if( is_player(me) )
		return tell(me, pnoun(2, me)+"是其中一位玩家，無法看別人的牌。\n");

	if( !MJdata[PLAYING] )
		return tell(me, "牌局還沒開始。\n");

	if( member_array(me, MJdata[ONLOOKER]) == -1 )
	{
		MJdata[ONLOOKER] += ({ me });
		startup_title_screen(me, TITLE_SCREEN_LENGTH);

		if( !MJdata[FINISH] )
			show_all(me);

		me->add_status(OSTATUS);
		msg("$ME開始在麻將桌旁觀局。\n", me, 0, 1);
		return;
	}

	switch(arg)
	{
	case 0:
	case "cancel":
		reset_screen(me);
		me->remove_status(OSTATUS);
		MJdata[ONLOOKER] -= ({ me });
		msg("$ME取消了旁人觀局。\n", me, 0, 1);
		break;
	case "sea":show_sea(me);break;
	case "all":show_all(me);break;
	default:show_all(me);break;
	}
}

// 找人替補位子
void do_substitute(object me, mixed arg)
{
	object target;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	if( !arg )
		return tell(me, pnoun(2, me)+"想找誰替補？\n");

	if( objectp(arg) )
		target = arg;
	else if( !objectp(target = find_player(arg) || present(arg, environment()) ) )
		return tell(me, "這裡沒有 "+arg+" 這個人。\n");

	if( is_player(target) )
		return tell(me, pnoun(3, target)+"已經加入牌局了，無法替補"+pnoun(2, me)+"。\n");

	startup_title_screen(target, TITLE_SCREEN_LENGTH);
	
	target->add_status(STATUS);
	set_temp("mjobject", this_object(), target);
	
	Psort[member_array(me, Psort)] = target;

	Pdata[target] = copy(Pdata[me]);
	Pdata[target][PLAYER]	= target;
	Pdata[target][ID]	= target->query_id(1);
	Pdata[target][IDNAME]	= target->query_idname();

	if( !userp(target) )
		Pdata[target][AI] = 1;
	else
		Pdata[target][AI] = 0;

	map_delete(Pdata, me);

	me->remove_status(STATUS);
	reset_screen(me);

	MJdata[ONLOOKER] -= ({ target });

	beep();

	if( !MJdata[PLAYING] )
		show_title();
	else if( !MJdata[FINISH] )
		show_all_tile();

	msg("$ME離開麻將桌，一手將$YOU拉了過來，並強壓$YOU坐到麻將桌前並說道："HIG"交給"+pnoun(2, target)+"了！\n"NOR, me, target, 1);

	if( !userp(me) )
		destruct(me);
}

// 修復損壞的牌局
void do_fixmj(object me, string arg)
{
	mapping tempdata;
	object AIob;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"並沒有加入牌局。\n");

	foreach( object ppl, mapping data in Pdata )
	{
		if( objectp(ppl) ) continue;
		
		foreach(data in values(Pdata))
			if( !objectp(data[PLAYER]) )
				tempdata = copy(data);

		AIob = new(MJAI_FILE);

		Pdata[AIob] = tempdata;
		Pdata[AIob][PLAYER]	= AIob;
		Pdata[AIob][ID]		= AIob->query_id(1);
		Pdata[AIob][IDNAME]	= AIob->query_idname();
		Pdata[AIob][AI] 	= 1;
		
		map_delete(Pdata, ppl);

		Psort[member_array(ppl, Psort)] = AIob;

		AIob->move(environment());

		msg("$ME突然衝到麻將桌前並說道："HIG"我來救大家了！\n"NOR, AIob, 0, 1);
		
		set_temp("mjobject", AIob);
		do_mo(AIob, arg);
	}
}

void heart_beat()
{
	int player;
	
	if( sizeof(Pdata) )
	{
		foreach( object ppl in keys(Pdata) )
			if( objectp(ppl) && userp(ppl) )	
				player++;
		if( !player )
		{
			foreach(object ppl in keys(Pdata))
				destruct(ppl);
			
			reset_all_data();
		}
	}
}
	
	
void create()
{
	if( file_size(DATA_PATH) > 0 )
		restore_object(DATA_PATH);

	set_idname("mj table", HIY"臺"NOR YEL"灣十六張麻將桌"NOR);
	set("long", HIY"臺"NOR YEL"灣十六張麻將桌，help mj 看指令列表"NOR);
	set("unit", "桌");
	set("mass", -1);
	set("flag/no_amount", 1);
	
	save_object(DATA_PATH);

	reset_all_data();
	
	set_heart_beat(3000);
}

void delete_data(string id)
{
	restore_object(DATA_PATH);
	map_delete(score, id);
	save_object(DATA_PATH);
}

int remove()
{
	foreach( object ppl in Psort )
	{
		if( !objectp(ppl) ) continue;

		ppl->remove_status(STATUS);
		reset_screen(ppl);

		if( !userp(ppl) )
			destruct(ppl);
	}

	foreach( object onlooker in MJdata[ONLOOKER] )
	{
		if( !onlooker ) continue;

		onlooker->remove_status(OSTATUS);
		// fixed by sinji on 2003/6/25 - *Bad argument 2 to delete_temp() Expected: object Got: 0.
		// 未檢查物件是否存在
		reset_screen(onlooker);
	}
}
