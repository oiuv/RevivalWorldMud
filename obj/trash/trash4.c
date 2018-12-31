/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : Trash.c
 * Author :
 * Date	  : 2003-5-3
 * Note	  : �U��
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	object *user = users();
	int usersize = sizeof(user);

	if( usersize )
		set_idname(WHT"briefs"NOR, user[random(usersize)]->query_name()+"��L���}����");
	else
		set_idname(WHT"briefs"NOR , "��L���}����");

	set_temp("status", WHT"�U��"NOR);

	if( this_object()->set_shadow_ob() ) return;

	set("unit", "��");
	set("value", -1);
	set("mass", 70);
	set("garbage", 1);
}

