/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_wizhall_10.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-10
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
	set("short", HIW"�]�p"NOR WHT"��s��"NOR);
	set("long",@TEXT
    �]�p��s�ǤW���a���ۤ��j�ɥN��護�檺�����ݿO�A�Υ~�������ǥѤ��m
�}�ɪ������M�g�i�ӡC
TEXT);

	set("exits", ([
		"south" : "/wiz/wizhall/room_wizhall_9",
		"north" : "/wiz/wizhall/room_wizhall_11",
	]));
	
	set("objects", ([
		"/obj/board/class_design" : 1,
	]));
	
	::reset_objects();
}
