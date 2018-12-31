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
			(: do_command("sweat") :)   : 50,
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
		do_command("say ��H"+pnoun(2, me)+"�������ť���M��");
	
	else if( strsrch(msg, "�_��") != -1 )
		do_command("say ���O�֧r�H�ڤ��{�ѭC");
		
	else if( strsrch(msg, "�Z��") != -1 )
		do_command("say �i�����Ĥl�A�Z���L�N��b�s�}�U�A�u���L�O��²�몺�A�ҡA�����M�b�a���Q�H���`");
		
	else if( strsrch(msg, "�¦�") != -1 )
	{
		do_command("sigh �¦Ъ�...");
		do_command("say �̪���l�̾x�o�m�m�����A�j�a�����O�¦б��F�Z����K�k�h�L�ܡA���ڪ��D�¦Цo���|���o�بƪ�");
		do_command("say �ڬO�ݵۦo���j���A�o�O�o�򵽸ѤH�N���i��|�����ب�...");
	}
	else if( strsrch(msg, "�H��") != -1 )
		do_command("say �@�H"+pnoun(2, me)+"�O�����s�H�ѶܡH���O�¦Цb�@�B���äs�}���o�{���A������H�Ѧ��G�èS������S�O���A�¦Ш��ɭԬݨ�]�N�H����F�^��");
	else if( strsrch(msg, "�s�}") != -1 )
		do_command("say �ڤ]���M�����Ӥs�}�b���̡A�ڥu���D���O�¦Ыe�@�}�l��`�s����M�v���Z�����ħ��ɧ�쪺");
	else if( strsrch(msg, "�ħ�") != -1 )
		do_command("say �����ħ��N�O�ڪ��M���F�A�z�n���n�]�Ӹոժ��A�ڳo�̥i�����ֺ��ħ��A�]�z�Ĩ�f�����A�z����");
	else if( strsrch(msg, "��") != -1 )
	{
		do_command("say �ӨӨӡA�o���īO�ұj������");
		msg("$ME�Q$YOU�j��F�@�����D�������į�A$ME�uı�@�}�Y�w�دt�C\n", me, this_object(), 1);
		me->faint();
		do_command("say �x�H�ݨӳo���Ĥ��j��A�ڴN����...�~���H����n�n���N���n�æY��");
	}
	else if( strsrch(msg, "�v��") != -1 )
	{
		do_command("say �Z�����f�ܩ_�ǡA�ڤ]���g�h�ݹL�L�X���A���]�L�q�_�w�L���f�]");
		do_command("say �γ\...�Ф��a�@����R���P�k����k�v�o�n�L�A���L�{�b���o�ǳ��ӿ�F");
	}
	else if( strsrch(msg, "�s��") != -1 )
		do_command("say �o�Ӧ~�Y�٦�����s��ܡH�ڤw�g�n�[�S�ݨ��F");
	else if( strsrch(msg, "�A�n") != -1 )
		do_command("say �����A�n�O�¦�¸���Z���Ϊ��A�o��������u���O����");
	else if( strsrch(msg, "�P�k") != -1 )
	{
		do_command("say �o��b�Ӻ}�G�F�A��N�S�O���������A�ҥH���`�O�٦o���P�k�A������P�k�`�O�Ʃ����w�A�ڤ]����o�o�b����");
		
		if( me->query_quest_finish(QUEST_YIN_LING_YU) && me->query_quest_step(QUEST_YIN_LING_YU_2) == 0)
		{
			me->set_quest_note(QUEST_YIN_LING_YU_2, HIC"���¦�"NOR CYN"������"NOR, "�J���F���ĦѤӱC�A���D�F�Ф��a�P�k���s�b�C");
			me->set_quest_step(QUEST_YIN_LING_YU_2, 1);
		}
	}
	else
		do_command("say ��H"+pnoun(2, me)+"�������ť���M��");
}

void reply_emote(object me, string verb, string args)
{
	if( !objectp(me) ) return;
}

void reply_get_object(object me, object ob, mixed amount)
{
	if( !objectp(ob) ) return;

	process_function(({
		1,
		do_command("say �o�O����H�ڤ��n�o�ӪF��..."),
		1,
		do_command("drop all "+ob->query_id(1)),
	}));
}


void create()
{
	set_idname("grandam", "�ѤӱC");
	set_temp("status", HIY"����"NOR YEL"����"NOR);

	set("long", "�@��b�s������åH���Ĭ��ͪ��ѤӱC�A�γ\�o���D�@�ǨƱ��A�յ۰ݰݬ�(say)�o�a�C");
	set("unit", "��");
	set("age", 62);
	set("gender", FEMALE_GENDER);

	// �ҰʥD�ʦ欰
	set_behavior();

	startup_living();
}
