/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : transfer.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-03-03
 * Note   : transfer ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <daemon.h>
#include <location.h>
#include <map.h>
#include <roommodule.h>

inherit COMMAND;

string help = @HELP
    �Цa�����v�ಾ���O�A���F���u�ಾ�ɩҦ��ж��]�w�P���e���N�@��
�ಾ�A���[�B�A�е��D�ؿv���h�L�k�ಾ�C

transfer here to 'ID'	- �N���a�Цa���ಾ���t�@�Ӫ��a
HELP;


private void do_command(object me, string arg)
{
	string targetid;
	object targetob;
	array loc = query_temp("location", me);
	mapping estdata;
	array building_info;
	array loctemp;
	int target_architectonic_level;

	if( !arrayp(loc) || !environment(me)->is_maproom() )
		return tell(me, pnoun(2, me)+"�������b�ؿv���f�i��Цa������C\n");
		
	if( ESTATE_D->whose_estate(loc) != me->query_id(1) )
		return tell(me, "�o�ɫؿv���ä��O�ݩ�"+pnoun(2, me)+"���C\n");

	estdata = ESTATE_D->query_loc_estate(loc);

	if( CITY_D->query_coor_data(loc, TYPE) != DOOR )
		return tell(me, pnoun(2, me)+"�u���ಾ�ؿv�����Цa���C\n");
	
	if( !arg || !sscanf(arg, "here to %s", targetid) )
		return tell(me, help);
	
	if( !objectp(targetob = find_player(targetid)) )
		return tell(me, "�ثe�u�W�S�� "+targetid+" �o�쪱�a�C\n");
	
	if( me == targetob )
		return tell(me, "�L�k�ಾ�Цa�����ۤv�C\n");

	if( query("city", targetob) != query("city", me) )
		return tell(me, "�u��N�Цa���ಾ���P�@�y�����������C\n");

	if( !same_environment(me, targetob) )
		return tell(me, targetob->query_idname()+"�����Ө�o�̤~��i��Цa���ಾ�C\n");

	target_architectonic_level = targetob->query_skill_level("architectonic") + targetob->query_skill_level("architectonic-adv");
	
	foreach(string sloc in estdata["roomtable"])
	{
		loctemp = restore_variable(sloc);	
		
		if( target_architectonic_level < sizeof(city_roomfiles(loctemp))-1 )
			return tell(me, targetob->query_idname()+"���ؿv�޳N�ޯ൥�Ť����H�Ө��o�˪��ؿv���C\n");
	}
	
	building_info = BUILDING_D->query_building(estdata["type"]);
	
	if( building_info[ROOMMODULE_MAXLIMIT] > 0 && ESTATE_D->query_owner_amount(targetid, estdata["type"]) >= building_info[ROOMMODULE_MAXLIMIT] )
		return tell(me, targetob->query_idname()+"�����ثؿv���ƶq�w�g��F�W���C\n");

		
	if( !ESTATE_D->transfer_estate(me->query_id(1), targetob->query_id(1), loc) )
		return tell(me, "�Цa���ಾ�o�Ϳ��~�A�гq���Ův�B�z�C\n");
	
	CHANNEL_D->channel_broadcast("city", me->query_idname()+"�N���"+CITY_D->query_city_idname(loc[CITY], loc[NUM])+"("+(loc[X]+1)+","+(loc[Y]+1)+")���Цa���ಾ��"+targetob->query_idname()+"�C", me);
	msg("$ME�����N���ؿv�����Цa���ಾ��$YOU�C\n", me, targetob, 1);
}
