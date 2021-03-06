/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : who.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : who 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <mudlib.h>
#include <message.h>
#include <status.h>

inherit COMMAND;

#define LIST_ALL	(1<<0)
#define LIST_WIZ	(1<<1)
#define LIST_FILTER	(1<<2)
#define LIST_NO_STATUS	(1<<3)
#define LIST_SHOW_NAME	(1<<4)

string help = @HELP
使用者列表指令

這個指令可簡略列出目前在線上的使用者，其後可接參數。

-l		列出所有使用者的所屬城市、職業、綽號、名字、代號及狀態
-s		不列出使用者詳細的狀態
-w		列出所有在線上的巫師(wizard)
-d		列出目前在線上斷線但還未離開遊戲的使用者
-c		列出同城市的使用者
-m		列出線上所有的新手指導員
-e		列出和您在同一個城市中的玩家
-n		顯示暱稱

選項可混合使用，如 who -lw

指令格式:
who [-[l][w][d][c][m][e]]

相關指令: finger, chfn, nick
HELP;

int sort_user(object user1, object user2)
{
	string user1_id = user1->query_id(1);
	string user2_id = user2->query_id(2);
	string user1_city = query("city", user1);
	string user2_city = query("city", user2);

	if( wizardp(user1) )
	{
		if( !wizardp(user2) ) return -1;

		return SECURE_D->level_num(user2_id) - SECURE_D->level_num(user1_id) || strcmp(user1_id, user2_id);
	}
	else if( wizardp(user2) ) return 1;

	if( !user1_city ) return 1;
	if( !user2_city ) return -1;

	if( user1_city != user2_city )
		return CITY_D->query_city_info(user1_city, "totalflourish") < CITY_D->query_city_info(user2_city, "totalflourish") ? 1 : -1;
	else
		return count(query("totalassets", user2), ">", query("totalassets", user1)) ? 1 : -1;
}

function query_sort_user_fp()
{
	return (: sort_user :);	
}

string who(object me, string arg)
{
	string msg;
	int flag, sizeof_users, sizeof_logining_users, sizeof_wizards;
	function filter;
	object *users;


	users = users();

	// 加入斷線中的使用者
	foreach( array info in call_out_info() )
	if( info[0]->is_user_ob() ) 
		users |= ({ info[0] });

	sizeof_users = sizeof(users);

	users = filter_array(users, (: !$1->is_login_ob() && !$1->is_client_ob() :));

	sizeof_logining_users = sizeof_users - sizeof(users);
	sizeof_wizards = sizeof(filter_array(users, (: wizardp($1) :)));

	if( arg && sscanf( arg, "-%s", arg ) )
	{
		foreach(int a in arg)
		switch(a)
		{
		case 'l':	
			flag |= LIST_ALL;
			break;
		case 'n':
			flag |= LIST_SHOW_NAME;
			break;
		case 's':
			flag |= LIST_NO_STATUS;
			break;
		case 'w':	
			flag |= LIST_FILTER;
			filter = (: wizardp($1) :);
			break;
		case 'd':	
			flag |= LIST_FILTER;
			filter = (: query_temp("net_dead", $1) :);
			break;
		case 'v':
			flag |= LIST_FILTER;
			filter = (: !environment($1) :);
			break;
		case 'm':
			flag |= LIST_FILTER;
			filter = (: query("newbiemaster", $1) :);
			break;
		case 'e':
			flag |= LIST_FILTER;
			filter = (: environment($1) && environment($1)->query_city() ? environment($1)->query_city() == environment($(me))->query_city() : 0 :);
			break;
		case 'c':
			if( objectp(me) )
			{	
				string my_city = query("city", me);
				flag |= LIST_FILTER;
				filter = (: $(my_city) == query("city", $1) :);
				break;
			}
		default: 	break;
		}
	}

	msg = "\n □ "MUD_FULL_NAME"．"HIG"線上玩家列表：\n"NOR;

	// 過濾同樣條件的玩家
	if( flag & LIST_FILTER )
		users = filter_array(users, filter);

	if( !sizeof(users) )
	{
		if( flag & LIST_FILTER )
			return msg+"\n 沒有任何玩家符合輸入的條件。\n\n";
		else
			return msg+"\n 沒有任何線上玩家。\n\n";
	}

	// 進行排序動作
	users = sort_array(users, (: sort_user :));

	if( flag & LIST_ALL )
	{
		string city, status, enterprise, title, nickname;

		msg += "                                                        "NOR WHT"┌───"NOR"───"HIW"───\n"NOR;

		foreach( object user in users )
		{
			if( wizardp(user) || flag & LIST_NO_STATUS )
				status = user->query_status(STATUS_NO_IDLE | STATUS_NO_OBBUFF);
			else
				status = user->query_status(STATUS_NO_IDLE);

			city = CITY_D->query_city_name(query("city", user))||"無國藉";
			enterprise = ENTERPRISE_D->query_enterprise_color_id(query("enterprise", user)) || "";
			title = query("title", user) || NOR WHT"─────"NOR;
			nickname = query("nickname", user) || "";

			msg += sprintf("【%|10s】%-14s %-10s %-16s"NOR WHT"│"NOR"%s %s\n"NOR, city, enterprise, title, nickname, user->query_idname(), status||"");
		}
		msg += sprintf(NOR WHT" %-37s"HIW"───"NOR"───"NOR WHT"───┴───"NOR"───"HIW"───\n"NOR, "共有 "+sizeof(users)+" 位符合搜尋條件");
	}
	else
	{
		string *names;
		int name_len, len, width = 3;

		if( flag & LIST_SHOW_NAME )
		{
			names = users->query_idname();
			name_len = 24;
		} else {
			names = users->query_id();
			name_len = 16;
		}

		if( me ) 
			width = (query_temp("windowsize/width", me)/(name_len+1)) || 3;

		msg += " ────────────\n";

		//foreach( object user in users )
		foreach(string name in names)
		{
			msg += sprintf(" %-"+name_len+"s", name) + (++len%width ? "" : "\n");
		}

		if(len%width) msg += "\n";
		msg += " ──────────── "NOR WHT"共有 "+sizeof(users)+" 位符合搜尋條件\n"NOR;
	}

	msg += HIG" 人數統計："NOR GRN"共 "+(sizeof_users)+" 位使用者，包括 "+sizeof_wizards+" 位巫師、"+(sizeof_users-sizeof_wizards-sizeof_logining_users)+" 位玩家，以及 "+sizeof_logining_users+" 人正在進入遊戲。\n"HIG" 系統負載："NOR GRN+query_load_average()+"。\n"NOR;

	return msg;
}

private void do_command(object me, string arg)
{
	me->more(who(me, arg));
}
