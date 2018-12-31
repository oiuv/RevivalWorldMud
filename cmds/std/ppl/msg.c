/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : msg.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-17
 * Note   : msg ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
    ��̪ܳ񦬨쪺 1000 �Ӳ��/�����W�D�άO�K�y�T���C

msg +����		�W�[�Y���T��������
msg -����		����Y���T��������
msg ����		�]�w�T���������ơA�̤֬��� 10 ���A�̦h���� 1000 ��
msg clear		�R���Ҧ��T��
msg info		�d�ߥثe���T����������

�T�������]�A�G���}�W�D(chat, city, rumor, news, ...), emotion, say, tell

HELP;

private void do_command(object me, string arg)
{
	int i, size;
	array data;
	object target;
	string type;
		
	if( arg == "clear" )
	{
		me->clear_msg_log();
		return tell(me, pnoun(2, me)+"�M�����Ҧ����T�������C\n");
	}

	if( arg == "info" )
	{
		mapping setup = query("msg", me);
		
		if( !mapp(setup) || !sizeof(keys(setup)-({ "size" })) )
			return tell(me, pnoun(2, me)+"�S���]�w��������T���C\n");
		
		map_delete(setup, "size");
		
		return tell(me, pnoun(2, me)+"�ثe�]�w���T���������ءG"+implode(keys(setup), ", ")+"\n");
	}
	
	if( arg && (size = to_int(arg)) )
	{
		if( size < 10 || size > 1000 )
			return tell(me, "�T���̤֬��� 10 ���A�̦h���� 1000 ���C\n");
		
		set("msg/size", size, me);
		tell(me, pnoun(2, me)+"�]�w�T���������Ƭ� "+size+" ���C\n");
		me->save();
		return;
	}

	if( arg && sscanf(arg, "+%s", type) == 1 )
	{
		if( type == "say" || type == "tell" || type == "emotion" || type == "announce" || CHANNEL_D->valid_channel(SECURE_D->level_num(me->query_id(1)), type) )
		{
			set("msg/"+type, 1, me);
			tell(me, pnoun(2, me)+"�}�l���� "+type+" ���T���C\n");
			me->save();
			return;
		}
		else
			return tell(me, "�L�k�]�w "+type+" �o�ذT���C\n");
	}
	else if( arg && sscanf(arg, "-%s", type) == 1 )
	{
		if( type == "say" || type == "tell" || type == "emotion" || type == "announce" || CHANNEL_D->valid_channel(SECURE_D->level_num(me->query_id(1)), type) )
		{
			delete("msg/"+type, me);
			tell(me, pnoun(2, me)+"������� "+type+" ���T���C\n");
			me->save();
			return;
		}
		else
			return tell(me, "�L�k�]�w "+type+" �o�ذT���C\n");
	}
	
	if( wizardp(me) && arg && objectp(target = find_player(arg)) && SECURE_D->level_num(target) <= SECURE_D->level_num(me) )
		data = target->query_msg_log();
	else
		data = me->query_msg_log();
    	
    	size = sizeof(data);
    	
    	if( size )
    	{
    		string *str;
    			
    		str = ({ (target?target->query_idname():"")+"�@�� "+(size/3)+" ���°T�������G\n�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n" });
    		
    		if( arg && !target )
    		{
    			for(i=0;i<size;i+=3)
    				if( data[i+1] == arg )
    					str += ({ HIW+ctime(data[i])[11..15]+NOR"-"+data[i+2] });
    		}
    		else
    		{
    			for(i=0;i<size;i+=3)
    				str += ({ HIW+ctime(data[i])[11..15]+NOR"-"+data[i+2] });
    		}
   
   		str += ({ "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�ثe�ɨ� "HIW+ctime(time())[11..15]+NOR"�w�w\n" });
   		me->more(implode(str, ""));
	}	
    	else
    		return tell(me, (target?target->query_idname():"")+"�ثe�S������T�������C\n");
}
