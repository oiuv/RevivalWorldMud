/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : allowmulti.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-09
 * Note   : allowmulti ���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        �з� allowmulti ���O�C
HELP;

private void command(object me, string arg)
{
	string id, *ids;
	object user;

	if( !is_command() ) return;

	if( !arg ) 
		return tell(me, pnoun(2, me)+"����]�w���Ǫ��a�i�H�P IP �s�u�H\n");
	
	ids = explode(arg, " ");
	
	if( sizeof(ids) < 2 )
		return tell(me, "�г]�w���H�W�����a�C\n");
		
	foreach(id in ids)
		if( !user_exists(lower_case(id)) )
			return tell(me, "�o�ӥ@�ɤW�S�� "+id+" �o�Ӫ��a�C\n");
	
	foreach(id in ids)
	{
		user = find_player(lower_case(id)) || load_user(lower_case(id));
		
		set("login/allowmulti", copy(ids - ({ id })), user);
		
		user->save();
	
		tell(me, "�]�w"+user->query_idname()+"�i���\�P IP �n�J�����a�� "+implode(ids - ({ id }), ", ")+"\n");
		
		
		if( !interactive(user) )
			destruct(user);
	}
	
	log_file("command/allowmulti", "���\�P IP �n�J�G"+implode(ids, ", "));
}
