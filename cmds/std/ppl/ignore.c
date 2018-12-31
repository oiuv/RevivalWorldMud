/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ignore.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-02
 * Note   : ignore ���O
 * Update :
 *  o 2000-00-00  
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
���O�榡:

�o�ӫ��O�i�H���A�������Y�ǤH��tell�Preply
���O�榡 : ignore        �C�X�Ҧ����ڵ��z�|���W��
           ignore -a id  �W�[�s�� id �i�W�椺
           ignore -d id  �N�Y�ӤH�� id �q�W�椤����

�������O:
	tell, channels
HELP;

private void do_command(object me, string str)
{
	int i = 0;
	string status, *deny, msg;

	if( !is_command() ) return;

	if( !arrayp( deny = query("deny_user", me) ) )
		deny = allocate(0);

	if( !stringp(str) )
	{
		if( arrayp(deny))
		{
			if( !sizeof(deny) ) 
				return tell( me, "�A�ثe�S�����Q�z���H .\n", CMDMSG);

			msg = "�A���Q�z���H�p�U :\n";

			for( i=0 ; i < sizeof(deny); i++)
				msg += sprintf( "[ %d ]\t%s\n", i+1, deny[i] );

			return tell( me, msg);
		} else return tell( me, "�A�ثe�S�����Q�z���H .\n", CMDMSG);
	}

	if( sscanf( str,"-d %s",status) )
	{
		if( member_array( status, deny ) == -1 )
			return tell( me, sprintf("%s �쥻�N���b�z���¦W�椤 .\n", status), CMDMSG);

		deny -= ({ status });
		set("deny_user", deny, me);
		me->save();
		return tell( me,"�A�N "+status+" �q�A���¦W�椤�����F .\n", CMDMSG);
	}

	if( sscanf(str, "-a %s",status) )
	{
		if( member_array( status, deny ) !=-1 ) 
			return tell( me, sprintf("%s �쥻�N�w�g�b�z���¦W�椤 .\n", status), CMDMSG);

		deny += ({ status });
		set("deny_user", deny, me );
		me->save();
		return tell( me, sprintf("%s ���\���[�J�¦W�椤 .\n", status ), CMDMSG);
	}

	return tell( me, help );

}
