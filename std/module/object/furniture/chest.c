/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chest.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-17
 * Note   : �í� - �m���c
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <material.h>

inherit OBJECT_ACTION_MOD;

// ��m���~
void do_put(object me, string arg)
{
	
	
}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�|���غc�����C",
	"action":
		([
			"put"	: (: do_put :),
		]),
]);


// �]�w������
nosave array object_info = ({
	
	// �� name
	"�m���c"
	
	// �� id
	,"chest"

	// �򥻧��ƻݨD
	,([ IRON : 2, WOOD : 15 ])
	
	// �s�y�����~�һݭn����ޭ�
	,1500
	
	// �򥻻s�y�t��(�C����u/�C�C���ɶ��@����)
	,40
	
	// �򥻻���
	,"600"
	
	// ��޾���
	,({ ({"chair"}) })
});
