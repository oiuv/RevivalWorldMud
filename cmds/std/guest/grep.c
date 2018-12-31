/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : grep.c
 * Author : Cookys@RevivalWorld
 * Date   : 2001-07-11
 * Note   : grep ���O�C
 * Update :
 *  o 2001-07-30 Cookys �״_�@�Ǥp bug
 *
 -----------------------------------------
 */

#include <feature.h>
#include <ansi.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        �з� grep ���O�C
HELP;

private void command(object me, string arg)
{
	int line,str_len,flag,deep;
	string str,src,dir,*files,return_str="",file_data;
	
	if( !is_command() ) return;
	
	if( !arg )	
		return tell(me, help, CMDMSG);
		
	if( sscanf(arg,"-d %s",arg))
		deep=1;
	if( sscanf(arg,"\"%s\" %s",str,src)!=2 && sscanf(arg,"'%s' %s",str,src)!=2 && sscanf(arg,"%s %s",str,src)!=2 )
	{
		src=".";
		str=arg;
	}
	/*
	if( src=="." )
		dir=resolve_path(me,src); 	
	else 
	{
		dir=resolve_path(me);
		files=({src});
	}
	*/
	if( src && src!="." )
	{
		dir=resolve_path(me,src); 
		if( file_size(dir)==-2 )
		{
			dir+="/";
			files=get_dir(dir);
		}
		else if (file_size(dir)!=-1) files=({""});
	} else dir=resolve_path(me);
	if(!files) files=({});
	//printf("Curr DIR:%s\nFILES:%O\n",dir,files);
		
	if( src=="." && !sizeof(files = get_dir(dir)) )
        {
                if (file_size(dir)==-2)    
                        tell(me, "���ؿ����L�����ɮסC\n", CMDMSG);
                else   	
                	tell(me, "�S���o���ɮסC\n", CMDMSG);
                
                return;
        }
        
        files-=({".",".."});
        tell(me, sprintf("�q %s ���M�� %s �r��G\n",dir,str), CMDMSG);
	str_len=strlen(str);
	if( !deep )
	foreach( string file in files)
	{
		file_data=read_file(dir+file);
		if( file_data && strsrch(file_data,str)!=-1 )
		{
			return_str+=sprintf(HIY"\n�b�ɮ� %s ���t�� %s �r��G\n"NOR,dir+file,str);
			foreach(string file_entry in explode(file_data,"\n"))
			{
				line++;
				if( (flag=strsrch(file_entry,str))!=-1 )
					return_str+=sprintf("%4d�G%s\n",line,file_entry[0..flag-1]+HIG HBMAG+file_entry[flag..flag+str_len-1]+NOR+file_entry[flag+str_len..<1]);
			}
			line=0;
		}
		//if(return_str) 	tell(me, return_str, CMDMSG);
		//	else tell(me, "�S���ŦX����ơI�I\n", CMDMSG);
		//return;
	} else for(int j=0;j<sizeof(files);j++ )
	{
		file_data=read_file(dir+files[j]);
		//return_str+=sprintf(HIC"�t�l�ؿ��`�h�j�M...\n\n"NOR);
		if( !file_data )
		{
			if( file_size(dir+files[j])==-2 )
			{
				//files+=get_dir(dir+files[j]+"/")-({".",".."});
				foreach(string tmp in get_dir(dir+files[j]+"/")-({".",".."}))
				{
					files+=({files[j]+"/"+tmp});
				
				//printf("directory added : %O\n",files[j]+"/"+tmp);
				}
			}
			//printf("i cant read_file:\t%O\n",dir+files[j]);
		}
		
		else	if( strsrch(file_data,str)!=-1 )
		{
			return_str+=sprintf(HIY"\n�b�ɮ� %s ���t�� %s �r��G\n"NOR,dir+files[j],str);
			
			foreach(string file_entry in explode(file_data,"\n"))
			{
				line++;
				if( (flag=strsrch(file_entry,str))!=-1 )
					return_str+=sprintf("%4d�G%s\n",line,file_entry[0..flag-1]+HIG HBMAG+file_entry[flag..flag+str_len-1]+NOR+file_entry[flag+str_len..<1]);
			}
			line=0;
		}
		//printf("getdir:%O\ndir:%O\n",get_dir(dir+files[j]),dir+files[j]+"/");
		
	  	//tell(me, return_str, CMDMSG);
		//	else tell(me, "�S���ŦX����ơI�I\n", CMDMSG);		
		if(return_str!="") 	tell(me, return_str, CMDMSG);
			else tell(me, "�S���ŦX����ơI�I\n", CMDMSG);
	}
	if(return_str!="") 	
		tell(me, return_str, CMDMSG);
	else 
		tell(me, "�S���ŦX����ơI�I\n", CMDMSG);
	//tell(me, return_str, CMDMSG);
}