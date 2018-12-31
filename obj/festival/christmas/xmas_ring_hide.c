/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_ring.c
 * Author : Kyo@RevivalWorld
 * Date   : 2006-12-25
 * Note   : �t�ϧ٫�
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
        set_idname("ring of christmas", HIR"�C"NOR RED"��"NOR HIW"�٫�"NOR);
        set_temp("status", HIY"2006"NOR HIC" ��"NOR CYN"��" NOR HIC"��");

        if( this_object()->set_shadow_database() ) return;

        set("long", "2006 Rw �t�Ϭ����٫����ê�, ���[�j�y�O���\��C\n");
        
        set("unit", "��");
        set("mass", 100);
        set("value", 10);
        set("badsell", 1);
        set("flag/no_amount", 1);
        set(BUFF_CHA, 1);

        ::setup_equipment(EQ_FINGER, HIG"��"NOR GRN"��"NOR);
}
