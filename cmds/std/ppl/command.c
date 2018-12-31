/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : command.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : command ���O
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
  ���O�d�߫��O

  �o�ӫ��O�i�d�ߥثe���ͪ��@�ɤ��Ҧ��i�ϥΪ����O�A���إi����

  wizard	�Ův���O
  player	���a���O
  channel	�W�D���O
  emotion	�����O
  other	��L

  ���O�榡:
  command <���إN��>

  �������O:
HELP;

mapping chinese =
([
	"wiz"			:	"�Ův���O",
	"ppl"			:	"���a���O",
	"chn"			:	"�W�D���O",
	"emt"			:	"�����O",
	"city"			:	"�����a�ϫ��O",
	"individual/mayor"	:	"�����p�H���O",
]);

private void command(object me, string arg)
{	
	int i, size;
	mixed all_commands;
	string *commands, msg="";

	if( !is_command() ) return;
	
	switch(arg)
	{
		case "wizard"	: arg = "wiz"; break;
		case "player"	: arg = "ppl"; break;
		case "channel"	: arg = "chn"; break;
		case "emotion"	: arg = "emt"; break;
		case "other"	: arg = "sub"; break;
		default:
			return tell(me, "�п�J�d�߫��O����: wizard, player, channel, emotion, other�C\n", CMDMSG);
	}
	all_commands = COMMAND_D->query_commands();

	if( arg == "sub" )
	{
		foreach( string dir, mapping cmds_map in all_commands[arg] )
		{
			commands = sort_array(keys(cmds_map), 1);
			
			msg += sprintf(HIC"%-12s"NOR CYN"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR, chinese[dir]);
					
			size = sizeof(commands);
			i = 0;
			while(i<size)
			{
				msg += sprintf("%@-15s\n", commands[i..i+4]);
				i += 5;
			}
		}
	}
	else if( arrayp(commands = all_commands[arg]) )
	{
		commands = sort_array(commands, 1);
				
		msg += sprintf(HIC"%-12s"NOR CYN"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR, chinese[arg]);
				
		size = sizeof(commands);
				
		while(i<size)
		{
			msg += sprintf("%@-15s\n", commands[i..i+4]);
			i += 5;
		}
	}
	else
		return error("���O�C��榡���~�C\n");

	msg += NOR CYN"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	me->more(msg);
}