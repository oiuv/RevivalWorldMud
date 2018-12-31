/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : agility-adv.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-09
 * Note   : �ӱ��L�P
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
	set("name", 		"�ӱ��L�P");

	// �ޯ�^��
	set("id",		"agility-adv");

	// �ޯ��Y��
	//set("title/100",	"");
	
	// �ޯ����
	set("note",		"�ӱ��ݩʯ�������");
}

// ����i�ǲ߹�H, �i�Ψӳ]�w�U�ؾǲ߱���
varargs int allowable_learn(object ob)
{
	if( undefinedp(ob) )
		return PLAYER_SKILL | NPC_SKILL;
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
	if( !(newlevel % 5) )
	{
		ob->add_agi(1);
		tell(ob, HIY+pnoun(2, ob)+"�W�[�F 1 �I���ӱ��ݩʡC\n"NOR);
	}
}

// ���Ť��᪺�ƥ�
void before_level_down(object ob, int oldlevel)
{
	if( !(oldlevel % 5) )
	{
		ob->add_agi(-1);
		tell(ob, HIY+pnoun(2, ob)+"��֤F 1 �I���ӱ��ݩʡC\n"NOR);
	}
}

// �g��ȼW�[�᪺�ƥ�
void added_exp(object ob, float exp)
{

}

// �O�_�u��ۧھǲ�
int self_learning()
{
	return 1;
}

// �S��аO
string query_special_type()
{
	return HIC"�}"NOR CYN"��"NOR;
}
