/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : hyperlink.c
 * Author : Cookys@RevivalWorld
 * Date   : 2002-09-15
 * Note   : �إ߻P�ק��ɮ׼��Y��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */
 
#include <feature.h>
#include <ansi.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        �з� hyperlink ���O
HELP;

#define	FROM	0
#define TO	1

#define ENV_OBJ  	0
#define ENV_EXIT	1
#define ENV_PATH	2

/*
** is_mapsys ���U�h,�� array ��_�ӵ����ĤG��,�ĤG���p�G���O mapsys �n�� exit name
** not_mapsys ���U�h�n�n�D exit name,�ݲĤG���O���O map_sys,���O���ܭn exitname
**
**
*/

void generate_2way_link(int flag)
{
	mixed data;
	
	if( !stringp( ( data=query_temp("hyperlink",this_player()))[TO][ENV_OBJ]) && !objectp(data[TO][ENV_OBJ]) )
		return tell(this_player(), "�Ш���n�s�����a�I�b�U�@�����O�C\n", CMDMSG);
		

	if( data[FROM][ENV_OBJ]==MAP_D)
		MAP_D->set_special_exit(data[FROM][ENV_PATH],data[TO][ENV_PATH]);
	else	set("exits/"+data[FROM][ENV_EXIT],data[TO][ENV_PATH],data[FROM][ENV_OBJ]);
	
	if(flag)
	{
		delete_temp("hyperlink",this_player());
		return tell(this_player(), "����!\n", CMDMSG);
	}
	
	if(data[TO][ENV_OBJ]==MAP_D)
		MAP_D->set_special_exit(data[TO][ENV_PATH],data[FROM][ENV_PATH]);
	else	set("exits/"+data[TO][ENV_EXIT],data[FROM][ENV_PATH],data[TO][ENV_OBJ]);
	delete_temp("hyperlink",this_player());
	return tell(this_player(), "�����I\n", CMDMSG);
	
}

private void command(object me, string arg)
{
	int flag;
	object env=environment(me);
	mixed data=query_temp("hyperlink",me);

        if( !is_command() ) return;

        if( !arg && !env->is_maproom() )
	        return tell(me, "�п�J�n�s�����X�f�W�٩Τ�V�C \n", CMDMSG);
	        
	if( sscanf(arg||"","-s %s",arg) || arg == "-s" )
		flag = 1;
	
	if( undefinedp(data) )
	{
		data = allocate(2);
		data[TO]=allocate(3);
		data[FROM]=allocate(3);
	}
	
	if( env->is_maproom() )
	{
		//mixed tmp=MAP_D->query_special_exit(query_temp("location",me));
		//if( stringp(tmp) || arrayp(tmp) )
		if( !undefinedp(MAP_D->query_special_exit(query_temp("location",me))) )
			return tell(me, "���B�w�g���s���X�f�A�Y�ɦa�ϤW�u�঳�@�B�X�f�C\n�Х��N��R����A�s���C\n", CMDMSG);
		
		if( stringp(data[FROM][ENV_OBJ]) || objectp(data[FROM][ENV_OBJ]) )
			data[TO]=({MAP_D,0,query_temp("location",me)});
		else data[FROM]=({MAP_D,0,query_temp("location",me)});
		
		set_temp("hyperlink",data,me);
		
		return generate_2way_link(flag);
	} else {
	
                if( query("exists", environment(me)) && !undefinedp(query("exits",environment(me) )[arg] ) )
			return tell(me, HIC + arg + NOR +" �o�ӥX�f�w�s�b�C\n", CMDMSG);
		if( stringp(data[FROM][ENV_OBJ]) || objectp(data[FROM][ENV_OBJ]) )
			data[TO]=({env,arg,file_name(env)});
		else data[FROM] =({env,arg,file_name(env)});
		
		set_temp("hyperlink",data,me);
		
		return generate_2way_link(flag);
	}			
		
		
	//set_temp("hyperlink",,me);

}
