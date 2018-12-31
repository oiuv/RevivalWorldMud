/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chicken_stud.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : ����
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
        set_idname("chicken stud","����");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "�Ψӥ�t�Ͳ����دb�C\n");
        set("unit", "��");
        set("mass", 1000);
        set("value", 500);
        set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		PASTURE,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�}�l�Ͳ��ɪ��T��
		"start_msg":"���s�̩B�B�B�a�s�ۡA�U�ۦY�ۭ��e���}�ơC\n" ,
			
	       	//�C���ݭn����ƺ����P�ƶq
	       	"material":
	       	([
	       		"/obj/materials/water":5,
	       		"/obj/farming/product/rice":5,
	       	]),
			
	       	//���~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/pasture/product/egg":50,
	       		"/obj/pasture/product/chicken":20,
	       	]),

		//�ͦ��ɪ��T��
		"harvest_msg":"$ME�w�ߦa�O�_�è��X�s�A�����סC\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"pasture":0,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"�i�J�F�@���e�N����t���A���������}�l���ۥ�t�C\n",
			50:	"�����̫�_��L�h���ͬ��A���i�H�`�N�쳡���������欰���Ǥ��P�C\n",
			100:	"�����������ͤU�F�X�����J�A�ö}�l��_�J�ӡC\n",
			200:	"�p���̤@�����a�}�J�ӥX�A�u���p���̤��_�a�s�s�s�A���G�ﲴ�e���s�@�ɥR���ۦn�_�ߡC\n",
			300:	"�p���v���������A�U���U���C\n",
			400:	"�p���̥������d�����j���������A�i�H�i�榬���F�C\n",
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
