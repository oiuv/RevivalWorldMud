/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : region.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-06
 * Note   : �x�Τg�a
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
    �����Ωx���i�ϥΦ����O�ӳW����������
    	region �A	- �N�g�a�W�����A�~��
    	region �u	- �N�g�a�W�����u�~��
    	region ��	- �N�g�a�W�����ӷ~��
    	region -d	- �����g�a�W��
HELP;

#define REGION_COST	500000

private void do_command(object me, string arg)
{
	string owner;
	array loc;
	object env = environment(me);

	if( !arg ) 
		return tell(me, "�п�J���`���W���ϦW��(�A, �u, ��)�C\n");

	if( !env || !env->is_maproom() ) 
		return tell(me, pnoun(2, me)+"�L�k�W���o���g�a�C\n");
	
	loc = query_temp("location", me);

	if( !CITY_D->city_exist(loc[CITY]) )
		return tell(me, "�o���a�ϤW�O����i��W���Ϫ��e�����C\n");

	if( !CITY_D->is_mayor_or_officer(loc[CITY], me) )
		return tell(me, pnoun(2, me)+"���O�o�y�����������Ωx���C\n");

	owner = CITY_D->query_coor_data(loc, "owner");
	
	if( owner )
	{
		if( owner == "GOVERNMENT/"+loc[CITY] )
			return tell(me, "�o���a�w�g�O�F���x�����g�a�A�L�k���s�W���C\n");
		
		return tell(me, "�o���a�w�g�O "+owner+" ���g�a�A�L�k���s�W���C\n");
	}

	switch( CITY_D->query_coor_data(loc, TYPE) )
	{
		case MOUNTAIN:
		case FOREST:
		case RIVER:
			return tell(me, "�o���a�O�۵M�a�ΡA�L�k�i��W���ϵe���C\n");
	}

	switch( arg )
	{
		case "�A":
			if( CITY_D->query_coor_data(loc, "region") == AGRICULTURE_REGION )
				return tell(me, "�o���g�a�쥻�N�O�A�~�ϡC\n");
			
			if( !CITY_D->change_assets(loc[CITY], -REGION_COST) )
				return tell(me, "��������u�ѤU $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+CITY_D->query_assets(loc[CITY])+"�A�����H���s�W���g�a�F�I\n");
		
			
			CITY_D->set_coor_data(loc, "region", AGRICULTURE_REGION);
			CITY_D->set_coor_data(loc, VALUE, "100000");
			CITY_D->set_coor_icon(loc, BGRN"�D"NOR);
			
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"�N"+CITY_D->query_city_idname(loc[CITY], loc[NUM])+" "HIG+(loc[X]+1)+","+(loc[Y]+1)+NOR" ���g�a�W����"HIG HBGRN"�A�~��"NOR"�C\n", me);
			msg("$ME��I�F���F���겣 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+REGION_COST+" �N���a�W�����A�~�ϡC\n", me, 0, 1);
			
			break;
		case "�u":
			if( CITY_D->query_coor_data(loc, "region") == INDUSTRY_REGION )
				return tell(me, "�o���g�a�쥻�N�O�u�~�ϡC\n");
			
			if( !CITY_D->change_assets(loc[CITY], -REGION_COST) )
				return tell(me, "��������u�ѤU $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+CITY_D->query_assets(loc[CITY])+"�A�����H���s�W���g�a�F�I\n");
		
			CITY_D->set_coor_data(loc, "region", INDUSTRY_REGION);
			CITY_D->set_coor_data(loc, VALUE, "200000");
			CITY_D->set_coor_icon(loc, BYEL"�D"NOR);
			
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"�N"+CITY_D->query_city_idname(loc[CITY], loc[NUM])+" "HIG+(loc[X]+1)+","+(loc[Y]+1)+NOR" ���g�a�W����"HIY HBYEL"�u�~��"NOR"�C\n", me);
			msg("$ME��I�F���F���겣 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+REGION_COST+" �N���a�W�����u�~�ϡC\n", me, 0, 1);
			break;
			
		case "��":
			if( CITY_D->query_coor_data(loc, "region") == COMMERCE_REGION )
				return tell(me, "�o���g�a�쥻�N�O�ӷ~�ϡC\n");

			if( !CITY_D->change_assets(loc[CITY], -REGION_COST) )
				return tell(me, "��������u�ѤU $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+CITY_D->query_assets(loc[CITY])+"�A�����H���s�W���g�a�F�I\n");
		
			CITY_D->set_coor_data(loc, "region", COMMERCE_REGION);
			CITY_D->set_coor_data(loc, VALUE, "300000");
			CITY_D->set_coor_icon(loc, BCYN"�D"NOR);
			
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"�N"+CITY_D->query_city_idname(loc[CITY], loc[NUM])+" "HIG+(loc[X]+1)+","+(loc[Y]+1)+NOR" ���g�a�W����"HIC HBCYN"�ӷ~��"NOR"�C\n", me);
			msg("$ME��I�F���F���겣 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+REGION_COST+" �N���a�W�����ӷ~�ϡC\n", me, 0, 1);
			break;
		
		case "-d":
			if( !CITY_D->query_coor_data(loc, "region") )
				return tell(me, "�o���a�쥻�N�S���W���C\n");
			
			CITY_D->delete_coor_data(loc, "region");
			CITY_D->delete_coor_data(loc, 6);
			CITY_D->delete_coor_icon(loc);
			
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"�����F"+CITY_D->query_city_idname(loc[CITY], loc[NUM])+" "HIG+(loc[X]+1)+","+(loc[Y]+1)+NOR" �즳���g�a�W���C\n", me);
			msg("$ME�����F���a�즳���g�a�W���C\n", me, 0, 1);
			break;
			
		default:
			return tell(me, "�п�J���`���W���ϦW��(�A, �u, ��)�C\n");
	}
}
