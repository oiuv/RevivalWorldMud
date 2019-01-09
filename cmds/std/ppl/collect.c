/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : collect.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-2-19
 * Note   : collect ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#define COLLECT_STR_COST	10

#define MATERIAL_PATH		"/obj/materials/"

#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <material.h>
#include <location.h>

inherit COMMAND;

string help = @HELP

�Ķ����O

�o�ӫ��O�i�b�a�ϤW�Ķ��U�ح�ơA��Ʀ@�����ءC

metal	���� - ��s�ϨϥΤQ�r�����u��Ķ�
stone	��� - ��s�Ϩϥ��p�Y���u��Ķ�
water	�M�� - ��e�y�ϥΤ������u��Ķ�
wood	��� - ��˪L�ϥΤ�����u��Ķ�
fuel	�U�� - ��˪L�ϥ���l���u��Ķ�

���O�榡:
collect metal		- �Ķ��@�Ӫ���(�Ķ����W�������Q�r�����u��)
collect 10 metal	- �@���Ķ��Q�Ӫ���
			  �i�`�٨ǳ\��O�P��֤u��l�a�v�üW�[�}�����~����v
collect info		- �d�߷��ӫ�������ƱĶ���T

�������O: info

HELP;

// �@���ķU�h�ٷU�h�O
private int cost_stamina(object me, int amount, int skilllevel)
{
	if( amount <= 10 )
		return me->cost_stamina(amount * (COLLECT_STR_COST - skilllevel/4));
	else if( amount <= 20 )
		return me->cost_stamina(amount * (COLLECT_STR_COST - skilllevel/4 - 1));
	else if( amount <= 50 )
		return me->cost_stamina(amount * (COLLECT_STR_COST - skilllevel/4 - 2));
	else if( amount <= 100 )
		return me->cost_stamina(amount * (COLLECT_STR_COST - skilllevel/4 - 3));

	return me->cost_stamina(amount * (COLLECT_STR_COST - skilllevel/4 - 4));
}

private void do_command(object me, string arg)
{
	array loc;
	object ob, env = environment(me);
	mapping coorrangetype;
	object *tools;
	int skilllevel, endurance, materialamount, amount;
	string sloc, arg2;
	string city;
	int num;
	string myid = me->query_id(1);

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�Ķ����ظ귽�H("HIW"���� metal"NOR"�B"WHT"��� stone"NOR"�B"HIC"�M�� water"NOR"�B"YEL"��� wood"NOR"�B"HIM"�U�� fuel"NOR")\n");

	if( (wizardp(me) && sscanf(arg, "info %s", city) == 1) || arg == "info" )
	{
		string msg;
		string id;
		mapping collection_record;
		mapping data;

		if( !city )
			city = query("city", me);

		if( !city )
			return tell(me, pnoun(2, me)+"���ݩ����@�y�����C\n");

		if( !CITY_D->city_exist(city) )
			return tell(me, "�S�� "+city+" �o�y�����C\n");

		msg = CITY_D->query_city_idname(city)+"������ƱĶ����p�p�U(�۰ʩ�C�� 23:00 �k�s)�G\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

		for(num=0;CITY_D->city_exist(city, num);num++)
		{
			msg += CITY_D->query_city_idname(city, num)+"\n";

			collection_record = CITY_D->query_collection_record(city, num);

			if( !sizeof(collection_record) )
				msg += "  �L�H�Ķ�\n";
			else
			foreach(id, data in collection_record)
			{
				msg += sprintf("  %-15s%-10s%-10s%-10s%-10s%-10s\n",
					capitalize(id),
					HIW+data["metal"]+NOR,
					WHT+data["stone"]+NOR,
					HIC+data["water"]+NOR,
					YEL+data["wood"]+NOR,
					HIM+data["fuel"]+NOR);
			}

			msg += "\n";
		}

		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += HIW"���� metal"NOR"�B"WHT"��� stone"NOR"�B"HIC"�M�� water"NOR"�B"YEL"��� wood"NOR"�B"HIM"�U�� fuel"NOR"\n\n";
		return me->more(msg);
	}

	if( !env || !env->is_maproom() )
		return tell(me, "�L�k�b�o�̱Ķ��귽�C\n");


	//���L������U���O
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	loc = query_temp("location", me);

	if( (MAP_D->query_map_system(loc)) != CITY_D )
		return tell(me, "�L�k�b�o�̱Ķ��귽�C\n");

 	city = loc[CITY];
	num = loc[NUM];

 	if( !CITY_D->is_citizen(me->query_id(1), city) )
 		return tell(me, pnoun(2, me)+"���O"+CITY_D->query_city_idname(city)+"�������A�L�k�b�o�̱Ķ��귽�C\n");

	coorrangetype = CITY_D->query_coor_range(loc, TYPE, 1);

	arg = lower_case(arg);

	if( sscanf(arg, "%d %s", amount, arg2) == 2 )
		arg = arg2;

	if( amount < 1 )
		amount = 1;

	if( amount > 1000 )
		return tell(me, "�̦h�@���u��Ķ� 1000 �Ӹ귽�C\n");

	switch(arg)
	{
		case "metal":
			skilllevel = me->query_skill_level(arg);

			coorrangetype = filter(coorrangetype, (: $2 == MOUNTAIN :));

			if( !sizeof(coorrangetype) )
				return tell(me, "���񪺰ϰ줤�èS���s�ϡA�S��k�}�Ī��ݡC\n");

			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));

			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"���W�S���ΨӶ}�Ī��ݪ��u��C\n");

			// �ˬd����ϰ쪺���Ʀs�q
			foreach(sloc in keys(coorrangetype))
			{
				loc = restore_variable(sloc);
				materialamount = CITY_D->query_section_info(city, num, "resource/"+arg);

				if( materialamount >= amount )
				{
					if( !cost_stamina(me, amount, skilllevel) )
						return msg("$ME��ݼN�N�A�w�g�S�O��A�}�Ī��ݤF...�C\n", me, 0, 1);

					endurance = query("endurance", tools[0]);

					// �u��l�a(�l�a���v�P�u��@�[�שM�ϥΪ̧ޯ����)
					if( !random(endurance + to_int(skilllevel*endurance/100.)) )
					{
						msg("$ME��W��"+tools[0]->query_idname()+"��M�y�ԡz���@�n�A��`�B���F�}�ӡA��M�O�S��k�A�~��ΨӶ}�Ī��ݤF...�C\n", me, 0, 1);

						if( !random(2) )
						{
							tell(me, pnoun(2, me)+"�q�o�Өƥ���o��h�����|�g��C\n");
							me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
						}

						destruct(tools[0], 1);
						return;
					}

					CITY_D->set_section_info(city, num, "resource/"+arg, materialamount-amount);

					ob = new(MATERIAL_PATH+arg);

					if( amount > 1 )
						set_temp("amount", amount, ob);

					msg("$ME��O�a�q�q�ۤ��w�X�F"+ob->short(1)+"\n", me, 0, 1);

					// ������ƱĶ���T
					CITY_D->add_collection_record(city, num, myid, arg, amount);

					if( !me->get_object(ob, amount) )
					{
						msg("$ME�w�g������"+ob->short(1)+"�F�I�u�n����b�a�W�C\n", me, 0, 1);
						ob->move_to_environment(me);
					}

					if( me->query_skill_level("metal") < 20 )
						me->add_skill_exp("metal", amount*(12+random(11)));
					else if( me->query_skill_level("metal") == 20 )
						tell(me, pnoun(2, me)+"���Ķ��ޯ൥�Ťw�g��F 20�A�L�k�A�ۧک��W���ɡA�󰪲`���ǰݥ����}�l��j�ǾǲߡC\n");

					me->add_social_exp(to_int(pow(amount*(20+random(21)), 0.9)));
					TREASURE_D->get_treasure_books(me, amount);
					return;
				}
			}
			return tell(me, "��y���������ݸ귽���w�g�Q�j�q�}�ġA�ѤU���O����A�ϥΪ��H�q�C\n");
			break;

		case "stone":
			skilllevel = me->query_skill_level(arg);

			coorrangetype = filter(coorrangetype, (: $2 == MOUNTAIN :));

			if( !sizeof(coorrangetype) )
				return tell(me, "���񪺰ϰ줤�èS���s�ϡA�S��k�}�ĥۧ��C\n");

			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));

			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"���W�S���ΨӶ}�ĥۧ����u��C\n");

			// �ˬd����ϰ쪺���Ʀs�q
			foreach(sloc in keys(coorrangetype))
			{
				loc = restore_variable(sloc);
				materialamount = CITY_D->query_section_info(city, num, "resource/"+arg);

				if( materialamount >= amount )
				{
					if( !cost_stamina(me, amount, skilllevel) )
						return msg("$ME��ݼN�N�A�w�g�S�O��A�}�ĥۧ��F...�C\n", me, 0, 1);

					endurance = query("endurance", tools[0]);

					// �u��l�a(�l�a���v�P�u��@�[�שM�ϥΪ̧ޯ����)
					if( !random(endurance + to_int(skilllevel*endurance/100.)) )
					{
						msg("$ME��W��"+tools[0]->query_idname()+"��M�y�ԡz���@�n�A��`�B���F�}�ӡA��M�O�S��k�A�~��ΨӶ}�ĥۧ��F...�C\n", me, 0, 1);

						if( !random(2) )
						{
							tell(me, pnoun(2, me)+"�q�o�Өƥ���o��h�����|�g��C\n");
							me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
						}
						destruct(tools[0], 1);
						return;
					}

					CITY_D->set_section_info(city, num, "resource/"+arg, materialamount-amount);

					ob = new(MATERIAL_PATH+arg);

					if( amount > 1 )
						set_temp("amount", amount, ob);

					msg("$ME��O�a�q���h�w�X�F"+ob->short(1)+"\n", me, 0, 1);

					// ������ƱĶ���T
					CITY_D->add_collection_record(city, num, myid, arg, amount);

					if( !me->get_object(ob, amount) )
					{
						msg("$ME�w�g������"+ob->short(1)+"�F�I�u�n����b�a�W�C\n", me, 0, 1);
						ob->move_to_environment(me);
					}

					if( me->query_skill_level("stone") < 20 )
						me->add_skill_exp("stone", amount*(12+random(11)));
					else if( me->query_skill_level("stone") == 20 )
						tell(me, pnoun(2, me)+"���Ķ��ޯ൥�Ťw�g��F 20�A�L�k�A�ۧک��W���ɡA�󰪲`���ǰݥ����}�l��j�ǾǲߡC\n");

					me->add_social_exp(to_int(pow(amount*(20+random(21)), 0.9)));
					TREASURE_D->get_treasure_books(me, amount);
					return;
				}
			}
			return tell(me, "��y�������ۧ��귽���w�g�Q�j�q�}�ġA�ѤU���O����A�ϥΪ��H�ۡC\n");
			break;

		case "water":
			skilllevel = me->query_skill_level(arg);

			coorrangetype = filter(coorrangetype, (: $2 == POOL || $2 == RIVER :));

			if( !sizeof(coorrangetype) )
				return tell(me, "���񪺰ϰ줤�èS�������A�S��k�Ķ��M���C\n");

			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));

			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"���W�S���ΨӶ������u��C\n");

			// �ˬd����ϰ쪺���Ʀs�q
			foreach(sloc in keys(coorrangetype))
			{
				loc = restore_variable(sloc);
				materialamount = CITY_D->query_section_info(city, num, "resource/"+arg);

				if( materialamount >= amount )
				{
					if( !cost_stamina(me, amount, skilllevel) )
						return msg("$ME��ݼN�N�A�w�g�S�O��A����F...�C\n", me, 0, 1);

					endurance = query("endurance", tools[0]);

					// �u��l�a(�l�a���v�P�u��@�[�שM�ϥΪ̧ޯ����)
					if( !random(endurance + to_int(skilllevel*endurance/100.)) )
					{
						msg("$ME��W��"+tools[0]->query_idname()+"��M�y�ԡz���@�n�A�������F�@�Ӥj�}�A��M�O�S��k�A�~��ΨӸˤ��F...�C\n", me, 0, 1);

						if( !random(2) )
						{
							tell(me, pnoun(2, me)+"�q�o�Өƥ���o��h�����|�g��C\n");
							me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
						}

						destruct(tools[0], 1);
						return;
					}

					CITY_D->set_section_info(city, num, "resource/"+arg, materialamount-amount);

					ob = new(MATERIAL_PATH+arg);

					if( amount > 1 )
						set_temp("amount", amount, ob);

					msg("$ME�ΤO�a�q�e����_�F"+ob->short(1)+"\n", me, 0, 1);

					// ������ƱĶ���T
					CITY_D->add_collection_record(city, num, myid, arg, amount);

					if( !me->get_object(ob, amount) )
					{
						msg("$ME�w�g������"+ob->short(1)+"�F�I�u�n����b�a�W�C\n", me, 0, 1);
						ob->move_to_environment(me);
					}

					if( me->query_skill_level("water") < 20 )
						me->add_skill_exp("water", amount*(12+random(11)));
					else if( me->query_skill_level("water") == 20 )
						tell(me, pnoun(2, me)+"���Ķ��ޯ൥�Ťw�g��F 20�A�L�k�A�ۧک��W���ɡA�󰪲`���ǰݥ����}�l��j�ǾǲߡC\n");

					me->add_social_exp(to_int(pow(amount*(20+random(21)), 0.9)));
					TREASURE_D->get_treasure_books(me, amount);
					return;
				}
			}
			return tell(me, "��y���������귽���w�g�Q�j�q�ϥΡA������o�۷�V�B����A�ϥΤF�C\n");
			break;

		case "wood":
			skilllevel = me->query_skill_level(arg);

			coorrangetype = filter(coorrangetype, (: $2 == FOREST :));

			if( !sizeof(coorrangetype) )
				return tell(me, "���񪺰ϰ줤�èS����L�A�S��k���C\n");

			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));

			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"���W�S���Ψӥ�쪺�u��C\n");

			// �ˬd����ϰ쪺���Ʀs�q
			foreach(sloc in keys(coorrangetype))
			{
				loc = restore_variable(sloc);
				materialamount = CITY_D->query_section_info(city, num, "resource/"+arg);

				if( materialamount >= amount )
				{
					if( !cost_stamina(me, amount, skilllevel) )
						return msg("$ME��ݼN�N�A�w�g�S�O��A���F...�C\n", me, 0, 1);

					endurance = query("endurance", tools[0]);

					// �u��l�a(�l�a���v�P�u��@�[�שM�ϥΪ̧ޯ����)
					if( !random(endurance + to_int(skilllevel*endurance/100.)) )
					{
						msg("$ME��W��"+tools[0]->query_idname()+"��M�y�ԡz���@�n�A��`�B���F�}�ӡA��M�O�S��k�A�~��Ψӥ��F...�C\n", me, 0, 1);

						if( !random(2) )
						{
							tell(me, pnoun(2, me)+"�q�o�Өƥ���o��h�����|�g��C\n");
							me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
						}

						destruct(tools[0], 1);
						return;
					}

					CITY_D->set_section_info(city, num, "resource/"+arg, materialamount-amount);

					ob = new(MATERIAL_PATH+arg);

					if( amount > 1 )
						set_temp("amount", amount, ob);

					msg("$ME�V�O�a�q������U�F"+ob->short(1)+"\n", me, 0, 1);

					// ������ƱĶ���T
					CITY_D->add_collection_record(city, num, myid, arg, amount);

					if( !me->get_object(ob, amount) )
					{
						msg("$ME�w�g������"+ob->short(1)+"�F�I�u�n����b�a�W�C\n", me, 0, 1);
						ob->move_to_environment(me);
					}

					if( me->query_skill_level("wood") < 20 )
						me->add_skill_exp("wood", amount*(12+random(11)));
					else if( me->query_skill_level("wood") == 20 )
						tell(me, pnoun(2, me)+"���Ķ��ޯ൥�Ťw�g��F 20�A�L�k�A�ۧک��W���ɡA�󰪲`���ǰݥ����}�l��j�ǾǲߡC\n");

					me->add_social_exp(to_int(pow(amount*(20+random(21)), 0.9)));
					TREASURE_D->get_treasure_books(me, amount);
					return;
				}
			}
			return tell(me, "��y��������L�귽�w�g�Q�j�q���A�ѤU���O����A�ϥΪ����p��]�C\n");
			break;

		case "fuel":
			skilllevel = me->query_skill_level(arg);

			coorrangetype = filter(coorrangetype, (: $2 == FOREST :));

			if( !sizeof(coorrangetype) )
				return tell(me, "���񪺰ϰ줤�èS���˪L�A�S��k�}�ĿU�ơC\n");

			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));

			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"���W�S���ΨӶ}�ĿU�ƪ��u��C\n");

			// �ˬd����ϰ쪺���Ʀs�q
			foreach(sloc in keys(coorrangetype))
			{
				loc = restore_variable(sloc);
				materialamount = CITY_D->query_section_info(city, num, "resource/"+arg);

				if( materialamount >= amount )
				{
					if( !cost_stamina(me, amount, skilllevel) )
						return msg("$ME��ݼN�N�A�w�g�S�O��A�}�ĿU�ƤF...�C\n", me, 0, 1);

					endurance = query("endurance", tools[0]);

					// �u��l�a(�l�a���v�P�u��@�[�שM�ϥΪ̧ޯ����)
					if( !random(endurance + to_int(skilllevel*endurance/100.)) )
					{
						msg("$ME��W��"+tools[0]->query_idname()+"��M�y�ԡz���@�n�A��`�B���F�}�ӡA��M�O�S��k�A�~��ΨӶ}�ĿU�ƤF...�C\n", me, 0, 1);

						if( !random(2) )
						{
							tell(me, pnoun(2, me)+"�q�o�Өƥ���o��h�����|�g��C\n");
							me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
						}

						destruct(tools[0], 1);
						return;
					}

					CITY_D->set_section_info(city, num, "resource/"+arg, materialamount-amount);

					ob = new(MATERIAL_PATH+arg);

					if( amount > 1 )
						set_temp("amount", amount, ob);

					msg("$ME��O�a�q�g�a�̨��X�F"+ob->short(1)+"\n", me, 0, 1);

					// ������ƱĶ���T
					CITY_D->add_collection_record(city, num, myid, arg, amount);

					if( !me->get_object(ob, amount) )
					{
						msg("$ME�w�g������"+ob->short(1)+"�F�I�u�n����b�a�W�C\n", me, 0, 1);
						ob->move_to_environment(me);
					}
					if( me->query_skill_level("fuel") < 20 )
						me->add_skill_exp("fuel", amount*(12+random(11)));
					else if( me->query_skill_level("fuel") == 20 )
						tell(me, pnoun(2, me)+"���Ķ��ޯ൥�Ťw�g��F 20�A�L�k�A�ۧک��W���ɡA�󰪲`���ǰݥ����}�l��j�ǾǲߡC\n");

					me->add_social_exp(to_int(pow(amount*(20+random(21)), 0.9)));
					TREASURE_D->get_treasure_books(me, amount);
					return;
				}
			}
			return tell(me, "��y�������U�Ƹ귽���w�g�Q�j�q�}�ġA�L�k�A���i�Q�Ϊ��U�ơC\n");
			break;



		default:
			return tell(me, pnoun(2, me)+"�Q�n�������ح�ơH("HIW"���� metal"NOR", "WHT"��� stone"NOR", "HIC"�M�� water"NOR", "YEL"��� wood"NOR", "HIM"�U�� fuel"NOR")\n");
			break;
	}
}
