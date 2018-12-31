/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : title_screen.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-7-2
 * Note   : included by Simul_Efun Object
 * Update :
 *  o 2005-02-28 Clode ���s�ק�

 -----------------------------------------
 */

#include <ansi.h>

varargs void tell(object target, string msg, string classes);

void startup_title_screen(object me, int line)
{
	int height = query_temp("windowsize/height", me);
	
	line += 2;
	
	if( line < 1 ) return;

	tell(me, repeat_string("\n", line)+CLR+FREEZE(line, height));
       
	set_temp("title_screen", line, me);
}

void spec_line_msg(object me, string msg, int line)
{
	if( line <= 0 ) return;

	tell(me, SAVEC+MOVEC(line, 1) + msg+REST);
        //tell(me, "\e[s\e["+line+";1H"+msg+"\e[u");
}

void reset_screen(object me)
{
	int line = query_temp("title_screen", me);
	
	while(line--) spec_line_msg(me, repeat_string(" ",80), line);
	
	tell(me, REST SAVEC UNFR REST);

	delete_temp("title_screen", me);
}

varargs void clear_screen(object me, int specline)
{
	int line = query_temp("title_screen", me);
	int width = query_temp("windowsize/width", me);

	if( specline )
		spec_line_msg(me, repeat_string(" ",width), specline);
	else while(line--)
		spec_line_msg(me, repeat_string(" ",width), line);
}
