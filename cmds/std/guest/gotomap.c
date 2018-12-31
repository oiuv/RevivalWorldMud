/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : gotomap.c
 * Author : Cookys@RevivalWorld
 * Date   : 2002-3-18
 * Note   : �����i�J�a�ϫ��O�C
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <location.h>
#include <map.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� gotomap ���O�C
HELP;

private void do_command(object me, string arg)
{
	string city;
	int num, x, y;
	array loc;
	
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "�п�J���T���a�ϦW��, �s��, �y�СC(��: gotomap center 1 10,10)\n");
	
	if( sscanf(arg, "'%s' %d %d,%d", city, num, x, y) ==4 || sscanf(arg, "%s %d %d,%d", city, num, x, y) == 4 )
	{
		loc = arrange_city_location(x-1, y-1, city, num-1);
		
		if( !MAP_D->query_map_system(loc) )
			return tell(me, "�o�O���X�z���a�Ϯy�СC\n");
				
		msg(HIW"$ME���i�q���a�Ϯy�� "+NOR WHT+arg+HIW" ���Ŷ��ǰe�h���C\n"NOR, me, 0, 1);
	
		me->move(loc);
		
		msg(HIW"$ME��M�q�Ŷ��ǰe�h�������F�X�ӡC\n"NOR, me, 0, 1);
	}
	else if( sscanf(arg, "'%s' %d", city, num) == 2 || sscanf(arg, "%s %d", city, num) == 2 )
	{
		loc = CITY_D->query_section_info(city, num-1, "wizhall");

		if( !MAP_D->query_map_system(loc) )
			return tell(me, "�o�O���X�z���a�Ϯy�СC\n");	
		
		msg(HIW"$ME���i�q���a�Ϯy�� "+NOR WHT+arg+HIW" ���Ŷ��ǰe�h���C\n"NOR, me, 0, 1);
		
		me->move(loc);
		
		msg(HIW"$ME��M�q�Ŷ��ǰe�h�������F�X�ӡC\n"NOR, me, 0, 1);
	}
	else
		return tell(me, "�п�J���T���a�ϦW��, �s��, �y�СC(��: gotomap center 1 10,10)\n");
}

