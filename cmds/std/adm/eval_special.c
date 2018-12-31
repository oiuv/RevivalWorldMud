/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : eval_special.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-21
 * Note   : eval_special ���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <object.h>
#include <map.h>

inherit COMMAND;

string help = @HELP
        �з� eval_special ���O�C
HELP;

private void eval_special()
{

		
	
	/*
	int x, y;
	array loc;
	string owner;
	string city;
	int num;
	
	foreach(city in CITY_D->query_cities())
	{
		for(num=0;num<CITY_D->query_city_num(city);num++)
		for(x=0;x<100;x++)
		for(y=0;y<100;y++)
		{
			loc = arrange_city_location(x, y, city, num);
			
			owner = CITY_D->query_coor_data(loc, "owner");
			
			if( !owner || belong_to_government(owner) ) continue;
			
			reset_eval_cost();

			if( !ESTATE_D->whose_estate(loc) )
			{
				
				CHANNEL_D->channel_broadcast("sys", save_variable(loc)+" error("+owner+").");
				ESTATE_D->remove_unknown_estate(loc);
			}
		}	
	}*/
}

private void startup_eval_special()
{
	int costtime;
	
	costtime = time_expression { eval_special(); };

        CHANNEL_D->channel_broadcast("news", sprintf("�t�ίS���s�����A�@��O %.3f ��C", costtime/1000000.));
}
	
private void command(object me, string arg)
{
        if( !is_command() ) return;

        CHANNEL_D->channel_broadcast("news", "��������t�ζi��S���s�A��s�L�{�i��ӶO�Ƭ����ܼƤ����A�еy�ԡC");

	call_out((: startup_eval_special :), 5);
}
