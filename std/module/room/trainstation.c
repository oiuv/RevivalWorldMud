/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : trainstation.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-11
 * Note   : ������
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

// OGC
void do_ogc(object me, string arg)
{
	//object env = environment(me);

	if( me->is_delaying() )
		return tell(me, me->query_delay_msg());

        msg("$ME�q�f�U���X�_�Ǫ��D��A�}�l���_�_�Ǫ��ʧ@...��...�@...�z...��...�C�C\n", me, 0, 1);
        
        me->faint();
}

// �ʶR����
void do_buy(object me, string arg)
{
	
	
	
	
}

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
				"ogc"	:	(: do_ogc :),
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
				"buy"	:	(: do_buy :),
			]),
	]),
]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIY"������"NOR

	// �}�i���ؿv�����̤֩ж�����
	,6

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,COMMERCE_REGION

	// �}�i�����O��
	,"50000000"
	
	// �ؿv���������ռаO
	,1

	// �c�a�^�m��
	,40
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,2
});
