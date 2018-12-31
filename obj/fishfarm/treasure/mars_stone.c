/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : Mars_stone.c
 * Author : Msr@RevivalWorld
 * Date   : 2006-05-22
 * Note   : ���P�k��
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
        set_idname(HIR"mars "NOR RED"stone"NOR, HIR"��"NOR RED"�P"HIC"�k��"NOR);
        set_temp("status", HIC"��"NOR CYN"��");

        if( this_object()->set_shadow_ob() ) return;

        set("long", "�@���Ѥ��P���Ӫ��k�ۡA���Y�W���G��F�@�ӤK�������������C\n");
        set("unit", "��");
        set("mass", 0);
        set("value", 100000000);
        set("flag/no_amount", 1);
        set(BUFF_INT, 70);
        set(BUFF_HEALTH_REGEN, 10);

        set("buff/status", HIR"��"NOR RED"�P"HIC"�k��"NOR);

        ::setup_equipment(EQ_EYE, HIG"��"NOR GRN"��"NOR);
}
