/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : idle.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-04
 * Note   : �o�b�ޯ�
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <skill.h>
#include <inherit.h>
#include <message.h>
#include <daemon.h>

inherit STANDARD_SKILL;

void create()
{
	// �ޯत��
	set("name", 		"�o�b�ޯ�");

	// �ޯ�^��
	set("id",		"idle");

	// �ޯ��Y��
	set("title/100",	HIW"�Ѱ���"NOR WHT"�v�v"NOR);
	
	// �ޯ����
	set("note",		"���C�������ʶi�J�ߧڹ��R�߷Q���޳N");
}

// ����i�ǲ߹�H, �i�Ψӳ]�w�U�ؾǲ߱���
varargs int allowable_learn(object ob)
{
	if( undefinedp(ob) )
		return 0;
	else
	{
		if( ob->is_npc() )
			return 0;
		else
			return 0;
	}
}

// �i�۳]�C�ӵ��Żݭn���g���, �Ϊ����ϥ� SKILL_D �w�]�ƾǦ�
int level_exp(int lv)
{
	return SKILL_D->query_level_exp(lv);
}

// �i�]�w�U�س̰����ű���, 
int max_level(object ob)
{
	return 100;
}

// �i�]�w���P���󤧸g��ȭ���
float exp_multiple()
{
	return 1.0;
}

// �ɯŤ��e���ƥ�
void after_level_up(object ob, int newlevel)
{
	if( newlevel == 100 )
	{
		ob->add_str(3);
		ob->add_phy(3);
		ob->add_int(3);
		ob->add_agi(3);
		ob->add_cha(3);
		tell(ob, HIY+pnoun(2, ob)+"���O�q�b�o�b�ޯ��F���Ů��������ɤF 3 �I�I�I�I\n"NOR);
		tell(ob, HIY+pnoun(2, ob)+"�����b�o�b�ޯ��F���Ů��������ɤF 3 �I�I�I�I\n"NOR);
		tell(ob, HIY+pnoun(2, ob)+"�����Ӧb�o�b�ޯ��F���Ů��������ɤF 3 �I�I�I�I\n"NOR);
		tell(ob, HIY+pnoun(2, ob)+"���ӱ��b�o�b�ޯ��F���Ů��������ɤF 3 �I�I�I�I\n"NOR);
		tell(ob, HIY+pnoun(2, ob)+"���y�O�b�o�b�ޯ��F���Ů��������ɤF 3 �I�I�I�I\n"NOR);
		tell(ob, HIY+"����o�b�ұo�ޯ�g��ȱN����۰��ର���|�g��ȡI�I�I\n"NOR, ENVMSG);
	}
}

// ���Ť��᪺�ƥ�
void before_level_down(object ob, int oldlevel)
{
	if( oldlevel == 100 )
	{
		ob->add_str(-3);
		ob->add_phy(-3);
		ob->add_int(-3);
		ob->add_agi(-3);
		ob->add_cha(-3);
		tell(ob, HIR+pnoun(2, ob)+"���O�q�b�o�b�ޯ୰�Ů��������C�F 3 �I�I�I�I\n"NOR);
		tell(ob, HIR+pnoun(2, ob)+"�����b�o�b�ޯ୰�Ů��������C�F 3 �I�I�I�I\n"NOR);
		tell(ob, HIR+pnoun(2, ob)+"�����Ӧb�o�b�ޯ୰�Ů��������C�F 3 �I�I�I�I\n"NOR);
		tell(ob, HIR+pnoun(2, ob)+"���ӱ��b�o�b�ޯ୰�Ů��������C�F 3 �I�I�I�I\n"NOR);
		tell(ob, HIR+pnoun(2, ob)+"���y�O�b�o�b�ޯ୰�Ů��������C�F 3 �I�I�I�I\n"NOR);
	}
}

// �g��ȼW�[�᪺�ƥ�
void added_exp(object ob, float exp)
{
	// �o�b�ޯມ�F����, exp �������t���L�ޯ�
	if( ob->query_skill_level("idle") == 100 )
	{
		ob->add_social_exp(to_int(exp));
	}
	// �٥���
	else
	{
		// ���[�˦� strength �ޯ�
		ob->add_skill_exp("strength", -exp/30.);

		// ���[�˦� physique �ޯ�
		ob->add_skill_exp("physique", -exp/30.);
			
		// ���[�˦� intelligence �ޯ�
		ob->add_skill_exp("intelligence", -exp/30.);

		// ���[�˦� agility �ޯ�
		ob->add_skill_exp("agility", -exp/30.);
				
		// ���[�˦� charisma �ޯ�
		ob->add_skill_exp("charisma", -exp/30.);
	}
}

// �O�_�u��ۧھǲ�
int self_learning()
{
	return 1;
}

// �S��аO
string query_special_type()
{
	return HIG"��"NOR GRN"�_"NOR;
}
