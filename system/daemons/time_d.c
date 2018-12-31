/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : time_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-03-04
 * Note   : �ɶ����F, ��ڤ@�p��==�C���@��, �g�٥@�ɤ����ɶ��B�z�ݭn
 *          �Q������T, �G�ĥ� heart_beat 0.1 ��B�z�ɶ����e
 * Update :
 *  o 2002-09-29 Clode �ץ��] 0.1 �� heart_beat() ��T�פ����y�� gametime ���~�t  
 *  o 2003-08-29 Clode �s�W crontab �u�@�Ƶ{�\��
 -----------------------------------------
 */

#define DATA_PATH		"/data/daemon/time.o"

#include <ansi.h>
#include <daemon.h>
#include <localtime.h>
#include <time.h>
#include <mudlib.h>
#include <message.h>

private int gametime;
private nosave int realtime;

private nosave int tick;
private nosave mapping event = allocate_mapping(0);

private nosave int *real_time = allocate(6);
private nosave int *game_time = allocate(6);

void calculate_GNP()
{
	foreach( string city in CITY_D->query_cities() )
		MONEY_D->calculate_NNP(city);
		
	MONEY_D->broadcast_GNP();
}

void happy_game_new_year()
{
	//string money;
	//string dst_unit;

	shout(BEEP"\n"MUD_CHINESE_NAME HIW+CHINESE_D->chinese_number(game_time[YEAR])+HIC"�~�ש�Ө�A�|�@�w�ˡI�I�I\n"NOR);
	
	/*
	shout(HIW"\n�u�W�C�H�N�i��o�ӤH�`�겣 1% ��"HIW"���]"HIW"�I�I�I\n"NOR);
	shout(HIW"\n�u�W�C�H�N�i��o�ӤH�`�겣 1% ��"HIW"���]"HIW"�I�I�I\n"NOR);
	shout(HIW"\n�u�W�C�H�N�i��o�ӤH�`�겣 1% ��"HIW"���]"HIW"�I�I�I\n"NOR);
	foreach( object user in users() )
	{
		if( environment(user) && user->is_user_ob() )
		{
			money = "0";
			
			catch(money = count(count(MONEY_D->query_assets(user->query_id(1)), "*", 1), "/", 100));
			
			dst_unit = MONEY_D->city_to_money_unit(query("city", user));
			
			if( dst_unit )
				money = EXCHANGE_D->convert(money, "RW", dst_unit);

			user->earn_money(dst_unit || "RW", money);
			
			tell(user, HIC+pnoun(2, user)+"�`�@��o�F"+HIY+QUANTITY_D->money_info(dst_unit || "RW", money)+HIC"�C\n"NOR, ENVMSG);
		}	
	}
	*/
}

void happy_real_new_year()
{
	shout(BEEP WHT"\n�褸" HIW+CHINESE_D->chinese_number(real_time[YEAR])+WHT"�~�ש�Ө�A�|�@�w�ˡI�I�I\n"NOR);
	shout(BEEP WHT"\n�褸" HIW+CHINESE_D->chinese_number(real_time[YEAR])+WHT"�~�ש�Ө�A�|�@�w�ˡI�I�I\n"NOR);
	shout(BEEP WHT"\n�褸" HIW+CHINESE_D->chinese_number(real_time[YEAR])+WHT"�~�ש�Ө�A�|�@�w�ˡI�I�I\n"NOR);
}

// �C���ɶ����u�@�Ƶ{
private nosave array game_crontab = ({
//"min hour wday mday mon year" : function
"0 * * * * *"		, (: NATURE_D->game_hour_process() :),			"�C�p�ɦ۵M�����ܰ�",
"0 0 * 0 0 *"		, (: happy_game_new_year :),				"�C�~�o�@�����]",
"0 0 * 0 * *"		, (: SALARY_D->game_month_process() :),			"�C��B�z�@���~��",
"0 0 * 0 * *"		, (: NATURE_D->game_month_process() :),			"�C��B�z�@���u�`�ܤ�",
"30 0 * 0 * *"		, (: calculate_GNP :),					"�C���@��GNP",
"40 0 * 0 * *"		, (: CITY_D->fund_handler() :),				"�C��B�z�@���F���g�O��X�P�|�����J",
});

// �u��ɶ����u�@�Ƶ{
private nosave array real_crontab = ({
//"min hour wday mday mon year" : function
"*/5 * * * * *"		, (: CITY_D->time_distributed_save() :),		"�C������������Ƥ����x�s",
//"0,12,24,36,48 * * * * *", (: MNLMUDLIST_D->distributed_connect() :),            "�C�Q�G������s MNLMUDLIST ����",
//"1,13,25,37,49 * * * * *", (: MYSQL_SCHEDULE_D->refresh_twmudlist() :),          "�C�Q�G������s mysql twmudlist",
"*/10 * * * * *"	, (: NEWS_D->broadcast_news() :),			"�C�Q�����s���s�D",
"*/10 * * * * *"	, (: save_object(DATA_PATH) :),				"�C 10 ���������@�� TIME_D ���",
"3 * * * * *"		, (: LOGIN_D->time_schedule() :),			"�C�p�ɬ����P����n�J���",
"4 * * * * *"		, (: LABOR_D->save_data() :),				"�C�p�ɬ������~�Ҥu���",
"5 * * * * *"		, (: SYSDATABASE_D->save() :),				"�C�p�ɬ����t�θ�Ʈw",
"6 * * * * *"		, (: MAP_D->save() :),					"�C�p�ɬ����@���a�ϤW�����~",
"7 * * * * *"		, (: HTML_D->create_html() :),				"�C�p�ɫظm�@����������",
"8 * * * * *"		, (: LOGIN_D->reset_login_count(real_time[HOUR]) :), 	"�C�p�ɭp��@�� login count",
"9 * * * * *"		, (: delete("mnlmudlist/maxusers/hour/"+real_time[HOUR], SYSDATABASE_D->query_ob()) :), "�C�p�ɭp��@�� maxuser",
//"10 * * * * *"	, (: MYSQL_SCHEDULE_D->refresh_who() :),		"�C�p�ɧ�s�@�� mysql who",
//"11 * * * * *"	, (: MYSQL_SCHEDULE_D->refresh_city_map() :),		"�C�p�ɧ�s�@�� 3dmap",
//"12 * * * * *"	, (: MYSQL_SCHEDULE_D->refresh_top_rich() :),		"�C�p�ɧ�s�@�� top_rich",
"* * * * * *"		, (: CITY_D->allcity_material_growup() :),		"�C�@���������@���������",
"0 */4 * * * *"		, (: SHOPPING_D->pop_product_choose() :),		"�C�|�p�����ܤ@���y��ӫ~",
"0 0 * 0 0 *"		, (: happy_real_new_year :),				"�u��ɶ��L�~",
"59 22 * * * *"		, (: CITY_D->reset_collection_record() :),		"���]�Ķ���T",
"55 5 * * * *"		, (: CLEAN_D->clean_user() :),				"�C�ѲM���@�����a���",
"0 3 * * * *"		, (: PPL_LOGIN_D->reset_newchar() :),			"�C�ѭ��]�@���s������U��",
"13 * * * * *"		, (: EXCHANGE_D->process_per_hour() :),			"�C�p�ɳB�z�@���ײv�ܰ�",
"14 * * * * *"		, (: EMOTE_D->save() :),				"�C�p���x�s�@�� EMOTE_D ���",
});

private nosave mapping crontab_process = allocate_mapping(0);

// �i���N call_out ���\��, �åH 0.1 ���ɶ����
void add_event(function fevent, int delay_time)
{
	if( !sizeof(event) || !event[fevent] ) 
		event = ([ fevent : delay_time ]);
	else
		event[fevent] = delay_time;
}

// ����ƥ�
void exec_event(function fevent)
{
	if( objectp(function_owner(fevent)) )
		evaluate(fevent);

	map_delete(event, fevent);
}

string replace_ctime(int t)
{
	string month, ctime = ctime(t);

	switch(ctime[4..6])
	{
		case "Jan"	:	month = "01";break;
		case "Feb"	:	month = "02";break;
		case "Mar"	:	month = "03";break;
		case "Apr"	:	month = "04";break;
		case "May"	:	month = "05";break;
		case "Jun"	:	month = "06";break;
		case "Jul"	:	month = "07";break;
		case "Aug"	:	month = "08";break;
		case "Sep"	:	month = "09";break;
		case "Oct"	:	month = "10";break;
		case "Nov"	:	month = "11";break;
		case "Dec"	:	month = "12";break;
	}

	return sprintf("%s/%s/%s %s", month, (ctime[8]==' '?"0"+ctime[9..9]:ctime[8..9]), ctime[<4..<1], ctime[11..15]);
}

string season_period(int m)
{
	switch(m)
	{
		case 2..4:	return "�K";
		case 5..7:	return "�L";
		case 8..10:	return "��";
		case 11:	return "�V";
		case 0..1:	return "�V";
		default:	return 0;
	}
}

string week_period(int w)
{
	switch(w)
	{       
		case 0: 	return "��";
		case 1: 	return "�@";
		case 2: 	return "�G";
		case 3: 	return "�T";
		case 4: 	return "�|";
		case 5: 	return "��";
		case 6: 	return "��";
		default:	return 0;
	}
}

string hour_period(int h)
{
	switch(h)
	{
		case 0..5: 	return "���";
		case 6..11: 	return "�W��";
		case 12:	return "����";
		case 13..18:	return "�U��";
		case 19..23:	return "�ߤW";
		default:	return 0;
	}
}

/* �^�Ǯɶ��}�C ({ �� �� § �� �� �~ }) */
int *query_gametime_array()
{
	return game_time;
}	

int *query_realtime_array()
{
	return real_time;
}

/* �^�ǼƦ�ɶ���� �U�� 3:39 */
string gametime_digital_clock()
{
	int h = game_time[HOUR];
	int m = game_time[MIN];

	return hour_period(h)+" "+(h==12 || (h%=12)>9?""+h:" "+h)+":"+(m>9?""+m:"0"+m);
}
string realtime_digital_clock()
{
	int h = real_time[HOUR];
	int m = real_time[MIN];

	return hour_period(h)+" "+(h==12 || (h%=12)>9?""+h:" "+h)+":"+(m>9?""+m:"0"+m);
}

string time_description(string title, int *t)
{
	return sprintf( NOR WHT+title+NOR"%s�~�M%s�M%s��%s��P��%s�M%s%s�I%s��"NOR
	    ,t[YEAR]==1?"��":CHINESE_D->chinese_number(t[YEAR])
	    ,season_period(t[MON])
	    ,!t[MON]?"��":CHINESE_D->chinese_number(t[MON]+1)
	    ,CHINESE_D->chinese_number(t[MDAY]+1)
	    ,week_period(t[WDAY])
	    ,hour_period(t[HOUR])
	    ,CHINESE_D->chinese_number(t[HOUR] > 12 ? t[HOUR]%12 : t[HOUR])
	    ,CHINESE_D->chinese_number(t[MIN])
	);
}

string game_time_description()
{
	return time_description(MUD_CHINESE_NAME, game_time);
}

string real_time_description()
{
	return time_description("����", real_time);
}

int query_game_time()
{
	return gametime*60;
}

int query_real_time()
{
	return realtime;
}

int *analyse_time(int t)
{
	int *ret = allocate(6);
	string ctime;

	ctime = ctime(t);

	sscanf(ctime,"%*s %*s %d %d:%d:%*d %d",ret[MDAY], ret[HOUR], ret[MIN], ret[YEAR]);

	switch(ctime[0..2])
	{
		case "Sun": ret[WDAY] = 0; break;
		case "Mon": ret[WDAY] = 1; break;
		case "Tue": ret[WDAY] = 2; break;
		case "Wed": ret[WDAY] = 3; break;
		case "Thu": ret[WDAY] = 4; break;
		case "Fri": ret[WDAY] = 5; break;
		case "Sat": ret[WDAY] = 6; break;
		default	  : return 0;
	}

	switch(ctime[4..6])
	{
		case "Jan": ret[MON] = 0; break;
		case "Feb": ret[MON] = 1; break;
		case "Mar": ret[MON] = 2; break;
		case "Apr": ret[MON] = 3; break;
		case "May": ret[MON] = 4; break;
		case "Jun": ret[MON] = 5; break;
		case "Jul": ret[MON] = 6; break;
		case "Aug": ret[MON] = 7; break;
		case "Sep": ret[MON] = 8; break;
		case "Oct": ret[MON] = 9; break;
		case "Nov": ret[MON] = 10; break;
		case "Dec": ret[MON] = 11; break;
		default	  : return 0;
	}
	ret[MDAY] -= 1;
	return ret;
}

void process_crontab(array crontab, int *timearray)
{
	int i, j, row, divider, fit, timecost, crontabsize;
	string s, script, note, *timescript;
	function fp;

	crontabsize = sizeof(crontab);

	for(row=0;row<crontabsize;row+=3)
	{
		reset_eval_cost();
		script = crontab[row];
		fp = crontab[row+1];
		note = crontab[row+2];

		timescript = allocate(6);
		
		if( sscanf(script, "%s%*(( |\t)+)%s%*(( |\t)+)%s%*(( |\t)+)%s%*(( |\t)+)%s%*(( |\t)+)%s%*(( |\t)+)%s", 
			timescript[0],
			timescript[1],
			timescript[2],
			timescript[3],
			timescript[4],
			timescript[5]) != 11 ) continue;
		
		for(i=0;i<6;i++)
		{
			fit = 0;

			foreach(s in explode(timescript[i], ","))
			{
				j = to_int(s);
				
				if( !undefinedp(j) )
				{
					if( j == timearray[i] )
					{
						fit = 1;
						break;
					}
				}
				else if( s == "*" || (sscanf(s, "*/%d", divider) && !(timearray[i]%divider)) )
				{
					fit = 1;
					break;
				}
			}
			
			if( !fit ) break;
		}
		
		if( !fit ) 
			continue;

		timecost = time_expression {
			catch(evaluate(fp));
		};
		
		if( timecost > 1000000 )
			CHANNEL_D->channel_broadcast("sys", sprintf("TIME_D: crontab '%s'(%.6f sec) %s", script, timecost/1000000., note));
	}
}

// �W�ߥX�ӨC�����禡
void process_per_second()
{

	
}

void reset_gametime()
{
	gametime = -20000000;	
}

/* �C���ɶ��C�@����(�Y��ڮɶ��C2.5��)����@�� process_gametime */
void process_gametime()
{
	game_time = analyse_time(++gametime * 60);
	game_time[YEAR] -= 1863;

	process_crontab(game_crontab, game_time);
}

/* �u��ɶ��C�@��������@�� process_realtime */
void process_realtime()
{
	array localtime = localtime(realtime);
	
	real_time = ({ 
		localtime[LT_MIN], 
		localtime[LT_HOUR], 
		localtime[LT_WDAY], 
		localtime[LT_MDAY]-1, 
		localtime[LT_MON], 
		localtime[LT_YEAR] });
	
	process_per_second();

	if( !localtime[LT_SEC] )
		process_crontab(real_crontab, real_time);
}

// �C 1 ��B��@��
// ��ڤ@�ѵ���C���@��

void heart_beat()
{
	realtime = time();

	// ���� event �B�z
	if( sizeof(event) )
	foreach( function fevent, int delay_time in event )
		if( !--event[fevent] )
			exec_event(fevent);

	// �C 2 ��۷��C���@����, time �C�W�[ 1 �N��C���@����
	if( !(++tick % 2) ) 
		process_gametime();

	process_realtime();
}

private void create()
{
	if( !restore_object(DATA_PATH) )
		save_object(DATA_PATH);
	
	game_time = allocate(6);
	real_time = allocate(6);
	event = allocate_mapping(0);
	
	process_gametime();
	set_heart_beat(10);
}

int remove()
{
	return save_object(DATA_PATH);
}

string query_name()
{
	return "�ɶ��t��(TIME_D)";
}
