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

inherit EQUIP;

inherit OBJECT_ACTION_MOD;

// �]�w����ʧ@

nosave mapping action_info =
([
        "help"  : "�|���غc�����C",
]);


// �]�w������
nosave array object_info = ({
        
        // �� name
        "�٫�"
        
        // �� id
        ,"ring"

        // ����,�w�q�~����
        ,"ring"

        // �򥻧��ƻݨD
        ,([ GOLD:2 ])
        
        // �s�y�����~�һݭn����ޭ�
        ,10000

        // �򥻻s�y�t��(�C����u/�C�C���ɶ��@����)
        ,20

        // �򥻻���
        ,"1500"

        // ��޾���
        ,0

        // ����Ѽ�
        ,([ "equip":"finger", "applyattr/phy":1 , "addloading":1000 ])
});
