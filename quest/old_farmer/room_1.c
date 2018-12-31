/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_1.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-19
 * Note   : �зǩж�
 * Update :
 *  2006-09-07 �E�� by Tzj@RevivalWorld
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
	set("short", GRN"�A��"NOR);
	set("long",@TEXT
    �o���Τl�ݰ_�ӤQ�����¡A��s������g�L���j�B���w�g�����ϥΡC�Τ����|
�P�\��ۦU���U�˪��A�Ѥu��A���ݰ_�Ӧ��G�w�g�n�@�}�l�S�ϥΤF�C�b�������
�n�X�u�Ŧ̳U�A�u���@��w�g�~�ڪ��ѹA�ү��b���������a�y�¡A���G�S�֤S�j��
�ˤl�C
TEXT);

	set("exits", ([
		//��]�w�X�f�G"east" 	: ({ 50, 50, 0, "lightforest", 0, 0, "50/50/0/lightforest/0/0" }),
		//��]�w�X�f�G"south"	: "/quest/old_farmer/room_2",
		//�өж����F������ܮy�ЬO51,51�Acode�]�w�O50,50�C
		
		//�H�U��`06-09-07�A�� tzj �E����ܧ󪺦�m�C
		"south" : ({ 74, 41, 0, "lightforest", 0, 0, "74/41/0/lightforest/0/0" }),
		"east"  : "/quest/old_farmer/room_2",
	]));
	
	set("objects", ([
		"/quest/old_farmer/npc_old_farmer" : 1 ,
	]));
	
	::reset_objects();
	replace_program(STANDARD_ROOM);
}
