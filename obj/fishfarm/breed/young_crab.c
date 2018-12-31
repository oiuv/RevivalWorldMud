/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : young_crab.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-30
 * Note   : �p����
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
        set_idname("young crab","�p����");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "�p���ɡC\n");
        set("unit", "��");
        set("mass", 150);
        set("value", 4000);
        set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		FISHFARM,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�}�l�Ͳ��ɪ��T��
		"start_msg":"�p���ɦb������e���ʵۡA���y�����a�y�L�C\n" ,
			
	       	//�C���ݭn����ƺ����P�ƶq
	       	"material":
	       	([
	       		"/obj/materials/water":10,
	       		"/obj/materials/wood":20,
	       		"/obj/materials/stone":10,
	       	]),
			
	       	//���~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/fishfarm/product/big_crab":8,
	       	]),

		//�ͦ��ɪ��T��
		"harvest_msg":"$ME�w�ߦa���X�j�h�ɡC\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"fishfarm":70,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"�p���ɦb�����|�B�]�Ӷ]�h�C\n",
			100:	"�p���ɪ��Ⱖ�p�g�@���b�|�P��������C\n",
			300:	"�p���ɷU�ӷU�j�A�{�l�����a�D�F�_�ӡC\n",
			500:	"���ɪ��Ⱖ�g�U�ӷU�j�]�U�ӷU���O�C\n",
			800:	"���ɦ����ۨⰦ�¶ª��j���]�A�R�R�a�Ѥ������o�Y�ݡC\n",
			1200:	"�κ����j�h�ɤw�g������i�H�������{�פF�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo�j�h�ɭ̳��򪺭ᦺ�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo�������j�h�ɭ̳Q�j���R���C\n",
			TYPHOON:"�䭷��ŧ�ϱo�������j�h�ɭ̳Q�j���R���C\n",
			FOEHN:	"�I�����jŧ���������j�h�ɭ̦]���ŹL���Ӧ��C\n",
		]),
	]));
}
