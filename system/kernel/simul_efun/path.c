/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : path.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-7-2
 * Note   : included by Simul_Efun Object
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

/* �^�� id �Ův���ӤH�ؿ����| */
string wiz_home(string id)
{
 	if( !stringp(id) ) return 0;
 	
        return "/wiz/home/"+id;
}

/* �Ǧ^ user �H�����s�ɸ��| */
string user_path(string id)
{
	if( !stringp(id) ) return 0;
	
        return "/data/user/"+id[0..0]+"/"+id+"/";
}

string user_data(string id)
{
	if( !stringp(id) ) return 0;
	
	return "/data/user/"+id[0..0]+"/"+id+"/data.o";
}

string user_mail(string id)
{
	if( !stringp(id) ) return 0;
		
	return "/data/user/"+id[0..0]+"/"+id+"/mail.o";	
}

string user_file_name(string id)
{
	if( !stringp(id) ) return 0;
	
	return "/data/user/"+id[0..0]+"/"+id;
}

string user_deleted_path(string id)
{
	if( !stringp(id) ) return 0;
	
	return "/data/backup/deleted/"+id[0..0]+"/"+id+"/";
}

string user_backup_path(string id)
{
	if( !stringp(id) ) return 0;

	return "/data/backup/user/"+id[0..0]+"/"+id+"/";
}

void add_slash(string ref path)
{
	if( path[<1] != '/' ) path += "/";
}

string resolve_path(mixed me, string arg)
{
	int i, j;
	string cwd, *stack;

	if( objectp(me) )
	{	
		cwd = query("current_work/directory", me);
		if( !stringp(cwd) )
			cwd = wizardp(me) ? wiz_home(me->query_id(1)) : "/";
	}	
	else if( stringp(me) )
	{
		cwd = me;
		if( cwd[0] != '/' ) cwd = "/"+cwd;
	}
	else return 0;
	
	// �B�z�S��Ѽ�
	switch(arg)
	{
		case 0:
		case "":
		case ".":	return cwd;
		case "..":	return cwd == "/" ? cwd : cwd[0..strsrch(cwd[0..<2], "/", -1)];
		case "me":	return objectp(me) ? file_name(me) : cwd;
		case "here":	return objectp(me) && environment(me) ? file_name(environment(me)) : cwd;
	}
	
	// �ഫ����P�۹���|
        switch(arg[0])
        {
                case '/': cwd = arg; break;
                case '~':
                	switch(arg[1])
                	{
                		case 0:
                		case '/':
                			if( objectp(me) ) cwd = wiz_home(me->query_id(1))+"/"+arg[2..];
                			break;
                		default:
                			cwd = wiz_home(arg[1..]);
                	}
                        break;
                default:
                        cwd += arg;
                        break;
        }
        
        // �R�������n�� "" �P "."
        stack = explode(cwd, "/") - ({ "" , "." });
	
	i = sizeof(stack);
	
	while(i--)
	{
		if( stack[i] == ".." )	
		{
			stack[i] = 0;
			j++;
		}
		else if(j)
		{
			stack[i] = 0;
			j--;
		}
	}
	
 	return "/" + implode(stack, "/");
}	

/* �T�O�ɮ׸��|����*/
void intact_path(string file)
{
        string path="", layer, *dir;

        if( file_size(file)!=-1 ) return;

        dir = explode(file, "/")[0..<2];
	foreach( layer in dir )
	{
		path += "/"+layer;
		mkdir(path);
	}
}


string *deep_path_list(string root)
{
	string file, *list=({});
	
	foreach( file in get_dir(root) )
	{
		if( file_size(root+file) == -2 )
			list += deep_path_list(root+file+"/");
		else list += ({ root+file });
	}
	
	return list;
}

int char_exists(string id)
{
	return file_size(user_path(id)) == -2;
}

string *all_player_ids()
{
	string *all_ids = allocate(0);
	int charhead;
	
	for( charhead = 'a'; charhead <= 'z' ; charhead++ )
		all_ids += get_dir("/data/user/"+sprintf("%c", charhead)+"/");
		
	return all_ids;
}