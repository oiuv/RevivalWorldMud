/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : exchange.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-17
 * Note   : exchange ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        �з� exchange ���O�C
HELP;

private void do_command(object me, string arg)
{
	mapping exchange_data = EXCHANGE_D->query_exchange_data();
	
	string msg = HIW"�U�D�n�����ײv��Ӫ�p�U�G\n"NOR;
	
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	foreach( string money_unit, float exchange in exchange_data )
	{
		msg += sprintf("%-30s %.6f\n", CITY_D->query_city_idname(MONEY_D->money_unit_to_city(money_unit)), exchange);
	}
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	return tell(me, msg+"\n", CMDMSG);
}
