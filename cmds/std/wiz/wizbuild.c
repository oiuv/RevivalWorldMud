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
 
/*
�u                                                            �t
�U��  8. ���O   �a       11. �j��   �D        14. ����   ��   �U
�U    9. ���   �s       12. �h��   �b        15. ���O   �F   �U
�U�[  10. �G�O   �p      13. �D�F   ��        16. �J��   ��   �U
*/

#include <map.h>
#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <material.h>
#include <message.h>
#include <location.h>

inherit COMMAND;

string help = @HELP
        �з� build ���O�C
HELP;

string list = @LIST
�z                                                            �{
�U��  �ߢ��  �~�w�� ������ ��w�� �z�w�{ ������  6. ����  �� �U
�U    ��  ��  �x���x �x���x ������ �x���x ������  	      �U
�U��  ����  ���w�� ������ ���w�� �|�w�} ������  7. ���  �� �U
�u                                                            �t
�U8. �A��   9. �M��   0.���}                                  �U
�|                                                            �}
 �w�����п�J"quit"�w������ɫؿv�п�J"clean"�w
 
LIST;

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

mapping obj = ([
1	:	({ "�~","��","��","��","�w","�x" }),
2	:	({ "��","��","��","��","��","�x" }),
3	:	({ "��","��","��","��","�w","��" }),
4	:	({ "�z","�{","�|","�}","�w","�x" }),
5	:	({ "��","��","��","��","��","��" }),
6	:	"��",
7	:	"��",
8	:	ansi("$HIG$$BGRN$��$NOR$"),
]);

void build_wall(string pic, object me)
{
	array loc = query_temp("location", me);
	(MAP_D->query_map_system(loc))->set_coor_data(loc, TYPE, WALL);
	(MAP_D->query_map_system(loc))->set_coor_icon(loc, pic);
	tell(me, pnoun(2, me)+"�b�o���a�W�y�_�F�@��'"+pic+"'����C\n", CMDMSG);
}

/*
void build_scenery(string pic, object me)
{
	array loc = query_temp("location", me);
	(MAP_D->query_map_system(loc))->set_coor_data(loc[X], loc[Y], loc[CITY], loc[NUM], TYPE, SCENERY);
	(MAP_D->query_map_system(loc))->set_coor_icon(loc[X], loc[Y], loc[CITY], loc[NUM], pic);
	tell(me, pnoun(2, me)+"�b�o���a�W�سy�F'"+pic+"'���[�C\n", CMDMSG);
}
*/

void build_farm(string pic, object me)
{
	array loc = query_temp("location", me);
	(MAP_D->query_map_system(loc))->set_coor_data(loc, TYPE, FARM);
	(MAP_D->query_map_system(loc))->set_coor_icon(loc, pic);
	msg("$ME�b�o���a�W�p�g�A�ǳƧ@���A�ΥЦa�C\n",me,0,1);
	tell(me, pnoun(2, me)+"�i�H help here �ǲ߹A�Ъ��鷧��k�C\n", CMDMSG);
}
	
void build_door(string pic, object me)
{
	array loc = query_temp("location", me);
	(MAP_D->query_map_system(loc))->set_coor_data(loc, TYPE, DOOR);
	(MAP_D->query_map_system(loc))->set_coor_icon(loc, pic);
	tell(me, pnoun(2, me)+"�b�o���a�W�سy�@����'"+pic+"'�C\n", CMDMSG);
}

void clean_build(object me)
{
	array loc = query_temp("location", me);
	(MAP_D->query_map_system(loc))->delete_coor_data(loc, TYPE);
	(MAP_D->query_map_system(loc))->set_coor_icon(loc, "�D");
	tell(me, "�M���ؿv�����C\n", CMDMSG);
}

void select_icon(object me, string arg)
{
	int choice, type;
	mixed value;
	arg = lower_case(arg);
	
	if( sscanf( arg, "%d%s", choice, arg ) != 2 )
	{
		tell(me, list+"�п�J�C��W���^��μƦr�C<�سy����п�X�Ʀr�[�^�媺�զX�C�ҡG2a>\n:", CMDMSG);
		input_to((: select_icon, me :));
		return;
	}
	
	if( (value = obj[choice]) )
	{
		switch(choice)
		{
			case 1..5:
				if( arg=="" || !intp(type = wall_type[arg]) )
				{
					tell(me, list+"�سy����п�X�Ʀr�[�^�媺�զX�C�ҡG2a�C\n:", CMDMSG);
					input_to((: select_icon, me :));
					return;
				}
				build_wall(value[type], me);
				break;
			case 6..7:
				build_door(value, me);
				break;
			case 8:
				build_farm(value, me);
				break;
			case 9:
				clean_build(me);
				break;
			case 0:
				return tell(me, "�����ؿv���O�C\n", CMDMSG);
				break;
			default:
				tell(me, list+"\n�п�J�Ʀr���"+pnoun(2, me)+"�n�سy���F��C< �سy����п�X�Ʀr�[�^��C�ҡG2a >\n:", CMDMSG);
				input_to((: select_icon, me :));
		}
	}
}
	
private void command(object me, string arg)
{
	string owner;
	array loc = query_temp("location", me);

	if( !is_command() ) return;
	
	if( !arrayp(loc) || !environment(me)->is_maproom() ) 
		return tell(me, pnoun(2, me)+"����b�o�̫سy����δ��[�C\n", CMDMSG);
	
	owner = (MAP_D->query_map_system(loc))->query_coor_data(loc, "owner");
	
	/*
	if( owner != me->query_id(1) )
	{
		if( owner == "GOVERNMENT" )
		{
			if( CITY_D->query_city_info(environment(me)->query_city(), "official/mayor") != me->query_id(1) )
				return tell(me, "�o���a�O�����Φa�A"+pnoun(2, me)+"�S���v�Q�b�o�W���سy����F��C\n", CMDMSG);
		}
		else if( owner )
			return tell(me, "�o���a�O "+owner+" ���A"+pnoun(2, me)+"�S���v�Q�b�o�W���سy����F��C\n", CMDMSG);
		//else if( CITY_D->city_exist(loc[CITY]) )
		//	return tell(me, "�L�k�b���������N�سy�ؿv�A�����̷ӥ��`���ʶR�g�a�{�Ƕi��C\n", CMDMSG);
	}
	*/
	if( arg )
		return select_icon(me, arg);
		
	tell(me, list+"\n�п�J�Ʀr���"+pnoun(2, me)+"�n�سy���F��C< �سy����п�X�Ʀr�[�^��C�ҡG2a >\n:", CMDMSG);
	input_to((: select_icon, me :));
}
