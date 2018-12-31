/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : info.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-01-31
 * Note   : ��T�d�߫��O�C
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <location.h>
#include <material.h>

inherit COMMAND;

string help = @HELP
        �з� info ���O�C
HELP;

private void do_command(object me, string arg)
{
	object env = environment(me);
	mapping data;
	string msg;
	array loc = query_temp("location", me) || env->query_location();

	if( !arrayp(loc) ) 
		return tell(me, "�L�k�d�ߦ��a��ơC\n");

	if( !arg || arg == "here" )
	{
		msg = pnoun(2, me)+"�{�b�Ҧb��m��������T�p�U�G\n";
	
		if( CITY_D->is_city_location(loc) )
		{
			data = CITY_D->query_coor_data(loc);
		
			if( !data["owner"] )
				msg += "�֦���  �G�L\n";
			else if( belong_to_government(data["owner"]) )
				msg += "�֦���  �G���F�� "+CITY_D->query_city_idname(loc[CITY])+"\n";
			else if( belong_to_enterprise(data["owner"]) )
				msg += "�֦���  �G���~���� "+ENTERPRISE_D->query_enterprise_color_id(data["owner"][11..])+"\n";
			else
				msg += "�֦���  �G"+(find_player(data["owner"]) ? find_player(data["owner"])->query_idname() +HIC" �u�W"NOR : capitalize(data["owner"])+HIR" ���u"NOR)+"\n";
	
			switch( data["region"] )
			{
				case AGRICULTURE_REGION:
					msg += "�g�a�W���G"HIG"�A�~��\n"NOR;
					break;
				case INDUSTRY_REGION:
					msg += "�g�a�W���G"HIY"�u�~��\n"NOR;
					break;
				case COMMERCE_REGION:
					msg += "�g�a�W���G"HIC"�ӷ~��\n"NOR;
					break;
				default:
					msg += "�g�a�W���G�L\n"NOR;
					break;
			}
				
			msg += "�g�a���ȡG$"+MONEY_D->city_to_money_unit(loc[CITY])+" "+NUMBER_D->number_symbol(data[VALUE])+"\n";
			msg += "�a�����ȡG$"+MONEY_D->city_to_money_unit(loc[CITY])+" "+NUMBER_D->number_symbol(ESTATE_D->query_estate_value(loc))+"\n";
	

		}
		else
			msg += "�@��L�ڪ����ϰϰ�...�C\n";
	}
	else
		return tell(me, "�ثe�u��d�ߤg�a��ơC\n");

	return tell(me, msg);
}
	
