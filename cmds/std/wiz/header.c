/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : header.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-15
 * Note   : �إ߻P�ק��ɮ׼��Y��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�i�۰ʫإ��ɮ׼��Y, �åi�ק���Y���e
ex :
header filename.c //�Y filename �|�L RW �榡���Y, �h�۰ʫإ߹w�]���Y, �@�̬��U�����O���H
header filename author clode // �]�w filename �@�̬� clode
header filename update update_info // �۰ʩ��U���W update ��T
header filename note note_info // ��g NOTE(����) ��

�P�ɤ䴩�h���J
to header filename author ��J�h�W�@��
to header filename update ��J�h���s��T
to header filename note ��J�h����Ѹ�T
HELP;


string default_header	= @HEADER
/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : %^FILENAME%^
 * Author : %^AUTHOR%^
 * Date   : %^DATE%^
 * Note   : 
 * Update :
 *  o %^THISYEAR%^-00-00
 *
 -----------------------------------------
 */

HEADER;

private int create_default_header(object me, string filename, string ref file)
{
	string header, create_date;
	
	if( !is_command() ) return 0;
	
	create_date = TIME_D->replace_ctime(time());
	
	header = terminal_colour(default_header, 
		([
			"FILENAME"	: filename[(strsrch(filename, "/",-1)+1)..], 
			"AUTHOR"	: capitalize(me->query_id(1))+"@RevivalWorld",
			"DATE"		: create_date[6..9]+"-"+create_date[0..1]+"-"+create_date[3..4],
			"THISYEAR"	: create_date[6..9],
		]));
	
	file = header + file;
	
	return write_file(filename, file, 1);
}

private void command(object me, string arg)
{
	int line;
	string header, *parse, *efile;
	string file, filename, option, arg2;
       	
       	if( !is_command() ) return;
       	
       	if( !arg )
       		return tell(me, "�п�J�Q�n�إ߼��Y���ɮצW�١C\n", CMDMSG);

	parse = single_quote_parse(arg);
	
	arg = parse[0];
	
	if( sizeof(parse) >= 3 )
	{
		option = parse[1];
		arg2 = implode(parse[2..], " ");
	}

	arg = resolve_path(me, arg);
	
	intact_file_name(ref arg);
	
	filename = arg;
	
	if( !file_exist(filename) )
		return tell(me, "�S�� "+filename+" �o���ɮסC\n", CMDMSG);
	
	if( filename[<2..<1] == __SAVE_EXTENSION__ )
		return tell(me, "�o���ɮ׫������A�X�إ߼��Y�C\n", CMDMSG);
		
	file = read_file(arg);
	
	// �ˬd�O�_�w�g�إ߼��Y
	if( file[0..37] != "/* This program is a part of RW mudlib" )
	{
		if( create_default_header(me, filename, ref file) )
			tell(me, header +"\n"+filename+" �w�]���Y�إߧ����C\n", CMDMSG);
		else
			return tell(me, pnoun(2, me)+"���v�������H�B�z�o���ɮסC\n", CMDMSG);
	}
	else if( !option || !arg2 )
		return tell(me, filename+" �����Y�w�g�إߡC\n", CMDMSG);
	
	if( option && arg2 )
	{
		efile = explode(file, "\n");
		
		switch( lower_case(option) )
		{
			case "author":
			{
				string author, *multi_author;
				
				multi_author = explode(arg2, "\n");
				
				for(int i=0;i<sizeof(multi_author);i++)
				{
					if( !i )
						author = remove_fringe_blanks(capitalize(multi_author[i]))+NOR"@RevivalWorld\n";
					else
						author += " *          "+remove_fringe_blanks(capitalize(multi_author[i]))+NOR"@RevivalWorld\n";
				}

				foreach( string ref f in efile )
				{
					if( f[0..10] == " * Author :" )
					{
						f = f[0..10]+" "+remove_fringe_blanks(kill_repeat_ansi(author));
						break;
					}
				}
							
				break;
			}
			case "update":
			{
				string update="", *multi_update;
				string date = TIME_D->replace_ctime(time());
				
				multi_update = explode(arg2, "\n");
				
				for(int i=0;i<sizeof(multi_update);i++)
				{
					if( !i )
						update += " *  o "+date[6..9]+"-"+date[0..1]+"-"+date[3..4]+" "+capitalize(me->query_id(1))+" "+remove_fringe_blanks(multi_update[i])+NOR+"\n";		
					else
						update += " *"+repeat_string(" ",16+strlen(me->query_id(1)))+remove_fringe_blanks(multi_update[i])+NOR+"\n";
				}
				
				for(int i=0;i<sizeof(efile);i++)
				{
					if( efile[i][0..10] == " * Update :" )
					{
						if( efile[i+1][10..15] == "-00-00" )
						{
							efile[i+1] = " "+remove_fringe_blanks(update);
							break;
						}
						
						while( efile[++i] != " *" );
						
						efile = efile[0..i-1] + ({ " "+remove_fringe_blanks(kill_repeat_ansi(update)) }) + efile[i..];
						
						break;
					}
				}
				
				break;
			}
			case "note":
			{
				string note, *multi_note;
				
				multi_note = explode(arg2, "\n");
				
				for(int i=0;i<sizeof(multi_note);i++)
				{
					if( !i )
						note = remove_fringe_blanks(multi_note[i])+NOR+"\n";
					else
						note += " *          "+remove_fringe_blanks(multi_note[i])+NOR+"\n";
				}
				
				foreach( string ref f in efile )
				{
					if( f[0..10] == " * Note   :" )
					{
						f = f[0..10]+" "+remove_fringe_blanks(kill_repeat_ansi(note));
						break;
					}
				}
				
				break;
			}
			default: return tell(me, "�S�� "+option+" �o�ӿﶵ�C\n", CMDMSG);
		}
		
		line = member_array(" */", efile) + 1;
		
		if( write_file(filename, implode(efile, "\n"), 1) )
		{
			CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�ק� "+filename+" ���Y�C\n");
			tell(me, read_file(filename, 1, line) +"\n\n"+filename+" �w�]���Y�إߧ����C\n", CMDMSG);
		}
		else
			return tell(me, pnoun(2, me)+"���v�������H�B�z�o���ɮסC\n", CMDMSG);
		
	}
}