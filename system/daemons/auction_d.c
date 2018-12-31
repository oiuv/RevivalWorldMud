/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : auction_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-11
 * Note   : �����F
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
		msg = HIB"�ثe���@�ɥ��b��檺�ӫ~�p�U�G\n"NOR;
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg +=    "�s�� ���~                                  �ƶq    �ثe�̰��X�� �X����  �Ѿl�ɶ�\n"NOR;
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
		foreach(data in auction)
		{
			ob = load_object(data["basename"]);
			
			if( !objectp(ob) ) continue;
			
			top_price = data["top_price"];
			top_buyer = data["top_buyer"];
		
			if( data["status"] == 1 )
				status = HIR"����"NOR;
			else if( data["status"] == 2 )
				status = HIG"�y��"NOR;
			else if( data["status"] == 3 )
				status = HIY"��X"NOR;
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
		
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += "list '�s��' �[�ݸө��~���Բө����\n\n";
		
		return me->more(msg);
	}
	
	i = to_int(arg);
	
	if( i < 1 || i > sizeof(auction) )
		return tell(me, "�п�J���T���s���C\n");
		
	data = auction[i-1];
	
	ob = load_object(data["basename"]);
	
	if( !objectp(ob) )
		error(data["basename"]+" ������J����");
	
	if( data["status"] == 1 )
		status = HIR"����"NOR;
	else if( data["status"] == 2 )
		status = HIG"�y��"NOR;
	else if( data["status"] == 3 )
		status = HIY"��X"NOR;
	else
		status = 0;
				
	msg =  "�s�� "+i+" �����~��Ʀp�U�G\n";
	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
	msg += "����    �G"+capitalize(data["seller"])+"\n";
	msg += "���~    �G"+ob->query_idname()+"\n";
	msg += "�ƶq      �G"+NUMBER_D->number_symbol(data["amount"])+"\n";
	msg += "��橳��  �G$"+MONEY_D->query_default_money_unit()+" "+NUMBER_D->number_symbol(data["base_price"])+"\n";
	msg += "�����ʶR���G$"+MONEY_D->query_default_money_unit()+" "+NUMBER_D->number_symbol(data["direct_price"])+"\n";
	msg += "�Ѿl�ɶ�  �G"+(status || (data["minute"]+" ����"))+"\n";
	
	if( data["finish_time"] > 0 )
	{
		if( data["status"] == 2 )
			msg += "�t�Φ^��  �G�����~�N�� "+TIME_D->replace_ctime(data["finish_time"]+14*24*60*60)+" �Ѩt�Φ۰ʧR��\n";
		else
			msg += "���̻�^�G�o�Ъ̥��� "+TIME_D->replace_ctime(data["finish_time"]+7*24*60*60)+" ���e������~�̡A�N�}����̻�^�C\n";
	}

	msg += "�v�Ъ��p  �G\n";
	
	if( !sizeof(data["auction_data"]) )
		msg += "    �L�H�X��\n";
	else foreach(id in sort_array(keys(data["auction_data"]), (: count( $(data)["auction_data"][$1], "<", $(data)["auction_data"][$2] ) ? 1 : -1 :)))
		msg += sprintf("    %-16s �ثe�X�� %s\n", capitalize(id), money(money_unit, data["auction_data"][id]));
	
	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
	
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
		tell(me, pnoun(2, me)+"�M�w���v�гo�ө��~�C\n");
		return me->finish_input();
	}

	num = i+1;

	// ��Ƥ��P
	if( !mapp(auction[i]) || 
		auction[i]["status"] != data["status"] ||
		auction[i]["basename"] != data["basename"] ||
		auction[i]["amount"] != data["amount"] ||
		auction[i]["seller"] != data["seller"] ||
		auction[i]["base_price"] != data["base_price"]
	)
	{
		tell(me, "���~�s�����Ǥw�g���ܡA�Э��s�v�СC\n");
		return me->finish_input();
	}

	if( auction[i]["status"] == 1 )
	{
		tell(me, "�o�����~�w�g���СC\n");
		return me->finish_input();
	}
	else if( auction[i]["status"] == 2 )
	{
		tell(me, "�o�����~�w�g�y�СC\n");
		return me->finish_input();
	}
	else if( auction[i]["status"] == 3 )
	{
		tell(me, "�o�����~�w�g�Q�����ʶR�C\n");
		return me->finish_input();
	}

	if( !sizeof(auction[i]["auction_data"]) )
	{
		if( count(price, "<", auction[i]["base_price"]) )
		{
			tell(me, "�v���������������橳��("+money(money_unit, auction[i]["base_price"])+")�C\n");
			return me->finish_input();
		}

		if( !me->spend_money(money_unit, price, BANK_PAYMENT) )
		{
			tell(me, pnoun(2, me)+"�������� "+money(money_unit, price)+"�C\n");
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
				
			CHANNEL_D->channel_broadcast("auc", HIY"�����ʶR�G"NOR+me->query_idname()+"��� "+num+" �s�������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�ۥX "HIY+money(money_unit, price)+NOR" �������ʶR���A��浲�СC\n");	

			if( objectp(seller = find_player(auction[i]["seller"])) )
				tell(seller, me->query_idname()+"�N "HIY+money(money_unit, price)+NOR" ú�I��"+pnoun(2, seller)+"�C\n");
			
			me->finish_input();
			return;	
		}

		CHANNEL_D->channel_broadcast("auc", HIC"�X���G"NOR+me->query_idname()+"��� "+num+" �s�������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�ۥX "HIY+money(money_unit, price)+NOR" ���v�л��C\n");
		
		save();

		return me->finish_input();
	}
	else
	{
		top_buyer = auction[i]["top_buyer"];
		top_price = auction[i]["top_price"];

		// �����ʶR
		if( count(price, ">=", auction[i]["direct_price"]) )
		{
			if( !me->spend_money(money_unit, price, BANK_PAYMENT) )
			{
				tell(me, pnoun(2, me)+"�������� "+money(money_unit, price)+"�C\n");
				return me->finish_input();
			}

			// �N�W�@���v�Ъ̪����k��
			if( top_buyer )
				MONEY_D->earn_money(top_buyer, money_unit, top_price);

			auction[i]["status"] = 3;
			auction[i]["minute"] = 0;
			auction[i]["auction_data"][me->query_id(1)] = price;
			auction[i]["finish_time"] = time();

			refresh_top_buyer(i);
			
			MONEY_D->earn_money(auction[i]["seller"], money_unit, price);

			CHANNEL_D->channel_broadcast("auc", HIY"�����ʶR�G"NOR+me->query_idname()+"��� "+num+" �s�������~�u"+ob->query_idname()+"�v�ۥX "HIY+money(money_unit, price)+NOR" �������ʶR���A��浲�СC\n");	
			
			if( objectp(seller = find_player(auction[i]["seller"])) )
				tell(seller, me->query_idname()+"�N "HIY+money(money_unit, price)+NOR" ú�I��"+pnoun(2, seller)+"�C\n");

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
				tell(me, "�C���v�Ъ��t�Z�����O�ثe�̰��v�л� + 5% �������H�W(�t�έp�⵲�G���G"+money(money_unit, next_price)+")\n");
				return me->finish_input();
			}
			
			if( count(price, ">", count(top_price, "*", 5)) )
			{
				tell(me, "�v�л����o����{���̰��X���� 5 ���C\n");
				return me->finish_input();
			}
	
			if( !me->spend_money(money_unit, price, BANK_PAYMENT) )
			{	
				tell(me, pnoun(2, me)+"�������� "+money(money_unit, price)+"�C\n");
				return me->finish_input();
			}

			// �N�W�@���v�Ъ̪����k��
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
				
				CHANNEL_D->channel_broadcast("auc", HIY"�����ʶR�G"NOR+me->query_idname()+"��� "+num+" �s�������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�ۥX "HIY+money(money_unit, price)+NOR" �������ʶR���A��浲�СC\n");	
				
				if( objectp(seller = find_player(auction[i]["seller"])) )
					tell(seller, me->query_idname()+"�N "HIY+money(money_unit, price)+NOR" ú�I��"+pnoun(2, seller)+"�C\n");

				return me->finish_input();
			}
	
			auction[i]["auction_data"][me->query_id(1)] = price;			
			
			refresh_top_buyer(i);

			CHANNEL_D->channel_broadcast("auc", HIC"�X���G"NOR+me->query_idname()+"��� "+num+" �s�������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�ۥX "HIY+money(money_unit, price)+NOR" ���v�л��C\n");	
			
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
		return tell(me, "�п�J���T���v�и��(help bid)�C\n");
		
	if( num < 1 || num > sizeof(auction) )
		return tell(me, "�п�J���T���s���C\n");

	i = num - 1;
	
	if( auction[i]["status"] == 1 )
		return tell(me, "�o�����~�w�g���СC\n");
	else if( auction[i]["status"] == 2 )
		return tell(me, "�o�����~�w�g�y�СC\n");
	else if( auction[i]["status"] == 3 )
		return tell(me, "�o�����~�w�g�Q�����ʶR�C\n");

	ob = load_object(auction[i]["basename"]);
	
	if( !objectp(ob) )
		error(auction[i]["basename"]+" ������J���~�C\n");

	if( price == "cancel" )
	{
		if( auction[i]["seller"] != me->query_id(1) )
			return tell(me, "�u������̤~��������C\n");

		if( auction[i]["status"] )
			return tell(me, "�����~�w�g���Щάy�ЩγQ�����ʶR�F�A�L�k�A�������C\n");

		if( sizeof(auction[i]["auction_data"]) )
			return tell(me, "�w�g���H�ѻP�v�СA�L�k�A�������F�C\n");

		auction[i]["status"] = 2;
		auction[i]["minute"] = 0;
		auction[i]["finish_time"] = time();
		auction[i]["auction_data"] = allocate_mapping(0);
		auction[i]["top_buyer"] = 0;
		auction[i]["top_price"] = 0;

		CHANNEL_D->channel_broadcast("auc", HIG"�������G"NOR+me->query_idname()+"������ "+num+" �s�����~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�����C\n");
		
		return;
	}

	if( me->query_id(1) == auction[i]["seller"] )
		return tell(me, "���̤���ѻP�v�СC\n");

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
		return tell(me, "�п�J���T���v�л�(1-1000000000000)�C\n");	

	tell(me, "�O�_�T�w�H "HIY+money(money_unit, price)+NOR" �������v�Сu"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v(y/n)�H");
	
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
		return tell(me, "�п�J���T�����O(help receive)�C\n");

	// �B���Y�Ӧa�I
	if( sscanf(arg, "%d to %d %d,%d", i, num, x, y) == 4 )
	{
		if( i < 1 || i > sizeof(auction) )
			return tell(me, "�п�J���T���s���C\n");

		i--;

		if( !auction[i]["status"] )
			return tell(me, "�������~�٦b�v�е{�Ǥ��C\n");

		if( auction[i]["status"] == 2 && me->query_id(1) != auction[i]["seller"] )
			return tell(me, "�y�Ъ��~�����ѩ��̻�^�C\n");

		ob = load_object(auction[i]["basename"]);
		
		if( !objectp(ob) )
			error(auction[i]["basename"]+" ��Ƹ��J���~�C\n");

		if( auction[i]["status"] != 2 && auction[i]["top_buyer"] != me->query_id(1) )
			return tell(me, pnoun(2, me)+"�èS���o�гo�ө��~�C\n");

		if( !CITY_D->city_exist(city, num-1) )
			return tell(me, "�o�y�����èS���� "+num+" �ìP�����C\n");
	
		loc = arrange_city_location(x-1, y-1, city, num-1);

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
			case "warehouse":
				break;
			default:
				return tell(me, delivery->query_room_name()+"�S����k��������ӫ~�C\n");
		}
		
		if( delivery->query_module_file()->over_capacity(delivery_master, "products", auction[i]["amount"], capacity) )
			return tell(me, delivery->query_room_name()+"�L�k�A�e�ǳo��h�����~�F�C\n");

		if( auction[i]["status"] != 2 )
			msg("$ME�N�o�Ъ��~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�B���"+query("short", delivery)+loc_short(loc)+"�C\n", me, 0, 1);	
		else
			msg("$ME�N�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v��^�ùB���"+query("short", delivery)+loc_short(loc)+"�C\n", me, 0, 1);
			
		delivery->query_module_file()->input_object(delivery_master, "products", auction[i]["basename"], auction[i]["amount"]);
		remove_auction(i);
		
		delivery_master->save();
	}
	else if( big_number_check(arg) && sscanf(arg, "%d", i) == 1 )
	{
		if( i < 1 || i > sizeof(auction) )
			return tell(me, "�п�J���T���s���C\n");
		
		i--;

		if( !auction[i]["status"] )
			return tell(me, "�������~�٦b�v�е{�Ǥ��C\n");

		if( auction[i]["status"] == 2 && me->query_id(1) != auction[i]["seller"] )
			return tell(me, "�y�Ъ��~�����ѩ��̻�^�C\n");
			
		
		ob = load_object(auction[i]["basename"]);
		
		if( !objectp(ob) )
			error(auction[i]["basename"]+" ��Ƹ��J���~�C\n");
	
		if( auction[i]["status"] != 2 && auction[i]["top_buyer"] != me->query_id(1) )
			return tell(me, pnoun(2, me)+"�èS���o�гo�ө��~�C\n");

		cloneob = new(auction[i]["basename"]);
		
		if( !me->available_get(cloneob, auction[i]["amount"]) )
		{
			destruct(cloneob);
			return tell(me, pnoun(2, me)+"�����ʳo��h�F��C\n");
		}
			
		if( auction[i]["status"] != 2 )
			msg("$ME�N�o�Ъ��~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v����쨭�W�C\n", me, 0, 1);
		else
			msg("$ME�N�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v��^�쨭�W�C\n", me, 0, 1);
				
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
		return tell(me, "�п�J���T�����f���O(help receive)�C\n");
}

void heart_beat()
{
	int size = sizeof(auction);
	object ob;
	object seller;
	string money_unit = MONEY_D->query_default_money_unit();

	for(int i=0;i<size;i++)
	{
		// �W�L 14 �ѥ���������~�Ѩt�ΧR��
		if( auction[i]["finish_time"] > 0 && time() > auction[i]["finish_time"] + 14*24*60*60 )
		{
			auction[i] = 0;
		}
		// �W�L 7 �ѥ���������~�ŧi�y��
		else if( (auction[i]["status"] == 1 || auction[i]["status"] == 3) && auction[i]["finish_time"] > 0 && time() > auction[i]["finish_time"] + 7*24*60*60 )
		{
			auction[i]["status"] = 2;
			auction[i]["finish_time"] = time();
			
			if( objectp(seller = find_player(auction[i]["seller"])) )
				tell(seller, pnoun(2, seller)+"�����~�W�L 7 �ѵL�H����A�}����̻�^�C\n");
		}
		else if( auction[i]["minute"] && !(--auction[i]["minute"]) )
		{
			ob = load_object(auction[i]["basename"]);
			
			if( !objectp(ob) ) continue;

			if( !sizeof(auction[i]["auction_data"]) )
			{
				auction[i]["status"] = 2;
				auction[i]["finish_time"] = time();

				CHANNEL_D->channel_broadcast("auc", HIG"�y�СG"NOR+"�s�� "+(i+1)+" �������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�ŧi�y�СC\n");
				
				if( objectp(seller = find_player(auction[i]["seller"])) )
					tell(seller, pnoun(2, seller)+"�����~�w�g�y�СA�Цۦ��^�C\n");
			}
			else
			{
				refresh_top_buyer(i);
				
				MONEY_D->earn_money(auction[i]["seller"], money_unit, auction[i]["top_price"]);

				auction[i]["status"] = 1;
				auction[i]["finish_time"] = time();

				CHANNEL_D->channel_broadcast("auc", HIR"���СG"NOR+"�s�� "+(i+1)+" �������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�� "+capitalize(auction[i]["top_buyer"])+" �H "HIY+money(money_unit, auction[i]["top_price"])+NOR" ����бo�C\n");
				
				if( objectp(seller = find_player(auction[i]["seller"])) )
					tell(seller, pnoun(2, seller)+"�����~���\���СA���� "HIY+money(money_unit, auction[i]["top_price"])+NOR"�C\n");
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
	return "���t��(AUCTION_D)";
}
