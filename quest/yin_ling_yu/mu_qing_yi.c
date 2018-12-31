/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : grandam.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-13
 * Note   : 
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

/*
	�t�ιw�]�� NPC �����禡
	void reply_emote(object me, string verb, string args);
	void reply_say(object me, string msg);
	void reply_catch_tell(object me, string verb, string args);
	void reply_get_object(object me, object ob, mixed amount);
	void reply_get_money(object me, string unit, mixed money);
*/

#include <ansi.h>
#include <inherit.h>
#include <gender.h>
#include <daemon.h>
#include <quest.h>

inherit STANDARD_NPC;

void do_command(string cmd);

void set_behavior()
{
	set("behavior/random", 
	    ([
			(: do_command("sing") :)   : 50,
	    ]));
}

void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{
	if( !objectp(me) ) return;

	if( !msg || !msg[0] )
		return;
		
	if( strsrch(msg, "�_��") != -1 )
	{
		do_command("say �ڤ����D�L�O�֡A���ڥi�H�q�o�ӦW�r���P����@�ѤH�����j�R");
		return;
	}
	
	if( strsrch(msg, "�Z��") != -1 )
	{
		do_command("say �ڤ����D�L�O�֡A���ڪ��D�L�w�g�D�J�����A�H�ʹN�O�p���A�ñ��z�Ҿ֦����@���a...");
		return;
	}
	
	if( strsrch(msg, "�¦�") != -1 )
	{
		do_command("say ��...�¦Ъ��A�o���¾�ܰQ�H���w�A��~�e�ڴ��J���L�o�A�Ⱥ޷�ɧڤw�w�����骺���G�A���ڤ����Ԥߧi�D�o�o�ӥ��w�o�ͪ��d�@");
		return;
	}
	
	if( strsrch(msg, "�H��") != -1 )
	{
		do_command("say �¦Щҫ����X�����賥�s�H�Ѫ��S��ä��b�󥦪��ĩʡA�ӬO�b��ĭ�|�F�Ƥd�~�Ҳ��ͪ��F��");
		return;
	}
	
	if( strsrch(msg, "�F��") != -1 )
	{
		do_command("smile "+me->query_id(1));
		do_command("say "+pnoun(2, me)+"�|�J�W���¦СA�K�O���t�H�A�W�Ѧ۷|��"+pnoun(2, me)+"����");
		
		if( me->query_quest_step(QUEST_YIN_LING_YU_2) == 1 )
		{
			me->set_quest_note(QUEST_YIN_LING_YU_2, QUEST_YIN_LING_YU_2_NAME, "���F�P�k�y�p�C�̡z�A���D�F��ӳo�賥�s�H�Ѩ㦳�ۯ������F�ʡC");
			me->set_quest_step(QUEST_YIN_LING_YU_2, 2);
		}
		return;
	}
	
	if( strsrch(msg, "����") != -1 )
	{
		do_command("say �ѹa�|��ô�a�H��.....");
		return;
	}
		
	if( me->query_quest_step(QUEST_CITY_WALKER) == 1 && strsrch(msg, "�����C�H") != -1 )
	{
		do_command("say ��H"+pnoun(2, me)+"�Q���D�@������L���Ʊ��ܡH");
		me->set_quest_note(QUEST_CITY_WALKER, QUEST_CITY_WALKER_NAME, "�γ\��Ū���q�ֵ��p�C��ťť�C");
		me->set_quest_step(QUEST_CITY_WALKER, 2);
		return;
	}
	
	if( me->query_quest_step(QUEST_CITY_WALKER) == 2 && strsrch(msg, "�@�b�֦����ۥ�������") != -1 )
	{
		msg("$ME�`�N��$YOU���F�����ά�MŸ�ʤF�@�U�A���H�Y�S�^�_�쥻���ˤl...�C\n", me, this_object(), 1);
		do_command("say ...��Ӧp��");
		do_command("say �o���֥s�@�u��@�Ӥ����_�����ڡv�A"+pnoun(2, me)+"�O�ۧa�C");
		
		me->set_quest_note(QUEST_CITY_WALKER, QUEST_CITY_WALKER_NAME, "��ӳo���֥s�@�u��@�Ӥ����_�����ڡv�C");
		me->set_quest_step(QUEST_CITY_WALKER, 3);
		return;
	}
}

void reply_emote(object me, string verb, string args)
{
	if( !objectp(me) ) return;
	
	do_command("bow "+me->query_id(1));
}

void reply_get_object(object me, object ob, mixed amount)
{
	if( !objectp(ob) ) return;

	process_function(({
		1,
		do_command("say �ѹa�|��ô�a�H..."),
		1,
		do_command("drop all "+ob->query_id(1)),
	}));
}


void create()
{
	set_idname("mu qing yi", HIG"�p"NOR GRN"�C��"NOR);
	set_temp("status", HIG"�@�~"NOR GRN"�P�k"NOR);

	set("long", "�n�M�q���~���k�l�A�o����護���ϤH���������ۦo�A���b�o�����Ǥ��ѱo�ۺF��©�C");
	set("unit", "��");
	set("age", 24);
	set("gender", FEMALE_GENDER);

	// �ҰʥD�ʦ欰
	set_behavior();

	startup_living();
}
