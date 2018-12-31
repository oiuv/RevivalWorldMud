/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_wizhall_7.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-23
 * Note   : �зǩж�
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <daemon.h>

inherit STANDARD_ROOM;

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;

/*
varargs string percent_bar(int len, string str, int cur, int max, int flag)
{
	int percent_len;
	float percent = 0.;

	if( !len ) return "Something Wrong With Bar.";
	if( max ) percent = to_float(cur)/to_float(max);
	percent_len = to_int(len * percent);
	str = sprintf("%|:" + len + "s", str);
	return sprintf("%s%s"NOR"%s%s"NOR" %s",
	    U + BBLU, (percent_len ? str[0..percent_len-1] : ""),
	    BRED, str[percent_len..],
	    (flag ? sprintf("%s%.1f%%", (percent >= 0.1 ? "":" "), percent*100):"")
	);
}

void refresh_clode_time()
{
	int days, hours, minutes;
	int time = 1129284000 - time();
	string bar, percent;
	if( time < 0 )
	{
		set("status", HIW"��"NOR WHT"����"HIY"���V�F�I�I\n"NOR);
		return;
	}
	
	days = time / 60 / 60 / 24;
	hours = time / 60 / 60 % 24;
	minutes = time / 60 % 60;
	percent = sprintf("%.2f", (7257600-time)/72576.);
	bar = percent_bar(72, percent+"%", 7257600-time, 7257600);
	set("status", CYN"�Z��"HIW"��"NOR WHT"����"NOR"("WHT"Cl"NOR"o"HIW"de"NOR")"CYN"���V����l�٦� "HIC+days+NOR CYN" �� "HIC+hours+NOR CYN" �� "HIC+minutes+NOR CYN" ��(�w�g�L�F "HIC+percent+"%"NOR CYN")�C\n\n"+bar+"\n\n"NOR);

	call_out( (: refresh_clode_time :), 60 );
}
*/

void create()
{
	//int num;
	//array wizhallloc;

	set("short", HIW"�Ův"NOR WHT"�j�U"NOR);
	set("long",@TEXT
�@�@�Ův�j�U���W���a���ۤ��j�ɥN��護�檺�����ݿO�A�Υ~�������ǥѤ��m�}
�ɪ������M�g�i�ӡA�A�P����|�P���ӳ��ˮ𮧡A���G���W�n�Q�]�k���A�o�̬O��
�`�Ův�̥洫�߱o���a��A�b�A���e���\��ۤ@���d���O�W���K���F�K�K�³ª��r
���A�W���g�ۤ@�����Ѫ��]�k��r�C
TEXT);

	set("exits", ([
		"down" : "/wiz/wizhall/room_wizhall_4",
		"up" : "/wiz/wizhall/room_wizhall_17",
	]));

	set("objects", ([
		"/obj/board/wizhall" : 1,
	]));

	set("exits/south", CITY_D->query_section_info("wizard", 0, "wizhall"));

	/*
	foreach( string city in CITY_D->query_cities() )
	{
		num = 0;
		
		while(CITY_D->city_exist(city, num++))
		{
			wizhallloc = CITY_D->query_section_info(city, num-1, "wizhall");
			
			if( arrayp(wizhallloc) )
				set("exits/"+city+num, wizhallloc);
			else
				set("exits/"+city+num, ({ 49,49,0,city,num-1,0,"49/49/0/"+city+"/"+(num-1)+"/0" }));
		}
	}
	
	foreach( string area in AREA_D->query_areas() )
	{
		num = 0;
		
		while(AREA_D->area_exist(area, num++))
			set("exits/"+area+num, ({ 49,49,0,area,num-1,0,"49/49/0/"+area+"/"+(num-1)+"/0" }));
	}
	*/

	::reset_objects();
	
	//refresh_clode_time();
}

varargs void catch_tell(string msg)
{
	log_file("wizhall/chatlog", msg, -1);
}
