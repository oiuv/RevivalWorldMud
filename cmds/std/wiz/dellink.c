/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : dellink.c
 * Author : Cookys@RevivalWorld
 * Date   : 2002-09-15
 * Note   : �إ߻P�ק��ɮ׼��Y��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <ansi.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        �з� dellink ���O
HELP;


private void command(object me, string arg)
{
	object env=environment(me);
	
	if( !is_command() ) return;

        if( !arg && !env->is_maproom() )
	        return tell(me, "�п�J�n�s�����X�f�W�٩Τ�V�C \n", CMDMSG);
	        
	if( env->is_maproom() )
	{
		if( undefinedp(MAP_D->query_special_exit(query_temp("location",me))) )
			return tell(me, "���B�èS���S��s���X�f�C\n", CMDMSG);
			
		MAP_D->del_special_exit(query_temp("location",me));
		return tell(me, "�S��X�f�w�R���C\n", CMDMSG);
	} else {
	
		if( undefinedp(query("exits",env )[arg] ) )
			return tell(me, HIC + arg + NOR +" �o�ӥX�f���s�b�C\n", CMDMSG);	
		
		delete("exits/"+arg,env);
		return tell(me, HIC+arg+NOR+"�X�f�w�R���C\n", CMDMSG);
	}
	
}
