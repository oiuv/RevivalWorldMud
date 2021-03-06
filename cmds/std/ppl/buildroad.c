/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : buildroad.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-05
 * Note   : 建造道路
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <location.h>

inherit COMMAND;

string help = @HELP
    造路指令，造路費用每段為 500,000
buildroad		- 在此地建造道路
buildroad -d		- 拆除此地的道路
HELP;

#define BUILDROAD_COST	500000

private void do_command(object me, string arg)
{
	string owner;
	array loc;
	object env = environment(me);
	
	if( !env->is_maproom() )
		return tell(me, "只有在地圖中才能夠建造道路。\n", CMDMSG);

	loc = query_temp("location", me);

	if( !CITY_D->is_mayor_or_officer(loc[CITY], me) )
		return tell(me, pnoun(2, me)+"不是這座城市的市長或官員。\n", CMDMSG);

	owner = (MAP_D->query_map_system(loc))->query_coor_data(loc, "owner");

	if( owner != "GOVERNMENT/"+loc[CITY] )
		return tell(me, "這塊土地並不是政府土地，無法用來建設道路。\n", CMDMSG);

	switch( (MAP_D->query_map_system(loc))->query_coor_data(loc, TYPE) )
	{
		case 0:
		case ROAD:
			break;
		default:
			return tell(me, "這裡已經有其他地形或是建築物，無法在此建造道路。\n", CMDMSG);
	}

	if( arg )
	{
		if( arg == "-d" )
		{
			(MAP_D->query_map_system(loc))->delete_coor_data(loc);
			(MAP_D->query_map_system(loc))->delete_coor_icon(loc);
			
			return tell(me, pnoun(2, me)+"拆除掉了此處的道路。\n", CMDMSG);
		}
		
		if( noansi_strlen(arg) > 14 )
			return tell(me, "路名最長不能超過七個中文字。\n", CMDMSG);

		arg = kill_repeat_ansi(arg+NOR);
		
		if( (MAP_D->query_map_system(loc))->query_coor_data(loc, TYPE) == ROAD && CITY_D->query_coor_data(loc, "roadname") == arg )
			return tell(me, "這塊土地上已經建造了道路「"+arg+"」。\n", CMDMSG);
		
		if( CITY_D->city_exist(loc[CITY]) )
		{
			CITY_D->change_assets(loc[CITY], -BUILDROAD_COST);
		
			msg("$ME花費了市府資產 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+NUMBER_D->number_symbol(BUILDROAD_COST)+"，在此開闢了一條新道路，名叫「"+arg+"」。\n", me, 0, 1);
		}
		else
			msg("$ME在此開闢了一條新道路，名叫「"+arg+"」。\n", me, 0, 1);
			
		(MAP_D->query_map_system(loc))->set_coor_data(loc, "roadname", arg);
	}
	else
	{
		if( (MAP_D->query_map_system(loc))->query_coor_data(loc, TYPE) == ROAD && !(MAP_D->query_map_system(loc))->query_coor_data(loc, "roadname") )
			return tell(me, "這塊土地上已經建造了道路。\n", CMDMSG);
		
		if( CITY_D->city_exist(loc[CITY]) )
		{
			CITY_D->change_assets(loc[CITY], -BUILDROAD_COST);
		
			msg("$ME花費了市府資產 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+NUMBER_D->number_symbol(BUILDROAD_COST)+"，在此開闢了一條新道路。\n", me, 0, 1);
		}
		else
			msg("$ME在此開闢了一條新道路。\n", me, 0, 1);
	}

	(MAP_D->query_map_system(loc))->set_coor_data(loc, TYPE, ROAD); 
	(MAP_D->query_map_system(loc))->set_coor_icon(loc, "　");
	
}
