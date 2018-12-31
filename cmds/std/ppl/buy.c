/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : buy.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : buy ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <location.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�ʶR���O
    buy here			- �ʶR�����g�a
    buy here for enterprise	- �����~�ʶR�����g�a
    
HELP;

private void do_command(object me, string arg)
{
	if( !arg ) return tell(me, pnoun(2, me)+"�Q�R����H\n");

	if( arg == "here" || arg == "here for enterprise" )
	{
		string money_unit, price, owner;
		array loc;
		object env = environment(me);
		mapping data;
		
		if( !env || !env->is_maproom() ) 
			return tell(me, pnoun(2, me)+"�L�k�R�U�o�g�a�C\n");
	
		loc = query_temp("location", me);
		
		if( load_object(MAP_D->query_map_system(loc)) != load_object(CITY_D) )
			return tell(me, pnoun(2, me)+"�L�k�b����~�ʶR�g�a�C\n");

		if( !CITY_D->query_coor_data(loc, "region") )
			return tell(me, pnoun(2, me)+"�u���ʶR�w�g�g�L���F���W�����g�a�C\n");
		
		data = CITY_D->query_coor_data(loc);

		if( query("city", me) != query("city", env) )
			return tell(me, pnoun(2, me)+"���O"+CITY_D->query_city_idname(query("city", env))+"�������A�L�k�ʶR�������g�a�C\n");

		if( wizardp(me) && loc[CITY] != "wizard" )
			return tell(me, "�Ův�ȯ���b"+CITY_D->query_city_idname("wizard")+"�ʶR�g�a�P�سy�ؿv���C\n");

		if( nullp(owner = data["owner"]) )
		{
			if( nullp(data[TYPE]) )
			{
				money_unit = MONEY_D->city_to_money_unit(loc[CITY]);
				price = data[VALUE];
	
				if( arg == "here" )
				{
					if( !me->spend_money(money_unit, price) )
						return tell(me, "�o���a���� $"+money_unit+" "+NUMBER_D->number_symbol(price)+" ���A"+pnoun(2, me)+"���W�� $"+money_unit+" �{�������F�C\n");
					
					CITY_D->change_assets(query("city", env), price);
					
					if( CITY_D->set_coor_data(loc, "owner", me->query_id(1)) )
						tell(me, pnoun(2, me)+"��F "+money(money_unit, price)+" ���R�U�F�o���g�a�C\n");
					
					CITY_D->set_coor_icon(loc, BWHT"��"NOR);
					
					// �]�w�Цa����T
					ESTATE_D->set_land_estate(me->query_id(1), loc);
				}
				else if( arg == "here for enterprise" )
				{
					string enterprise = query("enterprise", me);
					
					money_unit = MONEY_D->query_default_money_unit();
	
					if( !enterprise )
						return tell(me, pnoun(2, me)+"�èS���[�J������~���ΡC\n");
						
					if( !ENTERPRISE_D->enterprise_exists(enterprise) )
						return tell(me, pnoun(2, me)+"�ҥ[�J�����~���Τw�g���s�b�C\n");
						
					if( ENTERPRISE_D->change_assets(enterprise, "-"+price) )
						CHANNEL_D->channel_broadcast("ent", me->query_idname()+"��O���~����b"+loc_short(loc)+"�ʶR�F�@�����~�g�a�C\n", me);
					else if( me->spend_money(money_unit, price) )
					{
						ENTERPRISE_D->change_invest(enterprise, me->query_id(1), price);
						CHANNEL_D->channel_broadcast("ent", me->query_idname()+"��O�ӤH����b"+loc_short(loc)+"�ʶR�F�@�����~�g�a�C\n", me);
						tell(me, "���~���Ϊ��{����������H�ʶR�g�a�F�A�]����O"+pnoun(2, me)+"���p�H��� "+money(money_unit, price)+" �ʶR�g�a�C\n");
					}
					else
						return tell(me, "���~����P"+pnoun(2, me)+"�� $"+money_unit+" �{���������H�ʶR�o���g�a�F�C\n");
					
					CITY_D->set_coor_data(loc, "owner", "ENTERPRISE/"+enterprise);
					CITY_D->set_coor_icon(loc, BWHT"��"NOR);
					
					// �]�w�Цa����T
					ESTATE_D->set_land_estate("ENTERPRISE/"+enterprise, loc);
				}			
				return;
			}
			else
				return tell(me, pnoun(2, me)+"�L�k�ʶR�o���a�C\n");
		}
		else if( owner == me->query_id(1) )
			return tell(me, "�o���a�w�g�O"+pnoun(2, me)+"���F�C\n");
		else if( belong_to_government(owner) )
			return tell(me, "�o���a�O���F���Φa�A�L�k�ʶR�C\n");
		else if( belong_to_enterprise(owner) )
			return tell(me, "�o���a�w�Q���~����"+ENTERPRISE_D->query_enterprise_color_id(owner[11..])+"���ʡA�L�k�ʶR�C\n");
		else
			return tell(me, "�o���a�w�g�Q "+capitalize(owner)+" ���ʶR�C\n");
	}
	else
		return tell(me, pnoun(2, me)+"�L�k�b�o���ʶR���󪫫~�C\n");
}
