/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_wizhall_17.c
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
	set("short", HIW"�Ův"NOR WHT"�q�D"NOR);
	set("long",@TEXT
�@�@�Ův�q�D�q���U�ӧŮv���������������A�u���C�Ӥ�V��L�h���O�`��������
���f�A�q�D���ⰼ���z�W���ۦU���U�˪��J��@�~�Pø�e�A�������L�k�z�Ѩ�@�~
���t�N�A���H�b�o�ӳq�D�ਤ�Pı��@�ѯ������𮧡C
TEXT);

	set("exits", ([
		"down" : "/wiz/wizhall/room_wizhall_7",
		"up" : "/wiz/wizhall/room_wizhall_8",
		"north" : "/wiz/wizhall/room_wizhall_9",
	]));
	
	::reset_objects();
}
