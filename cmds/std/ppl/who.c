/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : who.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : who ���O
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
�ϥΪ̦C����O

�o�ӫ��O�i²���C�X�ثe�b�u�W���ϥΪ̡A���i���ѼơC

-l		�C�X�Ҧ��ϥΪ̪����ݫ����B¾�~�B�︹�B�W�r�B�N���Ϊ��A
-s		���C�X�ϥΪ̸ԲӪ����A
-w		�C�X�Ҧ��b�u�W���Ův(wizard)
-d		�C�X�ثe�b�u�W�_�u���٥����}�C�����ϥΪ�
-c		�C�X�P�������ϥΪ�
-m		�C�X�u�W�Ҧ����s����ɭ�
-e		�C�X�M�z�b�P�@�ӫ����������a
-n		��ܼʺ�

�ﶵ�i�V�X�ϥΡA�p who -lw

���O�榡:
who [-[l][w][d][c][m][e]]

�������O: finger, chfn, nick
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

	// �[�J�_�u�����ϥΪ�
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

	msg = "\n �� "MUD_FULL_NAME"�D"HIG"�u�W���a�C��G\n"NOR;

	// �L�o�P�˱��󪺪��a
	if( flag & LIST_FILTER )
		users = filter_array(users, filter);

	if( !sizeof(users) )
	{
		if( flag & LIST_FILTER )
			return msg+"\n �S�����󪱮a�ŦX��J������C\n\n";
		else
			return msg+"\n �S������u�W���a�C\n\n";
	}

	// �i��Ƨǰʧ@
	users = sort_array(users, (: sort_user :));

	if( flag & LIST_ALL )
	{
		string city, status, enterprise, title, nickname;

		msg += "                                                        "NOR WHT"�z�w�w�w"NOR"�w�w�w"HIW"�w�w�w\n"NOR;

		foreach( object user in users )
		{
			if( wizardp(user) || flag & LIST_NO_STATUS )
				status = user->query_status(STATUS_NO_IDLE | STATUS_NO_OBBUFF);
			else
				status = user->query_status(STATUS_NO_IDLE);

			city = CITY_D->query_city_name(query("city", user))||"�L����";
			enterprise = ENTERPRISE_D->query_enterprise_color_id(query("enterprise", user)) || "";
			title = query("title", user) || NOR WHT"�w�w�w�w�w"NOR;
			nickname = query("nickname", user) || "";

			msg += sprintf("�i%|10s�j%-14s %-10s %-16s"NOR WHT"�x"NOR"%s %s\n"NOR, city, enterprise, title, nickname, user->query_idname(), status||"");
		}
		msg += sprintf(NOR WHT" %-37s"HIW"�w�w�w"NOR"�w�w�w"NOR WHT"�w�w�w�r�w�w�w"NOR"�w�w�w"HIW"�w�w�w\n"NOR, "�@�� "+sizeof(users)+" ��ŦX�j�M����");
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

		msg += " �w�w�w�w�w�w�w�w�w�w�w�w\n";

		//foreach( object user in users )
		foreach(string name in names)
		{
			msg += sprintf(" %-"+name_len+"s", name) + (++len%width ? "" : "\n");
		}

		if(len%width) msg += "\n";
		msg += " �w�w�w�w�w�w�w�w�w�w�w�w "NOR WHT"�@�� "+sizeof(users)+" ��ŦX�j�M����\n"NOR;
	}

	msg += HIG" �H�Ʋέp�G"NOR GRN"�@ "+(sizeof_users)+" ��ϥΪ̡A�]�A "+sizeof_wizards+" ��Ův�B"+(sizeof_users-sizeof_wizards-sizeof_logining_users)+" �쪱�a�A�H�� "+sizeof_logining_users+" �H���b�i�J�C���C\n"HIG" �t�έt���G"NOR GRN+query_load_average()+"�C\n"NOR;

	return msg;
}

private void do_command(object me, string arg)
{
	me->more(who(me, arg));
}
