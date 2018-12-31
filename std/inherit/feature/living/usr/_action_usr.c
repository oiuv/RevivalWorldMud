/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _action_usr.c
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
#include <ansi.h>

private nosave mapping actions = allocate_mapping(0);

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
	
private nomask void do_multi_actions(string verb, string arg, mapping fit_actions, object *action_obs, string input)
{
	int i = to_int(input);
	
	if( i < 1 ) 
		i = 1;
	
	if( i > sizeof(fit_actions) )
		i = sizeof(fit_actions);
	
	i--;

	if( functionp(fit_actions[action_obs[i]]) & FP_OWNER_DESTED )
		return tell(this_object(), verb+" ���O�禡���о֦��̤w�Q�R���A�гq���Ův�B�z�C\n"); 

	evaluate(fit_actions[action_obs[i]], this_object(), arg, action_obs[i]);
	
	this_object()->finish_input();
}

private nomask void do_action(mapping fit_actions, string verb, string arg)
{
	if( sizeof(fit_actions) == 1 )
	{
		function verb_fp = values(fit_actions)[0];
			
		if( functionp(verb_fp) & FP_OWNER_DESTED )
			return tell(this_object(), verb+" �����O�禡���о֦��̤w�Q�R���A�гq���Ův�B�z�C\n"); 

		evaluate(verb_fp, this_object(), arg, keys(fit_actions)[0]);
	}
	else
	{
		int i;
		object *action_obs = allocate(0);
		string msg = "�����O�@ "+sizeof(fit_actions)+" �ؿ�ܡA�п��"+pnoun(2, this_object())+"�n���� "+verb+" ���O���ؼСG\n";
			
		foreach( object ob , function func in fit_actions )
		{
			msg += HIY+(++i)+". "NOR+(ob->short(1)||query("short", ob)||file_name(ob))+"\n";
			action_obs += ({ ob });
		}

		msg += "�п�J���X�G";
			
		tell(this_object(), msg);

		input_to( (: do_multi_actions, verb, arg, fit_actions, action_obs :) );
	}
}
