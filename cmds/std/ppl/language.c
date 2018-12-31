/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : language.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-09-08
 * Note   : language ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
#include <ansi.h>

inherit COMMAND;

string help = @HELP
�Y��½Ķ���O(Alpha Version)

�Q�� Google Translate �@�Y�� Mud ½Ķ

�ثe�i½Ķ�y����

en	- �^��

HELP;

private void do_command(object me, string arg)
{
	string language = query("translate/language", me);

	if( !arg )
	{
		if( !language )
			return tell(me, pnoun(2, me)+"�ثe�S���@����Y��½Ķ�C\n");

		return tell(me, pnoun(2, me)+"�ثe�ҨϥΪ��y�����G"+language+"�C\n");
	}
	
	switch(arg)
	{
		case "-d":
		{
			if( !language )
				return tell(me, pnoun(2, me)+"�쥻�N�S���]�w�Y��½Ķ�\��C\n");

			delete("translate/language", me);
			tell(me, HIY+pnoun(2, me)+"�����F�Y��½Ķ���\��C\n"NOR);
			me->save();
			break;
		}
		case "en":
		{
			
			tell(me, HIR"ĵ�i�G�ثe½Ķ�\��|�����ơA�i��ɭP�\�h��r�����áA����½Ķ�\��Ы� language -d�C\n"NOR);
			tell(me, HIC+pnoun(2, me)+"�}�ҧY��½Ķ�\��A�y���]�w���^��(en)�C\n"NOR);
			set("translate/language", arg, me);
			me->save();
			break;
		}
		default:
			return tell(me, "�S�� "+arg+" �y���C\n");
			break;
	}
}
