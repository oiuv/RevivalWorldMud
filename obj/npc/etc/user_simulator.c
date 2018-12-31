/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : girl.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-13
 * Note   : 
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_NPC;

void create()
{
	object user = users()[random(sizeof(users()))];
        set_idname(user->query_id(),user->query_name());
	set("unit", "��");
	
        //set("behavior/shopping", random(30)+10);
	startup_living();
}
