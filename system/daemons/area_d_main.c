/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : area_d_main.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-18
 * Note   : �ϰ���F�D��, �B�z�Ҧ��ϰ��Ʈw
 * Update :
 *
 -----------------------------------------
 */

//#pragma save_binary
#include <ansi.h>
#include <map.h>
#include <areadata.h>
#include <lock.h>
#include <location.h>
#include <daemon.h>
#include <object.h>
#include <terrain.h>
#include <gender.h>
#include <message.h>
#include <mudlib.h>
#include <nature.h>
#include <nature_data.h>
#include <roommodule.h>
#include <time.h>
#include <envvar.h>


#define LOG			"daemon/area"

#define SAVE_INFO		(1<<0)
#define SAVE_DATA		(1<<1)
#define SAVE_MAP		(1<<2)
#define SAVE_ALL		(SAVE_INFO|SAVE_DATA|SAVE_MAP)

#define MAP		"map"
#define RMAP		"realtime_map"
#define DATA		"data"
#define INFO		"info"
#define CROOM		"arearoom"

#define RANDOM_STRING		({ "�ᷦ��", "�O�ֲy", "�R�𫽫�", "�δο}", "�h��", "������", "�D�k�H", "�g�a��", "�����q�c", "�H����", "�æ��z����", "�¹D�Ѥj", "�Ϧ��l", "�o�����ʪ�", "�ּu�o�g��", "������" })

private int number;
private int save_all_time;

private mapping areas;
private mapping sort_save_list;


varargs int area_exist(string area, int num)
{
	if( undefinedp(num) )
		return mapp(areas[area]);
	else
		return mapp(areas[area]) && mapp(areas[area][num]);
}

varargs int map_exists(string map, int section)
{
	if( undefinedp(section) )
		return mapp(areas[map]);
	else
		return mapp(areas[map]) && mapp(areas[map][section]);
}

int is_area_location(mixed map)
{
	if( arrayp(map) )
		return area_exist(map[AREA], map[NUM]);
	else if( stringp(map) )
		return area_exist(map);

	return 0;
}

varargs string query_area_id(string area, int num)
{
	if( undefinedp(num) )
	{
		if( !area_exist(area) ) return 0;
		return areas[area][INFO]["color_id"];
	}
	else
	{
		if( !area_exist(area, num) ) return 0;
		return areas[area][INFO]["color_id"] + " "+(num+1);
	}
}

varargs string query_area_name(string area, int num)
{
	if( undefinedp(num) )
	{
		if( !area_exist(area) ) return 0;
		return areas[area][INFO]["name"];
	}
	else
	{
		if( !area_exist(area, num) ) return 0;
		return areas[area][INFO]["name"] + (areas[area][INFO]["section"][num]["name"] || "��"+CHINESE_D->chinese_number(num+1)+"��");
	}
}

varargs string query_area_idname(string area, int num)
{
	if( undefinedp(num) )
	{
		if( !area_exist(area) ) return 0;
		return areas[area][INFO]["name"]+"<"+areas[area][INFO]["color_id"]+">";
	}
	else
	{
		if( !area_exist(area, num) ) return 0;
		return areas[area][INFO]["name"]+(areas[area][INFO]["section"][num]["name"] || "��"+CHINESE_D->chinese_number(num+1)+"��")+"<"+areas[area][INFO]["color_id"]+" "+(num+1)+">";
	}
}

int valid_coordinate(mixed x, int y, string area, int num)
{
	if( intp(x) )
		return x>=0 && x<MAP_WIDTH && y>=0 && y<MAP_LENGTH && area_exist(area, num);
	else if( arrayp(x) )
		return x[X]>=0 && x[X]<MAP_WIDTH && x[Y]>=0 && x[Y]<MAP_LENGTH && area_exist(x[AREA], x[NUM]);
	else
		return 0;
}

/* ��Ҧ��ϰ�s�� */
void assign_areas_num()
{
	number = 0;

	sort_save_list = allocate_mapping(0);

	foreach( string area in get_dir("/area/") )
	{
		if( file_size("/area/"+area) != -2 ) continue;

		sort_save_list[number++] = area;
	}
}


void save_options(string area, int num, int bit)
{
	// �ഫ�a�Ϧ� HTML
	AREA_D->translate_map_to_html(area, num);

	// �g�J�򥻸�T��
	if( bit & SAVE_INFO )
	{

		if( !area_exist(area) || undefinedp(areas[area][INFO]) )
		{
			CHANNEL_D->channel_broadcast("sys", "�ॢ "+area+" �򥻸�T");
			log_file(LOG, "�ॢ "+area+" �򥻸�T");
		}
		else if( !write_file(AREA_INFO(area), save_variable( copy(areas[area][INFO]) ), 1) )
		{
			CHANNEL_D->channel_broadcast("sys", "�L�k�g�J "+area+" �򥻸�T��");
			log_file(LOG, "�L�k�g�J "+area+" �򥻸�T��");
		}
	}

	// �g�J�y�и����
	if( bit & SAVE_DATA )
	{
		if( !area_exist(area) || undefinedp(areas[area][num][DATA]) )
		{
			CHANNEL_D->channel_broadcast("sys", "�ॢ "+area+" �y�и��");
			log_file(LOG, "�ॢ "+area+" �y�и��");
		}

		else if( !write_file(AREA_NUM_DATA(area, num), save_variable( copy(areas[area][num][DATA]) ), 1) )
		{
			CHANNEL_D->channel_broadcast("sys", "�L�k�g�J "+area+" "+num+" �y�и����");
			log_file(LOG, "�L�k�g�J "+area+" "+num+" �y�и����");
		}
	}

	// �g�J�a�ϸ����
	if( bit & SAVE_MAP )
	{
		if( !area_exist(area) || undefinedp(areas[area][num][MAP]) )
		{
			CHANNEL_D->channel_broadcast("sys", "�ॢ "+area+" �a�ϸ��");
			log_file(LOG, "�ॢ "+area+" �a�ϸ��");
		}

		else if( !write_file(AREA_NUM_MAP(area, num), save_variable( copy(areas[area][num][MAP]) ), 1) )
		{	
			CHANNEL_D->channel_broadcast("sys", "�L�k�g�J "+area+" "+num+" �a�ϸ����");
			log_file(LOG, "�L�k�g�J "+area+" "+num+" �a�ϸ����");
		}
	}
}

varargs void save_area(string area, int num, int bit)
{
	int time_exp;

	if( !undefinedp(num) )
	{
		string options = "";

		if( !area_exist(area, num) ) return;

		time_exp = time_expression { save_options(area, num, undefinedp(bit) ? SAVE_ALL : bit); };

		if( bit & SAVE_INFO ) options += "�򥻸�T ";
		if( bit & SAVE_DATA ) options += "�y�и�� ";
		if( bit & SAVE_MAP  ) options += "�a�ϸ�� ";

		if( options != "" )
			CHANNEL_D->channel_broadcast("nch",sprintf("�ϰ�G�x�s�ϰ�u%s�v%s(%.6f sec)�C",query_area_idname(area, num), options, time_exp/1000000.));
	}
	else if( area_exist(area) )
	{
		foreach( num, mapping data in areas[area] )
		if( intp(num) )
			time_exp += time_expression { save_options(area, num, SAVE_ALL); };

		CHANNEL_D->channel_broadcast("nch",sprintf("�ϰ�G�x�s�ϰ�u%s�v�Ҧ����(%.6f sec)�C",query_area_idname(area), time_exp/1000000.));
	}
}

/* �� TIME_D �w���I�s, �ϰ��ƽ��y�x�s */
void time_distributed_save()
{
	string area;
	int num;

	if( sizeof(areas) < 1 ) return;

	area = sort_save_list[++number] || sort_save_list[(number=0)];
	
	for(num=0;area_exist(area, num);num++)
		call_out( (: save_area :), (num+1)*10, area, num);
}

/* �x�s�Ҧ� area ��T */
int save_all()
{
	int time_exp;

	// ���קK�b shutdown ��, �y������ save_all ������, �G�]�m save_all_time �@�ˬd
	if( time() - save_all_time < 30 ) return 0;

	save_all_time = time();

	time_exp = time_expression {

		foreach( string area in get_dir(AREA_ROOT) )
		if( file_size(AREA_PATH(area)) == -2 )
			save_area(area);
	};// end of time_expression

	CHANNEL_D->channel_broadcast("nch",sprintf("�ϰ�G�����x�s�Ҧ��ϰ��Ƨ���(�@�O %.6f sec)�C", time_exp/1000000.));
	return 1;
}

void initialize_area_data(string area)
{
	if( !stringp(area) ) return;

	areas[area] 			= allocate_mapping(0);
	areas[area][INFO] 		= allocate_mapping(0);
	areas[area][INFO]["section"]	= allocate_mapping(0);
}

void initialize_section_data(string area, int num)
{
	if( !stringp(area) || !intp(num) ) return;

	areas[area][num] = allocate_mapping(5);

	areas[area][num][DATA] = allocate_mapping(0);
	areas[area][num][MAP]  = allocate_mapping(0);
	areas[area][num][RMAP] = allocate_mapping(0);
	areas[area][num][CROOM]= load_arearoom(area, num);
}

/* �إ߰ϰ�s����, �ҩl�a�ϸ�Ʈw�P�إ߰ϰ�a�� */
int create_section(string area, int maptype)
{
	array newmap;
	mapping newdata;

	int x, y, num=0, all_pop;

	// �۰ʷj�M�s��
	while( file_size(AREA_NUM(area, num)) == -2 ) num++;

	mkdir(AREA_NUM(area, num));		// �إߤl�ϰ�ؿ�
	mkdir(AREA_NUM(area, num)+"npc");
	mkdir(AREA_NUM(area, num)+"obj");

	initialize_section_data(area, num);
	
	areas[area][INFO]["section"][num] = allocate_mapping(0);

	newdata = allocate_mapping(100);

	// �إ߹w�]�y�и��
	for(y=0;y<MAP_LENGTH;y++)
	{
		newdata[y] = allocate_mapping(1);

		for(x=0;x<MAP_WIDTH;x++)
			newdata[y][x] = allocate_mapping(0);
	}

	// �إ߰ϰ�зǦa��
	newmap = copy(allocate(MAP_LENGTH, copy(allocate(MAP_WIDTH, GRN"�D"NOR))));

	// �إ߯S��a��
	TERRAIN_D->create_area_map_terrain(ref newmap, ref newdata, maptype);
	
	areas[area][num][MAP] = copy(newmap);
	areas[area][num][RMAP]= copy(newmap);
	areas[area][num][DATA]= copy(newdata);

	// �ҩl���ϤѮ�
	NATURE_D->activate_nature(areas[area][num][CROOM]);
	
	log_file(LOG, area+" �ϰ�إ߷s���� "+num);

	CHANNEL_D->channel_broadcast("news", HIY"�b�@�ɪ��Y�B�X�{�F���s���ϰ�y"+NOR+query_area_idname(area, num)+HIY"�z�C\n"NOR);

	save_area(area, num, SAVE_ALL);

	return all_pop;
}

/* �إߥ��s�ϰ�, �ҩl�ϰ��Ʈw */
int create_area(object me, string color_area_id, string color_area_name, int maptype)
{
	int all_pop, len;
	string area_id;

	color_area_id = kill_repeat_ansi(lower_case(color_area_id)+NOR);
	area_id = remove_ansi(color_area_id);

	// �N color_area_id �Ĥ@�r�� upper_case
	color_area_id = ansi_capitalize(color_area_id);

	// �w�g�s�b area id
	//if( area_exist(area_id) ) return -1;

	// �D�­^�大 area id
	foreach( int a in area_id )
	if( (int) a<'a' || (int) a>'z' ) 
		return -2;

	// area_name �r���ƿ��~
	len = noansi_strlen(color_area_name);
	if(  len > 10 || len % 2 )
		return -3;

	// �D�¤��大 area name	
	foreach( int a in remove_ansi(color_area_name) )
	if(  a <= ' ' || --len%2 && (a < 160 || a > 255) )
		return -4;

	color_area_name = kill_repeat_ansi(color_area_name+NOR);

	// ���Ъ��ϰ줤��W��
	//foreach( string tmp, mapping data in areas )
	//if( mapp(data[INFO]) && remove_ansi(data[INFO]["name"]) == remove_ansi(color_area_name) ) return -6;

	// �إ߻ݭn���ؿ�
	mkdir(AREA_PATH(area_id));		// �ϰ�D�ؿ�

	// ��l�ưϰ���
	initialize_area_data(area_id);

	areas[area_id][INFO]["id"] 		= area_id;
	areas[area_id][INFO]["color_id"]	= color_area_id;
	areas[area_id][INFO]["name"] 		= color_area_name;
	areas[area_id][INFO]["maptype"]		= maptype;

	// �إ߲Ĥ@�ϰ����
	all_pop = create_section(area_id, maptype);

	assign_areas_num();

	log_file(LOG, me->query_id()+"�إߥ��s�ϰ� "+area_id+"�C\n");

	return 1;
}

/* �R���l�ϰ���� */
void delete_section(string area, int num)
{
	if( !areas[area][num] ) return;

	// �����Ҧ��b�ϰ쪫�󤤪�����
	foreach( object inv in all_inventory(areas[area][num][CROOM]) )
	{
		if( userp(inv) )
		{
			reset_screen(inv);
			inv->move(WIZ_HALL);
			msg("$ME�Ҧb���ϰ���`�F�A�u�n�y���ܧŮv�j�U�C\n",inv,0,1);
		}
		else
			destruct(inv);
	}

	// �R���l�ϰ쪫��
	destruct( areas[area][num][CROOM] );

	// �R���l�ϰ�ؿ�
	rmtree(AREA_NUM(area, num));

	// �R���l�ϰ�Ҧ���T
	map_delete(areas[area], num);

	log_file(LOG, "�R�� "+area+" �ϰ���� "+num);
	CHANNEL_D->channel_broadcast("sys", "�R�� "+area+" �ϰ���� "+num);
}

/* �R����Ӱϰ� */
void delete_area(string area)
{
	int num;

	if( !area_exist(area) ) return;

	num = sizeof(areas[area]);

	// �R���Ҧ�����
	while(num--)
		delete_section(area, num);

	log_file(LOG,"�R�� "+area+" �ϰ�C\n");
	CHANNEL_D->channel_broadcast("news", query_area_idname(area)+"�q���ۥ@�ɦa�ϤW�����C\n");

	// �M���ϰ���
	map_delete(areas, area);

	if( !sizeof(areas) )
		areas = allocate_mapping(0);

	// �R���ϰ�ؿ�
	rmtree(AREA_PATH(area));

	// ���s�s�ưϰ�Ƨ�
	assign_areas_num();
}

/* �]�w�Y�@�ϰ�a�ϤW�Y�@�y�Ф��䤤�@���ܼƭ� */
mixed set_coor_data(array loc, mixed prop, mixed data)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( undefinedp(prop) || !mapp(areas[area][num][DATA][y][x]) )
		return 0;

	if( prop == FLAGS )
	{
		if( !intp(data) ) return 0;
		return (areas[area][num][DATA][y][x][prop] |= data);
	}
	else
		return (areas[area][num][DATA][y][x][prop] = data);
}

/* ��@�y�Ф��ܼƭȰ��ƭȥ[��B�z */
mixed add_coor_data(array loc, mixed prop, int data)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( undefinedp(prop) || !mapp(areas[area][num][DATA][y][x]) )
		return 0;

	return (areas[area][num][DATA][y][x][prop] += data);
}

/* �R���@�y�Ф���� */
varargs void delete_coor_data(array loc, mixed prop, int bit)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( !mapp(areas[area][num][DATA][y][x]) ) return;

	// �Y�L prop �h���s��l�Ʃж���T, �H�f�����쪬
	if( !prop ) 
		areas[area][num][DATA][y][x] = allocate_mapping(0);

	else if( prop == FLAGS )
	{
		if( !undefinedp(bit) )
			areas[area][num][DATA][y][x][prop] ^= bit;
		else
			map_delete(areas[area][num][DATA][y][x], prop);
	}
	else
	{
		map_delete(areas[area][num][DATA][y][x], prop);

		// �s�էR���\��
		foreach(mixed ori_prop, mixed data in areas[area][num][DATA][y][x])
		{
			if( stringp(ori_prop) && stringp(prop) && sscanf(ori_prop, prop+"/%*s") == 1 )
				map_delete(areas[area][num][DATA][y][x], ori_prop);
		}
	}
}


/* �Ǧ^�Y�@�ϰ�a�ϤW�Y�@�y�Ф��䤤�@���ܼƭ� */
varargs mixed query_coor_data(array loc, mixed prop)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( undefinedp(prop) )
		return areas[area][num][DATA][y][x];
	else
		return areas[area][num][DATA][y][x][prop];
}

/* �d��ʬd�M, �Ǧ^��ư}�C */
/* ��Υb�|��Ƹs�j�M�k, �j�M�ɶ��P radius ������e���� */
mapping query_coor_range(array loc, string prop, int radius)
{
	string area;
	int num, x, y;
	int i, j, xf, xt, yf, yt;
	mapping coor, data = allocate_mapping(0);

	if( !arrayp(loc) ) return 0;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	coor = areas[area][num][DATA];

	if( radius < 1 ) return ([ save_variable(loc):coor[y][x][prop] ]);

	xf = x-radius; if( xf < 0 ) 	xf = 0;
	xt = x+radius; if( xt > 99 ) 	xt = 99;
	yf = y-radius; if( yf < 0 )	yf = 0;
	yt = y+radius; if( yt > 99 )	yt = 99;
	
	radius *= radius;
	
	for(i=xf;i<=xt;i++)
		for(j=yf;j<=yt;j++)
			if( (i-x)*(i-x) + (j-y)*(j-y) <= radius )
				data[save_variable(arrange_area_location(i,j,area,num))] = coor[j][i][prop];
	return data;
}
/* �]�w�y�Цa�Ϲϧ� */
void set_coor_icon(array loc, string icon)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) || noansi_strlen(icon) != 2 ) return;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( valid_coordinate(x, y, area, num) )
		areas[area][num][MAP][y][x] = areas[area][num][RMAP][y][x] = icon;
}

/* �]�w�y�Цa�Ϲϧ� */
void set_real_time_coor_icon(array loc, string icon)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) || noansi_strlen(icon) != 2 ) return;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( valid_coordinate(x, y, area, num) )
		areas[area][num][RMAP][y][x] = icon;
}

/* �Ǧ^�y�Цa�Ϲϧ� */
varargs string query_coor_icon(array loc, int raw)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	return !undefinedp(raw) ? remove_ansi(areas[area][num][MAP][y][x]) : areas[area][num][MAP][y][x];
}

/* ���]�y�Цa�Ϲϧ� */
void reset_coor_icon(array loc)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	areas[area][num][RMAP][y][x] = areas[area][num][MAP][y][x];
}

// �R���y�Цa�Ϲϧ�
void delete_coor_icon(array loc)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	areas[area][num][RMAP][y][x] = GRN"�D"NOR;
	areas[area][num][MAP][y][x] = GRN"�D"NOR;
}

/* �]�w�ϰ�򥻸�T */
mixed set_area_info(string area, string prop, mixed info)
{
	if( !stringp(area) || !stringp(prop) || undefinedp(info) || !area_exist(area) ) return 0;

	return areas[area][INFO][prop] = info;
}

/* �^�ǰϰ�򥻸�T */
varargs mixed query_area_info(string area, string prop)
{
	if( !stringp(area) || !area_exist(area) ) return 0;

	if( !undefinedp(prop) )
		return copy(areas[area][INFO][prop]);
	else
		return copy(areas[area][INFO]);
}

/* �R���ϰ�򥻸�T */
int delete_area_info(string area, string prop)
{
	if( !stringp(area) || !stringp(prop) || !area_exist(area) ) return 0;

	map_delete(areas[area][INFO], prop);

	return 1;
}
/* �]�w�ϰ���ϰ򥻸�T */
mixed set_section_info(string area, int num, string prop, mixed info)
{
	if( !stringp(area) || !stringp(prop) || undefinedp(info) || !area_exist(area, num) ) return 0;

	return areas[area][INFO]["section"][num][prop] = info;
}

/* �^�ǰϰ���ϰ򥻸�T */
varargs mixed query_section_info(string area, int num, string prop)
{
	if( !stringp(area) || !area_exist(area, num) ) return 0;

	if( !undefinedp(prop) )
		return copy(areas[area][INFO]["section"][num][prop]);
	else
		return copy(areas[area][INFO]["section"][num]);
}

/* �R���ϰ���ϰ򥻸�T */
int delete_section_info(string area, int num, string prop)
{
	if( !stringp(area) || !stringp(prop) || !area_exist(area, num) ) return 0;

	map_delete(areas[area][INFO]["section"][num], prop);

	return 1;
}

// �ק�ϰ��`�W��
void change_area_name(string area, string name)
{
	int len;

	if( !area_exist(area) || !stringp(name) ) return 0;

	len = noansi_strlen(name);
	if(  len > 10 || len % 2 ) return 0;

	// �D�¤��大 area name	
	foreach( int a in remove_ansi(name) )
	if(  a <= ' ' || --len%2 && (a < 160 || a > 255) )
		return 0;

	name = kill_repeat_ansi(remove_fringe_blanks(name)+NOR);

	CHANNEL_D->channel_broadcast("news", query_area_idname(area)+"������W���u"+name+"�v");

	set_area_info(area, "name", name);
}

// �ק�l�ϰ�W�� 
void change_section_name(string area, int num, string name)
{
	int len;
	string original_idname;

	if( !area_exist(area, num) || !stringp(name) ) return 0;

	len = noansi_strlen(name);
	if(  len > 10 || len % 2 ) return 0;

	// �D�¤��大 area name	
	foreach( int a in remove_ansi(name) )
	if(  a <= ' ' || --len%2 && (a < 160 || a > 255) )
		return 0;

	name = kill_repeat_ansi(remove_fringe_blanks(name)+NOR);

	original_idname = query_area_idname(area, num);

	set_section_info(area, num, "name", name);

	CHANNEL_D->channel_broadcast("news", original_idname+"������W���u"+query_area_idname(area, num)+"�v");
}		



/* �Ǧ^�Ҧ��ϰ�W�� */
string *query_areas()
{
	return filter_array(keys(areas), (: stringp($1) :));
}

int query_sizeof_areas()
{
	return sizeof(query_areas());
}


object query_maproom(array loc)
{
	string area = loc[AREA];
	int num = loc[NUM];

	if( !area_exist(area, num) ) return 0;

	return areas[area][num][CROOM];
}

object *query_all_maproom()
{
	object *all_maproom = allocate(0);

	foreach( string area, mapping m1 in areas )
	foreach( int num, mapping m2 in m1 )
	{
		if( intp(num) )
			all_maproom += ({ m2[CROOM] });
	}
	return all_maproom;
}

varargs array query_map(array loc, int realtime)
{
	string area = loc[AREA];
	int num = loc[NUM];

	if( !area_exist(area, num) ) return 0;

	return realtime ? areas[area][num][RMAP] : areas[area][num][MAP];
}

/* �Ǧ^�b�Y�ϰ줤���Ҧ����a */
varargs object *query_area_players(string area, int num)
{
	if( undefinedp(num) )
	{
		if( area_exist(area) )
			return filter_array(users(), (: query_temp("in_area/id",$1) == $(area) :));

		return 0;
	}

	if( area_exist(area, num) )
		return filter_array(users(), (: query_temp("in_area/id",$1) == $(area) && query_temp("in_area/num",$1) == $(num) :));

	return 0;
}

/* �Ǧ^�b�Y�ϰ줤���Ҧ���~���a */
varargs object *query_outdoor_players(string area, int num)
{
	if( undefinedp(num) )
	{
		if( area_exist(area) )
		{
			object *outdoor_players = allocate(0);

			foreach( num in keys(areas[area]) )
			if( intp(num) )
				outdoor_players += filter_array(all_inventory(areas[area][num][CROOM]), (: userp($1) :));

			return outdoor_players;
		}
		return 0;
	}

	if( area_exist(area, num) )
		return filter_array(all_inventory(areas[area][num][CROOM]), (: userp($1) :));

	return 0;
}

mapping query_areas_info()
{
	return copy(areas);	
}

int query_sections(string area)
{
	return sizeof(filter_array(keys(areas[area]), (: intp($1) :)));
}

mapping query_action(array loc)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( !area_exist(area, num) || !valid_coordinate(x, y, area, num) || undefinedp(areas[area][num][DATA]) ) return 0;

	switch( areas[area][num][DATA][y][x][TYPE] )
	{
		case FARM:	return 0;
		case PAVILION:
		case TREE:
		case GRASS:
			return ([
			    "sit":		([ 0:COMMAND_D->find_sub_command("area", "sit") ]),
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
int direction_check(int x, int y, string area, int num)
{
	int flag;
	mapping coor, data = areas[area][num][DATA];

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

// �^�ǰϰ�y�л���
string query_coor_help(array loc, string prop, int radius)
{
	string area = loc[AREA];
	int num = loc[NUM], y = loc[Y], x = loc[X];

	switch( areas[area][num][DATA][y][x][TYPE] )
	{
	case FARM:
		return @FARM_HELP
    �b�o���Цa�W�i�H����(����)����@�P���ΡA���O�p�U
    
    sow ���]�κؤl�W 	���W�Y�����]�άO�ؤl�A�N�i�H�Φ����O�Ӵ����μ���
    irrigate water  	���W�Y�O�������ܡA�N�i�H�Φ����O����@
    reap		�ئ�����K�i�H���榹���O�Ӧ���
    
    ���H�W�o�Ǥu�@���O�n��O��O���A�åB�n�h��M���]�κؤl�P�����A���]�~
�誺�n�a�P���賣�怜�ε��G���ܭ��n���v�T�A�t�~�}�l��@��N�����n������@
�ܪ�������A�_�h�Цa�W���A�@���N�|�\���o�C

FARM_HELP;

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
	string shortname = AREA_D->query_coor_data(loc, "short");

	if( shortname )
		return shortname;

	switch( AREA_D->query_coor_data(loc, TYPE) )
	{
	case 0:
		return NOR YEL"�"NOR;
		break;
	case ROAD:
		return HIW"�D��";
		break;
	case BRIDGE:
		return HIR"����";
		break;
	case FARM:
		return HIG"�A��";
		break;
	case PASTURE:
		return NOR YEL"����";
		break;
	case DOOR:
		return HIY "���f";
		break;
	case WALL:
		return NOR WHT"���";
		break;
	case SEA:
		return HIB "���v";
		break;
	case RIVER:
		{
			switch( AREA_D->query_coor_data(loc, FLAGS) )
			{
				case FLOW_NORTH:
					return HBBLU + "�y�V���_���e�y";
					break;
				case FLOW_SOUTH:
					return HBBLU + "�y�V���n���e�y";
					break;
				case FLOW_EAST:
					return HBBLU + "�y�V���F���e�y";
					break;
				case FLOW_WEST:
					return HBBLU + "�y�V���誺�e�y";
					break;
				default:
					return HBBLU + "�e  �y";
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
		return NOR HBYEL"���O";
	case POOL:
		return NOR HBCYN"����";
	case PAVILION:
		return NOR HBGRN"�D�F";
	case TREE:
		return NOR HBGRN"�j��";
	case GRASS:
		return NOR HBGRN"��a";
	case STATUE:
		return NOR HBWHT"�J��";

	default:
		return repeat_string(" ", 16);
		break;
	}
}

// x_start , y_start ���i���d�� x,y �y�Ъ��_�l��
// x_center �� y_center �O�Ψӭp��q���}�l�H����m���b�����i�۰ʦ��Y
// x �O��y�СA y �O�����y�СAarea �O�ϰ�W�Anum �h�O�ĴX�Ӥl���A���ǫh�O�D��[0]
// Code �ݭn�A��z

/*
          �x          
    0     �x    3     
�w�w�w�w�w�q�w�w�w�w�w
    1     �x    2     
          �x          
*/
string show_map(array loc)
{
	int i, j, k, l, dir, x, y, num, range,
	x_start, y_start, 
	x_center, y_center;

	string area, retmap;
	array map2, map3;

	if( !loc ) return 0;

	x	=loc[X];
	y	=loc[Y];
	area	=loc[AREA];
	num	=loc[NUM];

	x_center = (AREA_VISION_WIDTH+1)/2;
	y_center = (AREA_VISION_LENGTH+1)/2;

	if( !valid_coordinate(x, y, area, num) ) return "���~���a�ϰ϶��I";

	

	range = AREA_VISION_WIDTH/3;

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
		case SNOWY:		range -= 3; break;
		case SHOWER:		range -= 2; break;
		case TYPHOON:		range -= 4; break;
		case FOEHN:		break;
	}

	range*=range;


	// �� wiz ��K��
	if( wizardp(this_player()) && query("env/no_weather",this_player()) )
		range = (AREA_VISION_WIDTH/3)*(AREA_VISION_WIDTH/3);

	// �ϤW������u
	retmap = NOR WHT"\e[s\e[1;1H�z"+sprintf(HIG" %|3d "NOR+WHT"��"+HIG" %|3d "NOR+WHT+"�w",x+1,y+1)+NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w "HIW+TIME_D->gametime_digital_clock()+NOR WHT" �w "HIW+(NATURE_D->query_nature(MAP_D->query_maproom(loc)))[NATURE_WEATHER][WEATHER_CHINESE]+NOR WHT" �{";

	dir = direction_check(x, y, area, num);

	// ��ܰ϶��p�⤽��
	x_start = (x>=x_center)*(x+1-x_center) - ((x+x_center)>MAP_WIDTH)*(x_center+x-MAP_WIDTH);
	y_start = (y>=y_center)*(y+1-y_center) - ((y+y_center)>MAP_LENGTH)*(y_center+y-MAP_LENGTH);

	map2 = copy(areas[area][num][RMAP][y_start..y_start+AREA_VISION_LENGTH-1]);
	map3 = allocate(AREA_VISION_LENGTH, 0);

	if( range == (AREA_VISION_WIDTH/3)*(AREA_VISION_WIDTH/3) )
	{	
		map3 = allocate(AREA_VISION_LENGTH, allocate(AREA_VISION_WIDTH, 1));
		for(j=0;j<AREA_VISION_LENGTH;j++)
			map2[j] = map2[j][x_start..x_start+AREA_VISION_WIDTH-1];
	}
	else
	{
		for(j=0;j<AREA_VISION_LENGTH;j++)
			map3[j] = allocate(AREA_VISION_WIDTH);

		for(j=0;j<AREA_VISION_LENGTH;j++)
		{
			map2[j] = map2[j][x_start..x_start+AREA_VISION_WIDTH-1];

			for(i=0;i<AREA_VISION_WIDTH;i++)
			{
				if( map2[j][i] == HIY"�F"NOR )
				{
					for(k=j-4;k<=j+4&&k<AREA_VISION_LENGTH;k++)
						for(l=i-4;l<=i+4&&l<AREA_VISION_WIDTH;l++)
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

	for(j=0;j<AREA_VISION_LENGTH;j++)
	{
		retmap += "\e["+(j+2)+";1H"NOR WHT"�x"NOR;

		for(i=0;i<AREA_VISION_WIDTH;i++)
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
		retmap += NOR WHT"�x\n"NOR;
	}

	// �ϤU����u
	retmap += "\e["+(j+2)+";1H"NOR WHT"�|["NOR+
	sprintf("%|18s", coor_short_name(loc)+NOR)+
	NOR WHT"]�w�w�w�w�w�w�w�w�w�w"+
	sprintf(NOR"%|10s"NOR WHT"�w"NOR"%|10s"NOR WHT, AREA_D->query_area_name(area), AREA_D->query_section_info(area, num, "name")||("��"+CHINESE_D->chinese_number(num+1)+"��"))+
	"�}\e["+(j+3)+
	";1H"NOR GRN"_________________________________________________________���Ϧa��_\e[u"NOR;
	
	return retmap;
}

int query_area_nums(string area)
{
        return sizeof(filter_array(keys(areas[area]), (: intp($1) :)));
}


string show_map2(array loc)
{
	int i, j, k, l, dir, x, y, num, range,
	x_start, y_start, 
	x_center, y_center;

	string area, retmap;
	array map2, map3,link;

	if( !loc ) return 0;

	x	=loc[X];
	y	=loc[Y];
	area	=loc[AREA];
	num	=loc[NUM];

	x_center = (AREA_VISION_WIDTH+1)/2;
	y_center = (AREA_VISION_LENGTH+1)/2;

	if( !valid_coordinate(x, y, area, num) ) return "���~���a�ϰ϶��I";

	

	range = AREA_VISION_WIDTH/3;

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
		case SNOWY:		range -= 3; break;
		case SHOWER:		range -= 2; break;
		case TYPHOON:		range -= 4; break;
		case FOEHN:		break;
	}

	range*=range;


	// �� wiz ��K��
	if( wizardp(this_player()) && query("env/no_weather",this_player()) )
		range = (AREA_VISION_WIDTH/3)*(AREA_VISION_WIDTH/3);

	// �ϤW������u
	retmap = NOR WHT"\e[s\e[1;1H�z"+sprintf(HIG" %|3d "NOR+WHT"��"+HIG" %|3d "NOR+WHT+"�w",x+1,y+1)+NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w "HIW+TIME_D->gametime_digital_clock()+NOR WHT" �w "HIW+(NATURE_D->query_nature(MAP_D->query_maproom(loc)))[NATURE_WEATHER][WEATHER_CHINESE]+NOR WHT" �{";

	dir = direction_check(x, y, area, num);

	
	x_start = x+1-x_center;
	y_start = y+1-y_center;
	
	map2 = allocate(AREA_VISION_LENGTH, allocate(AREA_VISION_WIDTH, "�@"));

	//map2=allocate(AREA_VISION_LENGTH);
	//for(j=0;j<AREA_VISION_LENGTH;j++)
	//	map2[j]=allocate(AREA_VISION_WIDTH, "�F");
	//foreach(array aa in map2)
	//tell(find_player("cookys"),implode(map2[0],"")+"\n");

	if( x_start < 0 )
	{
		int abs_x=abs(x_start);
		//�H�b�k
		if( y_start < 0 )
		{
			int abs_y=abs(y_start);
			// �H�b�k�U��
			for( j=0;j<AREA_VISION_LENGTH;j++)
			{
				
				if( j >= abs_y )
				{
					// �k�U
					map2[j][abs_x+1..<1]=copy(areas[area][num][RMAP][y_start+j][0..AREA_VISION_WIDTH-abs_x]);
					// ���U
					if( link=AREA_D->query_section_info(area, num, "maplink_west") )
					{
						map2[j][0..abs_x]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[y_start+j][<abs_x..<1]);
					}
				} else {
					// �k�W
					if( link=AREA_D->query_section_info(area, num, "maplink_north") )
					{
						map2[j][abs_x+1..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[MAP_LENGTH-abs_y+j][0..AREA_VISION_WIDTH-abs_x]);
						// ���W
						if( link=AREA_D->query_section_info(link[0], link[1], "maplink_west") )
							map2[j][0..abs_x]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[MAP_LENGTH-abs_y+j][<abs_x..<1]);
					}	
				}
			}
		
		} else if( y_start+AREA_VISION_LENGTH-1 > MAP_LENGTH )
		{
			int abs_y=(y_start+AREA_VISION_LENGTH)-MAP_LENGTH;
			// �H�b�k�W��
			for( j=0;j<AREA_VISION_LENGTH;j++)
			{
				if(  j >= AREA_VISION_LENGTH-abs_y)
				{
					// �k�U
					if( link=AREA_D->query_section_info(area, num, "maplink_south") )
					{
						map2[j][abs_x..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[j-(AREA_VISION_LENGTH-abs_y)][0..AREA_VISION_WIDTH-abs_x]);
						// ���U
						if( link=AREA_D->query_section_info(link[0], link[1], "maplink_west") )
							map2[j][0..abs_x-1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[j-(AREA_VISION_LENGTH-abs_y)][<abs_x..<1]);
					}
					
				} else {
					//MAP_LENGTH-(AREA_VISION_LENGTH-abs_y)+j
					// ���W
					if( link=AREA_D->query_section_info(area, num, "maplink_west") )
						map2[j][0..abs_x-1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[MAP_LENGTH-(AREA_VISION_LENGTH-abs_y)+j][<abs_x..<1]);
					//�k�W
					//
					map2[j][abs_x..<1]=copy(areas[area][num][RMAP][MAP_LENGTH-(AREA_VISION_LENGTH-abs_y)+j][0..AREA_VISION_WIDTH-abs_x]);
					
				}
			}
		} else {
			// �H�b�k�b
			if( link=AREA_D->query_section_info(area, num, "maplink_west") )
			{
				for(j=0;j<AREA_VISION_LENGTH;j++)
				{
					map2[j][0..abs_x]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[y_start+j][<abs_x..<1]);
					map2[j][abs_x..<1]=copy(areas[area][num][RMAP][y_start+j][0..AREA_VISION_WIDTH-abs_x]);
				}
			}
		}
	} else if( x_start+AREA_VISION_WIDTH - 1 > MAP_WIDTH )
	{
		int abs_x= x_start + AREA_VISION_WIDTH -MAP_WIDTH;
		//�H�b��
		if( y_start < 0 )
		{
			// �H�b���U��
			int abs_y=abs(y_start);
					
			for( j=0;j<AREA_VISION_LENGTH;j++)
			{
				//�̧Ƕ�J
				//���U
				if( j >= abs_y )
				{
					map2[j][0..AREA_VISION_WIDTH-abs_x-1]=copy(areas[area][num][RMAP][y_start+j][<AREA_VISION_WIDTH-abs_x..<1]);
					// �k�U
					if( link=AREA_D->query_section_info(area, num, "maplink_east") )
						map2[j][AREA_VISION_WIDTH-abs_x..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[j-abs_y][0..abs_x-1]);
				} else {
					//���W
					
					if( link=AREA_D->query_section_info(area, num, "maplink_north") )
					{
						map2[j][0..AREA_VISION_WIDTH-abs_x-1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[MAP_LENGTH-abs_y+j][<AREA_VISION_WIDTH-abs_x..<1]);
						if( link=AREA_D->query_section_info(link[0], link[1], "maplink_east") )
							map2[j][AREA_VISION_WIDTH-abs_x..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[MAP_LENGTH-abs_y+j][0..abs_x-1]);
					} else if( link=AREA_D->query_section_info(area, num, "maplink_east") )	{
						if( link=AREA_D->query_section_info(link[0],link[1], "maplink_north") )
							map2[j][AREA_VISION_WIDTH-abs_x..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[MAP_LENGTH-abs_y+j][0..abs_x-1]);
					}
				}
			}
			
		} else if( y_start+AREA_VISION_LENGTH-1 > MAP_LENGTH )
		{
			// �H�b���W��
			int abs_y=(y_start+AREA_VISION_LENGTH)-MAP_LENGTH;
			
			for( j=0;j<AREA_VISION_LENGTH;j++)
			{
				if( j >= AREA_VISION_LENGTH-abs_y )
				{
					// ���U
					if( link=AREA_D->query_section_info(area, num, "maplink_south") )
					{
						map2[j][0..AREA_VISION_WIDTH-abs_x-1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[j-(AREA_VISION_LENGTH-abs_y)][<AREA_VISION_WIDTH-abs_x..<1]);
						//�k�U
						if( link=AREA_D->query_section_info(link[0],link[1], "maplink_east") )
							map2[j][AREA_VISION_WIDTH-abs_x..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[j-(AREA_VISION_LENGTH-abs_y)][0..abs_x-1]);
					}
				} else {
					// ���W
					map2[j][0..AREA_VISION_WIDTH-abs_x-1]=copy(areas[area][num][RMAP][MAP_LENGTH-(AREA_VISION_LENGTH-abs_y)+j][<AREA_VISION_WIDTH-abs_x..<1]);
					//�k�W
					if( link=AREA_D->query_section_info(area, num, "maplink_east") )
					{
							map2[j][AREA_VISION_WIDTH-abs_x..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[MAP_LENGTH-(AREA_VISION_LENGTH-abs_y)+j][0..abs_x-1]);
					}
				}
			}
		} else {
			// �H�b���b
			if( link=AREA_D->query_section_info(area, num, "maplink_east") )
			{
				for(j=0;j<AREA_VISION_LENGTH;j++)
				{
					map2[j][0..AREA_VISION_WIDTH-abs_x]=copy(areas[area][num][RMAP][y_start+j][<AREA_VISION_WIDTH-abs_x..<1]);
					map2[j][AREA_VISION_WIDTH-abs_x..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[y_start+j][0..abs_x-1]);
					
				}
			}
		}
	} else {
		// X �m��
		
		if( y_start < 0 )
		{
			// �H�b�U
			int abs_y=abs(y_start);
			if( link=AREA_D->query_section_info(area, num, "maplink_north") )
			{
				map2[0..abs_y]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[<abs_y..<1]);
				map2[abs_y..<1]=copy(areas[area][num][RMAP][0..AREA_VISION_LENGTH-abs_y]);
			}
			
			for(j=0;j<AREA_VISION_LENGTH;j++)
				map2[j] = map2[j][x_start..x_start+AREA_VISION_WIDTH-1];
		
		} else if( y_start+AREA_VISION_LENGTH+1 > MAP_LENGTH )
		{
			int abs_y=(y_start+AREA_VISION_LENGTH)-MAP_LENGTH;
			// �H�b�W
			if( link=AREA_D->query_section_info(area, num, "maplink_south") )
			{
				map2[0..<abs_y]=copy(areas[area][num][RMAP][y_start..<1]);
				map2[<abs_y-1..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[0..abs_y]);
				
			}
			for(j=0;j<AREA_VISION_LENGTH;j++)
				map2[j] = map2[j][x_start..x_start+AREA_VISION_WIDTH-1];
		} else {
			//�ϰ줺
			map2 = copy(areas[area][num][RMAP][y_start..y_start+AREA_VISION_LENGTH-1]);
			for(j=0;j<AREA_VISION_LENGTH;j++)
				map2[j] = map2[j][x_start..x_start+AREA_VISION_WIDTH-1];
		}
	}


//	foreach(array aa in map2)
//		tell(find_player("cookys"),implode(aa,"")+"\n");
	//map2 = copy(areas[area][num][RMAP][y_start..y_start+AREA_VISION_LENGTH-1]);
	map3 = allocate(AREA_VISION_LENGTH, 0);

// ��ܰ϶��p�⤽��
	x_start = (x>=x_center)*(x+1-x_center) - ((x+x_center)>MAP_WIDTH)*(x_center+x-MAP_WIDTH);
	y_start = (y>=y_center)*(y+1-y_center) - ((y+y_center)>MAP_LENGTH)*(y_center+y-MAP_LENGTH);


	if( range == (AREA_VISION_WIDTH/3)*(AREA_VISION_WIDTH/3) )
	{	
		map3 = allocate(AREA_VISION_LENGTH, allocate(AREA_VISION_WIDTH, 1));
		//for(j=0;j<AREA_VISION_LENGTH;j++)
		//	map2[j] = map2[j][x_start..x_start+AREA_VISION_WIDTH-1];
	}
	else
	{
		for(j=0;j<AREA_VISION_LENGTH;j++)
			map3[j] = allocate(AREA_VISION_WIDTH);

		for(j=0;j<AREA_VISION_LENGTH;j++)
		{
		//	map2[j] = map2[j][x_start..x_start+AREA_VISION_WIDTH-1];

			for(i=0;i<AREA_VISION_WIDTH;i++)
			{
				if( map2[j][i] == HIY"�F"NOR )
				{
					for(k=j-4;k<=j+4&&k<AREA_VISION_LENGTH;k++)
						for(l=i-4;l<=i+4&&l<AREA_VISION_WIDTH;l++)
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

	for(j=0;j<AREA_VISION_LENGTH;j++)
	{
		retmap += "\e["+(j+2)+";1H"NOR WHT"�x"NOR;

		for(i=0;i<AREA_VISION_WIDTH;i++)
		{
			//tell(find_player("cookys"),sprintf("i:%d j:%d\n%O\n",i,j,map2));
			if( map3[j][i] )
			{
				//if( j == y-y_start && i == x-x_start )
				if( j==y_center-1 && i ==x_center-1 )
					retmap += ansi_part(map2[j][i])+HIC"��"NOR;
				else 		
					retmap += map2[j][i];
			}

			else
				retmap += WHT"��"NOR;
		}
		retmap += NOR WHT"�x\n"NOR;
	}

	// �ϤU����u
	retmap += "\e["+(j+2)+";1H"NOR WHT"�|["NOR+
	sprintf("%|18s", coor_short_name(loc)+NOR)+
	NOR WHT"]�w�w�w�w�w�w�w�w�w�w"+
	sprintf(NOR"%|10s"NOR WHT"�w"NOR"%|10s"NOR WHT, AREA_D->query_area_name(area), AREA_D->query_section_info(area, num, "name")||("��"+CHINESE_D->chinese_number(num+1)+"��"))+
	"�}\e["+(j+3)+
	";1H"NOR MAG"__________________________________________________________________\e[u"NOR;

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


varargs int valid_move(object me, string direction, array loc)
{
	int x, y, num, *worldmaploc, maplink;

	mapping data;
	string area;
	array nextloc;

	if( !objectp(me) ) return 0;

	if( !loc )
		loc = query_temp("location", me);

	if( !arrayp(loc) )
		return 0;
		
	x = copy(loc[X]);
	y = copy(loc[Y]);
	area = loc[AREA];
	num = loc[NUM];

	worldmaploc = MAP_D->query_world_map_location(area, num);

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

	nextloc = arrange_area_location(x, y, area, num);

	if( !valid_coordinate(x, y, area, num) )
		return 0;

	data = AREA_D->query_coor_data(nextloc);

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
	string area, my_idname;
	array loc, nextloc;
	string random_thing = RANDOM_STRING[random(sizeof(RANDOM_STRING))];

	if( !objectp(me) || !arrayp(loc = query_temp("location",me)) ) return;

	x = copy(loc[X]);
	y = copy(loc[Y]);
	area = loc[AREA];
	num = loc[NUM];
	
	if( !area_exist(area) ) return;

	worldmaploc = MAP_D->query_world_map_location(area, num);

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
		
		if( linked_map )
		{
			nextloc = arrange_area_location(x, y, linked_map[0], linked_map[1]);
			
			broadcast(loc, my_idname+"��"+CHINESE_D->to_chinese(direction)+"���}�C\n", 0, me);
			me->move(nextloc);
			broadcast(nextloc, my_idname+weather_go[NATURE_D->query_nature(query_maproom(loc))[NATURE_WEATHER][WEATHER_ID]]+"�A��"+CHINESE_D->to_chinese(corresponding[direction])+"���F�L�ӡC\n", 0, me);
			
			me->follower_move(loc, nextloc);
			return;
		}
		error(area+" �q�� "+linked_map[0]+" �X�f���~");
	}
	nextloc = arrange_area_location(x, y, area, num);

	data = AREA_D->query_coor_data(nextloc);

	delete_temp("go_resist", me);

	// �q���ϰ�ؿv������
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
		
		return tell(me, "���ؿv�����@��k��A�ثe�L�k�i�J�C\n");
	}

	remove_call_out(query_temp("call_out_handle/flowing", me));

	// �ˬd�S��g�a�κA������
	switch(data[TYPE])
	{
	case RIVER:
		cost_stamina = 100 - me->query_skill_level("sport")/2;

		switch( AREA_D->query_coor_data(nextloc, FLAGS) )
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

	me->follower_move(loc, nextloc);
}

void remove_player_icon(object me, array loc)
{
	int x, y, num;
	string area;
	object *others;

	if( !arrayp(loc) ) return;

	x = loc[X];
	y = loc[Y];
	area = loc[AREA];
	num = loc[NUM];

	if( !area_exist(area, num) || !valid_coordinate(x, y, area, num) || undefinedp(areas[area][num][RMAP]) || undefinedp(areas[area][num][MAP]) ) return;

	others = filter_array(present_objects(me)-({me}), (: userp($1) :));

	switch( sizeof(others) )
	{
	case 0:	areas[area][num][RMAP][y][x] = areas[area][num][MAP][y][x];break;
	case 1:	areas[area][num][RMAP][y][x] = ansi_part(areas[area][num][MAP][y][x]) + (query("gender", others[0]) == MALE_GENDER ? HIG + "��" : HIR + "��") + NOR;break;
	default:areas[area][num][RMAP][y][x] = ansi_part(areas[area][num][MAP][y][x]) + HIC"��"NOR;break;
	}
}

void set_player_icon(object me, array loc)
{
	int x, y, num;
	string area;
	object *others;

	if( !arrayp(loc) ) return;

	x = loc[X];
	y = loc[Y];
	area = loc[AREA];
	num = loc[NUM];

	if( !valid_coordinate(x, y, area, num) ) return;

	others = filter_array(present_objects(me) - ({ me }), (: userp($1) :));

	if( sizeof(filter_array(present_objects(me), (: userp($1) :))) > 1 )
		areas[area][num][RMAP][y][x] = ansi_part(areas[area][num][MAP][y][x]) + HIC"��"NOR;
	else
		areas[area][num][RMAP][y][x] = ansi_part(areas[area][num][MAP][y][x]) + (query("gender", me) == MALE_GENDER ? HIG + "��" : HIR + "��") + NOR;
}


// �N�a���ഫ�� HTML �榡
void translate_map_to_html(string area, int num)
{
	array arr;
	string map_graph = query_area_idname(area, num)+"\n";

	if( area_exist(area, num) )
	{
		foreach( arr in areas[area][num][RMAP] )
			map_graph += kill_repeat_ansi(implode(arr, "")+" \n");

		write_file("/www/map/areamap_"+area+"_"+num+"_ansi", map_graph, 1);

		HTML_D->make_html(map_graph, ([
			"bgcolor"	:"black",
			"filename"	:"/www/map/areamap_"+area+"_"+num+".html",
			"title"		:remove_ansi(MUD_FULL_NAME+" "+AREA_D->query_area_idname(area, num)),
			"refresh"	:300,
			"fontsize"	:12,
		    ]));
	}
}

// �Ω��ʭק�ϰ��Ʈw���e
/*
void change_data()
{
	foreach( string area, mapping map1 in areas )
	foreach( mixed num, mapping map2 in areas )
	{
		if( !intp(num) ) continue;
		for(int y=0;y<100;y++)
		for(int x=0;x<100;x++)
		{

		}
	}
}
*/
/* ���m�Ҧ���� */
void restore_all_data()
{
	string area;
	mixed num;

	areas = allocate_mapping(0);

	/* ��l��Ū���Ҧ��ϰ��T */
	foreach( area in get_dir(AREA_ROOT) )
	{
		if( file_size(AREA_PATH(area)) != -2 ) continue;

		initialize_area_data(area);

		areas[area][INFO] = restore_variable(read_file(AREA_INFO(area)));

		// Ū���Ҧ��l�ϰ���
		foreach( num in get_dir(AREA_PATH(area)) )
		{
			if( !sscanf(num, "%d", num) ) continue;
			
			reset_eval_cost();

			initialize_section_data(area, num);

			CHANNEL_D->channel_broadcast("sys", "�ϰ�G��l�ưϰ� "+area+" ���� "+num+" �Ϧa�ϸ�Ʈw");

			areas[area][num][DATA] = restore_variable(read_file(AREA_NUM_DATA(area, num)));
			areas[area][num][MAP] = restore_variable(read_file(AREA_NUM_MAP(area, num)));
			areas[area][num][RMAP] = copy(areas[area][num][MAP]);
		}
	}

	// ��Ҧ��ϰ�i��s���Ƨ�
	assign_areas_num();
}
string query_name()
{
	return "���Ϩt�ΥD��(AREA_D_MAIN)";
}
