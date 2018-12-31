/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _action_transport.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-29
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

// �B�骫�~
void do_transport(object me, string arg, string database)
{
	int i, which = 1, size, capacity;
	object master, product, delivery, delivery_master;
	mapping products;
	string tmp, amount = "1", env_city, cost, productname;
	string delivery_owner;
	int delivery_x, delivery_y, num;
	string *productlist, pamount, basename;
	array loc;
	
	master = environment(me)->query_master();

	env_city = master->query_city();

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�B�餰�򪫫~�H\n");

	if( query("owner", master) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"���O���B���֦��̡C\n");

	if( sscanf(arg, "%s to %d %d,%d", arg, num, delivery_x, delivery_y) != 4 )
		return tell(me, "�п�J���T���B��ت��a��m�C\n");

	if( !CITY_D->city_exist(env_city, num-1) )
		return tell(me, "�o�y�����èS���� "+num+" �ìP�����C\n");

	loc = arrange_city_location(delivery_x-1, delivery_y-1, env_city, num-1);
	
	delivery = load_module(loc);
	
	if( !objectp(delivery) )
		return tell(me, "�y��"+loc_short(loc)+"�B�èS������ؿv���C\n");
		
	delivery_owner = query("owner", delivery);
	
	if( delivery_owner != me->query_id(1) )
	{
		if( ENTERPRISE_D->same_enterprise(delivery_owner, me->query_id(1)) )
			tell(me, delivery->query_room_name()+"�����~���κX�U���~�A�֦��̬� "+capitalize(delivery_owner)+"�A���\�����B��C\n");
		else
			return tell(me, delivery->query_room_name()+"�ä��O"+pnoun(2, me)+"���ؿv���C\n");
	}
	
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
			return tell(me, delivery->query_room_name()+"�S����k�������󪫫~�C\n");
	}
	
	switch( query("function", delivery) )
	{
		case "front":
		case "warehouse":
			break;
		default:
			return tell(me, delivery->query_room_name()+"�S����k�������󪫫~�C\n");
	}
	
	if( delivery_master == master )
		return tell(me, pnoun(2, me)+"���B�e�����~�a�I����P���a�ۦP�Φ��ۦP�s�ꤤ�ߡC\n");

		
	sscanf(arg, "%s %s", amount, productname);
	
	if( !productname || (amount != "all" && !(amount = big_number_check(amount))) )
	{
		productname = arg;
		amount = "1";
	}
	
	if( sscanf( productname, "%s %d", tmp, which ) == 2 )
		productname = tmp;

	if( which < 1 ) which = 1;

	products = query(database, master);
	
	if( !sizeof(products) )
		return tell(me, query("short", master)+"�ثe�S�����󪫫~�C\n");

	productlist = implode(values(products), (:$1+$2:))||allocate(0);
	size = sizeof(productlist);

	for(i=0;i<size;i+=2)
	{
		basename = productlist[i];
		pamount = productlist[i+1];

		catch(product = load_object(basename));

		// �Y���~�w�g����
		if( !objectp(product) )
		{
			error(basename+" �ӫ~��ƿ��~�C");
			continue;
		}

		if( (i+2)/2 == to_int(big_number_check(productname)) || (product->query_id(1) == lower_case(productname) && !(--which)) )
		{
			if( amount == "all" )
				amount = pamount;
			else if( count(amount,">",pamount) )
				return tell(me, "�o�̨èS���o��h"+(query("unit", product)||"��")+product->query_idname()+"�C\n");
			else if( count(amount, "<", 1) )
				return tell(me, "�п�J���T���B��ƶq�C\n");
			
			
			cost = count(amount,"*",copy(query("setup/price/"+replace_string(basename, "/", "#"), master)||query("value", product)));
			
			// �ƶq�W�L�i�W�[�ƶq
			if( over_capacity(delivery_master, database, amount, capacity) )
				return tell(me, delivery->query_room_name()+"�L�k�A�e�ǳo��h�����~�F�C\n");
				
			input_object(delivery_master, database, basename, amount);

			output_object(master, database, basename, amount);
			
			if( find_player(delivery_owner) == me )
				msg("$ME�N"+QUANTITY_D->obj(product, amount)+"�B���"+delivery->query_room_name()+"�C\n", me, 0, 1);
			else
				msg("$ME�N"+QUANTITY_D->obj(product, amount)+"�B���"+delivery->query_room_name()+"�C\n", me, find_player(delivery_owner), 1);
			
			master->save();
			delivery->save();
			return;
		}
	}
	
	return tell(me, "�o�̨èS�� "+productname+" �o�ت��~�C\n");
}