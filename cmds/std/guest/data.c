/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : data.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-4-21
 * Note   : data ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <location.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� data ���O�C
HELP;

private void command(object me, string arg)
{
	string data;
	object ob;
	string res = resolve_path(me, arg);

	if( !is_command() ) return;

	if( !arg || arg == "me" ) 
		ob = me;

	else if( arg == "here" )
	{
		object env = environment(me);

		if( !env ) 
			return tell(me, "�A�èS���B��������ҷ��C\n", CMDMSG);

		if( env->is_maproom() ) 
		{
			array loc = query_temp("location", me);
			data = "�A�Ҧb�y�� "+query_temp("location",me)[CODE]+" ����Ʀp�U\n";
			data += sprintf("%O\n",	(MAP_D->query_map_system(loc))->query_coor_data(loc));
			tell(me, data, CMDMSG);
			return;
		}
		else ob = env;
	}

	if( ob || objectp(ob = present(arg) || find_object(res) || load_object(res) || find_player(arg)) )
	{
		data = (ob->short()||query("short",ob)||file_name(ob))+"�������Ʈw��T�p�U�G\n";

		data += HIG+remove_ansi(sprintf("%O\n",  ob->query_database()||"�L"))+NOR;
		data += NOR GRN+remove_ansi(sprintf("%O\n",  ob->query_temp_database()||"�L"))+NOR;
	}
	else
		return tell(me, "�S�� "+arg+" �o�Ӫ���C\n", CMDMSG);

	me->more(data);

}
