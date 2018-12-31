/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : command_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-4
 * Note   : �t�Ϋ��O&�W�D���O&�����O��
 * Update :
 *  o 2001-07-29 Clode �ק���O���[�c, �������O�j�M�Ĳv
 *  o 2001-07-31 Clode �ק�������O�{��, �����^�Ǩ禡���д��ɮĲv
 *  o 2002-09-23 Clode �A���ק���O���[�c, �H���j���O����ϥζq�[�ַj�M�t��
 *                     �üW�[�ӤH���O�����]�w
 -----------------------------------------
 */

#include <daemon.h>
#include <secure.h>
#include <message.h>

#define LEVEL		0
#define FPOINTER	1
 
#define COMMAND_ROOT	"/cmds/"

private nosave mapping default_aliases = ([
    "l":	"look",
    "n":	"go north",
    "e":	"go east",
    "w":	"go west",
    "s":	"go south",
    "u":	"go up",
    "d":	"go down",
    "nu":	"go northup",
    "eu":	"go eastup",
    "wu":	"go westup",
    "su":	"go southup",
    "nd":	"go northdown",
    "ed":	"go eastdown",
    "wd":	"go westdown",
    "sd":	"go southdown",
    "ne":	"go northeast",
    "se":	"go southeast",
    "nw":	"go northwest",
    "sw":	"go southwest",
    "i":	"inventory",
    "sc":	"score",
    "k":	"kill",
    "sk":	"skill",
    "ver":	"version",
    "h":	"history",
    "hp":	"score s",
    "est":	"estate",
    "man":	"help",
    "reply":	"tell -r",
]);

string default_alias(string verb)
{
	return default_aliases[verb];
}
mapping query_default_alias()
{
	return default_aliases;
}

private nosave mapping std_commands;
private nosave mapping sub_commands;
private nosave mapping chn_commands;
private nosave mapping emt_commands;

private nosave mapping standard_commands_path =
([
STANDARD	:	"std/npc",
PLAYER		:	"std/ppl",
GUEST		:	"std/guest",
WIZARD		:	"std/wiz",
ADVISOR		:	"std/adv",
ADMIN		:	"std/adm",
]);

/* ���s��z�Ҧ����O�� */
private nomask void reset_commands()
{
	int level;
	object ob;
	function fp;
	string verb, dir, *dirarr;

	std_commands = allocate_mapping(7);
	
	std_commands[STANDARD] 	= allocate_mapping(0);
	std_commands[PLAYER] 	= allocate_mapping(0);
	std_commands[GUEST] 	= allocate_mapping(0);
	std_commands[WIZARD] 	= allocate_mapping(0);
	std_commands[ADVISOR] 	= allocate_mapping(0);
	std_commands[ADMIN] 	= allocate_mapping(0);

	sub_commands = allocate_mapping(0);
	chn_commands = allocate_mapping(0);
	emt_commands = allocate_mapping(0);
	
	/* �����O����z */
	foreach( verb, function emt_fp in EMOTE_D->query_emotions_fpointer() )
	{
		emt_commands[verb] = emt_fp;
		
		foreach( level, mapping data in std_commands )
			if( level > STANDARD ) std_commands[level][verb] = emt_fp;
	}
				
	/* �W�D���O����z */
	foreach( verb, array fp_data in CHANNEL_D->query_channels_fpointer() )
	{
		chn_commands[verb] = fp_data[FPOINTER];
		
		foreach( level, mapping data in std_commands )
			if( level >= fp_data[LEVEL] ) std_commands[level][verb] = fp_data[FPOINTER];
	}

			
	/* �зǧŮv���O�P���a���O����z */
	foreach( int dir_level, dir in standard_commands_path )
	foreach( verb in get_dir(COMMAND_ROOT+dir+"/") )
	{
		if( verb[<2..<1] != ".c" ) 
			continue;
		else
			verb = verb[0..<3];
			
		if( catch( ob = load_object(COMMAND_ROOT+dir+"/"+verb+".c") ) )
		{
			tell(this_player(), verb+" �sĶ�P���J����\n");
			continue;
		}
		
		if( !(fp = ob->query_fp()) ) continue;

		foreach( level, mapping data in std_commands )
			if( level >= dir_level ) std_commands[level][verb] = fp;
	}
	
	dirarr = get_dir(COMMAND_ROOT) - ({"std"});


	/* �S��l���O���Τ@��z */
	while(sizeof(dirarr))
	{
		dir = dirarr[0];
		
		if( file_size(COMMAND_ROOT+dir) == -2 )
		{
			sub_commands[dir] = allocate_mapping(0);
			
			foreach( verb in get_dir(COMMAND_ROOT+dir+"/") )
			{
				if( file_size(COMMAND_ROOT+dir+"/"+verb) == -2 )
				{
					dirarr += ({ dir+"/"+verb });
					continue;
				}
	
				if( verb[<2..<1] != ".c" ) 
					continue;
				else
					verb = verb[0..<3];
					
				if( catch( ob = load_object(COMMAND_ROOT+dir+"/"+verb+".c") ) )
				{
					tell(this_player(), verb+" �sĶ�P���J����\n");
					continue;
				}
				
				if( !(fp = ob->query_fp()) ) continue;
		
				sub_commands[dir][verb] = fp;
			}
			
			if( !sizeof(sub_commands[dir]) )
				map_delete(sub_commands, dir);
		}
		dirarr = dirarr[1..];
	}
}

nomask mapping query_commands()
{
	return 
	([
		"wiz": keys(std_commands[ADMIN])-keys(std_commands[PLAYER])-keys(chn_commands),
		"ppl": keys(std_commands[PLAYER])-keys(chn_commands)-keys(emt_commands),
		"chn": keys(chn_commands),
		"emt": keys(emt_commands),
		"sub": copy(sub_commands),
	]);
}

nomask int do_reset()
{
	reset_commands();
	return 1;
}

/* �M��Ův���O���� */
nomask function find_wiz_command(string verb)
{
	object me = this_player(1);
	object prev = previous_object();

	if( !objectp(me) || !interactive(me) || (!interactive(prev) && !is_command(prev)) ) return 0;

	return std_commands[me->query_commands_authority()||PLAYER][verb];
}

/* �M�䪱�a���O���� */
nomask function find_ppl_command(string verb)
{
	return std_commands[PLAYER][verb];
}

/* �M��NPC���O���� */
nomask function find_npc_command(string verb)
{
	return std_commands[STANDARD][verb] || chn_commands[verb] || emt_commands[verb];
}

string* query_available_command(int level)
{
	return keys(std_commands[level]);	
}

object find_command_object(string verb)
{
	return function_owner(std_commands[ADMIN][verb]);
}

/* �M��S����O���� */
nomask function find_sub_command(string dir, string verb)
{
	mapping commands;
	
	if( !mapp(commands = sub_commands[dir]) ) return 0;
	
	return commands[verb];
}

/* �M���W�D���O���� */
nomask function find_chn_command(string verb)
{
	return chn_commands[verb];
}

/* �M������O���� */
nomask function find_emt_command(string verb)
{
	return emt_commands[verb];
}


private void create() 
{
	if( clonep() ) 
		destruct(this_object());

	reset_commands();
}
string query_name()
{
	return "���O�t��(COMMAND_D)";
}
