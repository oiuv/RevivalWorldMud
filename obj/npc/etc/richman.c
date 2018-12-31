/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : richman.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-10
 * Note   : 
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>
#include <gender.h>

inherit STANDARD_NPC;

void create()
{
        set_idname("rich man","�����H");

	set("unit", "��");
        set("gender", FEMALE_GENDER);

        set("behavior/shopping", random(30)+10);

	startup_living();
}
