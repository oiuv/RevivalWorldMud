/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rose.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
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
        set_idname("rose","����");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long","������y�G�۷R�E���Q�E�R���C\n");
        set("unit", "��");
        set("mass", 5);
        set("value", 500);
}