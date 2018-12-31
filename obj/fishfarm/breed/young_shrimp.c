/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : young_shrimp.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-30
 * Note   : �p��
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
        set_idname("young shrimp","�p��");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "�p���C\n");
        set("unit", "��");
        set("mass", 100);
        set("value", 1000);
        set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		FISHFARM,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�}�l�Ͳ��ɪ��T��
		"start_msg":"�p���|�B�娫�ۡA���y�����a�y�L�C\n" ,
			
	       	//�C���ݭn����ƺ����P�ƶq
	       	"material":
	       	([
	       		"/obj/materials/water":10,
	       		"/obj/materials/wood":10,
	       	]),
			
	       	//���~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/fishfarm/product/big_shrimp":12,
	       	]),

		//�ͦ��ɪ��T��
		"harvest_msg":"$ME�w�ߦa���X�󽼡C\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"fishfarm":40,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"�p���b�����|�B��Ӵ�h�C\n",
			100:	"�p���b�z��������b���󤤧ֳt������C\n",
			240:	"�p���U�ӷU�j�A������]�����]�X�C��F�C\n",
			360:	"�󽼪���Ƹ}�ֳt���\�ʵۡA�b�����ֳt��ʡC\n",
			540:	"�󽼦Y�o�������ݦb�����R�R�a�𮧡C\n",
			720:	"�κ����󽼤w�g������i�H�������{�פF�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo�󽼭̳��򪺭ᦺ�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo�������󽼭̳Q�j���R���C\n",
			TYPHOON:"�䭷��ŧ�ϱo�������󽼭̳Q�j���R���C\n",
			FOEHN:	"�I�����jŧ���������󽼭̦]���ŹL���Ӧ��C\n",
		]),
	]));
}
