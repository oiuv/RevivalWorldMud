/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : crash.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */
#include <ansi.h>
#include <mudlib.h>
#include <daemon.h>

#define LOG		"system/crashes"

/* ��o�ͤF driver segmentation fault �� bus error �����~��, �N�I�s���禡 */
private void crash(string error, object command_giver, object current_object)
{
	/*
	log_file(LOG, MUD_ENGLISH_NAME + " crashed on: " + ctime(time()) +", error: " + error + "\n");
	log_file(LOG, "command_giver: " + (command_giver ? file_name(command_giver) : "none") + "\n");
	log_file(LOG, "current_object: " + (current_object ? file_name(current_object) : "none") + "\n");
	log_file(LOG, "call_stack(0): "+sprintf("%O\n", call_stack(0)));
	log_file(LOG, "call_stack(1): "+sprintf("%O\n", call_stack(1)));
	log_file(LOG, "call_stack(2): "+sprintf("%O\n", call_stack(2)));
	log_file(LOG, "call_stack(3): "+sprintf("%O\n", call_stack(3)));
	log_file(LOG, "this_player(): "+sprintf("%O\n", this_player()));
	log_file(LOG, "this_player(1): "+sprintf("%O\n", this_player(1)));
	log_file(LOG, "this_player_command: " + (this_player() ? sprintf("%O\n", query_temp("command", this_player())) : "none"));
	log_file(LOG, "previous_object(): "+sprintf("%O\n", previous_object()));
	*/
	// �x�s���a���
	foreach( object ob in users() )
	{
		reset_eval_cost();

		log_file(LOG, " last command :"+ob->query_id(1)+save_variable(query_temp("command", ob)));
		catch(flush_messages(ob)); // �N���e���T���ߨ�e�ܨϥΪ�
		catch(ob->save());
	}

	log_file(LOG, "���a��Ƴƥ�����");

	// �������D�n���
	MONEY_D->save();
	MAP_D->save();
	ESTATE_D->save();
	CITY_D->save_all();

  // �ϩҦ����󥿱`����
	foreach( object ob in objects() )
	{
		reset_eval_cost();
		catch(ob->remove());
	}

	log_file(LOG, "�t�θ�Ƴƥ�����");

	catch(shout(MUD_FULL_NAME+HIR"�t�εo�ͤF�e�ҥ��������~�A�n CRASH �F�A�t�θ�Ƴƥ�����!!\n"NOR));

}

/* mud �w�C�������L�{ */
private int slow_shutdown( int min )
{
	shout("system",MUD_FULL_NAME"�t�ΰO���餣���A�A�L "+min+" �����n CRASH �F!!\n");
	return 1;
}
