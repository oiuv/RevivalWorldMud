/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : inherits.c
 * Author : Sinji@RevivalWorld
 * Date   : 2003-06-03
 * Note   : 
 * Update :
 *  o 2003-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
���O���ܡGinherits <���� | �ɮצW��>

�����O�ΥH�d�ߤ@�Ӫ�����~��(inherit)��������(parents)
HELP;

private varargs string inherit_tree(object ob, int indent);
private void command(object me, string arg)
{
	int deep;
	object ob;

	if( !arg ) return tell(me, "���O���ܡGinherits <���� | �ɮצW��>\n", CMDMSG);
	if( sscanf(arg, "-a %s", arg) ) deep = 1;
	ob = present(arg);
	if( !ob )
	{
		string file = resolve_path(me, arg);
		ob = find_object(file);
	}
	if( !ob ) return tell(me, "�S�� " + arg + " �o���ɮשΪ���C\n", CMDMSG);
	if( deep )
	{
		string *parents = deep_inherit_list(ob);
		if( !parents ) return tell(me, "���� " + base_name(ob) + " �S���~�ӥ��󪫥�C\n", CMDMSG);
		tell(me, base_name(ob) + " ���~�Ӫ��Ҧ�������p�U�G\n");
		tell(me, implode(parents, "\n") + "\n");
		return;
	}
	tell(me, base_name(ob) + " ���~�Ӿ�p�U�G\n");
	tell(me, inherit_tree(ob), CMDMSG);
	return;
}
private varargs string inherit_tree(object ob, string indent)
{
	string shift, msg = "", *parents = shallow_inherit_list(ob);
	int arr_size = sizeof(parents);

	if( !indent )
	{
		if( !arr_size ) return "���� " + base_name(ob) + " �S���~�ӥ��󪫥�C\n";
		msg = base_name(ob) + "\n";
		indent = "";
	}
	for(int i=0; i<arr_size; i++)
	{
		msg += indent;
		if( i == 0 && i == arr_size-1 ) msg += "�|";
		else if( i == (arr_size - 1) ) msg += "�|";
		else msg += "�u";
		msg += parents[i] + "\n";
		shift = indent + (i == (arr_size - 1) ? "  ":"�x");
		msg += inherit_tree(find_object(parents[i]), shift);
	}
	return msg;
}
