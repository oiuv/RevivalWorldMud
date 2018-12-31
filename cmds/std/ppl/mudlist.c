/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mudlist.c
 * Author : Cookys@RevivalWorld
 * Date   : 2001-2-5
 * Note   : mudlist ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <i2d.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� mudlist ���O�C
HELP;

int online;

string cstatus(int status)
{
	string str="";
	
	str += status & ENCODE_CONFIRM 	? HIY"Cf"NOR"|" : YEL"Cf"NOR"|";
	str += status & GB_CODE		? HIG"GB"NOR"|" : GRN"GB"NOR"|";
	str += status & ANTI_AD		? HIR"AD"NOR"|" : RED"AD"NOR"|";
	str += status & IGNORED		? HIB"Ig"NOR"|--" : BLU"Ig"NOR"|--";

	if( status & ONLINE)
	{
		str+=HIW"�s�u"NOR;
		online++;	
	}
	if( status&SHUTDOWN)	str+=YEL"����"NOR;
	if( status&DISCONNECT)	str+=WHT"�_�u"NOR;

	return sprintf("%-18s",str);

}

// call by HTML_D, no private declare
varargs void do_command(object me, string arg, int html)
{
	mapping mudlist,incoming_mudlist,d,z=([]),x=([]);
	object i2;
	string str="",t;
	mixed c=({}),v=({});

	if( !i2=find_object(INTERMUD2_D) )
		return tell(me, "�������F�èS���Q���J�C\n", CMDMSG);

	if( arg )
	{
		mapping m=fetch_variable("mudlist",i2)+fetch_variable("incoming_mudlist",i2);
		string *msg;
		if( msg = i2->fetch_mudname(arg,1) )
		{
			foreach(string a in msg)
			printf("%O\n",m[a]);
			tell(me, "ok\n", CMDMSG);
		}
		else tell(me, "no such mud !\n", CMDMSG);
		return;
	}

	mudlist = fetch_variable("mudlist",i2);
	incoming_mudlist= fetch_variable("incoming_mudlist",i2);

	foreach(t,d in mudlist)
	if(!undefinedp(z[d["NAME"]]))
	{
		if( typeof(z[d["NAME"]])=="array" )
			z[d["NAME"]]=z[d["NAME"]]+({t});
		else    z[d["NAME"]]=({z[d["NAME"]] })+({t});
	}
	else    z[d["NAME"]]=t;

	foreach(t,d in incoming_mudlist)
	x[d["NAME"]]=t;

	c=keys(z);
	c = sort_array(c,1);
	v=keys(x);
	v = sort_array(v,1);

	str+="��INTERMUD_2 MUDLIST\n\n"HIY"Cf"NOR"-�t�Φ��\�۰ʧP�_�s�X����\n"HIG"GB"NOR"-�T�{��GB�s�X\n"HIR"AD"NOR"-�T�{���j�q�s�i�T��\n"HIB"Ig"NOR"-�j���T��\n\n";
	str+=sprintf("�{�b�ɨ�G%s\n\n���T�w�`�n�C��\n%-:20s%-26s%16s%5s %18s %-16s\n",TIME_D->replace_ctime(time()),"�W��","����W��(�u�W�H��)","IP ��}","��","���A","�̫ᱵĲ");
	str+=repeat_string("�w",53)+"\n";        
	foreach(t in c)
	{
		
		string color="";
//		shout(sprintf("%s	\n",t));
		if( typeof(z[t])=="array" )
		{
			foreach(mixed tmp in z[t])
			{
				if(time()-mudlist[tmp]["LASTESTCONTACT"] > 24*60*60 ) color=HIR;
				else color="";
				if( mudlist[tmp]["STATUS"] & GB_CODE || (!undefinedp(mudlist[tmp]["ENCODING"]) && lower_case(mudlist[tmp]["ENCODING"]))=="gb")
					str+=sprintf("%-:20s%-26s%16s%5s %18s %-16s\n"NOR,remove_ansi(G2B(t)),(mudlist[tmp]["MUDNAME"]?G2B(mudlist[tmp]["MUDNAME"]):"")+NOR+(mudlist[tmp]["USERS"]?"("+mudlist[tmp]["USERS"]+")":""),mudlist[tmp]["HOSTADDRESS"],mudlist[tmp]["PORT"]+"",cstatus(mudlist[tmp]["STATUS"]),color+TIME_D->replace_ctime(mudlist[tmp]["LASTESTCONTACT"]));
				else 	str+=sprintf("%-:20s%-26s%16s%5s %18s %-16s\n"NOR,remove_ansi(t),mudlist[tmp]["MUDNAME"]||"",mudlist[tmp]["HOSTADDRESS"],mudlist[tmp]["PORT"]+"",cstatus(mudlist[tmp]["STATUS"]),color+TIME_D->replace_ctime(mudlist[tmp]["LASTESTCONTACT"]));	
			}
		}

		else 
		{
			if(time()-mudlist[z[t]]["LASTESTCONTACT"]> 24*60*60 ) color=HIR;
			else color="";
			if( mudlist[z[t]]["STATUS"]& GB_CODE || (!undefinedp(mudlist[z[t]]["ENCODING"]) && lower_case(mudlist[z[t]]["ENCODING"])=="gb"))
				str+=sprintf("%-:20s%-26s%16s%5s %18s %-16s\n"NOR,t,(mudlist[z[t]]["MUDNAME"]?G2B(mudlist[z[t]]["MUDNAME"]):"")+NOR+(mudlist[z[t]]["USERS"]?"("+mudlist[z[t]]["USERS"]+")":""),mudlist[z[t]]["HOSTADDRESS"],mudlist[z[t]]["PORT"]+"",cstatus(mudlist[z[t]]["STATUS"]),color+TIME_D->replace_ctime(mudlist[z[t]]["LASTESTCONTACT"]));
//				str+=sprintf("%-:20s%-26s%16s%5s %18s %-16s\n"NOR,remove_ansi(G2B(t)),mudlist[z[t]]["MUDNAME"]?G2B(mudlist[z[t]]["MUDNAME"]):"",mudlist[z[t]]["HOSTADDRESS"],mudlist[z[t]]["PORT"]+"",cstatus(mudlist[z[t]]["STATUS"]),color+TIME_D->replace_ctime(mudlist[z[t]]["LASTESTCONTACT"]));
			else 	str+=sprintf("%-:20s%-26s%16s%5s %18s %-16s\n"NOR,remove_ansi(t),mudlist[z[t]]["MUDNAME"]||"",mudlist[z[t]]["HOSTADDRESS"],mudlist[z[t]]["PORT"]+"",cstatus(mudlist[z[t]]["STATUS"]),color+TIME_D->replace_ctime(mudlist[z[t]]["LASTESTCONTACT"]));
		}

	}

	str+=repeat_string("�w",53)+"\n�@ "+sizeof(mudlist)+" ����ơA�� "+online+" �� Mud �s�u���C\n\n�����ݽT�{�C��\n";
	str+=sprintf("%-:20s%-26s%16s%5s %18s %-16s\n","�W��","����W��(�u�W�H��)","IP ��m","��","�^��","�̫ᱵĲ");
	str+=repeat_string("�w",53)+"\n";
	foreach(t in v)
	str+=sprintf("%-:20s%-26s%16s%5s %18d %-16s\n",t,(incoming_mudlist[x[t]]["MUDNAME"]||"")+NOR+(incoming_mudlist[x[t]]["USERS"]?"("+incoming_mudlist[x[t]]["USERS"]+")":""),incoming_mudlist[x[t]]["HOSTADDRESS"],incoming_mudlist[x[t]]["PORT"]+"",incoming_mudlist[x[t]]["CONNECTION"],TIME_D->replace_ctime(incoming_mudlist[x[t]]["LASTESTCONTACT"]));
	str+=repeat_string("�w",53)+"\n�@ "+sizeof(incoming_mudlist)+" ����ơA�U����T��s:"+TIME_D->replace_ctime(fetch_variable("refresh_limit",i2)+REFRESH_INCOMING_TIME)+"\n\n�`�p�@ "+sizeof(mudlist+incoming_mudlist)+" �� Mud �C\n";
	
	online=0;

	if( html )
		HTML_D->translate(str, (["title":"Mudlist", "filename":"/www/mudlist.htm","refresh":60,]));
	else
		me->more(str);
	
	return;
}