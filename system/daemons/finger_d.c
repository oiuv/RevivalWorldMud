/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : finger_d.c
 * Author : Clode@RevivalWorld
 * Date	  : 2001-5-11
 * Note	  : 玩家資訊精靈
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <location.h>
#include <gender.h>

string query_time_short(int t)
{
	int y, m, d, h;
	string msg;
	//y = t	/60/60/24/365;
	//m = t	/60/60/24/30%13;
	d = t /60/60/24;//%30;
	h = t /60/60%24;

	msg = (y ? sprintf("%2dY", y) :	"" ) + 
	((m || y) ? sprintf("%2dM", m) : "" ) +
	((d || m || y) ? sprintf("%2dD", d) : "" ) +
	((h || d || m || y) ? sprintf("%2dH", h) : "<1H" );
	return msg;
}

string query_time_long(int t)
{
	int y, m, d, h,	n, s;
	string msg;
	y = t /60/60/24/365;
	//m = t	/60/60/24/30%13;
	d = t /60/60/24%365;
	h = t /60/60%24;
	n = t /60%60;
	s = t %60;

	msg = (y ? sprintf("%d 年 ", y)	: "" ) + 
	//((m || y) ? sprintf("%d	個月 ",	m) : ""	) +
	((d || m || y) ? sprintf("%d 天 ", d) : "" ) +
	((h || d || m || y) ? sprintf("%d 小時 ",	h) : ""	) +
	((n || h || d || m || y) ? sprintf("%d 分 ", n)	: "" ) +
	((s || n || h || d || m || y) ? sprintf("%d 秒", s) : "無資料" );

	return msg;
}

string finger_all()
{
	object *users;
	string msg, ip, pip;

	users =	sort_array(users(), (: strcmp(query_ip_number($1), query_ip_number($2)) > 0 ? -1 : 1 :));

	msg  = sprintf("%-12s %6s %8s %5s %s\n", "名稱", "連線", "連線總合", "閒置", "連線資訊"); 
	msg += "───────────────────────────────────\n";

	// 先顯示登入中之玩家
	foreach( object	ob in users )
	{
		msg += sprintf("%-12s %6s %8s %5d %s\n",
		    ansi_capitalize(ob->query_id(1)) || NOR WHT"(正在登入)"NOR,
		    query_temp("login/time", ob) ? query_time_short(time() - query_temp("login/time", ob)) : "",
		    query_temp("login/time", ob) ? query_time_short(query("total_online_time", ob) + (time() - query_temp("login/time", ob))) : "",
		    query_idle(ob)/60,
		    NETWORK_D->query_network_short(ob));

		ip = query_ip_number(ob);
		// Class C
		pip = ip[0..strsrch(ip, ".", -1)];
		if( strsrch(msg, pip) != strsrch(msg, pip, -1) )
			msg = replace_string(msg, pip, HIY + pip + NOR);
		/*
		// Class D
		if( strsrch(msg, ip) != strsrch(msg, ip, -1) )
			msg = replace_string(msg, ip, HIR + ip + NOR);*/

	}
	msg += "───────────────────────────────────\n";
	return msg;
}

string finger_user(string id)
{
	int netstat, login_time, last_login, flag;
	string msg, status;
	object me, user;

	user = find_player(id);
	if( !objectp(user) )
	{
		user = load_user(id);
		netstat	= 1;
	}
	if( !objectp(user) )
		return "沒有 " + id + " 這位玩家。\n";
	if( query_temp("net_dead", user) )
		netstat	= 2;
	if( me = this_player() ) // 只有本身及巫師能觀看電子信箱
		flag = (me == user || wizardp(me));

	login_time = query_temp("login/time", user);
	last_login = query("last_login/time", user);
	
	msg = "───────────────────────────────────\n";
	msg += sprintf(
	    "玩家名稱：%-36s "
	    "性別權限：%s\n"
	    "連線總合：%-36s "
	    "連線狀態：%s\n"
	    "電子信箱：%-36s "
	    "所屬城市：%s\n\n",
	    user->query_idname(),
	    (query("gender", user) == MALE_GENDER ? "男" : "女")+" "+capitalize(SECURE_D->level_name(id)),
	    query_time_long(query("total_online_time", user) + (netstat ? 0:time() - query("last_on/time", user))),
	    (netstat ? (netstat == 1 ? "離線":"斷線") : "線上"),
	    (flag ? (query("email", user) || "無資料") : "*** 私人資訊 ***"),
	    CITY_D->query_city_idname(query("city", user)) || "流浪遊民",
	);
	

	flag = (flag ||	!SECURE_D->is_wizard(id));

	msg += sprintf("上次離線：%s (%s)\n",
	    (last_login ? TIME_D->replace_ctime(last_login):"無資料"),
	    (flag ? (query("last_login/ip", user) || "無資料")+(wizardp(me)&&query("last_login/ip_number", user)!=query("last_login/ip_name", user)?" <"+(query("last_login/ip_name", user)||"無資料")+">":""): "*** 已加密 ***"),
	);

	if( netstat == 1 ) // 狀態為離線
	{
		destruct(user);
		msg += "───────────────────────────────────\n";
		return msg;
	}

	msg += sprintf(
	    "本次上線：%s (%s)\n",
	    TIME_D->replace_ctime(login_time),
	    (flag ? (query_ip_number(user) || "無資料")+(wizardp(me)&&query_ip_number(user)!=query_ip_name(user)?" <"+(query_ip_name(user)||"無資料")+">":""): "*** 已加密 ***"),
	);

	if( netstat == 2 ) // 狀態為斷線
		return msg;

	msg += sprintf(
	    "連線時間：%s\n"
	    "閒置時間：%s\n",
	    query_time_long(time() - login_time),
	    ( interactive(user) && query_idle(user) > 180 ? query_time_long(query_idle(user)) : "活動中"),
	);
	
	if( !wizardp(user)	)
	{
		array loc = query_temp("location", user);
		string location;
		object env = environment(user);

		if( env	)
		{
			if( env->is_maproom() )
				location = (CITY_D->query_city_idname(loc[CITY], loc[NUM])||AREA_D->query_area_idname(loc[AREA], loc[NUM]))+(wizardp(me)?"，座標<"+(loc[X]+1)+","+(loc[Y]+1)+">":"");
			else
				location = query("short", env);
		}
		msg += sprintf("所在位置：%s\n", (location || "處在虛無的世界中"));
	}
	
	status = user->query_status();
	
	msg += "工作狀態："+(status||"無")+"\n";
	msg += "───────────────────────────────────\n";
	return msg;
}


void create()
{

}

string query_name()
{
	return "角色查詢系統(FINGER_D)";
}
