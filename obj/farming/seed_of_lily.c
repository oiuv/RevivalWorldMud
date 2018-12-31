/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : seed_of_lily.c
 * Author : Tzj@RevivalWorld
 * Date   : 2003-05-30
 * Note   : �ʦX�ؤl
 * Update :
 *  o 2000-05-30
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
        set_idname("seed of lily",HIW"�ʦX"NOR"�ؤl");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("unit", "��");
        set("mass", 2);
        set("value", 200);
        set("long","�ݭn�p�ߨ��@�~�����[0m[37;1m�ʦX[0m�ᦷ�C\n");
        set("badsell", 1);

        // �ؤl��T
        set("seed_data",
                ([
                        //��������, ��@, ���Ωү���O�]�w
                        "str_cost": ([
                                "sow"           :5, //�����μ��خɻݭn����O
                                "irrigate"      :9, //��@�C�ɤ��ݭn����O 
                                "reap"          :8 //���ήɻݭn����O
                                ]),
                        
                        //�ؤl����
                        "seed":"�ʦX�ؤl",
                        
                        //�]�w�����άO����
                        "sow_type":"����",
                        
                        //���β��~�����|�P�ƶq�]�w
                        "reap":({ 3, "/obj/farming/lily.c"}),
                        
                        //�U�ӨB�J���A�@�T��
                        "msg":
                        ([
                                // �������خɪ��T��
                                "sow":
                                        "�ʦX�ؤl�����b�и̡A�٬ݤ��X���t�C\n" ,
                                
                                // ��@�ɪ��T��
                                "irrigate":
                                ({
                                        2,"�ʦX�ؤl�ޥX�@�ڮڹ�޺����ͪ��C\n",
                                        3,"�ʦX���t�C�C�ͪ��A�W�����񸭤@�@�ήi�}�ӡC\n",
                                        6,"�ʦX��Ы_�X�F�\�h�ʦX��c�A�A�L���[�N�|�}��F�C\n",
                                        7,"��}���զ�ʦX��b�񸭪��MŨ���U�t��L��A�A���@�U�l�N�i�H�Ħ��F�C\n",
                                }),
                                
                                // �������ήɪ��T��
                                "reap":
                                ({
                                        10,
                                        "�ʦX��ФW���ʦX�w�������A�w�g�i�H�Ħ��F�C\n",
                                        "�o���ʦX��Цb�Ӥߨ��@���U�A���F�\�h�ʦX�C\n�@�Ħ��F$NUM�����ʦX��C\n"
                                }),
                                
                                // ��o�ɪ��T��
                                "neglect":
                                ({      
                                        25,
                                        "�o���ʦX��Ц]�����[�S������A�񸭤@�����\����s�C\n",
                                        "�o���ʦX��Ц]�[�����[���@�A�w�g������o�F�C\n",
                                }),
                                
                                "weather":
                                ([
                                        "RAINY":"�j�B�ϱo�\�h�ʦX���ڪw���G��F�C\n",
                                        "SNOWY":"�j���ϦʦX��оD�����Y�����B�`�C\n",
                                        "WINDY":"�g���Ϥ@�������ʦX���t�Q�d�y�j�_�C\n",
                                ]),
                        ]),
                ])
        );
}
