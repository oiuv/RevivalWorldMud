/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : go.c
 * Author : Cookys@RevivalWorld
 * Date   : 2001-7-20
 * Note   : ���ʫ��O�C
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <lock.h>
#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <location.h>
#include <message.h>
#include <delay.h>
#include <buff.h>
#include <citydata.h>

inherit COMMAND;

string help = @HELP
���ʫ��O

�o�ӫ��O�i�H���z�b���ͪ��@�ɤ��ۥѪ��樫�A��z�Q�h���a��
�q�`�i�H�樫����V�p�U:

north		�_	northeast	�F�_
south		�n	northwest	��_
east		�F	southeast	�F�n
west		��	southwest	��n
up		�W	down		�U

�H�W�o�Ǥ�V�����O�b�t�Τ������w�]���O�W (�аѦ� alias -d)
���F��K�ϥΡA�z�u�ݭn��J��V�Y�i�A�p: n, s, e, w, ne, nw, se, sw, u, d
�b�Y�ǰϰ�i��|�X�{�ҥ~����V�A�z�u�n��J�Ӥ�V�����W�Y�i�C

�b�ؿv�����h�i�Q�� go '�Ӽh��' ���F�ӼӼh

�Ҧp : go 100			- ���F 100F

�������O: look
HELP;

mapping corresponding =
([
    "north"         :"south",
    "south"         :"north",
    "east"          :"west",
    "west"          :"east",
    "northwest"     :"southeast",
    "northeast"     :"southwest",
    "southwest"     :"northeast",
    "southeast"     :"northwest",
    "up"            :"down",
    "down"          :"up",
]);

private void do_command(object me, string arg)
{
	mixed exit;
	mapping exits;
	string my_idname, from;
	object env, newenv;
	int moving_stamina_cost;

	if( !arg )
		return tell(me, "�A�Q�n�����̨��H\n");
	
	// �O�ǤH
	if( query("prisoner", me) )
	{
		msg("$ME����}�Q�K��R��A�L�k�ۥѦ�ʡC\n", me, 0, 1);
		return;
	}
		
	//���L������U���O
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	env = environment(me);

	if( !objectp(env) ) 
		return tell(me, pnoun(2, me)+"�ثe�L�k�V�����m���ʡA�гq���Ův�B�z�C\n");

	my_idname = me->query_idname();

	if( env->is_maproom() )
	{
		switch(MAP_D->valid_move(me, arg))
		{
			case 1:
			{
				moving_stamina_cost = -me->query_all_buff(BUFF_MOVING_STAMINA);
				
				if( moving_stamina_cost > 0 )
				{
					if( !me->cost_stamina(moving_stamina_cost) )
						return tell(me, pnoun(2, me)+"�w�g�S����O�~��e�i�F�C\n");
					else
						msg("$ME�ӶO�F "+moving_stamina_cost+" �I����O�ĤO���e��i�C\n", me, 0, 1);
				}
				
				if( me->query_loading() > me->query_max_loading() )
				{
					msg("$ME��ۨI��������e�i�A���רs�O�O���q�ߡC\n", me, 0, 1);
					me->faint();
					return;
				}
				MAP_D->move(me, arg);
				
				return;
				break;
			}
			case -1:
			{
				if( wizardp(me))
					return MAP_D->move(me, arg);
				else
					return tell(me, "��"+CHINESE_D->to_chinese(arg)+"��V�����Q���F�A"+pnoun(2, me)+"�S��k�������C\n");
					
				break;
			}
			case -2:
			{
				if( query_temp("go_resist", me) == "WALL_"+arg )
					msg("$ME�������D"+CHINESE_D->to_chinese(arg)+"��O��A�o�̵M�q�������F�U�h�A��L�B�����򭻸z�@�ˡC\n", me, 0, 1);
				else
				{
					set_temp("go_resist", "WALL_"+arg, me);
					tell(me, "��"+CHINESE_D->to_chinese(arg)+"��V�O�@������A"+pnoun(2, me)+"�S��k�������C\n");
				}
				return;
				break;
			}
			default:
			{
				return tell(me, "�� "+arg+" ����V�S���X���C\n");
				break;
			}
		}
	}

	// �@�벾��
	if( !mapp(exits = query("exits", env)) || !sizeof(exits) )
		return tell(me, "�o�̥|�P���S���X�f�A"+pnoun(2, me)+"�Q������̥h�H\n");

	if( exit = exits[arg] )
	{
		if( me->is_fighting() )
		{
			if( !COMBAT_D->do_flee(me) )
				return;
		}

		from = CHINESE_D->to_chinese(corresponding[arg]);

		if( functionp(exit) )
			exit = evaluate(exit, me);

		// �Y�O�y�ХX�f
		if( arrayp(exit) )
		{
			if( (MAP_D->query_map_system(exit))->query_coor_data(exit, "lock") & LOCKED && !wizardp(me))
				return tell(me, "�� "+capitalize(arg)+" ��V�����Q���F�A"+pnoun(2, me)+"�S��k�������C\n");

			if( !wizardp(me) && (MAP_D->query_map_system(exit))->query_coor_data(exit, "lock") & WIZLOCKED )
				return tell(me, "�� "+capitalize(arg)+" ��V���@�D�ѤѦӭ�������צ�A"+pnoun(2, me)+"�S��k�������C\n");

			broadcast(env, my_idname+"��"+(CHINESE_D->to_chinese(arg)||" "+capitalize(arg)+NOR" ��V")+"�����f���F�X�h�C\n", 0, me);

			me->move(exit);

			if( from )
				broadcast(exit, my_idname+"�q"+from+"�����̨��F�X�ӡC\n", 0, me);
			else
				broadcast(exit, my_idname+"���F�L�ӡC\n", 0, me);

			me->follower_move(env, exit);
		}
		// �Y�O����W�٥X�f
		else if( stringp(exit) )
		{
			if( query("lock/"+arg, env) & LOCKED && !wizardp(me))                       
				return tell(me, "�� "+capitalize(arg)+" ��V�����Q���F�A"+pnoun(2, me)+"�S��k�������C\n");

			if( !wizardp(me) && query("lock/"+arg, env) & WIZLOCKED )
				return tell(me, "�� "+capitalize(arg)+" ��V���@�D�ѤѦӭ�������צ�A"+pnoun(2, me)+"�S��k�������C\n");

			broadcast(env, my_idname+"��"+(CHINESE_D->to_chinese(arg)||" "+capitalize(arg)+NOR" ��V")+"���}�C\n", 0, me);

			me->move(exit);

			if( from )
				broadcast(exit, my_idname+"�q"+from+"���F�L�ӡC\n", 0, me);
			else
				broadcast(exit, my_idname+"���F�L�ӡC\n", 0, me);

			me->follower_move(env, exit);
		}
		else
			tell(me, "�o�Ӥ�V���X�f�Q�����F�A�L�k�q�L�C\n");
	}
	else if( to_int(arg) > 0 && big_number_check(arg) )
	{
		int current_floor = env->query_floor()-1;
		int floor = to_int(arg)-1;
		string msg = "";
		array loc = env->query_location();

		if( current_floor == floor )
			return tell(me, pnoun(2, me)+"�w�g�O�b "+(floor+1)+" �ӤF�C\n");

		if( !arrayp(loc) ) return tell(me, "�o�̨S�� "+arg+" �ӡC\n");

		// �ֳt�W��
		if( current_floor < floor )
		{
			newenv = environment(me);

			for(current_floor = env->query_floor();current_floor<=floor;current_floor++)
			{
				exit = CITY_NUM_ROOM(loc[CITY], loc[NUM]) + "/"+loc[X]+"_"+loc[Y]+"_"+(current_floor?current_floor+"_":"")+env->query_building_type()+".o";

				if( !file_exists(exit) )
					return tell(me, "�o�̨S�� "+arg+" �ӡC\n");

				if( query("lock/up", newenv) & LOCKED )
					return tell(me, "���� "+(current_floor+1)+" �Ӫ��q�D�Q���F�C\n");

				newenv = load_object(exit[0..<3]);
			}
			
			//for(current_floor = env->query_floor();current_floor<=floor;current_floor++)		
			msg += pnoun(2, me)+"�f���q�詹�� "WHT+(floor+1)+NOR" �ӤW�h�C\n";

			tell(me, msg);
			me->move(newenv);
			me->follower_move(env, newenv);
		}
		// �ֳt�U��
		else
		{
			newenv = environment(me);

			for(current_floor = env->query_floor()-2;current_floor>=floor;current_floor--)
			{	
				exit = CITY_NUM_ROOM(loc[CITY], loc[NUM]) + "/"+loc[X]+"_"+loc[Y]+"_"+(current_floor?current_floor+"_":"")+env->query_building_type()+".o";
					
				if( !file_exists(exit) )
					return tell(me, "�o�̨S�� "+arg+" �ӡC\n");

				if( query("lock/down", newenv) & LOCKED )
					return tell(me, "���� "+(current_floor+1)+" �Ӫ��q�D�Q���F�C\n");
					
				newenv = load_object(exit[0..<3]);
			}

			//for(current_floor = env->query_floor()-2;current_floor>=floor;current_floor--)
				msg += pnoun(2, me)+"�f���q�詹�� "WHT+(floor+1)+NOR" �ӤU�h�C\n";
			
			tell(me, msg);
			me->move(newenv);
			me->follower_move(env, newenv);
		}

		msg("$ME�q�q��̨��F�X�ӡC\n", me, 0, 1);
	}
	else
		tell(me, "�� "+arg+" ����V�S���X���C\n");
}

