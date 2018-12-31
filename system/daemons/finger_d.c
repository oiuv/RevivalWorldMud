/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : finger_d.c
 * Author : Clode@RevivalWorld
 * Date	  : 2001-5-11
 * Note	  : ���a��T���F
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

	msg = (y ? sprintf("%d �~ ", y)	: "" ) + 
	//((m || y) ? sprintf("%d	�Ӥ� ",	m) : ""	) +
	((d || m || y) ? sprintf("%d �� ", d) : "" ) +
	((h || d || m || y) ? sprintf("%d �p�� ",	h) : ""	) +
	((n || h || d || m || y) ? sprintf("%d �� ", n)	: "" ) +
	((s || n || h || d || m || y) ? sprintf("%d ��", s) : "�L���" );

	return msg;
}

string finger_all()
{
	object *users;
	string msg, ip, pip;

	users =	sort_array(users(), (: strcmp(query_ip_number($1), query_ip_number($2)) > 0 ? -1 : 1 :));

	msg  = sprintf("%-12s %6s %8s %5s %s\n", "�W��", "�s�u", "�s�u�`�X", "���m", "�s�u��T"); 
	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";

	// ����ܵn�J�������a
	foreach( object	ob in users )
	{
		msg += sprintf("%-12s %6s %8s %5d %s\n",
		    ansi_capitalize(ob->query_id(1)) || NOR WHT"(���b�n�J)"NOR,
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
	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
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
		return "�S�� " + id + " �o�쪱�a�C\n";
	if( query_temp("net_dead", user) )
		netstat	= 2;
	if( me = this_player() ) // �u�������ΧŮv���[�ݹq�l�H�c
		flag = (me == user || wizardp(me));

	login_time = query_temp("login/time", user);
	last_login = query("last_login/time", user);
	
	msg = "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
	msg += sprintf(
	    "���a�W�١G%-36s "
	    "�ʧO�v���G%s\n"
	    "�s�u�`�X�G%-36s "
	    "�s�u���A�G%s\n"
	    "�q�l�H�c�G%-36s "
	    "���ݫ����G%s\n\n",
	    user->query_idname(),
	    (query("gender", user) == MALE_GENDER ? "�k" : "�k")+" "+capitalize(SECURE_D->level_name(id)),
	    query_time_long(query("total_online_time", user) + (netstat ? 0:time() - query("last_on/time", user))),
	    (netstat ? (netstat == 1 ? "���u":"�_�u") : "�u�W"),
	    (flag ? (query("email", user) || "�L���") : "*** �p�H��T ***"),
	    CITY_D->query_city_idname(query("city", user)) || "�y���C��",
	);
	

	flag = (flag ||	!SECURE_D->is_wizard(id));

	msg += sprintf("�W�����u�G%s (%s)\n",
	    (last_login ? TIME_D->replace_ctime(last_login):"�L���"),
	    (flag ? (query("last_login/ip", user) || "�L���")+(wizardp(me)&&query("last_login/ip_number", user)!=query("last_login/ip_name", user)?" <"+(query("last_login/ip_name", user)||"�L���")+">":""): "*** �w�[�K ***"),
	);

	if( netstat == 1 ) // ���A�����u
	{
		destruct(user);
		msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
		return msg;
	}

	msg += sprintf(
	    "�����W�u�G%s (%s)\n",
	    TIME_D->replace_ctime(login_time),
	    (flag ? (query_ip_number(user) || "�L���")+(wizardp(me)&&query_ip_number(user)!=query_ip_name(user)?" <"+(query_ip_name(user)||"�L���")+">":""): "*** �w�[�K ***"),
	);

	if( netstat == 2 ) // ���A���_�u
		return msg;

	msg += sprintf(
	    "�s�u�ɶ��G%s\n"
	    "���m�ɶ��G%s\n",
	    query_time_long(time() - login_time),
	    ( interactive(user) && query_idle(user) > 180 ? query_time_long(query_idle(user)) : "���ʤ�"),
	);
	
	if( !wizardp(user)	)
	{
		array loc = query_temp("location", user);
		string location;
		object env = environment(user);

		if( env	)
		{
			if( env->is_maproom() )
				location = (CITY_D->query_city_idname(loc[CITY], loc[NUM])||AREA_D->query_area_idname(loc[AREA], loc[NUM]))+(wizardp(me)?"�A�y��<"+(loc[X]+1)+","+(loc[Y]+1)+">":"");
			else
				location = query("short", env);
		}
		msg += sprintf("�Ҧb��m�G%s\n", (location || "�B�b��L���@�ɤ�"));
	}
	
	status = user->query_status();
	
	msg += "�u�@���A�G"+(status||"�L")+"\n";
	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
	return msg;
}


void create()
{

}

string query_name()
{
	return "����d�ߨt��(FINGER_D)";
}
