/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : skill.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-04
 * Note   : skill ���O
 * Update :
 *  o 2003-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <skill.h>
#include <message.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
�i�C�X�ثe�֦����ޯ�B�ޯ൥�Ťθg��ȡB�ɤW�@�ũҶ��g��ȡC

skill		- �C�X�ۤv���ޯ���
skill [���u]	- �C�X�z�Y����u���ޯ���

�t���Y�g: sk
HELP;

varargs string percent_bar(int len, string str, int cur, int max, int flag)
{
	int percent_len;
	float percent = 0.;

	if( !len ) return "Something Wrong With Bar.";
	if( max ) percent = to_float(cur)/to_float(max);
	percent_len = to_int(len * percent);
	str = sprintf("%|:" + len + "s", str);
	return sprintf("%s%s"NOR"%s%s"NOR" %6s",
	    U + BBLU, (percent_len ? str[0..percent_len-1] : ""),
	    BRED, str[percent_len..],
	    (flag ? sprintf("%.1f%%", percent*100):"")
	);
}

string skill_list(object me, object ob)
{
	string msg;
	string skill, skill_type;
	mapping skills;
	object skillob;
	mapping data;
	string level_color, grade;
	int prev_level_exp, next_level_exp;
		
	skills = query("skills", ob);

	if( !mapp(skills) || !sizeof(skills) )
		return (ob==me?pnoun(2, me):ob->query_idname())+"�ثe�S���ǲߥ���ޯ�C\n";

	msg = (ob==me?pnoun(2, me):ob->query_idname())+"�ثe�Ҿǲߨ쪺�ޯ�@���G\n";
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += sprintf(HIW"  %-30s %4s %4s %|30s\n"NOR, "�ޯ�W��", "����", "����", "�g���");
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

	foreach( skill in SKILL_D->query_sorted_skills() )
	{
		if( !(data = skills[skill]) )
		{
			if( !SKILL_D->skill_exists(skill) )
				skill_type = skill;
				
			continue;
		}

		skillob = load_object(SKILL(skill));
		
		if( !objectp(skillob) ) continue;

		if( skill_type )
		{
			msg += "- "+skill_type+" -\n";
			skill_type = 0;
		}
		
		switch(data["level"])
		{
			case 0..10:	level_color = HIC;	grade = "�L"NOR CYN"��"; break;
			case 11..20:	level_color = HIC;	grade = "��"NOR CYN"��"; break;
			case 21..30:	level_color = HIC;	grade = "�z"NOR CYN"��"; break;
			case 31..40:	level_color = HIG;	grade = "��"NOR GRN"�x"; break;
			case 41..50:	level_color = HIG;	grade = "��"NOR GRN"�q"; break;
			case 51..60:	level_color = HIG;	grade = "��"NOR GRN"�|"; break;
			case 61..70:	level_color = HIY;	grade = "��"NOR YEL"��"; break;
			case 71..80:	level_color = HIY;	grade = "�e"NOR YEL"�q"; break;
			case 81..90:	level_color = HIY;	grade = "��"NOR YEL"�q"; break;
			case 91..99:	level_color = HIR;	grade = "��"NOR RED"��"; break;
			case 100..999:	level_color = HIR;	grade = "��"NOR RED"��"; break;
		}
		prev_level_exp = (SKILL(skill))->level_exp(ob->query_skill_level(skill));
		next_level_exp = ob->next_level_exp(skill);
		

		msg += sprintf("  %-30s %s%4d"NOR" %s%|4s "NOR" %s %s\n"NOR,
			    (SKILL(skill))->query_idname(),
			    level_color, data["level"],
			    level_color, grade,
			    percent_bar(
				30,
				sprintf("%d", to_int(data["exp"])),
				to_int(data["exp"]) - prev_level_exp,
				next_level_exp - prev_level_exp,
				1
			    ), 
			    (SKILL(skill))->query_special_type()||"",	    
		);
	}

	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	return msg;
}

private void do_command(object me, string arg)
{
	object ob;

	if( wizardp(me) )
	{
		if( arg ) 
			ob = find_player(arg) || present(arg); 
		else 
			ob = me;

		if( !objectp(ob) || !ob->is_living() )
			return tell(me, "�S�� "+arg+" �o�ӤH�C\n");
	}
	else if( arg && objectp(ob = present(arg)) && ob->is_living() )
	{
		if( query("boss", ob) != me->query_id(1) )
			return tell(me, ob->query_idname()+"�ä��O"+pnoun(2, me)+"���Ϊ��Ҥu�C\n");
	}
	else ob = me;

	me->more(skill_list(me, ob))+"\n";
}
