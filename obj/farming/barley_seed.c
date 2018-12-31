/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : barley seedlings.c
 * Author : 
 * Date   : 2002-2-20
 * Note   : �j���ؤl
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
        set_idname("barley seed","�j���ؤl");

        if( this_object()->set_shadow_ob() ) return;

        set("unit", "��");
        set("mass", 10);        // ���q
        set("value", 10);        // ����
	set("badsell", 1);

        // �ؤl��T
        set("seed_data",
                ([
                        //��������, ��@, ���Ωү���O�]�w
                        "str_cost": ([
                                "sow"           :7, //�����μ��خɻݭn����O
                                "irrigate"      :15, //��@�C�ɤ��ݭn����O 
                                "reap"          :15 //���ήɻݭn����O
                                ]),

                        //�ؤl����
                        "seed":"�j���ؤl",

                        //�]�w�����άO����
                        "sow_type":"����",

                        //���β��~���ƶq�]�w�P���~���|
                        "reap":({ 15, "/obj/farming/barley.c"}),

                        //�U�ӨB�J���A�@�T��
                        "msg":
                        ([
                                // �������خɪ��T��
                                "sow":
                                        "�j���ؤl����a�G������Цa�A�@���Y�Y�V�a���Pı�C\n" ,

                                // ��@�ɪ���@���ƻP�T��
                                "irrigate":
                                ({
                                        1,"���ФW�p�p�����]�w�g�������j�F�C\n",
                                        2,"�u�����Ф@����N�s�M�A�R���F�ͮ�C\n",
                                        4,"���ФW���j���w�g���������F�C\n",
                                        6,"�u���@��������⪺�j���A�w�g���ΦA��@�F�A�@�ߵ��@�}�l�K�i�H���ΡC\n",
                                }),

                                // �������ήɵ��ݬ�ƻP�T��
                                "reap":
                                ({
                                        20,
                                        "�o�����ФW���j���w�g���������F�A�ש�i�H���ΤF�C\n",
                                        "�o�����иg�L�h��Ӫ���@�A�ש󦨼����ΤF�C\n�@���ΤF$NUM�窺�j���C\n"
                                }),

                                // ��o�ɪ���ƻP�T��
                                "neglect":
                                ({      
                                        300,
                                        "�o�����Хѩ�h��ӯʥF��@�A�W�����j�������\��F�C\n",
                                        "�o�����Хѩ�h��ӯʥF��@�A�w�g������o�F�C\n",
                                }),

                                // �Ѯ�v�T�ԭz
                                "weather":
                                ([
                                        "RAINY":"�s��Ӫ��B�q�ϱo�@�Ǥj���w���G��F�C\n",
                                        "SNOWY":"�j���s�s�Y���ϱo�j���̳���a�\��F�C\n",
                                        "WINDY":"�g���jŧ�ϱo�����j���Q�j���d�y���_�C\n",
                                ]),
                        ]),
                ])
        );
}

