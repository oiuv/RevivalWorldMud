/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : quest.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-05
 * Note   : quest ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
    ���Ȭd�ߡA�d�ߤw�����Υ��������U�����ȻP���ȵ���

	quest		��ܥ��ȸ�T

HELP;

private void do_command(object me, string arg)
{
	int i;
	string msg;
	object ob;
	mapping quest;
	int total, totalopen;
	if( arg && wizardp(me) && objectp(ob = find_player(arg)) )
	{
		if( !sizeof(quest = query("quest", ob)) )
			return tell(me, ob->query_idname()+"�|������������ȡC\n");
			
		msg = ob->query_idname()+"�����ȸ�T�p�U(�@ "+sizeof(quest)+" ������)�G\n";
	}
	else
	{
		ob = me;
		
		if( !sizeof(quest = query("quest", ob)) )
			return tell(me, pnoun(2, me)+"�|������������ȡC\n");

		msg = pnoun(2, me)+"�����ȸ�T�p�U(�@ "+sizeof(quest)+" ������)�G\n";
	}
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	foreach(string questname, mapping data in quest)
	{
		if( undefinedp(data["name"]) ) continue;

		msg += sprintf(HIW"%-4s"NOR"%s %s\n    "HIB"���O"NOR"�G%s\n"NOR, (++i)+".", data["name"]+NOR YEL" �w "HIY+TIME_D->replace_ctime(data["time"])+NOR, (data["finish"]? GRN"("HIG"�w�����A�`�@�B�J�G"+(data["step"]?data["step"]+"/":"")+QUEST_D->query_quest_steps(questname)+"�A�������ơG"+data["finish"]+NOR GRN")"NOR:RED"("HIR"�������A�ثe�B�J:"+data["step"]+"/"+QUEST_D->query_quest_steps(questname)+NOR RED")"NOR), data["note"]?data["note"]:"�L����");
	}
	
	total = QUEST_D->query_total_quests_amount();
	totalopen = QUEST_D->query_total_opened_quests_amount();
	
	//msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n�`���ȼơG"+HIW+total+NOR WHT"�B�����}��G"+HIW+totalopen+NOR WHT"�B���դ��G"+HIW+(total-totalopen)+"\n"NOR;
	me->more(msg);
}