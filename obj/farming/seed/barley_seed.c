/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : barley_seed.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �j���ؤl
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
	set_idname("barley seed","�j���ؤl");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long","�i�Ψ��C�s�Χ@�����ֺ����~�C\n");
	set("unit", "��");
	set("mass", 10);
	set("value", 10);
	set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		FARM,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�������خɪ��T��
		"start_msg":"�j���ؤl����a�G������Цa�A�@���Y�Y�V�a���Pı�C\n" ,
			
	       	//�C���ؤl�ί��]�ݭn����Ƽƶq
	       	"material":
	       	([
	       		"/obj/materials/water":6,
	       	]),
			
	       	//���β��~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/farming/product/barley":50,
	       	]),

		//�������ήɪ��T��
		"harvest_msg":"$ME�w�ߦa���γo�����������СC\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"farming":10,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"���ФW�p�p�����]�w�g�������j�F�C\n",
			50:	"�u�����Ф@����N�s�M�A�R���F�ͮ�C\n",
			150:	"���ФW���j���w�g���������F�C\n",
			300:	"�u���@��������⪺�j���A�w�g���ΦA��@�F�A�@�ߵ��@�}�l�K�i�H���ΡC\n",
			400:	"�o�����ФW���j���w�g���������F�A�ש�i�H���ΤF�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo�j������a�\��F�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo�@�Ǥj���w���G��F�C\n",
			TYPHOON:"�䭷��ŧ�ϱo�����j���Q�j���d�y���_�C\n",
			FOEHN:	"�I�����jŧ���������j���ֳt���\���C\n",
		]),
	]));
}

