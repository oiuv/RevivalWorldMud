/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_wizhall_3.c
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
	set("short", HIW"�Ův"NOR WHT"���� "NOR YEL"�F�_��"NOR);
	set("long",@TEXT
    �o�̪��|�P�\���F�j�Ѫ����y�A�x�����ȱi�W�Ѽg�۫e�H�b�o�ӥ@�ɤ��Ȧ檺
�I�I�w�w�A�]�A�U�ئ������㪺�C����ƻP�L�̪��߱o�P���A�o�إѵL�Ƹg��ֿn
�ӨӪ������_�w�O�L����...�C
TEXT);

	set("exits", ([
		"west" : "/wiz/wizhall/room_wizhall_4",
	    ]));

	set("objects", ([
		"/obj/etc/lotto":1,
	    ]));

	::reset_objects();
}
