/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : space.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-24
 * Note   : �Ӫ��`�p
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


// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"jail"	:
	([
		"short"	: HIB"�Ӫ�"NOR BLU"�`�p"NOR,
		"help"	:
			([
"topics":
@HELP
    �Ψӥ}�T�ǤH���a��C
HELP,

"arrest":
@HELP
�e���Y�쪱�a�A�Ϊk�p�U�G
  arrest '���aID'		- �N�Y��u�W�����e��(�������O)
HELP,

"release":
@HELP
����Y�쪱�a�A�Ϊk�p�U�G
  release '���aID'		- �N�Y��Q���b�ʺ�������������(�����P�x�����O)
HELP,

			]),
		"action":
			([

			]),
	]),
]);


// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIB"�Ӫ�"NOR BLU"�`�p"NOR

	// �}�i���ؿv�����̤֩ж�����
	,10

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,INDUSTRY_REGION

	// �}�i�����O��
	,"10000000000"
	
	// �ؿv���������ռаO
	,1

	// �c�a�^�m��
	,500
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,4
});

