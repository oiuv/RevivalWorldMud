/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sunflower.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : �V�鸪
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
        set_idname("sunflower","�V�鸪");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long","�V�鸪��y�G�R�}�E����E���ۡE�L�q�E�㻮�C\n");
        set("unit", "��");
        set("mass", 10);
        set("value", 400);
}
