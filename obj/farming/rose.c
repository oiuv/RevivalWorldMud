/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rose.c
 * Author : Tzj@RevivalWorld
 * Date   : 2003-5-22
 * Note   : ����
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
        set_idname("rose","[0m[31;1m����[0m");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long","�ۥj�H�ӶH�x�R����[0m[31;1m����[0m�A���Ӱe�R�H�i���N�Q���쪺�ĪG�C\n");
        set("unit", "��");
        set("mass", 5);
        set("value", 500);
}