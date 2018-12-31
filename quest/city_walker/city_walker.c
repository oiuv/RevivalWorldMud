/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : city_walker.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-27
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
	object ob;

	if( !objectp(me) || !msg ) return;

	if( strsrch(msg, "����") != -1 )
	{
		do_command("say �����O�ӤH��������j���l...�Ʊ�b�ڦ��ͤ��~�̥i�H�C�M�@�ɦU�j����...�ݺɦU�ؤH���ͬ�...�C");
		return;
	}
		
	if( strsrch(msg, "�C") != -1 )
	{
		do_command("say ��...�O���A�ȹC�O�ڳ̳��w���@��ƤF...");
		do_command("say �ٰO�o�Q�G�~�e���J��@��M�ڤ@�˫ܳ��w��B�Ȧ檺�֤H...��{�b���M���h���L���@���֩O�C");
		
		return;
	}

	if( strsrch(msg, "��") != -1 )
	{
		do_command("say �L���֬O�o�˼g��..."+@TEXT

�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w
 �C���ݨ�j��½�s�A��D�ܱo���R�A�a�K�y�Z����ڭ̱a��������
 ���P�ϰ�A�ߤ��A�N�|�Ԥ����@�Ӥ����_�����ڡG
 �M��A�����K��ɨ��}�ɪ��M�s�E�E�E
 ���H�B�ձy���E�E�E�g�T�x���J��L�ߡE�E�E�ѵ�O�H����h�j�E�E�E
 ���饫�e�u�����R�A�O�a�y�����Y�v�A���ڭ̻P�@�ɦP�B�߰ʡE�E�E
 �̭��n���A�L�ר�����̡A����I�l��@�����H��𮧡G
 �@�b�������h���F��誺�ڷQ�B�@�b�֦����ۥ��������I
�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w
	
TEXT
	);
		do_command("say �h��O�H�`�誺�@���֪��I�o���֪��W�r�O...�x�A�V�|�ڳ��M�ѤF�o���֪��W�r�F...");
		
		return;
	}

	if( strsrch(msg, "�W�r") != -1 )
	{
		do_command("sigh");
		do_command("say �o��n���@���֧ڳ��M�ѤF�����W�r...");
		
		if( query("total_online_time", me) < 24*60*60*30 )
			return tell(me, pnoun(2, me)+"���W�u�ɶ����� 30 �ѵL�k�i�榹�����ȡC\n");

		if( me->query_quest_step(QUEST_CITY_WALKER) == 0 )
		{
			if( me->query_quest_finish(QUEST_CITY_WALKER) && time() < me->query_quest_time(QUEST_CITY_WALKER) + 60*60*24)
				return tell(me, pnoun(2, me)+"�b�̪񪺤@�Ѥ����~��o�ӤH��͹L�C\n");
			
			me->set_quest_note(QUEST_CITY_WALKER, QUEST_CITY_WALKER_NAME, "�����C�H���G���L�ѰO���֦W�۷�b�N�A�γ\�o�@�ɤW���H���D�C");
			me->set_quest_step(QUEST_CITY_WALKER, 1);
		}
		
		return;
	}
	
	if( me->query_quest_step(QUEST_CITY_WALKER) == 3 && strsrch(msg, "��@�Ӥ����_������") != -1 )
	{			
		do_command("ah");
		do_command("say ��I��I�N�O�o�ӦW�r�A��...�p���A�A�{�b�@�w���b���ӫ����̼g�֧a...");
		do_command("say �ڷQ"+pnoun(2, me)+"�]�O�q�j�����Ӫ��a�I�ڳo�̦��ӪF���"+pnoun(2, me)+"�ӻ����ӫܹ�ΡA�N��"+pnoun(2, me)+"�a�C");
		
		ob = new("/quest/city_walker/land_essence");

		msg("$YOU�N"+ob->query_idname()+"�浹$ME�C\n", me, this_object(), 1);
		
		ob->set_keep();
		ob->move(me);

		me->set_quest_finish(QUEST_CITY_WALKER, QUEST_CITY_WALKER_NAME, "�������C�H���F�֦W�A�o��F�@�Ǽ��y�~�C");
		CHANNEL_D->channel_broadcast("city", me->query_idname()+" �Ѷ}�F�u"+QUEST_CITY_WALKER_NAME+"�v", me);
		return;
	}
}

void reply_emote(object me, string verb, string args)
{

}

int target_gone(object me)
{

}

void reply_get_object(object me, object ob, mixed amount)
{

}

void create()
{
	set_idname("city walker",NOR CYN"�����C�H"NOR);

	set("unit", "��");
	set("age", 32);
	set("gender", MALE_GENDER);
	set("long", "�@�ӥ|�B�Ȧ檺�����C�H�C");
	
	// �ҰʥD�ʦ欰
	set_behavior();

	startup_living();
}
