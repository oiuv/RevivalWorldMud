/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _buy_action.c
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
#include <npc.h>

int over_capacity(object env, string database, string amount, mixed capacity);
void input_object(object env, string database, string basename, string amount);
void output_object(object env, string database, string basename, string amount);


#define STORE_CAPACITY		100000
#define WAREHOUSE_CAPACITY	-1

// �R�J�ӫ~
void do_buy(object me, string arg, string database, string buyer)
{
	int price_skill_level;
	int i, which = 1, size, capacity;
	object env, product, newproduct, delivery, delivery_master;
	mapping products;
	string tmp, amount = "1", env_city, cost, earn, productname, delivery_city;
	int delivery_x, delivery_y, num;
	string *productlist, pamount, basename;
	array loc;
	
	env = environment(me)->query_master();

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�R����ӫ~�H\n");
	
	env_city = env->query_city();

	if( query("mode", env) )
		return tell(me, environment(me)->query_room_name()+"�ثe���A���u�޲z�Ҧ��v�A����c�檫�~�C\n");

	if( 
		sscanf(arg, "%s to '%s' %d %d,%d", arg, delivery_city, num, delivery_x, delivery_y) == 5 ||
		sscanf(arg, "%s to %s %d %d,%d", arg, delivery_city, num, delivery_x, delivery_y) == 5
	)
	{
		if( !CITY_D->city_exist(delivery_city||env_city, num-1) )
			return tell(me, "�o�y�����èS���� "+num+" �ìP�����C\n");
	
		loc = arrange_city_location(delivery_x-1, delivery_y-1, delivery_city||env_city, num-1);
		
		delivery = load_module(loc);
		
		if( !objectp(delivery) )
			return tell(me, "�y��"+loc_short(loc)+"�B�èS������ؿv���C\n");
			
		if( query("owner", delivery) != me->query_id(1) )
			return tell(me, delivery->query_room_name()+"�ä��O"+pnoun(2, me)+"���ؿv���C\n");
		
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
				return tell(me, delivery->query_room_name()+"�S����k��������ӫ~�C\n");
		}
		
		switch( query("function", delivery) )
		{
			case "front":
			case "storeroom":
			case "warehouse":
				break;
			default:
				return tell(me, delivery->query_room_name()+"�S����k��������ӫ~�C\n");
		}
		
		if( delivery_master == env )
			return tell(me, pnoun(2, me)+"���B�e���ӫ~�a�I����P���a�ۦP�Φ��ۦP�s�ꤤ�ߡC\n");
	}
		
	sscanf(arg, "%s %s", amount, productname);
	
	if( !productname || (amount != "all" && !(amount = big_number_check(amount))) )
	{
		productname = arg;
		amount = "1";
	}
	
	if( sscanf( productname, "%s %d", tmp, which ) == 2 )
		productname = tmp;

	if( which < 1 ) which = 1;

	if( env->query_building_type() == "trading_post" )
		products = fetch_variable("allowable_trade");
	else
		products = query(database, env);
	
	if( !sizeof(products) )
		return tell(me, query("short", env)+"�ثe�S���c�����ӫ~�C\n");

	productlist = implode(values(products), (:$1+$2:))||allocate(0);
	size = sizeof(productlist);
	price_skill_level = me->query_skill_level("price");

	for(i=0;i<size;i+=2)
	{
		basename = productlist[i];
		pamount = productlist[i+1];

		if( catch(product = load_object(basename)) )
			continue;

		// �ɮפw�g����
		if( !objectp(product) )
		{
			error(basename+" �ӫ~��ƿ��~�C");
			continue;
		}

		if( (i+2)/2 == to_int(big_number_check(productname)) || (product->query_id(1) == lower_case(productname) && !(--which)) )
		{
			if( amount == "all" )
			{
				if( count(pamount, "<", 1) )
					return tell(me, pnoun(2, me)+"�L�k�R�U�Ҧ���"+product->query_idname()+"�C\n");
				else
					amount = pamount;
			}
			else if( pamount != "-1" && count(amount,">",pamount) )
				return tell(me, "�o�̨èS���c��o��h"+(query("unit", product)||"��")+product->query_idname()+"�C\n");
			else if( count(amount, "<", 1) )
				return tell(me, "�п�J���T���ʶR�ƶq�C\n");
			
			foreach(string shelf, array data in products)
				if( member_array(basename, data) != -1 && !shelf )
					return tell(me, product->query_idname()+"�ثe�ȨѮi�ܡC\n");
			
			cost = count(amount,"*",copy(query("setup/price/"+replace_string(basename, "/", "#"), env)||query("value", product)));
			earn = cost;

			if( delivery_master )
			{
				// �����B�e���B�~���O 1%
				if( query("function", env) == "lobby" )
				{
					if( count(cost, "/", 1000) == "0" )
						cost = count(cost, "+", 1*(100-price_skill_level));
					else
						cost = count(cost, "+", count((100-price_skill_level), "*", count(cost, "/", 1000)));
				}
				// �ƶq�W�L�i�W�[�ƶq
				if( over_capacity(delivery_master, database, amount, capacity) )
					return tell(me, delivery->query_room_name()+"�L�k�A�e�ǳo��h�����~�F�C\n");
			}
						
			if( !me->spend_money(MONEY_D->city_to_money_unit(env_city), cost) )
				return tell(me, pnoun(2,me)+"���W�� $"+MONEY_D->city_to_money_unit(env_city)+" �������F�I�I\n");
				
			if( delivery_master )
			{
				msg("$ME��F"HIY+QUANTITY_D->money_info(env_city, cost)+NOR" �ʶR�F"+QUANTITY_D->obj(product, amount)+"�A�ä�I "+sprintf("%.1f", (100-price_skill_level)/10.)+"% ���B�O�����e�F"+query("short", delivery)+loc_short(loc)+"�C\n",me,0,1);
				log_file("command/buy", me->query_idname()+"��F"HIY+QUANTITY_D->money_info(env_city, cost)+NOR" �ʶR�F"+QUANTITY_D->obj(product, amount)+"�A�ä�I "+sprintf("%.1f", (100-price_skill_level)/10.)+"% ���B�O�����e�F"+query("short", delivery)+loc_short(loc)+"�C");

				if( query("function", delivery_master) == "storeroom" )
					input_object(delivery_master, "storeroom", basename, amount);
				else
					input_object(delivery_master, database, basename, amount);
					
				delivery_master->save();
			}
			else
			{
				newproduct = new(basename);
				
				if( !query("flag/no_amount", newproduct) )
					set_temp("amount", amount, newproduct);
	
				if( !me->get_object(newproduct, amount) )
				{
					me->earn_money(MONEY_D->city_to_money_unit(env_city), cost);
					tell(me, pnoun(2, me)+"���W�����~�Ӧh�Τӭ��L�k�A����h�F��F�C\n");
					return;
				}
				
				msg("$ME��F"HIY+QUANTITY_D->money_info(env_city, cost)+NOR" �ʶR�F"+QUANTITY_D->obj(product, amount)+"�C\n",me,0,1);
				//log_file("command/buy", me->query_idname()+"��F"HIY+QUANTITY_D->money_info(env_city, cost)+NOR" �ʶR�F"+QUANTITY_D->obj(product, amount)+"�C");
			}

			switch(buyer)
			{
				case "GOVERNMENT":
					break;
				case "ENVIRONMENT":
					set("money", count(copy(query("money", env)), "+", earn), env);
					break;
			}

			if( pamount != "-1" )
				output_object(env, database, basename, amount);

			if( !userp(me) && product->is_module_object() && !SECURE_D->is_wizard(query("produce/designer", product)) )
			{
				set("sells", count(query("sells", product), "+", amount), product);
				
				//hmm
				product->save();
				TOP_D->update_top("product",
					query("produce/time", product) + product->query_id(1),
					query("sells", product),
					product->query_idname(),
					query("value", product),
					query("produce/designer", product),
					query("produce/factory", product));

				// TOP_D->set_top("product", ({ query("produce/time", product)+product->query_id(1), query("sells", product), product->query_idname(), query("value", product), query("produce/designer", product), query("produce/factory", product) }), 30);
			}
				
			return;
		}
	}
	
	return tell(me, "�o�̨èS���c�� "+productname+" �o�ذӫ~�C\n");
}