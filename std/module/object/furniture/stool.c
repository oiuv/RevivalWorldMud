/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : stool.c
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
	"����"
	
	// �� id
	,"stool"

	// �򥻧��ƻݨD
	,([ GOLD:1000, IRON:1000 ])
	
	// �s�y�����~�һݭn����ޭ�
	,1000000
	
	// �򥻻s�y�t��(�C����u/�C�C���ɶ��@����)
	,400
	
	// �򥻻���
	,"20000000"
	
	// ��޾���
	,({ ({"chair"}), ({"chest"}) })
});
