/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rod.c
 * Author : Kyyung@RevivalWorld
 * Date   : 2003-05-17
 * Note   : ����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <material.h>

inherit OBJECT_ACTION_MOD;

inherit ROD_ACTION_MOD;

// �]�w����ʧ@

nosave mapping action_info =
([
        "help"  : "�|���غc�����C",
        "action":
                ([
                        "hook"   : (: do_hook :),
                     "fishing"   : (: do_fishing :),
                ]),

]);


// �]�w������
nosave array object_info = ({
        
        // �� name
        "����"
        
        // �� id
        ,"rod"

        // ����,�w�q�~����
        ,"rod"

        // �򥻧��ƻݨD
        ,([ IRON:1, WOOD:2 ])
        
        // �s�y�����~�һݭn����ޭ�
        ,1000

        // �򥻻s�y�t��(�C����u/�C�C���ɶ��@����)
        ,20

        // �򥻻���
        ,"100"

        // ��޾���
        ,0

        // ����Ѽ�
        ,([ "rod_level":10, "level":0 , "endurance":100, "mass":100 ])
});
