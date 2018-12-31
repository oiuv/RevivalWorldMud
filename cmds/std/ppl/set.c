/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : set.c
 * Author : Cwlin@RevivalWorld
 * Date   : 2003-06-07
 * Note   : 
 * Update :
 *  o 2003-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <type.h>

inherit COMMAND;

#define VAR_TYPE        0
#define VAR_REGEXP      1
#define VAR_LEVEL	2
#define VAR_COMMENT     3

// �Ѽƫ��A
#define TYPE_BOOLEAN	0
#define	TYPE_INTEGER	1
#define TYPE_STRING	2

// �ϥ��v��
#define LV_PLAYER	0
#define LV_WIZARD	1

string help = @HELP
�]�w�����ܼ�

�o�ӫ��O�Ψӳ]�w�����ܼơA�i���ϥΪ̦ۥѽվ���ߦn�B�ߺD���ާ@���ҡC

���O�榡:
set			��ܩҦ��i�ѧ�諸�����ܼƻP�]�w��
set -l			��ܩҦ��i�ѧ�諸�����ܼƻP����
set <�ܼ�> <��>		�N�����ܼ� <�ܼ�> ���ȧ�אּ <��>
set -d <�ܼ�>		�N�����ܼ� <�ܼ�> ��אּ�t�ιw�]��

�������O: chfn, alias,
HELP;
private nosave mapping ENV_VARS = 
([
//	�ܼƦW��	: ({ �Ѽƫ��A, �ѼƮ榡(���W��ܪk), �v��, ���� }),
	"no_map"	: ({ TYPE_BOOLEAN, "[0-1]", 	LV_PLAYER, "�}���Y�ɦa����� (0: �}��, 1: ����)" }),
	"rumor"		: ({ TYPE_INTEGER, "[0-2]", 	LV_PLAYER, "�����W�D�ΦW�Ҧ� (0: �ΦW, 1: �_�W, 2: ���a�s��)" }),
	"invis"		: ({ TYPE_BOOLEAN, "[0-1]", 	LV_WIZARD, "���μҦ� (0: �X�{, 1: ����)" }),
	"no_weather"	: ({ TYPE_BOOLEAN, "[0-1]", 	LV_WIZARD, "�����Ѯ�v�T (0: ���v�T, 1: �����v�T)" }),
        "timezone"	: ({ TYPE_STRING,  "[a-zA-Z]", 	LV_PLAYER, "�ɰ�" }),
        "edit_prompt"	: ({ TYPE_BOOLEAN, "[0-1]", 	LV_PLAYER, "�ϥΤ�r�s�边����ܩR�O���ܦr�� (0: �����, 1: ���)" }),
        "no_player_city": ({ TYPE_BOOLEAN, "[0-1]", 	LV_PLAYER, "���a�W�٤��e�������W����� (0: ���, 1:�����)" }),
 //       "no_id_color"	: ({ TYPE_BOOLEAN, "[0-1]",	LV_PLAYER, "����W�٤��m�����(0: �m��, 1: �¥�)" }),
 //       "no_color"	: ({ TYPE_BOOLEAN, "[0-1]",	LV_PLAYER, "�Ҧ���r���m�����(0: �m��, 1: �¥�)" }),
]);

private void command(object me, string arg)
{
	mixed val;
	string var, msg;
	mapping myvars;
	array var_data;

	if( !is_command() ) return;

	if( !myvars = query("env", me) ) myvars = allocate_mapping(0);

	// ��ܥثe���]�w
	if( !arg || arg == "-l" )
	{
		msg = pnoun(2, me) + "�ثe���\�]�w�������ܼƦp�U�G\n";
		foreach(var, var_data in ENV_VARS)
		{
			if( var_data[VAR_LEVEL] == LV_WIZARD && !wizardp(me) ) continue;

			if( arg != "-l" )
			{
				val = myvars[var];
				if( var_data[VAR_TYPE] == TYPE_BOOLEAN ) val = (val ? "�O":"�_");
				if( var_data[VAR_TYPE] == TYPE_STRING ) val = (val ? val:"���]�w");
			}
			else val = var_data[VAR_COMMENT];
			
			msg += sprintf(HIW BLK" ["HIW"%-|16s"BLK"]"NOR"\t\t -> " + val + "\n", var);
		}
		return tell(me, msg);
	}

	if( sscanf(arg, "%s %s", var, val) != 2 )
		return tell(me, "���O�榡���~�A�аѦ� help set �H���o��h���ϥθ�T�C\n");

	if( var == "-d" )
	{
		var = val;
		val = "0";
	}

	if( undefinedp(var_data = ENV_VARS[var]) )
		return tell(me, "�S�� " + var + " �o�������ܼơC\n");

	if( var_data[VAR_LEVEL] == LV_WIZARD && !wizardp(me) )
		return tell(me, "�S�� " + var + " �o�������ܼơC\n");

	if( val != "0" && !regexp(val, var_data[VAR_REGEXP]) )
		return tell(me, var + " �����ܼƤ����\�ϥ� " + val + " �o�ӭȡC\n" );

	switch(var_data[VAR_TYPE])
	{
		case TYPE_BOOLEAN:
			val = to_int(val);
			val = (val ? 1:0);
			if( !val ) map_delete(myvars, var);
			else myvars[var] = val;
			val = (val ? "�O":"�_");
			break;
		case TYPE_INTEGER:
			val = to_int(val);
			if( !val ) map_delete(myvars, var);
			else myvars[var] = val;
			break;
		case TYPE_STRING:
			val = sprintf("%s", val);
			if( val == "0" ) map_delete(myvars, var);
			else myvars[var] = val;
			break;
		default:break;
	}

	if( !sizeof(myvars) ) myvars = 0;
	set("env", myvars, me);

	return tell(me, "�����ܼ� " + var + " = " + val + " �]�w�����C\n");
}
 
