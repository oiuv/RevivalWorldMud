/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : bug_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-07-21
 * Note   : ���Φ^�����F
 * Update :
 *  o 2003-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>
#include <mudlib.h>

#define DATA_PATH		"/data/bug/"
#define DEFAULT_LOAD		10
#define SERIAL_NUMBER_KEY	"bug"

#define REPLY_AUTHOR		0
#define REPLY_MESSAGE		1

#define LIST_OPT_ALL	(1<<0)

array buginfo;

int bug_exists(string number)
{
	return file_exists(DATA_PATH+number);
}

void reset_buginfo()
{
	buginfo = allocate(0);
	
	foreach(string file in sort_array(get_dir(DATA_PATH), (: count($1, "<", $2) ? 1 : -1 :))[0..DEFAULT_LOAD-1])
		buginfo += ({ restore_variable(read_file(DATA_PATH+file)) });
}

// �C�X Bug �C��
string list_bug(int options)
{
	string listmsg;
	array list_buginfo;
	
	if( options & LIST_OPT_ALL )
	{
		list_buginfo = allocate(0);
		
		foreach(string file in get_dir(DATA_PATH))
			list_buginfo += ({ restore_variable(read_file(DATA_PATH+file)) });
	}
	else
		list_buginfo = buginfo;

	list_buginfo = sort_array(list_buginfo, (: count($1["number"], "<", $2["number"]) ? 1 : -1 :));
	listmsg =  "\n"+MUD_FULL_NAME+HIY" ���Φ^���t��\n"NOR;
	listmsg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	listmsg += "�s�� �^����                   �D�D                          �B�z���A  �^��\n";
	listmsg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	foreach( mapping data in list_buginfo )
	{		
		listmsg += sprintf(HIY"%-4s"NOR" %-24s %-:30s%|8s %5s\n", data["number"], data["author_idname"], data["title"], data["status"], sizeof(data["reply"]) ? sizeof(data["reply"])+"" : "");	
	}
	listmsg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	if( options & LIST_OPT_ALL )
		listmsg += "�C�X�Ҧ��^�����\n";
	else
		listmsg += "�C�X�̪� "+DEFAULT_LOAD+" ���^�����\n";	
	
	return listmsg;
}

// �s�W Bug ���
string add_bug(object me, string title, string content)
{
	mapping bugdata = allocate_mapping(0);
	
	if( !objectp(me) || !stringp(title) || !stringp(content) )
		error("BUG_D add_bug() ���~��J�Ѽ�");
	
	bugdata["time"] = time();
	bugdata["number"] = SERIAL_NUMBER_D->query_serial_number(SERIAL_NUMBER_KEY);
	
	bugdata["author_id"] = me->query_id(1);
	bugdata["author_idname"] = me->query_idname();
	
	bugdata["title"] = title;
	bugdata["content"] = content;
	
	bugdata["status"] = HIR"���B�z"NOR;

	bugdata["reply"] = allocate(0);
			
	if( write_file(DATA_PATH+bugdata["number"], save_variable(bugdata)) )
		reset_buginfo();
	else
		error("BUG_D �L�k�x�s�s�W bug ���");
		
	CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�s�W Bug �^���A�u"+title+NOR"�v�A�s���G"+bugdata["number"]+"�C");
	
	return bugdata["number"];
}

// �^�� Bug �B�z����
varargs void reply_bug(object me, string number, string status, string message)
{
	mapping data;

	if( !bug_exists(number) )
		error("BUG_D reply_bug() �L���ɮ�");
	else
		data = restore_variable(read_file(DATA_PATH+number));
	
	data["status"] = status;
	CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�ק�s���� "+number+" �� Bug �B�z���p���u"+status+"�v");
	
	if( find_player(data["author_id"]) )
		tell(find_player(data["author_id"]), me->query_idname()+"�^���s���� "+number+" �� Bug �B�z���p���u"+status+"�v�C\n");

	if( !undefinedp(message) )
	{
		if( !arrayp(data["reply"]) )
			data["reply"] = allocate(0);

		data["reply"] += ({ ({ me->query_idname(), message }) });
		CHANNEL_D->channel_broadcast("sys", me->query_idname()+"��J�s���� "+number+" �� Bug �B�z�^��");
	}
	
	write_file(DATA_PATH+number, save_variable(data), 1);
	reset_buginfo();
}

// ���� Bug ���
varargs void remove_bug(object me, string number, int reply)
{
	if( !bug_exists(number) )
		error("BUG_D remove_bug() �L���ɮ�");

	// �R����� Bug ��T
	if( undefinedp(reply) )
	{
		if( !rm(DATA_PATH+number) )
			error("BUG_D remove_bug() �L�k�R���ɮ�");

		reset_buginfo();
	}
	// �R���䤤�@�Ӧ^�����
	else
	{
		mapping data = restore_variable(read_file(DATA_PATH+number));

		reply--;
		
		if( reply < sizeof(data["reply"]) && reply >= 0 )
		{
			if( reply == 0 )
				data["reply"] = data["reply"][1..];
			else
				data["reply"] = data["reply"][0..reply-1] + data["reply"][reply+1..];
		}
		
		write_file(DATA_PATH+number, save_variable(data), 1);
		reset_buginfo();
	}
}

// �d�� Bug ���
string query_bug(string number)
{
	string bugmsg;
	mapping data;
	
	if( !bug_exists(number) )
		error("BUG_D query_bug() �L���ɮ�");
	else
		data = restore_variable(read_file(DATA_PATH+number));
		
	bugmsg =  sprintf(HIM"�s��"NOR" %-20s "HIM"�D�D"NOR" %s\n"NOR, data["number"], data["title"]);
	bugmsg += sprintf(HIM"�ɶ�"NOR" %-20s "HIM"�^��"NOR" %s\n", TIME_D->replace_ctime(data["time"]), data["author_idname"]);
	bugmsg += sprintf(HIM"���p"NOR" %-20s "HIM"�^��"NOR" %d\n", data["status"], sizeof(data["reply"]));
	bugmsg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	bugmsg += data["content"]+"\n";
	
	if( sizeof(data["reply"]) )
	{
		int replies;
		
		foreach( array reply in data["reply"] )
		{
			replies++;
			bugmsg += HIY"\n\n*** �� "+replies+" �g�^���G"+reply[REPLY_AUTHOR]+HIY+" ***\n"NOR;
			bugmsg += reply[REPLY_MESSAGE]+NOR"\n";
		}
	}
	
	bugmsg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	return bugmsg;
}

void create()
{
	reset_buginfo();
}
string query_name()
{
	return "���Φ^���t��(BUG_D)";
}
