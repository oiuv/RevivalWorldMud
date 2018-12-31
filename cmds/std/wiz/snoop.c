/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : snoop.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-4-14
 * Note   : snoop ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� snoop ���O�C
snoop 		target			�}�l�� target ��ť
snoop -d 	(target||all)   	������ target ��ť
snoop -log 	target			�}�l�O��
snoop -log	target	filename	�N�T���O���� filename
snoop -logstop 	target			����O��

HELP;

//#define SNOOP_SHADOW "/obj/etc/snoop_shadow.c"

mapping container=([]);

void notify_snooper_catch(object snoopee,string msg)
{
	string msgs = "";
	
	if( undefinedp(container[snoopee]) )
	{
		remove_shadow(snoopee);
		return;
	}
	
	if( query_temp("snoop_log",snoopee) )
	{
		write_file(query_temp("snoop_log",snoopee),sprintf("--%s--\n%s",ctime(time())[4..18],msg));
	}
	
        foreach( string message in explode(msg,"\n") )
	if( message )
                msgs+=sprintf(WHT"[%|12s]-"NOR"%s"NOR"\n",remove_ansi(snoopee->query_id()),message);
	
	
	foreach( object snooper in container[snoopee])
	{
		tell(snooper,msgs,"snoop");
	}
	
}

void notify_snooper_cmd(object snoopee, string msg)
{
	//string msgs = "";
	
	if( undefinedp(container[snoopee]) )
		return;
	
	if( query_temp("snoop_log",snoopee) )
	{
		write_file(query_temp("snoop_log",snoopee),sprintf("%s\n",HIY+msg+NOR));
	}
	
	foreach( object snooper in container[snoopee])
	{
		//tell(snooper,HIY+remove_ansi(msg)+NOR);
		tell(snooper,sprintf(WHT"[%|12s]-"HIY"%s"NOR"\n",remove_ansi(snoopee->query_id()),remove_ansi(msg)),"snoop");
	}
	
}

void remove_user(object ob)
{
	if( call_stack(1)[3]!=load_object("/cmds/std/ppl/quit.c"))
	{
		tell(find_player("cookys"),sprintf("%O\n",call_stack(1)));
		return ;
	}
		
	//remove_shadow(ob);
	
	map_delete(container,ob);
}

private void command(object me, string arg)
{
	object target;
	string msg="";
	
	if( !is_command() ) return;

//	old_target = query_snooping(me);

	if( !arg )
	{
		if( !sizeof(container) ) return tell(me,"�S���H�b��ť���I\n");
		foreach(object snoopee,mixed snoopers in container)
		{
			msg+=sprintf("%15s �G%s\n%15s",snoopee->query_idname(),query_temp("snoop_log",snoopee)?(HIR+query_temp("snoop_log",snoopee)+NOR):" ","");
			//printf("%O\n",container);
			if( sizeof(snoopers) )
			foreach(object snooper in snoopers)	
				msg+=sprintf("%12O",snooper->query_idname());
			msg+="\n";
		}
		if( !msg ) 
			tell(me,"�S���H�b��ť���I\n");
		else 	tell(me,msg);
		return ;
	}
	
	
	
	if( sscanf(arg,"-d %s",arg)==1 )
	{
		if( arg=="all")
		{
			foreach(object snoopee, mixed snoopers in container)
				if( arrayp(snoopers) && member_array(me, snoopers)!=-1 )
				{
					container[snoopee] = container[snoopee]-({me});
					tell(me,"������"+snoopee->query_idname()+"����ť�C\n");
					if( sizeof(container[snoopee])<1 && !query_temp("snoop_log",snoopee) )
					{
						map_delete(container,snoopee);
						//remove_shadow(snoopee);
					}
				}
			return;
		}
		
		if( !(target=find_player(arg) || present(arg, environment(me)) ) )
			return tell(me,"�o��S�� "+arg+" �o�˪F��I\n");
		
		if( undefinedp(container[target]) )
			return tell(me,"�A�èS�� snoop "+target->query_idname()+"�C\n");
		
		if( member_array(me,container[target]) == -1 )
			return tell(me,"�A�èS�� snoop "+target->query_idname()+"�C\n");
			
		container[target]-=({me});
		
		if( sizeof(container[target])<1 && !query_temp("snoop_log",target) )
		{
			map_delete(container,target);
			//remove_shadow(target);
		}		
		tell(me,"������"+target->query_idname()+"����ť�C\n");
		
		return;
		
	}
	
	if( sscanf(arg,"-log %s",arg)==1 )
	{
		string file;
		
		
		if( sscanf(arg,"%s %s",arg,file)==2 );
		
		
		if( !(target=find_player(arg) || present(arg, environment(me)) ) )
			return tell(me,"�o��S�� "+arg+" �o�˪F��I\n");
		
		if( !file )  file="/log/command/snoop_log/"+replace_string(target->query_id(1)," ","_");
		
		set_temp("snoop_log",file,target);
	//	new(SNOOP_SHADOW,target);
		if( undefinedp(container[target]) )
			container[target]=({});
		tell(me,"�}�l�O�� "+target->query_idname()+"���@�|�@�ʨ�[ "+file+" ]�C\n");
		return;
	}
	
	if( sscanf(arg,"-logstop %s",arg)==1 )
	{
		if( !(target=find_player(arg) || present(arg, environment(me)) ) )
			return tell(me,"�o��S�� "+arg+" �o�˪F��I\n");
		
		if( undefinedp(container[target]) || !query_temp("snoop_log",target))
			return tell(me,"�L���ӴN���b log ���I\n");
			
		delete_temp("snoop_log",target);
		
		if( sizeof(container[target]) < 1 )
		{
			map_delete(container,target);
			//remove_shadow(target);
		}
		
		tell(me,"������"+target->query_idname()+"�� log �ʧ@�C\n");
		
		return;
	}
	
	if( !(target=find_player(arg) || present(arg, environment(me)) ) )
		return tell(me,"�o��S�� "+arg+" �o�˪F��I\n");
		
	if( target == me )
			return tell(me, pnoun(2, me)+"�L�k��ť�ۤv�C\n");

	//new(SNOOP_SHADOW,target);
	
	if( undefinedp(container[target]) )
		container[target]=allocate(0);
	container[target]-=({0});
	container[target] += ({ me });
	
	
	tell(me, pnoun(2, me)+"�}�l��"+target->query_idname()+"�[�H��ť�C\n");
	
	//foreach(object ob in children("/obj/etc/snoop_shadow")) if( clonep(ob) && !query_shadowing(ob) ) destruct(ob);
}               

void remove()
{
	foreach(object snoopee,mixed snoopers in container)
	{
		if( sizeof(snoopers) )
			foreach(object snooper in snoopers)	
				tell(snooper,"�����G"+snoopee->query_idname()+"����ť�C\n");
		//remove_shadow(snoopee);
	}
}