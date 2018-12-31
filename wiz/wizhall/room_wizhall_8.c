/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_wizhall_8.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-23
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
	set("short", HIW"�Ův"NOR WHT"�|ĳ��"NOR);
	set("long",@TEXT
�@�@�Ův�|ĳ�U���ⰼ����a�ƺ��ۥ��j�۴ȡA�\�h�Ův�����b�W�����۰Q�׵ۭ�
�jĳ�D�A�P�ɤ]�O���Ův�̲�ѻE�|���a��A�o�̪��|�g��¶�۳\�h���j���A�q��
�ۦU�ӧŮv���𮧳B�C
TEXT);

	set("exits", ([
		"down" : "/wiz/wizhall/room_wizhall_17",
		"sinji" : "/wiz/home/sinji/workroom",
                "cookys" : "/wiz/home/cookys/workroom",
                "abcd" : "/wiz/home/abcd/workroom",
		"ekac" : "/wiz/home/ekac/workroom",
	]));
	
	set("objects", ([
		"/obj/board/meeting" : 1,
	]));
	
	::reset_objects();
}
