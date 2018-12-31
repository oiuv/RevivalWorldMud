/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rice seedlings.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-2-20
 * Note   : �_��
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
	set_idname("rice seedling","�_��");

	if( this_object()->set_shadow_ob() ) return;

	set("unit", "��");
	set("mass", 10);	// ���q
	set("value", 1);	// ����
	set("badsell", 1);

	// �ؤl��T
	set("seed_data",
		([
			//��������, ��@, ���Ωү���O�]�w
			"str_cost": ([
				"sow"		:5, //�����μ��خɻݭn����O
				"irrigate"	:10, //��@�C�ɤ��ݭn����O 
				"reap"		:10 //���ήɻݭn����O
				]),

			//�ؤl����
			"seed":"�_��",

			//�]�w�����άO����
			"sow_type":"����",

			//���β��~���ƶq�]�w�P���~���|
			"reap":({ 15, "/obj/farming/rice.c"}),

			//�U�ӨB�J���A�@�T��
			"msg":
			([
				// �������خɪ��T��
				"sow":
					"�_������a��������Цa�A�@���Y�Y�V�a���Pı�C\n" ,

				// ��@�ɪ���@���ƻP�T��
				"irrigate":
				({
					1,"�_�ФW�p�p�����]�w�g�������j�F�C\n",
					2,"�u���_�Ф@����N�s�M�A�R���F�ͮ�C\n",
					3,"�_�ФW���_�̤w�g���������F�C\n",
					5,"�u���@��������⪺�_�СA�w�g���ΦA��@�F�A�@�ߵ��@�}�l�K�i�H���ΡC\n",
				}),

				// �������ήɵ��ݬ�ƻP�T��
				"reap":
				({
					20,
					"�o���A�ФW���_�̤w�g���������F�A�ש�i�H���ΤF�C\n",
					"�o���A�иg�L�h��Ӫ���@�A�ש󦨼����ΤF�C\n�@���ΤF$NUM�U���_�̡C\n"
				}),

				// ��o�ɪ���ƻP�T��
				"neglect":
				({	
					300,
					"�o���A�Хѩ�h��ӯʥF��@�A�W�����_�]�����\��F�C\n",
					"�o���A�Хѩ�h��ӯʥF��@�A�w�g������o�F�C\n",
				}),

				// �Ѯ�v�T�ԭz
				"weather":
				([
					"RAINY":"�s��Ӫ��B�q�ϱo�@�ǽ_�]�w���G��F�C\n",
					"SNOWY":"�j���s�s�Y���ϱo�_�]�̳���a�\��F�C\n",
					"WINDY":"�g���jŧ�ϱo�����_�]�Q�j���d�y���_�C\n",
				]),
			]),
		])
	);
}