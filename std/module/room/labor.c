/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : labor.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-03
 * Note   : �H�O���ЩҪ���
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <citydata.h>
#include <gender.h>
#include <object.h>

inherit ROOM_ACTION_MOD;

#define LABOR_RENT	"100000"

#define LABOR_ID	"ID"
#define LABOR_IDNAME	"IDNAME"
#define LABOR_AGE	"AGE"
#define LABOR_SKILLS	"SKILLS"

// ���ΤH�O
void do_hire(object me, string arg)
{
	int num, i;
	object newnpc;
	object env = environment(me)->query_master();
	int leadership_level;
	int leadership_adv_level;
	string money_unit = env->query_money_unit();

	if( !arg )
		return tell(me, "�C�ӤH�O���жO�ݭn "HIY+money(money_unit, LABOR_RENT)+NOR"�C\n"+pnoun(2, me)+"�Q�n���Υ��s�H�O(Boy/Girl)�H�άO���Φ��g�窺�H�O(list �C��)�H\n");

	arg = lower_case(arg);

	leadership_level = me->query_skill_level("leadership");
	leadership_adv_level = me->query_skill_level("leadership-adv");
	
	if( sizeof(query("hirelabors", me)) >= (leadership_level + leadership_adv_level)/5 )
		return tell(me, pnoun(2, me)+"�ثe����ɯ�O�̦h�u�න�� "+ ((leadership_level + leadership_adv_level)/5)+" �H�C\n");

	if( !me->spend_money(money_unit, LABOR_RENT) )
		return tell(me, pnoun(2, me)+"�S�������ΤH�O�F�C\n");

	switch(arg)
	{
	case "b":
	case "boy":
		newnpc = load_object("/npc/"+SERIAL_NUMBER_D->query_serial_number("labor"));
		set("gender", MALE_GENDER, newnpc);
		set("birthday", TIME_D->query_game_time(), newnpc);
		newnpc->set_idname("poor boy", "�L�a�i�k���p�k��");
		break;
	case "g":
	case "girl":
		newnpc = load_object("/npc/"+SERIAL_NUMBER_D->query_serial_number("labor"));
		set("gender", FEMALE_GENDER, newnpc);
		set("birthday", TIME_D->query_game_time(), newnpc);
		newnpc->set_idname("poor girl", "�L�a�i�k���p�k��");
		break;
	default:
		if( (num = to_int(arg)) > 0 )
		{
			foreach( string file, array info in LABOR_D->query_labors() )
			{
				if( ++i == num )
				{
					newnpc = load_object(file);
					break;
				}	
			}

			if( !objectp(newnpc) )
				return tell(me, "�п�J���T���s���C\n");
			else
				LABOR_D->remove_labor(base_name(newnpc));
		}
		else
			return tell(me, pnoun(2, me)+"�Q�n���Ψk���٬O�k��(Boy/Girl)�άO��J list ���s���H\n");
	}


	set("boss", me->query_id(1), newnpc);

	newnpc->save();
	newnpc->move_to_environment(me);

	msg("$ME��F "HIY+money(money_unit, LABOR_RENT)+NOR" �����ΤF�@�ӤH�O�A�u��$YOU�w�w�q���ᨫ�F�X�ӡC\n", me, newnpc, 1);
	newnpc->do_command("follow "+me->query_id(1));
	newnpc->do_command("say ����"+me->query_name()+HIG"�@�N���ΧڡA�ڤ@�w�|�n�n���ƪ�...\n"NOR);

	set("hirelabors", (query("hirelabors", me)||({})) | ({base_name(newnpc)}), me);
	set("hired", count(query("hired", env), "+", 1), env);

	env->save();
	log_file("command/hire", sprintf("%s hired %O\n", me->query_id(1), newnpc));
}

private void confirm_fire(object me, object npc, string arg)
{
	if( lower_case(arg) == "n" || lower_case(arg) == "no" )
	{
		int suicide;
		string basename;
		string idname;

		msg("$ME��F�@�n���D�G�s"+pnoun(2, npc)+"�u���S��ť��I�H�ڥs"+pnoun(2, npc)+"�u�o�I�I\n", me, 0, 1);
		npc->do_command("cry");
		msg("$YOU�ˤ߱����a���i"+query("short", environment(me))+"������A�����b$ME���e�C\n", me, npc, 1);

		// �i�J LABOR_D ���ݨ�L�H����
		LABOR_D->add_labor(npc);

		basename = base_name(npc);
		idname = npc->query_idname();

		// NPC �۱�, �û�����
		if( (query("relationship/"+me->query_id(1), npc) > 500 && random(5)) || !random(10) )
			suicide = 1;

		log_file("command/fire", sprintf("%s fired %O\n", me->query_id(1), npc ));

		npc->move(VOID_OB);
		destruct(npc);
		me->save();
		me->finish_input();

		if( suicide )
		{	
			broadcast(environment(me), 
			    HIY"�uť���j���f�~��M�ǨӸ��H�̪���I�A"+idname+HIY"�q���B���ӳ����F�U�ӡA�۱��F�C\n"
			    "�u��"+idname+HIY"��W���ۤ@�i�g��"HIR"��"NOR RED"��"HIY"���r���A���g�F�X�Ӧr�G�u�ڤ��|�Ǧ��󪺡A�@�����O�ڦۤv���n...�v�C\n"NOR
			);

			LABOR_D->remove_labor(basename);
			rm(basename+".o");
		}

	}
	else
	{
		msg("$ME���D�G�n�a�I�N��"+pnoun(2, npc)+"�~���b�ڨ��ǧa�I\n", me, 0, 1);
		npc->do_command("jump");
		npc->do_command("say ���󪺤j���j�w�ڥû������|�ѰO���I^_^");
		me->finish_input();
	}
}

// �}���H�O
void do_fire(object me, string arg)
{
	object npc;

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�}���֡H\n");

	if( !objectp(npc = present(arg, environment(me))) )
		return tell(me, "�o�̨S�� "+arg+" �o�ӤH�C\n");

	if( !npc->is_living() )
		return tell(me, pnoun(2, me)+"�Q�n�}��"+npc->query_idname()+"�H�����ᱼ���O����֡H\n");

	if( query("boss", npc) != me->query_id(1) )
		return tell(me, npc->query_idname()+"�ä��O�A���Ϊ��H�O�C\n");

	msg("$ME�h�J$YOU�D�G"+pnoun(2, npc)+"���a�A�ڤw�g���A�ݭn"+pnoun(2, npc)+"�F�C\n", me, npc, 1);
	npc->do_command("sob");
	npc->do_command("say �D�D"+pnoun(2, me)+"�I�����~��b"+pnoun(2, me)+"���ǰ��Ʀn�ܡH�S���o���u�@�ڬ����U�h���I");
	tell(me, "\n�n�~��d�U"+npc->query_idname()+"��"+pnoun(2, me)+"�u�@�ܡH(Yes/No)");
	set_this_player(me);
	input_to((:confirm_fire, me, npc:));
}

void do_list(object me, string arg)
{
	int i, skill_num;
	int total_skill_level;
	string msg, skill;
	mapping skills, data;
	object ob;
	mapping labors = LABOR_D->query_labors();

	if( !sizeof(labors) )
		return tell(me, "�ثe�S�����󦳸g�窺�H�O�i�H���ΡC\n");

	if( arg && (i = to_int(arg)) && i > 0 && i <= sizeof(labors) )
	{
		foreach(string file, mapping info in labors)
		{
			if( !(--i) )
			{
				ob = load_object(file);
				msg = COMMAND_D->find_command_object("score")->score_standard(ob);
				msg += COMMAND_D->find_command_object("skill")->skill_list(me, ob);
				
				me->more(msg);
				return;
			}
		}
	}
	
	i = 0;

	msg =  NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg +=        "�s�� �H�O�W��                         �ʧO �ޯ�� �����`�X �n�D�~��(��)\n";
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

	foreach( string file, mapping info in labors )
	{
		ob = load_object(file);

		skills = query("skills", ob);

		skill_num = 0;
		total_skill_level = 0;
		if( mapp(skills) )
		foreach( skill, data in skills )
		{
			skill_num++;

			total_skill_level += data["level"];
		}
		msg += sprintf("%-3d. %-33s"HIW" %s  "HIG"%-6d"HIC" %-9d"HIY"$%s\n"NOR, ++i, ob->query_idname(), query("gender", ob)==1?"�k":"�k", skill_num, total_skill_level, NUMBER_D->number_symbol(SALARY_D->query_labor_salary(ob)));
	}

	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += HIW"list '�s��' "NOR WHT"�d�߸Բ��ݩʻP�ޯ���\n"NOR;

	me->more(msg);
}
// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
    "lobby"	:
    ([
	"short"	: NOR GRN"���Щ�"NOR,
	"help"	:
	([
	    "topics":
@HELP
    ���ЩҴ��Ѥu�@�P�԰��H�O���ѵ��C
HELP,

	    "hire":
@HELP
���ΤH�O�����O�A�Ϊk�p�U�G
  hire boy		- ���Τ@����s���k�ʤH�O
  hire girl		- ���Τ@����s���k�ʤH�O
  hire 2		- ���νs���� 2 ���w�s�b�H�O
HELP,

	    "fire":
@HELP
�}���H�O�����O�A�Ϊk�p�U�G
  fire john		- �N�W�l�s john ���H�O�}��
HELP,

	    "list":
@HELP
�C�X�ثe�w�s�b���H�O�A�Ϊk�p�U�G
  list			- �C�X�Ҧ��ثe�w�s�b���L�H���Ϊ��H�O
  list '�s��'		- �d�߸ӤH�O���ݩʻP�ޯ���
HELP,
	]),
	"action":
	([
	    "hire"		: (: do_hire :),
	    "fire"		: (: do_fire :),
	    "list"		: (: do_list :),
	]),
    ]),
]);


// �]�w�ؿv�����
nosave array building_info = ({

    // �ؿv��������W��
    HIG"�H�O���Щ�"NOR

    // �}�i���ؿv�����̤֩ж�����
    ,1

    // �������̤j�ۦP�ؿv�ƶq����(0 �N������)
    ,0

    // �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
    ,GOVERNMENT

    // �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
    ,AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION

    // �}�i�����O��
    ,"5000000"

    // �ؿv���������ռаO
    ,0

    // �c�a�^�m��
    ,20

    // �̰��i�[�\�Ӽh
    ,1

    // �̤j�ۦP�ؿv���ƶq(0 �N������)
    ,0

    // �ؿv���ɥN
    ,1
});
