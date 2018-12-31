/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : help.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-13
 * Note   : help ���O, �L��A�ӧ�o�ӧ˦�menu�a..
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#define HELP_DIR	"/doc/help/"

#include <ansi.h>
#include <help.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�������O

�o�ӫ��O�i�H���ϥΪ̨��o�ϥΦU���\�઺����
���ϥΪ̯�󻴩������x���ͪ��@�ɪ��ާ@�覡

���O�榡:
help			�����D�D�C��
help <�����D�D>		���o���� <�����D�D> ���i�@�B��T

�������O:
HELP;

private varargs string help_style(string title, string arg, string content, string lastmodify)
{
	return sprintf(
		" %-52s%24s\n" 
		WHT"�z�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{\n"NOR // 78 �r���e
		"\n%s\n"
		WHT"�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}\n"NOR + (!undefinedp(lastmodify)?CYN"%77s\n"NOR:"") ,
		HIW+title+NOR + " - ", NOR WHT"["HIW + arg + NOR WHT"]"NOR, arg=="color"?content:ansi(content), "�̫��s�ɶ� "+lastmodify);
}

private void do_command(object me, string arg)
{
	string help, cmd_alias;
	mapping info;
	object ob, env = environment(me);
	function fp;

	if( !arg ) arg = "topics";

	if( !env )
		return tell(me, pnoun(2, me)+"������b�_�Ǫ��a��A�гq���Ův�B�z�C\n");

	// ���һ���
	if( arg == "here" )
	{
		array loc = query_temp("location", me);

		if( !env )
			return tell(me, pnoun(2, me)+"�ثe�ä����B��������Ҥ����C\n");

		// �a�Ϯy�л���
		if( env->is_maproom() )
			return me->more((MAP_D->query_map_system(loc))->query_coor_help(loc) || "�o�Ӧa��èS������S�O�������C\n");

		// �Ҳթж��\�໡��
		else if( mapp(info = env->query_action_info(query("function", env))) && mapp(info["help"]) && info["help"]["topics"])
			return me->more(info["help"]["topics"]);

		// �@��ж��\�໡��	
		else
			return me->more(fetch_variable("help", env) || "�o�Ӧa��èS������S�O�������C\n");
	}

	if( query("help/"+arg, env) )
		return me->more(help_style("���󻡩�", arg, query("help/"+arg, env)));

	// ���󻡩�
	if( objectp(ob = present(arg)) && stringp(fetch_variable("help", ob)) )
		return me->more(help_style("���󻡩�", arg, fetch_variable("help", ob)));

	// �Ҳի��O����
	if( mapp(info = env->query_action_info(query("function", env))) && mapp(info["help"]) && info["help"][arg] )
		return me->more(help_style("�Ҳի��O����", arg, info["help"][arg]));

	// �@�뻡�������
	if( file_size(HELP_DIR+arg) > 0 )
	{
		string title, content;
		help = read_file(HELP_DIR + arg);
		if( sscanf(help, "%s|\n%s", title, content) )
			help = help_style(title, arg, content, TIME_D->replace_ctime(stat(HELP_DIR+arg)[1]));
		return me->more(help);
		//return me->more_file(HELP_DIR+arg);
	}
	if( stringp(cmd_alias = COMMAND_D->default_alias(arg)) )
		arg = explode(cmd_alias, " ")[0];

	// ���a�t�Ϋ��O����
	if( functionp(fp = COMMAND_D->find_ppl_command(arg)) )
	{
		help = fetch_variable("help", function_owner(fp)) || "�����O�õL���󻡩��C\n";
		return me->more(help_style("���a���O����", arg, help));
	}
	if( wizardp(me) )
	{
		// �Ův�t�Ϋ��O����
		if( functionp(fp = COMMAND_D->find_wiz_command(arg)) )
		{
			help = fetch_variable("help", function_owner(fp)) || "�����O�õL���󻡩��C\n";
			return me->more(help_style("�Ův���O����", arg, help));
		}

		if( arg[<2..<1] == "()" ) arg = arg[0..<3];

		// MudOS ��󻡩�
		foreach(string dir in SEARCH_PATHS)
			if( file_size(dir+arg) > 0 )
				return me->more_file(dir+arg);
	}

	tell(me, "�S�� "+arg+" �����������C\n");
}
