/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : gag_user.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-02-05
 * Note   : force ���O
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
        gag_user <id> �]�w�Y�H���i�ϥ� channels
        gag_user -d <id> �Ѱ��Y�H���i�ϥ� channels
HELP;

private void command(object me, string arg)
{
	object you;

	if( !is_command() ) return;

	if(!arg) return tell( me, help );

	if( sscanf(arg,"-d %s",arg) == 1 )
	{
		if( !objectp( you = find_player(arg) ))
			return tell( me, "�u�W�S�� " + arg + " �o�ӤH\n");

                delete("no_channel", you);
		tell( me, you->query_idname() + "�}�l�i�H�ϥΤ����W�D\n" );
		tell( you, "�z�q�{�b�_�Ѱ��T��ϥΤ����W�D\n");
		return;
	}	

	if( objectp( you = find_player(arg) ) )
	{
                set( "no_channel", 1, you);
		tell( me, you->query_idname() + "�w�g�L�k�l�Τ����W�D\n" );
		tell( you, "�z�q�{�b�_�L�k�ϥΤ����W�D, �p��������D�ЦV�Ův����\n");
		return;
	} else
		return tell( me, "�u�W�S�� " + arg + " �o�ӤH\n");


}
