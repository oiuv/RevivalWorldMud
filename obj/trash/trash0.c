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
	set_idname(WHT"toilet brush"NOR ,"�}�ꪺ�����" );

	set_temp("status", WHT"�U��"NOR);
	
	if( this_object()->set_shadow_ob() ) return;

	set("unit", "��");
	set("value", -1);
	set("mass", 180);
	set("garbage", 1);
}

