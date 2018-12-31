/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : Barley.c
 * Author : 
 * Date   : 2003-4-23
 * Note   : �j��
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
        set_idname("barley","�j��");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("unit", "�U");
        set("mass", 500);
        set("value", 200);
}

