/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : nature_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-07-22
 * Note   : �۵M���Һ��F, ���, �Ѩa
 * Update :
 *
 -----------------------------------------
 */

#include <nature.h>
#include <nature_data.h>
#include <ansi.h>
#include <time.h>
#include <daemon.h>
#include <message.h>


// �۵M���
mapping nature;

string season;
int hour;

int flag_temperature(int temp)
{
	int flag;
	switch(temp)
	{
	case TEMPERATURE_LV_0:
		flag = WE_TEMPERATURE_0; break;
	case TEMPERATURE_LV_1:
		flag = WE_TEMPERATURE_1; break;
	case TEMPERATURE_LV_2:
		flag = WE_TEMPERATURE_2; break;
	case TEMPERATURE_LV_3:
		flag = WE_TEMPERATURE_3; break;
	default: break;
	}

	return flag;
}

int hour_plus(int h)
{
	h += hour;

	if( h >= 24 )
		return h - 24;
	else
		return h;
}

// �i��Ѯ�Ƨ�
void arrange_queue(object environment)
{
	int season_term = SEASON_DATA[season][SEASON_ELEMENT];
	int temperature_term = flag_temperature(nature[environment][TEMPERATURE]);
	int humidity_term = nature[environment][HUMIDITY];
	int wind_term = nature[environment][WIND];
	string weather_msg;

	array fit_weather;

	fit_weather = filter_array(ALL_WEATHER, (: 
		($(season_term) & $1[WEATHER_OCCUR_TERM]) &&
		($(temperature_term) & $1[WEATHER_OCCUR_TERM]) &&
		($(humidity_term) & $1[WEATHER_OCCUR_TERM]) &&
		($(wind_term) & $1[WEATHER_OCCUR_TERM]) :));

	// �Y�S������@���ŦX
	if( !sizeof(fit_weather) )
		fit_weather = ({ WEATHER_CLOUDY });

	nature[environment][CURRENT_WEATHER] = probability(allocate_mapping(fit_weather, map(fit_weather, (: $1[WEATHER_PROBABILITY] :))));
	nature[environment][NEXTTIME_ARRANGE] = hour_plus(probability(nature[environment][CURRENT_WEATHER][WEATHER_HOLDTIME_TERM]));
	
	switch(nature[environment][CURRENT_WEATHER][WEATHER_ID])
	{
		case SUNNY: 	weather_msg = HIC"\n    ��"NOR CYN"�Ʀ��ն��b�ѪŤW�w�w�ƵۡA�۷�M�n�C"NOR; break;
		case CLOUDY: 	weather_msg = HIW"\n    ��"NOR WHT"�ѪŧG���ۯQ���A�@�T���B���Ӫ��ˤl�C"NOR; break;
		case RAINY: 	weather_msg = HIW"\n    ��"NOR WHT"�w���n�v���ǤF�}�ӡA�}�l�U�_�F�ֲִӫB�C"NOR; break;
		case WINDY: 	weather_msg = HIY"\n    ��"NOR YEL"�j�a�}�l��_�F�j���A�a�_�Ӫ��F�дX�G�ϤH�C���}�����C"NOR; break;
		case SNOWY: 	weather_msg = WHT"\n    ��"NOR HIW"���檺�H�����A������z���ճ��w�w�q�ѤW�Ƹ�"NOR; break;
		case SHOWER: 	weather_msg = HIW"\n    ��"NOR WHT"�u�F���F���v���p�B�n�A����F�j�a�W���U���C"NOR; break;
		case TYPHOON: 	weather_msg = HIG"\n    ��"NOR GRN"�䭷���a�ۻ��Bŧ���ӨӡA�U���Q�R�ݱo�����}�H�C"NOR; break;
		case FOEHN: 	weather_msg = HIY"\n    ��"NOR YEL"�I�����a�۰��šA�Ҹg���B�ܱo�@�����I�C"NOR; break;
	}
	
	filter_array(all_inventory(environment), (: userp($1) :))->catch_tell(weather_msg+"\n\n");
}

// �Ұʦ۵M���Ү���
void activate_nature(object environment)
{
	if( !objectp(environment) )
		error("NATURE_D activate_nature() arg 1 error.");

	nature[environment] = allocate_mapping(0);
	
	nature[environment][CURRENT_DISASTER] = allocate(5);

	// �Ѯ�ƥ�
	nature[environment][EVENT] = query("nature_event", environment);

	// ��l�Ʒū�
	nature[environment][TEMPERATURE] = range_random(SEASON_DATA[season][SEASON_TEMPERATURE_LOW], SEASON_DATA[season][SEASON_TEMPERATURE_HIGH]);

	// ��l�����
	nature[environment][HUMIDITY] = probability(HUMIDITY_PROBABILITY[season]);

	// ��l�ƭ��t
	nature[environment][WIND] = probability(WIND_PROBABILITY[season]);

	// �w�ƤѮ�Ƨ�
	arrange_queue(environment);
}

// �פ�۵M���Ү���
void deactivate_nature(object environment)
{
	map_delete(nature, environment);
}


// ���o�ثe�۵M���ұ���
varargs array query_nature(object environment)
{
	if( undefinedp(environment) || undefinedp(nature[environment]) )
		return copy(nature);
	
	return ({ nature[environment][CURRENT_WEATHER], 
		nature[environment][CURRENT_DISASTER],
		nature[environment][TEMPERATURE],
		nature[environment][HUMIDITY],
		nature[environment][WIND] });
}

void game_month_process()
{
	array gametime = TIME_D->query_gametime_array();
	
	/* �u�`�ܴ��ɧ�s�Ѯ��� */
	switch( gametime[MON] )
	{
		case 2:
			shout(HIC"\n    ��K"NOR CYN"�ѨӤF�A�᭻�w�w�ƴ��F�L��......�C\n"NOR);
			break;
		case 5:
			shout(HIR"\n    ��L"NOR RED"�Ѩ�F�A��Ű����a�ɰ��F......�C\n"NOR);
			break;
		case 8:
			shout(HIY"\n    ���"NOR YEL"�ѨӤF�A�@�}�D�N�w�wŧ��......�C\n"NOR);
			break;
		case 10:
			shout(HIY"\n    ��"NOR YEL"������ı�w�M�i�J�F�`��......�C\n"NOR);
			break;
		case 11:
			shout(HIW"\n    ��V"NOR WHT"�Ѩ�F�A�j�a�I�R�L�n......�C\n"NOR);
			break;
		default:return;
	}
}

// �� TIME_D �I�s, �C�C���p�ɩI�s�@��(��ڮɶ� 2.5 ����)
void game_hour_process()
{
	array gametime = TIME_D->query_gametime_array();

	season = TIME_D->season_period(gametime[MON]);
	hour = gametime[HOUR];

	// ��ܤӶ��ɰ_
	if( SEASON_DATA[season][SEASON_SUNRISE_HOUR] == hour )
		shout(HIC"\n    ��"NOR CYN"�¶��w�w�ѪF��ɰ_...\n\n"NOR);

	// ��ܤӶ����U
	else if( SEASON_DATA[season][SEASON_SUNSET_HOUR] == hour )
		shout(HIY"\n    ��"NOR YEL"�i���w�w�Ѧ�踨�U...\n\n"NOR);


	foreach( object environment, mapping data in nature )
	{
		if( !objectp(environment) )
		{
			nature[environment] = 0;
			continue;
		}

		// �L�շū�
		if( hour >= 5 && hour <= 16 ) 
			nature[environment][TEMPERATURE] += range_random(-1, 2);
		else
			nature[environment][TEMPERATURE] += range_random(-2, 1);

		if( nature[environment][TEMPERATURE] > 38 )
			nature[environment][TEMPERATURE] = 38;
		else if( nature[environment][TEMPERATURE] < -2 )
			nature[environment][TEMPERATURE] = -2;
	
		// �L�����
		nature[environment][HUMIDITY] = probability(HUMIDITY_PROBABILITY[season]);

		// �L�խ��t
		nature[environment][WIND] = probability(WIND_PROBABILITY[season]);

		if( --nature[environment][NEXTTIME_ARRANGE] <= 0 )
			arrange_queue(environment);

/*
		CHANNEL_D->channel_broadcast("sys", file_name(environment)+" ����ɶ��G"+data[NEXTTIME_ARRANGE]+" �Ѯ�G"+nature[environment][CURRENT_WEATHER][WEATHER_CHINESE]+" �ūסG"+query_current_temperature(environment)+" ����G"+nature[environment][HUMIDITY]+" ���t�G"+nature[environment][WIND]);

		foreach( array disaster in nature[environment][CURRENT_WEATHER][WEATHER_RELATED_DISASTER] )
		{
			
			CHANNEL_D->channel_broadcast("sys", "�޵o"+disaster[0]+"���v�G"+
			    ((disaster[DISASTER_OCCUR_TERM] & nature[environment][HUMIDITY])&&
			    (disaster[DISASTER_OCCUR_TERM] & flag_temperature(nature[environment][TEMPERATURE]))&&
			    (disaster[DISASTER_OCCUR_TERM] & nature[environment][WIND]) ? range_random(disaster[DISASTER_OCCUR_PROB]-10,disaster[DISASTER_OCCUR_PROB]+10) +"/1000" : "���|�o��"));
		}
*/
	}
}

void debug()
{
	printf("%O\n", nature);
}

void create()
{
	nature = allocate_mapping(0);

	game_hour_process();

	// �ҰʩҦ� maproom �۵M�欰
	foreach( object environment in MAP_D->query_all_maproom() )
	{
		if( query("no_weather", environment) ) 
			continue;

		activate_nature(environment);
	}
}
string query_name()
{
	return "�۵M���Ҩt��(NATURE_D)";
}
