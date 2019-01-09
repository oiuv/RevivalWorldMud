/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : city_d_main.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-10
 * Note   : �������F�D��, �B�z�Ҧ�������Ʈw
 * Update :
 *  o 2002-09-17 Clode ���s�]�p������Ƶ��c�P�x�s��Ƶ{��
 *  o 2002-09-17 Clode �קﳡ�� code �����e����O
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <lock.h>
#include <location.h>
#include <daemon.h>
#include <object.h>
#include <citydata.h>
#include <terrain.h>
#include <gender.h>
#include <message.h>
#include <mudlib.h>
#include <nature.h>
#include <nature_data.h>
#include <roommodule.h>
#include <time.h>
#include <envvar.h>


#define LOG			"daemon/city"
#define DEFAULT_COOR_DATA	([ ])

#define SAVE_INFO		(1<<0)
#define SAVE_DATA		(1<<1)
#define SAVE_MAP		(1<<2)
#define SAVE_ESTATE		(1<<3)
#define SAVE_ALL		(SAVE_INFO|SAVE_DATA|SAVE_MAP|SAVE_ESTATE)

#define MAP		"map"
#define RMAP		"realtime_map"
#define DATA		"data"
#define INFO		"info"
#define CROOM		"cityroom"

#define OCCUPY_MONEY	100000000

#define RANDOM_STRING		({ "�O�ֲy", "�R�𫽫�", "�δο}", "�h��", "������", "�U�N�����D��", "��e�b", "�����q�c", "�H����", "�æ��z����", "�¹D�Ѥj", "�Ϧ��l", "�o�����ʪ�", "�ּu�o�g��", "������" })
#define CITY_SCALE		({HIY"��"NOR YEL"��"NOR, HIG"�m"NOR GRN"��"NOR, HIW"��"NOR WHT"��"NOR, HIC"��"NOR CYN"�|"NOR, HIR"�j"NOR RED"����"NOR})

private int number;
private int save_all_time;

private mapping cities;
private mapping sort_save_list;

varargs int city_exist(string city, int num)
{
	if( undefinedp(num) )
		return mapp(cities[city]);
	else
		return mapp(cities[city]) && mapp(cities[city][num]);
}

varargs int map_exists(string map, int section)
{
	if( undefinedp(section) )
		return mapp(cities[map]);
	else
		return mapp(cities[map]) && mapp(cities[map][section]);
}

int is_city_location(mixed map)
{
	if( arrayp(map) )
		return city_exist(map[CITY], map[NUM]);
	else if( stringp(map) )
		return city_exist(map);

	return 0;
}

varargs string query_city_id(string city, int num)
{
	if( undefinedp(num) )
	{
		if( !city_exist(city) ) return 0;
		return cities[city][INFO]["color_id"];
	}
	else
	{
		if( !city_exist(city, num) ) return 0;
		return cities[city][INFO]["color_id"] + " "+(num+1);
	}
}

varargs string query_city_name(string city, int num)
{
	if( undefinedp(num) )
	{
		if( !city_exist(city) || !cities[city] || !cities[city][INFO] ) return 0;
		return cities[city][INFO]["name"];
	}
	else
	{
		if( !city_exist(city, num) ) return 0;
		return cities[city][INFO]["name"] + (cities[city][INFO]["section"][num]["name"] || "��"+CHINESE_D->chinese_number(num+1)+"�ìP����");
	}
}

varargs string query_city_idname(string city, int num)
{
	if( undefinedp(num) )
	{
		if( !city_exist(city) ) return 0;
		if( !cities[city] || !cities[city][INFO] ) return 0;
		return cities[city][INFO]["name"]+"<"+cities[city][INFO]["color_id"]+">";
	}
	else
	{
		if( !city_exist(city, num) ) return 0;
		return cities[city][INFO]["name"]+(cities[city][INFO]["section"][num]["name"] || "��"+CHINESE_D->chinese_number(num+1)+"�ìP����")+"<"+cities[city][INFO]["color_id"]+" "+(num+1)+">";
	}
}

mapping query_public_facility(string city)
{
	int num, x, y;
	array all_coor_data;
	mapping facility = allocate_mapping(0);

	if( !CITY_D->city_exist(city) ) return 0;

	while(CITY_D->city_exist(city, num))
	{
		all_coor_data = cities[city][num][DATA];

		for(x=0;x<100;x++)
			for(y=0;y<100;y++)
			{
				if( all_coor_data[y][x][ROOM] )
					switch( all_coor_data[y][x][ROOM] )
					{
						case "cityhall":
						case "trading_post":
						case "recycle":
						case "labor":
						case "postoffice":
						case "bank":
						case "auction":
						case "jail":
						case "trainstation":
						case "prodigy":
							facility["���@�ؿv��"]++;
							break;
						default:
							break;
					}
				else
					switch( all_coor_data[y][x][TYPE] )
					{
						case MOUNTAIN:
							facility["�s��"]++;
							break;
						case TREE:
						case GRASS:
						case PAVILION:
						case FOREST:
							facility["�˪L"]++;
							break;
						case POOL:
						case RIVER:
							facility["�e�y"]++;
							break;
						case ROAD:
							facility["�D��"]++;
							break;
						case BRIDGE:
							facility["����"]++;
							break;
						case STATUE:
						case LIGHT:
						case 0:
							facility["�Ŧa"]++;
							break;
					}
			}

		num++;
	}

	return facility;
}

int valid_coordinate(mixed x, int y, string city, int num)
{
	if( intp(x) )
		return x>=0 && x<MAP_WIDTH && y>=0 && y<MAP_LENGTH && city_exist(city, num);
	else if( arrayp(x) )
		return x[X]>=0 && x[X]<MAP_WIDTH && x[Y]>=0 && x[Y]<MAP_LENGTH && city_exist(x[CITY], x[NUM]);
	else
		return 0;
}

/* ��Ҧ������s�� */
void assign_cities_num()
{
	number = 0;

	sort_save_list = allocate_mapping(0);

	foreach( string city in get_dir("/city/") )
	{
		if( file_size("/city/"+city) != -2 ) continue;

		sort_save_list[number++] = city;
	}
}


// ���Ƽƶq����, �����g�b CITY_D �̮Ĳv����
void material_growup(string city, int num)
{
	mapping fund = CITY_D->query_city_info(city, "fund");
	mapping section_info = cities[city][INFO]["section"][num];

	section_info["resource/metal"] += (fund["�s��"]+30)*2;
	section_info["resource/stone"] += (fund["�s��"]+30)*2;
	section_info["resource/water"] += (fund["�e�y"]+30)*2;
	section_info["resource/wood"] += (fund["�˪L"]+30)*2;
	section_info["resource/fuel"] += (fund["�˪L"]+30)*2;

	if( section_info["resource/metal"] > 10000 * (fund["�s��"]+30) )
		section_info["resource/metal"] = 10000 * (fund["�s��"]+30);

	if( section_info["resource/stone"] > 10000 * (fund["�s��"]+30) )
		section_info["resource/stone"] = 10000 * (fund["�s��"]+30);

	if( section_info["resource/water"] > 10000 * (fund["�e�y"]+30) )
		section_info["resource/water"] = 10000 * (fund["�e�y"]+30);

	if( section_info["resource/wood"] > 10000 * (fund["�˪L"]+30) )
		section_info["resource/wood"] = 10000 * (fund["�˪L"]+30);

	if( section_info["resource/fuel"] > 10000 * (fund["�˪L"]+30) )
		section_info["resource/fuel"] = 10000 * (fund["�˪L"]+30);

	cities[city][INFO]["section"][num] = section_info;
}

void allcity_material_growup()
{
	foreach( string city, mapping data1 in cities )
	foreach( mixed num, mapping data2 in data1 )
	{
		if( intp(num) )
			material_growup(city, num);
	}
}

void save_options(string city, int num, int bit)
{
	// �ഫ�a�Ϧ� HTML
	CITY_D->translate_map_to_html(city, num);

	// �g�J�򥻸�T��
	if( bit & SAVE_INFO )
	{

		if( !city_exist(city) || undefinedp(cities[city][INFO]) )
		{
			CHANNEL_D->channel_broadcast("sys", "�ॢ "+city+" �򥻸�T");
			log_file(LOG, "�ॢ "+city+" �򥻸�T");
		}
		else if( !write_file(CITY_INFO(city), save_variable( copy(cities[city][INFO]) ), 1) )
		{
			CHANNEL_D->channel_broadcast("sys", "�L�k�g�J "+city+" �򥻸�T��");
			log_file(LOG, "�L�k�g�J "+city+" �򥻸�T��");
		}
	}

	// �g�J�y�и����
	if( bit & SAVE_DATA )
	{
		if( !city_exist(city) || undefinedp(cities[city][num][DATA]) )
		{
			CHANNEL_D->channel_broadcast("sys", "�ॢ "+city+" �y�и��");
			log_file(LOG, "�ॢ "+city+" �y�и��");
		}

		else if( !write_file(CITY_NUM_DATA(city, num), save_variable( copy(cities[city][num][DATA]) ), 1) )
		{
			CHANNEL_D->channel_broadcast("sys", "�L�k�g�J "+city+" "+num+" �y�и����");
			log_file(LOG, "�L�k�g�J "+city+" "+num+" �y�и����");
		}
	}

	// �g�J�a�ϸ����
	if( bit & SAVE_MAP )
	{
		if( !city_exist(city) || undefinedp(cities[city][num][MAP]) )
		{
			CHANNEL_D->channel_broadcast("sys", "�ॢ "+city+" �a�ϸ��");
			log_file(LOG, "�ॢ "+city+" �a�ϸ��");
		}

		else if( !write_file(CITY_NUM_MAP(city, num), save_variable( copy(cities[city][num][MAP]) ), 1) )
		{
			CHANNEL_D->channel_broadcast("sys", "�L�k�g�J "+city+" "+num+" �a�ϸ����");
			log_file(LOG, "�L�k�g�J "+city+" "+num+" �a�ϸ����");
		}
	}

	// �g�J�Цa�������
	if( bit & SAVE_ESTATE )
	{
		if( !write_file(CITY_NUM_ESTATE(city, num), save_variable(ESTATE_D->query_city_estate(city, num)), 1) )
		{
			CHANNEL_D->channel_broadcast("sys", "�L�k�g�J "+city+" "+num+" �Цa�������");
			log_file(LOG, "�L�k�g�J "+city+" "+num+" �Цa�������");
		}
	}
}

varargs void save_city(string city, int num, int bit)
{
	int time_exp;

	if( !undefinedp(num) )
	{
		string options = "";

		if( !city_exist(city, num) ) return;

		time_exp = time_expression { save_options(city, num, undefinedp(bit) ? SAVE_ALL : bit); };

		if( bit & SAVE_INFO ) options += "�򥻸�T ";
		if( bit & SAVE_DATA ) options += "�y�и�� ";
		if( bit & SAVE_MAP  ) options += "�a�ϸ�� ";
		if( bit & SAVE_ESTATE) options += "�Цa����� ";

		if( options != "" )
			CHANNEL_D->channel_broadcast("nch",sprintf("�����G�x�s�����u%s�v%s(%.6f sec)�C",query_city_idname(city, num), options, time_exp/1000000.));

		call_out((:ESTATE_D->handle_freeland($(city), $(num)):), (num+1)*5);
	}
	else if( city_exist(city) )
	{
		for(num=0;city_exist(city, num);num++)
		{
			time_exp += time_expression { save_options(city, num, SAVE_ALL); };
			call_out((:ESTATE_D->handle_freeland($(city), $(num)):), (num+1)*5);
		}

		CHANNEL_D->channel_broadcast("nch",sprintf("�����G�x�s�����u%s�v�Ҧ����(%.6f sec)�C",query_city_idname(city), time_exp/1000000.));
	}
}

/* �� TIME_D �w���I�s, ������ƽ��y�x�s */
void time_distributed_save()
{
	string city;
	int num;

	if( sizeof(cities) < 1 ) return;

	city = sort_save_list[++number] || sort_save_list[(number=0)];

	for(num=0;city_exist(city, num);num++)
	{
		broadcast(cities[city][num][CROOM], "\n"+query_city_idname(city)+"���a�߲`�B��M�Ǩӡu�F�v���@�n�A�H�۪��O�j�a�@�}�n�̡C\n");
		call_out( (: save_city :), (num+1)*20, city, num);
	}

	CITY_D->rehash_flourish(city);

	if( CITY_D->query_city_info(city, "crashtime") > 0 )
	{
		if( CITY_D->query_city_info(city, "crashtime") - time() < 0 )
			CITY_D->city_collapse(city);
	}
}

/* �x�s�Ҧ� city ��T */
int save_all()
{
	int time_exp;

	// ���קK�b shutdown ��, �y������ save_all ������, �G�]�m save_all_time �@�ˬd
	if( time() - save_all_time < 30 ) return 0;

	save_all_time = time();

	time_exp = time_expression {
		foreach( string city in get_dir(CITY_ROOT) )
		if( file_size(CITY_PATH(city)) == -2 )
			save_city(city);
	};// end of time_expression

	CHANNEL_D->channel_broadcast("nch",sprintf("�����G�����x�s�Ҧ�������Ƨ���(�@�O %.6f sec)�C", time_exp/1000000.));
	return 1;
}

// �w�s�b���������]���
void reset_city_data(string city)
{
	int x, y;

	// �����O fallen city �~������]���
	if( !cities[city]
	    || !cities[city][INFO]
	    || !cities[city][INFO]["fallen"] ) return;


	// ���]�������
	cities[city][INFO]["citizens"] 			= allocate(0);
	cities[city][INFO]["citizens_jointime"] 	= allocate_mapping(0);

	// ���]����
	cities[city][INFO]["government/mayor"] 		= 0;

	// ���]�x��
	cities[city][INFO]["government/officer"] 	= allocate_mapping(0);

	// ���]�ɥN
	cities[city][INFO]["age"]			= 0;

	// ���]�g�O���
	cities[city][INFO]["fund"]			= allocate_mapping(0);

	// ���]�����겣���
	cities[city][INFO]["assets"]			= "0";

	cities[city][INFO]["crashtime"]			= 0;
	// ���]���ϸ��
	cities[city][INFO]["section"] 			= allocate_mapping(0);
	cities[city][INFO]["section"][0] 		= allocate_mapping(0);

	// �~���H�f
	cities[city][INFO]["section"][0]["flourish"]	= 0;
	cities[city][INFO]["section"][0]["resident"] 	= 0;


	for(y=0;y<MAP_LENGTH;y++)
		for(x=0;x<MAP_WIDTH;x++)
		{
			switch(cities[city][0][DATA][y][x][TYPE])
			{
			case RIVER:
			case FOREST:
			case MOUNTAIN:

				break;
			default:
				cities[city][0][DATA][y][x] = DEFAULT_COOR_DATA;
				cities[city][0][RMAP][y][x] = "�D";
				cities[city][0][MAP][y][x] = "�D";
				break;
			}
		}
}

// �гy������l�Ƹ��
void initialize_city_data(string city)
{
	if( !stringp(city) ) return;

	cities[city] 					= allocate_mapping(0);
	cities[city][INFO] 				= allocate_mapping(0);
	cities[city][INFO]["citizens"] 			= allocate(0);
	cities[city][INFO]["citizens_jointime"] 	= allocate_mapping(0);
	cities[city][INFO]["government/mayor"] 		= 0;
	cities[city][INFO]["government/officer"] 	= allocate_mapping(0);
	cities[city][INFO]["fund"]			= allocate_mapping(0);
	cities[city][INFO]["name"] 			= NOR WHT"�o�󳣥�"NOR;
	cities[city][INFO]["age"]			= 0;
	cities[city][INFO]["emblem"]			= ({ "          ", " �o�󳣥� ", "          " });
	cities[city][INFO]["assets"]			= "0";

	cities[city][INFO]["section"]   		= allocate_mapping(0);
	cities[city][INFO]["section"][0] 		= allocate_mapping(0);
	cities[city][INFO]["section"][0]["resident"] 	= 0;
	cities[city][INFO]["section"][0]["flourish"] 	= 0;

	cities[city][INFO]["fallen"]			= 1;

	cities[city][0] 				= allocate_mapping(5);
	cities[city][0][DATA] 				= allocate_mapping(0);
	cities[city][0][MAP]  				= allocate(0);
	cities[city][0][RMAP] 				= allocate(0);
	cities[city][0][CROOM]				= load_cityroom(city, 0);

}

// ����o�󫰥������s����
void occupy_new_city(object me, string cityid, string cityname, string moneyunit)
{
	array loc = query_temp("location", me);
	string city, newcity, defaultmoneyunit = MONEY_D->query_default_money_unit();
	int *worldmaploc, strlen;

	if( !arrayp(loc) || !cityid ) return;

	city = loc[CITY];

	if( !cities[city][INFO]["fallen"] )
		return tell(me, "�o�y�����ä��O�o�󫰥��A�L�k����C\n");

	strlen = noansi_strlen(cityname);
	if( strlen < 2 || strlen > 10 )
		return tell(me, "����������W�٦r�ƥ������� 1 - 5 �Ӥ���r�C\n");

	strlen = noansi_strlen(cityid);
	if( strlen < 3 || strlen > 12 )
		return tell(me, "�������^��W�٦r�ƥ������� 3 - 12 �ӭ^��r�C\n");

	if( !is_chinese(cityname) )
		return tell(me, "����������W�٥u��ϥΤ���C\n");

	if( !is_english(cityid) )
		return tell(me, "�������^��W�٥u��ϥέ^��r���C\n");

	moneyunit = remove_fringe_blanks(upper_case(remove_ansi(moneyunit)));

	if( !is_english(moneyunit) )
		return tell(me, "�f����쥲���O�^��C\n");

	if( strlen(moneyunit) != 2 )
		return tell(me, "�f����쥲���O 2 �ӭ^��r���C\n");

	cityid = lower_case(cityid);
	cityid = kill_repeat_ansi(replace_string(cityid, "\n", "")+NOR);
	newcity = remove_ansi(cityid);
	cityid = ansi_capitalize(cityid);

	cityname = kill_repeat_ansi(replace_string(cityname, "\n", "")+NOR);

	if( newcity != city && (city_exist(newcity) || AREA_D->area_exist(newcity)) )
		return tell(me, "�ϰ� "+newcity+" �w�g�s�b�A�ШϥΨ�L�W�١C\n");

	if( MONEY_D->money_unit_to_city(moneyunit) )
		return tell(me, "������ $"+moneyunit+" �w�g�Q�ϥΡC\n");

	if( !me->spend_money(defaultmoneyunit, OCCUPY_MONEY) )
		return tell(me, pnoun(2, me)+"�����֦� $"+defaultmoneyunit+" "+NUMBER_D->number_symbol(OCCUPY_MONEY)+" ���{���Ӿ�y�o�Ӽo�󫰥���~�����C\n");

	tell(me, pnoun(2, me)+"��O�F $"+defaultmoneyunit+" "+NUMBER_D->number_symbol(OCCUPY_MONEY)+" ����y�æ���F�o�y�o�󳣥��C\n");
	CHANNEL_D->channel_broadcast("news", me->query_idname()+"��O�F $"+defaultmoneyunit+" "+NUMBER_D->number_symbol(OCCUPY_MONEY)+" ��y�æ���F"+query_city_idname(city)+"�A�è��W���u"+cityname+"<"+cityid+">�v�A�f����쬰�u"+moneyunit+"�v�C");

	if( newcity != city )
	{
		// ���� cityroom
		set("city", newcity, cities[city][0][CROOM]);
		set("num", 0, cities[city][0][CROOM]);

		// �����������
		cities[newcity] = copy(cities[city]);

		foreach(object ob in all_inventory(cities[city][0][CROOM]))
		{
			loc = query_temp("location", ob);

			if( !arrayp(loc) ) continue;

			ob->move(arrange_city_location(loc[X], loc[Y], newcity, 0));
		}
		rename("/city/"+city+"/", "/city/"+newcity+"/");



		// �R���¦����
		map_delete(cities, city);
	}

	cities[newcity][INFO]["government/mayor"] = me->query_id(1);
	cities[newcity][INFO]["id"] = newcity;
	cities[newcity][INFO]["color_id"] = cityid;
	cities[newcity][INFO]["name"] = cityname;
	cities[newcity][INFO]["age"] = 1;
	cities[newcity][INFO]["emblem"]			= ({HIR"��"HIY"��"HIG"��"HIB"��"HIM"��"NOR, sprintf("%|10s", cityname), HIM"��"HIB"��"HIG"��"HIY"��"HIR"��"NOR});
	map_delete(cities[newcity][INFO], "fallen");

	CITY_D->register_citizen(me->query_id(1), newcity);
	set("city", newcity, me);

	// �إߪ��ľ���
	MONEY_D->create_new_finance_system(newcity, moneyunit);

	// �إߩЦa�����
	ESTATE_D->create_city_estate(newcity, 0);

	// ���� SHOPPING_D ���
	SHOPPING_D->rehash_storelist();
	SHOPPING_D->rehash_money();

	worldmaploc = MAP_D->query_world_map_location(city, 0);

	if( arrayp(worldmaploc) )
	{
		MAP_D->delete_world_map(worldmaploc[0], worldmaploc[1]);
		MAP_D->set_world_map(worldmaploc[0], worldmaploc[1], newcity, 0);
	}

	// ���s�Ƨǫ�����T
	assign_cities_num();
}

void occupy_section_city(object me)
{
	int num;
	array loc = query_temp("location", me);
	string city, mycity, moneyunit;
	int *worldmaploc;

	mycity = query("city", me);

	if( !city_exist(mycity) || !arrayp(loc) ) return;

	city = loc[CITY];

	while(city_exist(mycity, num)) num++;

	if( !cities[city][INFO]["fallen"] )
		return tell(me, "�o�y�����ä��O�o�󫰥��A�L�k����C\n");

	moneyunit = MONEY_D->city_to_money_unit(mycity);

	if( count(cities[mycity][INFO]["assets"], "<", 1000000000 ) )
		return tell(me, CITY_D->query_city_idname(mycity)+"�������겣�ܤֻݾ֦� $"+moneyunit+" "+NUMBER_D->number_symbol(1000000000)+" �~��A����s���ìP�����C\n");

	CITY_D->change_assets(mycity, "-"+OCCUPY_MONEY);

	// ���� cityroom
	set("city", mycity, cities[city][0][CROOM]);
	set("num", num, cities[city][0][CROOM]);

	// �����������
	cities[mycity][num] = copy(cities[city][0]);
	cities[mycity][INFO]["section"][num] = allocate_mapping(0);

	foreach(object ob in all_inventory(cities[city][0][CROOM]))
	{
		loc = query_temp("location", ob);

		if( !arrayp(loc) ) continue;

		ob->move(arrange_city_location(loc[X], loc[Y], mycity, num));
	}

	mkdir(CITY_NUM(mycity, num));		// �l�����ؿ�
	mkdir(CITY_NUM_ROOM(mycity, num));	// �l�����ж��ؿ�


	tell(me, pnoun(2, me)+"��O�F���F���겣 $"+moneyunit+" "+NUMBER_D->number_symbol(OCCUPY_MONEY)+" ����y�æ���F�o�y�o�󳣥��C\n");
	CHANNEL_D->channel_broadcast("news", me->query_idname()+"��O�F���F���겣 $"+moneyunit+" "+NUMBER_D->number_symbol(OCCUPY_MONEY)+" ��y�æ���F"+query_city_idname(city)+"�A����"+query_city_idname(mycity, num)+"�C");

	// �R���¦����
	map_delete(cities, city);
	rmtree(CITY_PATH(city));

	// �إߩЦa�����
	ESTATE_D->create_city_estate(mycity, num);

	// ���� SHOPPING_D ���
	SHOPPING_D->rehash_storelist();
	SHOPPING_D->rehash_money();

	worldmaploc = MAP_D->query_world_map_location(city, 0);

	if( arrayp(worldmaploc) )
	{
		MAP_D->delete_world_map(worldmaploc[0], worldmaploc[1]);
		MAP_D->set_world_map(worldmaploc[0], worldmaploc[1], mycity, num);
	}

	// ���s�Ƨǫ�����T
	assign_cities_num();
}

// �إߥ��s�a�λP���]���
void make_new_terrain(string city)
{
	int x, y;
	array newmap;
	mapping newdata;

	newdata = allocate_mapping(100);

	// �إ߹w�]�y�и��
	for(y=0;y<MAP_LENGTH;y++)
	{
		newdata[y] = allocate_mapping(1);

		for(x=0;x<MAP_WIDTH;x++)
			newdata[y][x] = copy(DEFAULT_COOR_DATA);
	}

	// �إ߫����зǦa��
	newmap = copy(allocate(MAP_LENGTH, copy(allocate(MAP_WIDTH, "�D"))));

	// �إ߯S��a��
	TERRAIN_D->create_city_map_terrain(ref newmap, ref newdata);

	cities[city][0][MAP] = copy(newmap);
	cities[city][0][RMAP]= copy(newmap);
	cities[city][0][DATA]= copy(newdata);

	log_file(LOG, "�إ߷s�����a�� "+city);

	save_city(city, 0, SAVE_ALL);
}

// �إ߷s�����a��
void create_new_city()
{
	int i;
	string city;

	while(cities[(city = "fallencity"+(++i))]) ;

	// �إ߫����ɮץؿ�
	mkdir(CITY_PATH(city));

	// ��l�ƫ������
	initialize_city_data(city);

	cities[city][INFO]["id"] 		= city;
	cities[city][INFO]["color_id"]		= NOR WHT+capitalize(city)+NOR;


	mkdir(CITY_NUM(city, 0));		// �l�����ؿ�
	mkdir(CITY_NUM_ROOM(city, 0));		// �l�����ж��ؿ�

	make_new_terrain(city);

	// �ҩl���ϤѮ�
	NATURE_D->activate_nature(cities[city][0][CROOM]);

	// ���� SHOPPING_D ���
	SHOPPING_D->rehash_storelist();
	SHOPPING_D->rehash_money();

	log_file(LOG, "�إ߷s�����a�� "+city);

	CHANNEL_D->channel_broadcast("news", HIG"�@�ɦa�ϤW���Y�B�X�{�F�s���ϰ�G"NOR+query_city_idname(city));

	// ���s�s�������x�s����
	assign_cities_num();

	save_city(city, 0, SAVE_ALL);
}


// ��������(���]����)
void city_collapse(string city)
{
	int num;
	object room, inv;
	object citizen, cityroom;
	string id, filename;
	string debtassets, newcity;
	array loc;
	int *worldmaploc;

	if( !city_exist(city) ) return;

	if( count(cities[city][INFO]["assets"], ">=", 0) ) return;

	CHANNEL_D->channel_broadcast("news", query_city_idname(city)+"�t�ŶW�L�T�ѾɭP�F�������˸ѡA�����o���Y���ɰʡA�~���M���A�����س]�}�a�����C");
	CHANNEL_D->channel_broadcast("news", query_city_idname(city)+"�F���Y��F�A�q�������b�o�ӥ@�ɤ��A�ȳѤU���H���l��P�e�j���t�šC");

	debtassets = count(
	    EXCHANGE_D->convert(cities[city][INFO]["assets"], MONEY_D->city_to_money_unit(city), MONEY_D->query_default_money_unit())
	    , "/", CITY_D->query_sections(city));

	// �B�z�������
	foreach( id in cities[city][INFO]["citizens"] )
	{
		citizen = load_user(id);

		if( !objectp(citizen) ) continue;

		delete("city", citizen);

		tell(citizen, pnoun(2, citizen)+"�������Y��F�A"+pnoun(2, citizen)+"�����L���y���C���C\n");

		citizen->save();

		if( !userp(citizen) )
			destruct(citizen);
	}

	while(city_exist(city, num))
	{
		cityroom = cities[city][num][CROOM];

		// �������������Ҧ��ж������Ҧ�����
		foreach( filename in get_dir(CITY_NUM_ROOM(city, num)) )
		{
			room = find_object(CITY_NUM_ROOM(city, num)+filename[0..<3]);

			if( !objectp(room) ) continue;

			foreach( inv in all_inventory(room) )
			{
				if( userp(inv) )
				{
					inv->move(WIZ_HALL);
					msg("$ME�Ҧb�������F���˸ѤF�A�ؿv���D��}�a�A�u�n�y���ܧŮv�j�U�C\n",inv,0,1);
				}
				else
					destruct(inv);
			}
			destruct(room);
		}


		// �����l�����Цa����T
		ESTATE_D->remove_city_estate(city, num);

		// �N�ìP��������ಾ���s����
		if( num > 0 )
		{
			newcity = city+num;

			mkdir(CITY_PATH(newcity));
			mkdir(CITY_NUM(newcity, 0));		// �l�����ؿ�
			mkdir(CITY_NUM_ROOM(newcity, 0));	// �l�����ж��ؿ�

			// �ഫ���
			initialize_city_data(newcity);
			cities[newcity][0] = copy(cities[city][num]);
			cities[newcity][INFO] = copy(cities[city][INFO]);
			cities[newcity][INFO]["id"] = newcity;
			cities[newcity][INFO]["color_id"] = cities[city][INFO]["color_id"]+num;

			reset_city_data(newcity);
			cities[newcity][INFO]["assets"] = debtassets;
			map_delete(cities[city], num);

			// �R���l�����ؿ�
			rmtree(CITY_NUM(city, num));

			// ���� cityroom
			set("city", newcity, cityroom);
			set("num", 0, cityroom);

			cities[newcity][0][CROOM] = cityroom;

			foreach(object ob in all_inventory(cityroom))
			{
				loc = query_temp("location", ob);

				if( !arrayp(loc) ) continue;

				ob->move(arrange_city_location(loc[X], loc[Y], newcity, 0));
			}

			worldmaploc = MAP_D->query_world_map_location(city, num);

			if( arrayp(worldmaploc) )
			{
				MAP_D->delete_world_map(worldmaploc[0], worldmaploc[1]);
				MAP_D->set_world_map(worldmaploc[0], worldmaploc[1], newcity, 0);
			}

		}

		num++;
	}

	cities[city][INFO]["fallen"] = 1;
	reset_city_data(city);
	cities[city][INFO]["assets"] = debtassets;

	// �R���������Ĩt�θ�T
	MONEY_D->remove_finance_system(city);

	// ���� SHOPPING_D ���
	SHOPPING_D->rehash_storelist();
	SHOPPING_D->rehash_money();

	// ���s�s�������x�s����
	assign_cities_num();
}



// �R����ӫ������Ҧ����
void delete_city(string city)
{
	object room, inv;
	object citizen;
	string id, filename;
	int *worldmaploc;

	// �u��R���o�󫰥�
	if( !city_exist(city) || !cities[city][INFO]["fallen"] ) return;

	worldmaploc = MAP_D->query_world_map_location(city, 0);

	// �������������Ҧ��ж������Ҧ�����
	foreach( filename in get_dir(CITY_NUM_ROOM(city, 0)) )
	{
		room = find_object(CITY_NUM_ROOM(city, 0)+filename[0..<3]);

		if( !objectp(room) ) continue;

		foreach( inv in all_inventory(room) )
		{
			if( userp(inv) )
			{
				inv->move(WIZ_HALL);
				msg("$ME�Ҧb���������`�F�A�u�n�y���ܧŮv�j�U�C\n",inv,0,1);
			}
			else
				destruct(inv);
		}
		destruct(room);
	}

	// �����Ҧ��b�������󤤪�����
	foreach( inv in all_inventory(cities[city][0][CROOM]) )
	{
		if( userp(inv) )
		{
			reset_screen(inv);
			inv->move(WIZ_HALL);
			msg("$ME�Ҧb���������`�F�A�u�n�y���ܧŮv�j�U�C\n",inv,0,1);
		}
		else
			destruct(inv);
	}

	// �פ���ϤѮ�
	NATURE_D->deactivate_nature(cities[city][0][CROOM]);

	// �R���l��������
	destruct( cities[city][0][CROOM] );

	// �R���l�����ؿ�
	rmtree(CITY_NUM(city, 0));

	// �R���l�����Ҧ���T
	map_delete(cities[city], 0);

	log_file(LOG, "�R�� "+city+" �������� "+0);
	CHANNEL_D->channel_broadcast("sys", "�R�� "+city+" �������� "+0);


	// �ק�Ҧ��������
	foreach(id in cities[city][INFO]["citizens"])
	{
		if( objectp(citizen = find_player(id)) )
		{
			delete("city", citizen);
			citizen->save();
		}
		else if( objectp(citizen = load_user(id)) )
		{
			delete("city", citizen);
			citizen->save();
			destruct(citizen);
		}
	}

	log_file(LOG,"�R�� "+city+" �����C\n");
	CHANNEL_D->channel_broadcast("news", HIG+query_city_idname(city)+HIG"�b�@�}�ѷn�a�ʫ�I�J�����A�û��q�@�ɦa�ϤW�����C\n"NOR);

	// �M���������
	map_delete(cities, city);

	// �����l�����Цa����T
	ESTATE_D->remove_city_estate(city, 0);

	// ���m���O�t��
	SHOPPING_D->rehash_storelist();
	SHOPPING_D->rehash_money();

	// �R���������Ĩt�θ�T
	MONEY_D->remove_finance_system(city);

	// �Y���@�ɮy��, �h�R��
	if( arrayp(worldmaploc) )
		MAP_D->delete_world_map(worldmaploc[0], worldmaploc[1]);

	// �R�������ؿ�
	rmtree(CITY_PATH(city));

	// ���s�s�ƫ����Ƨ�
	assign_cities_num();
}

/* �]�w�Y�@�����a�ϤW�Y�@�y�Ф��䤤�@���ܼƭ� */
mixed set_coor_data(array loc, mixed prop, mixed data)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( undefinedp(prop) || !mapp(cities[city][num][DATA][y][x]) )
		return 0;

	if( prop == FLAGS )
	{
		if( !intp(data) ) return 0;
		return (cities[city][num][DATA][y][x][prop] |= data);
	}
	else
		return (cities[city][num][DATA][y][x][prop] = data);
}

/* ��@�y�Ф��ܼƭȰ��ƭȥ[��B�z */
mixed add_coor_data(array loc, mixed prop, int data)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( undefinedp(prop) || !mapp(cities[city][num][DATA][y][x]) )
		return 0;

	return (cities[city][num][DATA][y][x][prop] += data);
}

/* �R���@�y�Ф���� */
varargs void delete_coor_data(array loc, mixed prop, int bit)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( !mapp(cities[city][num][DATA][y][x]) ) return;

	// �Y�L prop �h���s��l�Ʃж���T, �H�f�����쪬
	if( !prop )
	{
		if(this_object()->query_coor_data(loc, TYPE) == BRIDGE)
		{
			this_object()->set_coor_data(loc, TYPE, RIVER);
			this_object()->set_coor_icon(loc, BBLU HIB"��"NOR);
			this_object()->delete_coor_data(loc, "owner");
		}
		else
		{
			int origin_region = CITY_D->query_coor_data(loc, "region");

			cities[city][num][DATA][y][x] = DEFAULT_COOR_DATA;

			switch( origin_region  )
			{
			case AGRICULTURE_REGION:
				this_object()->set_coor_data(loc, 6, "100000");
				this_object()->set_coor_data(loc, "region", AGRICULTURE_REGION);
				break;
			case INDUSTRY_REGION:
				this_object()->set_coor_data(loc, 6, "200000");
				this_object()->set_coor_data(loc, "region", INDUSTRY_REGION);
				break;
			case COMMERCE_REGION:
				this_object()->set_coor_data(loc, 6, "300000");
				this_object()->set_coor_data(loc, "region", COMMERCE_REGION);
				break;
			}
		}
	}

	else if( prop == FLAGS )
	{
		if( !undefinedp(bit) )
			cities[city][num][DATA][y][x][prop] ^= bit;
		else
			map_delete(cities[city][num][DATA][y][x], prop);
	}
	else
	{
		map_delete(cities[city][num][DATA][y][x], prop);

		// �s�էR���\��
		foreach(mixed ori_prop, mixed data in cities[city][num][DATA][y][x])
		{
			if( stringp(ori_prop) && stringp(prop) && sscanf(ori_prop, prop+"/%*s") == 1 )
				map_delete(cities[city][num][DATA][y][x], ori_prop);
		}
	}
}


/* �Ǧ^�Y�@�����a�ϤW�Y�@�y�Ф��䤤�@���ܼƭ� */
varargs mixed query_coor_data(array loc, mixed prop)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( undefinedp(prop) )
		return cities[city][num][DATA][y][x];
	else
		return cities[city][num][DATA][y][x][prop];
}

array query_all_coor_data(string city, int num)
{
	if( !city_exist(city, num) )
		return 0;

	return cities[city][num][DATA];
}

/* �d��ʬd�M, �Ǧ^��ư}�C */
/* ��Υb�|��Ƹs�j�M�k, �j�M�ɶ��P radius ������e���� */
mapping query_coor_range(array loc, string prop, int radius)
{
	string city;
	int num, x, y;
	int i, j, xf, xt, yf, yt;
	mapping coor, data = allocate_mapping(0);

	if( !arrayp(loc) ) return 0;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	coor = cities[city][num][DATA];

	if( radius < 1 ) return ([ save_variable(loc):coor[y][x][prop] ]);

xf = x-radius; if( xf < 0 ) 	xf = 0;
xt = x+radius; if( xt > 99 ) 	xt = 99;
yf = y-radius; if( yf < 0 )	yf = 0;
yt = y+radius; if( yt > 99 )	yt = 99;

radius *= radius;

for(i=xf;i<=xt;i++)
	for(j=yf;j<=yt;j++)
		if( (i-x)*(i-x) + (j-y)*(j-y) <= radius )
			data[save_variable(arrange_city_location(i,j,city,num))] = coor[j][i][prop];
return data;
}
/* �]�w�y�Цa�Ϲϧ� */
void set_coor_icon(array loc, string icon)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) || noansi_strlen(icon) != 2 ) return;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( valid_coordinate(x, y, city, num) )
		cities[city][num][MAP][y][x] = cities[city][num][RMAP][y][x] = icon;
}

/* �]�w�y�Цa�Ϲϧ� */
void set_real_time_coor_icon(array loc, string icon)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) || noansi_strlen(icon) != 2 ) return;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( valid_coordinate(x, y, city, num) )
		cities[city][num][RMAP][y][x] = icon;
}

/* �Ǧ^�y�Цa�Ϲϧ� */
varargs string query_coor_icon(array loc, int raw)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	return !undefinedp(raw) ? remove_ansi(cities[city][num][MAP][y][x]) : cities[city][num][MAP][y][x];
}

/* ���]�y�Цa�Ϲϧ� */
void reset_coor_icon(array loc)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	cities[city][num][RMAP][y][x] = cities[city][num][MAP][y][x];
}

// �R���y�Цa�Ϲϧ�
void delete_coor_icon(array loc)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	switch( CITY_D->query_coor_data(loc, "region")  )
	{
	case AGRICULTURE_REGION:
		cities[city][num][RMAP][y][x] = BGRN"�D"NOR;
		cities[city][num][MAP][y][x] = BGRN"�D"NOR;
		break;
	case INDUSTRY_REGION:
		cities[city][num][RMAP][y][x] = BYEL"�D"NOR;
		cities[city][num][MAP][y][x] = BYEL"�D"NOR;
		break;
	case COMMERCE_REGION:
		cities[city][num][RMAP][y][x] = BCYN"�D"NOR;
		cities[city][num][MAP][y][x] = BCYN"�D"NOR;
		break;
	default:
		cities[city][num][RMAP][y][x] = "�D";
		cities[city][num][MAP][y][x] = "�D";
		break;
	}
}

/* �]�w�����򥻸�T */
mixed set_city_info(string city, string prop, mixed info)
{
	if( !stringp(city) || !stringp(prop) || undefinedp(info) || !city_exist(city) ) return 0;

	return cities[city][INFO][prop] = info;
}

/* �^�ǫ����򥻸�T */
varargs mixed query_city_info(string city, string prop)
{
	if( !stringp(city) || !city_exist(city) || !cities[city] || !cities[city][INFO] ) return 0;

	if( !undefinedp(prop) )
		return copy(cities[city][INFO][prop]);
	else
		return copy(cities[city][INFO]);
}

/* �R�������򥻸�T */
int delete_city_info(string city, string prop)
{
	if( !stringp(city) || !stringp(prop) || !city_exist(city) ) return 0;

	map_delete(cities[city][INFO], prop);

	return 1;
}
/* �]�w�������ϰ򥻸�T */
mixed set_section_info(string city, int num, string prop, mixed info)
{
	if( !stringp(city) || !stringp(prop) || undefinedp(info) || !city_exist(city, num) ) return 0;

	return cities[city][INFO]["section"][num][prop] = info;
}

/* �^�ǫ������ϰ򥻸�T */
varargs mixed query_section_info(string city, int num, string prop)
{
	if( !stringp(city) || !city_exist(city, num) ) return 0;

	if( !undefinedp(prop) )
		return copy(cities[city][INFO]["section"][num][prop]);
	else
		return copy(cities[city][INFO]["section"][num]);
}

/* �R���������ϰ򥻸�T */
int delete_section_info(string city, int num, string prop)
{
	if( !stringp(city) || !stringp(prop) || !city_exist(city, num) ) return 0;

	map_delete(cities[city][INFO]["section"][num], prop);

	return 1;
}


/* ���ܫ����겣 */
string change_assets(string city, mixed money)
{
	string new_assets;

	if( !stringp(city) || !money || !city_exist(city) ) return 0;

	if( !(money = big_number_check(money)) )
		return copy(query_city_info(city, "assets"));

	new_assets = count(query_city_info(city, "assets"), "+", money);

	if( count(new_assets, "<", 0) )
	{
		int crashtime = query_city_info(city, "crashtime");

		if( !crashtime )
			crashtime = set_city_info(city, "crashtime", time()+72*3600);

		crashtime -= time();

		if( count(money, ">", 0) )
			set_city_info(city, "tmpassets", new_assets);
		else if( count(count(new_assets, "-", query_city_info(city, "tmpassets")), "<", -10000000) )
		{
			if( crashtime > 0 )
				CHANNEL_D->channel_broadcast("news", query_city_idname(city)+"���F���t�� $"+MONEY_D->city_to_money_unit(city)+" "+NUMBER_D->number_symbol(new_assets)+"�A���{�]�F�M���A�˼� "+(crashtime/86400)+" �� "+(crashtime%86400/60/60)+" �� "+(crashtime%3600/60)+" �������Y�췴���C");
			else
				CHANNEL_D->channel_broadcast("news", query_city_idname(city)+"���F���t�� $"+MONEY_D->city_to_money_unit(city)+" "+NUMBER_D->number_symbol(new_assets)+"�A���{�]�F�M���A�����Y�N�Y�췴���C");

			set_city_info(city, "tmpassets", new_assets);
		}
	}
	else if( query_city_info(city, "crashtime") )
	{
		delete_city_info(city, "crashtime");
		delete_city_info(city, "tmpassets");
		CHANNEL_D->channel_broadcast("news", query_city_idname(city)+"���F���v�٭t�šA�Ѱ��]�F�M���C");
	}


	return set_city_info(city, "assets", new_assets);
}

string query_assets(string city)
{
	return query_city_info(city, "assets");
}

// �ק﫰���`�W��
void change_city_name(string city, string name)
{
	int len;

	if( !city_exist(city) || !stringp(name) ) return 0;

	len = noansi_strlen(name);
	if(  len > 10 || len % 2 ) return 0;

	// �D�¤��大 city name
	foreach( int a in remove_ansi(name) )
	if(  a <= ' ' || --len%2 && (a < 160 || a > 255) )
		return 0;

	name = kill_repeat_ansi(remove_fringe_blanks(name)+NOR);

	CHANNEL_D->channel_broadcast("news", query_city_idname(city)+"������W���u"+name+"�v");

	set_city_info(city, "name", name);
}

// �ק�l�����W��
void change_section_name(string city, int num, string name)
{
	int len;
	string original_idname;

	if( !city_exist(city, num) || !stringp(name) ) return 0;

	len = noansi_strlen(name);
	if(  len > 10 || len % 2 ) return 0;

	// �D�¤��大 city name
	foreach( int a in remove_ansi(name) )
	if(  a <= ' ' || --len%2 && (a < 160 || a > 255) )
		return 0;

	name = kill_repeat_ansi(remove_fringe_blanks(name)+NOR);

	original_idname = query_city_idname(city, num);

	set_section_info(city, num, "name", name);

	CHANNEL_D->channel_broadcast("news", original_idname+"������W���u"+query_city_idname(city, num)+"�v");
}



/* �Ǧ^�Ҧ������W�� */
string *query_cities()
{
	return filter_array(keys(cities), (: stringp($1) :));
}

int query_sizeof_cities()
{
	return sizeof(query_cities());
}

/* �Ǧ^�Y������, �Ҧ������� ID */
string *query_citizens(string city)
{
	if( !city_exist(city) ) return 0;

	return cities[city][INFO]["citizens"];
}

object query_maproom(array loc)
{
	string city = loc[CITY];
	int num = loc[NUM];

	if( !city_exist(city, num) ) return 0;

	return cities[city][num][CROOM];
}

object *query_all_maproom()
{
	object *all_maproom = allocate(0);

	foreach( string city, mapping m1 in cities )
	foreach( int num, mapping m2 in m1 )
	{
		if( intp(num) )
			all_maproom += ({ m2[CROOM] });
	}
	return all_maproom;
}

varargs array query_map(array loc, int realtime)
{
	string city = loc[CITY];
	int num = loc[NUM];

	if( !city_exist(city, num) ) return 0;

	return realtime ? cities[city][num][RMAP] : cities[city][num][MAP];
}

/* �Ǧ^�b�Y���������Ҧ����a */
varargs object *query_city_players(string city, int num)
{
	if( undefinedp(num) )
	{
		if( city_exist(city) )
			return filter_array(users(), (: query_temp("in_city/id",$1) == $(city) :));

		return 0;
	}

	if( city_exist(city, num) )
		return filter_array(users(), (: query_temp("in_city/id",$1) == $(city) && query_temp("in_city/num",$1) == $(num) :));

	return 0;
}

/* �Ǧ^�b�Y���������Ҧ���~���a */
varargs object *query_outdoor_players(string city, int num)
{
	if( undefinedp(num) )
	{
		if( city_exist(city) )
		{
			object *outdoor_players = allocate(0);

			foreach( num in keys(cities[city]) )
			if( intp(num) )
				outdoor_players += filter_array(all_inventory(cities[city][num][CROOM]), (: userp($1) :));

			return outdoor_players;
		}
		return 0;
	}

	if( city_exist(city, num) )
		return filter_array(all_inventory(cities[city][num][CROOM]), (: userp($1) :));

	return 0;
}

mapping query_cities_info()
{
	return copy(cities);
}

int query_sections(string city)
{
	return sizeof(filter_array(keys(cities[city]), (: intp($1) :)));
}

int is_citizen(string id, string city)
{
	if( !id || !city_exist(city) ) return 0;

	return member_array(id, cities[city][INFO]["citizens"]) != -1;
}

// ���U��������
int register_citizen(string id, string city)
{
	if( !id || !city_exist(city) ) return 0;

	cities[city][INFO]["citizens"] |= ({ id });
	cities[city][INFO]["citizens_jointime"][id] = time();

	return 1;
}

// ������������
int remove_citizen(string id)
{
	foreach( string city, mapping info in cities )
	{
		if( !cities[city] || !cities[city][INFO] || !cities[city][INFO]["citizens"]
		    || member_array(id, cities[city][INFO]["citizens"]) == -1 )
			continue;

		cities[city][INFO]["citizens"] -= ({ id });
		map_delete(cities[city][INFO]["citizens_jointime"], id);

		if( cities[city][INFO]["government/mayor"] == id )
			cities[city][INFO]["government/mayor"] = 0;

		if( cities[city][INFO]["government/officer"][id] ) {
			map_delete(cities[city][INFO]["government/officer"], id);
		}

		return 1;
	}

	return 0;
}

string query_player_city(string id)
{
	foreach( string city, mapping info in cities )
	{
		if( !mapp(info) || undefinedp(info[INFO]) ) continue;

		if( member_array(id, info[INFO]["citizens"]) != -1 )
			return city;
	}
	return 0;
}

mapping query_action(array loc)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( !city_exist(city, num) || !valid_coordinate(x, y, city, num) || undefinedp(cities[city][num][DATA])) return 0;

	switch( cities[city][num][DATA][y][x][TYPE] )
	{
	case PAVILION:
	case TREE:
	case GRASS:
		return ([
		    "sit":		([ 0:COMMAND_D->find_sub_command("city", "sit") ]),
	    ]);
default: return 0;
}
}

string position(int x, int y)
{
	switch(x)
	{
	case 0..32:
		switch(y)
		{
		case 0..32:	return "��_��";
		case 33..65:	return "����";
		case 66..99:	return "��n��";
		default:break;
		}
		break;
	case 33..65:
		switch(y)
		{
		case 0..32:	return "�_��";
		case 33..65:	return "����";
		case 66..99:	return "�n��";
		default:break;
		}
		break;
	case 66..99:
		switch(y)
		{
		case 0..32:	return "�F�_��";
		case 33..65:	return "�F��";
		case 66..99:	return "�F�n��";
		default:break;
		}
		break;
	default:break;
	}

	return 0;
}

//   1  2   4
//   8      16
//   32 64  128
int direction_check(int x, int y, string city, int num)
{
	int flag;
	mapping coor, data = cities[city][num][DATA];

	coor = data[y];

	if( x 			&& !(coor[x-1][FLAGS] & NO_MOVE)) 	flag |= 8;
	if( x!=99 		&& !(coor[x+1][FLAGS] & NO_MOVE)) 	flag |= 16;

	coor = data[y-1];

	if( x && y 		&& !(coor[x-1][FLAGS] & NO_MOVE)) 	flag |= 1;
	if( y && x!=99  	&& !(coor[x+1][FLAGS] & NO_MOVE)) 	flag |= 4;
	if( y 			&& !(coor[x][FLAGS] & NO_MOVE)) 	flag |= 2;

	coor = data[y+1];
	if( x && y!=99 		&& !(coor[x-1][FLAGS] & NO_MOVE)) 	flag |= 32;
	if( y!=99 		&& !(coor[x][FLAGS] & NO_MOVE)) 	flag |= 64;
	if( y!=99 && x!=99 	&& !(coor[x+1][FLAGS] & NO_MOVE)) 	flag |= 128;

	return flag;
}

// �^�ǫ����y�л���
string query_coor_help(array loc, string prop, int radius)
{
	string city = loc[CITY];
	int num = loc[NUM], y = loc[Y], x = loc[X];

	switch( cities[city][num][DATA][y][x][TYPE] )
	{
	case FARM:
	case PASTURE:
	case FISHFARM:
		return @GROWTH_HELP
    �Ͳ��覡�аѾ\ help grow
GROWTH_HELP;

	case ROAD:
		return @ROAD_HELP
    �o�O�@���g�ѥ��F���W���n���D���A�i�Ѧ�H�P�����樫�A�Y�O�}�����̦ܳn
�ɶq���b���Z�������W�A�Y�O���N�}�쥼�}�o���g�a�W���ܡA�ܥi��|�⨮�����L
���a��I

ROAD_HELP;
	default: return 0;
	}
}

string coor_short_name(array loc)
{
	string owner = CITY_D->query_coor_data(loc, "owner");
	string personal_short = CITY_D->query_coor_data(loc ,"short");

	if( owner )
	{
		if( belong_to_government(owner) )
			owner = query_city_name(owner[11..]);
		else if( belong_to_enterprise(owner) )
			owner = ENTERPRISE_D->query_enterprise_color_id(owner[11..]);
		else
			owner = capitalize(owner)+" ";
	}

	switch( CITY_D->query_coor_data(loc, TYPE) )
	{
	case 0:
		{
			if( owner )
			{
				return owner+"��"WHT"�g�a"NOR;
			}
			else
			{
				switch( CITY_D->query_coor_data(loc, "region") )
				{
				case AGRICULTURE_REGION:
					return GRN"�A�~�W����"NOR;
				case INDUSTRY_REGION:
					return YEL"�u�~�W����"NOR;
				case COMMERCE_REGION:
					return CYN"�ӷ~�W����"NOR;
				default:
					return HIY"��"NOR YEL"�a"NOR;
				}
			}
			break;
		}
	case ROAD:
		return (CITY_D->query_coor_data(loc ,"roadname"))||(HIW"�D"NOR WHT"��"NOR);
		break;
	case BRIDGE:
		return (CITY_D->query_coor_data(loc ,"bridgename"))||(HIR"��"NOR RED"��"NOR);
		break;
	case FARM:
		return personal_short||(owner + "��"HIG"�A��"NOR);
		break;
	case PASTURE:
		return personal_short||(owner + "��"NOR YEL"����"NOR);
		break;
	case FISHFARM:
		return personal_short||(owner + "��"HIB"�i�޳�"NOR);
		break;
	case DOOR:
		{
			// �ӶO�į�L�j�� mark �_��
			//mapping estdata = ESTATE_D->query_loc_estate(loc);
			//array building_info;

			//if( estdata )
			//	building_info = BUILDING_D->query_building(estdata["type"]);

			//if( !building_info )
				return personal_short || (owner+"���ؿv�����f");
			//else
			//	return personal_short || (owner+"��"+building_info[0]);
			break;
		}

	case WALL:
		return HIM + owner + HIM"���ؿv�u�a";
		break;

	case RIVER:
		{
			switch( CITY_D->query_coor_data(loc, FLAGS) )
			{
			case FLOW_NORTH:
				return HBBLU"�y�V���_���e�y"NOR;
				break;
			case FLOW_SOUTH:
				return HBBLU"�y�V���n���e�y"NOR;
				break;
			case FLOW_EAST:
				return HBBLU"�y�V���F���e�y"NOR;
				break;
			case FLOW_WEST:
				return HBBLU"�y�V���誺�e�y"NOR;
				break;
			default:
				return HBBLU"�e�y"NOR;
				break;
			}
			break;
		}

	case FOREST:
		return NOR HBGRN"��L";
		break;
	case MOUNTAIN:
		return NOR HBYEL"�s��";
		break;

	case LIGHT:
		return personal_short||(owner + NOR"��"HBYEL"���O"NOR);
	case POOL:
		return personal_short||(owner + NOR"��"HBCYN"����"NOR);
	case PAVILION:
		return personal_short||(owner + NOR"��"HBGRN"�D�F"NOR);
	case TREE:
		return personal_short||(owner + NOR"��"HBGRN"�j��"NOR);
	case GRASS:
		return personal_short||(owner + NOR"��"HBGRN"��a"NOR);
	case STATUE:
		return personal_short||(owner + NOR"��"HBWHT"�J��"NOR);
	default:
		return "�������g�a";
		break;
	}
}

mixed cookys_test()
{
	return cities;
}

// x_start , y_start ���i���d�� x,y �y�Ъ��_�l��
// x_center �� y_center �O�Ψӭp��q���}�l�H����m���b�����i�۰ʦ��Y
// x �O��y�СA y �O�����y�СAcity �O�����W�Anum �h�O�ĴX�Ӥl���A���ǫh�O�D��[0]
// Code �ݭn�A��z
string show_map(array loc)
{
	int i, j, k, l, dir, x, y, num, range,
	x_start, y_start,
	x_center, y_center;

	string city, retmap, emblem;
	string *column = allocate(CITY_VISION_LENGTH);
	array map2, map3;

	if( !loc ) return 0;

	x	=loc[X];
	y	=loc[Y];
	city	=loc[CITY];
	num	=loc[NUM];

	x_center = (CITY_VISION_WIDTH+1)/2;
	y_center = (CITY_VISION_LENGTH+1)/2;

	if( !valid_coordinate(x, y, city, num) ) return "���~���a�ϰ϶��I";

	// ��ܰ϶��p�⤽��
	x_start = (x>=x_center)*(x+1-x_center) - ((x+x_center)>MAP_WIDTH)*(x_center+x-MAP_WIDTH);
	y_start = (y>=y_center)*(y+1-y_center) - ((y+y_center)>MAP_LENGTH)*(y_center+y-MAP_LENGTH);

	range = CITY_VISION_WIDTH/3;

	switch((TIME_D->query_gametime_array())[HOUR])
	{
		case 0..4	: range -= 2;break;
		case 20..23	: range -= 2;break;
	}

	switch( NATURE_D->query_nature(MAP_D->query_maproom(loc))[NATURE_WEATHER][WEATHER_ID] )
	{
		case SUNNY:		break;
		case CLOUDY:		break;
		case RAINY:		range -= 1; break;
		case SNOWY:		range -= 2; break;
		case SHOWER:		range -= 1; break;
		case TYPHOON:		range -= 2; break;
		case FOEHN:		break;
	}

	range*=range;

	// �ϤW������u
	retmap  = SAVEC;	// �x�s��Ц�m
	retmap += "\e[1;1H";	// ��в��m�e�� 1, 1
	retmap += NOR WHT"�z"+sprintf(HIG" %|3d "NOR+WHT"��"+HIG" %|3d "NOR+WHT+"�w"HIG" %|2d "NOR WHT, x+1, y+1, num+1)+"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�s�w�w�w�w�w�{";

	dir = direction_check(x, y, city, num);

	emblem = query_city_info(city, "emblem");

	column[0]	=NOR WHT"�x"NOR+emblem[0]+NOR WHT"�x";
	column[1]	=NOR WHT"�x"NOR+emblem[1]+NOR WHT"�x";
	column[2]	=NOR WHT"�x"NOR+emblem[2]+NOR WHT"�x";
	column[3]	=NOR WHT"�x"HIW+TIME_D->gametime_digital_clock()+WHT"�x";
	column[4]	=NOR WHT"�x"HIW"�Ѯ�G"+(NATURE_D->query_nature(MAP_D->query_maproom(loc)))[NATURE_WEATHER][WEATHER_CHINESE]+WHT+"�x";
	column[5]	=NOR WHT"�u�w�w�w�w�w�t";
	column[6] 	=NOR WHT"�x  "+(dir&1?HIW"��":NOR WHT"��")	+(dir&2?HIW"��":NOR WHT"�x")	+(dir&4?HIW"��":NOR WHT"��")	+NOR WHT"  �x";
	column[7]	=NOR WHT"�x  "+(dir&8?HIW"��":NOR WHT"�w")	+HIW+"��"+NOR WHT		+(dir&16?HIW"��":NOR WHT"�w")	+NOR WHT"  �x";
	column[8]	=NOR WHT"�x  "+(dir&32?HIW"��":NOR WHT"��")	+(dir&64?HIW"��":NOR WHT"�x")	+(dir&128?HIW"��":NOR WHT"��")	+NOR WHT"  �x";

	map2 = copy(cities[city][num][RMAP][y_start..y_start+CITY_VISION_LENGTH-1]);
	map3 = allocate(CITY_VISION_LENGTH, 0);

	if( range == (CITY_VISION_WIDTH/3)*(CITY_VISION_WIDTH/3) )
	{
		map3 = allocate(CITY_VISION_LENGTH, allocate(CITY_VISION_WIDTH, 1));
		for(j=0;j<CITY_VISION_LENGTH;j++)
			map2[j] = map2[j][x_start..x_start+CITY_VISION_WIDTH-1];
	}
	else
	{
		for(j=0;j<CITY_VISION_LENGTH;j++)
			map3[j] = allocate(CITY_VISION_WIDTH);

		for(j=0;j<CITY_VISION_LENGTH;j++)
		{
			map2[j] = map2[j][x_start..x_start+CITY_VISION_WIDTH-1];

			for(i=0;i<CITY_VISION_WIDTH;i++)
			{
				if( map2[j][i] == HIY"�F"NOR )
				{
					for(k=j-4;k<=j+4&&k<CITY_VISION_LENGTH;k++)
						for(l=i-4;l<=i+4&&l<CITY_VISION_WIDTH;l++)
						{
							if( k>=0 && l>=0 && (l-i)*(l-i) + (k-j)*(k-j) <= 16 )
								map3[k][l] = 1;
						}
				}
				else if( !map3[j][i] && (i-x+x_start)*(i-x+x_start)*0.5 + (j-y+y_start)*(j-y+y_start) <= range )
					map3[j][i] = 1;
			}
		}
	}

	for(j=0;j<CITY_VISION_LENGTH;j++)
	{
		retmap += "\e["+(j+2)+";1H"NOR WHT"�x"NOR;	// ��в��ܵe�� j+2, 1

		for(i=0;i<CITY_VISION_WIDTH;i++)
		{
			if( map3[j][i] )
			{
				if( j == y-y_start && i == x-x_start )
					retmap += ansi_part(map2[j][i])+HIC"��"NOR;
				else
					retmap += map2[j][i];
			}

			else
				retmap += WHT"��"NOR;
		}
		retmap += column[j]+"\n"NOR;
	}

	// �ϤU����u
	retmap += "\e["+(j+2)+";1H";	// ��в��ܵe�� j+2, 1
	retmap += sprintf(NOR WHT"�|["NOR"%|32s"NOR WHT"]�w�w�w"NOR"%|10s"NOR WHT,
	    coor_short_name(loc),
	    CITY_D->query_section_info(city, num, "name") || (WHT"��"+CHINESE_D->chinese_number(num+1)+"����")
	) + "�r�w�w�w�w�w�}";

	retmap += "\e["+(j+3)+";1H";	// ��в��ܵe�� j+3, 1
	retmap += NOR YEL"_________________________________________________________���Ϧa��_\e[u"NOR;
	retmap += REST; // ��^�w�x�s����Ц�m

	return retmap;
}


mapping corresponding =
([
    "north"		:	"south",
    "south"		:	"north",
    "east"		:	"west",
    "west"		:	"east",
    "northwest"	:	"southeast",
    "northeast"	:	"southwest",
    "southwest"	:	"northeast",
    "southeast"	:	"northwest",
    "up"		:	"down",
    "down"		:	"up",
]);

mapping weather_go =
([
    SUNNY		:	"���Y�j���a",
    CLOUDY		:	"���H�۩M��",
    RAINY		:	"�}����۫B��",
    WINDY		:	"���й���",
    SNOWY		:	"�a�۴����ճ�",
    SHOWER		:	"�q�Y��}��O�O�a",
    TYPHOON		:	"�����@��k��a",
    FOEHN		:	"���ۼ���"
]);


varargs varargs int valid_move(object me, string direction, array loc)
{
	int x, y, num, *worldmaploc, maplink;

	mapping data;
	string city;
	array nextloc;

	if( !objectp(me) ) return 0;

	if( !loc )
		loc = query_temp("location", me);

	if( !arrayp(loc) )
		return 0;

	x = copy(loc[X]);
	y = copy(loc[Y]);
	city = loc[CITY];
	num = loc[NUM];

	worldmaploc = MAP_D->query_world_map_location(city, num);

	switch(direction)
	{
	case "north":		y--;		break;
	case "south":		y++;		break;
	case "west":		x--;		break;
	case "east":		x++;		break;
	case "northeast":	y--;x++; 	break;
	case "southwest":	y++;x--;  	break;
	case "northwest":	y--;x--; 	break;
	case "southeast":	y++;x++; 	break;
	case "maplink"	:	break;
	default: return 0;
	}

	if( y < 0 )
	{
		if( !arrayp(worldmaploc) ) return 0;

		y = MAP_LENGTH-1;
		worldmaploc[1]++;
		maplink = 1;
	}
	else if( y >= MAP_LENGTH )
	{
		if( !arrayp(worldmaploc) ) return 0;

		y = 0;
		worldmaploc[1]--;
		maplink = 1;
	}

	if( x < 0 )
	{
		if( !arrayp(worldmaploc) ) return 0;

		x = MAP_WIDTH-1;
		worldmaploc[0]--;
		maplink = 1;
	}
	else if( x >= MAP_WIDTH )
	{
		if( !arrayp(worldmaploc) ) return 0;

		x = 0;
		worldmaploc[0]++;
		maplink = 1;
	}

	if( maplink )
	{
		array linked_map = MAP_D->query_world_map(worldmaploc[0], worldmaploc[1]);

		if( linked_map )
		{
			nextloc = arrange_area_location(x, y, linked_map[0], linked_map[1]);

			return MAP_D->valid_move(me, "maplink", nextloc);
		}
		return 0;
	}
	nextloc = arrange_city_location(x, y, city, num);

	if( !valid_coordinate(x, y, city, num) )
		return 0;


	data = CITY_D->query_coor_data(nextloc);

	if( data[FLAGS] & NO_MOVE )
	{
		if( data[TYPE] == WALL )
			return -2;
	}

	switch(data[TYPE])
	{
	case DOOR:
		if( data["lock"] & LOCKED || (!wizardp(me) && data["lock"] & WIZLOCKED) )
			return -1;
		break;
	}

	return 1;
}

varargs void move(object me, string direction, int automove)
{
	int x, y, num, cost_stamina, *worldmaploc, maplink;
	mapping data;
	string city, my_idname;
	array loc, nextloc;
	string random_thing = RANDOM_STRING[random(sizeof(RANDOM_STRING))];

	if( !objectp(me) || !arrayp(loc = query_temp("location",me)) ) return;

	x = copy(loc[X]);
	y = copy(loc[Y]);
	city = loc[CITY];
	num = loc[NUM];

	if( !city_exist(city) ) return;

	worldmaploc = MAP_D->query_world_map_location(city, num);

	my_idname = me->query_idname();

	switch(direction)
	{
	case "north":		y--;		break;
	case "south":		y++;		break;
	case "west":		x--;		break;
	case "east":		x++;		break;
	case "northeast":	y--;x++; 	break;
	case "southwest":	y++;x--;  	break;
	case "northwest":	y--;x--; 	break;
	case "southeast":	y++;x++; 	break;
	case "maplink"	:	break;
	default: return 0;
	}

	if( y < 0 )
	{
		if( !arrayp(worldmaploc) ) return 0;

		y = MAP_LENGTH-1;
		worldmaploc[1]++;
		maplink = 1;
	}
	else if( y >= MAP_LENGTH )
	{
		if( !arrayp(worldmaploc) ) return 0;

		y = 0;
		worldmaploc[1]--;
		maplink = 1;
	}

	if( x < 0 )
	{
		if( !arrayp(worldmaploc) ) return 0;

		x = MAP_WIDTH-1;
		worldmaploc[0]--;
		maplink = 1;
	}
	else if( x >= MAP_WIDTH )
	{
		if( !arrayp(worldmaploc) ) return 0;

		x = 0;
		worldmaploc[0]++;
		maplink = 1;
	}

	if( maplink )
	{
		array linked_map = MAP_D->query_world_map(worldmaploc[0], worldmaploc[1]);

		if( sizeof(linked_map) == 2 )
		{
			nextloc = arrange_area_location(x, y, linked_map[0], linked_map[1]);

			broadcast(loc, my_idname+"��"+CHINESE_D->to_chinese(direction)+"���}�C\n", 0, me);
			me->move(nextloc);
			broadcast(nextloc, my_idname+weather_go[NATURE_D->query_nature(query_maproom(loc))[NATURE_WEATHER][WEATHER_ID]]+"�A��"+CHINESE_D->to_chinese(corresponding[direction])+"���F�L�ӡC\n", 0, me);

			me->follower_move(loc, nextloc);
			return;
		}
		return 0;
	}
	nextloc = arrange_city_location(x, y, city, num);

	data = CITY_D->query_coor_data(nextloc);

	delete_temp("go_resist", me);

	// �q�������ؿv������
	if( stringp(data[ROOM]) )
	{
		// �S��X�f����
		if( data[SPECIAL_EXIT] )
		{
			broadcast(loc, my_idname+"��"+CHINESE_D->to_chinese(direction)+"���}�C\n", 0, me);
			me->move(data[SPECIAL_EXIT]);
			broadcast(data[SPECIAL_EXIT], my_idname+weather_go[NATURE_D->query_nature(query_maproom(loc))[NATURE_WEATHER][WEATHER_ID]]+"�A��"+CHINESE_D->to_chinese(corresponding[direction])+"���F�L�ӡC\n", 0, me);

			me->follower_move(loc, data[SPECIAL_EXIT]);

			return;
		}
		else
		{
			msg("$ME���}�F"+CHINESE_D->to_chinese(direction)+"�����i�J�F"+BUILDING_D->query_building(data[ROOM])[0]+"�C\n",me, 0, 1);
			me->move(CITY_ROOM_MODULE(city, num, x, y, data[ROOM]));
			broadcast(CITY_ROOM_MODULE(city, num, x, y, data[ROOM]), my_idname+"���}�F"+CHINESE_D->to_chinese(corresponding[direction])+"�����A"+weather_go[NATURE_D->query_nature(query_maproom(loc))[NATURE_WEATHER][WEATHER_ID]]+"�A�«Τ����F�i�ӡC\n", 0, me);

			me->follower_move(loc, CITY_ROOM_MODULE(city, num, x, y, data[ROOM]));

			return;
		}
	}

	remove_call_out(query_temp("call_out_handle/flowing", me));

	// �ˬd�S��g�a�κA������
	switch(data[TYPE])
	{
	case RIVER:
		cost_stamina = 100 - me->query_skill_level("sport")/2;

		switch( CITY_D->query_coor_data(nextloc, FLAGS) )
		{
		case FLOW_NORTH	: direction = "north"; break;
		case FLOW_SOUTH	: direction = "south"; break;
		case FLOW_EAST	: direction = "east"; break;
		case FLOW_WEST	: direction = "west"; break;
		default: direction = 0; break;
		}

		if( automove )
		{
			if( direction )
				msg("$ME�H�۪e�y�C�C�a��"+CHINESE_D->to_chinese(direction)+"�ƤF�L�h�C\n", me, 0, 1);
			else
				return msg("$ME�b�e���W���B���I�C\n", me, 0, 1);
		}
		else if( !me->cost_stamina(cost_stamina) )
			return tell(me, pnoun(2, me)+"�S����O�e�i�F�C\n");
		else
			msg("$ME�ĤO�a��"+CHINESE_D->to_chinese(direction)+"��L�h�A��O "+cost_stamina+" �I��O�C\n",me,0,1);

		if( !random(100) )
		{
			msg("$ME��a���@�b�A��M���Q�x�����F�U�h�A���ѳ������a���W������"+HIY+random_thing+NOR+"�C\n", me, 0, 1);
			me->faint();
		}

		me->add_social_exp(random(3));
		set_temp("call_out_handle/flowing",call_out( (: move, me, direction, 1 :), 2), me);
		me->move(nextloc);

		me->follower_move(loc, nextloc);
		return;
		break;

	case FOREST:
		cost_stamina = 25 - me->query_skill_level("sport")/5;

		if( !me->cost_stamina(cost_stamina) )
			return tell(me, pnoun(2, me)+"�S����O�e�i�F�C\n");

		msg("$ME�p�ߦa��"+CHINESE_D->to_chinese(direction)+"���@�K�˪L�e�i�A��O "+cost_stamina+" �I��O�C\n",me,0,1);
		me->add_social_exp(random(2));
		me->move(nextloc);
		me->follower_move(loc, nextloc);
		return;
		break;

	case MOUNTAIN:
		cost_stamina = 90 - me->query_skill_level("sport")/2;

		if( !me->cost_stamina(cost_stamina) )
			return tell(me, pnoun(2, me)+"�S����O�e�i�F�C\n");

		msg("$ME��ݼN�N�a��"+CHINESE_D->to_chinese(direction)+"���s�߫e�i�A��O "+cost_stamina+" �I��O�C\n",me,0,1);
		me->add_social_exp(random(3));
		me->move(nextloc);
		me->follower_move(loc, nextloc);
		return;
		break;
	case SEA:
		cost_stamina = 600 - me->query_skill_level("sport");

		if( !me->cost_stamina(cost_stamina) )
			return tell(me, pnoun(2, me)+"�S����O�e�i�F�C\n");

		msg("$ME�ĤO�a��"+CHINESE_D->to_chinese(direction)+"��L�h�A��O "+cost_stamina+" �I��O�C\n",me,0,1);
		me->add_social_exp(random(3));
		me->move(nextloc);
		me->follower_move(loc, nextloc);
		return;
		break;
	}

	// �@��a�Ϩt�β���
	broadcast(loc, my_idname+"��"+CHINESE_D->to_chinese(direction)+"���}�C\n", 0, me);
	me->move(nextloc);
	broadcast(nextloc, my_idname+weather_go[NATURE_D->query_nature(query_maproom(loc))[NATURE_WEATHER][WEATHER_ID]]+"�A��"+CHINESE_D->to_chinese(corresponding[direction])+"���F�L�ӡC\n", 0, me);

	if( is_city_location(nextloc) )
	{
		int coortype = query_coor_data(nextloc, TYPE);
		mapping fund = query_city_info(city, "fund");


		if( coortype == ROAD && fund["�D��"] < 120 && !random((fund["�D��"]+50)*(fund["�D��"]+50)) )
		{
			msg("$ME��M�@�}��i�~�[���ת��D���|�}���A���ѳ������a���W���Ǫ�"+HIY+random_thing+NOR+"�C\n", me, 0, 1);
			me->faint();
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"��M�@�}��i�~�[���ת��D���|�}("+query_city_id(city, num)+" "+(nextloc[X]+1)+","+(nextloc[Y]+1)+")���A���ѳ������a���W���Ǫ�"+HIY+random_thing+NOR+"�C", me);
		}
		else if( coortype == BRIDGE && fund["����"] < 120 && !random((fund["����"]+50)*(fund["����"]+50)) )
		{
			msg("$ME�}���U�����٬�M�_���A���ѳ������a���W���[�W��"+HIY+random_thing+NOR+"�C\n", me, 0, 1);
			me->faint();
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"�}���U������("+query_city_id(city, num)+" "+(nextloc[X]+1)+","+(nextloc[Y]+1)+")��M�_���A���ѳ������a���W���[�W��"+HIY+random_thing+NOR+"�C", me);

		}
		else if( !coortype && !random(1000) )
		{
			msg("$ME���b���C��������a�W�A��M���}���F�@�U�A���ѳ������a���W���䪺"+HIY+random_thing+NOR+"�C\n", me, 0, 1);
			me->faint();
			//CHANNEL_D->channel_broadcast("city", me->query_idname()+"���b���C��������a�W�A��M���}���F�@�U�A���ѳ������a���W���䪺"+HIY+random_thing+NOR+"�C", me);
		}
	}

	me->follower_move(loc, nextloc);
}

void remove_player_icon(object me, array loc)
{
	int x, y, num;
	string city;
	object *others;

	if( !arrayp(loc) ) return;

	x = loc[X];
	y = loc[Y];
	city = loc[CITY];
	num = loc[NUM];

	if( !city_exist(city, num) || !valid_coordinate(x, y, city, num) || undefinedp(cities[city][num][RMAP]) || undefinedp(cities[city][num][MAP]) ) return;

	others = filter_array(present_objects(me)-({me}), (: userp($1) :));

	switch( sizeof(others) )
	{
	case 0:	cities[city][num][RMAP][y][x] = cities[city][num][MAP][y][x];break;
	case 1:	cities[city][num][RMAP][y][x] = ansi_part(cities[city][num][MAP][y][x]) + (query("gender", others[0]) == MALE_GENDER ? HIG + "��" : HIR + "��") + NOR;break;
	default:cities[city][num][RMAP][y][x] = ansi_part(cities[city][num][MAP][y][x]) + HIC"��"NOR;break;
	}
}

void set_player_icon(object me, array loc)
{
	int x, y, num;
	string city;
	object *others;

	if( !arrayp(loc) ) return;

	x = loc[X];
	y = loc[Y];
	city = loc[CITY];
	num = loc[NUM];

	if( !valid_coordinate(x, y, city, num) ) return;

	others = filter_array(present_objects(me) - ({ me }), (: userp($1) :));

	if( sizeof(filter_array(present_objects(me), (: userp($1) :))) > 1 )
		cities[city][num][RMAP][y][x] = ansi_part(cities[city][num][MAP][y][x]) + HIC"��"NOR;
	else
		cities[city][num][RMAP][y][x] = ansi_part(cities[city][num][MAP][y][x]) + (query("gender", me) == MALE_GENDER ? HIG + "��" : HIR + "��") + NOR;
}


// �N�a���ഫ�� HTML �榡
void translate_map_to_html(string city, int num)
{
	array arr;
	string map_graph = query_city_idname(city, num)+"\n";

	if( city_exist(city, num) )
	{
		foreach( arr in cities[city][num][RMAP] )
			map_graph += kill_repeat_ansi(implode(arr, "")+" \n");

		write_file("/www/map/citymap_"+city+"_"+num+"_ansi", map_graph, 1);

		HTML_D->make_html(map_graph, ([
			"bgcolor"	:"black",
			"filename"	:"/www/map/citymap_"+city+"_"+num+".html",
			"title"		:remove_ansi(MUD_FULL_NAME+" "+CITY_D->query_city_idname(city, num)),
			"refresh"	:300,
			"fontsize"	:12,
		    ]));
	}
}

int is_mayor(string city, mixed ob)
{
	string id;

	if( !city_exist(city) ) return 0;

	if( stringp(ob) )
		id = ob;
	else if( objectp(ob) )
	{
		if( !stringp(id = ob->query_id(1)) )
			return 0;
	}
	else
		return 0;

	return id == cities[city][INFO]["government/mayor"];
}

int is_officer(string city, mixed ob)
{
	string id;

	if( !city_exist(city) ) return 0;

	if( stringp(ob) )
		id = ob;
	else if( objectp(ob) )
	{
		if( !stringp(id = ob->query_id(1)) )
			return 0;
	}
	else
		return 0;

	return !undefinedp(cities[city][INFO]["government/officer"][id]);
}

int is_mayor_or_officer(string city, mixed ob)
{
	string id;

	if( !city_exist(city) ) return 0;

	if( stringp(ob) )
		id = ob;
	else if( objectp(ob) )
	{
		if( !stringp(id = ob->query_id(1)) )
			return 0;
	}
	else
		return 0;

	return id == cities[city][INFO]["government/mayor"] || !undefinedp(cities[city][INFO]["government/officer"][id]);
}

int query_city_num(string city)
{
	int num;

	while(city_exist(city, ++num));

	return num;
}

mapping query_city_fund(string city)
{
	int num;
	mapping fund, facilities, result;

	if( !city_exist(city) ) return 0;

	result = allocate_mapping(0);
	fund = query_city_info(city, "fund") || allocate_mapping(0);
	facilities = query_public_facility(city);

	num = query_city_num(city);

	result["�s��"] 		= count(fund["�s��"], "*", 320*400*num);
	result["�˪L"] 		= count(fund["�˪L"], "*", 320*400*num);
	result["�e�y"] 		= count(fund["�e�y"], "*", 160*400*num);

	result["�D��"] 		= count(count(fund["�D��"], "*", facilities["�D��"]), "*", 20);
	result["����"] 		= count(count(fund["����"], "*", facilities["����"]), "*", 600);

	result["�Ŧa"] 		= count(facilities["�Ŧa"], "*", 2000);

	result["���@�ؿv��"]	= count(facilities["���@�ؿv��"], "*", 1000000);

	return result;
}

string query_age_name(string city)
{
	switch(query_city_info(city, "age"))
	{
	case 0:
		return HIW"�v�e"NOR WHT"�ɥN"NOR;
		break;
	case 1:
		return HIG"�A�~"NOR GRN"�ɥN"NOR;
		break;
	case 2:
		return HIY"�u�~"NOR YEL"�ɥN"NOR;
		break;
	case 3:
		return HIC"����"NOR CYN"�ɥN"NOR;
		break;
	case 4:
		return HIB"�t�z"NOR BLU"�ɥN"NOR;
		break;
	case 5:
                return HIR"�@��"NOR RED"����"NOR;
		break;
	default:
		return HIR"����"NOR RED"�֤�"NOR;
		break;
	}
}

void fund_handler()
{
	string fundname, totalmoney, money, city, citizen;
	mapping currentassets = allocate_mapping(0);

	foreach( city in query_cities() )
	currentassets[city] = cities[city][INFO]["assets"];

	// �B�z�x�|���
	TAX_D->game_month_process();

	foreach( city in query_cities() )
	{
		totalmoney = 0;

		// �o�󳣥����p��g�O
		if( cities[city][INFO]["fallen"] ) continue;

		// �@��g�O�p��
		foreach( fundname, money in query_city_fund(city) )
			totalmoney = count( totalmoney, "-", money );

		foreach( citizen in query_citizens(city) )
			totalmoney = count( totalmoney, "-", count(ESTATE_D->query_all_estate_value(citizen), "/", 15000));

		change_assets(city, totalmoney);

		cities[city][INFO]["last_total_gain"] = count(cities[city][INFO]["assets"], "-", currentassets[city]);
	}
}

void add_collection_record(string city, int num, string id, string material, int amount)
{
	if( !city_exist(city, num) ) return;


	if( !mapp(cities[city][INFO]["section"][num]["material_collection_record"]) )
		cities[city][INFO]["section"][num]["material_collection_record"] = allocate_mapping(0);

	if( !mapp(cities[city][INFO]["section"][num]["material_collection_record"][id]) )
		cities[city][INFO]["section"][num]["material_collection_record"][id] = allocate_mapping(0);

	cities[city][INFO]["section"][num]["material_collection_record"][id][material] += amount;
}

void reset_collection_record()
{
	string city;
	int num;

	foreach(city in query_cities())
		for(num=0;num<query_city_num(city);num++)
			cities[city][INFO]["section"][num]["material_collection_record"] = allocate_mapping(0);
}

mapping query_collection_record(string city, int num)
{
	return cities[city][INFO]["section"][num]["material_collection_record"];
}

// �Ω��ʭק﫰����Ʈw���e
/*
void change_data()
{
	foreach( string city, mapping map1 in cities )
	foreach( mixed num, mapping map2 in map1 )
	{
		if( !intp(num) ) continue;

		for(int y=0;y<100;y++)
		for(int x=0;x<100;x++)
		{

		}
	}
}
*/

string query_city_scale(string city, int num)
{
	int level;

	if( !city_exist(city, num) ) return 0;

	level = cities[city][INFO]["section"][num]["level"];

	return CITY_SCALE[level];
}

// �c�a�׻P�~���H�f�p��
void rehash_flourish(string city)
{
	int i, level, flourish, num, cityuser;
	int now_time = time();
	int last_on_time;
	int time_cost;

	float myflourish;
	mapping data, estdata, cityestdata;
	string id;
	array myestates;
	int register_open;
	object ob;

	if( !city_exist(city, num) ) return;

	cityuser = sizeof(filter_array(users(), (: query("city", $1) == $(city) :)));

	cities[city][INFO]["totalflourish"] = 0;

	if( cities[city][INFO]["fallen"] ) return;

	// �ˬd�����O�_���}����U
	if( sizeof(objects((: query("function", $1) == "register" && $1->query_city() == $(city) && !BUILDING_D->is_building_room_locked($1) :))) )
		register_open = 1;

	cities[city][INFO]["register_open"] = register_open;

	for(num=0;city_exist(city, num);num++)
	{
		flourish = 0;

		data = cities[city][num][DATA];

		cityestdata = ESTATE_D->query_city_estate(city, num);

		if( sizeof(cityestdata) )
		foreach(id, myestates in cityestdata)
		{
			if( belong_to_government(id) || belong_to_enterprise(id) )
				continue;

			time_cost = 0;
			if( !find_player(id) && user_exists(id) )
			{
				ob = load_user(id);

				last_on_time = query("last_on/time", ob);

				if( last_on_time > 0 )
					time_cost = now_time - last_on_time;

				destruct(ob);
			}

			foreach(estdata in myestates)
			{
				if( sizeof(estdata["roomtable"]) > 0 && BUILDING_D->is_building_locked(restore_variable(estdata["roomtable"][0]), estdata) )
					continue;

				myflourish = ESTATE_D->query_estate_flourish(estdata);

				if( time_cost > 0 )
					myflourish /= 1 + time_cost/86400.;

				flourish += to_int(myflourish);
			}
		}

		// �u�W���a�^�m�c�a��
		flourish += cityuser*300;

		// �H���ܰ�
		flourish += range_random(-100, 100);

		// �̧C��
		if( flourish < 10 )
			flourish = random(10)+1;

		// �g�O�v�T
		foreach(i in values(query_city_info(city, "fund")))
			flourish += i;

		cities[city][INFO]["section"][num]["flourish"] = flourish;

		cities[city][INFO]["section"][num]["resident"] = to_int(pow(to_float(flourish)*1000, 0.8));

		cities[city][INFO]["totalflourish"] += flourish;

		level = cities[city][INFO]["section"][num]["level"];

		// ����
		if( cities[city][INFO]["section"][num]["resident"] < level*1000000 && level > 0 )
		{
			cities[city][INFO]["section"][num]["level"] = level-1;
			CHANNEL_D->channel_broadcast("news", query_city_idname(city, num)+"�~���H�f�`�Ƥ֩�"HIW" "+NUMBER_D->number_symbol(level*1000000)+" "NOR"�H�A�W�ҭ����u"+CITY_SCALE[level-1]+"�v");
		}
		// �ɯ�
		else if( cities[city][INFO]["section"][num]["resident"] > (level+1)*1000000 && level < 4 )
		{
			cities[city][INFO]["section"][num]["level"] = level+1;
			CHANNEL_D->channel_broadcast("news", query_city_idname(city, num)+"�~���H�f�`�Ƭ�}"HIW" "+NUMBER_D->number_symbol((level+1)*1000000)+" "NOR"�H�A�W�Ҥɬ��u"+CITY_SCALE[level+1]+"�v");
		}
	}
}


void rehash_all_flourish()
{
	foreach( string city in query_cities() )
	rehash_flourish(city);
}

mapping query_city_data(string city, int num)
{
	if( undefinedp(city) )
		return cities;
	else if( undefinedp(num) )
		return cities[city];
	else
		return cities[city][num];
}

/* ���m�Ҧ���� */
void restore_all_data()
{
	string city, roomfile;
	mixed num;

	cities = allocate_mapping(0);

	/* ��l��Ū���Ҧ�������T */
	foreach( city in get_dir(CITY_ROOT) )
	{
		if( file_size(CITY_PATH(city)) != -2 ) continue;

		cities[city] = allocate_mapping(0);

		if(!read_file(CITY_INFO(city))) {
			CHANNEL_D->channel_broadcast("sys", sprintf("%s �gŪ����r��Ȭ� 0 ... ���L�C", CITY_INFO(city)));
			continue;
		}

		if(!(cities[city][INFO] = restore_variable(read_file(CITY_INFO(city))))) {
			CHANNEL_D->channel_broadcast("sys", sprintf("%s �Ҧs��ƥH restore_variable Ū�X���G�� 0 ... ���L�C", CITY_INFO(city)));
		}

		// Ū���Ҧ��l�������
		foreach( num in get_dir(CITY_PATH(city)) )
		{
			if( !sscanf(num, "%d", num) ) continue;

			CHANNEL_D->channel_broadcast("sys", "�����G��l�ƫ��� "+city+" ���� "+num+" ���Ϧa�ϸ�Ʈw");

			cities[city][num] 	= allocate_mapping(0);

			if(!read_file(CITY_NUM_DATA(city, num))) {
				CHANNEL_D->channel_broadcast("sys", "DATA �gŪ����r��Ȭ� 0 ... ���L�C");
			}
			else if(!(cities[city][num][DATA] = restore_variable(read_file(CITY_NUM_DATA(city, num))))) {
				CHANNEL_D->channel_broadcast("sys", "DATA �Ҧs��ƥH restore_variable Ū�X���G�� 0 ... ���L�C");
			}

			if(!read_file(CITY_NUM_MAP(city, num))) {
				CHANNEL_D->channel_broadcast("sys", "MAP �gŪ����r��Ȭ� 0 ... ���L�C");
			}
			else if(!(cities[city][num][MAP] = restore_variable(read_file(CITY_NUM_MAP(city, num))))) {
				CHANNEL_D->channel_broadcast("sys", "MAP �Ҧs��ƥH restore_variable Ū�X���G�� 0 ... ���L�C");
			}
			else
				cities[city][num][RMAP] = copy(cities[city][num][MAP]);

			cities[city][num][CROOM] = load_cityroom(city, num);

			// ���J�Ҧ� Module Room
			foreach( roomfile in get_dir(CITY_NUM_ROOM(city, num)) )
			{
				reset_eval_cost();

				catch(load_object(CITY_NUM_ROOM(city, num)+roomfile[0..<3]));
			}
		}
	}

	// ��Ҧ������i��s���Ƨ�
	assign_cities_num();
}

string query_name()
{
	return "�����t�ΥD��(CITY_D_MAIN)";
}
