/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : girl.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-10
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
inherit "/quest/old_farmer/fuck_robotking.c";

void do_command(string cmd);
void make_money();

void set_behavior()
{
	//set("behavior/walking", 120);
	
	set("behavior/random", 
	    ([
		(: make_money :) : 50,
		(: do_command("inn �{�l�n�j��...") :)   : 50,
		(: do_command("cough") :)               : 50,
		(: do_command("sigh �̪񴺮�t�A�u�@���S�F...") :) : 50,
		(: this_object()->drop_money("RW", 1) && msg("$ME���f�U�̦��G���X�F�@�ǪF��A���L�èS���`�N��...\n", this_object(), 0, 1) :) : 50,
	    ]));
}

void make_money()
{
	int i, m = random(5) + 1;
	object ob, *obs;
	
	obs = obs = present_objects(this_object());
	obs = filter(obs, (: $1->is_user_ob() :));
	if( !(i = sizeof(obs)) ) return;
	ob = obs[random(i)];
	process_function(({
		(: msg("$ME�n���^�������U�V$YOU�Q���C\n", this_object(), $(ob), 1) :),
		4,
		(: (objectp($(ob)) && environment($(ob)) == environment(this_object())) ? 1 : -1 :),
		(:
		    $(ob)->spend_money("RW", $(m)) &&
		    msg("$YOU�@�ɤ߳n�q�f�U�ǥX�F "HIY"$RW " + $(m) + NOR" ��$ME�C\n", this_object(), $(ob), 1) ||
		    this_object()->earn_money("RW", $(m)) &&
		    do_command("kneel " + $(ob)->query_id(1))
		:),
	    }));
}

void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{
	if( same_environment(this_object(), me) && msg && strsrch(msg, "��") != -1 )
		do_command("nod "+me->query_id(1)+" ��I��I�ڦn�Q�Y����...");
}

void reply_emote(object me, string verb, string args)
{
	if( same_environment(this_object(), me) )
		do_command(verb + " " + me->query_id(1) + " " + (args||""));
}

int target_gone(object me)
{
	if( same_environment(this_object(), me) )
		return 1;
	
	do_command("say �áH�I�H�]���h�F�O�A��ӬO�Ӥ��D�^�����j�n�H...�u�n�u�n...");
	
	if( objectp(me) )
		delete_temp("quest/"QUEST_OLD_FARMER"/running", me);
	
	set_behavior();
	return -1;
}

void reply_get_object(object me, object ob, mixed amount)
{
	if( !objectp(ob) ) return;
	
	if( base_name(ob) == "/obj/farming/product/rice" )
	{
		if( time() - me->query_quest_time(QUEST_OLD_FARMER) < 60*60*24 )
		{
			destruct(ob);
			return do_command("say �ڡI"+pnoun(2, me)+"���O�W�����Ӥj�n�H�ܡH�u��������"+pnoun(2, me)+"�S���ڶ��Y�A�u�O���¤F");
		}
		
		// ����@�ǥ�è��ѥΤ��P char �Ѥ@�˪� quest by Msr
		if( check_login_ip(me) )
		{
			destruct(ob);
			return do_command("say �ڡI"+pnoun(2, me)+"���O�W�����Ӥj�n�H�ܡH�u��������"+pnoun(2, me)+"�S���ڶ��Y�A�u�O���¤F");
		}
		if( query("total_online_time", me) < 24*60*60 )
		{
			destruct(ob);
			return tell(me, pnoun(2, me)+"���W�u�ɶ����� 1 �ѵL�k�i�榹�����ȡC\n");
		}
		
		if( query_temp("quest/"QUEST_OLD_FARMER"/running", me) )
		{
			destruct(ob);
			return;
		}
		
		set_temp("quest/"QUEST_OLD_FARMER"/running", 1, me);
		
		delete_behavior();
		
		process_function(({
			2,
			(: target_gone($(me)) :),
			(: do_command("wa") :),
			2,
			(: target_gone($(me)) :),
			(: do_command("thank "+$(me)->query_id(1)+" "+pnoun(2, $(me))+"�u�O�Ӥj�n�H�o") :),
			
			4,
			(: target_gone($(me)) :),
			(: do_command("say "+pnoun(2, $(me))+"�������A���ڦn�n�Q�Q�n������"+pnoun(2, $(me))) :),
			(: do_command("ponder") :),
			5,
			(: target_gone($(me)) :),
			(: do_command("ah") :),
			2,
			(: target_gone($(me)) :),
			(: do_command("say ���M�o���O����j���F���F��A�������O�ڤ��Q�h�~�Ӫ��M��...") :),
			2,
			(: target_gone($(me)) :),
			(: do_command("say ��"+pnoun(2, $(me))+"��褣�t�A���ڱN���ͪ��A�~�g��б©�A�a�I") :),
			2,
			(: target_gone($(me)) :),
			(: msg("$ME�@�f�����z�F���Q�h�~�Ӫ��A�@�g��A$YOU��o�F�j�q���|�g��ȡC\n", this_object(), $(me), 1 ) :),
			(: $(me)->add_social_exp(300000+random(300000)) :),
			(: delete_temp("quest/"QUEST_OLD_FARMER"/running", $(me)) :),
			(: !wizardp($(me)) && CHANNEL_D->channel_broadcast("city", $(me)->query_idname()+" �Ѷ}�F�u"+QUEST_OLD_FARMER_NAME+"�v", $(me)) :),
			(: $(me)->set_quest_finish(QUEST_OLD_FARMER, QUEST_OLD_FARMER_NAME, "�J���F�y�Ⱦj���ѹA�ҡz�A���F�L���Y�A�o��F�y���|�g��z�C") :),
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
			do_command("drop all "+ob->query_id(1)),
		    }));
	}
}

void reply_get_money(object me, string unit, mixed money)
{
	return do_command("say ����"+pnoun(2, me)+"�I"+pnoun(2, me)+"�u�O�Ӥj�n�H�A�����D"+pnoun(2, me)+"�i���i�H���ڤ@�f���Y�O�H");
}

void create()
{
	set_idname("old farmer","�Ⱦj���ѹA��");
	
	set("unit", "��");
	
	set("age", 78);
	set("gender", MALE_GENDER);
	set("money/RW", "1000000");
	set("title", HIG"�A��"NOR);
	
	// �ҰʥD�ʦ欰
	set_behavior();
	
	startup_living();
}
