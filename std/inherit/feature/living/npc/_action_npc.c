/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _action_npc.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-8-20
 * Note   : RW action �B�z�覡:
 *	    1. ���Ҫ���P���Ҫ��󪺤��e���W�ҥ]�t�� action, �Ҧb�ϥΪ̤U�F���O�ɧY�ɷj�M
 *	    2. �ϥΪ̪��󪺤��e���W�ҥ]�t�� action, �h�O�b���e���i�J�����}�ϥΪ̪����, 
 *�@�@�@�@�@�@ �i�� add_action �P remove_action ���ʧ@.
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <daemon.h>
#include <function.h>
#include <message.h>

nosave mapping actions = allocate_mapping(0);

private nomask void initialize_actions()
{
	actions = allocate_mapping(0);
}

mapping query_actions()
{
	return copy(actions);
}

/*
  �ϥΩ����i�J this_object() ��, �[�J�����󤧩Ҧ� actions
  �άO�����[�J�Ӫ��󪺬Y�� action
*/
varargs nomask void add_action(object ob, mapping commands)
{
	mapping action;
	
	if( !objectp(ob) ) return;
	
	action = commands || ob->query_actions();
	
	if( !mapp(action) ) return;
		
	foreach( string verb, function fp in action )
	{
		if( !mapp(actions[verb]) )
			actions[verb] = ([ ob : fp ]);
		else 
			actions[verb] += ([ ob : fp ]);
	}
}

/* 
  �ϥΩ�������} this_object() ��, �R�������󤧩Ҧ� actions 
  �άO���������Ӫ��󪺬Y�� action
*/
varargs nomask void remove_action(object ob, mapping commands)
{
	mapping action;
	
	if( !objectp(ob) ) return;
	
	action = commands || ob->query_actions();
	
	if( !mapp(action) ) return;
	
	foreach( string verb, function fp in action )
	{
		if( !mapp(actions[verb]) ) continue;
		
		map_delete(actions[verb], ob);
		
		if( !sizeof(actions[verb]) ) map_delete(actions, verb);
	}
}

private nomask void do_action(mapping fit_actions, string verb, string arg)
{
	function verb_fp;
	
	// NPC ���ަp�󪽱�����Ĥ@�Ӫ�����O
	verb_fp = values(fit_actions)[0];
			
	if( functionp(verb_fp) & FP_OWNER_DESTED )
		error(verb+" �����O�禡���о֦��̤w�Q�R���A�гq���Ův�B�z�C\n");
				
	evaluate(verb_fp, this_object(), arg);
}
