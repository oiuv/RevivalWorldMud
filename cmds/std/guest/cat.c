/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cat.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-6-10
 * Note   : cat ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
#include <runtime_config.h>

inherit COMMAND;

string help = @HELP
	�з� cat ���O
HELP;

private void command(object me, string arg)
{
	object ob;
	
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "�п�J�ɦW�C\n", CMDMSG);

	if( objectp(ob = present(arg)) )
		arg = base_name(ob);
	else
		arg = resolve_path(me, arg); 
	
	intact_file_name(ref arg);
	
	if( !file_exist(arg) )
		return tell(me, "�S�� "+arg+" �o���ɮסC\n", CMDMSG);

	if( file_size(arg) > get_config(__MAX_READ_FILE_SIZE__) )
		return tell(me, "�ɮ׹L�j( > "+get_config(__MAX_READ_FILE_SIZE__)+" )�A�Шϥ� more �˵��C\n", CMDMSG);

	set("current_work/file", arg, me);
    	return tell(me, read_file(arg)+"\n\n", CMDMSG);
}
