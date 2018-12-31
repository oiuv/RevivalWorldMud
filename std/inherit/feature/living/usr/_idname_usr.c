/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _idname_usr.c
 * Author : Clode@RevivalWorld
 * Date   : 2000-12-29
 * Note   : �ϥΪ̦W�٨t��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <status.h>

#define ID	0
#define NAME	1

private string *id_name;

// �� present �I�s, �i�樭���T�{
nomask int id(string arg)
{
	string no_ansi_id;
	
	if( !arg || !arrayp(id_name) )
		return 0;

	no_ansi_id = remove_ansi(id_name[ID]);
	arg = lower_case(arg);

	return arg == no_ansi_id || arg == capitalize(no_ansi_id);
}

// �^�ǨϥΪ� ID
nomask varargs string query_id(int raw)
{
        return id_name ? copy(undefinedp(raw) ? id_name[ID] : remove_ansi(id_name[ID])) : 0;
}

// �^�ǨϥΪ� NAME 
nomask varargs string query_name(int raw)
{
        return id_name ? copy(undefinedp(raw) ? id_name[NAME] : remove_ansi(id_name[NAME])) : 0;
}

// �^�ǨϥΪ̧���W��
nomask varargs string query_idname(int raw)
{
	if( !arrayp(id_name) ) return 0;
	return copy(undefinedp(raw) ? id_name[NAME]+"("+ansi_capitalize(id_name[ID])+")" : remove_ansi(id_name[NAME])+"("+capitalize(remove_ansi(id_name[ID]))+")");
}

varargs nomask string *set_idname(string id, string name)
{
	int valid_write;
	
	if( !stringp(id) && !stringp(name) ) return 0;
	
	if( previous_object() != load_object(PPL_LOGIN_D) && previous_object() != load_object("/cmds/std/ppl/chfn") ) return 0;

	if( !arrayp(id_name) )
	{
		id_name = allocate(2);
		valid_write = 1;
	}
	
	if( stringp(id) )
	{
		id = kill_repeat_ansi(lower_case(remove_fringe_blanks(id))+NOR);
		
		// ���a ID �u��ϥέ^��
		foreach( int a in remove_ansi(id) )
			if( a < 'a' || a > 'z') return 0;
 
		if( valid_write )
			id_name[ID] = id;
		// �إߨ���� ID �A�]�������
		else if( remove_ansi(id_name[ID]) != remove_ansi(id) )
			return 0;
		else
			id_name[ID] = id;
	}
	
	if( stringp(name) )
	{
		name = kill_repeat_ansi(remove_fringe_blanks(name)+NOR);
		
		id_name[NAME] = name;
	}

	return copy(id_name);
}

varargs string add_status(string status, int sec)
{
	mapping statusmap = query_temp("status");
	
	if( sec < 0 )
		error("���X�z�����");
	else if( undefinedp(sec) )
		sec = -1;

	if( !mapp(statusmap) )
		statusmap = allocate_mapping(0);
		
	statusmap[status] = sec;
	
	set_temp("status", statusmap);
}

varargs void remove_status(string status)
{
	mapping statusmap = query_temp("status");
	
	if( !mapp(statusmap) ) return;
		
	if( !undefinedp(status) )
		map_delete(statusmap, status);
	else
	{
		delete_temp("status");
		return;
	}
	
	if( !sizeof(statusmap) )
		delete_temp("status");
}

varargs string query_status(int flag)
{
	int idle;

	string status_buff;
	string status = "";
	string *extra_status;

	if( mapp(extra_status = query_temp("status")) && sizeof(extra_status) )
		foreach(string key, int sec in extra_status)
			status += key+NOR" ";

	if( query("faint") ) 
		status += HIR"��"NOR RED"�� "NOR;
	if( in_input() )	
		status += HIW"��"NOR WHT"�J "NOR;
	if( in_edit() )		
		status += HIW"�s"NOR WHT"�� "NOR;
	
	if( !interactive(this_object()) )
		status += HIW"�_"NOR WHT"�u<"+find_call_out(query_temp("net_dead"))+"> "NOR;
	else if( !(flag & STATUS_NO_IDLE) && (idle = query_idle(this_object())/60) > 10 )
		status += HIW"�b<"NOR WHT+(idle>=60?idle/60+":":"")+(idle%60>9?"":"0")+(idle%60)+HIW"> "NOR;
	
	if( !(flag & STATUS_NO_OBBUFF) )
	foreach(object ob in this_object()->query_equipment_objects())
	{
		if( stringp(status_buff = query("buff/status", ob)) )
			status += status_buff+" ";
	}

	if( status == "" )
		return 0;
	
	return status+NOR;
}

varargs string short()
{
	string ret="";
	string status = query_status();
	string city = query("city");
	string enterprise = query("enterprise");
	string title = query("title");
	string nickname = query("nickname");
	
	if( city && !query("env/no_player_city", this_player(1)) )
		ret += (CITY_D->query_city_name(city)||"") + " ";
		
	if( enterprise )
		ret += (ENTERPRISE_D->query_enterprise_color_id(enterprise)||"") + " ";
		
	if( title )
		ret += title + " ";	

	if( nickname )
	{
		if( sizeof(ret) > 0 && ret[<1] == ' ' )
			ret = ret[0..<2]+"�u"+nickname+"�v";
		else
			ret = ret + "�u"+nickname+"�v";
	}

	ret += query_idname();
	
	if( status )
		ret += " "+status;

	return ret;
}

void add_title(string title)
{
	string *all_titles = query("all_titles") || allocate(0);
		
	all_titles |= ({ title });
		
	tell(this_object(), pnoun(2, this_object())+"��o�u"+title+"�v���ٸ��C\n");
		
	set("all_titles", all_titles);

	this_object()->save();
}

void remove_title(string title)
{
	string *all_titles = query("all_titles") || allocate(0);
	
	if( member_array(title, all_titles) == -1 ) return;

	all_titles -= ({ title });
		
	tell(this_object(), pnoun(2, this_object())+"�ॢ�u"+title+"�v���ٸ��C\n");
		
	set("all_titles", all_titles);
	
	if( query("title") == title )
		delete("title");

	this_object()->save();
}

int have_title(string title)
{
	string *all_titles = query("all_titles") || allocate(0);
	
	return member_array(title, all_titles) != -1 ? 1 : 0;
}

int no_save(string file)
{
	if( sizeof(id_name) != 2 || !id_name[ID] || !id_name[NAME] || !id_name[ID][0] || !id_name[NAME][0] )
	{
		CHANNEL_D->channel_broadcast("sys", "�ϥΪ̪����a��ĵ�i�G"+file+"�A�����\����x�s�C");
		return 1;
	}
	return 0;
}

varargs string long(int need_quantity)
{
	return DESCRIPTION_D->query_living_description(this_object(), need_quantity);
}