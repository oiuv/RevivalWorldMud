/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wiz_login_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2000-12-29
 * Note   : �Ův�n�����F
 * Update :
 *  o 2001-02-09 Clode �R�� port ���k, ��i�@�ǵ{���g�k
 *  o 2001-07-20 Clode ���g repeat login id ���ˬd
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <login.h>
#include <daemon.h>
#include <object.h>
#include <secure.h>
#include <mudlib.h>
#include <message.h>

#define INITIALIZE		1
#define INPUT_ID		2
#define INPUT_PASSWORD		3
#define RECONNECT		4
#define OLD_PLAYER		5
#define ENTER_GAME		6

private nomask varargs void logon_handle(int state, object ob, string arg)
{
	ob->reset_time_out(); // ���]Time_out�ɶ�

	if( arg && query("encode/gb", ob) ) arg = G2B(arg);
	
	switch(state)
	{
	/* �ҩl Login */
	case INITIALIZE:
		{
			ob->directly_receive("Current charset is " + (query("encode/gb", ob) ? "Simplified Chinese (GB)":"Traditional Chinese (BIG5)") + "\n");
			ob->directly_receive("Please input GB/BIG5 to change charset or directly login user.\n");

			tell(ob, "�z���Ův�^��W�r�G", CLIENTIGNORE);
			input_to( (: logon_handle, INPUT_ID, ob :) );

			return;
		}
		
	/* ��J ID */
	case INPUT_ID:
		{	
			int level_num;
			
			if( !arg || arg == "" )
				return logon_handle(INITIALIZE, ob);
			
			arg = remove_ansi(remove_fringe_blanks(lower_case(arg)));
			
			if( arg == "gb" )
			{
				set("encode/gb", 1, ob);
				return this_object()->logon(ob);
			}
			else if( arg == "big5" )
			{
				delete("encode/gb", ob);
				return this_object()->logon(ob);
			}

			if( file_size(user_data(arg)) <= 0 )
			{
				tell(ob, "���C���S�� "+arg+" �o��Ův�C\n", CONNECTFAILED);
				return logon_handle(INITIALIZE, ob);
			}
			
			level_num = SECURE_D->level_num(arg);
			
			if( level_num < GUEST )
			{
				tell(ob, HIG"�u���Ův�~��q�s���� "+WIZ_PORT+" �n�J�A���a�бq "+PPL_PORT+" �n�J�C\n"NOR, CONNECTFAILED);
				return logon_handle(INITIALIZE, ob);
			}
			
			if( level_num < LOGIN_D->query_wiz_lock() )
			{
				tell(ob, HIY"�t�Χ�s���@���A�ثe�Ȥ��\�v���b "+SECURE_D->level_num_to_level_name(LOGIN_D->query_wiz_lock())+" �H�W���ϥΪ̳s�u�C\n"NOR, CONNECTFAILED);
				return logon_handle(INITIALIZE, ob);
			}
			
			if( LOGIN_D->is_repeat_login(arg) )
			{
				tell(ob, "��p�z��J�� "+capitalize(arg)+" ���b�Q�n�����A�Фŭ��еn���C\n", CONNECTFAILED);
				return logon_handle(INITIALIZE, ob);
			}
			else
				set("id", arg, ob);

			tell(ob, "�п�J�K�X�G", CLIENTIGNORE);
			input_to( (: logon_handle, INPUT_PASSWORD, ob :), 1 );
			return;           
		}

	/* ��J�K�X */
	case INPUT_PASSWORD:
		{
			string password = PASSWORD_D->query_password(query("id", ob));

			if( !arg || crypt(arg, password) != password )
			{
				tell(ob, "�K�X��J���~�C\n", CONNECTFAILED);
					
				if( LOGIN_D->input_wrong_password(ob) >= 3 )
				{
					tell(ob, "�A�w�g��J���~�K�X�T���A�Э��s�n�J�A�աC\n", CONNECTFAILED);
					destruct(ob); return;
				}
				
				tell(ob, "�п�J���T�K�X�G", CLIENTIGNORE);
				input_to( (: logon_handle, INPUT_PASSWORD, ob :), 1 ); return;
			}

			// �Y�b�C�������ۦP ID �ϥΪ̪���
			if( find_player(query("id", ob)) )
			{
				set("player", find_player(query("id", ob)), ob);
				
				// �Y�����ʪ���, �h�߰ݬO�_���N
				if( interactive(query("player", ob)) )
				{
					tell(ob, "�A�Q���N�ثe�b�C�����ۦP���H����(Y/N)�H\n", CMDMSG);
					input_to( (: logon_handle, RECONNECT, ob :) );return;
				}
				// �������s�s�u���N����
				else
					logon_handle(RECONNECT, ob, "y");return;
			}

			/* �K�X�ˬd���T, �i�J�C�� */
			return logon_handle(OLD_PLAYER, ob);
		}

	/* �ª��a�n�� */
	case OLD_PLAYER:
		{
			object user_ob = load_user(query("id", ob));
			
			if( !objectp(user_ob) )
			{
				object login_ob = new(LOGIN_OB);
				
				tell(ob, "�L�k���J "+capitalize(query("id", ob))+" ���x�s�ɡA���p���Ův�B�z�C\n", CONNECTFAILED);
				exec(login_ob, ob);
				destruct(ob);
				logon_handle(INITIALIZE, login_ob);return;
			}

			set_temp("login/ident", query("ident", ob), user_ob);
			set_temp("login/remoteport", query("remote_port", ob), user_ob);
			set_temp("login/terminaltype", query("terminaltype", ob), user_ob);
			set_temp("windowsize", query("windowsize", ob) || DEFAULT_WINDOW_SIZE, user_ob);

			if( query("encode/gb", ob) )
				set("encode/gb", 1, user_ob);
			else
				delete("encode/gb", user_ob);

			// �� LOGIN_OB ������ USER_OB
			exec( user_ob , ob );
			destruct(ob);

			return logon_handle(ENTER_GAME, user_ob);
		}

	/* ���s�s�u */
	case RECONNECT:
		{
			object reconnect_ob;
			
			if( !arg || remove_ansi(remove_fringe_blanks(lower_case(arg))) != "y" )
			{
				tell(ob, "�z�M�w�����N�C�����ۦP���H���C\n", CONNECTFAILED);
				ob->reset_database();
				
				logon_handle(INITIALIZE, ob);return;
			}

			if( !objectp(reconnect_ob = query("player", ob)) )
			{
				tell(ob, "�쥻�C�������H���w�g�����A�Э��s�n�J�C\n", CONNECTFAILED);
				ob->reset_database();
				logon_handle(INITIALIZE, ob);return;
			}
				
			if( !interactive(reconnect_ob) )
				reconnect_ob->reconnect();
			else
			{
				object temp = new(VOID_OB);
				
				tell(reconnect_ob, "�t�~�@��ϥΪ̦� "+query_ip_name(ob)+" �s�u���N�F�A�C\n", ETCMSG);
				
				/* ���N�쥻�����a�ഫ�ܤ@�ӼȮ� USER_OB �A�N���R�� */
				exec(temp, reconnect_ob);
				destruct(temp);
			}

			set_temp("reconnect", 1, reconnect_ob);
			set_temp("login/ident", query("ident", ob), reconnect_ob);
			set_temp("login/remoteport", query("remote_port", ob), reconnect_ob);
			set_temp("windowsize", query("windowsize", ob) || DEFAULT_WINDOW_SIZE, reconnect_ob);
			set_temp("login/terminaltype", query("terminaltype", ob), reconnect_ob);

			if( query("encode/gb", ob) )
				set("encode/gb", 1, reconnect_ob);
			else
				delete("encode/gb", reconnect_ob);

			/* ���N�� user_ob */
			exec( reconnect_ob , ob );
			destruct(ob);

			return logon_handle(ENTER_GAME, reconnect_ob);
		}

	/* �i�J�C�� */
	case ENTER_GAME:
		{	
			// �i�J�C��
			LOGIN_D->enter_game(ob);
			return;

		}/* ENTER_GAME */

	default: return logon_handle(INITIALIZE, ob);
	
	}/* END SWITCH */
}

void wait_for_login(object login_ob, string arg)
{
	tell(login_ob, "���J���A�еy�� ...\n", CMDMSG);
	input_to( (: wait_for_login, login_ob :) );
}
nomask void logon(object login_ob)
{
	/* ���J�t�� */
	if( !SYSTEM_D->valid_login() )
	{
		tell(login_ob, HIY + MUD_FULL_NAME + "���J���A�еy�� ...\n"NOR, CMDMSG);
		SYSTEM_D->add_loginuser( login_ob );
		input_to( (: wait_for_login, login_ob :) );
		return;
	}

	if( !LOGIN_D->check_login_attacker(query_ip_number(login_ob)) )
	{
		login_ob->directly_receive(HIR"\n�ܩ�p�A�o�� IP["+query_ip_number(login_ob)+"] ���n�J�ʧ@�ӹL�W�c�A�еy�ԦA�աC\n"NOR);
		CHANNEL_D->channel_broadcast("sys", "�n�J�G"+query_ip_number(login_ob)+" ���n�J�ʧ@�ӹL�W�c");
		destruct(login_ob);
		return;
	}

	LOGIN_D->show_login_message(login_ob);
	logon_handle(INITIALIZE, login_ob);
}
int remove()
{
	foreach(object user in users())
	{
		if( !user->is_login_ob() ) continue;
		tell(user, HIR"\n�ܩ�p�A�n���t�Χ�s�A�·бz�A���s�n���C\n"NOR, CONNECTFAILED);
		flush_messages(user);
		destruct(user);
	}
}

string query_name()
{
	return "�Ův�n�J�t��(WIZ_LOGIN_D)";
}
