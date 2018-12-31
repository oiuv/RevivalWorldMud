/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : airport.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-21
 * Note   : ��ھ���
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <npc.h>
#include <condition.h>
#include <delay.h>

inherit ROOM_ACTION_MOD;


// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"toilet"	:
	([
		"short"	: HIY"���@�Z��"NOR,
		"help"	:
			([
"topics":
@HELP
    �����������@�Z�ҡC
read			�\Ū���y
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job": 0,
		"action":
			([
				
			]),
	
	]),	
	"platform"	:
	([
		"short"	: HIY"��x"NOR,
		"help"	:
			([
"topics":
@HELP
    �ȫȷf������x�C
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job": 0,
		"action":
			([

			]),
	
	]),	
	"lobby"	:
	([
		"short"	: HIY"����"NOR YEL"�j�U"NOR,
		"help"	:
			([
"topics":
@HELP
    �ʶR�������j�U�C
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job": 0,
		"action":
			([

			]),
	]),
]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIC"���"NOR CYN"����"NOR

	// �}�i���ؿv�����̤֩ж�����
	,12

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,COMMERCE_REGION

	// �}�i�����O��
	,"180000000"
	
	// �ؿv���������ռаO
	,1

	// �c�a�^�m��
	,120
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,3
});
