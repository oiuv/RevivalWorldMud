/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : horse_stud.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �ذ�
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
        set_idname("horse stud","�ذ�");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "�Ψӥ�t�Ͳ����دb�C\n");
        set("unit", "��");
        set("mass", 5000);
        set("value", 4000);
        set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		PASTURE,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�}�l�Ͳ��ɪ��T��
		"start_msg":"�������ذ��s�|�B���D�b�]�A�R�����O�P�®�C\n" ,
			
	       	//�C���ݭn����ƺ����P�ƶq
	       	"material":
	       	([
	       		"/obj/materials/water":4,
	       		"/obj/farming/product/herbage":15,
	       	]),
			
	       	//���~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/pasture/product/horseflesh":7,
	       	]),

		//�ͦ��ɪ��T��
		"harvest_msg":"$ME�w�ߦa�O�_�è��X�s�A�����סC\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"pasture":50,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"�i�J�F�@���e�N����t���A�ذ������}�l���ۥ�t�C\n",
			50:	"�ذ��̫�_��L�h���ͬ��A���i�H�`�N�쳡���������欰���Ǥ��P�C\n",
			150:	"�����������{�l�����j�F�_�ӡA���G�O�h���F�C\n",
			300:	"�����̤��Y���U�F�\�h���d���p���A�u�������̲Ӥߪ���¶�b�p�����ǨûQ���e�̪�����C\n",
			500:	"�p���v���������A�U���U���C\n",
			700:	"�p���̥������d�����j���������A�i�H�i�榬���F�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo���s�̳��򪺭ᦺ�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo���������s�̨��D�f���C\n",
			TYPHOON:"�䭷��ŧ�ϱo���������s�̳Q�j���j�Ӫ����Y�{���C\n",
			FOEHN:	"�I�����jŧ�����������s�̹L�������Ӧ��C\n",
		]),
	]));
}
