/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : levy.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-04-04
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
    �����Ωx���i�ϥΦ����O�ӱj��x���g�a
levy here	- �x���ثe�ү���m���g�a�A�Y�g�a���Y�ؿv�����@�����A�N�|��
                  ����ɫؿv�����쥻���g�a�֦��̳ॢ���ؿv�����Ҧ��g�a
levy north	- �x���_�䪺�ؿv���Τg�a
levy south	- �x���n�䪺�ؿv���Τg�a
levy east	- �x�����䪺�ؿv���Τg�a
levy west	- �x���F�䪺�ؿv���Τg�a
levy -d		- ����w�x�����F���g�a

�@�Ӥ뤺���W�u������	- �u��x�����~���a��(�Ҧp�Ŧa�B�A�СB����B���[)
�W�L�@�Ӥ를�W�u������	- �i�x����Ҧ��������Цa��
HELP;

private void confirm_levy(object me, array loc, string owner, string arg)
{
	if( arg != "y" && arg != "yes" )
		return me->finish_input();

	ESTATE_D->remove_estate(loc);
	
	CITY_D->set_coor_data(loc, "owner", "GOVERNMENT/"+loc[CITY]);
	CITY_D->set_coor_icon(loc, BMAG"�D"NOR);

	ESTATE_D->set_land_estate("GOVERNMENT/"+loc[CITY], loc);
	
	if( !owner )
		CHANNEL_D->channel_broadcast("city", me->query_idname()+"�j��x���F"+CITY_D->query_city_idname(loc[CITY], loc[NUM])+" "HIG+(loc[X]+1)+","+(loc[Y]+1)+NOR" ���g�a�C\n", me);
	else
		CHANNEL_D->channel_broadcast("city", me->query_idname()+"�j��x���F "HIY+capitalize(owner)+NOR" �b"+CITY_D->query_city_idname(loc[CITY], loc[NUM])+" "HIG+(loc[X]+1)+","+(loc[Y]+1)+NOR" ���g�a�C\n", me);
	
	return msg("$ME�j��x���F�o���g�a�A�H�ѥ����ϥΡC\n", me, 0, 1);
}

private void do_command(object me, string arg)
{
	string owner;
	array loc;
	object env = environment(me);
	object ownerob;

	if( !env || !env->is_maproom() ) 
		return tell(me, pnoun(2, me)+"�L�k�x���o���g�a�C\n");
	
	loc = query_temp("location", me);

	if( arg == "north" )
		loc = arrange_city_location(loc[X], loc[Y]-1, loc[CITY], loc[NUM]);
	else if( arg == "south" )
		loc = arrange_city_location(loc[X], loc[Y]+1, loc[CITY], loc[NUM]);
	else if( arg == "west" )
		loc = arrange_city_location(loc[X]-1, loc[Y], loc[CITY], loc[NUM]);
	else if( arg == "east" )
		loc = arrange_city_location(loc[X]+1, loc[Y], loc[CITY], loc[NUM]);

	if( !CITY_D->valid_coordinate(loc[X], loc[Y], loc[CITY], loc[NUM]) )
		return tell(me, "���Ӥ�V���y�п��~�C\n");

	if( !CITY_D->is_mayor_or_officer(loc[CITY], me) )
		return tell(me, pnoun(2, me)+"���O�o�y�����������Ωx���C\n");
		
	owner = CITY_D->query_coor_data(loc, "owner");
	
	if( arg == "-d" )
	{
		if( owner != "GOVERNMENT/"+loc[CITY] )
			return tell(me, "�o���a�쥻�N���O�����Φa�C\n");
		
		if( CITY_D->query_coor_data(loc, TYPE) == ROAD )
			return tell(me, pnoun(2, me)+"��������D���C\n");
			
		if( CITY_D->query_coor_data(loc, TYPE) == BRIDGE )
			return tell(me, pnoun(2, me)+"����������١C\n");

		CITY_D->delete_coor_data(loc);

		ESTATE_D->remove_estate(loc);

		CHANNEL_D->channel_broadcast("city", me->query_idname()+"���X"+CITY_D->query_city_idname(loc[CITY], loc[NUM])+" "HIG+(loc[X]+1)+","+(loc[Y]+1)+NOR" ���F���g�a�C\n", me);
	
		return msg("$ME���X�F�o���F���g�a�A�H�ѥ����ʶR�C\n", me, 0, 1);
	}


	if( owner )
	{
		if( belong_to_government(owner) )
			return tell(me, "�o���a�w�g�O�����Φa�C\n");

		if( belong_to_enterprise(owner) )
			return tell(me, "�L�k�x�����~�g�a�C\n");

		if( !CITY_D->is_mayor(loc[CITY], me) )
			return tell(me, "�o���g�a�� "+owner+" �Ҿ֦��A�u����������x���p�H�g�a�C\n");

		if( !sizeof(ESTATE_D->query_loc_estate(loc)["roomtable"]) )
		{
			tell(me, "�O�_�T�w�L�v�j��x�� "+capitalize(owner)+" �֦����o������ "HIY+money(MONEY_D->city_to_money_unit(loc[CITY]), ESTATE_D->query_estate_value(loc))+NOR" �����g�a�H(yes/no)");
			input_to( (: confirm_levy, me, loc, owner :) );
			return;
		}
	
		if( find_player(owner) )
			return tell(me, pnoun(2, me)+"�L�k�j���x���u�W���a���Цa���C\n");
			
		ownerob = load_user(owner);

		// �@�Ӥ를�W�u�i�x���Цa��
		if( !wizardp(me) && time() - query("last_on/time", ownerob) < 30*24*60*60 )
		{
			tell(me, ownerob->query_idname()+"�@�Ӥ뤺���g�W�u�A�����\�x���Цa���C\n");
			destruct(ownerob);
			return;
		}
		
		tell(me, "�O�_�T�w�L�v�j��x��"+ownerob->query_idname()+"�o������ "HIY+money(MONEY_D->city_to_money_unit(loc[CITY]), ESTATE_D->query_estate_value(loc))+NOR" �����g�a�H(yes/no)");
		
		destruct(ownerob);
		
		input_to( (: confirm_levy, me, loc, owner :) );
			
		return;
	}
	
	if( !CITY_D->query_coor_data(loc, "region") )
		return tell(me, pnoun(2, me)+"�u��x���w�]�w�n�W���Ϥ��g�a�C\n");

	switch( CITY_D->query_coor_data(loc, TYPE) )
	{
		case MOUNTAIN:
		case FOREST:
		case RIVER:
			return tell(me, "�o���a�O�۵M�a�ΡA�L�k�x���C\n");
			break;
	}

	confirm_levy(me, loc, owner, "yes");
}
