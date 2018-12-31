/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _command_npc.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-4
 * Note   : �q���ͪ����O�t��
 * Update :
 *  o 2001-07-08 Clode ��z action �{���X
 *
 * P.S Action �� fetch_variable �ݭ��g
 -----------------------------------------
 */

#include <daemon.h>
#include <message.h>
#include <function.h>

private nomask void initialize_actions();
private nomask void do_action(mapping fit_actions, string verb, string arg);
private void evaluate_command(function command_fp, string verb, string arg);

private void evaluate_command(function command_fp, string verb, string arg)
{
	if( functionp(command_fp) & FP_OWNER_DESTED )
		return tell(this_object(), "�u"+verb+"�v�����O�禡���о֦��̤w�Q�R���A�гq���Ův�B�z�C\n"); 
		
	evaluate(command_fp, this_object(), arg);
}

private nomask int process_command(string verb, string arg)
{
	function command_fp;
	mapping all_actions;
	object ob;
	object env = environment();
	object *env_action_ob;
	mapping inv_ob_actions;
	
	all_actions = this_object()->query_actions() || allocate_mapping(0);

	if( objectp(env) )
	{
		env_action_ob = present_objects(this_object());

		if( !env->is_maproom() )
			env_action_ob += ({ env });
		
		// �S��X�f���O²��
		if( !arg && query("exits/"+verb, env) )
		{
			arg = verb;
			verb = "go";
		}
	}

	if( !mapp(all_actions[verb]) )
		all_actions[verb] = allocate_mapping(0);

	/* �N�Ҧ� actions ��X�_�� */
	if( sizeof(env_action_ob) )
	foreach( ob in env_action_ob )
		if( !userp(ob) && mapp(inv_ob_actions = ob->query_actions()) && functionp(inv_ob_actions[verb]) )
			all_actions[verb] += ([ ob : inv_ob_actions[verb] ]);
	
	// ���� Action
	if( sizeof(all_actions[verb]) )
	{
		do_action(all_actions[verb], verb, arg);
		return 1;
	}
	// ����t�Ϋ��O
	else if( functionp(command_fp = COMMAND_D->find_npc_command(verb)) )
	{	
		evaluate_command(command_fp, verb, arg);
		return 1;
	}
	else 
		return 0;
}

void do_command(string cmd)
{
	string verb;
	string args;
	
	if( !cmd || !cmd[0] || query_temp("disable/cmd") ) return;
	
	if( sscanf(cmd, "%s %s", verb, args) != 2 )
		verb = cmd;
	
	set_this_player(this_object());

	if( !process_command(verb, args) )
	{
		process_command("say", "��p�A�ڤ��M���u"+cmd+"�v�ӫ�򰵡C");
	}
}