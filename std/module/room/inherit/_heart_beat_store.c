/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _heart_beat_store.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-28
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <map.h>
#include <location.h>
#include <npc.h>

void output_object(object env, string database, string basename, string amount);
void refresh_class(object env, string database);

void virtual_shopping(object master)
{
	int i, size, average_desire, productdesire, num, security, management, ratio, totalrooms;
	string city, cost, replace_file_name, shoppingmsg="";
	object product, *msgrooms, slave, boss;
	mapping products;
	mixed value, amount;

	if( query("mode", master) )
		return;
	
	products = query("products", master);
	
	if( !mapp(products) || !sizeof(products) )
		return;

	city = master->query_city();
	num = master->query_city_num();

	msgrooms = allocate(0);
	security = query("shopping/security", master);
	management = query("shopping/management", master);
	
	foreach( string file in (master->query_slave() || allocate(0)) + ({ base_name(master) }) )
	{
		slave = find_object(file);
		
		if( !objectp(slave) ) continue;
		
		if( sizeof(filter_array(all_inventory(slave), (: userp($1) :))) )
			msgrooms += ({ slave });
		
		average_desire += query("shopping/desire", slave);
	}

	totalrooms = sizeof(master->query_slave())+1;
	average_desire /= totalrooms;
	set("shopping/average_desire", average_desire, master);
	
	boss = find_player(query("owner", master));

	ratio = SHOPPING_D->shopping_cost_ratio(city, num);

	foreach( string shelf, string *data in products )
	{	
		size = sizeof(data);
		
		if( !shelf ) continue;
			
		for(i=0;i<size;i+=2)
		{
			if( random(ratio) ) continue;

			replace_file_name = replace_string(data[i], "/", "#");
			
			productdesire = average_desire + query("shopping/productdesire/"+replace_file_name, master);
			
			if( productdesire > random(180) )
			{				
				if( catch(product = load_object(data[i])) )
					continue;
		
				if( !objectp(product) && !file_exists(data[i]) )
				{
					output_object(master, "products", data[i], "all");
					continue;
				}

				value = ""+(query("setup/price/"+replace_file_name, master) || query("value", product));
				
				// �ʶR���M����v�T�ƶq
				amount = to_int(ceil( pow(productdesire, 0.45) * pow(totalrooms, 0.3) / pow(sizeof(value), 0.6) ));

				if( ratio > 1 )
					amount /= pow(ratio, 0.7);

				amount = random(to_int(amount)+1);

				if( amount <= 0 ) continue;

				if( count( amount, ">", data[i+1]) )
					amount = data[i+1];

				cost = count(amount, "*", value);
				
				// ���O��X�����F
				if( count(SHOPPING_D->query_shopping_info(city, num, "money"), "<", cost) )
					continue;
					
				output_object(master, "products", data[i], amount);
				
				if( security < totalrooms && !random(security) )
				{
					broadcast(master, HIY"�[�l�W��"+product->query_idname()+HIY"���G�����֤F�@��...�C\n"NOR);
					
					if( boss )
						tell(boss, master->query_room_name()+"�ǨӮ����G���G���@�Ǥp�����b���[�l�W���ӫ~...�C\n");
					
					continue;
				}
				
				set("money", count(query("money", master), "+", cost), master);
				set("totalsell", count(amount, "+", query("totalsell", master)), master);
	
				if( sizeof(msgrooms) )
					shoppingmsg += "�����N"+QUANTITY_D->obj(product, amount)+"�浹�F�U�ȡA�æ���"+HIY+QUANTITY_D->money_info(city, cost)+NOR"�C\n";
				
				SHOPPING_D->shopping_cost_money(city, num, cost);
			}
		}
	}
	
	if( sizeof(msgrooms) )
	{
		foreach( slave in msgrooms )
		{
			broadcast(slave, shoppingmsg);
			
			if( !random(3) )
			switch(average_desire)
			{
				case 1..30:
					broadcast(slave, "�}�}�������U�Ȧb���̿��ʵ۰ӫ~...�C\n"); break;
				case 31..60:
					broadcast(slave, "�U�ȭ̻��P�a�s���ۦU�ذӫ~...�C\n"); break;
				case 61..100:
					broadcast(slave, "�U�ȭ̥��Y���զa�Q�׵ۦU�ذӫ~...�C\n"); break;
				case 101..200:
					broadcast(slave, "���x�����̥R�����U�ȭ̪��w���n...�C\n"); break;
				case 201..300:
					broadcast(slave, "�j�q���U�ȭ̿��Ħa�ɨ����ʪ����ֽ�...�C\n"); break;
				case 301..99999:
					broadcast(slave, "���̨��D�W�����F�D�W�ӨӪ��U�ȡA���x�D�Z...�C\n"); break;
			}
		}
	}
	
	
	
	if( security < totalrooms && !random(10) )
	{
		if( boss )
			tell(boss, master->query_room_name()+"�����u���X���i�G�ө������ޫO��("+security+")�C��ثe���s������ƶq("+totalrooms+")�A�N���ӫ~�D�Ѫ��ü{...�C\n");
	}
	
	if( management < totalrooms && !random(10) )
	{
		if( boss )
			tell(boss, master->query_room_name()+"�����u���X���i�G�ө����޲z�W��("+management+")�C��ثe���s������ƶq("+totalrooms+")�A�y�������c�a�ת��l��(-"+totalrooms+")...�C\n");
	}
}


// �p�ⲣ�~���ʶR���A�ȻP���~�ۨ�����ίS�ʦ����A�P�ؿv���L��
varargs void calculate_master_data(object master, string spec_product)
{
	int i, size, desire, security, productdesire, valuepercent, valuesize, management;
	string popproduct;
	object product, inv;
	mapping products;
	mixed value, setvalue;
	string owner = query("owner", master);

	popproduct = SHOPPING_D->query_pop_product();

	products = query("products", master);

	if( mapp(products) )
	foreach( string shelf, string *data in products )
	{	
		size = sizeof(data);
		
		for(i=0;i<size;i+=2)
		{
			if( spec_product && data[i] != spec_product ) continue;
			
			productdesire = 0;

			switch(shelf)
			{
				case "�P�P": productdesire += 60; break;
				case "�i��": continue; break;
				default: break;
			}
					
			if( data[i] == popproduct )
				productdesire += 40;
			else
				SHOPPING_D->add_product(data[i]);
				
			if( catch(product = load_object(data[i])) )
				continue;

			if( !objectp(product) && !file_exists(data[i]) )
			{
				output_object(master, "products", data[i], "all");
				continue;
			}

			value = query("value", product);
			
			if( count(value, "<=", 0) )
			{
				set("shopping/productdesire/"+replace_string(data[i], "/", "#"), -10000,  master);
				continue;
			}

			setvalue = query("setup/price/"+replace_string(data[i], "/", "#"), master)||value;
			
			valuepercent = to_int(count(count(count(value, "-", setvalue), "*", 100), "/", value));
			
			if( valuepercent < -10000 ) valuepercent = -10000;

			// ���檺�ӫ~
			if( query("badsell", product) && (count(setvalue, ">", value) || count(setvalue, ">", 10000000)) )
				productdesire = -9999;

			// ����v�T�ʶR��
			productdesire += 1.5*valuepercent;
			
			// ����U�Q, �ʶR���e 3 ����U��
			valuesize = sizeof(setvalue+"");
			
			if( valuesize > 2 )
				productdesire -= pow(valuesize-2, 3.0);

			// ���a�Ͳ����~
			if( product->is_module_object() )
				productdesire += 30;

			// �H���R��i��
			productdesire += range_random(-5, 5);

			set("shopping/productdesire/"+replace_string(data[i], "/", "#"), productdesire,  master);
		}
	}
	
	// �����p��
	foreach( inv in all_inventory(master) )
	{
		if( query("job/cur", inv) == CLERK && query("boss", inv) == owner )
		{
			desire += inv->query_skill_level("eloquence")/10 + inv->query_cha();
			security += inv->query_skill_level("security");
			management += inv->query_skill_level("storemanage");
		}
	}
	
	set("shopping/basic_desire", desire/100, master);
	set("shopping/security", security/6, master);
	set("shopping/management", management/5, master);
}


// �p��ؿv���R��A�P���~�L��
void calculate_shopping_desire(object room)
{
	int desire, floor;
	array loc;
	object ownerob, master, broom;
	string roomlong, ownerid;
	mapping products;

	master = room->query_master();
	
	products = query("products", master);
	
	if( !mapp(products) || !sizeof(products) || BUILDING_D->is_building_room_locked(room) )
	{
		delete("shopping/desire", room);
		return;
	}

	// �Ӽh���׼v�T
	if( query("firstfloor", room) && objectp(broom = load_object(query("firstfloor", room))) )
		floor = query("floor", broom);
	else
		floor = query("floor", room);

	desire += floor + pow(2, (floor / 20.));

	loc = room->query_location();
	
	// �̷Ӱϰ첣�ͤ��P���ʶR����
	switch( CITY_D->query_coor_data(loc, "region") )
	{
		// �ӷ~��
		case COMMERCE_REGION:
			desire += 20; break;
		// �A�~��
		case AGRICULTURE_REGION:
			desire += 12; break;
		// �u�~��
		case INDUSTRY_REGION:
			desire += 3; break;
	}
	
	roomlong = query("long", room);
	
	// ���ԭz
	if( stringp(roomlong) && strsrch(roomlong, "�o�����") == -1 && strlen(roomlong) > 100 )
		desire += 5;
	
	// �۵P
	if( strlen(CITY_D->query_coor_icon(loc)) > 2 )
		desire += 3;
		
	// �Y����b�u�W, �̷Ӧ���y�O�W�[�ʶR��
	ownerid = query("owner", room);
	if( stringp(ownerid) && objectp(ownerob = find_player(ownerid)) )
		desire += ownerob->query_cha();
	
	// ���@��ө��K���קΦ��Ӱ髬�A�W�[�ʶR��
	/*
	foreach(string sloc, string type in CITY_D->query_coor_range(loc, ROOM, 1))
	{
		loc = restore_variable(sloc);
		
		if( type == "store" && objectp(broom = find_object("/city/"+loc[CITY]+"/"+loc[NUM]+"/room/"+loc[X]+"_"+loc[Y]+"_"+type)) )
			desire += 3*(query("floor", broom)/10) || 1;
	}
	*/

	set("shopping/desire", desire+query("shopping/basic_desire", master), room);
}


//��s�P��ƦW
void update_top_totalsell(object room)
{
	string id = query("owner", room);
	
	if( SECURE_D->is_wizard(id) ) return;

	TOP_D->update_top("storesell", base_name(room), query("totalsell", room), id, room->query_room_name(), room->query_city());
}

// �߸� (�C 5 ��@��)
void heart_beat(object room)
{
	string *slaves = room->query_slave() || allocate(0);
	int store_heart_beat;
	
	if( query("function", room) != "front" ) return;

	// �p�G SHOPPING_D �S�����J, �h return;
	if( !find_object(SHOPPING_D) ) return;
	
	if( !(store_heart_beat = query_temp("store_heart_beat", room)) )
		store_heart_beat = set_temp("store_heart_beat", random(1200)+1, room);
	else
		addn_temp("store_heart_beat", 1, room);

	// ���O�欰
	if( find_player(query("owner", room)) )
		virtual_shopping(room);
	else if( !(store_heart_beat % 3) )
		virtual_shopping(room);
	
	// 5 �����p��@���ӫ~�ʶR��
	if( !(store_heart_beat%61) )
		calculate_master_data(room);

	// 10 �����p��@���R��
	if( !(store_heart_beat%121) )
		calculate_shopping_desire(room);
	
	// 45 ������s�@�� TOP
	if( !(store_heart_beat%541) )
		update_top_totalsell(room);
	
	// 30 �����x�s�@��
	if( !(store_heart_beat%361) )
		room->save();

	foreach(string file in slaves)
	{
		reset_eval_cost();

		if( !objectp(room = load_object(file)) ) continue;

		if( !(store_heart_beat = query_temp("store_heart_beat", room)) )
			store_heart_beat = set_temp("store_heart_beat", random(10000)+1, room);
		else
			addn_temp("store_heart_beat", 1, room);
			
		// 10 �����p��@���R��
		if( !(store_heart_beat%122) )
			calculate_shopping_desire(room);
	
		// 120 �����x�s�@��
		if( !(store_heart_beat%1441) )
			room->save();
	}

	
}
