/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _more.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-13
 * Note   : more ������ܨt���~����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <message.h>
#include <runtime_config.h>

#define LINE_NUMBER	1

private void step_more(string *msg, int line, int size, int height, string cmd)
{
	int _height = height;
	string bar;

	sscanf(cmd, "%d", line);
	switch(cmd)
	{
	case "b":	
		tell(this_object(), implode(msg[(line -= _height*2>line?line:_height*2)..(line+=_height)-1],"\n")+"\n");
		break;
	case "q":
		tell(this_object(), "\n");
		this_object()->finish_input();
		return;
	default:
		if( size > _height )
			tell(this_object(), "\n" + implode(msg[line..(line+=_height)-1],"\n")+"\n");
		else 
			return tell(this_object(), "\n" + implode(msg[line..], "\n") + "\n");
		break;
	}

	if( line >= size )
	{
		this_object()->finish_input();
		return;
	}
	bar = sprintf(
		NOR WHT"�w�w �T������ "HIG"%3d"HIW"/"NOR GRN"%-3d"HIW"(%2d%%)"NOR WHT" �w�w "HIW"<ENTER>"NOR WHT"�U���A"HIW"B"NOR WHT" �W���A"HIW"<�Ʀr>"NOR WHT" ���ܦ�A"HIW"Q"NOR WHT" ���} �w�w"NOR,
		line,
		size,
		line*100/size
	);

	tell(this_object(), bar);
	
	input_to( (:step_more, msg, line, size, height:) );
}

private void more_process(string *msg, int start_line, int flag)
{
	int num, blanks, height, size;

	height = query_temp("windowsize/height")-1;

	if( height < 1 ) 
		height = 21;

	height -= query_temp("title_screen")+1;

	size = sizeof(msg);
	
	blanks = sizeof(size+"");

	if( flag & LINE_NUMBER )
		for(num = 1;num <= size;num++)
			msg[num-1] = sprintf(HIW"%-"+blanks+"d- "NOR"%s", num, msg[num-1]);  

	step_more(msg , start_line, size, height, "");
}

varargs nomask void more(string arg, int flag)
{
	string *msg;

	if( !arg || arg == "" ) return;	

	msg = explode(arg, "\n");

	if( arg[0] == '\n' ) msg = ({""}) + msg;

	more_process(msg, 0, flag);
}

nomask varargs void more_file(string file, int start_line, int flag, string extra)
{
	int bytes=0, fsize, max_byte_transfer;
	string str="", *msg;

	if( !file || file == "" ) return;

	fsize = file_size(file);

	if( fsize <= 0 ) return;

	max_byte_transfer = get_config(__MAX_BYTE_TRANSFER__);

	do
	{
		str += read_bytes(file, bytes, max_byte_transfer);
		bytes += max_byte_transfer;
	}
	while( bytes < fsize );

	msg = explode(str, "\n");

	if( !undefinedp(extra) )
		msg += ({ extra });

	more_process(msg, start_line-1, flag);	
}
