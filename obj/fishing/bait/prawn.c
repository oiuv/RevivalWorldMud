/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : worm.c
 * Author :
 * Date	  : 2003-5-27
 * Note	  : ����
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <message.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

int is_bait()
{
	return 1;
}
void create()
{
	set_idname("prawn", "���תd");
	set_temp("status", HIY"����"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "��");
	set("value", 3);
	set("mass", 2);
	set("bait", 35);
	set("badsell", 1);

}