/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pig_stud.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �ؽ�
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
        set_idname("pig stud","�ؽ�");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "�Ψӥ�t�Ͳ����دb�C\n");
        set("unit", "��");
        set("mass", 5000);
        set("value", 1000);
        set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		PASTURE,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�}�l�Ͳ��ɪ��T��
		"start_msg":"�D�ʹʪ��ؽެݨ�}�ƴN�ĹL�h�g�Y�A�Y�o���L���O�}�ơC\n" ,
			
	       	//�C���ݭn����ƺ����P�ƶq
	       	"material":
	       	([
	       		"/obj/materials/water":5,
	       		"/obj/farming/product/corn":5,
	       		"/obj/farming/product/rice":5,
	       	]),
			
	       	//���~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/pasture/product/pork":25,
	       	]),

		//�ͦ��ɪ��T��
		"harvest_msg":"$ME�w�ߦa�O�_�è��X�s�A���ަסC\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"pasture":20,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"�i�J�F�@���e�N����t���A�ؽޤ����}�l���ۥ�t�C\n",
			50:	"�ؽޭ̫�_��L�h���ͬ��A���i�H�`�N�쳡�������ަ欰���Ǥ��P�C\n",
			150:	"���������ި{�l�����j�F�_�ӡA���G�O�h���F�C\n",
			300:	"���ޭ̤��Y���U�F�\�h���d���p�ޡA�u�����ޭ̲Ӥߪ���¶�b�p�ި��ǨûQ���e�̪�����C\n",
			500:	"�p�޳v���������A�U���U���C\n",
			600:	"�p�ޭ̥������d�����j�������ޡA�i�H�i�榬���F�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo�޸s�̳��򪺭ᦺ�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo�������޸s�̨��D�f���C\n",
			TYPHOON:"�䭷��ŧ�ϱo�������޸s�̳Q�j���j�Ӫ����Y�{���C\n",
			FOEHN:	"�I�����jŧ���������޸s�̹L�������Ӧ��C\n",
		]),
	]));
}
