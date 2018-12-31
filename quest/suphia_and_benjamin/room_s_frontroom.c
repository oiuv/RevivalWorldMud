/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_s_frontroom.c
 * Author : tzj@RevivalWorld
 * Date   : 2006-09-09
 * Note   : Ĭ��Ȯa�����U
 * Update :
 *  
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
	set("short", HIW"���U"NOR);
	set("long",@TEXT
�@�@�o�̥|�g�������ܰ��b�A�Ф�������ۥO�H�P��ŷx���H�H�Z����A�z�L��
��i�M���ݨ��~�������C�b�ж���������m�ۭ��E�۶H�����\�y���\��Υ|�i
��s�\�ȡA�Ӧb�������h��m�ۤ@�i�E���n�Ԫ��ξA���ȡA�ѳo�̦��G�i�H��
��Τl���t�@���O�p�СC
TEXT);


	set("exits", ([
		//���F���OĬ��ȥL�̪��ж�
		"east"  : "/quest/suphia_and_benjamin/room_s_alvin_room",
		
		//���n���O���f
		"south"  : ({ 80, 68, 0, "lightforest", 0, 0, "80/68/0/lightforest/0/0" }),
		
		//���訫�O��� = ="
		//"west"  : ,
		
		//���_���O�p��
		"north"   : "/quest/suphia_and_benjamin/room_s_kitchen",
		
	]));

	set("lock/east", 1);
	set("objects", ([
		"/quest/suphia_and_benjamin/npc_suphia" : 1 ,
	]));


	::reset_objects();
	replace_program(STANDARD_ROOM);
}
