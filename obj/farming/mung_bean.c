/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rice seedlings.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-2-20
 * Note   : ��
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("mung bean","��");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("unit", "��");
	set("mass", 5);
	set("value", 5);
	set("badsell", 1);

	// �ؤl��T
	set("seed_data",
		([
			//��������, ��@, ���Ωү���O�]�w
			"str_cost": ([
				"sow"		:2, //�����μ��خɻݭn����O
				"irrigate"	:3, //��@�C�ɤ��ݭn����O 
				"reap"		:3 //���ήɻݭn����O
				]),
			
			//�ؤl����
			"seed":"��",
			
			//�]�w�����άO����
			"sow_type":"����",
			
			//���β��~�����|�P�ƶq�]�w
			"reap":({ 20, "/obj/farming/mung_bean.c"}),
			
			//�U�ӨB�J���A�@�T��
			"msg":
			([
				// �������خɪ��T��
				"sow":
					"�񨧾���a�G������Цa�A�@���Y�Y�V�a���Pı�C\n" ,
				
				// ��@�ɪ��T��
				"irrigate":
				({
					1,"�񨧥ФW�p�p�����]�w�g�������j�F�C\n",
					2,"�u���񨧥Ф@����N�s�M�A�R���F�ͮ�C\n",
					3,"�񨧥ФW���񨧤w�g���������F�C\n",
					4,"�u���@�����o�o���񨧡A�w�g���ΦA��@�F�A�@�ߵ��@�}�l�K�i�H���ΡC\n",
				}),
				
				// �������ήɪ��T��
				"reap":
				({
					20,
					"�o���񨧥ФW���񨧤w�g���������F�A�ש�i�H���ΤF�C\n",
					"�o���񨧥иg�L�h��Ӫ���@�A�ש󦨼����ΤF�C\n�@���ΤF$NUM�����񨧡C\n"
				}),
				
				// ��o�ɪ��T��
				"neglect":
				({	
					200,
					"�o���񨧥Хѩ�h��ӯʥF��@�A�W�����񨧪޺����\��F�C\n",
					"�o���񨧥Хѩ�h��ӯʥF��@�A�w�g������o�F�C\n",
				}),
				
				"weather":
				([
					"RAINY":"�s��Ӫ��B�q�ϱo�@�Ǻ񨧪w���G��F�C\n",
					"SNOWY":"�j���s�s�Y���ϱo�񨧭]�̳���a�\��F�C\n",
					"WINDY":"�g���jŧ�ϱo�����񨧭]�Q�j���d�y���_�C\n",
				]),
			]),
		])
	);
}