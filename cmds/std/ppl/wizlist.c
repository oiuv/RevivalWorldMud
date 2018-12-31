/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wizlist.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-4-11
 * Note   : wizlist ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <ansi.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�Ův�M��

�o�ӫ��O�i�H�d�߭��ͪ��@�ɤ����Ҧ��Ův�Υثe�u�W���Ův�A
�p�z������ð��w�ﬢ�߽u�W�Ův�A�ΧQ�� 'bug' ���O�^���C�����D�C

�������O:
HELP;

#define OVERDAY		30

nosave string *wiz_levels = ({ "admin", "advisor", "wizard", "guest" });

private void do_command(object me, string arg)
{
	int online, now_time = time();
	object user;
	string color, str;
	mapping final=allocate_mapping(5), num=allocate_mapping(0);
	mapping wizards = SECURE_D->query_wizards();

	final["admin"] = 	WHT"�w "HIW"Administrator �t�κ޲z��"NOR WHT"  �w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n\n  "NOR;
        final["advisor"] = 	WHT"�w "HIW"Advisor �t���U��"NOR WHT"  �w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n\n  "NOR;
        final["wizard"] = 	WHT"�w "HIW"Wizard �Ův"NOR WHT"   �w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n\n  "NOR;
        final["guest"] = 	WHT"�w "HIW"Guest �S�O�ӻ�"NOR WHT"  �w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n\n  "NOR;
	if( wizardp(me) )
	foreach( string id in sort_array(keys(wizards), 1) )
	{
		if( find_player(id) )
			++online && color = HIC;
		else if( file_size(user_path(id)) != -2 )
			color = HIR"�򥢨���";
		else
		{
			int last_login;
			if( !objectp(user = load_user(id)) ) color = HIR;
			// �W�L 30 �ѥ��W�u
			else if(
				!(last_login = query("last_login/time", user)) ||
				now_time - last_login > 60*60*24*OVERDAY
			) color = NOR YEL;
			else color = CYN;
			destruct(user);
		}
		final[wizards[id]] += sprintf("%-13s"NOR"%s", color+capitalize(id), ++num[wizards[id]]%6 ? "" : "\n  ");
	}
	else
	foreach(string id in sort_array(keys(wizards), 1))
	{
		if( find_player(id) )
			++online && color = HIC;
		else 
			color = CYN;
		
		final[wizards[id]] += sprintf("%-13s"NOR"%s", color+capitalize(id), ++num[wizards[id]]%6 ? "" : "\n  ");
	}
	str = sprintf(NOR WHT"\n  �Ův�@�� "HIW"%d"NOR WHT" ��A�u�W "HIW"%d"NOR WHT" ��\n\n"NOR, sizeof(wizards), online);
	str = sprintf("%63s", str);
	foreach(string lv_name in wiz_levels)
		str += final[lv_name] + (!(num[lv_name]%6) ? "\n" : "\n\n");
	str += NOR WHT"  �p"+pnoun(2, me)+"������ð��w�ﬢ�߽u�W�Ův�A�ΧQ�� '"HIW"bug"NOR WHT"' ���O�^���C�����D�C\n\n"NOR;
	tell(me, str, CMDMSG);
}
