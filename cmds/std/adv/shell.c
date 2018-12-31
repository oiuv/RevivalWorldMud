/*
  [A      Move cursor up one row, stop if a top of screen
  [B      Move cursor down one row, stop if at bottom of screen
  [C      Move cursor forward one column, stop if at right edge of screen
  [D      Move cursor backward one column, stop if at left edge of screen
  [H      Home to row 1 column 1 (also [1;1H)
  [J      Clear from current position to bottom of screen
  [K      Clear from current position to end of line
  [24;80H Position to line 24 column 80 (any line 1 to 24, any column 1 to 132)
  [0m     Clear attributes to normal characters
  [7m     Add the inverse video attribute to succeeding characters
  [0;7m   Set character attributes to inverse video only
*/

#include <daemon.h>
#include <feature.h>
#include <ansi.h>

//#define ESC	"\e"

#define CURSOR_UP	ESC"[A"
#define CURSOR_DOWN	ESC"[B"
#define CURSOR_RIGHT	ESC"[C"
#define CURSOR_LEFT	ESC"[D"
#define BACKSPACE	ESC"U"
//#define CLR		ESC"[2J"	// [J      Clear from current position to bottom of screen

inherit COMMAND;

string help = @HELP
        �з� realgo ���O�C
HELP;

nosave string msg_buffer = "";
void command_shell(string arg, object me, int h, int w);

string cursor_home(int h, int w)
{
	return sprintf(CSI"%d;%dH", h, w);
}
void print_command(object me, string arg, int h, int w)
{
	arg = replace_string(arg, "\e", "");
	tell(me, sprintf("%s"HBMAG"%-" + w + "s"NOR"%s",
		cursor_home(h, 0),
		arg,
		cursor_home(h-1, 0),
		));
}

void command(object me, string arg)
{
	int h = query_temp("windowsize/height", me);
	int w = query_temp("windowsize/width", me);

	tell(me, CLR"�i�J Command Shell (���}�Ы� 'q')\n");
	
	print_command(me, "", h, w);
	get_char((: command_shell :), 0, me, h, w);
}
/*
get_char:
�r���Ҧ��A���o�ϥΪ̿�J���C�Ӧr���A�C�Ӧr���ҷ|�Ǧp���w���禡��
���ѼƲĤ@�Ӧr��

*/

void command_shell(string arg, object me, int h, int w)
{
	if( !arg ) arg = "";
	/*if( strlen(arg) >= 2 && arg[0..1] == BACKSPACE )
		msg_buffer = msg_buffer[0..<2];
	else*/
		msg_buffer += sprintf("%c", arg[0]); // �N��J���C�Ӧr���s�J�w�İϤ�

	if( msg_buffer[<1] == '\n' ) // �P�_��J���T���O�_�t������r��
	{
		string msg;

		msg = msg_buffer[0..<2];
		if( msg[<1] == '\r' ) msg = msg[0..<2];
		// �P�_��J���R�O
		switch(msg)
		{
		case "q":
			tell(me, "\n���} Command Shell�C\n");
			return;	
		default:
			tell(me, msg + "\n");
			me->force_me(msg);
		}
		msg_buffer = "";
	}
/*
	if( msg_buffer == CURSOR_UP || msg_buffer == CURSOR_DOWN  )
	{
		msg_buffer = "�O�d�\����";
	}
*/
	print_command(me, msg_buffer, h, w);
	get_char((: command_shell :), 0, me, h, w);
}                                       