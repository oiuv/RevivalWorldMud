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
	set_idname("tsou chip", "�g�q�H��");
	set_temp("status", HIY"����"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "��");
	set("value", 2);
	set("mass", 1);
	set("bait", 25);
	set("badsell", 1);

}