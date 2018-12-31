/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : clone.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-4-11
 * Note   : clone ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <ansi.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
���O�榡�Gclone <����|�ɮ�>

�ƻs�@�Ӫ�����ɮסC
HELP;

private void command(object me, string arg)
{
	string amount, *parse;
	object obj;
	
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "���O�榡�Gclone <����|�ɮ�>\n");
		
	parse = single_quote_parse(arg);
	
	if( sizeof(parse) > 1 )
	{
		if( to_float(parse[0]) == 0. )
			arg = parse[0];
		else
		{
			amount = parse[0];
			arg = parse[1];
			
			if( count(amount, "<", 1) ) amount = "1";
		}
	}
	else
		arg = parse[0];
	
	if( objectp(obj = present(arg)) )
	{	
		arg = base_name(obj);
		obj = new(arg);
	}
	else
	{
		arg = resolve_path(me, arg);
		intact_file_name(ref arg);
	}

	if( arg[<2..<1] == ".o" ) arg = arg[0..<3];

	if( !objectp(obj = clone_object(arg)) )
		return tell(me, "�L�k�ƻs���� "+arg+"�C\n");

	if( environment(me) ) 
	{
		if( amount && !obj->is_living() )
		{
			if( !query("flag/no_amount", obj) )
				set_temp("amount", amount, obj);
		}
		else amount = "1";
		
		msg("$ME���⥭���A��F�X�U��ի�A��M�b�@�}"HIC"��"NOR CYN"��"NOR"�U�X�{�F "+NUMBER_D->number_symbol(amount)+" "+(query("unit", obj)||"��")+("$YOU"||file_name(obj))+"�C\n", me,obj, 1, ENVMSG);

		if( obj->is_living() )
		{
			if( query_temp("location", me) )
				obj->move(copy(query_temp("location", me)));
			else
				obj->move(environment(me));
		}
		else if( !me->get_object(obj, amount) )
		{
			tell(me, pnoun(2, me)+"�����ʨ���h�F��C\n");
			
			if( query_temp("location", me) )
				obj->move(copy(query_temp("location", me)));
			else
				obj->move(environment(me));
		}
		return;
	}
	else
	{
		destruct(obj);
		tell(me, pnoun(2, me)+"�ä����B��������ҡA�гq���Ův�B�z�C\n");
	}
}