/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : collect.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-2-19
 * Note   : collect 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#define COLLECT_STR_COST	10

#define MATERIAL_PATH		"/obj/materials/"

#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <material.h>
#include <location.h>

inherit COMMAND;

string help = @HELP

採集指令

這個指令可在地圖上採集各種原料，原料共分五種。

metal	金屬 - 於山區使用十字鎬類工具採集
stone	原石 - 於山區使用鑽頭類工具採集
water	清水 - 於河流使用水桶類工具採集
wood	原木 - 於森林使用木鋸類工具採集
fuel	燃料 - 於森林使用鏟子類工具採集

指令格式:
collect metal		- 採集一個金屬(採集身上必須有十字鎬類工具)
collect 10 metal	- 一次採集十個金屬
			  可節省些許體力與減少工具損壞率並增加稀有物品獲取率
collect info		- 查詢當日該城市的原料採集資訊

相關指令: info

HELP;

// 一次採愈多省愈多力
private int cost_stamina(object me, int amount, int skilllevel)
{
	if( amount <= 10 )
		return me->cost_stamina(amount * (COLLECT_STR_COST - skilllevel/4));
	else if( amount <= 20 )
		return me->cost_stamina(amount * (COLLECT_STR_COST - skilllevel/4 - 1));
	else if( amount <= 50 )
		return me->cost_stamina(amount * (COLLECT_STR_COST - skilllevel/4 - 2));
	else if( amount <= 100 )
		return me->cost_stamina(amount * (COLLECT_STR_COST - skilllevel/4 - 3));

	return me->cost_stamina(amount * (COLLECT_STR_COST - skilllevel/4 - 4));
}

private void do_command(object me, string arg)
{
	array loc;
	object ob, env = environment(me);
	mapping coorrangetype;
	object *tools;
	int skilllevel, endurance, materialamount, amount;
	string sloc, arg2;
	string city;
	int num;
	string myid = me->query_id(1);

	if( !arg )
		return tell(me, pnoun(2, me)+"想要採集哪種資源？("HIW"金屬 metal"NOR"、"WHT"原石 stone"NOR"、"HIC"清水 water"NOR"、"YEL"原木 wood"NOR"、"HIM"燃料 fuel"NOR")\n");

	if( (wizardp(me) && sscanf(arg, "info %s", city) == 1) || arg == "info" )
	{
		string msg;
		string id;
		mapping collection_record;
		mapping data;

		if( !city )
			city = query("city", me);

		if( !city )
			return tell(me, pnoun(2, me)+"不屬於任何一座城市。\n");

		if( !CITY_D->city_exist(city) )
			return tell(me, "沒有 "+city+" 這座城市。\n");

		msg = CITY_D->query_city_idname(city)+"之當日原料採集狀況如下(自動於每日 23:00 歸零)：\n";
		msg += WHT"─────────────────────────────────────\n"NOR;

		for(num=0;CITY_D->city_exist(city, num);num++)
		{
			msg += CITY_D->query_city_idname(city, num)+"\n";

			collection_record = CITY_D->query_collection_record(city, num);

			if( !sizeof(collection_record) )
				msg += "  無人採集\n";
			else
			foreach(id, data in collection_record)
			{
				msg += sprintf("  %-15s%-10s%-10s%-10s%-10s%-10s\n",
					capitalize(id),
					HIW+data["metal"]+NOR,
					WHT+data["stone"]+NOR,
					HIC+data["water"]+NOR,
					YEL+data["wood"]+NOR,
					HIM+data["fuel"]+NOR);
			}

			msg += "\n";
		}

		msg += WHT"─────────────────────────────────────\n"NOR;
		msg += HIW"金屬 metal"NOR"、"WHT"原石 stone"NOR"、"HIC"清水 water"NOR"、"YEL"原木 wood"NOR"、"HIM"燃料 fuel"NOR"\n\n";
		return me->more(msg);
	}

	if( !env || !env->is_maproom() )
		return tell(me, "無法在這裡採集資源。\n");


	//忙碌中不能下指令
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	loc = query_temp("location", me);

	if( (MAP_D->query_map_system(loc)) != CITY_D )
		return tell(me, "無法在這裡採集資源。\n");

 	city = loc[CITY];
	num = loc[NUM];

 	if( !CITY_D->is_citizen(me->query_id(1), city) )
 		return tell(me, pnoun(2, me)+"不是"+CITY_D->query_city_idname(city)+"的市民，無法在這裡採集資源。\n");

	coorrangetype = CITY_D->query_coor_range(loc, TYPE, 1);

	arg = lower_case(arg);

	if( sscanf(arg, "%d %s", amount, arg2) == 2 )
		arg = arg2;

	if( amount < 1 )
		amount = 1;

	if( amount > 1000 )
		return tell(me, "最多一次只能採集 1000 個資源。\n");

	switch(arg)
	{
		case "metal":
			skilllevel = me->query_skill_level(arg);

			coorrangetype = filter(coorrangetype, (: $2 == MOUNTAIN :));

			if( !sizeof(coorrangetype) )
				return tell(me, "附近的區域中並沒有山區，沒辦法開採金屬。\n");

			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));

			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"身上沒有用來開採金屬的工具。\n");

			// 檢查附近區域的材料存量
			foreach(sloc in keys(coorrangetype))
			{
				loc = restore_variable(sloc);
				materialamount = CITY_D->query_section_info(city, num, "resource/"+arg);

				if( materialamount >= amount )
				{
					if( !cost_stamina(me, amount, skilllevel) )
						return msg("$ME氣喘噓噓，已經沒力氣再開採金屬了...。\n", me, 0, 1);

					endurance = query("endurance", tools[0]);

					// 工具損壞(損壞機率與工具耐久度和使用者技能相關)
					if( !random(endurance + to_int(skilllevel*endurance/100.)) )
					{
						msg("$ME手上的"+tools[0]->query_idname()+"突然『啪』的一聲，手柄處裂了開來，顯然是沒辦法再繼續用來開採金屬了...。\n", me, 0, 1);

						if( !random(2) )
						{
							tell(me, pnoun(2, me)+"從這個事件中獲得更多的社會經驗。\n");
							me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
						}

						destruct(tools[0], 1);
						return;
					}

					CITY_D->set_section_info(city, num, "resource/"+arg, materialamount-amount);

					ob = new(MATERIAL_PATH+arg);

					if( amount > 1 )
						set_temp("amount", amount, ob);

					msg("$ME賣力地從礦石中鑿出了"+ob->short(1)+"\n", me, 0, 1);

					// 紀錄原料採集資訊
					CITY_D->add_collection_record(city, num, myid, arg, amount);

					if( !me->get_object(ob, amount) )
					{
						msg("$ME已經拿不動"+ob->short(1)+"了！只好先放在地上。\n", me, 0, 1);
						ob->move_to_environment(me);
					}

					if( me->query_skill_level("metal") < 20 )
						me->add_skill_exp("metal", amount*(12+random(11)));
					else if( me->query_skill_level("metal") == 20 )
						tell(me, pnoun(2, me)+"的採集技能等級已經到達 20，無法再自我往上提升，更高深的學問必須開始到大學學習。\n");

					me->add_social_exp(to_int(pow(amount*(20+random(21)), 0.9)));
					TREASURE_D->get_treasure_books(me, amount);
					return;
				}
			}
			return tell(me, "整座城市的金屬資源都已經被大量開採，剩下都是不能再使用的碎礦。\n");
			break;

		case "stone":
			skilllevel = me->query_skill_level(arg);

			coorrangetype = filter(coorrangetype, (: $2 == MOUNTAIN :));

			if( !sizeof(coorrangetype) )
				return tell(me, "附近的區域中並沒有山區，沒辦法開採石材。\n");

			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));

			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"身上沒有用來開採石材的工具。\n");

			// 檢查附近區域的材料存量
			foreach(sloc in keys(coorrangetype))
			{
				loc = restore_variable(sloc);
				materialamount = CITY_D->query_section_info(city, num, "resource/"+arg);

				if( materialamount >= amount )
				{
					if( !cost_stamina(me, amount, skilllevel) )
						return msg("$ME氣喘噓噓，已經沒力氣再開採石材了...。\n", me, 0, 1);

					endurance = query("endurance", tools[0]);

					// 工具損壞(損壞機率與工具耐久度和使用者技能相關)
					if( !random(endurance + to_int(skilllevel*endurance/100.)) )
					{
						msg("$ME手上的"+tools[0]->query_idname()+"突然『啪』的一聲，手柄處裂了開來，顯然是沒辦法再繼續用來開採石材了...。\n", me, 0, 1);

						if( !random(2) )
						{
							tell(me, pnoun(2, me)+"從這個事件中獲得更多的社會經驗。\n");
							me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
						}
						destruct(tools[0], 1);
						return;
					}

					CITY_D->set_section_info(city, num, "resource/"+arg, materialamount-amount);

					ob = new(MATERIAL_PATH+arg);

					if( amount > 1 )
						set_temp("amount", amount, ob);

					msg("$ME賣力地從岩層鑿出了"+ob->short(1)+"\n", me, 0, 1);

					// 紀錄原料採集資訊
					CITY_D->add_collection_record(city, num, myid, arg, amount);

					if( !me->get_object(ob, amount) )
					{
						msg("$ME已經拿不動"+ob->short(1)+"了！只好先放在地上。\n", me, 0, 1);
						ob->move_to_environment(me);
					}

					if( me->query_skill_level("stone") < 20 )
						me->add_skill_exp("stone", amount*(12+random(11)));
					else if( me->query_skill_level("stone") == 20 )
						tell(me, pnoun(2, me)+"的採集技能等級已經到達 20，無法再自我往上提升，更高深的學問必須開始到大學學習。\n");

					me->add_social_exp(to_int(pow(amount*(20+random(21)), 0.9)));
					TREASURE_D->get_treasure_books(me, amount);
					return;
				}
			}
			return tell(me, "整座城市的石材資源都已經被大量開採，剩下都是不能再使用的碎石。\n");
			break;

		case "water":
			skilllevel = me->query_skill_level(arg);

			coorrangetype = filter(coorrangetype, (: $2 == POOL || $2 == RIVER :));

			if( !sizeof(coorrangetype) )
				return tell(me, "附近的區域中並沒有水源，沒辦法採集清水。\n");

			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));

			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"身上沒有用來集水的工具。\n");

			// 檢查附近區域的材料存量
			foreach(sloc in keys(coorrangetype))
			{
				loc = restore_variable(sloc);
				materialamount = CITY_D->query_section_info(city, num, "resource/"+arg);

				if( materialamount >= amount )
				{
					if( !cost_stamina(me, amount, skilllevel) )
						return msg("$ME氣喘噓噓，已經沒力氣再舀水了...。\n", me, 0, 1);

					endurance = query("endurance", tools[0]);

					// 工具損壞(損壞機率與工具耐久度和使用者技能相關)
					if( !random(endurance + to_int(skilllevel*endurance/100.)) )
					{
						msg("$ME手上的"+tools[0]->query_idname()+"突然『啪』的一聲，底部裂了一個大洞，顯然是沒辦法再繼續用來裝水了...。\n", me, 0, 1);

						if( !random(2) )
						{
							tell(me, pnoun(2, me)+"從這個事件中獲得更多的社會經驗。\n");
							me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
						}

						destruct(tools[0], 1);
						return;
					}

					CITY_D->set_section_info(city, num, "resource/"+arg, materialamount-amount);

					ob = new(MATERIAL_PATH+arg);

					if( amount > 1 )
						set_temp("amount", amount, ob);

					msg("$ME用力地從河中舀起了"+ob->short(1)+"\n", me, 0, 1);

					// 紀錄原料採集資訊
					CITY_D->add_collection_record(city, num, myid, arg, amount);

					if( !me->get_object(ob, amount) )
					{
						msg("$ME已經拿不動"+ob->short(1)+"了！只好先放在地上。\n", me, 0, 1);
						ob->move_to_environment(me);
					}

					if( me->query_skill_level("water") < 20 )
						me->add_skill_exp("water", amount*(12+random(11)));
					else if( me->query_skill_level("water") == 20 )
						tell(me, pnoun(2, me)+"的採集技能等級已經到達 20，無法再自我往上提升，更高深的學問必須開始到大學學習。\n");

					me->add_social_exp(to_int(pow(amount*(20+random(21)), 0.9)));
					TREASURE_D->get_treasure_books(me, amount);
					return;
				}
			}
			return tell(me, "整座城市的水資源都已經被大量使用，水質顯得相當混濁不能再使用了。\n");
			break;

		case "wood":
			skilllevel = me->query_skill_level(arg);

			coorrangetype = filter(coorrangetype, (: $2 == FOREST :));

			if( !sizeof(coorrangetype) )
				return tell(me, "附近的區域中並沒有樹林，沒辦法伐木。\n");

			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));

			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"身上沒有用來伐木的工具。\n");

			// 檢查附近區域的材料存量
			foreach(sloc in keys(coorrangetype))
			{
				loc = restore_variable(sloc);
				materialamount = CITY_D->query_section_info(city, num, "resource/"+arg);

				if( materialamount >= amount )
				{
					if( !cost_stamina(me, amount, skilllevel) )
						return msg("$ME氣喘噓噓，已經沒力氣再伐木了...。\n", me, 0, 1);

					endurance = query("endurance", tools[0]);

					// 工具損壞(損壞機率與工具耐久度和使用者技能相關)
					if( !random(endurance + to_int(skilllevel*endurance/100.)) )
					{
						msg("$ME手上的"+tools[0]->query_idname()+"突然『啪』的一聲，手柄處裂了開來，顯然是沒辦法再繼續用來伐木了...。\n", me, 0, 1);

						if( !random(2) )
						{
							tell(me, pnoun(2, me)+"從這個事件中獲得更多的社會經驗。\n");
							me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
						}

						destruct(tools[0], 1);
						return;
					}

					CITY_D->set_section_info(city, num, "resource/"+arg, materialamount-amount);

					ob = new(MATERIAL_PATH+arg);

					if( amount > 1 )
						set_temp("amount", amount, ob);

					msg("$ME努力地從巨木鋸下了"+ob->short(1)+"\n", me, 0, 1);

					// 紀錄原料採集資訊
					CITY_D->add_collection_record(city, num, myid, arg, amount);

					if( !me->get_object(ob, amount) )
					{
						msg("$ME已經拿不動"+ob->short(1)+"了！只好先放在地上。\n", me, 0, 1);
						ob->move_to_environment(me);
					}

					if( me->query_skill_level("wood") < 20 )
						me->add_skill_exp("wood", amount*(12+random(11)));
					else if( me->query_skill_level("wood") == 20 )
						tell(me, pnoun(2, me)+"的採集技能等級已經到達 20，無法再自我往上提升，更高深的學問必須開始到大學學習。\n");

					me->add_social_exp(to_int(pow(amount*(20+random(21)), 0.9)));
					TREASURE_D->get_treasure_books(me, amount);
					return;
				}
			}
			return tell(me, "整座城市的樹林資源已經被大量砍伐，剩下都是不能再使用的幼小樹苗。\n");
			break;

		case "fuel":
			skilllevel = me->query_skill_level(arg);

			coorrangetype = filter(coorrangetype, (: $2 == FOREST :));

			if( !sizeof(coorrangetype) )
				return tell(me, "附近的區域中並沒有森林，沒辦法開採燃料。\n");

			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));

			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"身上沒有用來開採燃料的工具。\n");

			// 檢查附近區域的材料存量
			foreach(sloc in keys(coorrangetype))
			{
				loc = restore_variable(sloc);
				materialamount = CITY_D->query_section_info(city, num, "resource/"+arg);

				if( materialamount >= amount )
				{
					if( !cost_stamina(me, amount, skilllevel) )
						return msg("$ME氣喘噓噓，已經沒力氣再開採燃料了...。\n", me, 0, 1);

					endurance = query("endurance", tools[0]);

					// 工具損壞(損壞機率與工具耐久度和使用者技能相關)
					if( !random(endurance + to_int(skilllevel*endurance/100.)) )
					{
						msg("$ME手上的"+tools[0]->query_idname()+"突然『啪』的一聲，手柄處裂了開來，顯然是沒辦法再繼續用來開採燃料了...。\n", me, 0, 1);

						if( !random(2) )
						{
							tell(me, pnoun(2, me)+"從這個事件中獲得更多的社會經驗。\n");
							me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
						}

						destruct(tools[0], 1);
						return;
					}

					CITY_D->set_section_info(city, num, "resource/"+arg, materialamount-amount);

					ob = new(MATERIAL_PATH+arg);

					if( amount > 1 )
						set_temp("amount", amount, ob);

					msg("$ME賣力地從土地裡取出了"+ob->short(1)+"\n", me, 0, 1);

					// 紀錄原料採集資訊
					CITY_D->add_collection_record(city, num, myid, arg, amount);

					if( !me->get_object(ob, amount) )
					{
						msg("$ME已經拿不動"+ob->short(1)+"了！只好先放在地上。\n", me, 0, 1);
						ob->move_to_environment(me);
					}
					if( me->query_skill_level("fuel") < 20 )
						me->add_skill_exp("fuel", amount*(12+random(11)));
					else if( me->query_skill_level("fuel") == 20 )
						tell(me, pnoun(2, me)+"的採集技能等級已經到達 20，無法再自我往上提升，更高深的學問必須開始到大學學習。\n");

					me->add_social_exp(to_int(pow(amount*(20+random(21)), 0.9)));
					TREASURE_D->get_treasure_books(me, amount);
					return;
				}
			}
			return tell(me, "整座城市的燃料資源都已經被大量開採，無法再找到可利用的燃料。\n");
			break;



		default:
			return tell(me, pnoun(2, me)+"想要收集哪種原料？("HIW"金屬 metal"NOR", "WHT"原石 stone"NOR", "HIC"清水 water"NOR", "YEL"原木 wood"NOR", "HIM"燃料 fuel"NOR")\n");
			break;
	}
}
