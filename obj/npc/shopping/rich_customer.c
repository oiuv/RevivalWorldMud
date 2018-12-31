/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rich_customer.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-10
 * Note   : 
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <gender.h>

inherit STANDARD_NPC;

void create()
{
        set_idname("rich customer", NOR RED+"�U��"+HIR+"�����Ѥj"+NOR);

	set("unit", "��");
        set("gender", MALE_GENDER);

        set("behavior/shopping", random(51)+250);
	set("rich_customer", 1);

	this_object()->add_phy(1000);

	startup_living();
	
	set_temp("status", NOR CYN"�ʪ�"NOR);
}
