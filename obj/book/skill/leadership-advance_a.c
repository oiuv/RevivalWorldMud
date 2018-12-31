/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : leadership-advance_a.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-30
 * Note   : �αs��O�ޯ��
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <skill.h>

#define BOOK_SKILL 	"leadership-adv"
#define BOOK_EXP	10000
#define BOOK_LEVEL	"A"
#define BASE_SKILL	([ "leadership" : 100 ])
	
inherit STANDARD_OBJECT;

mapping actions;

void do_readbook(object me, string arg)
{
	foreach(string skill, int level in BASE_SKILL)
	{
		if( me->query_skill_level(skill) < level )
			return tell(me, pnoun(2, me)+"���u"+(SKILL(skill))->query_idname()+"�v�����٤����H�Ψ��A�ѡu"+(SKILL(BOOK_SKILL))->query_idname()+"�v�C\n");
	}
	
 	msg("$ME�}�l�{�uŪ�_$YOU�A�u��$ME�ŵM�}�Ԧ������_���I�Y�A���G�q���Y�o��F�ܦh���F��C\n", me, this_object(), 1);

	tell(me, pnoun(2, me)+"��o�F "+BOOK_EXP+" �I�u"+(SKILL(BOOK_SKILL))->query_idname()+"�v�g��ȡC\n");
	me->add_skill_exp(BOOK_SKILL, BOOK_EXP);
	
	msg("$YOU�b$ME�\Ū����K�Ƭ��H���C\n", me, this_object(), 1);

	destruct(this_object(), 1);
}

void create()
{
	set_idname(HIG+(SKILL(BOOK_SKILL))->query_id()+NOR GRN" book "BOOK_LEVEL+NOR, HIG+(SKILL(BOOK_SKILL))->query_name()+NOR GRN"�޳N�� "BOOK_LEVEL" ��"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	if( this_object()->set_shadow_database() ) return;
	
	set("unit", "��");
	set("mass", 300);
	set("value", 10000);
	set("badsell", 1);

	actions = ([ "readbook" : (: do_readbook :) ]);

}