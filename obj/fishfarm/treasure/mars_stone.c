/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : Mars_stone.c
 * Author : Msr@RevivalWorld
 * Date   : 2006-05-22
 * Note   : 火星隕石
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
        set_idname(HIR"mars "NOR RED"stone"NOR, HIR"火"NOR RED"星"HIC"隕石"NOR);
        set_temp("status", HIC"戰"NOR CYN"神");

        if( this_object()->set_shadow_ob() ) return;

        set("long", "一顆由火星飛來的隕石，石頭上似乎刻了一個八爪章魚的型像。\n");
        set("unit", "顆");
        set("mass", 0);
        set("value", 100000000);
        set("flag/no_amount", 1);
        set(BUFF_INT, 70);
        set(BUFF_HEALTH_REGEN, 10);

        set("buff/status", HIR"火"NOR RED"星"HIC"隕石"NOR);

        ::setup_equipment(EQ_EYE, HIG"眼"NOR GRN"睛"NOR);
}
