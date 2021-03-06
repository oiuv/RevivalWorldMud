/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : stock.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-05-31
 * Note   : 證券交易所
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

inherit ROOM_ACTION_MOD;

#define PROCEDURE_FUND	50		// 2/100 的手續費
#define MAX_OWN_STOCK_TYPE 10		// 最大可同時持有的股票種類

string stockvalue_change_description(float change)
{
	change = to_float(change);

	if( change > 0 )
		return sprintf(HIR"▲%5.2f"NOR, change);
	else if( change < 0 )
		return sprintf(HIG"▼%5.2f"NOR, -change);
	else
		return sprintf(HIW"  %5.2f"NOR, change);
}


void do_list(object me, string arg)
{
	int i, sort;
	string amount, totalvalue;
	float value;
	string enterprise;
	string sortdesc;
	mapping data;
	string *enterprise_stockvalue_sort;
	string msg = "企業名稱                   股價 排名 最新漲跌   可購張數   目前持股  持股股價  持股總值 \n";
	msg += "─────────────────────────────────────────────\n";

	enterprise_stockvalue_sort = sort_array(ENTERPRISE_D->query_all_enterprises(), (: ENTERPRISE_D->query_enterprise_info($2, "stockvalue") > ENTERPRISE_D->query_enterprise_info($1, "stockvalue") ? 1 : -1 :));
		
	foreach(enterprise in ENTERPRISE_D->query_all_enterprises())
	{
		data = ENTERPRISE_D->query_enterprise_info(enterprise);
		
		value = to_float(query("stock/"+enterprise+"/value", me));
		amount = query("stock/"+enterprise+"/amount", me);
		totalvalue = count(count(amount, "*", to_int(value*1000000)), "/", 100);
		
		sort = member_array(enterprise, enterprise_stockvalue_sort)+1;
		
		switch(sort)
		{
			case 1: 	sortdesc = HIY BLINK + sort + NOR; break;
			case 2..10: 	sortdesc = HIY+ sort + NOR; break;
			default: 	sortdesc = NOR YEL + sort +NOR; break;
		}
				
		msg += sprintf("%-3d %-20s%7.2f %-4s %-8s %10s %10s   %7.2f  %s\n",
			++i,
			data["color_id"], 
			data["stockvalue"], 
			sortdesc,
			stockvalue_change_description(data["stockvalue_change"]),
			NUMBER_D->number_symbol(data["available_stockamount"]),
			NUMBER_D->number_symbol(amount),
			value,
			NUMBER_D->number_symbol(totalvalue)
			);
	}
	msg += "─────────────────────────────────────────────\n";
	msg += "股票單位為 10,000 元/張，交易手續費為 2%，股市每 10 分鐘變動一次\n\n";
	
	
	return me->more(msg);
}

void confirm_buystock(object me, string enterprise, string cost, string cost_extra, string new_amount, float new_value, float cur_value, int amount, string new_available_stockamount, float percent, string arg)
{
	if( !arg )
		return me->finish_input();
		
	arg = lower_case(arg);
	
	if( arg != "y" && arg != "yes" )
	{
		tell(me, "取消購入股票。\n");
		return me->finish_input();
	}

	if( !me->spend_money(MONEY_D->query_default_money_unit(), count(cost, "+", cost_extra)) )
	{
		tell(me, pnoun(2, me)+"身上沒有 "+money(MONEY_D->query_default_money_unit(), count(cost, "+", cost_extra))+"。\n");
		return me->finish_input();
	}
	
	set("stock/"+enterprise+"/amount", new_amount, me);
	set("stock/"+enterprise+"/value", new_value, me);
		
	ENTERPRISE_D->set_enterprise_info(enterprise, "available_stockamount", new_available_stockamount);
	CHANNEL_D->channel_broadcast("stock", me->query_idname()+"以 "+sprintf("%.2f", cur_value)+" 股價購入"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"股票 "+NUMBER_D->number_symbol(amount)+" 張，持股比例升為 "+sprintf("%.2f", percent)+"%");
	me->finish_input();
	
	TOP_D->update_top_stock(me);
	me->save();
}

void do_buystock(object me, string arg)
{
	int i, num, amount;
	string enterprise;
	mapping data;
	float old_value;
	float new_value;
	float cur_value;
	string old_amount;
	string new_amount;
	string new_available_stockamount;
	float percent;
	string cost;
	string cost_extra;
	
	if( !arg || sscanf(arg, "%d %d", num, amount) != 2 )
		return tell(me, "請輸入正確的格式。\n");

	if( num <= 0 || amount <= 0 )
		return tell(me, "請輸入大於 0 的數字。\n");

	i=0;
	foreach(enterprise in ENTERPRISE_D->query_all_enterprises())
	{
		if( ++i != num ) continue;
		
		data = ENTERPRISE_D->query_enterprise_info(enterprise);
		
		if( wizardp(me) )
		{
			if( !SECURE_D->is_wizard(data["president"]) )
				return tell(me, "購買股票會導致某企業獲利造成市場干預，巫師只能購買巫師所建立的企業股票。\n");
		}
		else if( SECURE_D->is_wizard(data["president"]) )
			return tell(me, "無法購買巫師建立的企業股票。\n");
		
		if( count(amount, ">", data["available_stockamount"]) )
			return tell(me, "目前可購買的張數只有 "+NUMBER_D->number_symbol(data["available_stockamount"])+" 張。\n");
			
		new_available_stockamount = count(data["available_stockamount"], "-", amount);

		cost = count(count(amount, "*", to_int(data["stockvalue"]*1000000)), "/", 100);
		cost_extra = count(cost, "/", PROCEDURE_FUND);

		old_amount = query("stock/"+enterprise+"/amount", me);
		new_amount = count(amount, "+", old_amount);
		
		cur_value = data["stockvalue"];
		old_value = to_float(query("stock/"+enterprise+"/value", me));
		new_value = to_float(count(count(count(to_int(old_value*1000000), "*", old_amount), "+", count(to_int(cur_value*1000000), "*", amount)), "/", new_amount))/1000000.;
		
		percent = to_float(count(count(new_amount, "*", 10000), "/", data["stockamount"]))/100.;
		
		if( !wizardp(me) && percent > 20 )
			return tell(me, "個人的持股比例不得超過 20%。\n");
		
		if( sizeof(query("stock", me)) >= MAX_OWN_STOCK_TYPE )
		{
			if( undefinedp(query("stock/"+enterprise, me)) )
				return tell(me, "同時持股種類不得超過 "+MAX_OWN_STOCK_TYPE+" 種。\n");
		}
		tell(me,
			HIY"股票目前股價  "NOR YEL+sprintf("%.2f", cur_value)+"\n"
			HIY"持股買入價格  "NOR YEL+sprintf("%.2f", old_value)+"\n"
			HIY"平均之後價格  "NOR YEL+sprintf("%.2f", new_value)+"\n"
			HIY"欲購股票張數  "NOR YEL+NUMBER_D->number_symbol(amount)+"\n"
			HIY"欲購股票總值  "NOR YEL+money(MONEY_D->query_default_money_unit(), cost)+"\n"
			HIY"交易手續費用  "NOR YEL+money(MONEY_D->query_default_money_unit(), cost_extra)+" (2%)\n"
			HIY"購入總共花費  "NOR YEL+money(MONEY_D->query_default_money_unit(), count(cost, "+", cost_extra))+"\n"
			HIY"持股比例升為  "NOR YEL+sprintf("%.2f", percent)+"%\n"
			HIY"是否確定購入股票?"NOR YEL"(Yes/No)"NOR
		);

		input_to((: confirm_buystock, me, enterprise, cost, cost_extra, new_amount, new_value, cur_value, amount, new_available_stockamount, percent :));
			
		return;
	}
	
	tell(me, "並沒有這個編號的股票。\n");
}

void confirm_sellstock(object me, string enterprise, string earn, string earn_extra, string new_amount, float old_value, float cur_value, int amount, string new_available_stockamount, float percent, string arg)
{
	if( !arg )
		return me->finish_input();
		
	arg = lower_case(arg);
	
	if( arg != "y" && arg != "yes" )
	{
		tell(me, "取消販賣股票。\n");
		return me->finish_input();
	}
	
	me->earn_money(MONEY_D->query_default_money_unit(), count(earn, "-", earn_extra));
	
	if( count(new_amount, "<=", 0) )
		delete("stock/"+enterprise, me);
	else
		set("stock/"+enterprise+"/amount", new_amount, me);
		
	ENTERPRISE_D->set_enterprise_info(enterprise, "available_stockamount", new_available_stockamount);
	CHANNEL_D->channel_broadcast("stock", me->query_idname()+"以 "+sprintf("%.2f", cur_value)+" 股價售出"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"股票 "+NUMBER_D->number_symbol(amount)+" 張，持股比例降為 "+sprintf("%.2f", percent)+"%");	
	me->finish_input();
	
	TOP_D->update_top_stock(me);
	me->save();
}

void do_sellstock(object me, string arg)
{
	int i, num, amount;
	string old_amount;
	string new_amount;
	float cur_value;
	float old_value;
	string earn;
	string earn_extra;
	float percent;
	string enterprise;
	mapping data;
	string new_available_stockamount;

	if( !arg || sscanf(arg, "%d %d", num, amount) != 2 )
		return tell(me, "請輸入正確的格式。\n");

	if( num <= 0 || amount <= 0 )
		return tell(me, "請輸入大於 0 的數字。\n");
	
	i=0;
	foreach(enterprise in ENTERPRISE_D->query_all_enterprises())
	{
		if( ++i != num ) continue;
		
		data = ENTERPRISE_D->query_enterprise_info(enterprise);
		
		old_amount = copy(query("stock/"+enterprise+"/amount", me));
		new_amount = count(old_amount, "-", amount);
		
		if( count(amount, ">", old_amount) )
			return tell(me, pnoun(2, me)+"目前擁有"+data["color_id"]+"的股票張數只有 "+NUMBER_D->number_symbol(old_amount)+" 張。\n");
			
		new_available_stockamount = count(data["available_stockamount"], "+", amount);
		
		cur_value = data["stockvalue"];
		old_value = to_float(query("stock/"+enterprise+"/value", me));
		earn = count(count(amount, "*", to_int(cur_value*1000000)), "/", 100);
		earn_extra = count(earn, "/", PROCEDURE_FUND);
		
		percent = to_float(count(count(new_amount, "*", 10000), "/", data["stockamount"]))/100.;

		tell(me,
			HIY"股票目前股價  "NOR YEL+sprintf("%.2f", cur_value)+"\n"
			HIY"持股平均價格  "NOR YEL+sprintf("%.2f", old_value)+"\n"
			HIY"欲售股票張數  "NOR YEL+NUMBER_D->number_symbol(amount)+"\n"
			HIY"欲售股票總值  "NOR YEL+money(MONEY_D->query_default_money_unit(), earn)+"\n"
			HIY"交易手續費用  "NOR YEL+money(MONEY_D->query_default_money_unit(), earn_extra)+" (2%)\n"
			HIY"售出總共獲得  "NOR YEL+money(MONEY_D->query_default_money_unit(), count(earn, "-", earn_extra))+"\n"
			HIY"持股比例降為  "NOR YEL+sprintf("%.2f", percent)+"%\n"
			HIY"是否確定售出股票?"NOR YEL"(Yes/No)"NOR
		);
		
		//tell(me, "是否確定以 "+sprintf("%.2f", cur_value)+" 股價售出"+data["color_id"]+"股票 "+NUMBER_D->number_symbol(amount)+" 張\n得金 "+money(MONEY_D->query_default_money_unit(), earn)+"，持股比例將為 "+sprintf("%.2f", percent)+"%(Yes/No)：");
		
		input_to((: confirm_sellstock, me, enterprise, earn, earn_extra, new_amount, old_value, cur_value, amount, new_available_stockamount, percent :));
			
		return;
	}
	
	tell(me, "並沒有這個編號的股票。\n");
}


// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"hall"	:
	([
		"short"	: NOR HIW"交易所大廳"NOR,
		"help"	:
			([
"topics":
@HELP
    證券交易的地方。
HELP,

"list":
@HELP
顯示股市資訊
list		列出所有股市資料
HELP,

"buy":
@HELP
買入股票的指令
buy 2 300	買入第 2 個股票 300 張
HELP,

"sell":
@HELP
賣出股票的指令
sell 3 100	賣出第 3 個股票 100 張
HELP,
			]),
		"heartbeat":0,	// 實際時間 1 秒為單位
		"job": 0,
		"action":
			([
				"list" : (: do_list :),
				"buy" : (: do_buystock :),
				"sell" : (: do_sellstock :),
			]),
	]),
]);


// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIG"證券"NOR GRN"交易所"NOR

	// 開張此建築物之最少房間限制
	,2

	// 城市中最大相同建築數量限制(0 代表不限制)
	,5

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,ENTERPRISE

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,COMMERCE_REGION

	// 開張儀式費用
	,"75000000"
	
	// 建築物關閉測試標記
	,0

	// 繁榮貢獻度
	,0
	
	// 最高可加蓋樓層
	,1
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,1
});
