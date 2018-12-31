/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sit.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-20
 * Note   : �������O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <delay.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
    �A�i�H�ϥγo�ӫ��O�b�����O�B�j��B�D�F���a��𮧡A��O����_�|��
���q����]�������ɭ��٭n�֤@�I�C�Ӧb�o�T�Ӧa��𮧪��ĪG�]�������P�A�o�I�N
�d�ݦU�����F�C�p�G���b�A����S���o�Ǵ��[���ܡA�A�]�i�H�Ҽ{�R���b���񪺦a
�سy(build)�@�ӡC

�t�~ sit �i�H�ѥ𮧳]�I���֦��̨ӳ]�w���\�ϥΦ��]�I�����a�W��
     sit -l          �d�ݥثe���\�ϥΦW��
     sit -onlyme     �u���\�֦��̦ۤv�ϥ�
     sit -open       �}��Ҧ��H�ϥ�
     sit 'ID'        �]�w�}�񵹬Y�ǯS�w���a�ϥ�(�A��J�@������)

HELP;

#define REGEN	60

private void do_command(object me, string arg)
{
	int cost, time, regen, type;
	array loc = query_temp("location", me);
	string owner, myid;
	string *allow;
	string map_system;

	if( !arrayp(loc) || (!CITY_D->is_city_location(loc) && !AREA_D->is_area_location(loc)) )
		return tell(me, pnoun(2, me)+"�L�k�b�o�Ӧa��𮧡C\n");

	map_system = MAP_D->query_map_system(loc);

	myid = me->query_id(1);
	owner = map_system->query_coor_data(loc, "owner");
	allow = map_system->query_coor_data(loc, "sit_allow");		
	type = map_system->query_coor_data(loc, TYPE);
	
	if( type != PAVILION && type != TREE && type != GRASS )
		return tell(me, "�o�Ӧa��S��k���U�𮧡C\n");
	
	if( map_system == CITY_D )
	{
		if( arg  )
		{
			if( arg == "-l" )
			{
				if( sizeof(allow) )
					tell(me, "���𮧳]�I�ثe�Q���\�ϥΪ����a�� "+implode(allow, " ")+"�C\n");
				else if( arrayp(allow) )
					tell(me, "���𮧳]�I�ثe�u���\�֦��̦ۤv�ϥΡC\n");
				else
					tell(me, "���𮧳]�I�ثe�}��Ҧ��H�ϥΡC\n");
				
				return;
			}
	
			if( owner != me->query_id(1) )
				return tell(me, pnoun(2, me)+"���O���a���֦��H�A�����o�ӥ𮧦a�I������]�w�C\n");
				
			if( arg == "-onlyme" )
			{
				CITY_D->set_coor_data(loc, "sit_allow", ({ }));
				return tell(me, pnoun(2, me)+"�N���a�]�w���u��"+pnoun(2, me)+"�ۤv�i�H�ϥΡC\n");
			}
			else if( arg == "-open" )
			{
				CITY_D->delete_coor_data(loc, "sit_allow");
				return tell(me, pnoun(2, me)+"�N���a�]�w���}��Ҧ��H�ϥΡC\n");
			}
			else
			{
				allow = allow || allocate(0);
				arg = remove_ansi(lower_case(arg));
				
				if( strlen(arg) > 12 )
					return tell(me, "�п�J���T�����a ID�C\n");
	
				if( member_array(arg, allow) != -1 )
				{
					allow -= ({ arg });
					tell(me, pnoun(2, me)+"���� "+capitalize(arg)+" �ϥΦ��]�I�����C\n");
				}
				else
				{
					if( sizeof(allow) > 20 )
						return tell(me, "���\�W��L�k�W�L 20 �H�C\n");
	
					allow |= ({ arg });
					tell(me, pnoun(2, me)+"���\ "+capitalize(arg)+" �ϥΦ��]�I�C\n");
				}
				
				CITY_D->set_coor_data(loc, "sit_allow", allow);
				return;
			}	
		}
		
		if( owner != myid && arrayp(allow) && member_array(myid, allow) == -1 )
			return tell(me, "�o�O "+capitalize(owner)+" �p�H�֦����]�I�A"+pnoun(2, me)+"�ثe���Q���\�ϥΡC\n");
	}

	if( me->is_delaying() )
		return tell(me, me->query_delay_msg());

	switch( type )
	{
		case PAVILION:
			cost = me->query_stamina_max() - me->query_stamina_cur();
			msg("$ME�ʵ�"+(map_system->query_coor_data(loc, "short")||"�D�F")+"�����۬W�ǡA���b�۴ȤW�ǲD��(��_ "+cost+" ��O)�C\n", me, 0, 1);
			regen = me->stamina_regen()+REGEN;
			time = 2*cost/regen + 1;
			
			if( time <= 0 )
				tell(me, "�𮧧����C\n");
			else
			{
				set_temp("rest_regen/stamina", REGEN, me);
				me->start_delay(REST_DELAY_KEY, time, pnoun(2, me)+"���b"+(map_system->query_coor_data(loc, "short")||"�D�F")+"���𮧡C\n", "�𮧧����C\n", bind((: delete_temp("rest_regen", $(me)), $(me)->set_stamina_full() :), me));
			}
			break;
			
		case TREE:
			cost = me->query_health_max() - me->query_health_cur();
			msg("$ME�ʵ�"+(map_system->query_coor_data(loc, "short")||"��F")+"�A�b�@�K���𽮤U�ɨ��۵M����(��_ "+cost+" �ͩR)�C\n", me, 0, 1);
			
			regen = me->health_regen()+REGEN;
			time = 2*cost/regen + 1;
			
			if( time <= 0 )
				tell(me, "�𮧧����C\n");
			else
			{
				set_temp("rest_regen/health", REGEN, me);
				me->start_delay(REST_DELAY_KEY, time, pnoun(2, me)+"�����b��U�𮧡C\n", "�𮧧����C\n", bind((: delete_temp("rest_regen", $(me)), $(me)->set_health_full() :), me));
			}
			break;
			
		case GRASS:
			cost = me->query_energy_max() - me->query_energy_cur();
			msg("$ME�׽��b"+(map_system->query_coor_data(loc, "short")||"���")+"�W�A���L�������ث�(��_ "+cost+" �믫)�C\n", me, 0, 1);
			
			regen = me->energy_regen()+REGEN;
			time = 2*cost/regen + 1;
			
			if( time <= 0 )
				tell(me, "�𮧧����C\n");
			else
			{
				set_temp("rest_regen/energy", REGEN, me);
				me->start_delay(REST_DELAY_KEY, time, pnoun(2, me)+"�����b"+(map_system->query_coor_data(loc, "short")||"���")+"�W�𮧡C\n", "�𮧧����C\n", bind((: delete_temp("rest_regen", $(me)), $(me)->set_energy_full() :), me));
			}
			break;
		default:
			error("sit error.");
	}
}
