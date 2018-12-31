/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : clam.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �F��
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
        set_idname("clam","�F��");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "�ΨӰ��i�ï]���F�ߡC\n");
        set("unit", "��");
        set("mass", 50);
        set("value", 10000);
        set("badsell", 1);

	// �ؤl��T
	set("growth",
	([
		//����
		"type":		FISHFARM,

		//�W��
		"idname":	this_object()->query_idname(),
		
		//�}�l�Ͳ��ɪ��T��
		"start_msg":"�F�߾���a�ƦC�b�����A���y�����a�y�L�C\n" ,
			
	       	//�C���ݭn����ƺ����P�ƶq
	       	"material":
	       	([
	       		"/obj/materials/water":20,
	       		"/obj/materials/wood":20,
	       	]),
			
	       	//���~���ƶq�]�w�P���~���|
	       	"harvest":
	       	([
	       		"/obj/fishfarm/product/pearl":3,
	       	]),

		//�ͦ��ɪ��T��
		"harvest_msg":"$ME�w�ߦa���X�F�ߤ����ï]�C\n",

		//�شӦ������~�һݭn���ޯ�P�䵥��
		"skill":
		([
			"fishfarm":80,
		]),

		//�����ɪ��T��
		"growing":
		([
			5:	"�F�ߦb���������a�R��A�@�Ǥp���p���b�����ʡC\n",
			100:	"�F���R�R�a�b�����@�ʤ]���ʡC\n",
			300:	"�F�߬�M�@�f��R�F�@���X�ӡA���ۨ�����ߤS�����X�F�_�ӡC\n",
			600:	"�F�ߦb���������a�R��A�@�Ǥp���p���b�����ʡC\n",
			1200:	"�F�߷U���U�j�A�q�ǥi�H�����ݨ�զ⪺�ï]�b���{ģ�C\n",
			1800:	"�F�ߪ��j�p�w�g�j�쨬�H�i�榬���F�C\n",
		]),
	
		// �Ѯ�v�T�ԭz
		"weather":
		([
			SNOWY:	"�j���s�s�Y���ϱo�F�̳߭��򪺭ᦺ�C\n",
			SHOWER:	"�s��Ӫ��B�q�ϱo�������F�̳߭Q�j���R���C\n",
			TYPHOON:"�䭷��ŧ�ϱo�������F�̳߭Q�j���R���C\n",
			FOEHN:	"�I�����jŧ���������F�̦߭]���ŹL���Ӧ��C\n",
		]),
	]));
}
