/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : coarse_drill.c
 * Author : Clode@RevivalWorld
 * Date	  : 2005-04-06
 * Note	  : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname("coarse drill", "�ʻs�p�Y");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�F���j�q�Ͳ����G�����~�A�ΨӱĶ���ۡA�~��C���A�l�a�v�۷��C");
	set("endurance", 100);
	set("unit", "��");
	set("value", 100);
	set("mass", 500);
	set("collection_tool", "stone");
	set("badsell", 1);
}
