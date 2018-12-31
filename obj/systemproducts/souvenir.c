/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : object.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-08-11
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <daemon.h>

inherit STANDARD_OBJECT;

varargs void create(string file, string type)
{
	string name;

	if( CITY_D->city_exist(type) )
		name = CITY_D->query_city_name(type)+HIW"����"NOR WHT"�~"NOR;
	else
		name = capitalize(type)+HIW" ����"NOR WHT"�~"NOR;

        set_idname(type+" souvenir", name);
        
        if( !undefinedp(file) && this_object()->set_shadow_ob(load_object("/obj/systemproducts/souvenir.c")) ) return;
        
        set("long","�¤�u�s�y�����������~�A�㦳���ûP�������ȡC\n");
        set("unit", "��");
        set("mass", 100);
        set("value", 1000);
        
        objects( (: base_name($1)[0..16] == "/product/souvenir" :) )->set_shadow_ob(this_object());
}