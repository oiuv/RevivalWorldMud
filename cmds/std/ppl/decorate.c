/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : decorate.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-11
 * Note   : 建築物內部裝潢指令。
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <location.h>
#include <message.h>
#include <citydata.h>

inherit COMMAND;

string help = @HELP
decorate [short [-d] | long [-d] | sign 字 | function [功能] | master [-d]]

decorate short			設定短名稱(房間與地圖上皆可設定)
decorate short -d		刪除短名稱(僅適用於地圖上)
decorate long			設定長敘述(房間與地圖上皆可設定)
decorate long -d		刪除長敘述(僅適用於地圖上)
decorate sign '字'		對於建築物內部房間，可設定屋頂字樣
decorate function '功能'	對於建築物內部房間，可設定房間功能型態
decorate function		列出可設定之房間型態
decorate function -d		取消目前此房間之功能型態
decorate master 主部門座標  	可於房間中設定相對應的主要連鎖部門所在都市與座標(例 2 50,50)
decorate master -d		刪除房間中的主要連鎖部門設定

HELP;

#define MAX_LONG_WIDTH		72
#define MAX_LONG_LINE		20
#define MAX_LONG_LINE_MAP	3
#define MAX_SHORT_WIDTH		24
#define MAX_SHORT_WIDTH_MAP	18

private varargs void input_room_short(object me, object env, string arg, int no_encode)
{
	array loc = query_temp("location", me);

	if( !arg || !arg[0] )
	{
		tell(me, "取消修改。\n");
		return me->finish_input();
	}
	
	if( !no_encode && query("encode/gb", me) ) arg = G2B(arg);

	arg = ansi(arg);
	arg = remove_fringe_blanks(arg);
	arg = kill_repeat_ansi(arg+NOR);

	if( (env->is_maproom() && noansi_strlen(arg) > MAX_SHORT_WIDTH_MAP) || noansi_strlen(arg) > MAX_SHORT_WIDTH )
	{
		tell(me, "短名稱最多只能 "+(env->is_maproom()?MAX_SHORT_WIDTH_MAP:MAX_SHORT_WIDTH)+" 個字元。\n");
		return me->finish_input();
	}

	if( env->is_maproom() )
	{
		(MAP_D->query_map_system(loc))->set_coor_data(loc, "short", arg);
		msg("$ME修改此地的短名稱為「"+arg+"」。\n", me, 0, 1);
		return;
	}
	else
	{
		set("short", arg, env);
		msg("$ME修改此地的短名稱為「"+arg+"」。\n", me, 0, 1);

		if( !env->save() )
			error("decorate save room error.");
	}	
	me->finish_input();
}

private void input_room_long(object me, object env, string arg)
{
	array loc = query_temp("location", me);

	if( !arg || !arg[0] )
	{
		tell(me, "取消修改。\n");
		return me->finish_input();
	}

	arg = multiwrap(arg, MAX_LONG_WIDTH);

	arg = ansi(arg);
	arg = kill_repeat_ansi(arg+NOR);

	//[&,->，][&'->、][&.->。][&!->！][&?->？]
	arg = replace_string(arg, "&,", "，");
	arg = replace_string(arg, "&'", "、");
	arg = replace_string(arg, "&.", "。");
	arg = replace_string(arg, "&!", "！");
	arg = replace_string(arg, "&?", "？");

	if( (env->is_maproom() && sizeof(explode(arg, "\n")) > MAX_LONG_LINE_MAP) || sizeof(explode(arg, "\n")) > MAX_LONG_LINE )
	{
		tell(me, "房間長敘述最多只能有 "+(env->is_maproom()?MAX_LONG_LINE_MAP:MAX_LONG_LINE)+" 行。\n");
		return me->finish_input();
	}

	if( env->is_maproom() )
	{
		(MAP_D->query_map_system(loc))->set_coor_data(loc, "long", arg+"\n");
		msg("$ME修改此地之長敘述為\n「\n"+arg+"\n"+repeat_string(" ", MAX_LONG_WIDTH)+"」。\n", me, 0, 1);
	}
	else
	{
		set("long", arg, env);
		msg("$ME修改此地之長敘述為\n「\n"+arg+"\n"+repeat_string(" ", MAX_LONG_WIDTH)+"」。\n", me, 0, 1);

		if( !env->save() )
			error("decorate save room error.");
	}
}

private void do_command(object me, string arg)
{
	object env = environment(me);
	string option;
	array loc = query_temp("location", me);
	string owner = query("owner", env);

	if( !arg )
		return tell(me, "請輸入想要裝潢的項目(short, long, function, sign, master)。\n");

	if( !env )
		return tell(me, pnoun(2, me)+"無法在這裡做裝潢的動作。\n");

	if( env->is_maproom() )
		owner = (MAP_D->query_map_system(loc))->query_coor_data(loc, "owner");

	if( !owner )
		return tell(me, "這間房間不是"+pnoun(2, me)+"的，無法裝潢這間房間。\n");
	else if( belong_to_government(owner) )
	{
		if( !wizardp(me) && !CITY_D->is_mayor_or_officer(env->query_city(), me) )
			return tell(me, pnoun(2, me)+"不是市長或官員，無法裝潢這間房間。\n");
	}
	else if( belong_to_enterprise(owner) )
	{
		if( owner[11..] != query("enterprise", me) )
			return tell(me, pnoun(2, me)+"不是"+ENTERPRISE_D->query_enterprise_color_id(query("enterprise", me))+"企業集團的成員，無法裝潢這間房間。\n");
	}
	else if( owner != me->query_id(1) )
		return tell(me, "這間房間不是"+pnoun(2, me)+"的，無法裝潢這間房間。\n");

	sscanf(arg, "%s %s", arg, option);

	switch(arg)
	{
	case "short":
		if( option == "-d" )
		{
			if( env->is_maproom() )
				(MAP_D->query_map_system(loc))->delete_coor_data(loc, "short");
			else
				return tell(me, "無法刪除房間短名稱。\n");

			return tell(me, "刪除短名稱完畢。\n");
		}
		else if( option )
			return input_room_short(me, env, option, 1);

		tell(me, HIC"\n請輸入此房間的短名稱："NOR);
		input_to( (: input_room_short, me, env :) );
		return;

	case "long":
		if( option == "-d" )
		{
			if( env->is_maproom() )
				(MAP_D->query_map_system(loc))->delete_coor_data(loc, "long");
			else
				return tell(me, "無法刪除房間長敘述。\n");

			return tell(me, "刪除長敘述完畢。\n");
		}
		tell(me, HIC"\n請輸入此房間的長敘述：\n"HIR"每行最多 "+MAX_LONG_WIDTH+" 字，最多 "+MAX_LONG_LINE+" 行\n"HIG"全形注音符號快速轉換：[&,->，][&'->、][&.->。][&!->！][&?->？]\n"NOR);
		me->edit( (: input_room_long, me, env :) );
		return;

	case "function":
		{
			mapping action_info;
			if( env->is_maproom() )
				return tell(me, "這裡無法做這樣的裝潢動作。\n");

			action_info = env->query_action_info();

			if( !option )
			{
				string msg = "這棟建築物能夠設置的房間功能有以下 "+sizeof(action_info)+" 種：\n";

				msg += WHT"───────────────────────────────\n"NOR;
				msg +=    " 功能         名稱         功能\n";
				msg += WHT"───────────────────────────────\n"NOR;
				foreach( string short, mapping info in action_info )
				msg += sprintf(" %-12s %-12s %s\n", HIY+capitalize(short)+NOR, info["short"], YEL+implode(keys(info["action"]), " ")+NOR);

				msg += WHT"───────────────────────────────\n請輸入 decorate function '功能'。\n\n"NOR;

				return tell(me, msg);
			}

			option = lower_case(option);

			if( option == "-d" )
			{
				if( env->remove_room_function() )
				{
					env->remove_master();
					env->remove_slave();
					return tell(me, "取消此房間功能設定完成，同時刪除連鎖設定。\n");
				}
				else
					return tell(me, "取消失敗。\n");
			}
			
			if( !wizardp(me) && strsrch(query("long", env), "油漆氣味") != -1 )
				return tell(me, "這間房間還沒有仔細裝潢過，太過雜亂無法設定房間功能。\n");

			if( option == "scenery" && env->query_floor() != 100 && env->query_floor() != 120 && env->query_floor() != 140 && env->query_floor() != 160 )
				return tell(me, "觀景台只有在 100F / 120F / 140F / 160F 等樓層可以設置。\n");
			
			if( query("function", env) == option )
				return tell(me, "此房間已經設定為此功能。\n");
			
			if( !env->set_room_function(option) )	
				return tell(me, "這棟建築物沒有這類型的內部房間。\n");

			env->remove_master();
			env->remove_slave();
			msg("$ME將這間房間設置成"+action_info[option]["short"]+"的用途。\n",me,0,1);

			break;
		}
	case "sign":
		{
			if( env->is_maproom() )
				return tell(me, pnoun(2, me)+"無法修改地圖上的圖形。\n");
	
			if( !option )
				return tell(me, "此處屋頂上的圖形是「"+(MAP_D->query_map_system(env->query_location()))->query_coor_icon(env->query_location())+NOR"」。\n");
			if( option == "-d" )
			{
				string origin_sign;
	
				if( env->is_maproom() )
					origin_sign = (MAP_D->query_map_system(loc))->query_coor_icon(loc);
				else
					origin_sign = (MAP_D->query_map_system(env->query_location()))->query_coor_icon(env->query_location());
	
				msg("$ME爬上了這裡的屋頂將原本的「"+origin_sign+"」字刷洗掉了。\n", me, 0, 1);
				(MAP_D->query_map_system(env->query_location()))->set_coor_icon(env->query_location(), "  ");
				break; 
			}
	
	
			if( noansi_strlen(option) > 2 )
				return tell(me, "招牌一個房間最多 2 個字元。\n");
	
			(MAP_D->query_map_system(env->query_location()))->set_coor_icon(env->query_location(), sprintf("%2s", kill_repeat_ansi(option+NOR)));
			msg("$ME拿著一支油漆刷，爬上了這裡的屋頂大大地寫了個「"+option+NOR"」字。\n",me, 0, 1);

			break;
		}
	case "master":
		{
			int x, y, section;
			array envloc = env->query_location();
			array newloc;
			string room_type, env_basename, master_basename;
			object master = env->query_master();
			string functionname;
			mapping products;
		
			if( env->is_maproom() )
				return tell(me, "這裡無法做這樣的裝潢動作。\n");

			if( !option )	
				return tell(me, "請輸入欲對應的主連鎖門部房間所在都市與座標(例如第二衛星都市：2 53,49)。\n",me, 0, 1);

			functionname = query("function", env);
			
			if( !functionname )
				return tell(me, "此房間未設定房間功能不能進行連鎖動作。\n");

			if( !mapp(env->query_action_info(functionname)) || !env->query_action_info(functionname)["master"] )
				return tell(me, "此種功能房間無法進行連鎖動作。\n");

			if( option == "-d" )
			{
				if( master == env )
					env->remove_slave();

				env->remove_master();
				env->save();
				return tell(me, "刪除此房間的對應主連鎖門部"+query("short", master)+"設定。\n");
			}

			if( sscanf(option, "%d %d,%d", section, x, y) != 3 )
				return tell(me, "請輸入欲對應的主連鎖門部房間所在都市與座標(例如第二衛星都市：2 53,49)。\n",me, 0, 1);

			if( sizeof(products = query("products", env)) )
				tell(me, HIY"注意：連鎖之後這個房間原本的庫存資料將會被掩蓋(所有資料以連鎖中心為準)\n解除連鎖後本房間原始的庫存資料便可以再顯示出來。\n"NOR);
				
			env->remove_master();
			env_basename = base_name(env);

			room_type = ESTATE_D->query_loc_estate(envloc)["type"];

			if( !CITY_D->city_exist(envloc[CITY], section-1) )
				return tell(me, CITY_D->query_city_idname(envloc[CITY])+"沒有第 "+section+" 衛星都市。\n");

			master_basename = CITY_ROOM_MODULE(envloc[CITY], (section-1), (x-1), (y-1), room_type);
			newloc = arrange_city_location(x-1, y-1, envloc[CITY], section-1);
			
			if( master_basename == env_basename )
				return tell(me, "對應的主連鎖門部不能是自己。\n");

			if( sizeof(query("slave", env)) )
				return tell(me, "此間房間是連鎖中心，已經被其他門部連鎖，無法再連鎖至其他門部。\n");

			if( !file_exists(master_basename) )
			{
				tell(find_player("sinji"), master_basename);
				return tell(me, "座標"+loc_short(newloc)+"處並沒有任何建築物。\n");
			}
			master = load_object(master_basename);

			if( query("owner", master) == "GOVERNMENT/"+envloc[CITY] )
			{
				if( !CITY_D->is_mayor_or_officer(envloc[CITY], me) )
					return tell(me, pnoun(2, me)+"不是市長或官員，無法裝潢這間房間。\n");
			}
			else if( query("owner", master) != me->query_id(1) )
				return tell(me, "座標"+loc_short(newloc)+"的"+master->query_room_name()+"並不是屬於"+pnoun(2, me)+"的。\n");

			if( master->query_master() != master )
				return tell(me, "座標"+loc_short(newloc)+"的"+master->query_room_name()+"已經有其自己所對應的主連鎖門部，無法被設定為主連鎖門部。\n");

			if( query("function", env) != query("function", master) )
				return tell(me, "座標"+loc_short(newloc)+"的"+master->query_room_name()+"與此間房間的功能並不相同，無法設定連鎖。\n");

			if( sizeof(master->query_slave()) > 500 )
				return tell(me, "連鎖房間數不得超過 500。\n");

			if( !env->set_master(master) )
				return tell(me, "無法設定其為主連鎖門部。\n");

			// 有人利用兩個連鎖互相交換牟利, 利用兩連鎖價差, 趁交換時未重新計算購買欲時牟利, 因此用以下程式修正
			env->query_module_file()->calculate_master_data(master);

			msg("$ME設定"+master->query_room_name()+"為此房間的主要連鎖部門。\n", me, 0, 1);
			break;
		}
	default:
		return tell(me, "請輸入想要裝潢的項目(short, long, function, sign, master)。\n");
	}

	if( env->save() )
		tell(me, "裝潢完畢。\n");
	else
		tell(me, "裝潢失敗。\n");
}
