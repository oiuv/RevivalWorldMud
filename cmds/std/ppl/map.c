/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : map.c
 * Author : Cookys@RevivalWorld
 * Date   : 2002-3-18
 * Note   : ��ܦa�Ϸ��p���O�C
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <location.h>

inherit COMMAND;

string help = @HELP
    ��ܧY�ɦa��
    
map				- ��ܷ�a���a�ϡA��Ҥج� 1:2
map '����' '�s��' -'���'	- ��ܬY�����a�ϡA�Ҧp map wizard 1 -2(��ҽd�� 1 - 5)
HELP;

private void do_command(object me, string arg)
{
	array loc;
	string city;
	int num, ratio;
	string map;

	if( !arg )
	{
		if( !arrayp(loc = query_temp("location", me)) )
			return tell(me, pnoun(2, me)+"�����b�a�ϤW�ϥγo�ӥ\��C\n");
		
		city = loc[CITY];
		num = loc[NUM];
		ratio = 2;
	}

	else if( sscanf(arg, "%s %d -%d", city, num, ratio) != 3 || ( !CITY_D->city_exist(city, --num) && !AREA_D->area_exist(city, num)) )
		return tell(me, "�п�J���T�������W�ٻP���|�Ͻs���C(��: map center 1 -2)\n");

	if( ratio > 10 ) ratio = 10;
	if( ratio < 1 ) ratio = 1;
		
	loc = arrange_city_location(1, 1, city, num);
	
	map = MAP_D->query_map_reduction(MAP_D->query_map(loc, 1), ratio);
	
	if( !map )
		tell(me, "�L�k���ͦ��a�ϡC\n");
	else
		tell(me, (CITY_D->query_city_idname(city, num)||AREA_D->query_area_idname(city, num))+"�Y�p "+ratio+" ��²�ϡG\n"+map+"\n\n");
}