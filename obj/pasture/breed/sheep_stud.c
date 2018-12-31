/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sheep_stud.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �ئ�
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
        set_idname("sheep stud","�ئ�");
        
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
		"start_msg":"�������ئϸs�|�B���B�M���Ϊ���A�R�����O�P�®�C\n" ,
			
	       	//�C���ݭn����ƺ����P�ƶq
	       	"material":
	       	([
	       		"/obj/materials/water":10,
	       		"/obj/farming/product/herbage":15,
	       	]),
			
	       	//���~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/pasture/product/mutton":10,
	       		"/obj/pasture/product/wool":3,
	       	]),

		//�ͦ��ɪ��T��
		"harvest_msg":"$ME�w�ߦa�O�_�è��X�s�A���ϦסC\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"pasture":40,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"�i�J�F�@���e�N����t���A�ئϤ����}�l���ۥ�t�C\n",
			50:	"�ئϭ̫�_��L�h���ͬ��A���i�H�`�N�쳡�������Ϧ欰���Ǥ��P�C\n",
			150:	"���������Ϩ{�l�����j�F�_�ӡA���G�O�h���F�C\n",
			300:	"���ϭ̤��Y���U�F�\�h���d���p�ϡA�u���ئϭ̲Ӥߪ���¶�b�p�Ϩ��ǨûQ���e�̪�����C\n",
			600:	"�p�ϳv���������A�U���U���C\n",
			800:	"�p�ϭ̥������d�����j�������ϡA�i�H�i�榬���F�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo�ϸs�̳��򪺭ᦺ�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo�������ϸs�̨��D�f���C\n",
			TYPHOON:"�䭷��ŧ�ϱo�������ϸs�̳Q�j���j�Ӫ����Y�{���C\n",
			FOEHN:	"�I�����jŧ���������ϸs�̹L�������Ӧ��C\n",
		]),
	]));
}
