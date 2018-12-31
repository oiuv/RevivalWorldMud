/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : john_barton.c
 * Author : 
 * Date   : 2005-01-05
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
	//set("behavior/walking", 120);

	set("behavior/random", 
	    ([
		(: do_command("say ���A�ڰO�o�o�������Ӧ����ڡA���|�䤣��O...") :)   : 50,
		(: do_command("sigh �䤣�쪺�ܦo�@�w�|�ܥ��檺...") :) : 50,
	    ]));
}
void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{
	if( !objectp(me) || !msg ) return;

	if( strsrch(msg, "��") != -1 || strsrch(msg, "��") != -1)
		do_command("say �ڷQ���e���ڥi�R���k�B�͡C");
		
	if( strsrch(msg, "��") != -1 )
	{
		do_command("say ���˪���ڡA�ڤ]�����D�A�ڷQ�j���O����}�G�@�I����a�C");
		
		if( !me->query_quest_finish(QUEST_BOHR) && me->query_quest_step(QUEST_BOHR) == 0 )
		{
			me->set_quest_note(QUEST_BOHR, QUEST_BOHR_NAME, "�J���F�j���E�ڹy�A�L���G�b��@�ت�C");
			me->set_quest_step(QUEST_BOHR, 1);
		}
	}
}

void reply_emote(object me, string verb, string args)
{
	if( objectp(me) ) {
		switch(verb) {
			case "hi":
				do_command(verb + " " +me->query_id(1)+ " " + "�A�n�A�аݦ�����ƶܡH");
				break;
			case "?":
				do_command("say �A�F���Ψ��ز����ݧڡA�ڥu�O�ӧ�F�誺�A�i���O��ڡC");
				break;
			default:
				do_command("sorry " +me->query_id(1)+ " ��p�ڥ��b���A�ЧO���Z��");
				break;
		}
	}
}

int target_gone(object me)
{
	if( same_environment(this_object(), me) )
		return 1;

	do_command("say �áH�I�H�]���h�F�O�A��ӬO�Ӥ��D�^�����j�n�H...�u�n�u�n...");

	if( objectp(me) )
		delete_temp("quest/"QUEST_BOHR"/running", me);

	set_behavior();
	return -1;
}

void reply_get_object(object me, object ob, mixed amount)
{
	string obname;

	if( !objectp(ob) ) return;

	obname = ob->query_name();

	if( me->query_quest_step(QUEST_BOHR) == 1 && query("behavior") && (base_name(ob) == "/obj/farming/product/lily" || base_name(ob) == "/obj/farming/product/rose"))
	{
		destruct(ob);
	
		if( query_temp("quest/"QUEST_BOHR"/running", me) )			
			return;

		set_temp("quest/"QUEST_BOHR"/running", 1, me);

		// ����D�ʦ欰
		delete_behavior();

		process_function(({
			2,
			(: target_gone($(me)) :),
			(: do_command("tsk �A�u���n�e��"+$(obname)+"�ڡH�o�ˤ��Ӧn�N��a") :),
			4,
			(: target_gone($(me)) :),
			(: do_command("say ���o�Ӱe���ڤk�B�ͦo�@�w�|�ܰ������A�n�a�A����"+pnoun(2, $(me))+"�F�C") :),
			(: do_command("thank "+$(me)->query_id(1) ) :),
			5,
			(: target_gone($(me)) :),
			(: do_command("ah") :),
			2,
			(: target_gone($(me)) :),
			(: do_command("say �o�ӬO�ڭ�b���W�ߨ쪺�A�ڤ����������F�]�S�ΡA�N�e���A�a��") :),
			5,
			(: target_gone($(me)) :),
			(: new(__DIR__"broken_rod")->move($(me)) :),
			(: msg("$ME�o��F�@�����ª��˻s����C\n", $(me), 0, 1) :),
			(: do_command("say ����ڨ��F�A�u�O���§A") :),
			(: delete_temp("quest/"QUEST_BOHR"/running", $(me)) :),
			(: $(me)->set_quest_note(QUEST_BOHR, QUEST_BOHR_NAME, pnoun(2, $(me))+"���j���E�ڹy���F��A�L���A�F�@��ݰ_�����ª��˻s����C") :),
			(: $(me)->set_quest_step(QUEST_BOHR, 2) :),
			// ��_�D�ʦ欰
			(: set_behavior() :),
		    }));
	}
	else
	{
		process_function(({
			1,
			do_command("say �o�O����H�ڤ��n�o�ӪF��..."),
			1,
			do_command("drop all"),
		    }));
	}
}
void create()
{
	set_idname("john barton",HIW"�j��"NOR WHT"�E�ڹy"NOR);

	set("unit", "��");
	set("age", 19);
	set("gender", MALE_GENDER);
	set("long", "�@��Ӧ۰�~���T�N�C�~�A�ݥL���ˤl���G���b�W�o�ۡC");
	
	// �ҰʥD�ʦ欰
	set_behavior();

	startup_living();
}
