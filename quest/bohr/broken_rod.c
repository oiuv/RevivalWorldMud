/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : broken_rod.c
 * Author : 
 * Date	  : 2003-05-03
 * Note	  : �}����
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <inherit.h>
#include <ansi.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("bamboo rod", "�˻s����" );

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�o�ڦ˻s������G�򥭱`���˻s���񤣦P�C");
        set("unit", "��");
        set("value", -1);
        set("mass", 200);
        set("rod_level", 50);
        set("level", 10);
        set("flag/no_amount", 1);
        set("badsell", 1);

}
