/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : map_d.c
 * Author : Cookys@RevivalWorld
 * Date   : 2001-06-13
 * Note   : �a�Ϫ���
 *	    �n�Q�� MAP_D �a�Ϩt�Τ�����, �����w�q�H�U�禡
 *          o string show_map(array loc, int cover_range)      // ��ܦa��
 *          o object query_maproom(array loc)                  // �^�Ǧa�Ϫ���
 *	    o object *query_all_maproom()                      // �^�ǩҦ��a�Ϫ���
 *          o int move(object ob, string dir)                  // �ϥΪ̲���
 *          o varargs array query_map(array loc, int realtime) // �^�Ǧa�ϰ}�C
 *	    o void remove_player_icon(object me, array loc)    // �Y�Q���H�ιϹ��г]�p���禡
 *          o void set_player_icon(object me, array loc)       // �Y�Q���H�ιϹ��г]�p���禡
 * mapping query_coor_range(array loc, string prop, int radius)// look.c
 *	      int valid_move(object me,string direction)	// combat_d �k�]�� in go.c
 * string coor_short_name(array loc)
 * varargs mixed query_coor_data(array loc, mixed prop)
 *	    �åB�����b map_system_table ���]�w�y�����ݧP�_�禡
 * Update :
 *  o 2001-06-20 Clode �ק�a���ɮ��x�s���| ex: /city/xx/realtime_map/1_map
 *  o 2001-08-07 Clode �j�T�ק�W�[�����
 *  o 2002-09-17 Clode �קﳡ�� code �����e����O
 *  o 2002-09-24 Clode ���s�]�w MAP_D ���w��, �Ϧa�Ϩt�Υ\���䴩�h�دS��a��
 *
 -----------------------------------------
 */
//#pragma save_binary
#include <daemon.h>
#include <ansi.h>
#include <location.h>

#define DATA_PATH	"/data/daemon/mapload.o"

private mapping mapload;
private mapping worldmap;
private nosave mapping coordinates = allocate_mapping(0);

// �Ѧ��]�w�i�Q�� MAP_D �a�Ϩt�Τ��Ҧ��t�ΦC��
nosave mapping map_system_table =
([
	// �t�ΦC��	// �y�Шt�����ݧP�_�禡
	CITY_D	:	(: CITY_D->is_city_location($1) :),
	AREA_D	:	(: AREA_D->is_area_location($1) :),
]);	

// �Ǧ^�a�Ϩt�κ���
string query_map_system(mixed map)
{		
	foreach( string system, function fp in map_system_table )
		if( find_object(system) && evaluate(fp, map) ) return system;
		
	return 0;
}

//int showmap_count=0;	
// �ѫ��O look �I�s, �Ǧ^�� title screen ���
string show_map(array loc)
{
	string map_system;
	
	if( !arrayp(loc) ) return 0;
	
	map_system = query_map_system(loc);
	//if( (showmap_count++ %10) == 0 )
	//	CHANNEL_D->channel_broadcast("nch", "map_d show_map count�G"+showmap_count);
	if( map_system )
		return map_system->show_map(loc);
	
	return 0;
}

// �Ǧ^�a�Ϫ���
object query_maproom(array loc)
{
	string map_system;
	
	if( !arrayp(loc) ) return 0;
	
	map_system = query_map_system(loc);
	
	if( map_system )
		return map_system->query_maproom(loc);
	
	return 0;
}

// �ಾ�ʥG?
varargs int valid_move(object me, string direction, array loc)
{
	string map_system;
	
	if( !arrayp(loc) )
		loc = query_temp("location", me);

	if( !arrayp(loc) ) return 0;
	
	map_system = query_map_system(loc);
	
	if( map_system )
		return map_system->valid_move(me, direction, loc);
	
	return 0;
}

object *query_all_maproom()
{
	object *all_maproom = allocate(0);
	
	foreach( string system, function fp in map_system_table )
		all_maproom += system->query_all_maproom() || allocate(0);
		
	return all_maproom;
}
	
// ���󲾰ʳB�z
void move(object ob, string dir)
{
	string map_system;
	
	array loc;
	
	if( !objectp(ob) )
		return;

	loc = query_temp("location", ob);

	if( !arrayp(loc) ) return;
	
	map_system = query_map_system(loc);
	
	if( map_system )
		map_system->move(ob, dir);
}

// look.c need
mapping query_coor_range(array loc, string prop, int radius)
{
	string map_system;
	
	map_system = query_map_system(loc);
	
	if( map_system )
		return map_system->query_coor_range(loc, prop,radius);
}

// look.c need
string coor_short_name(array loc)
{
	string map_system;
	
	map_system = query_map_system(loc);
	
	if( map_system )
		return map_system->coor_short_name(loc);
	
	return 0;
}

// look.c need
varargs mixed query_coor_data(array loc, mixed prop)
{
	string map_system;
	
	map_system = query_map_system(loc);
	
	if( map_system )
		return map_system->query_coor_data(loc,prop);
		
	return 0;
}

varargs array query_map(array loc, int realtime)
{
	string map_system;
	
	if( !arrayp(loc) ) return 0;
	
	map_system = query_map_system(loc);
	
	if( map_system )
	{
		if( !realtime )
			return map_system->query_map(loc);
		else
			return map_system->query_map(loc, realtime);
	}
	
	return 0;
}

string query_raw_map(array loc)
{
	string map_system, strmap = "";
	array rawmap;
	
	if( !arrayp(loc) ) return 0;
	
	map_system = query_map_system(loc);
	
	if( map_system )
		rawmap = map_system->query_map(loc);

	if( arrayp(rawmap) )
	{
		foreach( array arr in rawmap )
			strmap += implode(arr, "");
		return remove_ansi(strmap);
	}
	else
	 	return 0;
}

void remove_player_icon(object me, array loc)
{
	string map_system;
	
	if( !arrayp(loc) ) return;
	
	map_system = query_map_system(loc);
	
	if( map_system )
		map_system->remove_player_icon(me, loc);
}

void set_player_icon(object me, array loc)
{
	string map_system;
	
	if( !arrayp(loc) ) return;
	
	map_system = query_map_system(loc);
	
	if( map_system )
		map_system->set_player_icon(me, loc);
}

mapping query_action(array loc)
{
	string map_system;
	
	if( !arrayp(loc) ) return allocate_mapping(0);
	
	map_system = query_map_system(loc);
	
	if( map_system )
		return map_system->query_action(loc) || allocate_mapping(0);
	
	return allocate_mapping(0);
}


// �^���Y�p��Ҥ��a��
string query_map_reduction(array map, int ratio)
{
	int x, y;
	int x_size, y_size;
	string str = "";

	map = copy(map);
	
	if( ratio < 1 || ratio > 5 ) return 0;
	
	y_size = sizeof(map);
	
	for(y=0;y<y_size;y++)
	{
		if( y % ratio ) continue;
		
		x_size = sizeof(map[y]);
		
		for(x=0;x<x_size;x++)
		{
			if( x % ratio )
				map[y][x] = 0;
		}
		
		str += implode(map[y], "")+"\n";
	}

	return str+NOR WHT"[ "HIW"��� 1 : "+ratio+NOR WHT" ]\n"NOR;
}

void enter_coordinate(array loc, object ob)
{
	if( !arrayp(loc) || !objectp(ob) )
		return;

	if( !arrayp(coordinates[loc[CODE]]) )
		coordinates[loc[CODE]] = ({ ob });
	
	else coordinates[loc[CODE]] |= ({ ob });
}

void leave_coordinate(array loc, object ob)
{
	if( !arrayp(loc) || !objectp(ob) )
		return;

	if( !arrayp(coordinates[loc[CODE]]) )
		return;
		
	if( !sizeof(coordinates[loc[CODE]] -= ({ ob, 0 })) )
		map_delete(coordinates, loc[CODE]);
}

object *coordinate_inventory(array loc)
{
	return arrayp(loc) && coordinates[loc[CODE]] ? filter_array(coordinates[loc[CODE]], (: objectp($1) :)) : allocate(0);
}

mapping query_coordinates()
{
	return copy(coordinates);
}

void restore_map_object()
{
	array loc;
	object ob;
	string basename, amount;

	// ���s���˩Ҧ�����y��
	foreach( ob in objects((: environment($1) && environment($1)->is_maproom() :)) )
	{
		if( ob->query_database() )
			enter_coordinate(query_temp("location", ob), ob);
	}

	if( mapp(mapload) )
	foreach( string sloc, mapping data in mapload )
	{
		loc = restore_variable(sloc);
		
		if( !arrayp(loc) || sizeof(coordinate_inventory(loc)) ) continue;

		foreach( basename, amount in data )
		{
			reset_eval_cost();
			
			catch( ob = new(basename) || load_object(basename) );
			
			if( objectp(ob) )
			{
				if( !query("flag/no_amount", ob) )
					set_temp("amount", amount, ob);

				ob->move(loc, amount);
			}
			else
				CHANNEL_D->channel_broadcast("sys", "�L�k���J�a�Ϯy�Ъ��� "+sloc+" "+basename);
		}
	}
	
	mapload = allocate_mapping(0);

}

void create()
{
	if( clonep() ) 
		destruct(this_object());
	
	if( restore_object(DATA_PATH) )
	{
		if( !mapp(worldmap) )
			worldmap = allocate_mapping(0);
	
		call_out((: restore_map_object :), 30);
	}
	else
		CHANNEL_D->channel_broadcast("sys", "�L�k���J�a�Ϯy�Ъ���s��");
}

mapping query_range_inventory(array loc, int radius)
{
	string place;
	int num, x, y;
	mapping range_inv = allocate_mapping(0);

	if( !arrayp(loc) ) return 0;
	
	place = loc[PLACE];
	num = loc[NUM];
	x = loc[X];
	y = loc[Y];
	
	radius *= radius;

	foreach( object ob in all_inventory(query_maproom(loc)) )
	{
		loc = query_temp("location", ob);
		
		if( !arrayp(loc) ) continue;
		
		if( (loc[X]-x)*(loc[X]-x) + (loc[Y]-y)*(loc[Y]-y) <= radius )
			range_inv[ob] = loc;
	}
	
	return range_inv;
}

array set_world_map(int x, int y, string zone, int num)
{
	int ix, iy;
	mapping mdata;
	array adata;
	
	if( !intp(x) || !intp(y) || !stringp(zone) || !intp(num) )
		return 0;

	if( undefinedp(worldmap[x]) )
		worldmap[x] = allocate_mapping(0);
	
	// �Y���y�Фw�g�Q�]�w�F, ������ delete, ���ઽ���л\
	if( !undefinedp(worldmap[x][y]) )
		return 0;

	// �����\���г]�w
	foreach(ix, mdata in worldmap)
		foreach(iy, adata in mdata)
			if( adata[0] == zone && adata[1] == num )
				return 0;

	CHANNEL_D->channel_broadcast("nch", "�N�ϰ� "+zone+" "+num+" ���@�ɦa�Ϯy�г]�w�� "+x+","+y+"�C");

	worldmap[x][y] = ({ zone, num });
	
	HTML_D->create_map_html();

	return worldmap[x][y];
}
int *query_world_map_location(string zone, int num)
{
	int ix, iy;
	mapping mdata;
	array adata;
	
	foreach(ix, mdata in worldmap)
		foreach(iy, adata in mdata)
			if( adata[0] == zone && adata[1] == num )
				return ({ ix, iy });
	
	return 0;
}

varargs mixed query_world_map(int x, int y)
{
	if( undefinedp(x) && undefinedp(y) )
		return worldmap;
	
	if( undefinedp(worldmap[x]) )
		return 0;
		
	return worldmap[x][y];
}

array delete_world_map(int x, int y)
{
	if( undefinedp(worldmap[x]) )
		return 0;
	
	map_delete(worldmap[x], y);
	
	if( !sizeof(worldmap[x]) )
		map_delete(worldmap, x);
}

int save()
{
	string sloc;
	array loc;

	mapload = allocate_mapping(0);

	// �u�x�s�����̪����~
	foreach( object ob in objects((: environment($1) && environment($1)->is_maproom() :)) )
	{
		if( !ob->query_database() || !(loc = query_temp("location", ob)) || userp(ob) ) continue;
		
		if( !CITY_D->valid_coordinate(loc) ) continue;

		reset_eval_cost();

		sloc = save_variable(loc);
		
		if( !mapp(mapload[sloc]) )
			mapload[sloc] = allocate_mapping(0);
		
		mapload[sloc][base_name(ob)] = count(mapload[sloc][base_name(ob)], "+", query_temp("amount", ob) || 1);
	}
	
	return save_object(DATA_PATH);
}


int remove()
{
	return save();
}

string query_name()
{
	return "�a�Ϩt��(MAP_D)";
}

