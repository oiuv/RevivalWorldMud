/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : soy_bean.c
 * Author : Tzj@RevivalWorld
 * Date   : 2003-5-4
 * Note   : �j��
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
        set_idname("soy bean","�j��");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("unit", "��");
        set("mass", 7);
        set("value", 8);
        set("badsell", 1);
        
        // �ؤl��T
        set("seed_data",
                ([
                        //��������, ��@, ���Ωү���O�]�w
                        "str_cost": ([
                                "sow"           :3, //�����μ��خɻݭn����O
                                "irrigate"      :4, //��@�C�ɤ��ݭn����O 
                                "reap"          :5 //���ήɻݭn����O
                                ]),
                        
                        //�ؤl����
                        "seed":"�j��",
                        
                        //�]�w�����άO����
                        "sow_type":"����",
                        
                        //���β��~�����|�P�ƶq�]�w
                        "reap":({ 15, "/obj/farming/soy_bean.c"}),
                        
                        //�U�ӨB�J���A�@�T��
                        "msg":
                        ([
                                // �������خɪ��T��
                                "sow":
                                        "�j������a�G������Цa�A�@���Y�Y�V�a���Pı�C\n" ,
                                
                                // ��@�ɪ��T��
                                "irrigate":
                                ({
                                        3,"�j���ФW�p�p�����]�w�g�������j�F�C\n",
                                        5,"�u���j���Ф@����N�s�M�A�R���F�ͮ�C\n",
                                        10,"�j���ФW���j���w�g���������F�C\n",
                                        15,"�u���@�����o�o���j���A�w�g���ΦA��@�F�A�@�ߵ��@�}�l�K�i�H���ΡC\n",
                                }),
                                
                                // �������ήɪ��T��
                                "reap":
                                ({
                                        20,
                                        "�o���j���ФW���j���w�g���������F�A�ש�i�H�Ħ��F�C\n",
                                        "�o���j���иg�L�h��Ӫ���@�A�ש󦨼����ΤF�C\n�@���ΤF$NUM�����j���C\n"
                                }),
                                
                                // ��o�ɪ��T��
                                "neglect":
                                ({      
                                        200,
                                        "�o���j���Хѩ�h��ӯʥF��@�A�W�������޺����\��F�C\n",
                                        "�o���j���Хѩ�h��ӯʥF��@�A�w�g������o�F�C\n",
                                }),
                                
                                "weather":
                                ([
                                        "RAINY":"�s��Ӫ��B�q�ϱo�@�Ǥj���]�w���ӻG��F�C\n",
                                        "SNOWY":"�j���s�s�Y���ϱo�j���]�̳���a�\��F�C\n",
                                        "WINDY":"�g���jŧ�ϱo�����j���]�Q�j���d�y���_�C\n",
                                ]),
                        ]),
                ])
        );
}


