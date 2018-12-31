/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : lily_seed.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �ʦX�ؤl
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
        set_idname("lily seed","�ʦX�ؤl");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "�ʦX���y�G�¼�E�¼䪺�ߡE�ʦ~�n�X�E���j���R�C\n");
        set("unit", "��");
        set("mass", 2);
        set("value", 200);
        set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		FARM,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�������خɪ��T��
		"start_msg":"�ʦX�ؤl����a�G������Цa�A�@���Y�Y�V�a���Pı�C\n" ,
			
	       	//�C���ؤl�ί��]�ݭn����Ƽƶq
	       	"material":
	       	([
	       		"/obj/materials/water":15,
	       	]),
			
	       	//���β��~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/farming/product/lily":55,
	       	]),

		//�������ήɪ��T��
		"harvest_msg":"$ME�w�ߦa���γo���������ʦX��СC\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"farming":80,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"�ʦX�ؤl�ޥX�@�ڮڹ�޺����ͪ��C\n",
			50:	"�ʦX���t�C�C�ͪ��X�����ý��A�W�����񸭤@�@�ήi�}�ӡC\n",
			150:	"�ʦX�Ы_�X�F�\�h�ʦX��c�A�A�L���[�N�|�}��F�C\n",
			350:	"��}���զ�ʦX��b�񸭪��MŨ���U�t��L��A�A���@�U�l�N�i�H�Ħ��F�C\n",
			600:	"�ʦX��ФW���ʦX�w�������A�w�g�i�H�Ħ��F�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo�ʦX����a�\��F�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo�@�ǦʦX�w���G��F�C\n",
			TYPHOON:"�䭷��ŧ�ϱo�����ʦX�Q�j���d�y���_�C\n",
			FOEHN:	"�I�����jŧ���������ʦX�ֳt���\���C\n",
		]),
	]));
}
