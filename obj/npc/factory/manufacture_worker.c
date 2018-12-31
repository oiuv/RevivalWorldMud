/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : manufacture_worker.c
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
        set_idname("manufacture worker", "�Ͳ��u���u");

	set("unit", "��");
	set("position", HIY"���u"NOR);
        set("gender", random(2) ? MALE_GENDER : FEMALE_GENDER);
	set("salary", ({ 500, 5000 }));
	set_temp("status", HIR"�Ͳ�"NOR);
	
	startup_living();
}
