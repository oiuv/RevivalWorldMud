/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : im.c
 * Author : Whatup@RevivalWorld
 * Date   : 2002-02-01
 * Note   : Im ���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */
#define IM_D "/system/daemons/im_d.c"

#include <feature.h>
#include <message.h>

inherit COMMAND;

void who_list(object me);
void register(object me,string pro);
void help(object me);

string help = @HELP

���O�榡 : im [�U���Ѽ�]
            
    �o�ӫ��O�i�H���A�n�J�����W�y�檺�Y�ɳq�T�n��C�ثe�䴩��         
�Y�ɳq�T�n�馳 MSN �B YAHOO �o�G���C

    ���ާA�O�ĴX���ϥΡA�n�ϥ� im �ɡA���ݥ��� im login �@
�n�J���ʧ@�C

    �]�������ǿ骺���t�A���ǰT���i��|����@�Ǯɶ��~�|����C
     
     im login           - �n�J im �t�ΡC
     im quit            - �n�X im �t�ΡC
     im who             - �C�X�A���n�ͦW��C
     im register msn    - ���U msn �b���C
     im register yahoo  - ���U yahoo �b���C
     im add <protocol> <account> - �s�W�n�͡C
     im remove <account>- �����n�͡C
     im no              - ���P�N�L�H��A�[�J�n�ͦW��C
     im yes             - �P�N�L�H��A�[�J�n�ͦW��C
     im nick <nick>     - �ק� msn ���ʺ١C
     im tell <id> <msg> - �ǰT���Y�H�C

HELP;

private void command(object me, string arg)
{
	string who,msg;

	if( !is_command() ) return;

	if(!arg) return tell( me, help);

	if(arg == "login") 
	{
		if( query_temp("im_fd", me) 
		&& IM_D->query_users()[query_temp("im_fd", me)]["obj"] == me)	
			return tell("\n[IM Message]:�A���b�n�J���A�Х� im quit ���u�C\n");

		tell( me, "\n[IM Message]:�}�l�n�J�Y�ɳq�T���F�C\n");
		IM_D->login_irc(me);
		return;
	}

	if(!undefinedp(query_temp("im_fd", me))) 
	{
		if(arg == "who")
		{ 
			who_list(me);
			return ;
		}
		
		if(arg == "quit") 
		{
                        IM_D->send_command(query_temp("im_fd", me),"QUIT\r\n");
			delete_temp("im_fd", me);
			tell( me, "\n[IM Message]:�Y�ɳq�T���F���u�F�C\n");
			return;
		}

		if(sscanf(arg,"tell %s %s",who,msg) == 2)
			return IM_D->process_send_msg(me,who,msg);

		if(arg == "register msn") 
		{	
			register(me,"MSN");
			return;
		}
		
		if(arg == "register yahoo") 
		{
			register(me,"YAHOO");
			return;
		}
		
		if(sscanf(arg,"add %s %s",who,msg) == 2)
		{
			if(who != "msn" && who != "yahoo")
				return tell( me, "\n[IM Message]:�ثe�u��[�J msn �άO yahoo ���n�ͦW��C\n");

			IM_D->send_command(query_temp("im_fd", me), "PRIVMSG #bitlbee :add "+who+" "+msg);

			return tell( me, "\n[IM Message]:�A�� "+msg +" �[�J�A�� "+who +" �n�ͦW��C\n");
		}

		if(sscanf(arg,"remove %s",who) == 1)
		{
			int fd = query_temp("im_fd", me);

			if(!undefinedp(IM_D->query_users()[fd]["list"]) 
			&& !undefinedp(IM_D->query_users()[fd]["list"][who]))
			{
				IM_D->send_command(fd,  "PRIVMSG #bitlbee :remove "+who);
				return tell( me, "\n[IM Message]:�A�� "+who +" �q�A���n�ͦW��R���C\n");
			}
			return tell( me, "\n[IM Message]:�S���o�ӤH�@�I\n");
		}

		if(arg == "yes") 
		{
			IM_D->send_command(query_temp("im_fd", me), "PRIVMSG #bitlbee : yes");
			return tell( me, "\n[IM Message]:�A�P�N�L��A�[�J�L���n�ͦW��C\n");
		}

		if(arg == "no")
		{
			IM_D->send_command(query_temp("im_fd", me),  "PRIVMSG #bitlbee : no");
			return tell( me, "\n[IM Message]:�A���P�N�L�[�A�i�J�L���n�ͦW��C\n");
		}

		if(sscanf(arg,"nick %s",msg))
		{
			tell( me, "\n[IM Message]:�A�N�W�r�אּ�G"+msg+"�C\n");
			IM_D->send_command(query_temp("im_fd", me),  "PRIVMSG #bitlbee :nick msn \""+msg+"\"");
			return;
		}
		tell( me,"\n[IM Message]:�S���o���\��@�C\n");
	}
	else 
	return tell( me,"�A�èS���n�J\n");
	return;
}

void who_list(object me)
{
	mapping map = IM_D->query_users();
	tell( me,"�n�ͦW��C��G\n");

	foreach(int fd,mapping m in map)
	{
		if(m["obj"] == me && me->query_id(1) == m["id"] )
		{
			foreach(string k,string* v in m["list"])
			{
				if(v[2] == m["id"] || v[2] == "root") continue;
			tell( me, sprintf("(%-4s)[%-10s] %-70s \n",v[3]=="G"?"���}":"�u�W",v[2],v[4]+"("+v[0]+"@"+v[1]+")"));
			}
		}
	}
	return;
}

void register(object me,string pro)
{
	tell( me,"[IM Message] �п�J�A�� "+pro+" �b���A�άO�� . ����:");
	input_to("register_step1",me, pro);
	return;
}

void register_step1(string arg,object me,string pro)
{
	if(arg[0] == '.') 
	{
		tell( me, "[IM Message] �����]�w�C\n");
		return;
	}

	if(strlen(arg) < 5) 
	{
		tell( me, "[IM Message] �п�J�A�� "+pro+" �b���A�άO�� . ����:");
		input_to("register_step1",me,pro);
	}

	tell( me, "[IM Message] �п�J�A�� "+pro+" �K�X�A�άO�� . ����:");
	input_to("register_step2",me,pro,arg);
}

void register_step2(string arg,object me,string pro,string account)
{
	if(arg[0] == '.') 
	{
		tell( me, "[IM Message] �����]�w�C\n");
		return;
	}

	if(strlen(arg) < 5) 
	{
		tell( me, "[IM Message] �п�J�A�� "+pro+" �K�X�A�άO�� . ����:");
		input_to("register_step2",me,pro,account);
	}

	IM_D->del_account( query_temp("im_fd", me), pro);
	IM_D->register_account( query_temp("im_fd", me),pro,account,arg);
	tell( me, "[IM Message] "+pro+" �b���s�W�έק粒���C\n");
}
