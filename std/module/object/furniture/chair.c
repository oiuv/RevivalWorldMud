/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chair.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-17
 * Note   : �í� - �Ȥl
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <material.h>

inherit OBJECT_ACTION_MOD;

// ��
void do_sit(object me, string arg)
{
	
	
}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�|���غc�����C",
	"action":
		([
			"sit"	: (: do_sit :),
		]),
]);


// �]�w������
nosave array object_info = ({
	
	// �� name
	"�Ȥl"
	
	// �� id
	,"chair"

	// �򥻧��ƻݨD
	,([ WOOD:10 ])
	
	// �s�y�����~�һݭn����ޭ�
	,300
	
	// �򥻻s�y�t��(�C����u/�C�C���ɶ��@����)
	,30
	
	// �򥻻���
	,"300"
	
	// ��޾���
	,0
});
