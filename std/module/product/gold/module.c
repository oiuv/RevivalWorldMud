/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : module.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>

inherit PRODUCT_ACTION_MOD;

// ��m���~
void do_action(object me, string arg)
{


}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�|���غc�����C",
	"action":
		([
			"action"	: (: do_action :),
		]),
]);

// �]�w�^�O���v�T
void setup_inlay(object product, mapping inlay)
{
	set("design/inlay", 1, product);
}

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"��",
	
	//�^��W��
	"id":		"gold",

	//���ƻݨD
	"material":	([ "/obj/materials/metal": 100, "gold":1 ]),

	//�ޯ�ݨD
	"skill":	([ "metalclassify":80, "technology":5 ]),

	//�s�y�Ӯ�(����)
	"timecost":	1,
	
	//�ӶO��ޭ�
	"technology":	100,
	
	//�ӶO��o�O��
	"cost":		"1000",

	//���~����
	"class":	"���ݭ쪫��",
	
	//�Ͳ��u�t
	"factory":	"metal",
	
	//����
	"value":	"1000",
	
	//���q(g)
	"mass":		100,
]);
