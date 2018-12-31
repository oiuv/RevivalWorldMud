/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : corn seed.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �ɦ̺ؤl
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
	set_idname("corn seed","�ɦ̺ؤl");

	if( this_object()->set_shadow_ob() ) return;

	set("long","�㦳���@�f�������\���~�A�S�٬��f���B�ɸ����B�s���C\n");
	set("unit", "��");
	set("mass", 10);	// ���q
	set("value", 20);	// ����
	set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		FARM,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�������خɪ��T��
		"start_msg":"�ɦ̺ؤl����a�G������Цa�A�@���Y�Y�V�a���Pı�C\n" ,
			
	       	//�C���ؤl�ί��]�ݭn����Ƽƶq
	       	"material":
	       	([
	       		"/obj/materials/water":8,
	       	]),
			
	       	//���β��~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/farming/product/corn":60,
	       	]),

		//�������ήɪ��T��
		"harvest_msg":"$ME�w�ߦa���γo���������ɦ̥СC\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"farming":20,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"�ɦ̥ФW�p�p�����]�w�g�������j�F�C\n",
			50:	"�u���ɦ̥Ф@����N�s�M�A�R���F�ͮ�C\n",
			150:	"�ɦ̥ФW���ɦ̤w�g���������F�C\n",
			300:	"�u���@��������⪺�ɦ̥СA�w�g���ΦA��@�F�A�@�ߵ��@�}�l�K�i�H���ΡC\n",
			400:	"�o���A�ФW���ɦ̤w�g���������F�A�ש�i�H���ΤF�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo�ɦ̳���a�\��F�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo�@�ǥɦ̪w���G��F�C\n",
			TYPHOON:"�䭷��ŧ�ϱo�����ɦ̳Q�j���d�y���_�C\n",
			FOEHN:	"�I�����jŧ���������ɦֳ̧t���\���C\n",
		]),
	]));
}
