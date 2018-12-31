/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rd_engineer.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-17
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

int is_factory_staff()
{
	return 1;
}

void create()
{
        set_idname("RD engineer", "��o�u�{�v");

	set("unit", "��");
	set("position", HIY"�u�{�v"NOR);
        set("gender", random(2) ? MALE_GENDER : FEMALE_GENDER);
	set("salary", ({ 1000, 100000 }));
	set_temp("status", HIR"��o"NOR);
	
	startup_living();
}
