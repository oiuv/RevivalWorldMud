/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : nslookup.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-03
 * Note   : nslookup ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

private nosave mapping caller = allocate_mapping(0);

string help = @HELP
	�з� nslookup ���O�C
HELP;

private void command(object me, string arg)
{
       	if( !is_command() ) return;
       	
	if( !arg )
		return tell(me, "�п�J���d�ߪ�����W�٩� IP�C\n", CMDMSG);
	
	tell(me, "�d�ߤ��A�еy��...\n", CMDMSG);
	
	caller[resolve(arg, "callback")] = me;
}

void callback(string address, string resolved, int key)
{
	string msg="\n\n�d�ߵ��G:\n";
	
	msg += "Name    : "+(address||"���s�b����W��")+"\n";
	msg += "Address : "+(resolved||"���s�bIP")+"\n\n";

	if( objectp(caller[key]) )
	{
		tell(caller[key], msg, CMDMSG);
		caller[key]->finish_input();
	}
	
	map_delete(caller, key);
}