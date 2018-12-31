/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : lf.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-04-22
 * Note   : lf ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        �з� lf ���O�C
HELP;

#define NAME		0
#define ARGS_NUM	1
#define RETURN_TYPE	2
#define ARGS_TYPE	3..

private void command(object me, string arg)
{
	array funcs;
	string msg;
	object ob;

	if( !is_command() ) return;

	if( !arg )
		return tell(me, "�п�J���d�ߨ禡�C���ɮצW�٩Ϊ���W�١C\n", CMDMSG);
		
	if( !objectp(ob = find_player(arg) || present(arg, me) || present(arg, environment(me))) )
		arg = resolve_path(me, arg);
	
	if( !objectp(ob) && !objectp(ob = load_object(arg)) )
		return tell(me, "�S�� "+arg+" �o�Ӫ�����ɮסC\n", CMDMSG);
	
	if( !sizeof(funcs = functions(ob, 1)) )
		return tell(me, (ob->query_idname() || ob->short() || base_name(ob))+"����W�S�����󪺨禡�C\n", CMDMSG);

	msg = (ob->query_idname() || ob->short() || base_name(ob))+"���󪺨禡�C��p�U�G\n";
	
	funcs = sort_array(funcs, (: strcmp($1[NAME], $2[NAME]) :));
	foreach( array func in funcs )
	{
		msg += function_exists(func[NAME], ob) ? "" : NOR WHT"private "NOR;

		msg += func[RETURN_TYPE]+HIY+func[NAME]+NOR YEL"("+implode(map(func[ARGS_TYPE],(:$1[0..<2]:)),", ")+");\n"NOR;
	}
	
	me->more(msg+"\n");
}
