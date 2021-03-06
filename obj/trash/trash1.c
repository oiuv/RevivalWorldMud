/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : Trash.c
 * Author :
 * Date	  : 2003-5-3
 * Note	  : 垃圾
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <gender.h>

inherit	STANDARD_OBJECT;

void create()
{	
	object *maleuser = filter_array(users(), (: query("gender", $1) == MALE_GENDER :));
	int usersize = sizeof(maleuser);

	if( usersize )
		set_idname(WHT"condom"NOR, maleuser[random(usersize)]->query_name()+"用過的保險套");
	else
		set_idname(WHT"condom"NOR , "用過的保險套");

	set_temp("status", WHT"垃圾"NOR);

	if( this_object()->set_shadow_ob() ) return;

	set("unit", "個");
	set("value", -1);
	set("mass", 140);
	set("garbage", 1);
}

