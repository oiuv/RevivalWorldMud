/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _room_mod.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-08-24
 * Note   : room module
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <roommodule.h>
#include <estate.h>
#include <location.h>
#include <lock.h>
#include <citydata.h>

#define DISTRIBUTION_TIME	600

// �ж����O��
nosave mapping actions;
nosave int heartbeattick;
nosave int heartbeattime;
nosave int heartbeatcallout;

object query_master();

int is_module_room()
{
	return 1;
}

// �d�� Module Filename
string query_module_file()
{
	return query_temp("module_file");
}

// �� Action File �d�ߥ\����
varargs mapping query_action_info(string roomfunction)
{
	mapping action_info = fetch_variable("action_info", load_object(query_module_file()));
	
	if( !mapp(action_info ) )
		return 0;

	if( undefinedp(roomfunction) )
		return action_info;
	else
		return action_info[roomfunction];
}

void startup_heart_beat()
{
	set_heart_beat(1);
}

void stop_heart_beat()
{
	remove_call_out(heartbeatcallout);
	set_heart_beat(0);
}

// �Ұ� Action
int enable_action(string roomfunction)
{
	mapping action_info = query_action_info();
	
	if( !stringp(roomfunction) )
		roomfunction = query("function");
		
	if( !mapp(action_info) || !action_info[roomfunction] )
	{
		return 0;
	}

	// ���J Actions
	actions = action_info[roomfunction]["action"];

	// �������Ұ� heart_beat
	if( action_info[roomfunction]["heartbeat"] > 0 && this_object()->query_master() == this_object() )
	{
		heartbeattime = action_info[roomfunction]["heartbeat"] * 10;
		heartbeattick = random(heartbeattime);
		heartbeatcallout = call_out((:startup_heart_beat:), random(DISTRIBUTION_TIME)+1);
	}
	else
		stop_heart_beat();

	return 1;
}

// �d�ߥ\��趡���u�W��
string query_room_function_short()
{
	mapping function_action_info = query_action_info(query("function"));
	
	if( mapp(function_action_info) )
		return function_action_info["short"];
	else
		return 0;
}

// �]�w�ж��\�૬�A
int set_room_function(string roomfunction)
{
	if( this_object()->enable_action(roomfunction) )
	{
		set("function", roomfunction);
		return this_object()->save();
	}
	else
		return 0;	
}

string query_room_function()
{
	return query("function");	
}

int remove_room_function()
{
	delete("function");

	stop_heart_beat();
	
	actions = allocate_mapping(0);
	
	return this_object()->save();
}

string query_room_name()
{
	int num, x, y;
	string city;

	sscanf(base_name(this_object()), "/city/%s/%d/room/%d_%d_%*s", city, num, x, y);

	return query("short")+HIG"<"+(x+1)+","+(y+1)+">"NOR;
}

// �d�ߩж��Ҧb�����W��
string query_city()
{
	string city;
	
	sscanf(base_name(this_object()), "/city/%s/%*s", city);

	return city;	
}

// �d�ߩж��Ҧb��������
int query_city_num()
{
	int num;
	
	sscanf(base_name(this_object()), "/city/%*s/%d/room/%*s", num);
	
	return num;
}

string query_money_unit()
{
	return MONEY_D->city_to_money_unit(query_city());
}

// �d�ߩж��Ҧb����y��
array query_location()
{
	int num, x, y;
	string place;

	sscanf(base_name(this_object()), "/city/%s/%d/room/%d_%d_%*s", place, num, x, y);
	sscanf(base_name(this_object()), "/area/%s/%d/room/%d_%d_%*s", place, num, x, y);

	return arrange_city_location(x, y, place, num);
}

string query_coor_short()
{
	int x, y, z;
	string basename = base_name(this_object());
	
	if( sscanf(basename, "/city/%*s/%*d/room/%d_%d_%d_%*s", x, y, z) != 6 )
		sscanf(basename, "/city/%*s/%*d/room/%d_%d_%*s", x, y);
	
	if( z )
		return city_coor_short(x, y)+HIW+" "+(z+1)+NOR WHT"F"NOR;
	else
	{
		int floor = query("floor")+1;
		
		if( floor >= 20 )
			return city_coor_short(x, y)+HIW+" �Ӱ� "NOR WHT+floor+"�B"HIY+(floor/20)+" "NOR YEL"�Ŧa��"NOR;
		else
			return city_coor_short(x, y)+HIW+" �Ӱ� "NOR WHT+floor+NOR;
	}
}

// �^�ǫؿv�����A
string query_building_type()
{
	string type, basename = base_name(this_object());
	
	if( sscanf(basename, "/city/%*s/%*d/room/%*d_%*d_%*d_%s", type) != 6 )
		sscanf(basename, "/city/%*s/%*d/room/%*d_%*d_%s", type);
	
	return type;
	
}
// �]�w�D�n�s�곡��
int set_master(object ob)
{
	string file;
	
	if( !objectp(ob) || ob == this_object() ) return 0;

	file = base_name(ob);

	set("master", file);

	ob->set_slave(base_name(this_object()));

	stop_heart_beat();

	return ob->save() && this_object()->save();
}

// �d�ߥD�n�s�곡��
object query_master()
{
	string masterfile = copy(query("master"));

	if( !masterfile || !file_exist(masterfile+".o") )
		return this_object();

	else return load_object(masterfile);
}

// �����D�n�s�곡��
void remove_master()
{
	object master = query_master();

	if( master == this_object() ) return;

	delete("master");
	
	enable_action(query("function"));

	master->remove_slave(base_name(this_object()));
}

// �]�w���ݳ���
int set_slave(string file)
{
	string *slave = query("slave");

	if( !stringp(file) )
		return 0;

	if( !arrayp(slave) )
		set("slave", ({ file }));
	else 
		set("slave", slave | ({ file }));

	return 1;
}

// �������ݳ������
varargs void remove_slave(string file)
{
	string *slave = query("slave");
	
	if( !arrayp(slave) ) return;

	if( undefinedp(file) )
	{
		delete("slave");
		
		foreach( string files in slave )
		{				
			if( load_object(files) )	
				files->remove_master();
		}
		return;
	}
	
	if( member_array(file, slave) != -1 )
	{
		if( sizeof(slave - ({file})) )
			set("slave", slave - ({ file }));
		else
			delete("slave");
			
		if( load_object(file) )
			file->remove_master();
	}		
}
	
// �d�ߪ��ݳ���
string *query_slave()
{
	string *slaves = query("slave");
	
	/* �ΨӲM�������T���s���T
	if( sizeof(slaves) )
		foreach( string slave in slaves )
			if( !file_exist(slave+".o") )
				remove_slave(slave);
	*/		
	return slaves;
}

// �^�Ǿ�ɫؿv���ж�����
void query_building_rooms(object ref *rooms)
{
	object room;
	mapping exits = query("exits");
	
	if( !arrayp(rooms) )
		rooms = allocate(0);

	rooms |= ({ this_object() });

	if( !mapp(exits) ) return;
	
	foreach(string direction, mixed exit in exits)
	{
		if( stringp(exit) && objectp(room = find_object(exit)) )
		{
			if( member_array(room, rooms) == -1 )
			{
				room->query_building_rooms(&rooms);
			}
		}
	}
}


object *query_all_floors()
{
	object room, *allfloors = allocate(0);
	string city, type;
	int x, y, num, floor;	

	floor = query("floor");

	if( !floor ) return 0;
	
	sscanf(base_name(this_object()), "/city/%s/%d/room/%d_%d_%s", city, num, y, x, type);
	
	while(floor--)
		if( objectp(room = find_object("/city/"+city+"/"+num+"/room/"+y+"_"+x+"_"+(floor+1)+"_"+type)) )
			allfloors += ({ room });
		
	return allfloors;
}

array query_building_info()
{
	return fetch_variable("building_info", load_object(query_module_file()));
}

void heart_beat()
{
	int time;
	object module_ob;
	
	if( (heartbeattick++%heartbeattime) ) return;

	module_ob = find_object(query_module_file());

	time = time_expression {
		if( catch(module_ob->heart_beat(this_object())) )
		{
			CHANNEL_D->channel_broadcast("sys", file_name(this_object())+" �߸��禡�o�Ϳ��~�A����߸��C");
			stop_heart_beat();
		}
	};

	if( time > 300000 )
		CHANNEL_D->channel_broadcast("nch", file_name(this_object())+" ���߸�����ɶ� "+sprintf("%2.6f sec", time/1000000.));
}

void init(object ob)
{
	if( ob->is_module_npc() )
	{
		mapping data = query("objects") || allocate_mapping(0);	
		
		data[base_name(ob)] = "1";
		
		set("objects", data);
		
		this_object()->save();
	}
	
	query_module_file()->init(this_object(), ob);
}

void leave(object ob)
{
	if( ob->is_module_npc() )
	{
		mapping data = query("objects");
		
		if( !mapp(data) ) return;
		
		map_delete(data, base_name(ob));
		
		if( sizeof(data) )
			set("objects", data);
		else
			delete("objects");
			
		this_object()->save();
	}
	
	query_module_file()->leave(this_object(), ob);
}

void reset_inventory()
{
	mapping objects = allocate_mapping(0);
	
	delete("objects");
	
	foreach(object inv in all_inventory(this_object()))
	{
		if( userp(inv) ) continue;

		objects[base_name(inv)] = query_temp("amount", inv) || "1";
	}
	
	if( sizeof(objects) )
		set("objects", objects);
}

int query_floor()
{
	int floor;
	
	sscanf(base_name(this_object()), "/city/%*s/%*d/room/%*d_%*d_%d_%*s", floor);
	
	return floor+1;
}

// �� Virtual_D �I�s�ðe�J��������W�� file
varargs void create(string file, string type)
{
	string module_file;

	if( !stringp(file) || !stringp(type) ) return;
	
	module_file = ROOM_MODULE_PATH+type+".c";

	// �b Virtual_D �̫K�ˬd�O�_�ʤ� Action File
	if( !file_exist(module_file) )
		error("�ݭn Module File �� "+module_file+"�C\n");
	
	// ���� Action File
	set_temp("module_file", module_file);
	 
	// �Y�O�Ĥ@���Хߩж�, �h�]�w�򥻸��
	if( !this_object()->restore(file) ) 
	{
		array building = query_building_info();
		set("short", building[ROOMMODULE_SHORT]);
		set("long", multiwrap("    �o�O"+building[ROOMMODULE_SHORT]+"�ؿv�����@�ө|���W�����C(decorate)�����s�ж��A�u��"
		"�\�h�ؿv�᪺�o����Ƥ��M�������a�A�L�H��z�A�Ů𤤫h�R���F�|�����h�����d�P�o������A�ݨӱo���n�n���C�@�U�F�C", 72));

		this_object()->save(file);
	}
	else if( !this_object()->query_database() )
		error(file_name(this_object())+" Ū���ɮ׸��("+file+")���~\n");
	else
		enable_action(query("function"));
	
	this_object()->reset();
}
