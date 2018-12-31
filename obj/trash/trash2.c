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
#include <gender.h>

inherit	STANDARD_OBJECT;

void create()
{
	object *femaleuser = filter_array(users(), (: query("gender", $1) == FEMALE_GENDER :));
	int usersize = sizeof(femaleuser);

	if( usersize )
		set_idname(WHT"sanitary napkin"NOR, femaleuser[random(usersize)]->query_name()+"�ιL���åʹ�");
	else
		set_idname(WHT"sanitary napkin"NOR , "�ιL���åʹ�");

	set_temp("status", WHT"�U��"NOR);

	if( this_object()->set_shadow_ob() ) return;

	set("unit", "��");
	set("value", -1);
	set("mass", 180);
	set("garbage", 1);
}

