/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : title.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-02
 * Note   : title ���O
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
    �ٸ��]�w�A�b�ӤH�֦��S���N��N�|��o�U�ص}�����ٸ�
�U�دS��ƥ�Υ��ȵ��G�������|�o��S���ٸ�

	title		��ܥثe�ҥΪ��ٸ�
	title -l	�C�X�ثe�i�H�ϥΪ��ٸ�
	title -d	������ܥثe�ٸ�
	title '�Ʀr'	�]�w�Q�n��ܪ��ٸ�

HELP;

private void do_command(object me, string arg)
{
	string title = query("title", me);
	string *all_titles = query("all_titles", me);
    	
    	if( !arg )
    	{
    		if( !title )
    			return tell(me, pnoun(2, me)+"�ثe�S���]�w��ܥ���ٸ��C\n");
    		
    		return tell(me, pnoun(2, me)+"�ثe����ܪ��ٸ����u"+title+"�v�C\n");
    	}
    	else if( arg == "-l" )
    	{
    		int i;
    		string msg;
    		
    		if( !arrayp(all_titles) || !sizeof(all_titles) )
    			return tell(me, pnoun(2, me)+"�ثe�S������ٸ��i�H�ϥΡC\n");

    		msg = pnoun(2, me)+"�ثe�i�H�ϥΪ��ٸ��p�U�G\n";
		
		foreach( title in all_titles )
			msg += (++i)+". "+title+"\n";
			
		return tell(me, msg+"\n");
	}
	else if( arg == "-d" )
	{
		if( !title )
			return tell(me, pnoun(2, me)+"�ثe�S���]�w��ܥ���ٸ��C\n");
			
		delete("title", me);
		return tell(me, pnoun(2, me)+"������ܡu"+title+"�v�ٸ��C\n");
	}
	else if( to_int(arg) )
	{
		int i, j = to_int(arg);
		
		if( j < 1 )
			return tell(me, "�п�J���T���Ʀr�C\n");
			
    		if( !arrayp(all_titles) || !sizeof(all_titles) )
    			return tell(me, pnoun(2, me)+"�ثe�S������ٸ��i�H�ϥΡC\n");
    
		foreach( title in all_titles )
		{
			if( ++i == j )
			{
				set("title", title, me);
				return tell(me, pnoun(2, me)+"�]�w�n��ܪ��ٸ����u"+title+"�v�C\n");
			}
		}
		return tell(me, "�èS���� "+j+" �ӽs�����ٸ��C\n");
	}
	else if( wizardp(me) && find_player(arg) )
	{
		int i;
		string msg;
		object player = find_player(arg);
		
		all_titles = query("all_titles", player);
		
	    	if( !arrayp(all_titles) || !sizeof(all_titles) )
    			return tell(me, player->query_idname()+"�ثe�S������ٸ��i�H�ϥΡC\n");
    		
    		msg = player->query_idname()+"�ثe�i�H�ϥΪ��ٸ��p�U�G\n";
		
		foreach( title in all_titles )
			msg += (++i)+". "+title+"\n";
			
		return tell(me, msg+"\n");
	}
	return tell(me, help+"\n");
}