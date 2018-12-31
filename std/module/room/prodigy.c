
/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : prodigy.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-07
 * Note   : �@�ɩ_�[�ʧ@�~�Ӫ���
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

inherit ROOM_ACTION_MOD;

void do_buff(object me, string arg)
{
	
	
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"room"	:
	([
		"short"	: HIR"�_�[����"NOR,
		"help"	:
			([
"topics":
@HELP
    �_�[�����i���ѹC�ȥ�͡C
HELP,

"buff":
@HELP
�\�]�íѪ����O�A�Ϊk�p�U�G
  ������
HELP,

			]),
		"action":
			([
				"buff"	: (: do_buff :),
			]),
	]),

]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIR"�@��"NOR RED"�_�["NOR

	// �}�i���ؿv�����̤֩ж�����
	,16

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,1

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,COMMERCE_REGION

	// �}�i�����O��
	,"300000000"
	
	// �ؿv���������ռаO
	,1

	// �c�a�^�m��
	,400

	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,1
	
	// �ؿv���ɥN
	,1
});

