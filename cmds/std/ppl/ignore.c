/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ignore.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-02
 * Note   : ignore 指令
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
指令格式:

這個指令可以讓你不接受某些人的tell與reply
指令格式 : ignore        列出所有的拒絕理會的名單
           ignore -a id  增加新的 id 進名單內
           ignore -d id  將某個人的 id 從名單中拿掉

相關指令:
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
				return tell( me, "你目前沒有不想理的人 .\n", CMDMSG);

			msg = "你不想理的人如下 :\n";

			for( i=0 ; i < sizeof(deny); i++)
				msg += sprintf( "[ %d ]\t%s\n", i+1, deny[i] );

			return tell( me, msg);
		} else return tell( me, "你目前沒有不想理的人 .\n", CMDMSG);
	}

	if( sscanf( str,"-d %s",status) )
	{
		if( member_array( status, deny ) == -1 )
			return tell( me, sprintf("%s 原本就不在您的黑名單中 .\n", status), CMDMSG);

		deny -= ({ status });
		set("deny_user", deny, me);
		me->save();
		return tell( me,"你將 "+status+" 從你的黑名單中消除了 .\n", CMDMSG);
	}

	if( sscanf(str, "-a %s",status) )
	{
		if( member_array( status, deny ) !=-1 ) 
			return tell( me, sprintf("%s 原本就已經在您的黑名單中 .\n", status), CMDMSG);

		deny += ({ status });
		set("deny_user", deny, me );
		me->save();
		return tell( me, sprintf("%s 成功的加入黑名單中 .\n", status ), CMDMSG);
	}

	return tell( me, help );

}
