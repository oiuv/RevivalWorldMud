/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_2.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-18
 * Note   : �зǩж�
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_ROOM;

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;

void create()
{
	set("short", WHT"�}�Z�����ж�"NOR);
	set("long",@TEXT
    �o�̬O�}�Z�����ж��A��B�\���F�U�ت��֮�P���G�A�q�ж��̦U�زӤ��u��
���\�]�ݨӡA���G���O�@�ӹA�ҸӦ����\�]�A�i�H�ݥX�o�����Ӧ��g�L�k�Ĥl����
��J�Ӹ��C�L�C
TEXT);

	set("exits", ([
		"east" : "/quest/yin_ling_yu/room_1",
	]));
	
	::reset_objects();
}


void do_search(object me, string arg)
{
	if( !arg || arg=="" )
		return tell(me, pnoun(2, me)+"�Q�n�j�M����H\n");

	if( strsrch(arg, "���G") != -1 )
	{
		msg("$ME�չϦb���G�W��X�@�ǽu���A���Q�����̤F�@���A�����a�L�b�a�W�C\n", me, 0, 1);
		me->faint();
	}
	else if( strsrch(arg, "�֮�") != -1 )
	{
		msg("$ME�չϦb�֮�W��X�@�ǽu���A�����p�߸I�F�@�U�N�֮�L���A�����a�{�b$ME���}�W�C\n");
		me->faint();
	}
	else if( strsrch(arg, "�\�]") != -1 || strsrch(arg, "���C") != -1 )
		tell(me, "�|�B��m�ۦU�إi�R�S�¾몺�p�����A���H�P��L�񪺷��ɡC\n");
	else if( strsrch(arg, "�k") != -1 )
		tell(me, "���M�����D�o��k�ĬO�֡A���o�@�w�O��ŬX�U�a���k�l�C\n");
	else
		tell(me, pnoun(2, me)+"�Q�n�j�M����H\n");	
}

mapping actions = 
([
	"search" : (: do_search :),
]);