/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : clean_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-06-05
 * Note   : �M�����F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>

#define ADAY	(60*60*24)
#define LOG	"daemon/clean"

#define CLEAN_NEWBIE_ESTATE_DAYS	7	// 7 �ѥ��W�u�B�`�W�u�ɶ����� 2 �p�ɤ��s�⨤��尣��Цa��
#define CLEAN_NEWBIE_CHAR_DAYS		180	// 180 �ѥ��W�u�B�`�W�u�ɶ����� 2 �p�ɤ��s�⨤��尣�䨤��
#define CLEAN_ESTATE_DAYS		365	// 365 �ѥ��W�u���Ѫ��a�R����Цa��
#define CLEAN_CHAR_DAYS			3650	// 3650 �ѥ��W�u���Ѫ��a�R���䨤��


void clean_user()
{
	object user;
	int total, time = time(), overtime;
	string msg, dir, id;
        string *root_dir = filter_array(get_dir("/data/user/"), (: strlen($1) == 1 :));

	CHANNEL_D->channel_broadcast("news", "�t�ΰ���b���M�z�@�~�A�еy�ԡC");

        foreach( dir in root_dir )
        {
        	reset_eval_cost();

        	foreach( id in get_dir("/data/user/"+dir+"/") )
        	{
        		total++;
        		
        		if( SECURE_D->is_wizard(id) ) continue;

			if( find_player(id) && environment(find_player(id)) ) continue;
			
        		user = load_user(id);

			if( !objectp(user) )
			{
				CHANNEL_D->channel_broadcast("sys", "�L�k���J�ϥΪ̪��� "+id);
				log_file(LOG, "�L�k���J�ϥΪ̪��� "+id);
				continue;
			}
			
			overtime = (time - query("last_on/time", user))/ADAY;

			// �`�W�u�ɶ����� 2 �p�ɤ��s�⨤��
			if( query("total_online_time", user) < 7200 )
			{
				if( overtime > CLEAN_NEWBIE_ESTATE_DAYS && sizeof(ESTATE_D->query_player_estate(id)) )
				{
					ESTATE_D->remove_estate(id);
					msg = capitalize(id)+" �`�W�u�ɶ����� 2 �p�ɥB�W�L "+CLEAN_NEWBIE_ESTATE_DAYS+" �ѥ��W�u("+overtime+" ��)�A�^����Цa��";
					
					LOGIN_D->add_login_message(id, msg);
					CHANNEL_D->channel_broadcast("news", msg);
					log_file(LOG, msg);
				}
				else if( overtime > CLEAN_NEWBIE_CHAR_DAYS )
				{
					set("password", PASSWORD_D->query_password(id), user);
					user->save();
					
					msg = capitalize(id)+" �`�W�u�ɶ����� 2 �p�ɥB�W�L "+CLEAN_NEWBIE_CHAR_DAYS+" �ѥ��W�u("+overtime+" ��)�A�R���䨤��";
					
					CHAR_D->destruct_char(id);
					CHANNEL_D->channel_broadcast("news", msg);
					log_file(LOG, msg);
				}
			}
			else
			{
				if( overtime > CLEAN_ESTATE_DAYS && sizeof(ESTATE_D->query_player_estate(id)) )
				{
					ESTATE_D->remove_estate(id);
					msg = capitalize(id)+" �W�L "+CLEAN_ESTATE_DAYS+" �ѥ��W�u("+overtime+" ��)�A�^����Цa��";
					
					LOGIN_D->add_login_message(id, msg);
					CHANNEL_D->channel_broadcast("news", msg);
					log_file(LOG, msg);
				}
				else if( overtime > CLEAN_CHAR_DAYS )
				{
					set("password", PASSWORD_D->query_password(id), user);
					user->save();
					
					msg = capitalize(id)+" �W�L "+CLEAN_CHAR_DAYS+" �ѥ��W�u("+overtime+" ��)�A�R���䨤��";
					CHAR_D->destruct_char(id);
					CHANNEL_D->channel_broadcast("news", msg);
					log_file(LOG, msg);
				}
			}

			if( objectp(user) && !userp(user) )
				destruct(user);
        	}
        }
        
        CHANNEL_D->channel_broadcast("news", "�b���`�ơG"+total+"�A�M�z�@�~�����C");
}

void create()
{
	

}

string query_name()
{
	return "�ɮײM���t��(CLEAN_D)";
}
