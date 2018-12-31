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
	object *maleuser = filter_array(users(), (: query("gender", $1) == MALE_GENDER :));
	int usersize = sizeof(maleuser);

	if( usersize )
		set_idname(WHT"condom"NOR, maleuser[random(usersize)]->query_name()+"�ιL���O�I�M");
	else
		set_idname(WHT"condom"NOR , "�ιL���O�I�M");

	set_temp("status", WHT"�U��"NOR);

	if( this_object()->set_shadow_ob() ) return;

	set("unit", "��");
	set("value", -1);
	set("mass", 140);
	set("garbage", 1);
}

