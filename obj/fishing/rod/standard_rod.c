/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : rod.c
 * Author :
 * Date	  : 2003-5-3
 * Note	  : Just For Funny
 * Update :
 *  o 2000-00-00

 -----------------------------------------
 */


#include <ansi.h>
#include <message.h>
#include <inherit.h>
#include <map.h>
#include <daemon.h>
#include <delay.h>

inherit	STANDARD_OBJECT;
// �B�z�񳽻�
void do_hook(object me, string arg, object rod)
{
	object bait;

	if ( !arg )
		return tell(me,	pnoun(2, me)+"�Q�n�񤰻�F��b"+rod->short(1)+"�����_�W�H\n");

	if( !objectp(bait = present(arg, me)) )
		return tell(me,	pnoun(2, me)+"���W�S�� "+arg+" �o�تF��\n");

	if( !bait->is_bait() )
		return tell(me,	rod->query_idname()+"�������@����C\n");

	if( me->is_delaying() )
		return tell(me,	me->query_delay_msg());

	if( !query_temp("bait", rod) )
	{
		msg("$ME��"+bait->query_idname()+"���"+rod->query_idname()+"�����_�W�C\n", me,	0, 1);
		set_temp("bait", query("bait", bait), rod);
		set_temp("status", NOR"- "HIY"����"+bait->query_name()+NOR, rod);
		destruct(bait, 1);
	}
	else
		return tell(me,	rod->query_idname()+"�W�w�g������F�C\n");
}

// �B�z�_�c�ƥ�
void check_yn(object me, string arg)
{
	object ob;
	int i;

	me->remove_delay(FISHING_DELAY_KEY);

	if( arg == "" || lower_case(arg) == "y" )
	{
		switch( random(7) )
		{
			case 0:
				i = range_random(4, 10);
				ob = new("/obj/fishing/sp_fish/fish"+random(3));
				set_temp("amount", i, ob);
				msg("\n$ME�ߤ@��A�ĤO���@�ԡA��"+ob->short(1)+"�q�����ԤF�_�ӡA��"+this_object()->query_name()+"�~�M�@�v���ˡC\n", me, 0, 1);
				ob->move(me);
				break;
			case 1:
				i = range_random(8, 16);
				ob = new("/obj/fishing/sp_fish/fish"+random(3));
				set_temp("amount", i, ob);
				msg("\n$ME�ߤ@��A�ĤO���@�ԡA��"+ob->short(1)+"�q�����ԤF�_�ӡA��"+this_object()->query_name()+"�o�X�y�ԡz���@�n�Ať�_�Ӧ��G����ܤj���ˮ`�C\n", me, 0, 1);
				if( addn_temp("endurance", -1000) < 1 )
					set_temp("endurance", 10);
				ob->move(me);
				break;
			case 2:
				i = range_random(4, 10);
				ob = new("/obj/fishing/adv_fish/fish"+random(4));
				set_temp("amount", i, ob);
				msg("\n$ME�ߤ@��A�ĤO���@�ԡA��"+ob->short(1)+"�q�����ԤF�_�ӡA��"+this_object()->query_name()+"�~�M�@�v���ˡC\n", me, 0, 1);
				ob->move(me);
				break;
			case 3:
				i = range_random(8, 16);
				ob = new("/obj/fishing/adv_fish/fish"+random(4));
				set_temp("amount", i, ob);
				msg("\n$ME�ߤ@��A�ĤO���@�ԡA��"+ob->short(1)+"�q�����ԤF�_�ӡA��"+this_object()->query_name()+"�o�X�y�ԡz���@�n�Ať�_�Ӧ��G����ܤj���ˮ`�C\n", me, 0, 1);
				if( addn_temp("endurance", -600) < 1 )
					set_temp("endurance", 10);
				ob->move(me);
				break;
			case 4:
				if( query_temp("endurance") > 1000 )
				{
					check_yn(me, "y");
					return;
				}
				ob = new("/obj/fishing/other/box");
				msg("\n$ME�ߤ@��A�ĤO���@�ԡA��"+ob->short(1)+"�q�����ԤF�_�ӡA��"+this_object()->query_name()+"�o�]���L�k�Ө����O���_���F�C\n", me, 0, 1);
				set_temp("endurance", -1);
				ob->move(me);
				break;
			case 5:
				ob = new("/obj/fishing/other/box");
				msg("\n$ME�ߤ@��A�ĤO���@�ԡA��"+ob->short(1)+"�q�����ԤF�_�ӡA��"+this_object()->query_name()+"�o�X�y�ԡz���@�n�Ať�_�Ӧ��G����ܤj���ˮ`�C\n", me, 0, 1);
				if( addn_temp("endurance", -600) < 1 )
					set_temp("endurance", 10);
				ob->move(me);
				break;
			case 6:
				ob = new("/obj/fishing/other/box");
				msg("\n$ME�ߤ@��A�ĤO���@�ԡA��"+ob->short(1)+"�q�����ԤF�_�ӡA��"+this_object()->query_name()+"�~�M�@�v���ˡC\n", me, 0, 1);
				ob->move(me);
				break;
		}
	}
	else
		tell(me, pnoun(2, me)+"���ϫl�ΤO�ԡA�⳽�u���P�A����b���_�W�������]�F�C\n");
}
// �B�z�N�n����F��
void get_fish(object me)
{
	int rod_int, bait_int, fish_sk, break_int;
	object ob;
	if( !objectp(me) ) return;

	me->remove_delay(FISHING_DELAY_KEY);

	rod_int = query("rod_level");
	bait_int = query_temp("bait");
	fish_sk	= to_int(me->query_skill_level("fishing")/1.5);

	if( !random(200) )
		fish_sk += 50;

	// �ü�(���񵥯�) + �����ޯ൥�� + ���絥��
	switch(random(rod_int)+fish_sk+bait_int)
	{
		case 0..19:
			ob = new("/obj/trash/trash"+random(6));
			msg("\n$ME�@��"+this_object()->query_name()+"�_�Ӥ@�ݡA�~�M����F�D�D�D�D"+ob->query_idname()+"�C\n", me, 0, 1);
			break;
		case 20..39:
			msg("\n$ME�@��"+this_object()->query_name()+"�_�Ӥ@�ݡA�~�M���򳣨S����C\n", me, 0, 1);
			break;
		case 40..89:
			ob = new("/obj/fishing/fish/fish"+random(9));
			msg("\n$ME�@��"+this_object()->query_name()+"�_�Ӥ@�ݡA�~�M����F�D�D�D�D"+ob->query_idname()+"�C\n", me, 0, 1);
			break;
		case 90..109:
			ob = new("/obj/fishing/adv_fish/fish"+random(4));
			msg("\n$ME�@��"+this_object()->query_name()+"�_�Ӥ@�ݡA�~�M����F�D�D�D�D"+ob->query_idname()+"�C\n", me, 0, 1);
			break;
		case 110..129:
			ob = new("/obj/fishing/sp_fish/fish"+random(3));
			msg("\n�@�ѫe�ҥ������j�O�ԫl��$ME�t�I���h���ųQ�ԤJ���̡A��$ME�g�L�\�[�񰫡A�ש��"+ob->query_idname()+"�԰_�ӤF�C\n",	me, 0, 1);
			break;
		case 130..132:
                  	msg("\n�u��$ME�@���ϫl�ΤO���ԡA�i�O�٬O�����Ԥ��_�ӳo���쪺�F��C\n", me, 0, 1);
			tell(me, pnoun(2, me)+"�n�Ϻɥ��O�ΤO���ԶܡH("HIY"Y"NOR"/n)\n");
			input_to( (: check_yn, me :) );
			break;
		case 133..9999:
			ob = new("/obj/fishing/sp_fish/fish"+random(3));
			msg("\n�@�ѫe�ҥ������j�O�ԫl��$ME�t�I���h���ųQ�ԤJ���̡A��$ME�g�L�\�[�񰫡A�ש��"+ob->query_idname()+"�԰_�ӤF�C\n",	me, 0, 1);
			break;
	}

	delete_temp("bait");
	delete_temp("status");

	if( objectp(ob)	&& !me->get_object(ob)	)
	{
		msg("$ME�����ʨ���h�F��F�A�u�����_�Ӫ�"+ob->query_idname()+"�S���^�e�̡C\n", me, 0, 1);
		destruct(ob);
	}
	// �B�z����O�_�n�_�����v
	break_int = (100/(fish_sk||1));
	if( break_int < 5 ) break_int = 5;
	if( break_int > 15 ) break_int = 15;

	if( addn_temp("endurance", range_random(-break_int, 1),this_object() ) < 0 )
	{
		switch( random(3) )
		{
			case 0..1:
				msg("\n$ME��W��"+this_object()->query_name()+"�o�X�y�ԡz���@�n�Ať�_�Ӧ��G�w�g���_�F�C\n", me, 0, 1);
				break;
			case 2:
				msg("\n$ME��W��"+this_object()->query_name()+"�o�X�y�ԡz���@�n�A�@�a���ݵo�{���w�g�_�F�C\n", me, 0, 1);
				set_temp("endurance", -1);
				break;
		}
	}
}

void fishing(object me)
{
	if( !objectp(me) ) return;

	if( !me->cost_stamina(5) )
	{
		me->remove_delay(FISHING_DELAY_KEY);
		return tell(me,	pnoun(2, me)+"�Ӳ֤F�A�L�k�~��M�ߪ������C\n");
	}

	me->add_social_exp(150+random(150));

	// �B�z�����ƥ�
	switch(	random(10) ) {
		case 0..2:
			msg("\e[1;"+3+random(8)+"m�B�Цb���W�ư��ƪ��A���G�S�����򲧪��o�͡D�D�D\n"NOR,	me, 0, 1);
			call_out( (: fishing, me :), 6);
			break;
		case 3..5:
			msg("\e[1;"+3+random(8)+"m�B�Ц��G�L�L���ʤF�A���S�h�[�S�����\�ʤF�D�D�D\n"NOR,	me, 0, 1);
			call_out( (: fishing, me :), 6);
			break;
		case 6:
			if(!query_temp("bait"))
				msg("\n\e[1;"+3+random(8)+"m�B�Ь�M�r�O���I�J�����A�i�O$ME�o�Ӥ��ΩԤW����A���N�o�˶]���F�D�D�D\n"NOR, me, 0,	1);
			else
			{
				msg("\n\e[1;"+3+random(8)+"m�B�Ь�M�r�O���I�J�����A�i�O$ME�o�Ӥ��ΩԤW����A����N�o�˳Q�Y���F�D�D�D\n"NOR, me, 0, 1);
				delete_temp("bait");
				delete_temp("status");
			}

			me->remove_delay(FISHING_DELAY_KEY);
			break;
		case 7..9:
			msg("��M���A�B�Ф������E�P�W�U�\�ʵۡA$ME����ΤO�@�ԡD�D�D\n", me, 0,	1);
			if( !me->cost_stamina(5) )
			{
				me->remove_delay(FISHING_DELAY_KEY);
				msg("��$ME�Ԩ�@�b�ɡA��M�Pı����L�O�A�u�n��󳨰_�o�����F�C\n", me, 0, 1);
			}
			else
				call_out( (:get_fish ,me :), 2);
			break;
	}
}
void do_fishing(object me, string arg)
{
	int a = me->query_skill_level("fishing");
	int b = query("level");
	object env = environment(me);
	array loc = query_temp("location", me);

	// �S�v
	if( wizardp(me) ) {
		msg("$ME���X"+this_object()->query_name()+"�A���e�誺�e���ΤO�@�ϥX��C\n", me, 0,	1);
		call_out( (: fishing, me :), 1);
		return;
	}

	// �_��������L�k����
	if( query_temp("endurance") < 0 )
		return tell(me, pnoun(2, me)+"�L�k���_����"+this_object()->query_name()+"�ӳ����C\n");

	// �L�k�b�D�a�Ϥ�����
	if( !env->is_maproom() )
		return tell(me,	pnoun(2, me)+"�L�k�b�ж��̳����C\n");

	if( environment() != me )
		return tell(me, pnoun(2, me)+"�����N"+this_object()->query_name()+"���b��W�~��}�l�����C\n");

	// �ޯ൥�Ť���L�k������
	if( a < b )
		return tell(me, pnoun(2, me)+"�������g�礣���A�����Ӧp��ϥγo�ӳ���C\n");

	// Delay ������U���O
	if( me->is_delaying() )
		return tell(me,	me->query_delay_msg());

	if( member_array( RIVER	, values((MAP_D->query_map_system(loc))->query_coor_range(loc, TYPE, 1))) != -1	)
	{
		msg("$ME���X"+this_object()->query_name()+"�A���e�誺�e���ΤO�@�ϥX��C\n", me, 0, 1);
		call_out( (: fishing, me :), 5);
		me->start_delay(FISHING_DELAY_KEY, 999, pnoun(2, me)+"�����۳����C\n");
	}
	else
		return tell(me,	"�o����S���i�H�������a��C\n");
}

int remove()
{
	object env = environment();

	if( objectp(env) )
		env->remove_delay(FISHING_DELAY_KEY);
}

void leave(object ob)
{
	if( objectp(ob) )
		ob->remove_delay(FISHING_DELAY_KEY);
}

mapping	actions	=
([
	"fishing" 	: (: do_fishing :),
	"hook" 		: (: do_hook :)
]);
