/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : editnews.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-00-00
 * Note   : �s��s�D
 * Update :
 *  o 2003-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

#define TIME		0
#define MESSAGE		1

inherit COMMAND;

string help = @HELP
	�з� editnews ���O�C
HELP;

private void command(object me, string arg)
{
	string option;
       	
       	if( !is_command() ) return;
       	
       	if( arg == "-l" )
       	{
       		int i;
       		string msg = "�ثe���s�D��T�p�U�G\n";
       		
       		foreach(array news in NEWS_D->query_news())
       			msg += (++i)+". ("+TIME_D->replace_ctime(news[TIME])[0..4]+") "+news[MESSAGE]+NOR+"\n";
       			
       		return tell(me, msg+"\n");
	}
      	
       	if( !arg || sscanf(arg, "-%s %s", option, arg) != 2 )
                return tell(me, "editnews [-l | -a �T�� | -d �s��]\n");
       	
       	switch( option )
       	{
       		case "a":
			if( NEWS_D->add_news(arg) )
			{
				tell(me, "�s�W�s�D�����C\n");
				NEWS_D->broadcast_latest_news();
			}
			else
				tell(me, "�s�W�s�D���ѡC\n");
			break;
		case "1":
			CHANNEL_D->channel_broadcast("news", HIG"�榸���i "+NOR GRN"("+TIME_D->replace_ctime(time())[0..4]+") "+arg+NOR);
			break;
		case "d":
		{
			int i = to_int(arg);
			
			if( !i )
				return tell(me, "�п�J�n�R�����s�D�s���C\n");
				
			if( !NEWS_D->remove_news(i-1) )
				tell(me, "�п�J���T���s�D�s���C\n");
			else
				tell(me, "�R���s�D�����C\n");
			break;
		}
	}
}
