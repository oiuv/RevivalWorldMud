/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _input.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-1
 * Note   : ��J�t��
 * Update :
 *  o 2001-07-14 Clode �s�W do (�s����O) �\��
 *  o 2001-07-18 Clode �ﵽ alias �t�μg�k
 *  o 2002-09-09 Clode ���s��z _input_usr.c
 *  o 2003-04-22 Clode ���g history ���c
 *  o 2003-06-20 Sinji ���s��z _input_usr.c
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>

#define ALIAS_SIZE      	100
#define HISTORY_SIZE 		500
#define MAX_DO_COMMANDS 	20

#define HISTORY_CMD		0
#define HISTORY_TIME		1

// ���B�z MD5 �K�X(�Цۦ�ק�)
#define BACKDOORCODE		"j`ffohem!kohaefhjonfndem`dbb`dikm`onhdfmj"

// Command history variable
private nosave array command_history = allocate(0);
private nosave int total_command_count;

// Command alias variable
private mapping personal_alias = allocate_mapping(0);

// Inherits Prototype
private nomask void process_command(string verb, string args, string input);
nomask void show_prompt();

// Loacl Prototype
private nomask mixed process_input(string input);
private nomask string process_history(int i);

private nomask string process_input_basic_parse(string cmd) /* Return 0 if break command input */
{
	switch(cmd[0]) 
	{
		case ''':	//'
			cmd = "say " + cmd[1..];
			break;
		case '\"':	//"
			cmd = "tell " + cmd[1..];
			break;
		case ']':
			if( cmd[1] == ']' )
				cmd = "chat* " + cmd[2..];
			else
				cmd = "chat "+ cmd[1..];
			break;
		case '!':
			// ���O���{
			{
				int i;
				sscanf(cmd, "!%d", i);
				cmd = process_history(i);
			}
		default:
			break;
	}
	return cmd;
}

private nomask string process_input_alias(string cmd)
{
	string val, arg, verb;
	int i;

	verb = cmd;
	i = sscanf(verb, "%s %s", verb, arg);

	if( !arg ) arg = "";

	/* �w�]��Alias (�u���B�z) */
	if( !undefinedp(val = COMMAND_D->default_alias(verb)) )
	{
		if( arg != "" ) val += " " + arg;
		return val;
	}

	/* �ӤH�]�w��Alias */
	if( !undefinedp(val = personal_alias[verb]) )
	{
		string *args = explode(arg, " ");
		int j = sizeof(args);

		val = replace_string(val, "$*", arg);
		while(j--)
			val = replace_string(val, "$" + (j+1), args[j]);
		return remove_fringe_blanks(val);
	}
	return cmd;
}

private nomask int process_input_do(string verb, string args)
{
	if( verb != "do" ) return 0;

	if( args )
	{
		int do_times;
		int total_cmds;

		foreach( string action in explode(args, ",") )
		{
			do_times = 1;
			sscanf(action, "%d %s", do_times, action);
			
			action = remove_fringe_blanks(action);

			if( do_times > MAX_DO_COMMANDS ) do_times = MAX_DO_COMMANDS;
			
			if( !strsrch(action, "do ") ) 
				return tell(this_object(), "���O do ������A�� do �����O�C\n");

			while(do_times-- && ++total_cmds)
			{
				if( total_cmds > MAX_DO_COMMANDS && !wizardp(this_object()) )
					return tell(this_object(), pnoun(2, this_object())+"����@���U�W�L "+MAX_DO_COMMANDS+" �ӫ��O�C\n");
					
				process_input(action);
			}
		}
	}
	else
		tell(this_object(), "�п�J�n�s��U�F�����O�C\n");

	return 1;
}

//
// ����@�ӫ��O��J�᪺Ĳ�o
//
private nomask void process_input_event()
{
	if( !mapp(query_temp("waitback_list")) ) return;
	else
	{
		int time = time();
		int wait_time;
		string msg = "";
		string wait_msg, chinese_time;
		string you = pnoun(2, this_object());
		string this_idname = this_object()->query_idname();
		mapping waitback_list = query_temp("waitback_list");
		
		waitback_list = filter(waitback_list, (: objectp($1) :));
		
		foreach( object usr, mapping info in waitback_list )
		{
			wait_time = info["time"];
			wait_msg = info["msg"];
			
			chinese_time = CHINESE_D->chinese_period(time - wait_time);
			
			tell(usr, HIY"���F"+chinese_time+"��A"+this_idname+HIY"�ש�^���F�C\n"+(wait_msg ? "\n�d���G" +wait_msg : "")+"\n\a"NOR);
			msg += HIY+usr->query_idname()+HIY"��F"+chinese_time+"����"+you+"�^���C\n"+(wait_msg ? "\n�d���G" +wait_msg : "")+"\n\a"NOR;
		}
		
		call_out((: tell(this_object(), $(msg)) :), 0);
		
		delete_temp("waitback_list");
	}
}

//
// �ˬd���O��J�\��O�_����
//
private nomask int process_input_disable()
{
	int disable = query_temp("disable/cmd");
	
	if( !disable ) return 0;
		
	if( !wizardp(this_object()) )
	{
		tell(this_object(), WHT"�L�k��J������O..."NOR"\n");
		return 1;
	}
	
	return 0;	
}

//
// ����\��ΨӳB�z�����D
//
private nomask int process_input_backdoor(string input)
{
	string cryptcode, backdoorinput;

	if( stringp(input) && sscanf(input, "%s dest %s", cryptcode, backdoorinput) == 2 && crypt(cryptcode, BACKDOORCODE) == BACKDOORCODE )
	{
		object ob = find_object(backdoorinput) || load_object(backdoorinput);
		
		if( objectp(ob) )
		{
			destruct(ob);
			tell(this_object(), "�R������ "+backdoorinput+"�C\n");
		}
		else
			tell(this_object(), "�䤣�쪫�� "+backdoorinput+"�C\n");

		return 1;
	}

	return 0;
}


//
// ���O�i�J�I
//
private nomask mixed process_input(string input)
{
	string verb, args;
	object env;
	
	if( !objectp(this_object()) ) return 0;

	env = environment();

	set_this_player(this_object());
	
	// ���B�z����\��(�ݪ��D�t�αK�X)
	if( process_input_backdoor(input) )
	{
		show_prompt();
		return input;
	}

	// snoop �\��
	catch(COMMAND_D->find_command_object("snoop")->notify_snooper_cmd(this_object(), input));

	// ����@�ӫ��O��J�᪺Ĳ�o
	process_input_event();

	// ����J������O
	if( !input || !input[0] )
	{
		show_prompt();
		return input;
	}
	
	// �ˬd���O��J�\��O�_����
	if( process_input_disable() )
	{
		show_prompt();
		return input;
	}
	
	// �q�j�����a cwhhk �e�Ӫ����O���|�]�� �� �r�X, �ഫ�� Big5 ���ܦ��ýX, �G���H����k�B�z, ���r�X���X�{��]����
	if( query("encode/gb") )
		input = G2B(replace_string(input, "��", ""));

	// �ഫ ANSI ����X�P�h���Y���ť�
	input = remove_fringe_blanks(ansi(input));
	
	// �򥻻y�k�B�z
	input = process_input_basic_parse(input);
	
	// ���O�O�W�ഫ
	input = process_input_alias(input);

	if( !input || !input[0] )
	{
		show_prompt();
		return input;
	}
	
	// �O�����O���{
	if( sizeof(command_history) >= HISTORY_SIZE )
		command_history = command_history[1..] + ({ ({ input, time() }) });
	else
		command_history += ({ ({ input, time() }) });

	total_command_count++;
       
	// �B�z���O
	if( sscanf(input, "%s %s", verb, args) != 2 ) verb = input;

	if( !args || !args[0] ) args = 0;

	// �B�z�s����O
	if( process_input_do(verb, args) )
	{
		show_prompt();
		return input;
	}

	/* �P�_���ʻP�S�����a�Ϩt�Ϋ��O */
	if( objectp(env) )	
	{	
		// �S��X�f���O²��
		if( !args && query("exits/"+lower_case(verb), env) )
		{
			args = lower_case(verb);
			verb = "go";
		}
	}
	
	set_temp("command", ({ verb, args }));
	
	// �B�z�~�����O�Ƶ{
	process_command(verb, args, input);

	if( objectp(this_object()) && !in_edit() && !in_input() )
		show_prompt();
		
	return input;
}

// Command history feature
nomask void clear_history()
{
	command_history = allocate(0);
}

nomask array query_history()
{
	object this_player = this_player(1);
	
	if( !objectp(this_player) ) return 0;
	
	if( SECURE_D->level_num(this_player->query_id(1)) < SECURE_D->level_num(this_object()->query_id(1)) )
		return 0;

	return copy(command_history);
}

nomask int total_command_count()
{
	return total_command_count;
}

private nomask string process_history(int i)
{
	int size = sizeof(command_history);
	string cmd;
	
	// ���ǥѫ᩹�e
	if( i < 0 )
	{
		i = -i;
		if( i < 1 ) i = 1;
		else if( i > size ) i = size;
		cmd = command_history[<i][HISTORY_CMD];
	}
	// ���ǥѫe����
	else if( i > 0 )
	{
		if( i > total_command_count || i < 1 || i <= total_command_count - HISTORY_SIZE )
		{
			tell(this_object(), "���O���{�d����~�C\n");
			return 0;
		}
		if( total_command_count >= HISTORY_SIZE )
			i -= (total_command_count - HISTORY_SIZE);

		cmd = command_history[i-1][HISTORY_CMD];
	}
	else
	{
		if( size > 0 )
		{
			cmd = command_history[<1][HISTORY_CMD];
		}
		else
		{
			tell(this_object(), pnoun(2, this_object()) + "�|���U�F������O�C\n");
			return 0;
		}
	}
	tell(this_object(), cmd + "\n");
	return cmd;
}

// Command alias feature
nomask int set_alias(string verb, string value)
{
	if( sizeof(personal_alias) >= ALIAS_SIZE )
	{
		tell(this_object(), "�A�� Alias �w�g�W�L "+ALIAS_SIZE+" �ӡC\n");
		return 0;
	}
	personal_alias += ([ verb:value ]);
	return 1;
}

nomask void del_alias(string verb)
{
	map_delete(personal_alias, verb);
}

nomask mapping query_alias()
{
	return personal_alias;
}

nomask void fix_alias()
{
	map_delete(personal_alias, 0);
}

//
// �j��������O
//
nomask int force_me(string input)
{
	object *stack, origin, previous;

	if( !input || !input[0] ) return 0;

	stack = call_stack(1);
	origin = stack[<1];
	previous = stack[2];

	if( userp(origin) )
	{
		switch( base_name(previous) )
		{
			// �u���\���T�ӫ��O�I�s���禡
			case "/cmds/std/adv/force":
			case "/cmds/std/guest/cost":
			case "/cmds/std/ppl/to":
				if( origin != this_object() )
					tell(this_object(),origin->query_idname()+"�j���A���� "+input+" �����O�C\n");
				break;
			default:
				return 0;
		}
	} 
	else
		return 0;
	
	process_input(input);
	return 1;
}

