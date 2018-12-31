/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : expedition.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-24
 * Note   : ���I��a
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
#include <citydata.h>
#include <object.h>
#include <npc.h>

inherit ROOM_ACTION_MOD;

#define BASE_COST		20000
#define COST_PER_NPC		1000

void confirm_explore(object me, object env, mapping data, string arg)
{
	object ob;
	string basename = base_name(env);
	mapping adventuredata = allocate_mapping(0);
	string npctype;

	if( arg != "y" && arg != "yes" )
	{
		tell(me, "�������I���ʡC\n");
		return me->finish_input();
	}
	
	if( !me->spend_money(data["money_unit"], data["money"]) )
		return tell(me, pnoun(2, me)+"���W�������� "HIY+money(data["money_unit"], data["money"])+NOR"�C\n");

	adventuredata["from"] = basename;
	adventuredata["area"] = data["area"];
	adventuredata["num"] = data["num"];

	foreach(npctype in ({ "leader", "prospector", "explorer", "guard", "scout"}))
	{
		if( data[npctype] )
		{
			ob = find_object(data[npctype]);
			set("adventure", adventuredata, ob);
			ob->move(VOID_OB);
		}
	}
	
	ADVENTURE_D->start_adventure(env, data);
}

void do_plan(object me, string arg)
{
	object env = environment(me);
	string option;
	string value;
	string myid = me->query_id(1);

	if( query("owner", env) != myid )
		return tell(me, pnoun(2, me)+"���O�o�ӱ��I��a���֦��̡C\n");
		
	if( !arg )
		return tell(me, "�п�J���T�����O�榡(help plan)�C\n");
	
	// �}�l���I	
	if( arg == "start" )
	{
		mapping data = allocate_mapping(0);

		string msg;

		object leader_ob;
		object prospector_ob;
		object explorer_ob;
		object guard_ob;
		object scout_ob;
		
		data["leader"] = query("plan/team/leader", env);
		data["prospector"] = query("plan/team/prospector", env);
		data["explorer"] = query("plan/team/explorer", env);
		data["guard"] = query("plan/team/guard", env);
		data["scout"] = query("plan/team/scout", env);
		data["area"] = query("plan/area/area", env);
		data["num"] = query("plan/area/num", env);
		data["time"] = query("plan/time" , env);
		data["money"] = BASE_COST;
		data["money_unit"] = env->query_money_unit();
		data["boss"] = myid;

		if( ADVENTURE_D->is_doing_adventure(env) )
			return tell(me, "�o�ӭp�e�Ǫ����I���ʥ��b�i�椤�C\n");

		if( !data["area"] || !AREA_D->area_exist(data["area"], data["num"]) )
			return tell(me, "���]�w���I�ϰ�C\n");

		if( !data["time"] )
			return tell(me, "���]�w���I�ɶ��C\n");
		
		if( !stringp(data["leader"]) || !stringp(data["prospector"]) )
			return tell(me, "�@�ӱ��I�����������ܤ֭n���@�Ӷ����M�@�ӱ��ɪ̡C\n");
		
		if( !query("warehouse", env) )
			return tell(me, "�|���]�w���궰���a�C\n");
			
		if( stringp(data["leader"]) )
		{
			if( !file_exists(data["leader"]) )
				return tell(me, "���������u��Ƥw�g�����C\n");

			leader_ob = load_object(data["leader"]);
			
			if( !same_environment(me, leader_ob) )
				return tell(me, leader_ob->query_idname()+"���b�o�̡C\n");

			if( query("boss", leader_ob) != myid )
				return tell(me, leader_ob->query_idname()+"�����D���O"+pnoun(2, me)+"�C\n");
			
			if( query("job/cur", leader_ob) != ADVENTURER )
				return tell(me, "�������R�O"+leader_ob->query_idname()+"�q�Ʊ��I���u�@�C\n");

			data["money"] += COST_PER_NPC * data["time"];
		}
		
		if( stringp(data["prospector"]) )
		{
			if( !file_exists(data["prospector"]) )
				return tell(me, "���ɪ̪����u��Ƥw�g�����C\n");

			prospector_ob = load_object(data["prospector"]);
			
			if( !same_environment(me, prospector_ob) )
				return tell(me, prospector_ob->query_idname()+"���b�o�̡C\n");

			if( query("boss", prospector_ob) != myid )
				return tell(me, prospector_ob->query_idname()+"�����D���O"+pnoun(2, me)+"�C\n");

			if( query("job/cur", prospector_ob) != ADVENTURER )
				return tell(me, "�������R�O"+prospector_ob->query_idname()+"�q�Ʊ��I���u�@�C\n");

			data["money"] += COST_PER_NPC * data["time"];
		}
	
		if( stringp(data["explorer"]) )
		{
			if( !file_exists(data["explorer"]) )
				return tell(me, "���I�a�����u��Ƥw�g�����C\n");

			explorer_ob = load_object(data["explorer"]);
		
			if( !same_environment(me, explorer_ob) )
				return tell(me, explorer_ob->query_idname()+"���b�o�̡C\n");
		
			if( query("boss", explorer_ob) != myid )
				return tell(me, explorer_ob->query_idname()+"�����D���O"+pnoun(2, me)+"�C\n");
				
			if( query("job/cur", explorer_ob) != ADVENTURER )
				return tell(me, "�������R�O"+explorer_ob->query_idname()+"�q�Ʊ��I���u�@�C\n");

			data["money"] += COST_PER_NPC * data["time"];
		}
		
		if( stringp(data["guard"]) )
		{
			if( !file_exists(data["guard"]) )
				return tell(me, "�@�ê����u��Ƥw�g�����C\n");

			guard_ob = load_object(data["guard"]);
			
			if( !same_environment(me, guard_ob) )
				return tell(me, guard_ob->query_idname()+"���b�o�̡C\n");

			if( query("boss", guard_ob) != myid )
				return tell(me, guard_ob->query_idname()+"�����D���O"+pnoun(2, me)+"�C\n");

			if( query("job/cur", guard_ob) != ADVENTURER )
				return tell(me, "�������R�O"+guard_ob->query_idname()+"�q�Ʊ��I���u�@�C\n");

			data["money"] += COST_PER_NPC * data["time"];
		}
			
		if( stringp(data["scout"]) )
		{
			if( !file_exists(data["scout"]) )
				return tell(me, "���d�̪����u��Ƥw�g�����C\n");

			scout_ob = load_object(data["scout"]);

			if( !same_environment(me, scout_ob) )
				return tell(me, scout_ob->query_idname()+"���b�o�̡C\n");
				
			if( query("boss", scout_ob) != myid )
				return tell(me, scout_ob->query_idname()+"�����D���O"+pnoun(2, me)+"�C\n");

			if( query("job/cur", scout_ob) != ADVENTURER )
				return tell(me, "�������R�O"+scout_ob->query_idname()+"�q�Ʊ��I���u�@�C\n");

			data["money"] += COST_PER_NPC * data["time"];
		}
		
		msg =  "�o�����I����ƻP�ݭn�������B����\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += "�ϰ�    �G"+AREA_D->query_area_idname(data["area"], data["num"])+"\n";
		msg += HIW"��"NOR WHT"��"NOR"    �G"+leader_ob->query_idname()+"\n";
		msg += HIY"��"NOR YEL"�ɪ�"NOR"  �G"+prospector_ob->query_idname()+"\n";
		msg += HIG"��"NOR GRN"�I�a"NOR"  �G"+(explorer_ob ? explorer_ob->query_idname() : "�����w")+"\n";
		msg += HIR"�@"NOR RED"��"NOR"    �G"+(guard_ob ? guard_ob->query_idname() : "�����w")+"\n";
		msg += HIM"��"NOR MAG"�d��"NOR"  �G"+(scout_ob ? scout_ob->query_idname() : "�����w")+"\n";
		msg += "�ݭn�����G"HIY+money(data["money_unit"], data["money"])+NOR"\n";
		msg += "�ݭn����G�L\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

		tell(me, msg + "�O�_�T�w�}�l���I(y/n)�H");
		
		input_to( (: confirm_explore, me, env, data :) );

		return;
	}	
	
	if( sscanf(arg, "%s %s", option, value) != 2 )
		return tell(me, "�п�J���T�����O�榡(help plan)�C\n");
		
	switch(option)
	{
		case "area":
		{
			string area;
			int num;
			
			if( sscanf(value, "%s %d", area, num) != 2 )
				return tell(me, "�п�J���T�����O�榡(help plan)�C\n");
		
			num--;

			if( !AREA_D->area_exist(area, num) )
				return tell(me, "�S���o�Ӱϰ�C\n");
				
			set("plan/area/area", area, env);
			set("plan/area/num", num, env);
			
			env->save();
			
			msg("$ME�N���I�ϰ�]�w�b�u"+AREA_D->query_area_idname(area, num)+"�v�C\n", me, 0, 1);
		
			break;
		}
		case "team":
		{
			string id;
			string type;
			object ob;
			
			if( value == "-d" )
			{
				delete("plan/team", env);
				
				env->save();

				return tell(me, "�M���Ҧ����I�������]�w�C\n");	
			}
			
			if( sscanf(value, "%s as %s", id, type) != 2 )
				return tell(me, "�п�J���T�����O�榡(help plan)�C\n");
				
			if( !objectp(ob = present(id)) )
				return tell(me, "�o�̨S���o�ӪF��C\n");
		
			if( !ob->is_living() )
				return tell(me, ob->query_idname()+"���O�ͪ��C\n");
				
			if( query("boss", ob) != myid )
				return tell(me, ob->query_idname()+"�����D���O"+pnoun(2, me)+"�C\n");
				
			switch(type)
			{
				case "leader":
				{
					set("plan/team/"+type, base_name(ob), env);
					msg("$ME�N$YOU�]�w�����I����"HIW"��"NOR WHT"��"NOR"�C\n", me, ob, 1);
					env->save();
					break;
				}
				case "prospector":
				{
					set("plan/team/"+type, base_name(ob), env);
					msg("$ME�N$YOU�]�w�����I����"HIY"��"NOR YEL"�ɪ�"NOR"�C\n", me, ob, 1);
					env->save();
					break;
				}
				case "explorer":
				{
					set("plan/team/"+type, base_name(ob), env);
					msg("$ME�N$YOU�]�w�����I����"HIG"��"NOR GRN"�I�a"NOR"�C\n", me, ob, 1);
					env->save();
					break;
				}
				case "guard":
				{
					set("plan/team/"+type, base_name(ob), env);
					msg("$ME�N$YOU�]�w�����I����"HIR"�@"NOR RED"��"NOR"�C\n", me, ob, 1);
					env->save();
					break;
				}
				case "scout":
				{
					set("plan/team/"+type, base_name(ob), env);
					msg("$ME�N$YOU�]�w�����I����"HIM"��"NOR MAG"�d��"NOR"�C\n", me, ob, 1);
					env->save();
					break;
				}
				default:
				{
					return tell(me, "�S�� "+type+" �����������C\n");
					break;
				}
			}
			
			break;
		}
		case "time":
		{
			int time = to_int(value);
			
			if( time < 10 || time > 1440 )
				return tell(me, "���I�ɶ��̵u 10 �����A�̪� 1440 �����C\n");
				
			set("plan/time", time, env);
			env->save();
			msg("$ME�N�w�p�����I�ɶ��q�� "+time+" �����C\n", me, 0, 1);
			break;
		}
		default:
		{
			return tell(me, "�п�J���T�����O�榡(help plan)�C\n");
			break;
		}
	}
}

void do_setup(object me, string arg)
{
	string value;
	int x, y, num;
	object warehouse;
	string file;
	string city;
	object env = environment(me);
	object master = env->query_master();
	array loc = env->query_location();

	city = loc[CITY];

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�]�w���򶵥ءH(help setup)\n");
	
	if( query("owner", master) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"���O�o�ӱ��I��a���֦��̡C\n");

	sscanf(arg, "%s %s", arg, value);

	switch(arg)
	{
		case "warehouse":
		{
			if( value == "-d" )
			{
				if( !query("warehouse", master) )
					return tell(me, "�o�̭쥻�K�S���]�w���궰���a�C\n");
					
				delete("delivery", master);
				master->save();
				return tell(me, "�������궰���a�����C\n");
			}

			if( !value || sscanf(value, "%d %d,%d", num, x, y) != 3 )
				return tell(me, "�п�J���T���ܮw�y��(�� 1 21,33)�C\n");
			
			// ��f�ܭܮw
			file = CITY_ROOM_MODULE(city, (num-1), (x-1), (y-1), "warehouse");

			if( !objectp(warehouse = load_module(file)) || query("function", warehouse) != "warehouse" )
				return tell(me, "�y��"+loc_short(city, num-1, x-1, y-1)+"�èS���ܮw�C\n");
			
			if( warehouse != warehouse->query_master() )
			{
				file = base_name(warehouse->query_master());
				warehouse = warehouse->query_master();
			}

			if( query("owner", warehouse) != me->query_id(1) )
				return tell(me, warehouse->query_room_name()+"�ä��ݩ�"+pnoun(2, me)+"�C\n");
			
			set("warehouse", file, master);
			
			master->save();
			
			msg("$ME�]�w"+warehouse->query_room_name()+"�������I��a�����궰���a�C\n", me, 0, 1);
			
			break;
		}

		default:
			return tell(me, "�п�J���T���ﶵ(help setup)�C\n");
	}
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"plan"	:
	([
		"short"	: HIG"�p����"NOR,
		"job"	: ADVENTURER,
		"help"	:
			([
"topics":
@HELP
    ���I���ʪ��p���ǡC
HELP,

"plan":
@HELP
�p�e���I���e�A�Ϊk�p�U�G
plan area '�ϰ� ID' '���Ͻs��'		- �]�w�p�����I���ϰ�A�ҡGplan area memoryland 3
plan team '���u ID' as leader		- �]�w�Y�ӭ��u�@���o�����I������(���D�y�O)
plan team '���u ID' as prospector	- �]�w�Y�ӭ��u�@���o�����I�����ɪ�(���D�O�q)
plan team '���u ID' as explorer		- �]�w�Y�ӭ��u�@���o�����I�����I�a(���D���O)
plan team '���u ID' as guard		- �]�w�Y�ӭ��u�@���o�����I���@��(���D���)
plan team '���u ID' as scout		- �]�w�Y�ӭ��u�@���o�����I�����d��(���D�ӱ�)
plan team -d				- �M���Ҧ����I�����]�w
plan time '����'			- �]�w�w�p�����I�ɶ�
plan start				- �}�l���I�I
HELP,

"setup":
@HELP
�]�w�Ͳ��u��ƪ����O�A�Ϊk�p�U�G[�޲z���O]
  setup warehouse 1 75,90	- �]�w���궰���a�y�Цb�Ĥ@�ìP������ 75,90
  setup warehouse -d	 	- �������궰���a���]�w
HELP,

			]),
		"action":
			([
				"plan"		:	(: do_plan($1, $2) :),
				"setup"		:	(: do_setup($1, $2) :),
			]),
	]),
]);


// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIW"���I"NOR WHT"��a"NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,INDIVIDUAL

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,AGRICULTURE_REGION

	// �}�i�����O��
	,"200000"
	
	// �ؿv���������ռаO
	,1

	// �c�a�^�m��
	,0
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});

