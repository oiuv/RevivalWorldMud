/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : log.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-7-2
 * Note   : included by Simul_Efun Object
 * Update :
 *  o 2003-00-00

 -----------------------------------------
 */

#include <runtime_config.h>

// �����ɮڥؿ�
#define LOG_ROOT	"/log/"

#define DEFAULT_LOG_FILES	20

/* ������T */
varargs void log_file(string file, string log, int backup)
{
	string month, ctime, timedesc;

	object this_player = this_player(1);
	
	if( !stringp(file) || !stringp(log) ) return;
	
	file = LOG_ROOT + file;
	
	log = remove_ansi(log);

	if( log[<1] != '\n' ) log += "\n";
	
	//if( file_size(file)+strlen(log) > get_config(__MAX_READ_FILE_SIZE__) )
	if( file_size(file)+strlen(log) > 1024000 )
	{
		int i;
	
		while(file_size(file+"."+(++i)) > 0);

		while(--i)
		{
			if( !undefinedp(backup) )
			{
				// backup == -1 �ɬ��L��
				if( backup != -1 && i >= backup )
				{
					rm(file+"."+i);
					continue;
				}
			}
			else if( i >= DEFAULT_LOG_FILES )
			{
				rm(file+"."+i);
				continue;
			}

			rename(file+"."+i, file+"."+(i+1));
		}
		rename( file, file+"."+1 );
	}
	
	if( objectp(this_player) )
	{
		string ip = query_ip_number(this_player);
		
		log = (this_player->query_id(1)||"") + (ip ? "("+ip+") " :" ") + log;
	}

	ctime = ctime(time());

	switch(ctime[4..6])
	{
		case "Jan"	:	month = "01";break;
		case "Feb"	:	month = "02";break;
		case "Mar"	:	month = "03";break;
		case "Apr"	:	month = "04";break;
		case "May"	:	month = "05";break;
		case "Jun"	:	month = "06";break;
		case "Jul"	:	month = "07";break;
		case "Aug"	:	month = "08";break;
		case "Sep"	:	month = "09";break;
		case "Oct"	:	month = "10";break;
		case "Nov"	:	month = "11";break;
		case "Dec"	:	month = "12";break;
	}

	timedesc = sprintf("%s/%s/%s %s", month, (ctime[8]==' '?"0"+ctime[9..9]:ctime[8..9]), ctime[<4..<1], ctime[11..15]);

	write_file( file, timedesc+" "+log );
}
