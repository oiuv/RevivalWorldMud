/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : decorate.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-11
 * Note   : �ؿv���������C���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <location.h>
#include <message.h>
#include <citydata.h>

inherit COMMAND;

string help = @HELP
decorate [short [-d] | long [-d] | sign �r | function [�\��] | master [-d]]

decorate short			�]�w�u�W��(�ж��P�a�ϤW�ҥi�]�w)
decorate short -d		�R���u�W��(�ȾA�Ω�a�ϤW)
decorate long			�]�w���ԭz(�ж��P�a�ϤW�ҥi�]�w)
decorate long -d		�R�����ԭz(�ȾA�Ω�a�ϤW)
decorate sign '�r'		���ؿv�������ж��A�i�]�w�γ��r��
decorate function '�\��'	���ؿv�������ж��A�i�]�w�ж��\�૬�A
decorate function		�C�X�i�]�w���ж����A
decorate function -d		�����ثe���ж����\�૬�A
decorate master �D�����y��  	�i��ж����]�w�۹������D�n�s�곡���Ҧb�����P�y��(�� 2 50,50)
decorate master -d		�R���ж������D�n�s�곡���]�w

HELP;

#define MAX_LONG_WIDTH		72
#define MAX_LONG_LINE		20
#define MAX_LONG_LINE_MAP	3
#define MAX_SHORT_WIDTH		24
#define MAX_SHORT_WIDTH_MAP	18

private varargs void input_room_short(object me, object env, string arg, int no_encode)
{
	array loc = query_temp("location", me);

	if( !arg || !arg[0] )
	{
		tell(me, "�����ק�C\n");
		return me->finish_input();
	}
	
	if( !no_encode && query("encode/gb", me) ) arg = G2B(arg);

	arg = ansi(arg);
	arg = remove_fringe_blanks(arg);
	arg = kill_repeat_ansi(arg+NOR);

	if( (env->is_maproom() && noansi_strlen(arg) > MAX_SHORT_WIDTH_MAP) || noansi_strlen(arg) > MAX_SHORT_WIDTH )
	{
		tell(me, "�u�W�ٳ̦h�u�� "+(env->is_maproom()?MAX_SHORT_WIDTH_MAP:MAX_SHORT_WIDTH)+" �Ӧr���C\n");
		return me->finish_input();
	}

	if( env->is_maproom() )
	{
		(MAP_D->query_map_system(loc))->set_coor_data(loc, "short", arg);
		msg("$ME�ק惡�a���u�W�٬��u"+arg+"�v�C\n", me, 0, 1);
		return;
	}
	else
	{
		set("short", arg, env);
		msg("$ME�ק惡�a���u�W�٬��u"+arg+"�v�C\n", me, 0, 1);

		if( !env->save() )
			error("decorate save room error.");
	}	
	me->finish_input();
}

private void input_room_long(object me, object env, string arg)
{
	array loc = query_temp("location", me);

	if( !arg || !arg[0] )
	{
		tell(me, "�����ק�C\n");
		return me->finish_input();
	}

	arg = multiwrap(arg, MAX_LONG_WIDTH);

	arg = ansi(arg);
	arg = kill_repeat_ansi(arg+NOR);

	//[&,->�A][&'->�B][&.->�C][&!->�I][&?->�H]
	arg = replace_string(arg, "&,", "�A");
	arg = replace_string(arg, "&'", "�B");
	arg = replace_string(arg, "&.", "�C");
	arg = replace_string(arg, "&!", "�I");
	arg = replace_string(arg, "&?", "�H");

	if( (env->is_maproom() && sizeof(explode(arg, "\n")) > MAX_LONG_LINE_MAP) || sizeof(explode(arg, "\n")) > MAX_LONG_LINE )
	{
		tell(me, "�ж����ԭz�̦h�u�঳ "+(env->is_maproom()?MAX_LONG_LINE_MAP:MAX_LONG_LINE)+" ��C\n");
		return me->finish_input();
	}

	if( env->is_maproom() )
	{
		(MAP_D->query_map_system(loc))->set_coor_data(loc, "long", arg+"\n");
		msg("$ME�ק惡�a�����ԭz��\n�u\n"+arg+"\n"+repeat_string(" ", MAX_LONG_WIDTH)+"�v�C\n", me, 0, 1);
	}
	else
	{
		set("long", arg, env);
		msg("$ME�ק惡�a�����ԭz��\n�u\n"+arg+"\n"+repeat_string(" ", MAX_LONG_WIDTH)+"�v�C\n", me, 0, 1);

		if( !env->save() )
			error("decorate save room error.");
	}
}

private void do_command(object me, string arg)
{
	object env = environment(me);
	string option;
	array loc = query_temp("location", me);
	string owner = query("owner", env);

	if( !arg )
		return tell(me, "�п�J�Q�n���C������(short, long, function, sign, master)�C\n");

	if( !env )
		return tell(me, pnoun(2, me)+"�L�k�b�o�̰����C���ʧ@�C\n");

	if( env->is_maproom() )
		owner = (MAP_D->query_map_system(loc))->query_coor_data(loc, "owner");

	if( !owner )
		return tell(me, "�o���ж����O"+pnoun(2, me)+"���A�L�k���C�o���ж��C\n");
	else if( belong_to_government(owner) )
	{
		if( !wizardp(me) && !CITY_D->is_mayor_or_officer(env->query_city(), me) )
			return tell(me, pnoun(2, me)+"���O�����Ωx���A�L�k���C�o���ж��C\n");
	}
	else if( belong_to_enterprise(owner) )
	{
		if( owner[11..] != query("enterprise", me) )
			return tell(me, pnoun(2, me)+"���O"+ENTERPRISE_D->query_enterprise_color_id(query("enterprise", me))+"���~���Ϊ������A�L�k���C�o���ж��C\n");
	}
	else if( owner != me->query_id(1) )
		return tell(me, "�o���ж����O"+pnoun(2, me)+"���A�L�k���C�o���ж��C\n");

	sscanf(arg, "%s %s", arg, option);

	switch(arg)
	{
	case "short":
		if( option == "-d" )
		{
			if( env->is_maproom() )
				(MAP_D->query_map_system(loc))->delete_coor_data(loc, "short");
			else
				return tell(me, "�L�k�R���ж��u�W�١C\n");

			return tell(me, "�R���u�W�٧����C\n");
		}
		else if( option )
			return input_room_short(me, env, option, 1);

		tell(me, HIC"\n�п�J���ж����u�W�١G"NOR);
		input_to( (: input_room_short, me, env :) );
		return;

	case "long":
		if( option == "-d" )
		{
			if( env->is_maproom() )
				(MAP_D->query_map_system(loc))->delete_coor_data(loc, "long");
			else
				return tell(me, "�L�k�R���ж����ԭz�C\n");

			return tell(me, "�R�����ԭz�����C\n");
		}
		tell(me, HIC"\n�п�J���ж������ԭz�G\n"HIR"�C��̦h "+MAX_LONG_WIDTH+" �r�A�̦h "+MAX_LONG_LINE+" ��\n"HIG"���Ϊ`���Ÿ��ֳt�ഫ�G[&,->�A][&'->�B][&.->�C][&!->�I][&?->�H]\n"NOR);
		me->edit( (: input_room_long, me, env :) );
		return;

	case "function":
		{
			mapping action_info;
			if( env->is_maproom() )
				return tell(me, "�o�̵L�k���o�˪����C�ʧ@�C\n");

			action_info = env->query_action_info();

			if( !option )
			{
				string msg = "�o�ɫؿv������]�m���ж��\�঳�H�U "+sizeof(action_info)+" �ءG\n";

				msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=    " �\��         �W��         �\��\n";
				msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				foreach( string short, mapping info in action_info )
				msg += sprintf(" %-12s %-12s %s\n", HIY+capitalize(short)+NOR, info["short"], YEL+implode(keys(info["action"]), " ")+NOR);

				msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n�п�J decorate function '�\��'�C\n\n"NOR;

				return tell(me, msg);
			}

			option = lower_case(option);

			if( option == "-d" )
			{
				if( env->remove_room_function() )
				{
					env->remove_master();
					env->remove_slave();
					return tell(me, "�������ж��\��]�w�����A�P�ɧR���s��]�w�C\n");
				}
				else
					return tell(me, "�������ѡC\n");
			}
			
			if( !wizardp(me) && strsrch(query("long", env), "�o�����") != -1 )
				return tell(me, "�o���ж��٨S���J�Ӹ��C�L�A�ӹL���õL�k�]�w�ж��\��C\n");

			if( option == "scenery" && env->query_floor() != 100 && env->query_floor() != 120 && env->query_floor() != 140 && env->query_floor() != 160 )
				return tell(me, "�[���x�u���b 100F / 120F / 140F / 160F ���Ӽh�i�H�]�m�C\n");
			
			if( query("function", env) == option )
				return tell(me, "���ж��w�g�]�w�����\��C\n");
			
			if( !env->set_room_function(option) )	
				return tell(me, "�o�ɫؿv���S���o�����������ж��C\n");

			env->remove_master();
			env->remove_slave();
			msg("$ME�N�o���ж��]�m��"+action_info[option]["short"]+"���γ~�C\n",me,0,1);

			break;
		}
	case "sign":
		{
			if( env->is_maproom() )
				return tell(me, pnoun(2, me)+"�L�k�ק�a�ϤW���ϧΡC\n");
	
			if( !option )
				return tell(me, "���B�γ��W���ϧάO�u"+(MAP_D->query_map_system(env->query_location()))->query_coor_icon(env->query_location())+NOR"�v�C\n");
			if( option == "-d" )
			{
				string origin_sign;
	
				if( env->is_maproom() )
					origin_sign = (MAP_D->query_map_system(loc))->query_coor_icon(loc);
				else
					origin_sign = (MAP_D->query_map_system(env->query_location()))->query_coor_icon(env->query_location());
	
				msg("$ME���W�F�o�̪��γ��N�쥻���u"+origin_sign+"�v�r��~���F�C\n", me, 0, 1);
				(MAP_D->query_map_system(env->query_location()))->set_coor_icon(env->query_location(), "  ");
				break; 
			}
	
	
			if( noansi_strlen(option) > 2 )
				return tell(me, "�۵P�@�өж��̦h 2 �Ӧr���C\n");
	
			(MAP_D->query_map_system(env->query_location()))->set_coor_icon(env->query_location(), sprintf("%2s", kill_repeat_ansi(option+NOR)));
			msg("$ME���ۤ@��o����A���W�F�o�̪��γ��j�j�a�g�F�ӡu"+option+NOR"�v�r�C\n",me, 0, 1);

			break;
		}
	case "master":
		{
			int x, y, section;
			array envloc = env->query_location();
			array newloc;
			string room_type, env_basename, master_basename;
			object master = env->query_master();
			string functionname;
			mapping products;
		
			if( env->is_maproom() )
				return tell(me, "�o�̵L�k���o�˪����C�ʧ@�C\n");

			if( !option )	
				return tell(me, "�п�J���������D�s������ж��Ҧb�����P�y��(�Ҧp�ĤG�ìP�����G2 53,49)�C\n",me, 0, 1);

			functionname = query("function", env);
			
			if( !functionname )
				return tell(me, "���ж����]�w�ж��\�ण��i��s��ʧ@�C\n");

			if( !mapp(env->query_action_info(functionname)) || !env->query_action_info(functionname)["master"] )
				return tell(me, "���إ\��ж��L�k�i��s��ʧ@�C\n");

			if( option == "-d" )
			{
				if( master == env )
					env->remove_slave();

				env->remove_master();
				env->save();
				return tell(me, "�R�����ж��������D�s�����"+query("short", master)+"�]�w�C\n");
			}

			if( sscanf(option, "%d %d,%d", section, x, y) != 3 )
				return tell(me, "�п�J���������D�s������ж��Ҧb�����P�y��(�Ҧp�ĤG�ìP�����G2 53,49)�C\n",me, 0, 1);

			if( sizeof(products = query("products", env)) )
				tell(me, HIY"�`�N�G�s�ꤧ��o�өж��쥻���w�s��ƱN�|�Q���\(�Ҧ���ƥH�s�ꤤ�߬���)\n�Ѱ��s��᥻�ж���l���w�s��ƫK�i�H�A��ܥX�ӡC\n"NOR);
				
			env->remove_master();
			env_basename = base_name(env);

			room_type = ESTATE_D->query_loc_estate(envloc)["type"];

			if( !CITY_D->city_exist(envloc[CITY], section-1) )
				return tell(me, CITY_D->query_city_idname(envloc[CITY])+"�S���� "+section+" �ìP�����C\n");

			master_basename = CITY_ROOM_MODULE(envloc[CITY], (section-1), (x-1), (y-1), room_type);
			newloc = arrange_city_location(x-1, y-1, envloc[CITY], section-1);
			
			if( master_basename == env_basename )
				return tell(me, "�������D�s���������O�ۤv�C\n");

			if( sizeof(query("slave", env)) )
				return tell(me, "�����ж��O�s�ꤤ�ߡA�w�g�Q��L�����s��A�L�k�A�s��ܨ�L�����C\n");

			if( !file_exists(master_basename) )
			{
				tell(find_player("sinji"), master_basename);
				return tell(me, "�y��"+loc_short(newloc)+"�B�èS������ؿv���C\n");
			}
			master = load_object(master_basename);

			if( query("owner", master) == "GOVERNMENT/"+envloc[CITY] )
			{
				if( !CITY_D->is_mayor_or_officer(envloc[CITY], me) )
					return tell(me, pnoun(2, me)+"���O�����Ωx���A�L�k���C�o���ж��C\n");
			}
			else if( query("owner", master) != me->query_id(1) )
				return tell(me, "�y��"+loc_short(newloc)+"��"+master->query_room_name()+"�ä��O�ݩ�"+pnoun(2, me)+"���C\n");

			if( master->query_master() != master )
				return tell(me, "�y��"+loc_short(newloc)+"��"+master->query_room_name()+"�w�g����ۤv�ҹ������D�s������A�L�k�Q�]�w���D�s������C\n");

			if( query("function", env) != query("function", master) )
				return tell(me, "�y��"+loc_short(newloc)+"��"+master->query_room_name()+"�P�����ж����\��ä��ۦP�A�L�k�]�w�s��C\n");

			if( sizeof(master->query_slave()) > 500 )
				return tell(me, "�s��ж��Ƥ��o�W�L 500�C\n");

			if( !env->set_master(master) )
				return tell(me, "�L�k�]�w�䬰�D�s������C\n");

			// ���H�Q�Ψ�ӳs�꤬�ۥ洫�ȧQ, �Q�Ψ�s����t, �X�洫�ɥ����s�p���ʶR���ɦȧQ, �]���ΥH�U�{���ץ�
			env->query_module_file()->calculate_master_data(master);

			msg("$ME�]�w"+master->query_room_name()+"�����ж����D�n�s�곡���C\n", me, 0, 1);
			break;
		}
	default:
		return tell(me, "�п�J�Q�n���C������(short, long, function, sign, master)�C\n");
	}

	if( env->save() )
		tell(me, "���C�����C\n");
	else
		tell(me, "���C���ѡC\n");
}
