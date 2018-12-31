/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : customer.c
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

#define JAPANGIRLNAME	__DIR__"japangirlname"

void create()
{
	string *idname = explode(read_file(JAPANGIRLNAME), "\n");

        set_idname("customer", NOR CYN+"�U��"+HIC+idname[random(sizeof(idname))]+NOR);

	set("unit", "��");
        set("gender", FEMALE_GENDER);

        set("behavior/shopping", random(31)+10);

	this_object()->add_phy(100);

	startup_living();
	
	set_temp("status", NOR CYN"�ʪ�"NOR);
}
