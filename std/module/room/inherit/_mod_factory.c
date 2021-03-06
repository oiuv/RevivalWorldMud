/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _factory_action_mod.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-18
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <npc.h>
#include <ansi.h>
#include <message.h>
#include <daemon.h>
#include <material.h>
#include <location.h>
#include <skill.h>
#include <citydata.h>

inherit __DIR__"_object_database_mod.c";

#define PRODUCT_MODULE_PATH		"/std/module/product/"

#define MAINTAIN_STR_COST	300

void input_object(object env, string database, string basename, mixed amount);
void output_object(object env, string database, string basename, mixed amount);
varargs string query_module_basename(object env, string database, string module, string shelflimit);
varargs string query_object_amount(object env, string database, string basename, string shelflimit);

// 取得此工廠可以生產的產品 module 物件陣列
private string *query_available_products(object env)
{
	string file;
	string *types;
	object module;
	mapping product_info;
	string building_type;
	
	types = allocate(0);
	
	building_type = env->query_building_type();
	
	foreach(file in get_dir(PRODUCT_MODULE_PATH))
	{
		catch( module = load_object(PRODUCT_MODULE_PATH + file +"/module") );
		
		if( !objectp(module) ) continue;
		
		product_info = module->query_product_info();	
		
		if( product_info["factory"] == building_type )
			types += ({ file });
	}
	
	return types;
}

void do_line_action(object me, string arg)
{
	int productnum;
	object product;
	object env = environment(me);
	object master = env->query_master();
	object research = load_module(query("research", master));
	object warehouse = load_module(query("warehouse", master));
	string *productslist;

	if( query("owner", master) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"不是這間工廠的擁有者。\n");

	if( !objectp(research) )
		return tell(me, "尚未設定任何研發中心。\n");
	
	if( !objectp(warehouse) )
		return tell(me, "尚未設定任何倉儲中心。\n");
	
	if( !arg || !arg[0] )
		return tell(me, pnoun(2, me)+"想要做什麼樣的生產動作？('產品編號', start, stop)\n");

	arg = lower_case(arg);

	productnum = to_int(arg);
	
	if( productnum > 0 )
	{
		if( query("line/working", master) )
			return tell(me, "生產線正在運作中，必須先停止後才能更改生產產品種類。\n");
		
		productslist = query("productslist", research);
		
		if( productnum > sizeof(productslist) )
			return tell(me, "沒有編號 "+productnum+" 的產品。\n");
		
		catch( product = load_object(productslist[productnum-1]) );
		
		if( !objectp(product) )
			return tell(me, "無法載入 "+productslist[productnum-1]+" 這個產品！請通知巫師處理。\n");
	
		if( member_array(product->query_module(), query_available_products(master)) == -1 )
			return tell(me, "這間工廠無法生產這種產品。\n");
		
		set("product", base_name(product), master);
		master->save();	
		msg("$ME將此生產線的製造產品設定為「"+product->query_idname()+"」。\n", me, 0, 1);
	}
	else if( arg == "start" )
	{
		if( !query("product", master) )
			return tell(me, "尚未設定欲製造的產品，無法開始生產。\n");

		catch( product = load_object(query("product", master)) );
		
		if( !objectp(product) )
			return tell(me, "無法載入 "+query("product", master)+" 這個產品！請通知巫師處理。\n");

		set("line/working", 1, master);
		master->save();
		msg("$ME啟動了生產線上所有的設備，開始生產"+product->query_idname()+"。\n", me, 0, 1);
		
	}
	else if( arg == "stop" )
	{
		delete("line", master);
		master->save();
		msg("$ME將生產線上所有的設備關機，停止生產產品。\n", me, 0, 1);
	}

	else tell(me, "請輸入正確的指令格式。(help line)\n", me, 0, 1);
}

void do_list_action(object me, string arg)
{
	int i;
	string file, msg;
	object env = environment(me);
	object master = env->query_master();
	object research = load_module(query("research", master));
	object product;
	string *productslist;
	
	if( !objectp(research) )
		return tell(me, "尚未設定任何研發中心。\n");
	
	productslist = query("productslist", research);
	
	if( !sizeof(productslist) )
		return tell(me, "這個研發中心尚未設計出任何可以生產的產品。\n");
	
	msg = research->query_room_name()+"目前已經研發完成的產品如下：\n"WHT"────────────────────────────────\n"NOR;
	
	i = 0;
	foreach(file in productslist)
	{
		catch(product = load_object(file));
	
		if( !objectp(product) ) continue;
			
		msg += sprintf("%-4s %s\n", (++i)+".", product->query_idname());
	}
	
	msg += WHT"────────────────────────────────\n"NOR;
	
	me->more(msg);
}

void do_setup_action(object me, string arg)
{
	string value;
	int x, y, num;
	object research;
	object warehouse;
	string file;
	string city;
	object env = environment(me);
	object master = env->query_master();
	array loc = env->query_location();

	city = loc[CITY];

	if( !arg )
		return tell(me, pnoun(2, me)+"想要設定什麼項目？(help setup)\n");
	
	if( query("owner", master) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"不是這個工廠的擁有者。\n");

	sscanf(arg, "%s %s", arg, value);

	switch(arg)
	{
		case "research":
		{	
			if( value == "-d" )
			{
				if( !query("research", master) )
					return tell(me, "這裡原本便沒有設定研發中心。\n");
					
				delete("research", master);
				
				master->save();
				
				return tell(me, "刪除研發中心設定。\n");
			}

			if( !value || sscanf(value, "%d %d,%d", num, x, y) != 3 )
				return tell(me, "請輸入正確的研究中心座標(例 1 21,33)。\n");
			
			file = CITY_ROOM_MODULE(city, (num-1), (x-1), (y-1), "research");
			
			if( !objectp(research = load_module(file)) || query("function", research) != "laboratory" )
				return tell(me, "座標"+loc_short(city, num-1, x-1, y-1)+"並沒有研發中心。\n");

			if( research != research->query_master() )
			{
				file = base_name(research->query_master());
				research = research->query_master();
			}
			
			if( query("owner", research) != me->query_id(1) )
				return tell(me, research->query_room_name()+"並不屬於"+pnoun(2, me)+"。\n");
			
			set("research", file, master);
			
			master->save();
			
			msg("$ME設定"+research->query_room_name()+"為此生產線的研發中心。\n", me, 0, 1);
			
			break;
		}
		
		case "warehouse":
		{
			if( value == "-d" )
			{
				if( !query("warehouse", master) )
					return tell(me, "這裡原本便沒有設定倉儲中心。\n");
					
				delete("delivery", master);
				return tell(me, "取消存貨目標完畢。\n", CMDMSG);
			}

			if( !value || sscanf(value, "%d %d,%d", num, x, y) != 3 )
				return tell(me, "請輸入正確的倉庫座標(例 1 21,33)。\n");
			
			// 交貨至倉庫
			file = CITY_ROOM_MODULE(city, (num-1), (x-1), (y-1), "warehouse");

			if( !objectp(warehouse = load_module(file)) || query("function", warehouse) != "warehouse" )
				return tell(me, "座標"+loc_short(city, num-1, x-1, y-1)+"並沒有倉庫。\n");
			
			if( warehouse != warehouse->query_master() )
			{
				file = base_name(warehouse->query_master());
				warehouse = warehouse->query_master();
			}

			if( query("owner", warehouse) != me->query_id(1) )
				return tell(me, warehouse->query_room_name()+"並不屬於"+pnoun(2, me)+"。\n");
			
			set("warehouse", file, master);
			
			master->save();
			
			msg("$ME設定"+warehouse->query_room_name()+"為此生產線的倉儲中心。\n", me, 0, 1);
			
			break;
		}

		default:
			return tell(me, "請輸入正確的選項(help setup)。\n");
	}
}

// 30 秒呼叫一次
void heart_beat(object room)
{
	int amount;
	int userinside;
	int timecost;
	int management, factory_heart_beat;
	int not_enough_material;
	string *slaves = room->query_slave() || allocate(0);
	object research = load_module(query("research", room));
	object warehouse = load_module(query("warehouse", room));
	object *workers = filter(all_inventory(room), (: $1->is_module_npc() && query("job/cur", $1) == WORKER :));
	object worker;
	object material_ob;
	mapping product_info;
	mapping material_info;
	object product;
	string product_file;
	string material_file;
	mapping warehouse_database;

	
	if( query("function", room) != "line" ) return;

	if( !(factory_heart_beat = query_temp("factory_heart_beat", room)) )
		factory_heart_beat = set_temp("factory_heart_beat", random(1200)+1, room);
	else
		addn_temp("factory_heart_beat", 1, room);
	
	//兩小時存一次
	if( !(factory_heart_beat%240) )
	{
		room->save();
		workers->save();
		
		if( objectp(research) )
			research->save();
		
		if( objectp(warehouse) )
			warehouse->save();
	}

	if( !query("line/working", room) )
		return;

	if( !objectp(research) || !objectp(warehouse) || query("function", research) != "laboratory" || query("function", warehouse) != "warehouse" )
	{
		broadcast(room, "工廠缺少研發中心或倉儲中心的支援，導致生產停止。\n");
		delete("line", room);
		room->save();
		return;
	}
	
	foreach(worker in workers)
	{
		management += worker->query_skill_level("factorymanage")/5;
	}

	set("management", management, room);
	
	if( management < sizeof(slaves)+1 )
	{
		broadcast(room, "由於員工缺乏導致工廠管理能力不足，導致生產停止。\n");
		delete("line", room);
		room->save();
		return;
	}
	
	userinside = sizeof(filter_array(all_inventory(room), (: userp($1) :)));
	
	product_file = query("product", room);
	
	catch( product = load_object(product_file) );
	
	product_info = product->query_product_info();
	
	timecost = addn("line/timecost", 30 * (sizeof(slaves)+1), room);
	
	// 產能足夠
	if( timecost > product_info["timecost"] )
	{
		amount = timecost / product_info["timecost"];
		
		// 儲存剩餘產能
		set("line/timecost", timecost % product_info["timecost"], room);
		
		
		warehouse_database = query("products", warehouse);
		
		if( !mapp(warehouse_database) )
		{
			if( userinside )
				broadcast(room, "倉庫中無任何物品。\n");

			delete("line/timecost", room);
			return;
		}
		
		material_info = allocate_mapping(0);

		// 檢查原料是否足夠
		foreach( string basename_or_module, int need in product_info["material"] )
		{
			material_file = query_module_basename(warehouse, "products", basename_or_module, "工廠原料") || basename_or_module;

			if( count(query_object_amount(warehouse, "products", material_file, "工廠原料"), "<", need * amount) )
			{
				if( userinside )
				{
					if( objectp(material_ob = load_object(material_file)) )
						broadcast(room, "倉庫缺乏「"+material_ob->query_idname()+"」原料。\n");
					else if( objectp(material_ob = load_object("/std/module/product/"+material_file+"/module.c")) )
					{
						product_info = material_ob->query_product_info();
						broadcast(room, "倉庫缺乏「"+product_info["name"]+"("+capitalize(product_info["id"])+")"+"」原料。\n");
					}
					else
						broadcast(room, "倉庫缺乏「"+material_file+"」原料。\n");
				}

				delete("line/timecost", room);
				not_enough_material = 1;
			}
			
			material_info[material_file] = need;
		}
		
		if( not_enough_material ) return;
		
		foreach( string material, int need in material_info )
		{
			output_object(warehouse, "products", material, need * amount);
			
			if( userinside )
				broadcast(room, "工廠製造消耗了 "+(need * amount)+" 個"+load_object(material)->query_idname()+"。\n");
		}

		if( userinside )
			broadcast(room, "工廠製造生產出 "+amount+" 個"+product->query_idname()+"。\n");
			
		input_object(warehouse, "products", product_file, amount);
	}
}

string look_room(object room)
{
	if( query("function", room) == "line" )
	{
		string msg = "";
		object master = room->query_master();
		
		object research = load_module(query("research", master));
		object warehouse = load_module(query("warehouse", master));
		object product = load_module(query("product", master));
	
		msg += "產線運作："+(query("line/working", master) ? HIG"生產中"NOR : HIR"停止中"NOR)+"\n";
		msg += "製造產品："+(objectp(product) ? product->query_idname() : "無")+"\n";
		msg += "研發中心："+(objectp(research) ? research->query_room_name()+" "HIC"科技值 "NOR CYN+query("technology", research)+NOR : "無")+"\n";
		msg += "倉儲中心："+(objectp(warehouse) ? warehouse->query_room_name() : "無")+"\n";
		msg += "工廠管理："HIY+(sizeof(master->query_slave())+1)+"/"+query("management", master)+NOR"\n";
		return msg;
	}
}
