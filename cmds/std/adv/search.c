/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : search.c
 * Author : Sinji@RevivalWorld
 * Date   : 2003-06-21
 * Note   : 
 * Update :
 *  o 2003-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
inherit COMMAND;
string *file_name = ({});
string last_file;
int search(string path, string arg);
private void command(object me, string arg)
{
	string dir, msg, file;


	if( !arg || sscanf(arg, "%s %s", dir, arg) != 2 )
		return tell(me, "���O�榡�Gsearch <�ؿ�> <�r��>\n", CMDMSG);
	dir = resolve_path(me, dir);
	if( file_size(dir) != -2 )
		return tell(me, "�S���o�ӥؿ��C\n", CMDMSG);
	search(dir, arg);
	if( sizeof(file_name) ) {
		msg = "�ɮקt�� " + arg + " �r�ꪺ���G\n";
		foreach(file in file_name) msg += file + "\n";
	} else msg = "�S���ɮפ��t�� " + arg + " �r��C\n";
	printf(msg);
	file_name = ({});
	return;
}
int search(string path, string arg)
{
	string *files;
	int i;

	if( path[strlen(path)-1] != '/' ) path += "/";
	files = get_dir(path);
	for(i=0; i<sizeof(files); i++) {
		if( file_size(path + files[i]) == -2 ) search(path + files[i], arg);
		else {
			last_file = path + files[i];
			if( strsrch(read_file(path + files[i]), arg) != -1 )
				file_name += ({ path + files[i] });
		}
	}
	return 1;
}
string query_last_file()
{
	return last_file;
}
