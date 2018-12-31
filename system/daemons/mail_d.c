/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mail_d.c
 * Author : Cookys@RevivalWorld
 * Date   : 2002-09-26
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */
 
#include <message.h>
 
#define USER_MAIL_PATH(x) user_path(x)+"mail/"
#define MAIL_INDEX	"mail_index"+ __SAVE_EXTENSION__

#define USER_MAIL_INDEX(x) USER_MAIL_PATH(x)+MAIL_INDEX

#define MAIL_DATA 	0
#define LAST_USE 	1

#define EXPIRE_TIME	36000

#define MAIL_DATA_PREFERENCE 	0
#define MAIL_DATA_CONTENT	1

private nosave mapping mail_index=allocate_mapping(0);

int restore_personal_mail_index(string id)
{
	mapping user_mail_index;
	
	if( file_size(user_path(id))==-2 )
		return 0;
		
	if( !user_mail_index=restore_variable(read_file(USER_MAIL_INDEX(id))))
		return tell(this_player(), "�L�k���^�H��s�ɡA�гq���޲z�̡I\n", CMDMSG);
		
	mail_index[id]=({ user_mail_index , time() });
	
	return 1;
}

int save_personal_mail_index(string id)
{
	string user_mail_index;
	
	if( undefinedp(mail_index[id]) )
		return 0;
	
	if( !user_mail_index=save_variable(mail_index[id][MAIL_DATA]) )
		return 0;
		
	if( write_file(USER_MAIL_INDEX(id),user_mail_index,1) )
		return 1;
	return 0;
}


int check_user_mail(string id)
{
	if( undefinedp(mail_index[id]) )
		return restore_personal_mail_index(id);
		
	return 1;
}


#define DATE		0
#define SUBJECT		1
#define FLAG		2
#define REF		3

/* Mail Example:
**
** From:	(string)
** To:		(array)
** Mail-body	(string)
** Signature	(string)
**
**
**
** Mail Index Example:
**
** Filename:	(string) -> mapping index
** Date:	(int)
** Subject:	(string)
** Flag:	(int)
** Reference:	(array)
**
*/


varargs string query_maillist(string id)
{
	int public_flag=0,num=1;
	string mail_list="";
	
	if( !id )
		id = this_player(1)->query_id();
		
	if( id != this_player(1)->query_id() )
		public_flag=1;
		
	check_user_mail(id);
	
	if( public_flag==0 )
	{
		foreach(string filename,mapping data in mail_index[id][MAIL_DATA][MAIL_DATA_CONTENT])
		{
			mail_list += sprintf("%4d.\tfrom:%8s\tsubject:%20s\tdate:%15s\tflag:%8s\t%s\n",num,); 
			num++;
		}
	} else  foreach (string filename,mapping data in mail_index[id][MAIL_DATA][MAIL_DATA_CONTENT])
		{
			
		}
		
}

void create()
{
	if( clonep() ) 
		destruct(this_object());
}


void create_exist_user_dir()
{
	string *root_dir, dir, id;

	root_dir = filter_array(get_dir("/data/user/"), (: strlen($1) == 1 :));

	foreach( dir in root_dir )
	{
		foreach( id in filter_array(get_dir("/data/user/"+dir+"/"),(: $1[<2..<1] == __SAVE_EXTENSION__ :)) )
                {
                	//id=id[0..<3];
                	//shout(sprintf("making new dir for user %s.\n%s\n",id,"/data/user/"+dir+"/"+id));
         		//mkdir("/data/user/"+dir+"/"+id);
         		//mkdir("/data/user/"+dir+"/"+id+"/mail");
         		cp("/data/user/"+dir+"/"+id,"/data/user/"+dir+"/"+id[0..<3]+"/"+id);
                }
        }
}
string query_name()
{
	return "�q�l�l��t��(MAIL_D)";
}
