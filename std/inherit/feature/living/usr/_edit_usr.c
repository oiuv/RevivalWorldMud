/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _edit.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-7
 * Note   : �s��t��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>

#define BEGIN	sprintf(NOR"�~�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"WHT"["HIW"%18|s"NOR WHT"]"NOR"��\n", "���e Content")
#define END	sprintf(NOR"���w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"WHT"["HIW"%18|s"NOR WHT"]"NOR"��\n", "���� End")

void input_line(string arg, string *text, function f)
{
	int prompt;
	arg = ansi(arg);
	
	if( arg && query("encode/gb") ) 
		arg = G2B(arg);

	prompt = query("env/edit_prompt"); 
	
	switch(arg)
	{
		case ".":
			tell(this_object(), "\n" + END);
			evaluate(f, implode(text, "\n") + NOR);
			break;
		case "~q":
			tell(this_object(), "\n" + END + "�����s��C\n");
			this_object()->finish_input();
			break;
		case "~b":
			if( sizeof(text) )
			{
				tell(this_object(), "\n" + END + "\n �R���W�@���r�T��: " + text[<1] + "\n");
				text = text[0..<2];
				tell(this_object(), BEGIN + implode(text,"\n"));
				if( prompt ) tell(this_object(), "\n> ");
				input_to((: input_line :), 2, text, f);
				return;
			}
			tell(this_object(), "\n" + END + "�����s��C\n");
			this_object()->finish_input();
			break;
		case "~p":
			tell(this_object(), "\n" + END + "\n �z�w��J����r�T���p�U: \n");
			tell(this_object(), BEGIN + implode(text,"\n"));
			if( prompt ) tell(this_object(), "\n> ");
			input_to((: input_line :), 2, text, f);
			return;
		case "~h":
			tell(this_object(), @HELP

------------------------------------------
- Revival World ��r�s�边 �ϥλ��� -

�b�s���@�椤��J
'.' <�y��>	������r�s��
~q		������r�s��
~p		��ܥثe�w��J��r
~b		�R���W�@���r
~h		�ϥλ���
-------------------------------------------
HELP
			);
			if( prompt ) tell(this_object(), "> ");
			input_to((: input_line :), 2, text, f);
			return;
		default:
			text += ({ arg });
			if( prompt ) tell(this_object(), "\n" + arg + "\n> ");
			input_to((: input_line :), 2, text, f);
			return;
	}
}

void edit(function f, string text)
{
	string bar;
	bar  = "\n �w "HIW"��r�s�边"NOR" �w ��J '"HIW"."NOR"' ���� �w "HIW"~q"NOR" ���� �w "HIW"~h"NOR" ���� �w\n";
	bar += BEGIN;
	bar += text || "";

	tell(this_object(), bar);
	
	if( query("env/edit_prompt") ) 
		tell(this_object(), "> ");
	
	input_to((: input_line :), 2, text ? ({text}) : ({}), f);
}
