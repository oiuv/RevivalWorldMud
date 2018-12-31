/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : logout_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-7-29
 * Note   : �n�X���F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#define LOG			"daemon/connect"

#include <ansi.h>
#include <daemon.h>
#include <message.h>

void net_dead(object ob)
{
	string network_short = NETWORK_D->query_network_short(ob);

	msg("$ME���G�P�{��ͬ����s�u�X�F�t�� ... �_�u�F�C\n", ob, 0, 1, 0, ({ ob }));

	CHANNEL_D->channel_broadcast("sys", "�n�X�G"+ob->query_idname()+WHT"��"+network_short+"�_�u�A�˼Ƥ��������u�C", 1);	

	log_file(LOG, ob->query_idname()+" disconnect "+network_short+NOR+"\n");
	// CHANNEL_D->remove_user(ob);
}

private void save_user_info(object ob)
{
	mapping current_login = copy(query_temp("login", ob));
	
	if( !mapp(current_login) )
		return;

	addn("total_online_time", time() - query("last_on/time", ob), ob );

	current_login["time"] = time();
	
	set("last_login", current_login, ob); 
	
	// �����̫�@���b�u�W���ɶ�, �ھڦ���ƥ� CLEAN_D �B�z�b���R��
	set("last_on/time", time(), ob);

	if( environment(ob) )
		set("quit_place", copy(query_temp("location", ob)) || base_name(environment(ob)), ob);

	ob->save();
}

varargs void quit(object ob, string arg)
{
	object env = environment(ob);
	string network_short = NETWORK_D->query_network_short(ob);
	if( !userp(ob) && !query_temp("net_dead", ob) ) return;
	
	save_user_info(ob);

	/* ��s TOP ��� */
	TOP_D->calculate_top(ob);

	CHANNEL_D->remove_user(ob);

	if( query_temp("net_dead", ob) )
	{
		msg("$ME�_�u�W�L�������A�Q��X�C���F�C\n", ob, 0, 1);
		CHANNEL_D->channel_broadcast("sys", "�n�X�G"+ob->query_idname()+WHT"��"+network_short+"�_�u�L�[���}�C���F�C", 1);
	}
	else 
	{
		tell(ob, "\n"+pnoun(2, ob)+"�����@�s�u�F"+CHINESE_D->chinese_period(time() - query_temp("login/time", ob))+"�C\n");

		tell(ob, @POEM

----------

    ���A�y���F�A�ڤ��@���Ŷ����q�̡A
    ���@�O�ɶ����ۤH�C
    �M�ӡA�ڤS�O�t�z���C�l�A
    �a�y�A���ݯd�ڡC
    �o�g�a�ڤ@��ӡA
    �N�K�����h�C
				      ��G�T��
			---------------------- 
			         [1;30mRevival World[0m

POEM);

		if(!arg) 
			msg("$ME���}�C���F�C\n", ob, 0, 1);
		else
			msg("$ME���}�C���F�A�uť�컷��ӲӶǨӤ@�n�G" + arg + "�C\n", ob, 0, 1);
			
		CHANNEL_D->channel_broadcast("sys", "�n�X�G"+ob->query_idname()+WHT"��"+network_short+"���}�C���C", 1);
	}
		
	MAP_D->remove_player_icon(ob, query_temp("location", ob));

	log_file(LOG, ob->query_idname(1)+" quit "+network_short+NOR+"\n");	
		
	flush_messages(ob);
	
	if( env )
		env->leave(ob);

	destruct(ob);
}
string query_name()
{
	return "�n�X�t��(LOGOUT_D)";
}
