/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : girl.c
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
        set_idname("little girl","�i�R���p�k��");

	set("unit", "��");
        set("gender", FEMALE_GENDER);

        set("behavior/walking", random(30)+10);
        set("behavior/shopping", random(30)+10);

	startup_living();
}
