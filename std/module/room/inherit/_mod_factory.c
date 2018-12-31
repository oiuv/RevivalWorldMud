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

// ���o���u�t�i�H�Ͳ������~ module ����}�C
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
		return tell(me, pnoun(2, me)+"���O�o���u�t���֦��̡C\n");

	if( !objectp(research) )
		return tell(me, "�|���]�w�����o���ߡC\n");
	
	if( !objectp(warehouse) )
		return tell(me, "�|���]�w������x���ߡC\n");
	
	if( !arg || !arg[0] )
		return tell(me, pnoun(2, me)+"�Q�n������˪��Ͳ��ʧ@�H('���~�s��', start, stop)\n");

	arg = lower_case(arg);

	productnum = to_int(arg);
	
	if( productnum > 0 )
	{
		if( query("line/working", master) )
			return tell(me, "�Ͳ��u���b�B�@���A�����������~����Ͳ����~�����C\n");
		
		productslist = query("productslist", research);
		
		if( productnum > sizeof(productslist) )
			return tell(me, "�S���s�� "+productnum+" �����~�C\n");
		
		catch( product = load_object(productslist[productnum-1]) );
		
		if( !objectp(product) )
			return tell(me, "�L�k���J "+productslist[productnum-1]+" �o�Ӳ��~�I�гq���Ův�B�z�C\n");
	
		if( member_array(product->query_module(), query_available_products(master)) == -1 )
			return tell(me, "�o���u�t�L�k�Ͳ��o�ز��~�C\n");
		
		set("product", base_name(product), master);
		master->save();	
		msg("$ME�N���Ͳ��u���s�y���~�]�w���u"+product->query_idname()+"�v�C\n", me, 0, 1);
	}
	else if( arg == "start" )
	{
		if( !query("product", master) )
			return tell(me, "�|���]�w���s�y�����~�A�L�k�}�l�Ͳ��C\n");

		catch( product = load_object(query("product", master)) );
		
		if( !objectp(product) )
			return tell(me, "�L�k���J "+query("product", master)+" �o�Ӳ��~�I�гq���Ův�B�z�C\n");

		set("line/working", 1, master);
		master->save();
		msg("$ME�ҰʤF�Ͳ��u�W�Ҧ����]�ơA�}�l�Ͳ�"+product->query_idname()+"�C\n", me, 0, 1);
		
	}
	else if( arg == "stop" )
	{
		delete("line", master);
		master->save();
		msg("$ME�N�Ͳ��u�W�Ҧ����]�������A����Ͳ����~�C\n", me, 0, 1);
	}

	else tell(me, "�п�J���T�����O�榡�C(help line)\n", me, 0, 1);
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
		return tell(me, "�|���]�w�����o���ߡC\n");
	
	productslist = query("productslist", research);
	
	if( !sizeof(productslist) )
		return tell(me, "�o�Ӭ�o���ߩ|���]�p�X����i�H�Ͳ������~�C\n");
	
	msg = research->query_room_name()+"�ثe�w�g��o���������~�p�U�G\n"WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	i = 0;
	foreach(file in productslist)
	{
		catch(product = load_object(file));
	
		if( !objectp(product) ) continue;
			
		msg += sprintf("%-4s %s\n", (++i)+".", product->query_idname());
	}
	
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
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
		return tell(me, pnoun(2, me)+"�Q�n�]�w���򶵥ءH(help setup)\n");
	
	if( query("owner", master) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"���O�o�Ӥu�t���֦��̡C\n");

	sscanf(arg, "%s %s", arg, value);

	switch(arg)
	{
		case "research":
		{	
			if( value == "-d" )
			{
				if( !query("research", master) )
					return tell(me, "�o�̭쥻�K�S���]�w��o���ߡC\n");
					
				delete("research", master);
				
				master->save();
				
				return tell(me, "�R����o���߳]�w�C\n");
			}

			if( !value || sscanf(value, "%d %d,%d", num, x, y) != 3 )
				return tell(me, "�п�J���T����s���߮y��(�� 1 21,33)�C\n");
			
			file = CITY_ROOM_MODULE(city, (num-1), (x-1), (y-1), "research");
			
			if( !objectp(research = load_module(file)) || query("function", research) != "laboratory" )
				return tell(me, "�y��"+loc_short(city, num-1, x-1, y-1)+"�èS����o���ߡC\n");

			if( research != research->query_master() )
			{
				file = base_name(research->query_master());
				research = research->query_master();
			}
			
			if( query("owner", research) != me->query_id(1) )
				return tell(me, research->query_room_name()+"�ä��ݩ�"+pnoun(2, me)+"�C\n");
			
			set("research", file, master);
			
			master->save();
			
			msg("$ME�]�w"+research->query_room_name()+"�����Ͳ��u����o���ߡC\n", me, 0, 1);
			
			break;
		}
		
		case "warehouse":
		{
			if( value == "-d" )
			{
				if( !query("warehouse", master) )
					return tell(me, "�o�̭쥻�K�S���]�w���x���ߡC\n");
					
				delete("delivery", master);
				return tell(me, "�����s�f�ؼЧ����C\n", CMDMSG);
			}

			if( !value || sscanf(value, "%d %d,%d", num, x, y) != 3 )
				return tell(me, "�п�J���T���ܮw�y��(�� 1 21,33)�C\n");
			
			// ��f�ܭܮw
			file = CITY_ROOM_MODULE(city, (num-1), (x-1), (y-1), "warehouse");

			if( !objectp(warehouse = load_module(file)) || query("function", warehouse) != "warehouse" )
				return tell(me, "�y��"+loc_short(city, num-1, x-1, y-1)+"�èS���ܮw�C\n");
			
			if( warehouse != warehouse->query_master() )
			{
				file = base_name(warehouse->query_master());
				warehouse = warehouse->query_master();
			}

			if( query("owner", warehouse) != me->query_id(1) )
				return tell(me, warehouse->query_room_name()+"�ä��ݩ�"+pnoun(2, me)+"�C\n");
			
			set("warehouse", file, master);
			
			master->save();
			
			msg("$ME�]�w"+warehouse->query_room_name()+"�����Ͳ��u�����x���ߡC\n", me, 0, 1);
			
			break;
		}

		default:
			return tell(me, "�п�J���T���ﶵ(help setup)�C\n");
	}
}

// 30 ��I�s�@��
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
	
	//��p�ɦs�@��
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
		broadcast(room, "�u�t�ʤ֬�o���ߩέ��x���ߪ��䴩�A�ɭP�Ͳ�����C\n");
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
		broadcast(room, "�ѩ���u�ʥF�ɭP�u�t�޲z��O�����A�ɭP�Ͳ�����C\n");
		delete("line", room);
		room->save();
		return;
	}
	
	userinside = sizeof(filter_array(all_inventory(room), (: userp($1) :)));
	
	product_file = query("product", room);
	
	catch( product = load_object(product_file) );
	
	product_info = product->query_product_info();
	
	timecost = addn("line/timecost", 30 * (sizeof(slaves)+1), room);
	
	// ���ਬ��
	if( timecost > product_info["timecost"] )
	{
		amount = timecost / product_info["timecost"];
		
		// �x�s�Ѿl����
		set("line/timecost", timecost % product_info["timecost"], room);
		
		
		warehouse_database = query("products", warehouse);
		
		if( !mapp(warehouse_database) )
		{
			if( userinside )
				broadcast(room, "�ܮw���L���󪫫~�C\n");

			delete("line/timecost", room);
			return;
		}
		
		material_info = allocate_mapping(0);

		// �ˬd��ƬO�_����
		foreach( string basename_or_module, int need in product_info["material"] )
		{
			material_file = query_module_basename(warehouse, "products", basename_or_module, "�u�t���") || basename_or_module;

			if( count(query_object_amount(warehouse, "products", material_file, "�u�t���"), "<", need * amount) )
			{
				if( userinside )
				{
					if( objectp(material_ob = load_object(material_file)) )
						broadcast(room, "�ܮw�ʥF�u"+material_ob->query_idname()+"�v��ơC\n");
					else if( objectp(material_ob = load_object("/std/module/product/"+material_file+"/module.c")) )
					{
						product_info = material_ob->query_product_info();
						broadcast(room, "�ܮw�ʥF�u"+product_info["name"]+"("+capitalize(product_info["id"])+")"+"�v��ơC\n");
					}
					else
						broadcast(room, "�ܮw�ʥF�u"+material_file+"�v��ơC\n");
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
				broadcast(room, "�u�t�s�y���ӤF "+(need * amount)+" ��"+load_object(material)->query_idname()+"�C\n");
		}

		if( userinside )
			broadcast(room, "�u�t�s�y�Ͳ��X "+amount+" ��"+product->query_idname()+"�C\n");
			
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
	
		msg += "���u�B�@�G"+(query("line/working", master) ? HIG"�Ͳ���"NOR : HIR"���"NOR)+"\n";
		msg += "�s�y���~�G"+(objectp(product) ? product->query_idname() : "�L")+"\n";
		msg += "��o���ߡG"+(objectp(research) ? research->query_room_name()+" "HIC"��ޭ� "NOR CYN+query("technology", research)+NOR : "�L")+"\n";
		msg += "���x���ߡG"+(objectp(warehouse) ? warehouse->query_room_name() : "�L")+"\n";
		msg += "�u�t�޲z�G"HIY+(sizeof(master->query_slave())+1)+"/"+query("management", master)+NOR"\n";
		return msg;
	}
}
