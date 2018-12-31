/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : eval.c
 * Author : Sinji@RevivalWorld
 * Date   : 2001-9-14
 * Note   : eval ���O
 * Update :
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

#define EVAL_FILE       "eval.c"
#define COST_TRIM       -8
#define EXPT_TRIM       -2
inherit COMMAND;

string help = @HELP
        �з� eval ���O�C
HELP;

private int parse_script(string arg);
private void command(object me, string arg)
{
	string lines, path, file, err;
	object eval_ob;

	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "���O�榡�Geval <LPC Script>\n");

	arg = replace_string(arg, "\n", "");
	if( arg[<1] != ';' ) arg += ";";
	parse_script(arg);

	/* Following is content of eval file */
	lines = "// This file written by eval command. \n";
	lines += "#define COST_TRIM     " + COST_TRIM + "\n";
	lines += "#define EXPT_TRIM     " + EXPT_TRIM + "\n";
	lines += @FILE
#include <ansi.h>
#include <daemon.h>
#include <object.h>
#include <feature.h>
#include <function.h>
#include <eval.h>
#include <message.h>
#include <runtime_config.h>
#include <quest.h>

void create()
{
        mapping sinfo, einfo;
        int cost, time_exp; 
        float utime, stime, etime;
        string msg;
        object me = this_player();

        sinfo = rusage();
        cost = eval_cost();
        time_exp = time_expression {
FILE + arg + @FILE

        };
        einfo = rusage();
        cost -= eval_cost();

        utime = to_float(einfo["utime"] - sinfo["utime"])/1000000;
        stime = to_float(einfo["stime"] - sinfo["stime"])/1000000;
        etime = to_float(time_exp + EXPT_TRIM)/1000000;

        msg  = sprintf("\n�Ĳv����: %d\n", cost + COST_TRIM);
        msg += sprintf("�t�ήɶ�: %.6f s\n", stime);
        msg += sprintf("�ϥήɶ�: %.6f s\n", utime);
        msg += sprintf("�B��ɶ�: %.6f s\n", etime);
        
        tell(me, msg);
}
FILE;
	/* Above is content of eval file */
	path = wiz_home(me->query_id(1));
	file = sprintf("%s/%s", path, EVAL_FILE);
	
	if( file_size(path) != -2 )
		return tell(me, "�S�� "+path+" �o�ӥؿ��C\n�����e�Х��إߦ��ؿ��C\n");
	
	if( eval_ob = find_object(file) ) 
		destruct(eval_ob);
	
	write_file(file, lines, 1);
	
	if( !(err = catch(load_object(file))) )
	{
		tell(me, "�sĶ���\�C\n");
                rm(file);

		if( eval_ob = find_object(file) ) 
			destruct(eval_ob);
	} 
	else
		tell(me, "�sĶ���ѡC\n");
}

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
		printf("Prasing error: ���޸������`�����C\n");
	if( s_quote%2 )
		printf("Prasing error: ��޸������`�����C\n");
	if( s_symbol )
		printf("Prasing error: () �A���������� [%s�F %d �ӥk�A��]�C\n",
		    (s_symbol > 0 ? "��":"�h"), to_int(abs(s_symbol)));
	if( m_symbol )
		printf("Prasing error: [] �A���������� [%s�F %d �ӥk�A��]�C\n",
		    (m_symbol > 0 ? "��":"�h"), to_int(abs(m_symbol)));
	if( b_symbol )
		printf("Prasing error: {} �A���������� [%s�F %d �ӥk�A��]�C\n",
		    (b_symbol > 0 ? "��":"�h"), to_int(abs(b_symbol)));
}
