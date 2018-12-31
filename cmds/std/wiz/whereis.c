/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : whereis.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-8-14
 * Note   : whereis ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <location.h>

inherit COMMAND;

string help = @HELP
	�з� whereis ���O
HELP;

private void whereis(string target_file, string dir, string ref *results, string ref *elem)
{
	string path;
	
	if( file_size(dir) != -2 || !arrayp(get_dir(dir)) )
		return;

	foreach(string file in get_dir(dir))
	{
		path = dir + file;

		if( file_size(path) == -2 ) whereis(target_file, path+"/", ref results, ref elem);
		
		else
		{
			if( file == target_file || file[0..<3] == target_file )
				results += ({ path });
				
			else if( file[0..(strlen(elem[0])-1)] == elem[0] )
			{
				int all_fit = 1;
			
				if( sizeof(elem) >= 2 
					&& file[<strlen(elem[<1])..<1] != elem[<1] 
					&& file[0..<3][<strlen(elem[<1])..<1] != elem[<1] )
				{
					all_fit = 0;
				}
				
				else if( sizeof(elem) > 2 )
				{
					foreach( string e in elem[1..<2] )
						if( strsrch(file, e) == -1 ) all_fit = 0;
				}
				
				if( all_fit )
					results += ({ path });
			}
		}
	}	
}
		
private void command(object me, string arg)
{
	int count;
	object ob;
	string msg;
	string *elem, *results = allocate(0);
	
	if( !is_command() ) return;
	
	if( !arg ) return tell(me, "�п�J���j�M���ɮצW�٩Ϊ��a ID�C\n", CMDMSG);
	
	if( objectp(ob = find_player(arg)) )
	{
		string location;
		object env = environment(ob);
		
		if( env )
		{
			if( env->is_maproom() )
			{
				array loc = query_temp("location", ob);
				location = CITY_D->query_city_idname(loc[CITY])+NOR"��"+CHINESE_D->chinese_number(loc[NUM]+1)+"���|�ϡA�y��("+(loc[X]+1)+","+(loc[Y]+1)+")";
			}
			else
				location = query("short", env);
		}	
		
		return tell(me, ob->query_idname()+"�ثe�Ҧb��m�G"+location+"�C\n", CMDMSG);
	}
	
	elem = explode(arg, "*");
	
	whereis(arg,"/", ref results, ref elem);
	
	if( !sizeof(results) ) 
		return tell(me, "�S������ŦX�W�ٱ����ɮסC\n", CMDMSG);
		
	msg = "�j�M "+arg+"�A�@�� "+sizeof(results)+" �ӷj�M���G�C\n";
	
	foreach( string r in results )
		msg += sprintf(HIY"%-3d -"NOR" %-s\n",++count, r);
	
	me->more(msg+"\n");
	
}