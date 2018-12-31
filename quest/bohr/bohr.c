/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : QuestNPC.c
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
		(: do_command("sigh �̪񳣳����Ө쳽�A�p�G�ڨ��⳨���٦b���ܤ@�w���|�o�˪�") :)   : 50,
		(: do_command("glare ������|����ʤߤj�N��") :) : 50,
		(: do_command("why") :) : 50,
		(: do_command("inn") :) : 50,
		(: do_command("mutter �̪�u�I...") :) : 50,
	    ]));
}
void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{
	if( !objectp(me) || !msg ) return;

	if( strsrch(msg, "��") != -1 && strsrch(msg, "����") < 0 || strsrch(msg, "����") != -1)
		do_command("say �ڡH�ڥu�O�����Ӥw�աA�ڷQ���ӧ䤣�^�ӤF�աA�ڪ����񤣨��F");
		
	if( strsrch(msg, "����") != -1 )
		do_command("say ����o����ڡA���⳨��q�ڶ}�l�ǳ����N�@�����ڨ�{�b�A�]���O���򯫩_������A���L�N�O����F�I�}�F..");

	if( strsrch(msg, "�ʤߤj�N") != -1 )
		do_command("say ���Ӹܪ��ڡA�e�}�l�ڨ�s�̥h�˳��A�ܤF�I�s��N�I�I�j�ΡA���G���ӧڪ�����N�����F�A�ڹ�b�n���L��...");
}

void reply_emote(object me, string verb, string args)
{
	if( objectp(me) ) {
		switch(verb) {
			case "hi":
				do_command("sigh " +me->query_id(1)+ " �A�n�A������ƶ�");
				break;
			case "?":
				do_command("eh " +me->query_id(1)+ " �ݤ���ڡA�߱����O�ܦn�O�Ӷç�" );
				break;
			case "hmm":
				do_command("say ���F�H������ƶ�...");
				break;
			default:
				do_command("? " +me->query_id(1)+ " �ڭ̡D�D���O�ܼ��a�H����Q�F�H");
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
	if( !objectp(ob) ) return;

	if( me->query_quest_step(QUEST_BOHR) == 2 && query("behavior") && base_name(ob) == __DIR__"broken_rod"  )
	{
		destruct(ob);
		
		if( query_temp("quest/"QUEST_BOHR"/running", me) )
			return;

		set_temp("quest/"QUEST_BOHR"/running", 1, me);

		// ����D�ʦ欰
		delete_behavior();

		process_function(({
			4,
			(: target_gone($(me)) :),
			(: do_command("wa") :),
			4,
			(: target_gone($(me)) :),
			(: do_command("roll") :),
			(: do_command("scream "+$(me)->query_id(1) ) :),
			4,
			(: target_gone($(me)) :),
			(: do_command("say �o�D�D�o�⳨�񤣴N�O�ڪ��ܡH"+pnoun(2, $(me))+"�S�a�����٧ڪ��ܡH") :),
			4,
			(: target_gone($(me)) :),
			(: do_command("sob "+$(me)->query_id(1)+" "+pnoun(2, $(me))+"�u�O�Ӧn�H") :),
			4,
			(: target_gone($(me)) :),
			(: do_command("say �u�O�ӷP�E�F�A�ڸӫ�����"+pnoun(2, $(me))+"�O�C") :),
			(: do_command("ponder") :),
			6,
			(: target_gone($(me)) :),
			(: do_command("say �o�˦n�F�A�ڧ�ڪ������߱o���O�e"+pnoun(2, $(me))+"�n�F�A���M���O����j§�A���o�]�O��F�ڤQ�X�~���F��C") :),
			2,
			(: target_gone($(me)) :),
			(: new(__DIR__"fishing_note")->move($(me)) :),
			(: msg("$ME�o��F�������O�C\n", $(me), 0, 1) :),
			(: $(me)->add_title(HIW"����"NOR WHT"�F�H"NOR) :),
			(: delete_temp("quest/"QUEST_BOHR"/running", $(me)) :),
                        (: $(me)->set_quest_finish(QUEST_BOHR, QUEST_BOHR_NAME, pnoun(2, $(me))+"���i�����F�L������A�o��F�@�Ǽ��y�C") :),
			(: !wizardp($(me)) && CHANNEL_D->channel_broadcast("city", $(me)->query_idname()+"�Ѷ}�F�u"QUEST_BOHR_NAME"�v", $(me)) :),
			4,
			(: target_gone($(me)) :),
			(: msg("�u��$ME���_�˻s����p���l�l�������ۤW�����ǹСA�N�}����V���V�G�A��̫�~�M�㰦���x����"HIY"����"NOR"�C\n"NOR, this_object(), 0, 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg("$ME����������"HIY"����"NOR YEL"�p����"NOR"�D�`���Ī����R�ۡC\n"NOR, this_object(), 0, 1) :),
			(: do_command("mutter �o�U�l�ڤS�i�H�A�~�򪺳����F") :),
			(: do_command("thank "+$(me)->query_id(1) ) :),
			// ��_�D�ʦ欰
			(: set_behavior() :),
		    }));
		destruct(ob);
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
	set_idname("bohr",HIG"�i"NOR GRN"��"NOR);
	
	set("long", "�@��߷R�������~��k�l�A���ݥL�T�ܭW�y���ˤl���G���J��@�ǧx���C");
	set("unit", "��");
	set("age", 48);
	set("gender", MALE_GENDER);

	// �ҰʥD�ʦ欰
	set_behavior();

	startup_living();
}
                                                  
