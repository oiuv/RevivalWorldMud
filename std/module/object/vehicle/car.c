/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : car.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-17
 * Note   : ���� - �p�Ȩ�
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <material.h>

inherit OBJECT_ACTION_MOD;

inherit VEHICLE_ACTION_MOD;

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	:
		([
"topics":
@HELP
    �����C
HELP,
		]),
	"action":
		([
			"enter"	: (: ::do_enter_action($1, $2, $3) :),
		]),
]);


// �]�w������
nosave array object_info = ({
	
	// �� name
	"�p�Ȩ�"
	
	// �� id
	,"car"

	// �򥻧��ƻݨD
	,([ GOLD : 100, IRON : 1000 ])
	
	// �s�y�����~�һݭn����ޭ�
	,300000
	
	// �򥻻s�y�t��(�C����u/�C�C���ɶ��@����)
	,500
	
	// �򥻻���
	,"500000"
	
	// ��޾���
	,0
});
