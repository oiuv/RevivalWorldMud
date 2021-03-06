/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mudlist.c
 * Author : Cookys@RevivalWorld
 * Date   : 2001-2-5
 * Note   : mudlist 指令
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
	標準 mudlist 指令。
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
		str+=HIW"連線"NOR;
		online++;	
	}
	if( status&SHUTDOWN)	str+=YEL"關閉"NOR;
	if( status&DISCONNECT)	str+=WHT"斷線"NOR;

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
		return tell(me, "網路精靈並沒有被載入。\n", CMDMSG);

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

	str+="□INTERMUD_2 MUDLIST\n\n"HIY"Cf"NOR"-系統成功自動判斷編碼類型\n"HIG"GB"NOR"-確認為GB編碼\n"HIR"AD"NOR"-確認有大量廣告訊息\n"HIB"Ig"NOR"-隔絕訊息\n\n";
	str+=sprintf("現在時刻：%s\n\n□確定常駐列表\n%-:20s%-26s%16s%5s %18s %-16s\n",TIME_D->replace_ctime(time()),"名稱","中文名稱(線上人數)","IP 位址","埠","狀態","最後接觸");
	str+=repeat_string("─",53)+"\n";        
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

	str+=repeat_string("─",53)+"\n共 "+sizeof(mudlist)+" 筆資料，有 "+online+" 個 Mud 連線中。\n\n□等待確認列表\n";
	str+=sprintf("%-:20s%-26s%16s%5s %18s %-16s\n","名稱","中文名稱(線上人數)","IP 位置","埠","回應","最後接觸");
	str+=repeat_string("─",53)+"\n";
	foreach(t in v)
	str+=sprintf("%-:20s%-26s%16s%5s %18d %-16s\n",t,(incoming_mudlist[x[t]]["MUDNAME"]||"")+NOR+(incoming_mudlist[x[t]]["USERS"]?"("+incoming_mudlist[x[t]]["USERS"]+")":""),incoming_mudlist[x[t]]["HOSTADDRESS"],incoming_mudlist[x[t]]["PORT"]+"",incoming_mudlist[x[t]]["CONNECTION"],TIME_D->replace_ctime(incoming_mudlist[x[t]]["LASTESTCONTACT"]));
	str+=repeat_string("─",53)+"\n共 "+sizeof(incoming_mudlist)+" 筆資料，下次資訊更新:"+TIME_D->replace_ctime(fetch_variable("refresh_limit",i2)+REFRESH_INCOMING_TIME)+"\n\n總計共 "+sizeof(mudlist+incoming_mudlist)+" 個 Mud 。\n";
	
	online=0;

	if( html )
		HTML_D->translate(str, (["title":"Mudlist", "filename":"/www/mudlist.htm","refresh":60,]));
	else
		me->more(str);
	
	return;
}