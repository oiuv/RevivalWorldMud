/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : energy.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-30
 * Note   : �믫�V�m
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
	set("name", 		"�믫�V�m");

	// �ޯ�^��
	set("id",		"energy");

	// �ޯ��Y��
	//set("title/100",	"");
	
	// �ޯ����
	set("note",		"�믫�ȤW����O�V�m");
}

// ����i�ǲ߹�H, �i�Ψӳ]�w�U�ؾǲ߱���
varargs int allowable_learn(object ob)
{
	if( undefinedp(ob) )
		return PLAYER_SKILL | NPC_SKILL;
	else
	{
		if( ob->is_npc() )
			return 1;
		else
			return 1;
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
	if( ob->is_module_npc() )
		return 60;
	else
		return 100;
}

// �i�]�w���P���󤧸g��ȭ���
float exp_multiple()
{
	return 0.08;
}

// �ɯŤ��e���ƥ�
void after_level_up(object ob, int newlevel)
{
	// �C�ɤ@�ť[ 10 �I hp
	addn("abi/energy/max", 10, ob);
	tell(ob, HIY+pnoun(2, ob)+"�W�[�F 10 �I���믫�ȡC\n"NOR);
}

// ���Ť��᪺�ƥ�
void before_level_down(object ob, int oldlevel)
{
	// �C���@�Ŧ� 10 �I hp
	addn("abi/energy/max", -10, ob);
	tell(ob, HIY+pnoun(2, ob)+"���C�F 10 �I���믫�ȡC\n"NOR);
}

// �g��ȼW�[�᪺�ƥ�
void added_exp(object ob, float exp)
{

}

// �O�_�u��ۧھǲ�
int self_learning()
{
	return 0;
}

// �S��аO
string query_special_type()
{
	return "";
}
