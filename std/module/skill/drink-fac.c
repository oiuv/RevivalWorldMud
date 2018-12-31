/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : drink-fac.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-24
 * Note   : ���ƥ[�u�s�@
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
	set("name", 		"���ƥ[�u�s�@");

	// �ޯ�^��
	set("id",		"drink-fac");

	// �ޯ��Y��
	set("title/40",		NOR HIY"���ƥ[�u��"NOR);
	set("title/100",	NOR HIY"���ƥ[�u�v"NOR);
	
	// �ޯ����
	set("note",		"���ƥ[�u�s�@�޳N");
}

// ����i�ǲ߹�H, �i�Ψӳ]�w�U�ؾǲ߱���
varargs int allowable_learn(object ob)
{
	if( undefinedp(ob) )
		return PLAYER_SKILL;
	else
	{
		if( ob->is_npc() )
			return 0;
		else
		{
			return 0;
		}
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
	return 0.0666;
}

// �ɯŤ��e���ƥ�
void after_level_up(object ob, int newlevel)
{

}

// ���Ť��᪺�ƥ�
void before_level_down(object ob, int oldlevel)
{
	
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
