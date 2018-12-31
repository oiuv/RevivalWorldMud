/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : infodata.c
 * Author : 
 * Date   : 2005-10-04
 * Note   : infodata ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
���O�榡 : infodata <���󤧦W�٩��ɦW>

�Q�Φ��@���O�i�o���@�Ǧ����Ӫ��󪺸�T�C
HELP;

private string query_info( object ob )
{
	string str = "";
	if( userp(ob) )                str += "�ϥΪ� ";
	if( interactive(ob) )  		   str += "��ͦ� ";
	if( wizardp(ob) )              str += "�Ův ";
	if( clonep(ob) )               str += "�ƻs ";
	if( virtualp(ob) )             str += "���� ";
	if( query_heart_beat(ob) )     str += "�߸�:" + query_heart_beat(ob) + " ";

	return str;
}

private void command(object me, string arg)
{
	object obj;
	string str;

	if( !is_command() ) return;

	if (!arg) 
		return tell( me, "���O�榡 info <me|here|obj>\n", CMDMSG);

	if( arg=="me" ) obj = me;
	else 
	{
		intact_file_name(ref arg);
		obj = present(arg, me);
		if (!obj) obj = present(arg, environment(me));
		if (!obj) obj = find_object( resolve_path(me, arg) );
		if (!obj) return tell( me, "�S���o�˪���....�C\n", CMDMSG);
	}

	str = sprintf( "\n" + @DATA
���� [%O]
-----------------------------------------------------
�ɮסR          %s.c
���šR          %s
�ϥΰO����R    %d
�ݩʡR		%s
�ƻs�ӼơR      %d
�Ѧҳs���R      %d

DATA, 
	    obj,
	    base_name(obj),
	    SECURE_D->level_name(obj->query_id(1)),
	    memory_info(obj),
	    query_info(obj),
	    sizeof(children(base_name(obj)+".c")),
	    refs(obj)
	);

	return tell( me, str , CMDMSG );
}
