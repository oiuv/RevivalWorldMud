/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : login_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-9-21
 * Note   : �n�����F
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ip.h>
#include <ansi.h>
#include <daemon.h>
#include <login.h>
#include <object.h>
#include <secure.h>
#include <mudlib.h>
#include <message.h>
#include <time.h>
#include <gender.h>

private nosave string *welcome_messages, welcome_message;	// �w��T��
private nosave mapping wrong_password;				// ���~�K�X����
private int wizlock;						// �Ův�n�J������w
nosave int total_user;						// ���������ɮ׼�
mapping login_count;						// �n�J�έp
mapping ip_use;							// IP �ϥά���
private nosave mapping login_attacker;				// �ˬd�n�J����		


int save()
{
	return save_object(DATA);
}
int remove()
{
	return save();
}

void clear_ip_use() { ip_use = ([]); }

/* IP �ϥΰO�� */
void remove_ip_use(string ip_or_id)
{
	map_delete(ip_use, ip_or_id);

	foreach(string ip, string *id in ip_use)
	{
		if( member_array(ip_or_id, id) != -1 )
			ip_use[ip] -= ({ ip_or_id });
	}
}
varargs mixed query_ip_use(string ip)
{
	if( !undefinedp(ip) )
		return ip_use[ip];

	return ip_use;
}

/* �n�J�����v�� */
int set_wiz_lock(int i) // �]�w�n�J�����v��
{
	wizlock = i;
	save();
	return i;
}
int query_wiz_lock() // �^�ǵn�J�����v��
{
	return wizlock;
}


/* �n�J��T */
mapping query_login_count() // �^�ǵn�J�p�Ƹ�T
{
	return login_count;
}
void set_login_count() // �]�w�n�J��T
{
	int hour = TIME_D->query_realtime_array()[HOUR];
	int users_size = sizeof(users());

	login_count["total"]++;

	if( users_size > query("login/maxusers/hour/"+hour, SYSDATABASE_D->query_ob()) )
		set("login/maxusers/hour/"+hour, users_size, SYSDATABASE_D->query_ob());

	if( users_size > login_count["max"] )
	{
		login_count["max"] = users_size;
		save();
	}
}
void reset_login_count(int hour)
{
	delete("login/maxusers/hour/"+hour, SYSDATABASE_D->query_ob());

	set_login_count();
}

/* �ˬd�n�J������ */
int check_login_attacker(string ip)
{
	int time = time();

	foreach(string attacker, mapping data in login_attacker)
	{
		if( time - data["time"] > 600 )
			map_delete(login_attacker, attacker);
	}

	if( !mapp(login_attacker[ip]) )
	{
		login_attacker[ip] = allocate_mapping(0);
		login_attacker[ip]["times"] = 0;
	}
	
	login_attacker[ip]["time"] = time;
	
	if( ++login_attacker[ip]["times"] >= 20 )
		return 0;

	return 1;
}

mapping query_login_attacker()
{
	return login_attacker;
}

void set_login_attacker(string ip, int times)
{
	login_attacker[ip] = allocate_mapping(0);
	login_attacker[ip]["time"] = time();
	login_attacker[ip]["times"] = times;
}

// �ˬd�O�_���еn�J
private nomask int is_repeat_login(string arg)
{
	foreach(object child in children(LOGIN_OB))
	if( query("id", child) == arg ) return 1;
	return 0;
}

// �H���w��T��
string random_welcome_message()
{
	return welcome_messages[random(sizeof(welcome_messages))];
}

// ��� motd
void show_motd(object user)
{
	string msg = terminal_colour(read_file(MOTD), ([
		"MUD_NAME"	: MUD_FULL_NAME,
		"MUDLIB_VERSION": MUDLIB_VERSION_NUMBER,
		"MUDOS_VERSION"	: __VERSION__ ,
		"ARCH"		: __ARCH__,
		"UPDATE"	: TIME_D->replace_ctime(stat(MOTD)[1]),
	    ]));
	tell(user, msg);
}

private void remove_player(object ob)
{
	if( ob ) LOGOUT_D->quit(ob, 0);
	return;
}

// �ˬd���п�J���~�K�X
int input_wrong_password(object user)
{
	return ++wrong_password[user];
}

void add_login_message(string id, string msg)
{
	object player;

	if( !objectp(player = load_user(id)) ) return;
	msg = sprintf("%s[%s]- %s",
	    query("login_message", player) || "",
	    TIME_D->replace_ctime(time()),
	    msg
	);
	set("login_message", msg, player);	
	player->save();

	if( !userp(player) )
		destruct(player);
}

nomask string show_login_message(object login_ob)
{
	string msg;
	int cities, logins, wizards, players;

	msg = random_welcome_message();
	/* �p��ϥΪ̼� */
	foreach(object ob in users())
	{
		if( ob->is_user_ob() )
			(wizardp(ob) ? wizards++ : players++);
		else if( ob->is_login_ob() )
			logins++;
	}

	cities = sizeof(CITY_D->query_cities());
	msg += sprintf(
	    HIW"���ͪ��@��"NOR"�ثe�@�� %d �y�����B%s �Ө���A�w�p�s�u�H���@ %s ���C\n"
	    "�ثe�� %d ��Ův�B%d �쪱�a�A�@�p %d �H���b�u�W�A%d �H���b�n�J�C\n", 
	    cities, NUMBER_D->number_symbol(total_user), NUMBER_D->number_symbol(login_count["total"]),
	    wizards, players, wizards + players, logins);
	
	tell(login_ob, msg, SYSMSG);
}



// ���ݶi�J�C��
private nomask varargs void waiting_enter_game(object user, int timer, string arg)
{
	string ip;
	mixed quit_place;

	if( !user ) return;

	if( --timer )
	{
		//tell(user, ".");
		set_temp("call_out_handle", call_out( (: waiting_enter_game, user, timer :), 1 ), user);
		return;
	}

	if( intp(query_temp("call_out_handle", user)) )
		remove_call_out(copy(query_temp("call_out_handle", user)));

	delete_temp("call_out_handle", user);
	remove_input_to(user);

	//tell(user, ".\n");
	//user->show_msg_buffer();

	user->clear_msg_buffer();

	quit_place = copy(query("quit_place", user));

	if( !query_temp("reconnect", user) )
		switch( typeof(quit_place) )
	{
	case "array":
		if( MAP_D->query_map_system(quit_place) )
		{
			startup_title_screen(user, 11);
			user->move(quit_place);
		}
		else
		{
			tell(user, pnoun(2, user)+"�쥻���u�ɪ����Ҥw�g�����F�C\n");

			if( wizardp(user) )
				user->move(WIZROOM);
			else
				user->move(STARTROOM);
		}
		break;
	case "string":

		intact_file_name(ref quit_place);

		if( file_exist(quit_place) )
		{
			if( quit_place[<2..<1] == ".o" )
				quit_place = quit_place[0..<3];

			if( load_object(quit_place) == load_object(VOID_OB) )
				user->move(STARTROOM);
			else
				user->move(quit_place);
		}
		else
		{
			tell(user, pnoun(2, user)+"�쥻���u�ɪ����Ҥw�g�����F�C\n");

			if( wizardp(user) )
				user->move(WIZROOM);
			else
				user->move(STARTROOM);
		}
		break;
	default:
		if( wizardp(user) )
			user->move(WIZROOM);
		else
			user->move(STARTROOM);
	}

	user->finish_input();

	if( query_temp("reconnect", user) )
	{
		// ������u�����ұ����쪺�T��
		int net_dead_time = query_temp("net_dead_time", user);
		array msg_data = user->query_last_msg();

		if( net_dead_time )
			tell(user, YEL"\n"+pnoun(2, user)+"���_�u�� " + ctime(net_dead_time) + " ��{�b�L�F " + ((time()-net_dead_time)/60) + " �� " + ((time()-net_dead_time)%60) + " ��C\n"NOR);

		if( sizeof(msg_data) && net_dead_time && msg_data[<2] > net_dead_time )
		{
			tell(user, HIY"\n�b"+pnoun(2, user)+"�_�u��������������s���T���A�Шϥ� msg ���O�d�ݡC\n"NOR);
		}
		delete_temp("net_dead_time", user);
		msg("$ME���s�s�u�i�J�C���C\n", user, 0, 1);
		CHANNEL_D->channel_broadcast("sys","�n�J�G"+user->query_idname()+WHT"��"+NETWORK_D->query_network_short(user)+"���s�s�u�i�J�C", 1);
	}
	else
	{
		msg("$ME�i�J�C���C\n", user, 0, 1);
		CHANNEL_D->channel_broadcast("sys", "�n�J�G"+user->query_idname()+WHT"��"+NETWORK_D->query_network_short(user)+"�s�u�i�J�C", 1);
	}
	log_file(LOG, user->query_idname()+" login "+NETWORK_D->query_network_short(user)+NOR+"\n");

	// �����ϥιL�P�@ IP �����a
	ip = query_ip_number(user);

	if( stringp(ip) )
	{
		if( undefinedp(ip_use[ip]) )
			ip_use[ip] = ({ user->query_id(1) });
		else
			ip_use[ip] |= ({ user->query_id(1) });
	}

	if(query("faint", user)) {
		set_temp("disable/msg", 1, user);
		set_temp("disable/cmd", 1, user);
	}

}

// ���\���еn�J
int valid_multi_login(object user1, object user2)
{
	string *allowmulti1 = query("login/allowmulti", user1);
	string *allowmulti2 = query("login/allowmulti", user2);

	if( arrayp(allowmulti1) && member_array(user2->query_id(1), allowmulti1) != -1 )
		return 1;

	if( arrayp(allowmulti2) && member_array(user1->query_id(1), allowmulti2) != -1 )
		return 1;

	return 0;
}

// �ǳƶi�J�C��
void enter_game(object user)
{
	string my_ip, my_ident;

	// �Ұʤ��ʩʽ�
	user->enable_interactive();

	// �ҰʥͩR�{�H
	user->startup_living();

	set_temp("login/ip", query_ip_number(user), user);
	set_temp("login/ip_name", query_ip_name(user), user);
	set_temp("login/ip_port", query_ip_port(user), user);

	// �p��n���H��
	set_login_count();

	/* ��ܤ��G�� */
	show_motd(user);

	set("last_on/time", time(), user);

	// ���^���W���~
	if( !query_temp("reconnect", user) )
	{
		user->restore_inventory();
		set_temp("login/time", time(), user);
	}

	// �n�����[�T��
	if( query("login_message", user) )
	{
		tell(user, "\n" HIG "�t�ΰT���G\n"+query("login_message", user)+NOR+"\n\n");
		write_file(file_name(user)+"/old_login_message",query("login_message", user));
		delete("login_message", user);
	}


	my_ip = query_ip_number(user);
	my_ident = query_temp("login/ident", user);

	foreach(object ppl in users())
	{
		if( ppl == user || !environment(ppl) || !interactive(ppl) ) continue;

		if( query_ip_number(ppl) == my_ip )
		{				
			if( SECURE_D->level_num(ppl->query_id(1)) >= GUEST || SECURE_D->level_num(user->query_id(1)) >= GUEST )
				CHANNEL_D->channel_broadcast("sys", "�n�J�G"+user->query_idname()+"�P"+ppl->query_idname()+"�ۦP IP �X�k�n�J(�Ův����)�C");
			else if( valid_multi_login(user, ppl) )
				CHANNEL_D->channel_broadcast("sys", "�n�J�G"+user->query_idname()+"�P"+ppl->query_idname()+"�ۦP IP �X�k�n�J�C");
			else
			{
				tell(user, "\n"HIR+pnoun(2, user)+"�P"+ppl->query_idname()+HIR+"���еn���A�иԾ\���C���W�h(help rule)�C\n�z���H�W�ʧ@�w�V�Ův�q���A�P�ɱz�N�|�b 30 ���۰ʥѨt�ν����u�C\n�Y�O�Q�� IP ���ɾ��άO��L�S�����]�̥i�V�Ův�ӽж}�� IP ����C\n"NOR);
				CHANNEL_D->channel_broadcast("sys", "�n�J�G"+user->query_idname()+"�P"+ppl->query_idname()+"�ۦP IP �H�W�n�J�A30 ���𰣡C");
				
				if( query_ip_number(ppl) == "75.84.17.242" || query_ip_name(ppl)[<13..] == "csupomona.edu" )
				{
					CHANNEL_D->channel_broadcast("sys", "�n�J�G"+user->query_idname()+"�P"+ppl->query_idname()+"�ۦP IP �H�W�n�J�A���W�𰣡C");
					destruct(user);
					destruct(ppl);
					
				} else { 
					call_out( (: remove_player, user :), 30);
					set("cmdlimit", 30, user);
				}
			}
		}
	}

	if( IP_D->query_ip_status(my_ip) == CMDLIMIT && SECURE_D->level_num(user->query_id(1)) < GUEST)
	{
		set("cmdlimit", 30, user);
		CHANNEL_D->channel_broadcast("sys", "�n�J�G"+user->query_idname()+"�ҨϥΪ� IP ��m�N������O����t�׭���C");
	}
	else
		delete("cmdlimit", user);

	tell(user, NOR WHT"�п�J���N���~��� "+WAITING_ENTER_TIME+" ���۰ʶi�J�C��\n"NOR);
	set_temp("call_out_handle", call_out( (: waiting_enter_game, user, WAITING_ENTER_TIME :), 1 ), user);

	input_to( (: waiting_enter_game, user, 1 :) );
		
	if( query_temp("new_player", user) )
		CHANNEL_D->channel_broadcast("news", "�s���a "+user->query_idname()+"-"+(query("gender", user)==MALE_GENDER?HIG"�k"NOR:HIR"�k"NOR)+" ���ߦa�i�J�F�o�ӥ@�ɡA�@�y��M���ˤl...�C", 1);
}

// ��s�ϥΪ̪���
nomask void update_player(object player)
{
	string id;
	object *inv, temp;
	mapping temp_database;
	mixed env;
	int status;

	if( !objectp(player) || !interactive(player) ) return;

	temp_database = player->query_temp_database();

	inv = all_inventory(player);

	player->save();

	temp = new(VOID_OB);

	inv->move(temp);

	exec( temp, player );

	id = player->query_id(1);

	env = copy(query_temp("location", player) || environment(player) || load_object(VOID_OB));

	destruct(player);

	player = load_user(id);

	// �����ʨ� env
	player->move(env);

	exec( player, temp );

	player->enable_interactive();
	player->startup_living();

	inv->move(player);

	foreach(object ob in inv)
	if( ob->is_equipping() )
		player->equip(ob, &status);

	destruct(temp);

	player->set_temp_database(temp_database);

	tell(player, pnoun(2, player)+"��M�Pı��ۤv"HIW"�F"NOR WHT"��"NOR"�i�J�F�@�ӷs������....�C\n");
	CHANNEL_D->channel_broadcast("sys","��s�G"+player->query_idname()+NETWORK_D->query_network_short(player)+"��s���駹���C");
}

/* �ɶ��Ƶ{ (�C�p��) */
void time_schedule()
{
	int i;

	// �H�����w��T��
	while(file_size(WELCOME_MSG + i) > 0)
	{
		welcome_messages += ({ kill_repeat_ansi(terminal_colour(read_file(WELCOME_MSG + i),
			    ([
				"DRIVER"	: __VERSION__,
				"VERSION"	: MUDLIB_VERSION_NUMBER,
			    ])))
		});
		i++;
	}

	// ���s�p���`�H��
	total_user = 0;
	foreach(string prefix in get_dir("/data/user/"))
	total_user += sizeof(get_dir("/data/user/" + prefix + "/"));

	set_login_count();

	save();
}
private nomask void initialize()
{
	welcome_messages = allocate(0);
	wrong_password = allocate_mapping(0);
	login_attacker = allocate_mapping(0);

	if( !mapp(login_count) )	login_count = allocate_mapping(0);
	if( !mapp(ip_use) )		ip_use = allocate_mapping(0);

	time_schedule();
}


private void create()
{
	if( clonep() )
	{
		destruct(this_object());
		return;
	}
	if( !restore_object(DATA) ) save();
	initialize();
	
	SCHEDULE_D->set_event( 60*69*24, 1, load_object(PPL_LOGIN_D), "reset_cmdlimit");
}
string query_name()
{
	return "�n�J�t��(LOGIN_D)";
}
