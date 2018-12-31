/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _list_action.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-23
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>

void output_object(object env, string database, string basename, string amount);

// �C�X�ӫ~�C��
void do_list(object me, string arg, string database, int capacity)
{
	int i, j, size, average_desire;
	object env, ob;
	string msg, money_unit, popproduct, desirehint;
	string setprice, pricepercent, sell;
	mixed value;
	mapping obs;
	string *shelfsort, shelf, totalamount, replacename;
	array data;
	
	env = environment(me)->query_master();
	
	money_unit = MONEY_D->city_to_money_unit(env->query_city());
	
	// �Y�O���y����
	if( env->query_building_type() == "trading_post" )
		obs = fetch_variable("allowable_trade");
	else
		obs = query(database, env);
	
	if( !sizeof(obs) )
		return tell(me, query("short", env)+"�ثe�S�����󪫫~�C\n");
	
	popproduct = SHOPPING_D->query_pop_product();

	msg =  NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += HIW    "�s�� �ӫ~"+(query("mode", env)?HIR"(�޲z�Ҧ�)"NOR:HIC"(��~�Ҧ�)"NOR)+"                    �H�� �ƶq     ���($"+money_unit+")   ����� ����\n"NOR;
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	shelfsort = keys(obs);
	
	average_desire = query("shopping/average_desire", env);

	foreach( shelf in shelfsort )
	{
		switch(shelf)
		{
			case 0:		msg += HIC"�i��"NOR"\n"; break;
			case "�P�P":	msg += HIY"�P�P"NOR"\n"; break;
			default:	msg += HIG+shelf+NOR"\n"; break;
		}

		data = copy(obs[shelf]);

		size = sizeof(data);
		
		for(j=0;j<size;j+=2)
		{
			if( catch(ob = load_object(data[j]) ) )
				continue;
		
			// �ɮפw�g����, �R�����
			if( !objectp(ob) && !file_exists(data[j]) )
			{
				output_object(env, database, data[j], "all");
				continue;
			}
			
			replacename = replace_string(data[j], "/", "#");
			
			setprice = query("setup/price/"+replacename, env);
			value = query("value", ob);
			totalamount = count(totalamount, "+", data[j+1]);

			if( !query("setup/class/"+replacename, env) )
				desirehint = "";
			else
			switch(average_desire + query("shopping/productdesire/"+replacename, env))
			{
				case -1999999999..0:
					desirehint = ""; break;
				case 1..30:
					desirehint = NOR WHT"*"; break;
				case 31..60:
					desirehint = NOR WHT"**"; break;
				case 61..90:
					desirehint = NOR WHT"***"; break;
				case 91..120:
					desirehint = HIW"*"NOR WHT"***"; break;
				case 121..150:
					desirehint = HIW"**"NOR WHT"***"; break;
				case 151..180:
					desirehint = HIW"***"NOR WHT"***"; break;
				case 181..1000000000:
					desirehint = HIW BLINK"*"NOR HIW"**"NOR WHT"***"; break;
				default:
					
					desirehint = HIR"Bug"NOR; break;
			}
			
			if( setprice )
				pricepercent = count(count(setprice, "*", 100), "/", value);
			else
				pricepercent = "100";
			
			if( query("setup/sell/"+replacename+"/nosell", env) )
				sell = "";
			else if( !undefinedp(query("setup/sell/"+replacename+"/percent", env)) )
				sell = query("setup/sell/"+replacename+"/percent", env) +"%";
			else if( !undefinedp(query("setup/sell/all/percent", env)) )
				sell = query("setup/sell/all/percent", env) +"%";
			else
				sell = "";

			msg += sprintf(HIW"%-3d-"NOR" %-32s%6s "HIC"%-9s"HIY"%-13s"HIG"%5s"HIR"%5s"NOR"%s"NOR"\n", 
				++i, 
				ob->query_idname(),
				desirehint,
				(data[j+1]=="-1"?HIC"�L��"NOR:data[j+1])+"", 
				NUMBER_D->number_symbol(setprice||value),
				(count(pricepercent, ">", 999)?">999":pricepercent)+"%",
				sell,
				(data[j]==popproduct?HIR"Hot!"NOR:"")+(wizardp(me) ? " "+(average_desire+query("shopping/productdesire/"+replacename, env))+"":""),
				);
		}
	}
	
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += sprintf("%-54s%24s\n", NOR WHT"�@�� "HIW+i+NOR WHT" �ذӫ~���b�c�椤�A�e�q "+(capacity == -1 ? HIW"�L��"NOR:HIW+NUMBER_D->number_symbol(totalamount)+NOR WHT" / "HIW+NUMBER_D->number_symbol(count(capacity, "*", (sizeof(query("slave", env))+1))))+NOR, query("short", env));
	me->more(msg);
}

void do_warehouse_list(object me, string arg, string database, int capacity)
{
	int i, j, size;
	object env, ob;
	string msg, money_unit;
	string setprice, value;
	mapping obs;
	string *shelfsort, shelf, totalamount;
	array data;
	
	env = environment(me)->query_master();
	
	money_unit = MONEY_D->city_to_money_unit(env->query_city());
	
	// �Y�O���y����
	if( env->query_building_type() == "trading_post" )
		obs = fetch_variable("allowable_trade");
	else
		obs = query(database, env);
	
	if( !sizeof(obs) )
		return tell(me, query("short", env)+"�ثe�S�����󪫫~�C\n");
	
	msg =  NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += HIW    "�s��  ���~                                                          �ƶq\n"NOR;
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	shelfsort = keys(obs);
	
	foreach( shelf in shelfsort )
	{
		if( !shelf )
			msg += HIC"�i��\n"NOR;
		else
			msg += shelf + "\n";

		data = copy(obs[shelf]);

		size = sizeof(data);
		
		for(j=0;j<size;j+=2)
		{
			if( catch(ob = load_object(data[j]) ) )
				continue;
		
			// �ɮפw�g����, �R�����
			if( !objectp(ob) && !file_exists(data[j]) )
			{
				output_object(env, database, data[j], "all");
				continue;
			}
			
			setprice = query("setup/price/"+replace_string(data[j], "/", "#"), env);
			value = query("value", ob);
			totalamount = count(totalamount, "+", data[j+1]);

			msg += sprintf(HIW"%-4d-"NOR" %-50s%16s\n", 
				++i, 
				ob->query_idname(), 
				data[j+1]=="-1"?"�L��":data[j+1], 
				);
		}
	}
	
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += sprintf("%-54s%18s\n", NOR WHT"�@�� "HIW+i+NOR WHT" �ت��~�A�e�q "+(capacity == -1 ? HIW"�L��"NOR:HIW+NUMBER_D->number_symbol(totalamount)+NOR WHT" / "HIW+NUMBER_D->number_symbol(count(capacity, "*", (sizeof(query("slave", env))+1))))+NOR, query("short", env));
	me->more(msg);
}