/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : build
 * Author : Cookys@RevivalWorld
 * Date   : 2001-06-19
 * Note   : (�@�몱�a)�a�Ͻs����O, �u��سy����P�@�Ǥp���[�C
 * Update :
 *  o 2001-07-09 Clode ��g���O edit_map ���� build
 *
 -----------------------------------------
 */

#include <map.h>
#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <material.h>
#include <location.h>
#include <message.h>

#define ICON_ELEM		0
#define NAME_ELEM		1
#define MTR_ELEM		2

inherit COMMAND;


string list = @LIST
�z�w�s�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{
�U��U�ߢ��  �~�w�� ������ ��w�� �z�w�{ ������  6. ����  �� �U
�U  �U��  ��  �x���x �x���x ������ �x���x ������  	      �U
�U���U����  ���w�� ������ ���w�� �|�w�} ������  7. ���  �� �U
�u�w�q�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�t
�U���U10. ���O   [1;33m�F[m       11. ����   [1;36m��[m        12. �D�F   [1;37m��[m  �U
�U  �U13. �j��   [1;32m�D[m       14. ���   [0;32m�s[m        15. �J��   [1;35m��[m  �U
�U�[�U                                                        �U
�u�w�q�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�t
�U��U50. �A��   [1;32;42m��[m       51. ����   [1;33;43m��[m        52. �i�޳� [1;34;44m��[m  �U
�U  �U                                                        �U
�U�L�U                                                        �U
�|�w�r�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}
 (�ؿv���i����)       ls. �C�X����   er. ��ؿv  lv. �������}
LIST;

string help = @HELP

[0m
[0m �سy���O                                                             [build]
[0m������������������������������������������������������������������������������
[0m      ��A�g�ѨϥΤg�a�Ҧ��v���ӻ{��A�Ϊ̬O�Ѧۤv���W�ȨӪ������R�U�F�@����
[0m  ��ۤv���g�a�C�p�G�A���b�W���سy�Ǥ��򪺸ܡA�����a�û����u�O�@�ӳ�ª��Ŧa
[0m  �C�L�קA�O�n�إЩΪ̬O�}���A�@�w���o�سy�~�i�@�B�B�F���A�n���ت��C
[0m
[0m      ���U[33;1mbuild[0m���O����K�i�J�ؿv�Ҧ��A�|�X�{�p�U�e���G

HELP + list + @HELP

[0m  �D����G
[0m�@�@�@[36;1m�ߢ�ӢϢҢ���[0m�O������C
[0m
[0m        [36;1m��[0m��[32;1m���W[0m��[32m�ਤ[37m  [36;1m��[0m��[32;1m��[0;32m�u[37m  [36;1m��[0m��[32;1m�k�W[0m��[32m�ਤ
[0m        [36;1m��[0m��[32;1m��[0;32m�u[37m                  [36;1m��[0m��[32;1m��[0;32m�u
[0m        [36;1m��[0m��[32;1m���U[0m��[32m�ਤ[37m  [36;1m��[0m��[32;1m��[0;32m�u[37m  [36;1m��[0m��[32;1m�k�U[0m��[32m�ਤ
[0m
[0m      [36;1m����������[0m�O������˦��A�b���ܤW�w�����㻡���ϥܤF�C��쥲���n�P�˦��t
[0m      �X���@�աA���سy�C
[0m
[0m      [36;1m��[0m�M[36;1m��[0m�h�O������V�A�u���ؤJ�u[36;1m��[0m�v�Ρu[36;1m��[0m�v�N�i�H�سy�F�C
[0m
[0m      �t�~�A�Y�L���w�ا��h�|�سy�X�u[32m�ٯ�[37m�v���誺����C
[0m      �ϥΫا������O�榡��[33;1m2a by stone
[0m�@�@�@�@�@�@�@�@�@�@�@�@�@[33;1m6 by wood
[0m
[0m[33m      �������O: open
[0m
[0m�@�D���[�G
[0m�@�@�@10. ���O  [33;1m�F[0m�Хi�b�c�H�Ѯ𤤴����P�򪺯ਣ�סA�@��C
[0m      11. ����  [36;1m��[0m�Хi�ΨӱĶ�([33;1mcollect[0m) ���귽�A�@��C
[0m      12. �D�F  [1m��[0m�ХΨӥ�([33;1msit[0m) �A�ĪG�̨ΡA�@��C
[0m      13. �j��  [32;1m�D[0m�ХΨӥ�([33;1msit[0m) �A�ĪG���q�A�@��C
[0m      14. ���  [32m�s[37m�ХΨӥ�([33;1msit[0m) �A�ĪG���t�A�@��C
[0m      15. �J��  [35;1m��[0m�Ш㦳���_�]�O�A�i�O���|�P���A�СA�ϤѨa�ҳy�����l�������
[0m                  �C�C
[0m
[0m[33m      �������O: collect, sit
[0m
[0m  �D��L�G
[0m      50. �سy�A�Сu[32;42;1m��[0m�v
[0m      51. �سy�����u[33;43;1m��[0m�v
[0m      52. �سy�i�޳��u[34;44;1m��[0m�v
[0m
[0m[33m      �������O: grow
[0m
[0m    [33;1mls[0m�ЦC�X�ؿv���ܹϡC
[0m    [33;1mer[0m�бN����ؿv��A�~�i�H��X�Ϋسy��L�ؿv���C
[0m    [33;1mlv[0m�����}�ؿv�Ҧ��C
[0m
[0m[33m      �������O: sell here
[0m������������������������������������������������������������������������������
[0m                                                            �������s���@��[0m
HELP;

mapping wall_type = ([
"q"	:	0,
"e"	:	1,
"z"	:	2,
"c"	:	3,
"w"	:	4,
"x"	:	4,
"a"	:	5,
"d"	:	5,
]);

mapping walls = ([
1	:	({ "�~","��","��","��","�w","�x" }),
2	:	({ "��","��","��","��","��","�x" }),
3	:	({ "��","��","��","��","�w","��" }),
4	:	({ "�z","�{","�|","�}","�w","�x" }),
5	:	({ "��","��","��","��","��","��" }),
]);

mapping doors = ([
6	:	({ "��", 	"����" }),
7	:	({ "��", 	"���" }),
]);

mapping sceneries = ([
10	:	({ HIY"�F"NOR, 		"���O", ([METAL	:20, STONE:10, WOOD:5	]) }),
11	:	({ HIC"��"NOR, 		"����", ([WATER	:50, STONE:20		]) }),
12	:	({ HIW"��"NOR, 		"�D�F", ([WOOD	:15, STONE:15		]) }),
13	:	({ HIG"�D"NOR, 		"�j��", ([WATER	:30, WOOD:20		]) }),
14	:	({ NOR GRN"�s"NOR, 	"���", ([WATER	:15, WOOD:10		]) }),
15	:	({ HIM"��"NOR, 		"�J��", ([STONE	:40, METAL:20		]) }),
]);

mapping others = ([
50	:	({ HIG BGRN"��"NOR,	"�A��",	([WOOD:20			]) }),
51	:	({ HIY BYEL"��"NOR,	"����",	([WOOD:20			]) }),
52	:	({ HIB BBLU"��"NOR,	"�i�޳�",([WOOD:20			]) }),
]);
void select_icon(object me, string arg);


void do_build(object me, object material, array building_info, int type)
{

	int str_cost;
	int num;
	string mtr_name;
	mapping mtr_exerted = allocate_mapping(0);
	mapping mtr_ingredient;
	array loc = query_temp("location", me);


	if( CITY_D->query_coor_data(loc, TYPE) )
	{
		tell(me, "�o���g�a�W�w�g���ت��F�A�L�k�A���سy�A���D"+pnoun(2, me)+"������{�����ت��C\n");
		tell(me, "\n��J�Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");
		input_to((: select_icon, me :));
		return;
	}

	if( !objectp(material) )
	{
		if( !mapp(building_info[MTR_ELEM]) )
		{
			if( !me->cost_stamina(300) )
			{
				tell(me, "\n"+pnoun(2, me)+"�w�g�S����O�سy�F��F�C\n");
				return me->finish_input();
			}
			
			CITY_D->set_coor_data(loc, TYPE, type);
			CITY_D->set_coor_icon(loc, building_info[ICON_ELEM]);
		
			// �R�� material ��T
			CITY_D->delete_coor_data(loc, "material");

			tell(me, "�ӶO�F 300 �I��O�A"HIG"["+me->query_stamina_cur()+"/"+me->query_stamina_max()+"]"NOR"�C\n");
			msg("$ME���\�a�b�o���g�a�W�سy�F�u"+building_info[NAME_ELEM]+" "+building_info[ICON_ELEM]+"�v�C\n",me,0,1);
			me->add_social_exp(30+random(51));
			
			tell(me, "\n��J�Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");
			input_to((: select_icon, me :));
			return;
		}
		else
		{
			foreach( mtr_name, num in building_info[MTR_ELEM] )
				mtr_exerted[mtr_name] = CITY_D->query_coor_data(loc, "material/"+mtr_name);
	
			tell(me, "���Ƥ����A�سy�u"+building_info[NAME_ELEM]+" "+building_info[ICON_ELEM]+"�v�ݭn"+MATERIAL_D->material_required_info(building_info[MTR_ELEM])+"�A�ثe������ "+MATERIAL_D->material_percentage(building_info[MTR_ELEM], mtr_exerted)+"�C\n\n");
			return me->finish_input();
		}
	}
	else
		mtr_ingredient = query("material", material);

	if( !mapp(mtr_ingredient) )
	{
		tell(me, material->query_idname()+"�w�g�l�a�F�C\n");
		return me->finish_input();
	}

	if( sizeof(keys(mtr_ingredient) - keys(building_info[MTR_ELEM])) == sizeof(keys(mtr_ingredient)) )
	{
		tell(me, material->query_idname()+"���A�X�Ψӫسy���ؿv���C\n");
		return me->finish_input();
	}
		
	foreach( mtr_name, num in building_info[MTR_ELEM] )
	{
		if( !me->cost_stamina(mtr_ingredient[mtr_name]*50) )
		{
			tell(me, "\n"+pnoun(2, me)+"�w�g�S����O�سy�F��F�C\n");
			return me->finish_input();
		}
		
		mtr_exerted[mtr_name] = CITY_D->add_coor_data(loc, "material/"+mtr_name, mtr_ingredient[mtr_name]);

		// �C��쪺���Ʒ|�ӶO 50 �I��O
		str_cost += mtr_ingredient[mtr_name]*50;
	}

	tell(me, "�ӶO�F "+str_cost+" �I��O�A"HIG"["+me->query_stamina_cur()+"/"+me->query_stamina_max()+"]"NOR"�C\n");

	me->add_social_exp(str_cost/10+random(31));
	msg("$ME�Q��"+material->query_idname()+"�@�����Ʀb�o���g�a�W�سy�u"+building_info[NAME_ELEM]+" "+building_info[ICON_ELEM]+"�v�A�ثe�w�����G"+MATERIAL_D->material_percentage(building_info[MTR_ELEM], mtr_exerted)+"�C\n",me,0,1);
	
	destruct(material ,1);

	if( MATERIAL_D->is_accomplished(building_info[MTR_ELEM], mtr_exerted) )
	{
		msg("\n$ME�ҫسy���u"+building_info[NAME_ELEM]+" "+building_info[ICON_ELEM]+"�v���u�F�A�Фj�a��⹪�y���y�I\n", me, 0, 1);

		BUILDING_D->materialize_outdoor_building(me, CITY_D->query_coor_data(loc, "owner"), loc, building_info[ICON_ELEM], building_info[NAME_ELEM], type);	
		
		me->finish_input();
		return;
	}

	tell(me, "\n��J�Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");
	input_to((: select_icon, me :));
	return;
}

// �ؿv���
void build_wall(object me, object material, int choice, int wall_type)
{
	if( !objectp(material) )
		do_build(me, material, ({ GRN+walls[choice][wall_type]+NOR, "�T�����", 0}), WALL );
	else
	{
		mapping mtr_ingredient = query("material", material);
		
		if( mapp(mtr_ingredient) && mtr_ingredient[WOOD] )
			do_build(me, material, ({ NOR YEL+walls[choice][wall_type]+NOR, "��O���", ([ WOOD : 10 ]) }), WALL );
		else if( mapp(mtr_ingredient) && mtr_ingredient[STONE] )
			do_build(me, material, ({ NOR +walls[choice][wall_type]+NOR, "�ۧ����", ([ STONE : 30 ]) }), WALL );
		else
		{
			tell(me, material->query_idname()+"�ä��A�X�@��������ا��C\n\n");
			me->finish_input();
		}
	}
}

// �ؿv�j��
void build_door(object me, object material, int choice)
{
	if( !objectp(material) )
		do_build(me, material, ({ GRN+doors[choice][0]+NOR, doors[choice][1], 0}), DOOR );
	else
	{
		mapping mtr_ingredient = query("material", material);

		if( mapp(mtr_ingredient) && mtr_ingredient[WOOD] )
			do_build(me, material, ({ NOR YEL+doors[choice][0]+NOR, doors[choice][1], ([ WOOD : 10 ]) }), DOOR );
		else if( mapp(mtr_ingredient) && mtr_ingredient[STONE] )
			do_build(me, material, ({ NOR +doors[choice][0]+NOR, doors[choice][1], ([ STONE : 30 ]) }), DOOR );
		else
		{
			tell(me, material->query_idname()+"�ä��A�X�@�������ا��C\n\n");
			me->finish_input();
		}
	}
}

// �ؿv���[
void build_scenery(object me, object material, int choice)
{
	string my_id = me->query_id(1);
	string loc_owner = CITY_D->query_coor_data(query_temp("location", me), "owner");

	// ���O�u���F����\
	if( choice == 10 )
	{
		if( !belong_to_government(loc_owner) )
			return tell(me, "�o�ش��[�u��سy�b�F���g�a�W�C\n");
	}
	else if( loc_owner != my_id && !ENTERPRISE_D->same_enterprise(loc_owner, my_id) )
	{
		tell(me, "�o���a����سy�o�������[�C\n");
		return me->finish_input();
	}

	switch(choice)
	{
		case 10:
			do_build(me, material, sceneries[choice], LIGHT);
			break;
		case 11:
			do_build(me, material, sceneries[choice], POOL);
			break;
		case 12:
			do_build(me, material, sceneries[choice], PAVILION);
			break;
		case 13:
			do_build(me, material, sceneries[choice], TREE);
			break;
		case 14:
			do_build(me, material, sceneries[choice], GRASS);
			break;
		case 15:
			do_build(me, material, sceneries[choice], STATUE);
			break;
		default:
			tell(me, list+"\n��J�Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");
			input_to((: select_icon, me :));
			return;
	}
			
}

// �سy��L�S��ؿv
void build_other(object me, object material, int choice)
{
	string my_id = me->query_id(1);
	string loc_owner = CITY_D->query_coor_data(query_temp("location", me), "owner");

	if( loc_owner != my_id && !ENTERPRISE_D->same_enterprise(loc_owner, my_id) )
	{
		tell(me, "�o���a����سy�o�����ؿv���C\n");
		return me->finish_input();
	}
	
	switch(choice)
	{
		// �A��
		case 50:
			if( !(CITY_D->query_coor_data(query_temp("location", me), "region") & AGRICULTURE_REGION) )
				return tell(me, "���a�ä��O�A�~�ϡA�L�k�سy�A�СC\n");

			do_build(me, material, others[choice], FARM);
			return;
		// ����
		case 51:
			if( !(CITY_D->query_coor_data(query_temp("location", me), "region") & AGRICULTURE_REGION) )
				return tell(me, "���a�ä��O�A�~�ϡA�L�k�سy�����C\n");

			do_build(me, material, others[choice], PASTURE);
			return;
		// �i�޳�
		case 52:
			if( !(CITY_D->query_coor_data(query_temp("location", me), "region") & AGRICULTURE_REGION) )
				return tell(me, "���a�ä��O�A�~�ϡA�L�k�سy�i�޳��C\n");

			do_build(me, material, others[choice], FISHFARM);
			return;
		default:
			tell(me, list+"\n��J�Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");
			input_to((: select_icon, me :));
			return;
	}
}
		
		

void clean_build(object me)
{
	array loc = query_temp("location", me);
	
	if( !CITY_D->query_coor_data(loc, TYPE) )
	{
		tell(me, "�o���g�a�W�èS������ؿv�A���ݭn��C\n");
		
		return me->finish_input();
	}

	if( !me->cost_stamina(300) )
	{
		tell(me, pnoun(2, me)+"�w�g��ݼN�N�A�S����O��o�ǪF��F�C\n");
		return me->finish_input();
	}

	msg("$ME�N�سy�n���u"+CITY_D->query_coor_icon(loc)+"�v����F�C\n", me, 0, 1);
	tell(me, "�ӶO�F 300 �I��O�A"HIG"["+me->query_stamina_cur()+"/"+me->query_stamina_max()+"]"NOR"�C\n");

	CITY_D->delete_coor_data(loc, "growth");
	CITY_D->delete_coor_data(loc, "status");
	CITY_D->delete_coor_data(loc, TYPE);
	CITY_D->set_coor_icon(loc, BWHT"��"NOR);
	
	ESTATE_D->set_land_estate(CITY_D->query_coor_data(loc, "owner"), loc);
	
	tell(me, "\n��J�Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");
	input_to((: select_icon, me  :));
	return;
}

mapping move_string = ([
    "n":        "north",
    "e":        "east",
    "w":        "west",
    "s":        "south",
    "u":        "up",
    "d":        "down",
    "nu":       "northup",
    "eu":       "eastup",
    "wu":       "westup",
    "su":       "southup",
    "nd":       "northdown",
    "ed":       "eastdown",
    "wd":       "westdown",
    "sd":       "southdown",
    "ne":       "northeast",
    "se":       "southeast",
    "nw":       "northwest",
    "sw":       "southwest",
]);
void select_icon(object me, string arg)
{
	int choice;
	string owner;
	array loc = query_temp("location", me);
	
	string material_id;
	object material;

	if( !arg || !arg[0] )
	{
		tell(me, "\n��J�Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");
		input_to((: select_icon, me :));
		return;
	}
	
	if( !arrayp(loc) || !environment(me)->is_maproom() ) 
	{
		tell(me, pnoun(2, me)+"����b�o�̫سy����ؿv���C\n");
		return me->finish_input();
	}

	owner = CITY_D->query_coor_data(loc, "owner");
	
	if( !owner ) 
	{
		tell(me, "�o�O�@�����ݩ����H���g�a�A"+pnoun(2, me)+"�������R�U�o���a�~��b�W���سy�ؿv���C\n");
		return me->finish_input();
	}

	if( belong_to_government(owner) )
	{
		if( !CITY_D->is_mayor_or_officer(loc[CITY], me) )
		{
			tell(me, "�o���a�O���F���Φa�A"+pnoun(2, me)+"�S���v�Q�b�o�W���سy����F��C\n");
			return me->finish_input();
		}
	}
	else if( belong_to_enterprise(owner) )
	{
		if( owner[11..] != query("enterprise", me) )
		{
			tell(me, pnoun(2, me)+"���O"+ENTERPRISE_D->query_enterprise_color_id(query("enterprise", me))+"���~���Ϊ������A�L�k�b���سy����F��C\n");
			return me->finish_input();
		}
	}
	else if( owner != me->query_id(1) && !ENTERPRISE_D->same_enterprise(owner, me->query_id(1)) ) 
	{
		tell(me, "�o���a�O "+capitalize(owner)+" ���p�H�g�a�A"+pnoun(2, me)+"�S���v�Q�b�o�W���سy����F��C\n");
		return me->finish_input();
	}

	foreach( string coor, mixed data in CITY_D->query_coor_range(loc, ROOM, 1) )
                if( data )
                {
                        tell(me, pnoun(2, me)+"�����������o�ɫؿv���~�୫�s�سy�C\n");
                        return me->finish_input();
		}
	
	if( ESTATE_D->query_loc_estate(loc) && ESTATE_D->query_loc_estate(loc)["type"] != "land" )
	{
		tell(me, pnoun(2, me)+"�����������o�ɫؿv���~�୫�s�سy�C\n");
                return me->finish_input();
	}
		
	arg = lower_case(arg);

	switch(arg)
	{
		case "lv":
		case "leave":
		{
			tell(me, "�����سy�ʧ@�C\n");
			return me->finish_input();
		}
		case "l":
		case "look":
		{
			tell(me,MAP_D->show_map(query_temp("location", me), 0) || "�a�Ϩt�ο��~�A�гq���Ův�B�z�C\n");
			tell(me, "\n��J�Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");
			input_to((: select_icon, me :));
			return;
		}
		case "ls":
		case "list":
		{
			tell(me, list+"\n��J�Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");
			input_to((: select_icon, me :));
			return;
		}
		case "er":
		case "erase":
		{
			evaluate((: clean_build, me :));
			return;
		}
	}

	if( sscanf( arg, "%d%s", choice, arg ) != 2 )
	{
		// ��J��V���O ?
		if( member_array(arg, keys(move_string)+values(move_string))!=-1)
		{
			if( member_array(arg, keys(move_string)) != -1 )
				arg = move_string[arg];

			if( MAP_D->valid_move(me, arg) <= 0  )
			{
				tell(me, "�L�k���Ӥ�V�e�i�C\n");
				tell(me, "\n��J�Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");
				input_to((:select_icon, me:));
				return;
			}
				
			CITY_D->move(me, arg);
		} 
		
		tell(me, "\n��J�Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");
		input_to((: select_icon, me :));
		return;
	}
	
	// �ѪR���ƪ���
	if( sscanf(arg, "%s by %s", arg, material_id) == 2 )
	{
		if( !objectp(material = present(material_id, me) || present(material_id, environment(me))) )
		{
			tell(me, pnoun(2, me)+"������èS�� "+material_id+" �o�ӪF��C\n");
			tell(me, "\n��J�Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");
			input_to((: select_icon, me :));
			return;
		}
	}

	switch(choice)
	{
		// ���
		case 1..5:
			if( arg=="" || nullp(wall_type[arg]) )
			{
				tell(me, "\n��J���T���Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");
				input_to((: select_icon, me :));
				return;
			}
			evaluate((: build_wall, me, material, choice, wall_type[arg] :));
			break;
		// ��
		case 6..7:
			evaluate((: build_door, me, material, choice :));
			break;
		// ���[�w�d�d��
		case 10..49:
			evaluate((: build_scenery, me, material, choice :));
			break;
		// ��L�ؿv�w�d�d��
		case 50..99:
			evaluate((: build_other, me, material, choice :));
			break;

		default:
			tell(me, list+"\n��J�Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");
			input_to((: select_icon, me :));
			return;
	}
}

private void do_command(object me, string arg)
{
	string owner;
	array loc = query_temp("location", me);

	if( !arrayp(loc) || !CITY_D->is_city_location(loc) )
		return tell(me, pnoun(2, me)+"�u��b�����a�Ϥ��سy�ؿv���C\n");

	owner = CITY_D->query_coor_data(loc, "owner");
	
	if( !owner ) 
		return tell(me, "�o�O�@�����ݩ����H���g�a�A"+pnoun(2, me)+"�������R�U�o���a�~��b�W���سy�ؿv���C\n");

	if( belong_to_government(owner) )
	{
		if( !CITY_D->is_mayor_or_officer(loc[CITY], me) )
			return tell(me, "�o���a�O���F���Φa�A"+pnoun(2, me)+"�S���v�Q�b�o�W���سy����F��C\n");
	}
	else if( belong_to_enterprise(owner) )
	{
		if( owner[11..] != query("enterprise", me) )
			return tell(me, pnoun(2, me)+"���O"+ENTERPRISE_D->query_enterprise_color_id(query("enterprise", me))+"���~���Ϊ������A�L�k�b���سy����F��C\n");
	}
	else if( owner != me->query_id(1) && !ENTERPRISE_D->same_enterprise(owner, me->query_id(1)) ) 
		return tell(me, "�o���a�O "+capitalize(owner)+" ���p�H�g�a�A"+pnoun(2, me)+"�S���v�Q�b�o�W���سy����F��C\n");

	if( arg )
		return select_icon(me, arg);
	
	tell(me, list+"\n��J�Ʀr�ﶵ�P�ϥΪ�����(�� 2a by stone)�A"HIW"ls"NOR" ��ܦC��G\n");

	input_to((: select_icon, me :));
}
