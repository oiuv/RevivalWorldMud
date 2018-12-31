/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : shopping.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-30
 * Note   : shopping ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� shopping ���O�C
HELP;

private void command(object me, string arg)
{
	int num, time = time();
	string city, msg;
	
	if( !is_command() ) return;
	
	msg = HIW"����          �`�@���B      �ثe���B      �w�����B      ���O�t�v  �U����s  ���\n"NOR;
	msg +=NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;


	foreach(city in CITY_D->query_cities() )
	{
		for(num=0;CITY_D->city_exist(city, num);num++)
		{
			msg += sprintf("%-14s%-14s%-14s%-14s%-14d%4d  %d\n",
				CITY_D->query_city_id(city, num),
				SHOPPING_D->query_shopping_info(city, num, "totalmoney"),
				SHOPPING_D->query_shopping_info(city, num, "money"),
				SHOPPING_D->query_shopping_info(city, num, "predictmoney"),
				SHOPPING_D->query_shopping_info(city, num, "shoppingrate"),
				3600 - (time - SHOPPING_D->query_shopping_info(city, num, "moneyrehashtime")),
				SHOPPING_D->shopping_cost_ratio(city, num),
				);
		}
	}
	msg +=NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	tell(me, msg, CMDMSG);
}