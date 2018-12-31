/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : Questitem.c
 * Author : 
 * Date   : 2005-01-05
 * Note   : �������O
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <condition.h>

inherit STANDARD_OBJECT;

mapping actions;

void do_readnote(object me, string arg)
{	
	if( me->in_condition(FISHINGNOTE) )
		return tell(me, pnoun(2, me)+"�٦b�P�˪��[�����A���C\n");

 	msg("$ME�}�lŪ�_"+this_object()->query_idname()+"�A�u��$ME�D�`�٦P�������_���I�Y���g�A���G�q���Y�o��F�ܦh���F��C\n", me, 0, 1);
	
	me->start_condition(FISHINGNOTE);

	tell(me, pnoun(2, me)+"��o�F�@�ǳ����ޯ�g��ȡC\n");
	me->add_skill_exp("fishing", 1000);
}

void create()
{
	set_idname(HIW"f"WHT"ishing "HIW"n"WHT"ote",HIW"����"WHT"���O"NOR);
	
	if( this_object()->set_shadow_database() ) return;
	
	set("unit", "��");
	set("mass", 150);
	set("value", 40000);
	set("badsell", 1);

	actions = ([ "readnote" : (: do_readnote :) ]);

}
