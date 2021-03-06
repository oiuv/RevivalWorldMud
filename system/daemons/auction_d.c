/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : auction_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-11
 * Note   : 拍賣精靈
 * Update :
 *  o 2002-09-26 
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <money.h>

#define DATA		"/data/daemon/auction.o"

#define STORE_CAPACITY		100000
#define WAREHOUSE_CAPACITY	-1

array auction;


int save()
{
	save_object(DATA);
}

array query_auction()
{
	return auction;	
}

void add_auction(mapping data)
{	
	auction += ({ data });
	
	save();
}

void remove_auction(int i)
{
	auction = auction[0..i-1] + auction[i+1..];
	
	save();
}

void refresh_top_buyer(int i)
{
	string buyer, top_buyer;
	string price, top_price;

	if( i < 0 || i >= sizeof(auction) ) return;

	if( mapp(auction[i]["auction_data"]) )
	foreach(buyer, price in auction[i]["auction_data"])
	{
		if( count(price, ">", top_price) )
		{
			top_price = price;
			top_buyer = buyer;
		}
	}
	
	auction[i]["top_buyer"] = top_buyer;
	auction[i]["top_price"] = top_price;
}

void refresh_all_top_buyer()
{
	for(int i=0;i<sizeof(auction);i++)
		refresh_top_buyer(i);
}

void do_list(object me, string arg)
{
	int i;
	string msg;
	mapping data;
	object ob;
	string money_unit = MONEY_D->query_default_money_unit();
	string top_price;
	string top_buyer;
	string id;
	string status;
	
	if( !arg )
	{
		msg = HIB"目前全世界正在拍賣的商品如下：\n"NOR;
		msg += WHT"─────────────────────────────────────────\n"NOR;
		msg +=    "編號 拍賣品                                  數量    目前最高出價 出價者  剩餘時間\n"NOR;
		msg += WHT"─────────────────────────────────────────\n"NOR;
	
		foreach(data in auction)
		{
			ob = load_object(data["basename"]);
			
			if( !objectp(ob) ) continue;
			
			top_price = data["top_price"];
			top_buyer = data["top_buyer"];
		
			if( data["status"] == 1 )
				status = HIR"結標"NOR;
			else if( data["status"] == 2 )
				status = HIG"流標"NOR;
			else if( data["status"] == 3 )
				status = HIY"售出"NOR;
			else
				status = 0;

			msg += sprintf("%-5d%s%16s %-12s%4s\n",
				++i,
				ob->query_idname()+repeat_string(" ", 44-noansi_strlen(ob->query_idname()+NUMBER_D->number_symbol(data["amount"])))+HIG+NUMBER_D->number_symbol(data["amount"])+NOR,
				top_price ? HIY+NUMBER_D->number_symbol(top_price)+NOR : NOR YEL+NUMBER_D->number_symbol(data["base_price"])+NOR,
				top_buyer ? HIW+capitalize(top_buyer)+NOR : WHT+capitalize(data["seller"])+NOR,
				status || ((data["minute"]>=60)?HIM+(data["minute"]/60)+"h"+NOR:NOR BLINK MAG+data["minute"]+"m"NOR),
			);
		}
		
		msg += WHT"─────────────────────────────────────────\n"NOR;
		msg += "list '編號' 觀看該拍賣品的詳細拍賣資料\n\n";
		
		return me->more(msg);
	}
	
	i = to_int(arg);
	
	if( i < 1 || i > sizeof(auction) )
		return tell(me, "請輸入正確的編號。\n");
		
	data = auction[i-1];
	
	ob = load_object(data["basename"]);
	
	if( !objectp(ob) )
		error(data["basename"]+" 物件載入失敗");
	
	if( data["status"] == 1 )
		status = HIR"結標"NOR;
	else if( data["status"] == 2 )
		status = HIG"流標"NOR;
	else if( data["status"] == 3 )
		status = HIY"售出"NOR;
	else
		status = 0;
				
	msg =  "編號 "+i+" 之拍賣品資料如下：\n";
	msg += "──────────────────────────────────\n";
	msg += "拍賣者    ："+capitalize(data["seller"])+"\n";
	msg += "拍賣品    ："+ob->query_idname()+"\n";
	msg += "數量      ："+NUMBER_D->number_symbol(data["amount"])+"\n";
	msg += "拍賣底價  ：$"+MONEY_D->query_default_money_unit()+" "+NUMBER_D->number_symbol(data["base_price"])+"\n";
	msg += "直接購買價：$"+MONEY_D->query_default_money_unit()+" "+NUMBER_D->number_symbol(data["direct_price"])+"\n";
	msg += "剩餘時間  ："+(status || (data["minute"]+" 分鐘"))+"\n";
	
	if( data["finish_time"] > 0 )
	{
		if( data["status"] == 2 )
			msg += "系統回收  ：此拍賣品將於 "+TIME_D->replace_ctime(data["finish_time"]+14*24*60*60)+" 由系統自動刪除\n";
		else
			msg += "拍賣者領回：得標者未於 "+TIME_D->replace_ctime(data["finish_time"]+7*24*60*60)+" 之前領取拍賣品者，將開放拍賣者領回。\n";
	}

	msg += "競標狀況  ：\n";
	
	if( !sizeof(data["auction_data"]) )
		msg += "    無人出價\n";
	else foreach(id in sort_array(keys(data["auction_data"]), (: count( $(data)["auction_data"][$1], "<", $(data)["auction_data"][$2] ) ? 1 : -1 :)))
		msg += sprintf("    %-16s 目前出價 %s\n", capitalize(id), money(money_unit, data["auction_data"][id]));
	
	msg += "──────────────────────────────────\n";
	
	tell(me, msg);
}

void confirm_bid(object me, int i, object ob, string price, string money_unit, mapping data, string arg)
{
	int num;
	string top_price;
	string top_buyer;
	string next_price;
	object seller;

	if( arg != "yes" && arg != "y" )
	{
		tell(me, pnoun(2, me)+"決定不競標這個拍賣品。\n");
		return me->finish_input();
	}

	num = i+1;

	// 資料不同
	if( !mapp(auction[i]) || 
		auction[i]["status"] != data["status"] ||
		auction[i]["basename"] != data["basename"] ||
		auction[i]["amount"] != data["amount"] ||
		auction[i]["seller"] != data["seller"] ||
		auction[i]["base_price"] != data["base_price"]
	)
	{
		tell(me, "拍賣品編號順序已經改變，請重新競標。\n");
		return me->finish_input();
	}

	if( auction[i]["status"] == 1 )
	{
		tell(me, "這項拍賣品已經結標。\n");
		return me->finish_input();
	}
	else if( auction[i]["status"] == 2 )
	{
		tell(me, "這項拍賣品已經流標。\n");
		return me->finish_input();
	}
	else if( auction[i]["status"] == 3 )
	{
		tell(me, "這項拍賣品已經被直接購買。\n");
		return me->finish_input();
	}

	if( !sizeof(auction[i]["auction_data"]) )
	{
		if( count(price, "<", auction[i]["base_price"]) )
		{
			tell(me, "競價價錢必須高於拍賣底價("+money(money_unit, auction[i]["base_price"])+")。\n");
			return me->finish_input();
		}

		if( !me->spend_money(money_unit, price, BANK_PAYMENT) )
		{
			tell(me, pnoun(2, me)+"的錢不足 "+money(money_unit, price)+"。\n");
			return me->finish_input();
		}

		auction[i]["auction_data"] = ([ me->query_id(1) : price ]);

		refresh_top_buyer(i);

		if( count(price, ">=", auction[i]["direct_price"]) )
		{
			auction[i]["status"] = 3;
			auction[i]["minute"] = 0;
			auction[i]["finish_time"] = time();

			MONEY_D->earn_money(auction[i]["seller"], money_unit, price);
				
			CHANNEL_D->channel_broadcast("auc", HIY"直接購買："NOR+me->query_idname()+"對第 "+num+" 編號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」喊出 "HIY+money(money_unit, price)+NOR" 的直接購買價，拍賣結標。\n");	

			if( objectp(seller = find_player(auction[i]["seller"])) )
				tell(seller, me->query_idname()+"將 "HIY+money(money_unit, price)+NOR" 繳付給"+pnoun(2, seller)+"。\n");
			
			me->finish_input();
			return;	
		}

		CHANNEL_D->channel_broadcast("auc", HIC"出價："NOR+me->query_idname()+"對第 "+num+" 編號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」喊出 "HIY+money(money_unit, price)+NOR" 的競標價。\n");
		
		save();

		return me->finish_input();
	}
	else
	{
		top_buyer = auction[i]["top_buyer"];
		top_price = auction[i]["top_price"];

		// 直接購買
		if( count(price, ">=", auction[i]["direct_price"]) )
		{
			if( !me->spend_money(money_unit, price, BANK_PAYMENT) )
			{
				tell(me, pnoun(2, me)+"的錢不足 "+money(money_unit, price)+"。\n");
				return me->finish_input();
			}

			// 將上一位競標者的錢歸還
			if( top_buyer )
				MONEY_D->earn_money(top_buyer, money_unit, top_price);

			auction[i]["status"] = 3;
			auction[i]["minute"] = 0;
			auction[i]["auction_data"][me->query_id(1)] = price;
			auction[i]["finish_time"] = time();

			refresh_top_buyer(i);
			
			MONEY_D->earn_money(auction[i]["seller"], money_unit, price);

			CHANNEL_D->channel_broadcast("auc", HIY"直接購買："NOR+me->query_idname()+"對第 "+num+" 編號的拍賣品「"+ob->query_idname()+"」喊出 "HIY+money(money_unit, price)+NOR" 的直接購買價，拍賣結標。\n");	
			
			if( objectp(seller = find_player(auction[i]["seller"])) )
				tell(seller, me->query_idname()+"將 "HIY+money(money_unit, price)+NOR" 繳付給"+pnoun(2, seller)+"。\n");

			save();

			return me->finish_input();
		}
		else
		{
			next_price = count(top_price, "+", count(top_price, "/", 20));
	
			if( count(next_price, "==", top_price) )
				next_price = count(next_price, "+", 1);
	
			if( count(price, "<", next_price) )
			{
				tell(me, "每次競標的差距必須是目前最高競標價 + 5% 的底價以上(系統計算結果為："+money(money_unit, next_price)+")\n");
				return me->finish_input();
			}
			
			if( count(price, ">", count(top_price, "*", 5)) )
			{
				tell(me, "競標價不得高於現有最高出價的 5 倍。\n");
				return me->finish_input();
			}
	
			if( !me->spend_money(money_unit, price, BANK_PAYMENT) )
			{	
				tell(me, pnoun(2, me)+"的錢不足 "+money(money_unit, price)+"。\n");
				return me->finish_input();
			}

			// 將上一位競標者的錢歸還
			if( top_buyer )
				MONEY_D->earn_money(top_buyer, money_unit, top_price);

			if( count(price, ">=", auction[i]["direct_price"]) )
			{
				auction[i]["status"] = 3;
				auction[i]["minute"] = 0;
				auction[i]["auction_data"][me->query_id(1)] = price;
				auction[i]["finish_time"] = time();
	
				refresh_top_buyer(i);
				
				MONEY_D->earn_money(auction[i]["seller"], money_unit, price);
				
				CHANNEL_D->channel_broadcast("auc", HIY"直接購買："NOR+me->query_idname()+"對第 "+num+" 編號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」喊出 "HIY+money(money_unit, price)+NOR" 的直接購買價，拍賣結標。\n");	
				
				if( objectp(seller = find_player(auction[i]["seller"])) )
					tell(seller, me->query_idname()+"將 "HIY+money(money_unit, price)+NOR" 繳付給"+pnoun(2, seller)+"。\n");

				return me->finish_input();
			}
	
			auction[i]["auction_data"][me->query_id(1)] = price;			
			
			refresh_top_buyer(i);

			CHANNEL_D->channel_broadcast("auc", HIC"出價："NOR+me->query_idname()+"對第 "+num+" 編號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」喊出 "HIY+money(money_unit, price)+NOR" 的競標價。\n");	
			
			save();

			return me->finish_input();
		}
	}
	
	return me->finish_input();
}

void do_bid(object me, string arg)
{
	int i, num;
	string price;
	string money_unit = MONEY_D->query_default_money_unit();
	object ob;

	if( !arg || sscanf(arg, "%d %s", num, price) != 2 )
		return tell(me, "請輸入正確的競標資料(help bid)。\n");
		
	if( num < 1 || num > sizeof(auction) )
		return tell(me, "請輸入正確的編號。\n");

	i = num - 1;
	
	if( auction[i]["status"] == 1 )
		return tell(me, "這項拍賣品已經結標。\n");
	else if( auction[i]["status"] == 2 )
		return tell(me, "這項拍賣品已經流標。\n");
	else if( auction[i]["status"] == 3 )
		return tell(me, "這項拍賣品已經被直接購買。\n");

	ob = load_object(auction[i]["basename"]);
	
	if( !objectp(ob) )
		error(auction[i]["basename"]+" 物件載入錯誤。\n");

	if( price == "cancel" )
	{
		if( auction[i]["seller"] != me->query_id(1) )
			return tell(me, "只有原拍賣者才能取消拍賣。\n");

		if( auction[i]["status"] )
			return tell(me, "此拍賣品已經結標或流標或被直接購買了，無法再取消拍賣。\n");

		if( sizeof(auction[i]["auction_data"]) )
			return tell(me, "已經有人參與競標，無法再取消拍賣了。\n");

		auction[i]["status"] = 2;
		auction[i]["minute"] = 0;
		auction[i]["finish_time"] = time();
		auction[i]["auction_data"] = allocate_mapping(0);
		auction[i]["top_buyer"] = 0;
		auction[i]["top_price"] = 0;

		CHANNEL_D->channel_broadcast("auc", HIG"取消拍賣："NOR+me->query_idname()+"取消第 "+num+" 編號拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」的拍賣。\n");
		
		return;
	}

	if( me->query_id(1) == auction[i]["seller"] )
		return tell(me, "拍賣者不能參與競標。\n");

	if( price == "auto" )
	{
		if( !sizeof(auction[i]["auction_data"]) )
			price = auction[i]["base_price"];
		else if( count(count(auction[i]["top_price"], "/", 20), ">", 0) )
			price = count(auction[i]["top_price"], "+", count(auction[i]["top_price"], "/", 20));
		else
			price = count(auction[i]["top_price"], "+", 1);
		
		if( count(price, ">", auction[i]["direct_price"]) )
			price = auction[i]["direct_price"];
	}
	else
		price = big_number_check(price);
 	
	if( count(price, "<", 1) || count(price, ">", "1000000000000") )
		return tell(me, "請輸入正確的競標價(1-1000000000000)。\n");	

	tell(me, "是否確定以 "HIY+money(money_unit, price)+NOR" 的價格競標「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」(y/n)？");
	
	input_to( (: confirm_bid, me, i, ob, price, money_unit, copy(auction[i]) :) );
}

void do_receive(object me, string arg)
{
	int i;
	int num;
	int x;
	int y;
	object env = environment(me);
	string city = env->query_city();
	array loc;
	object delivery;
	object delivery_master;
	int capacity;
	object ob;
	object cloneob;

	if( !arg )
		return tell(me, "請輸入正確的指令(help receive)。\n");

	// 運輸到某個地點
	if( sscanf(arg, "%d to %d %d,%d", i, num, x, y) == 4 )
	{
		if( i < 1 || i > sizeof(auction) )
			return tell(me, "請輸入正確的編號。\n");

		i--;

		if( !auction[i]["status"] )
			return tell(me, "此項拍賣品還在競標程序中。\n");

		if( auction[i]["status"] == 2 && me->query_id(1) != auction[i]["seller"] )
			return tell(me, "流標物品必須由拍賣者領回。\n");

		ob = load_object(auction[i]["basename"]);
		
		if( !objectp(ob) )
			error(auction[i]["basename"]+" 資料載入錯誤。\n");

		if( auction[i]["status"] != 2 && auction[i]["top_buyer"] != me->query_id(1) )
			return tell(me, pnoun(2, me)+"並沒有得標這個拍賣品。\n");

		if( !CITY_D->city_exist(city, num-1) )
			return tell(me, "這座城市並沒有第 "+num+" 衛星都市。\n");
	
		loc = arrange_city_location(x-1, y-1, city, num-1);

		delivery = load_module(loc);
		
		if( !objectp(delivery) )
			return tell(me, "座標"+loc_short(loc)+"處並沒有任何建築物。\n");
			
		if( query("owner", delivery) != me->query_id(1) )
			return tell(me, delivery->query_room_name()+"並不是"+pnoun(2, me)+"的建築物。\n");
		
		delivery_master = delivery->query_master();

		switch( delivery->query_building_type() )
		{
			case "store":
				capacity = STORE_CAPACITY;
				break;
			case "warehouse":
				capacity = WAREHOUSE_CAPACITY;
				break;
			default:
				return tell(me, delivery->query_room_name()+"沒有辦法接收任何商品。\n");
		}
		
		switch( query("function", delivery) )
		{
			case "front":
			case "warehouse":
				break;
			default:
				return tell(me, delivery->query_room_name()+"沒有辦法接收任何商品。\n");
		}
		
		if( delivery->query_module_file()->over_capacity(delivery_master, "products", auction[i]["amount"], capacity) )
			return tell(me, delivery->query_room_name()+"無法再容納這麼多的物品了。\n");

		if( auction[i]["status"] != 2 )
			msg("$ME將得標物品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」運輸到"+query("short", delivery)+loc_short(loc)+"。\n", me, 0, 1);	
		else
			msg("$ME將「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」領回並運輸到"+query("short", delivery)+loc_short(loc)+"。\n", me, 0, 1);
			
		delivery->query_module_file()->input_object(delivery_master, "products", auction[i]["basename"], auction[i]["amount"]);
		remove_auction(i);
		
		delivery_master->save();
	}
	else if( big_number_check(arg) && sscanf(arg, "%d", i) == 1 )
	{
		if( i < 1 || i > sizeof(auction) )
			return tell(me, "請輸入正確的編號。\n");
		
		i--;

		if( !auction[i]["status"] )
			return tell(me, "此項拍賣品還在競標程序中。\n");

		if( auction[i]["status"] == 2 && me->query_id(1) != auction[i]["seller"] )
			return tell(me, "流標物品必須由拍賣者領回。\n");
			
		
		ob = load_object(auction[i]["basename"]);
		
		if( !objectp(ob) )
			error(auction[i]["basename"]+" 資料載入錯誤。\n");
	
		if( auction[i]["status"] != 2 && auction[i]["top_buyer"] != me->query_id(1) )
			return tell(me, pnoun(2, me)+"並沒有得標這個拍賣品。\n");

		cloneob = new(auction[i]["basename"]);
		
		if( !me->available_get(cloneob, auction[i]["amount"]) )
		{
			destruct(cloneob);
			return tell(me, pnoun(2, me)+"拿不動這麼多東西。\n");
		}
			
		if( auction[i]["status"] != 2 )
			msg("$ME將得標物品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」領取到身上。\n", me, 0, 1);
		else
			msg("$ME將「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」領回到身上。\n", me, 0, 1);
				
		if( !query("flag/no_amount", cloneob) )
		{
			set_temp("amount" , auction[i]["amount"], cloneob);
			
			cloneob->move(me);
		}
		else
			cloneob->move(me, auction[i]["amount"]);
		

		remove_auction(i);
	}
	else
		return tell(me, "請輸入正確的收貨指令(help receive)。\n");
}

void heart_beat()
{
	int size = sizeof(auction);
	object ob;
	object seller;
	string money_unit = MONEY_D->query_default_money_unit();

	for(int i=0;i<size;i++)
	{
		// 超過 14 天未領取的物品由系統刪除
		if( auction[i]["finish_time"] > 0 && time() > auction[i]["finish_time"] + 14*24*60*60 )
		{
			auction[i] = 0;
		}
		// 超過 7 天未領取的物品宣告流標
		else if( (auction[i]["status"] == 1 || auction[i]["status"] == 3) && auction[i]["finish_time"] > 0 && time() > auction[i]["finish_time"] + 7*24*60*60 )
		{
			auction[i]["status"] = 2;
			auction[i]["finish_time"] = time();
			
			if( objectp(seller = find_player(auction[i]["seller"])) )
				tell(seller, pnoun(2, seller)+"的拍賣品超過 7 天無人領取，開放拍賣者領回。\n");
		}
		else if( auction[i]["minute"] && !(--auction[i]["minute"]) )
		{
			ob = load_object(auction[i]["basename"]);
			
			if( !objectp(ob) ) continue;

			if( !sizeof(auction[i]["auction_data"]) )
			{
				auction[i]["status"] = 2;
				auction[i]["finish_time"] = time();

				CHANNEL_D->channel_broadcast("auc", HIG"流標："NOR+"編號 "+(i+1)+" 號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」宣告流標。\n");
				
				if( objectp(seller = find_player(auction[i]["seller"])) )
					tell(seller, pnoun(2, seller)+"的拍賣品已經流標，請自行領回。\n");
			}
			else
			{
				refresh_top_buyer(i);
				
				MONEY_D->earn_money(auction[i]["seller"], money_unit, auction[i]["top_price"]);

				auction[i]["status"] = 1;
				auction[i]["finish_time"] = time();

				CHANNEL_D->channel_broadcast("auc", HIR"結標："NOR+"編號 "+(i+1)+" 號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」由 "+capitalize(auction[i]["top_buyer"])+" 以 "HIY+money(money_unit, auction[i]["top_price"])+NOR" 價格標得。\n");
				
				if( objectp(seller = find_player(auction[i]["seller"])) )
					tell(seller, pnoun(2, seller)+"的拍賣品成功結標，收取 "HIY+money(money_unit, auction[i]["top_price"])+NOR"。\n");
			}
		}
	}
	
	auction -= ({ 0 });
}


int remove()
{
	return save();
}

void fix_auction()
{
	int i;
	for(i=0;i<sizeof(auction);i++)
	{
		if( mapp(auction[i]["auction_data"]) )
		foreach(string id, string price in auction[i]["auction_data"])
		{
			auction[i]["auction_data"][id] = replace_string(auction[i]["auction_data"][id], ",", "");	
		}
	}
}

void create()
{
	if( !restore_object(DATA) )
	{
		auction = allocate(0);

		save_object(DATA);
	}
	
	set_heart_beat(600);
}

string query_name()
{
	return "拍賣系統(AUCTION_D)";
}
