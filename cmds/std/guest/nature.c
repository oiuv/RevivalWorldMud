/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : nature.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-12
 * Note   : nature ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <time.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <nature.h>
#include <nature_data.h>

inherit COMMAND;

string help = @HELP
	�з� nature ���O�C
HELP;

private void command(object me, string arg)
{
	string msg;
	mapping nature = NATURE_D->query_nature();
	
	if( !is_command() ) return;

	msg = HIW"�ثe���u�`���u"+TIME_D->season_period(TIME_D->query_gametime_array()[MON])+"�ѡv\n�A�U�D�n�����۵M�{�H�p�U�G\n"NOR;
	msg += "����/�ϰ�                �Ѯ�  �ū�  ����  ���t  ����ɶ�\n";
	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
	
	foreach( object env, mapping data in nature )
		msg += sprintf("%-25s%-6s%-6d%-6d%-6d%-10d\n", CITY_D->query_city_id(query("city", env), query("num", env))||file_name(env), data[CURRENT_WEATHER][WEATHER_CHINESE], data[TEMPERATURE], data[HUMIDITY], data[WIND], data[NEXTTIME_ARRANGE] );

	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
	me->more(msg);
}








