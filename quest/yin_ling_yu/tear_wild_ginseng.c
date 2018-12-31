/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : tear wild ginseng.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-13
 * Note   : ���s�H�x
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("wild ginseng", HIY"���s"NOR YEL"�H��"NOR);
	set_temp("status", HIC"�\"NOR CYN"��"NOR);

	if( this_object()->set_shadow_ob() ) return;
	

	set("long","���G�O�@�ػ�}�����ħ��C\n�o�⤣�M�`�����s�H�������x��"HIB"��"NOR BLU"��"NOR"�A�����I����G�٦��@�q�G�ơA\n�ݨӱo��Ӽ������Ī��H�ݰݡC");
	set("unit", "��");
	set("mass", 300);
	set("value", 1000);
}
