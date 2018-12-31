/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_9.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : �зǩж�
 * Update :
 *  2006-09-07 �E�� by Tzj@RevivalWorld
 *  
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <quest.h>

inherit STANDARD_ROOM;

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;



void create()
{
	set("short", YEL"����"NOR"��"NOR);
	set("long",@TEXT
    �o�̬O�@���ﺡ�U���������ж��A���M�F��ܦh�A���Ҧ������~���ٺ⤫�M��
�ǡA�o�̪��D�H���ӬO�ӲӤߪ��H�A�����]���ٺ⥴�����Q�����b�A�ä����O�@��
�j�k�H�~���a��C
TEXT);

	set("exits", ([
		"south"		: "/quest/yin_ling_yu/room_3",
	]));
	
	::reset_objects();
}
