/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rice seedlings.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �_��
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <nature.h>
#include <map.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("rice seedling","�_��");

	if( this_object()->set_shadow_ob() ) return;

	set("long","�i�@���@�몺�D���C\n");
	set("unit", "��");
	set("mass", 10);	// ���q
	set("value", 3);	// ����
	set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		FARM,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�������خɪ��T��
		"start_msg":"�_������a�G������Цa�A�@���Y�Y�V�a���Pı�C\n" ,
			
	       	//�C���ؤl�ί��]�ݭn����Ƽƶq
	       	"material":
	       	([
	       		"/obj/materials/water":5,
	       	]),
			
	       	//���β��~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/farming/product/rice":50,
	       	]),

		//�������ήɪ��T��
		"harvest_msg":"$ME�w�ߦa���γo���������_�СC\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"farming":0,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"�_�ФW�p�p�����]�w�g�������j�F�C\n",
			50:	"�u���_�Ф@����N�s�M�A�R���F�ͮ�C\n",
			150:	"�_�ФW���_�̤w�g���������F�C\n",
			250:	"�u���@��������⪺�_�СA�w�g���ΦA��@�F�A�@�ߵ��@�}�l�K�i�H���ΡC\n",
			300:	"�o���A�ФW���_�̤w�g���������F�A�ש�i�H���ΤF�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo�_�@����a�\��F�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo�@�ǽ_�@�w���G��F�C\n",
			TYPHOON:"�䭷��ŧ�ϱo�����_�@�Q�j���d�y���_�C\n",
			FOEHN:	"�I�����jŧ���������_�@�ֳt���\���C\n",
		]),
	]));
}
