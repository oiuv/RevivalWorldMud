/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _idname_obj.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-13
 * Note   : ����m�W, ID
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <buff.h>

#define ID	0
#define NAME	1

string *id_name;

// Passerby@IT �e�� RW �� -.-  by Clode
// MudOS �B�z replace_string() �ɪ��Ĳv�Ӯt, �]�����Φ��禡 by Clode (2006/09/12)
/*
nomask string noregexp(string arg)
{
        arg = replace_string(arg, "\\", "\\\\");
        arg = replace_string(arg, "|", "\\|");
        arg = replace_string(arg, "*", "\\*");
        arg = replace_string(arg, "+", "\\+");
        arg = replace_string(arg, "?", "\\?");
        arg = replace_string(arg, "^", "\\^");
        arg = replace_string(arg, ".", "\\.");
        arg = replace_string(arg, "[", "\\[");
        arg = replace_string(arg, "]", "\\]");
        arg = replace_string(arg, "(", "\\(");
        arg = replace_string(arg, ")", "\\)");
        return arg;
}*/

// �h�q���Y�g�P�_
int id(string arg)
{
	int i, size;
	string *idarray;
	       
	if( !arg || !id_name[ID] ) return 0;

	//arg = "^"+noregexp(lower_case(remove_ansi(arg)));
	
	arg = lower_case(remove_ansi(arg));
	
	idarray = explode(lower_case(remove_ansi(id_name[ID])), " ");
	
	size = sizeof(idarray);
	
	for(i=0;i<size-1;i++)
		idarray |= ({ implode(idarray[i..], " ") });
	
	foreach(string id in idarray)
	{
		if( id[0..strlen(arg)-1] == arg )
			return 1;
	}
	
	return 0;
	
	// ID ²�g�P�O
	//return sizeof( regexp(idarray,arg) );
}

// �^�Ǫ��� ID
varargs string query_id(int raw)
{
	return undefinedp(raw) ? id_name[ID] : remove_ansi(lower_case(id_name[ID]));
}

// �^�Ǫ��� NAME 
varargs string query_name(int raw)
{
	return undefinedp(raw) ? id_name[NAME] : remove_ansi(id_name[NAME]);
}

// �^�Ǫ��󧹾�W��
varargs string query_idname(int raw)
{
	if( !arrayp(id_name) ) return 0;
	return undefinedp(raw) ? id_name[NAME]+"("+ansi_capitalize(id_name[ID])+")" : remove_ansi(id_name[NAME])+"("+capitalize(remove_ansi(id_name[ID]))+")";
}

varargs string *set_idname(string id, string name)
{
	if( !stringp(id) && !stringp(name) ) return 0;

	if( !arrayp(id_name) )
		id_name = allocate(2);
	
	if( stringp(id) )
	{
		id = kill_repeat_ansi(remove_fringe_blanks(id)+NOR);
 
 		if( !this_object()->is_living() )
 			id = replace_string(id, " ", "-");

		id_name[ID] = id;
	}
	
	if( stringp(name) )
	{
		name = kill_repeat_ansi(remove_fringe_blanks(name)+NOR);
		
		id_name[NAME] = name;
	}

	return id_name;
}

varargs string short(int need_quantity)
{
	string str = "";
	string status = "";

	if( query_temp("status") )
		status += " "+query_temp("status");
	if( query_temp("endurance") < 0)
		status += HIW" �l"NOR WHT"�a";

	if( need_quantity && !this_object()->is_living() )
	{
		mixed amount = query_temp("amount") || 1;
		
		if( !query("flag/no_amount") )
			str = " "+NUMBER_D->number_symbol(amount)+" "+(query("unit")||"��");
	}
	
	str += query_idname();

	if( status != "" )
		str += status;

	return str+NOR;
}

varargs string long(int need_quantity)
{
	return DESCRIPTION_D->query_object_description(this_object(), need_quantity);
}
