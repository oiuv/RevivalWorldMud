/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : herbage.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : ����
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
        set_idname("herbage","����");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long","�@���}�i�󭹩ʰʪ����}�ơC\n");
        set("unit", "��");
        set("mass", 400);
        set("value", 100);
}