/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sunflower_seed.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �V�鸪�ؤl
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
        set_idname("sunflower seed","�V�鸪�ؤl");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long","�V�鸪��y�G�R�}�E����E���ۡE�L�q�E�㻮�C\n");
        set("unit", "��");
        set("mass", 2);
        set("value", 80);
        set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		FARM,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�������خɪ��T��
		"start_msg":"�V�鸪�ؤl����a�G������Цa�A�@���Y�Y�V�a���Pı�C\n" ,
			
	       	//�C���ؤl�ί��]�ݭn����Ƽƶq
	       	"material":
	       	([
	       		"/obj/materials/water":10,
	       	]),
			
	       	//���β��~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/farming/product/sunflower":60,
	       	]),

		//�������ήɪ��T��
		"harvest_msg":"$ME�w�ߦa���γo���������V�鸪�СC\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"farming":60,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"�V�鸪�ؤl�ޥX�@�ڮڹ�޺����ͪ��C\n",
			50:	"�V�鸪���t�C�C�ͪ��X�����ý��A�W�����񸭤@�@�ήi�}�ӡC\n",
			150:	"�V�鸪�Ы_�X�F�\�h�V�鸪��c�A�A�L���[�N�|�}��F�C\n",
			350:	"�b�����U�@���������{�{���V�鸪���������A�A���@�U�l�N�i�H�Ħ��F�C\n",
			400:	"�V�鸪�ФW���V�鸪�w�������A�w�g�i�H�Ħ��F�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo�V�鸪����a�\��F�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo�@�ǦV�鸪�w���G��F�C\n",
			TYPHOON:"�䭷��ŧ�ϱo�����V�鸪�Q�j���d�y���_�C\n",
			FOEHN:	"�I�����jŧ���������V�鸪�ֳt���\���C\n",
		]),
	]));
}
