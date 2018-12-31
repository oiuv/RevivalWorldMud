/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : herbage_seed.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �����ؤl
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
        set_idname("herbage seed","����ؤl");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long","�@���}�i�󭹩ʰʪ����}�ơC\n");
        set("unit", "��");
        set("mass", 1);
        set("value", 3);
        set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		FARM,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�������خɪ��T��
		"start_msg":"����ؤl����a�G������Цa�A�@���Y�Y�V�a���Pı�C\n" ,
			
	       	//�C���ؤl�ί��]�ݭn����Ƽƶq
	       	"material":
	       	([
	       		"/obj/materials/water":5,
	       	]),
			
	       	//���β��~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/farming/product/herbage":50,
	       	]),

		//�������ήɪ��T��
		"harvest_msg":"$ME�w�ߦa���γo������������C\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"farming":0,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"����W�p�p�����]�w�g�������j�F�C\n",
			50:	"�u������@����N�s�M�A�R���F�ͮ�C\n",
			100:	"�Цa�W������w�g���������F�C\n",
			200:	"�u���@����C��⪺����A�w�g���ΦA��@�F�A�@�ߵ��@�}�l�K�i�H���ΡC\n",
			300:	"�o���Цa�W������w�g���������F�A�ש�i�H���ΤF�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo������a�\��F�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo�@�Ǫ���w���G��F�C\n",
			TYPHOON:"�䭷��ŧ�ϱo��������Q�j���d�y���_�C\n",
			FOEHN:	"�I�����jŧ������������ֳt���\���C\n",
		]),
	]));
}
