/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : seed_of_rose.c
 * Author : Tzj@RevivalWorld
 * Date   : 2003-5-22
 * Note   : �����ؤl
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
        set_idname("seed of rose",HIR"����"NOR+RED"�ؤl"NOR);
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("unit", "��");
        set("mass", 2);
        set("value", 100);
        set("long","�ݭn�p�ߨ��@�~�����[0m[31;1m����[0m�ᦷ�C\n");
        set("badsell", 1);
 
        // �ؤl��T
        set("seed_data",
                ([
                        //��������, ��@, ���Ωү���O�]�w
                        "str_cost": ([
                                "sow"           :3, //�����μ��خɻݭn����O
                                "irrigate"      :7, //��@�C�ɤ��ݭn����O 
                                "reap"          :10 //���ήɻݭn����O
                                ]),
                        
                        //�ؤl����
                        "seed":"�����ؤl",
                        
                        //�]�w�����άO����
                        "sow_type":"����",
                        
                        //���β��~�����|�P�ƶq�]�w
                        "reap":({ 5, "/obj/farming/rose.c"}),
                        
                        //�U�ӨB�J���A�@�T��
                        "msg":
                        ([
                                // �������خɪ��T��
                                "sow":
                                        "�����ؤl�����b�и̡A�٬ݤ��X���t�C\n" ,
                                
                                // ��@�ɪ��T��
                                "irrigate":
                                ({
                                        2,"�����ؤl�ޥX�@�ڮڹ�޺����ͪ��C\n",
                                        3,"�������t�C�C�ͪ��X�����ý��A�W�����񸭤@�@�ήi�}�ӡC\n",
                                        5,"�����Ы_�X�F�\�h������c�A�A�L���[�N�|�}��F�C\n",
                                        7,"�u���@���S�@����}���A���⪴���b�񸭪��MŨ���U���A�ʤH�A�A���@�U�l�N�i�H�Ħ��F�C\n",
                                }),
                                
                                // �������ήɪ��T��
                                "reap":
                                ({
                                        5,
                                        "�����ФW��������w�������A�w�g�i�H�Ħ��F�C\n",
                                        "�o�������Цb�Ӥߨ��@���U�A���F�\�h�����C\n�@�Ħ��F$NUM���������C\n"
                                }),
                                
                                // ��o�ɪ��T��
                                "neglect":
                                ({      
                                        50,
                                        "�o�������Ц]�����[�S������A�񸭤@�����\����s�C\n",
                                        "�o�������Ц]�[�����[���@�A�w�g������o�F�C\n",
                                }),
                                
                                "weather":
                                ([
                                        "RAINY":"�j�B�ϱo�����Ъ��ڪw���G��F�C\n",
                                        "SNOWY":"�j���Ϫ����оD�����Y�����B�`�C\n",
                                        "WINDY":"�g���Ϫ��@�������������t�Q�d�y�j�_�C\n",
                                ]),
                        ]),
                ])
        );
}


