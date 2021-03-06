/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : order.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : order 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        標準 parsing 指令。
HELP;

private int parse_script(string arg)
{
	int idx, chr, mark, marked;
	int s_symbol, m_symbol, b_symbol, d_quote, s_quote;
	foreach(string cmd in explode(arg, ";"))
	{
		for(idx=0; idx<strlen(cmd); idx++)
		{
			chr = cmd[idx];
			mark = (chr == '\\');
			if( mark ) marked++;
			if( !(s_quote%2) && (!marked || !(marked%2)) && chr == '\"' ) d_quote++;
			if( !(d_quote%2) && (!marked || !(marked%2)) && chr == '\'' ) s_quote++;
			if( !mark ) marked = 0;
			if( d_quote%2 ) continue;
			switch(chr)
			{
			case '(':       s_symbol++;     break;
			case ')':       s_symbol--;     break;
			case '[':       m_symbol++;     break;
			case ']':       m_symbol--;     break;
			case '{':       b_symbol++;     break;
			case '}':       b_symbol--;     break; 
			}
		}
	}
	if( d_quote%2 )
		printf("Prasing error: 雙引號不正常結束。\n");
	if( s_quote%2 )
		printf("Prasing error: 單引號不正常結束。\n");
	if( s_symbol )
		printf("Prasing error: () 括號必須成對 [%s了 %d 個右括號]。\n",
		    (s_symbol > 0 ? "少":"多"), to_int(abs(s_symbol)));
	if( m_symbol )
		printf("Prasing error: [] 括號必須成對 [%s了 %d 個右括號]。\n",
		    (m_symbol > 0 ? "少":"多"), to_int(abs(m_symbol)));
	if( b_symbol )
		printf("Prasing error: {} 括號必須成對 [%s了 %d 個右括號]。\n",
		    (b_symbol > 0 ? "少":"多"), to_int(abs(b_symbol)));
}
private void command(object me, string arg)
{
	if( !is_command() ) return;
	if( !arg )
		return tell(me, "指令格式：parsing <LPC Script>\n", CMDMSG);

	arg = replace_string(arg, "\n", "");
	if( arg[<1] != ';' ) arg += ";";
	
	parse_script(arg);
	
	tell(me, sprintf("分析結束。\n"), CMDMSG);
}
