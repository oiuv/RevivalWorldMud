/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : dest.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-7-5
 * Note   : dest ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� dest ���O�C
HELP;

private void command(object me, string arg)
{
	int dest_all;
	string str, ob_idname, amount, total_amount, location;
	object ob, env;
	
	if( !is_command() ) return;

	if( !arg ) return tell(me, "���O�榡�Gdest <����W||�ɮצW>�C\n", CMDMSG);
	
	if( sscanf(arg, "%s %s", amount, arg) == 2 )
	{
		// �p�G amount �ä��O�Ʀr���
		if( to_float(amount) == 0. )
		{	
			if( lower_case(amount) == "all" )
				dest_all = 1;
			else
			{
				arg = amount + " " + arg;
				amount = "1";
			}	
		}	
		else if( count(amount, "<", 1) ) amount = "1";
	}
	else amount = "1";
	
	ob = present(arg, me) || present(arg, environment(me));

	if( !objectp(ob) )
	{
		str = resolve_path(me, arg);
		ob = find_object(str) || find_object(str+".c") || find_object(str+".o");
	}

	if( !objectp(ob) )
		return tell(me, "�S�� "+arg+" �o�Ӫ�����ɮסC\n", CMDMSG);
	
	ob_idname = ob->query_idname()||file_name(ob);
	
	if( userp(ob) )
		return tell(me, pnoun(2, me)+"�L�k�R���@�ӨϥΪ̪���C\n", CMDMSG);
	
	total_amount = query_temp("amount",ob) || "1";
	
	env = environment(ob);
	
	if( env == me )
		location = "���W";
	else if( env == environment(me) )
		location = "�a�W";
	else
		location = "�O���餤";
	
	if( dest_all || count(total_amount, "<=", amount) )
	{	
		string unit = query("unit",ob)||"��";
		
		if( destruct(ob) )
			return msg("$ME�ϥX"HIW"��"NOR WHT"��"HIW"�x"NOR"�N"+location+(total_amount==" 1"?"�� "+NUMBER_D->number_symbol(total_amount)+" "+unit:"������")+"�u"+ob_idname+"�v�����F�H���C\n", me, 0, 1, ENVMSG);
			
		return tell(me, "�R������ "+arg+" ���ѡC\n", CMDMSG);
	}
	else
	{
		msg("$ME�ϥX"HIW"��"NOR WHT"��"HIW"�x"NOR"�N"+location+"�� "+NUMBER_D->number_symbol(amount)+" "+(query("unit",ob)||"��")+"�u"+ob_idname+"�v�����F�H���C\n", me, 0, 1, ENVMSG);
		
		if( env->is_living() )
			addn_temp("loading/cur", -to_int(count(amount,"*",query("mass",ob))), env);

		set_temp("amount", count(total_amount, "-", amount), ob);
		return;
	}
}