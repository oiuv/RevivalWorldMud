/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : yin_ling_yu.c
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
		(: do_command("inn ������|�o��...") :)   : 50,
		(: do_command("cold") :) : 50,
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
		do_command("say "+pnoun(2, me)+"������H");
	
	else if( me->query_quest_step(QUEST_YIN_LING_YU_2) == 2 && strsrch(msg, "�H��") != -1 )
	{
		do_command("say "+pnoun(2, me)+"�O���ڵ�"+pnoun(2, me)+"������H�ѶܡH���O�ڦb�L���˪L��誺�@�B���K�s�}���o�{��");
		do_command("shiver");
		do_command("say �p�G...�p�G���O���F�Z���A�ڮڥ��S���i��i�J���i�Ȫ��s�}��...��...�Z��");
		
		me->set_quest_step(QUEST_YIN_LING_YU_2, 3);
		me->set_quest_note(QUEST_YIN_LING_YU_2, QUEST_YIN_LING_YU_2_NAME, "���¦ЦV"+pnoun(2, me)+"���_�H�Ѫ��Ӿ��O�b�L���˪L��誺�@�����K�s�}���C");
	}
	else if( strsrch(msg, "�_��") != -1 )
		do_command("sob "+me->query_id(1)+" ��I��I���_��I�N�O�L�A�֥h�ϥL���I�L�b�j��...��...�֪��I");
	else if( strsrch(msg, "�Z��") != -1 )
		do_command("sob "+me->query_id(1)+" �Z���L�w�g���F...���...��ө��_��L...��...���O�ڪ���...");
	else if( strsrch(msg, "�H��") != -1 )
		do_command("say �H�ѡH"+pnoun(2, me)+"�����⳥�s�H�ѶܡH�i�H�����@�@�ܡH");
	else
		do_command("say "+pnoun(2, me)+"������H");
}

void reply_emote(object me, string verb, string args)
{
	if( !objectp(me) ) return;
	
	//do_command(verb + " " + me->query_id(1) + " " + (args||""));
}

int target_gone(object me)
{
	if( same_environment(this_object(), me) )
		return 1;
	
	do_command("say �H�O�H�o�@�ɤW���S�H�n�z�ڤF��...");
	
	if( objectp(me) )
	{
		tell(me, this_object()->query_idname()+"�b����I�۵ۡG�H�O�H�o�@�ɤW���S�H�n�z�ڤF��...\n");
		delete_temp("quest/"QUEST_YIN_LING_YU"/running", me);
	}
	
	set_behavior();
	return -1;
}

void reply_get_object(object me, object ob, mixed amount)
{
	if( !objectp(ob) ) return;
	
	if( me->query_quest_step(QUEST_YIN_LING_YU)==3 && query("behavior") && base_name(ob) == "/quest/yin_ling_yu/wild_ginseng" )
	{
		destruct(ob);
		
		if( query_temp("quest/"QUEST_YIN_LING_YU"/running", me) )
			return;
		
		set_temp("quest/"QUEST_YIN_LING_YU"/running", 1, me);
		
		// ����D�ʦ欰
		delete_behavior();
		
		process_function(({
			2,
			(: target_gone($(me)) :),
			(: msg("$ME�b�b�a�ݵ۳��s�H�x�A$YOU�i�D�o���_��w�g���`���T���C\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg("$ME�}�l����z�a���_�@�q�G�ơC\n\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���g���ӥ��Z���A�ҡA�W�s"HIW"�}�Z��"NOR"\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�j�ۯ��¦ФT�����L�O���¦Ъ��F�~�A�q�p��H�N�O"HIM"�C��"NOR MAG"�˰�"NOR"\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���b���¦ФQ�������ɭԡA���¦ЫK�M�a�H�h��s�W�L��"HIG"��"NOR GRN"�A"NOR"���ͬ�\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���¦Ъ��D�}�Z�����餣�n�A�o���ۦp���ʭ����u�@�A�K�ɱ`��s�}�U���U�L\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�N�o�˹L�F�n�X�~\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���¦ФG�Q�G���F�A"HIY"�F�F"NOR YEL"�ɥ�"NOR"���o�̵M�`�`�U�s���U�}�Z��\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���Ʊ��N�o�ͦb�e�X��\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�}�Z����M��ۯ��¦�"HIW"�j�q"NOR"�G�p��b�ܷСA���n�A�ӤF�n���n\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���ۻ��ۨ}�Z����L���y�µۡA�L����W�G�ۤ@��"HIR"�A"NOR RED"��"NOR"�A�����¦ШS���ݨ�\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���¦ЫD�`�ˤߦa�^��s��\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���D�}�Z�����A���w�o�F��\n", this_object(), $(me), 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�o�ɦb�s���W\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�J��F���b�s�ϿW�ۭ��C��"HIW"���_��"NOR"\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���_��O�Ӧ����b�c�a��������"HIG"�T�N"NOR GRN"�C�~"NOR"\n", this_object(), $(me), 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���M�O�즸����\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�����_�娣��F�������L�����¦�\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�̵M�j��a�w���ۯ��¦�\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���¦Ф]���D���_�媺�߫�\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���W�K����_�����F�o�w�g���k�B�ͪ���\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���_�寺���I�F�I�Y\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�Ѧ�"WHT"�w��"NOR"\n", this_object(), $(me), 1) :),
			3,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���_��b�U�s�e�ݤF���¦Ц��S������ݭn�������a��\n", this_object(), $(me), 1) :),
			7,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���¦зQ�F�Q\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�K���U�F���_�嶶�D���o�e�@��"HIY"���s"NOR YEL"�H��"NOR"��s�}�U���}�Z���a��\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���_��ˤ����I�F�I�Y�A���ۤ@��"HIY"���s"NOR YEL"�H��"NOR"���}�F\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���¦Ф߱��]�n�F�\�h�A�K�^��a��\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�j��@��\n", this_object(), $(me), 1) :),
			3,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���¦ФS��ߦa�U�s�h��}�Z���A�Ʊ�M�L�M�n�p��\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�Z���@��}�Z���a��\n", this_object(), $(me), 1) :),
			3,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�o���}�Z��"HIR"��"NOR RED"�f"NOR"���ۧ�M�A�w�M��\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"����o�@�������¦бY��F�A�o���A�Ѭ�����|�o�ͳo�بơA�O�ֿѱ��F�L�I�H\n", this_object(), $(me), 1) :),
			10,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���¦зQ�ۡA�������B�X�@�ӤH�y\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���_��I�L���M�O��"HIY"��"NOR YEL"�H��"NOR"�I\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���¦Ф@���^���b�}�Z�����骺���ǡA�O�o�`�F�L\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�o���ۡA"HIM"��"NOR MAG"��"NOR"�������o���_�M�l\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���۪��f��"HIR"��"NOR RED"��"NOR"�չϴM����_�媺�U��\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"¶�F�n�X���s��\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"��M���e�X�{�@�Өk�l�A�����æb�@��"HIW"�j"NOR WHT"��"NOR"��\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���O�Q�Ѩ��ө��_��\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"����F�����A�媺���_��\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�N�O�L���F�}�Z���I\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���¦�"WHT"Ÿ��"NOR"�ۡA���o�@�y�a������_�娭��\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�r�M���M�����_�娭�W��h\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�M�l�N�o�ˤ@�n���T�a�i�F���_�媺����\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���_��h�W�a�j�s�A"HIY"��"NOR YEL"��"NOR"�a�^�Y�ݤF��\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���M�O�o�A�o�O������...\n", this_object(), $(me), 1) :),
			10,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"��M�j���t�@�ݥX�{�@�s"HIW"�s"NOR WHT"��"NOR"�j�۵ۡG�b���I���ӤH�b���I�l���I�I\n", this_object(), $(me), 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���_��N���¦Ъ����l�ĤO�@��\n", this_object(), $(me), 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�Ϥ�ޥX�I�᪺�M�l�A"HIR"�A"NOR RED"��"NOR"���Q�ӥX�A�q�o���¦к����A��\n", this_object(), $(me), 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���_���ۯ��¦м��n�j�ۡG"HIY"�]���I�ֶ]�I"NOR"\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���۫K���ۤM�l���s�騺�ĥh...\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���¦��~�a�F�A�o�����D�O���@�^�ơA������|�o�ˡA���e�u�����_���a�g�����X���ۤM�l���b�M�s��̥�����\n", this_object(), $(me), 1) :),
			10,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���_�廷���ǨӼR���n�G"HIY"�ֶ]�I�I���ֶ]���I�I"NOR"\n", this_object(), $(me), 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���¦гQ�~�o�ਭ�N�]�A�]�L�F"HIG"��"NOR GRN"��"NOR"�A�]�L�F"HIB"�s"NOR BLU"��"NOR"�A���\�q�S�����L\n", this_object(), $(me), 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�ש�"WHT"��ɤO��"NOR"�A�˦b�a�W\n", this_object(), $(me), 1) :),
			12,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"�]�����L�F�h�[\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"�G��"NOR YEL" - "NOR"���ӫ�K����F$YOU...\n\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg("$ME�_�_����a�����o�q�G�ơA���_�y�X�����\���R���ۮ���\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg("$ME��⤤�����s�H�x�浹�F$YOU\n", this_object(), $(me), 1) :),
			2,
			(: target_gone($(me)) :),
			(: do_command("say �o�⳥�s�H�x�ڤw�g�S�ΤF�A�N���F"+pnoun(2, $(me))+"�a ") :),
			(: new("/quest/yin_ling_yu/tear_wild_ginseng")->move($(me)) :),
			(: $(me)->set_quest_finish(QUEST_YIN_LING_YU, QUEST_YIN_LING_YU_NAME, "�J���F�y���¦Сz�A���o���F���_��A�����_��w�g���F�C") :),
			(: delete_temp("quest/"QUEST_YIN_LING_YU"/running", $(me)) :),
			(: !wizardp($(me)) && CHANNEL_D->channel_broadcast("city", $(me)->query_idname()+" �Ѷ}�F�u"+QUEST_YIN_LING_YU_NAME+"�v", $(me)) :),
			6,
			(: target_gone($(me)) :),
			(: msg("$YOU�ݵۤ�W�⳥�s�H�x�W�����۲\���A$YOU�Q�o�q�d�G���G�ƾ_�٤F�A�[�[���o�@�y�C\n", this_object(), $(me), 1) :),
			
			6,
			(: target_gone($(me)) :),
			(: msg("�^�Y�@�ݥu�����¦Чb�b�a�ݵۻ���A$YOUı�o�N���o�R�@�R����n...�C\n", this_object(), $(me), 1) :),
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
			do_command("drop all "+ob->query_id(1)),
		    }));
	}
}


void create()
{
	set_idname("yin ling yu",HIC"��"NOR CYN"�¦�"NOR);
	set_temp("status", HIC"����"NOR CYN"����"NOR);
	
	set("long", "�@�Ӷˤ߱������k�l�A�����D�o����p���ˤߡC");
	set("unit", "��");
	set("age", 22);
	set("gender", FEMALE_GENDER);
	
	// �ҰʥD�ʦ欰
	set_behavior();
	
	startup_living();
}
