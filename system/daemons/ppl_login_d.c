/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ppl_login_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2000-12-29
 * Note   : ���a�n�����F
 * Update :
 *  o 2001-02-09 Clode �R�� port ���k, ��i�@�ǵ{���g�k
 *  o 2001-07-20 Clode ���g repeat login id ���ˬd
 *
 -----------------------------------------
 */

#include <ip.h>
#include <ansi.h>
#include <login.h>
#include <daemon.h>
#include <object.h>
#include <secure.h>
#include <gender.h>
#include <mudlib.h>
#include <message.h>
#include <ip.h>

#define INITIALIZE		1
#define INPUT_ID		2
#define INPUT_PASSWORD		3
#define INPUT_NEW_ID		4
#define INPUT_NEW_NAME		5
#define INPUT_NEW_PASSWORD	6
#define CONFIRM_PASSWORD	7
#define INPUT_EMAIL		8
#define INPUT_GENDER		9
#define RECONNECT		10
#define NEW_PLAYER		11
#define OLD_PLAYER		12
#define ENTER_GAME		13
#define FPASS_ID		14
#define FPASS_EMAIL		15

mapping newchar = allocate_mapping(0);
private nosave mapping ip_time = allocate_mapping(0);
private mapping login_ip_num = allocate_mapping(0);
string *reserved_id = ({  });

nomask void reset_newchar()
{
	newchar = allocate_mapping(0);	
}

nomask void reset_cmdlimit()
{
	login_ip_num = allocate_mapping(0);
}

mapping query_loginip()
{
	return login_ip_num;
}

// ��F�W�� 30 �N�]�w banip 24 �p��
private void set_user_cmdlimit( string ip )
{
	// �A check �@��
	if( login_ip_num[ip] >= 30 )
	{	
		if( !(IP_D->query_ip(ip)) )
		{
			IP_D->set_ip( this_object(), ip, BAN, 24*3600);
			IP_D->set_ip_note( this_object(), ip, "�P ip �@�� Login �W�L 30 ��");
		}
	}
}

private nomask varargs void logon_handle(int state, object ob, string arg)
{
	ob->reset_time_out(); // ���]Time_out�ɶ�

	if( arg && query("encode/gb", ob) ) arg = G2B(arg);

	switch(state)
	{
		/* �ҩl Login */
	case INITIALIZE:
		{
			string ip = query_ip_number(ob), ban;

			foreach(string limitedip, mapping data in IP_D->query_ipdata())
			if( regexp(ip, limitedip) && data["status"] == BAN )
			{
				ob->directly_receive(HIR"\nĵ�i�A�z�ҨϥΪ� IP("+limitedip+") ��m�ثe�Q�t�ΦC���T��n�J�C\n"NOR);
				CHANNEL_D->channel_broadcast("nch", ip+" ���ϥΪ̹��ըϥγQ�T� IP �n�J");
				destruct(ob);
				return;
			}

			if(sscanf( query_ip_name(ob), "%*s.%s.edu.tw", ban )==2)
			{
				if( ban == "cyut" )
				{
					tell( ob, sprintf( HIR"\nĵ�i�A�z�ҨϥΪ� IP(%s) ��m�ثe�Q�t�ΦC���T��n�J�C\n"NOR, query_ip_number(ob)));
					destruct(ob);
					return;
				}	
			}

			if(sscanf( query_ip_name(ob), "%*s.%*s.%s.edu.tw", ban )==3)
			{
				if( ban == "cyut" )
				{
					tell( ob, HIR"\nĵ�i�A�z�ҨϥΪ� IP(%s) ��m�ثe�Q�t�ΦC���T��n�J�C\n"NOR, query_ip_number(ob));
					destruct(ob);
					return;
				}	
			}			


			if( time() - ip_time[ip] < 60 )
			{
				tell(ob, sprintf( HIY"���s�u IP �Z���e���W�u�ɶ��ȹj %d ��, �� %d ���A�աC\n"NOR, time() - ip_time[ip], 60 + ip_time[ip] - time()));
				CHANNEL_D->channel_broadcast("nch", ip +" ���ϥΪ����W�� login �ɶ��L�u, �ڵ��n�J !");
				destruct(ob);
				return;
			} 

			ob->directly_receive("Current charset is " + (query("encode/gb", ob) ? "Simplified Chinese (GB)":"Traditional Chinese (BIG5)") + "\n");
			ob->directly_receive("Please input GB/BIG5 to change charset or directly login user.\n");

			tell(ob, sprintf(
				"�гy�s���C������п�J 'new'�C%s\n"
				"�z���^��W�r�G",
				(LOGIN_D->query_wiz_lock() < GUEST ? HIC"(���\���a�n�J)"NOR:HIR"(�T��a�n�J)"NOR)));

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

			if( arg == "new" && LOGIN_D->query_wiz_lock() < GUEST )
			{
				string ip = query_ip_number(ob);

				if( ip == "75.84.17.242" )
				{
					tell(ob, "��p�z�ҨϥΪ� ip �A�L�k�A���U�s����C\n");
					return logon_handle(INITIALIZE, ob);
				}
				
				if( query_ip_name(ob)[<13..] == "csupomona.edu" ) 
				{
					tell(ob, "��p�z�ҨϥΪ� ip �A�L�k�A���U�s����C\n");
					return logon_handle(INITIALIZE, ob);
				}
				
				if( ++newchar[ip] >= 5 )
				{
					CHANNEL_D->channel_broadcast("sys", "�n�J�G"+ip+" ���ϥΪ̥���s����w���U�W�L "+5+" ��A�L�k�A���U�s����C");
					tell(ob, "��p�z����s����w���U�W�L "+5+" ��A�L�k�A���U�s����C\n");
					return logon_handle(INITIALIZE, ob);
				}

				tell(ob, "�п�J�z�Q�ϥΪ��^��ID�G");
				input_to( (: logon_handle, INPUT_NEW_ID, ob :) ); return;
			}
			if( arg == "pass" )
			{
				tell(ob, "�п�J�z���^��ID�G");
				input_to( (: logon_handle, FPASS_ID, ob :) ); return;
			}

			level_num = SECURE_D->level_num(arg);

			if( sizeof(users()) > MAX_USERS && level_num < GUEST )
			{
				tell(ob, "��p���C�����a�W�u�H�Ƥ��̤j�W���� "+MAX_USERS+" ��A�Э@�ߵ��ԡC\n");
				return logon_handle(INITIALIZE, ob);
			}

			if( level_num < LOGIN_D->query_wiz_lock() )
			{
				tell(ob, HIY"�t�Χ�s���@���A�ثe�Ȥ��\�v���b "+SECURE_D->level_num_to_level_name(LOGIN_D->query_wiz_lock())+" �H�W���ϥΪ̳s�u�C\n"NOR);
				return logon_handle(INITIALIZE, ob);
			}

			if( file_size(user_data(arg)) <= 0 )
			{
				tell(ob, "���C���S�� "+arg+" �o�쪱�a�C\n");
				return logon_handle(INITIALIZE, ob);
			}

			if( LOGIN_D->is_repeat_login(arg) )
			{
				tell(ob, "��p�z��J�� "+capitalize(arg)+" ���b�Q�n�����A�Фŭ��еn���C\n");
				return logon_handle(INITIALIZE, ob);
			}
			else
				set("id", arg, ob);

			if( level_num > PLAYER )
				tell(ob, "�z���ϥΪ��v���� "+SECURE_D->level_num_to_level_name(level_num)+"�A�����z�����ѳs���� "+WIZ_PORT+" �W�u�~��֦��Ův�v���C\n");

			tell(ob, "�п�J�K�X�G");

			input_to( (: logon_handle, INPUT_PASSWORD, ob :), 1 );
			return;           
		}
	case FPASS_ID:
		{
			tell(ob, "�ӪA�ȩ|���ҥΡC");
			destruct(ob);
			return;

			if( file_size(user_data(arg)) <= 0 )
			{
				tell(ob, "���C���S�� "+arg+" �o�쪱�a�C\n");
				return logon_handle(INITIALIZE, ob);
			}
			set("id", arg, ob);
			tell(ob, "�п�J�Ө��⪺�H�c��m�G");
			input_to( (: logon_handle, FPASS_EMAIL, ob :) ); return;
		}
	case FPASS_EMAIL:
		{
			object user_ob = load_user(query("id", ob));
			if( query("email", user_ob) == arg )
			{
				tell(ob, "�t�Τw�N�s�K�X Email �ܱz���H�c�C\n");
				SMTP_D->mail(arg, "�ӪA�ȩ|���ҥ�", "���ͪ��@�� - "+query("id", ob)+" ����K�X");
			}
			else tell(ob, "�z��J����ƸӨ��⤣�šC\n");
			destruct(user_ob);
			destruct(ob);
			return;
		}
		/* ��J�K�X */
	case INPUT_PASSWORD:
		{
			string password = PASSWORD_D->query_password(query("id", ob));

			if( !arg || crypt(arg, password) != password )
			{
				tell(ob, "�K�X��J���~�C\n");

				if( LOGIN_D->input_wrong_password(ob) >= 3 )
				{
					tell(ob, "�A�w�g��J���~�K�X�T���A�Э��s�n�J�A�աC\n");
					destruct(ob); return;
				}

				tell(ob, "�п�J���T�K�X�G");
				input_to( (: logon_handle, INPUT_PASSWORD, ob :), 1 ); return;
			}

			// �Y�b�C�������ۦP ID �ϥΪ̪���
			if( find_player(query("id", ob)) )
			{
				object player = find_player(query("id", ob));
				set("player", player , ob);

				// �Y�����ʪ���, �h�߰ݬO�_���N
				if( interactive(player) )
				{
					tell(ob, "�A�Q���N�ثe�b�C�����ۦP���H����(Y/N)�H\n");
					input_to( (: logon_handle, RECONNECT, ob :) );
					return;
				}

				logon_handle(RECONNECT, ob, "y");return;
			}

			/* �K�X�ˬd���T, �i�J�C�� */
			return logon_handle(OLD_PLAYER, ob);
		}

		/* ��J�s�� ID */
	case INPUT_NEW_ID:
		{
			int len;
			string ban;

			if( sscanf( query_ip_name(ob), "%*s-%*s-%*s-%*s.dynamic.%s.net", ban ) ==5 ) 
			{
				if( ban == "hinet" )
				{
					tell( ob, sprintf(HIR"\nĵ�i�A�z�ҨϥΪ� IP(%s) ��m�ثe�Q�t�ΦC���T��n���s����C\n"NOR, query_ip_number(ob)));
					destruct(ob);
					return;
				}			
			}

			if( sscanf( query_ip_name(ob), "%*s-%*s-%*s-%*s..HINET-IP.%s.net", ban ) ==5 ) 
			{
				if( ban == "hinet" )
				{
					tell( ob, sprintf(HIR"\nĵ�i�A�z�ҨϥΪ� IP(%s) ��m�ثe�Q�t�ΦC���T��n���s����C\n"NOR, query_ip_number(ob)));
					destruct(ob);
					return;
				}			
			}

			if(sscanf( query_ip_name(ob), "%*s.%s.edu.tw", ban )==2)
			{
				if( ban = "cyut" )
				{
					tell( ob, HIR"\nĵ�i�A�z�ҨϥΪ� IP(%s) ��m�ثe�Q�t�ΦC���T��n���s����C\n"NOR, query_ip_number(ob));
					destruct(ob);
					return;
				}	
			}
			
			if(sscanf( query_ip_name(ob), "%*s.%*s.%s.edu", ban )== 3)
			{
				if( ban = "csupomona" )
				{
					tell( ob, HIR"\nĵ�i�A�z�ҨϥΪ� IP(%s) ��m�ثe�Q�t�ΦC���T��n���s����C\n"NOR, query_ip_number(ob));
					destruct(ob);
					return;
				}	
			}
			
			if( !arg || arg == "" )
			{
				tell(ob, "�п�J�z�Q�ϥΪ��^��N��(ID)�G");
				input_to( (: logon_handle, INPUT_NEW_ID, ob :) ); return;
			}

			len = strlen((arg = lower_case(arg)));

			if( (len = strlen(arg)) < 3 || len > 12 )
			{
				tell(ob, "�藍�_�A�A���^��W�r�u���� 3 �� 12 �Ӧr�C\n�п�J�z�Q�ϥΪ��^��N��(ID)�G");
				input_to( (: logon_handle, INPUT_NEW_ID, ob :) );return;
			}

			foreach( string a in arg )
			if( (int) a<'a' || (int) a>'z' ) 
			{
				tell(ob, "�藍�_�A�A���^��W�r�u��έ^��r���C\n�п�J�z�Q�ϥΪ��^��N��(ID)�G");
				input_to( (: logon_handle, INPUT_NEW_ID, ob :) );return;
			}

			if( member_array(arg, reserved_id) != -1 )
			{
				tell(ob, "�� ID ���t�ΫO�d�r�A�д��t�@�ӦW�r�C\n�п�J�z�Q�ϥΪ��^��N��(ID)�G");
				input_to( (: logon_handle, INPUT_NEW_ID, ob :) );return;
			}

			if( file_size(user_data(arg)) > 0 )
			{
				tell(ob, "���C���w�����a�ϥ� "+capitalize(arg)+"�A�д��t�@�ӦW�r�C\n�п�J�z�Q�ϥΪ��^��N��(ID)�G");
				input_to( (: logon_handle, INPUT_NEW_ID, ob :) );return;
			}

			if( LOGIN_D->is_repeat_login(arg) )
			{
				tell(ob, "��p�z��ID:"+capitalize(arg)+"���b�n�J���A�Фŭ��еn�J�C\n");
				return logon_handle(INITIALIZE, ob);
			}
			else
				set("id", arg, ob);


			tell(ob, "�п�J�z���W��(Name)�G");
			input_to( (: logon_handle, INPUT_NEW_NAME, ob :) );
			return;
		}

		/* ��J�s�W�� */
	case INPUT_NEW_NAME:
		{
			int len;
			if( !arg || (len=strlen(arg)) < 2 || len > 12 )
			{
				tell(ob, "�z������J 1 �� 12 �Ӧr���G");
				input_to( (: logon_handle, INPUT_NEW_NAME, ob :) ); return;
			}

			if( strsrch(arg, "�@") != -1 || strsrch(arg, " ") != -1 )
			{                	
				tell(ob, "�ФťΪŮ�ӷ��W�١C\n�п�J�W�١G");
				input_to( (: logon_handle, INPUT_NEW_NAME, ob :) ); return;
			}

			foreach( int a in arg )
			{
				if( a <= ' ' )
				{
					tell(ob, "�z���W�٤���α���r���C\n�п�J�W��(Name)�G");
					input_to( (: logon_handle, INPUT_NEW_NAME, ob :) );return;
				}
			}

			set("name", arg, ob);

			tell(ob, "�w��"+arg+"("+capitalize(query("id", ob))+")�i�J"MUD_FULL_NAME"�C\n");
			tell(ob, "�п�J�z�Q�ϥΪ��K�X�G");
			input_to( (: logon_handle, INPUT_NEW_PASSWORD, ob :), 1 );
			return;	
		}

		/* ��J�s���K�X */        
	case INPUT_NEW_PASSWORD:
		{
			if( !arg || strlen(arg) < 4 )
			{
				tell(ob, "�п�J�h��|�Ӧr�����K�X�G");
				input_to( (: logon_handle, INPUT_NEW_PASSWORD, ob :), 1 );return;
			}

			set("password", crypt(arg, 0), ob);
			tell(ob, "�ЦA��J�@���z���K�X�R");
			input_to( (: logon_handle, CONFIRM_PASSWORD, ob :), 1 );
			return;
		}

		/* �A���T�{�K�X */
	case CONFIRM_PASSWORD:
		{
			if( !arg || crypt( arg, query("password", ob) ) != query("password", ob) )
			{
				tell(ob, "�T�{�K�X���ѡC\n�Э��s��J�z�Q�ϥΪ��K�X�G");
				input_to( (: logon_handle, INPUT_NEW_PASSWORD, ob :), 1 );return;
			}

			PASSWORD_D->set_password(query("id", ob), query("password", ob));

			tell(ob, "�z���H�c��}�N�@������������ƥ��ɮת��̾ڡA�Ϊ����� Enter ����J�C\n�z���H�c��}�G");
			input_to( (: logon_handle, INPUT_EMAIL, ob :) ); return;
		}

		/* �]�wEMAIL */
	case INPUT_EMAIL:
		{
			if( !arg || arg=="" )
				tell(ob, "�z���@���N�~�ॢ����ɡA�V�Ův�ӽгƥ��ɮת��v�Q�C\n");
			else if( sscanf(arg,"%*s@%*s.%*s") != 3 )
			{
				tell(ob, "���H�c��}�榡�����D�C\n�Э��s��J�z���H�c��}�G");
				input_to( (: logon_handle, INPUT_EMAIL, ob :) ); return;
			}
			else set("email", arg, ob);

			tell(ob, "�п�ܱz���ʧO�A�k(M)�B�k(F)�R");
			input_to( (: logon_handle, INPUT_GENDER, ob :) ); return;
		}
		/* �]�w�ʧO */
	case INPUT_GENDER:
		{
			if( arg == "m" || arg == "M" ) 
				set("gender", MALE_GENDER, ob);
			else if( arg == "f" || arg == "F" ) 
				set("gender", FEMALE_GENDER, ob);
			else
			{
				tell(ob, "�п�J M �άO F�C\n�п�ܱz���ʧO�A�k(M)�B�k(F)�R");
				input_to( (: logon_handle, INPUT_GENDER, ob :) );return;
			}

			return logon_handle(NEW_PLAYER, ob);
		}	

		/* �s���a�n�� */
	case NEW_PLAYER:
		{
			object user_ob = load_object(user_file_name(query("id", ob)));

			/*�ഫ��ƨ��x�s*/
			user_ob->set_idname(query("id", ob), query("name", ob));

			set("email", query("email", ob), user_ob);
			set_temp("login/ident", query("ident", ob), user_ob);
			set_temp("login/remoteport", query("remote_port", ob), user_ob);
			set_temp("login/terminaltype", query("terminaltype", ob), user_ob);
			set_temp("windowsize", query("windowsize", ob) || DEFAULT_WINDOW_SIZE, user_ob);

			if( query("encode/gb", ob) )
				set("encode/gb", 1, user_ob);
			else
				delete("encode/gb", user_ob);

			set_temp("new_player", 1, user_ob);

			exec( user_ob, ob );

			CHAR_D->create_char(user_ob, query("gender", ob));

			user_ob->save();

			destruct(ob);

			return logon_handle(ENTER_GAME, user_ob);
		}

		/* �ª��a�n�� */
	case OLD_PLAYER:
		{
			object user_ob = load_user(query("id", ob));

			if( !objectp(user_ob) )
			{
				object login_ob = new(LOGIN_OB);

				tell(ob, "�L�k���J "+capitalize(query("id", ob))+" ���x�s�ɡA���p���Ův�B�z�C\n");
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
				tell(ob, "�z�M�w�����N�C�����ۦP���H���C\n");
				ob->reset_database();

				logon_handle(INITIALIZE, ob);return;
			}

			if( !objectp(reconnect_ob = query("player", ob)) )
			{
				tell(ob, "�쥻�C�������H���w�g�����A�Э��s�n�J�C\n");
				ob->reset_database();
				logon_handle(INITIALIZE, ob);return;
			}

			if( !interactive(reconnect_ob) )
				reconnect_ob->reconnect();
			else
			{
				object temp = new(VOID_OB);

				tell(reconnect_ob, "�t�~�@��ϥΪ̦� "+query_ip_name(ob)+" �s�u���N�F�A�C\n");

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

			delete_temp("new_player", reconnect_ob);

			return logon_handle(ENTER_GAME, reconnect_ob);
		}

		/* �i�J�C�� */
	case ENTER_GAME:
		{
			// �O���� ip �W�u�ɶ�
			string ip = query_ip_number(ob);
			ip_time[ip] = time();
			
			// �O���� ip �W�u����
			if( !login_ip_num[ip] )
				login_ip_num[ip] = 1;
			else
			if( login_ip_num[ip] >= 30 )
			{
				login_ip_num[ip] = 30;
				set_user_cmdlimit(ip);
			} else
				login_ip_num[ip] = ( login_ip_num[ip] + 1 );
			
			// �i�J�C��
			LOGIN_D->enter_game(ob);
			return;

		}/* ENTER_GAME */

	default: return logon_handle(INITIALIZE, ob);

	}/* END SWITCH */

}

void wait_for_login(object login_ob, string arg)
{
	tell(login_ob, "���J���A�еy�� ...\n");
	input_to( (: wait_for_login, login_ob :) );
}

nomask void logon(object login_ob)
{
	/* ���J�t�� */
	if( !SYSTEM_D->valid_login() )
	{
		tell(login_ob, HIY + MUD_FULL_NAME + "���J���A�еy�� ...\n"NOR);
		SYSTEM_D->add_loginuser( login_ob );
		input_to((: wait_for_login, login_ob :));
		return;
	}

	LOGIN_D->show_login_message(login_ob);

	if( !LOGIN_D->check_login_attacker(query_ip_number(login_ob)) )
	{
		login_ob->directly_receive(HIR"\n�ܩ�p�A�o�� IP["+query_ip_number(login_ob)+"] ���n�J�ʧ@�ӹL�W�c�A�еy�ԦA�աC\n"NOR);
		//CHANNEL_D->channel_broadcast("sys", "�n�J�G"+query_ip_number(login_ob)+" ���n�J�ʧ@�ӹL�W�c�A�j�����u");
		//destruct(login_ob);
		logon_handle(INITIALIZE, login_ob);
		return;
	}

	logon_handle(INITIALIZE, login_ob);
}

int remove()
{
	foreach(object user in users())
	{
		if( !user->is_login_ob() ) continue;
		tell(user, HIR"\n�ܩ�p�A�n���t�Χ�s�A�·бz�A���s�n���C\n"NOR);
		flush_messages(user);
		destruct(user);
	}
}

string query_name()
{
	return "���a�n�J�t��(PPL_LOGIN_D)";
}
