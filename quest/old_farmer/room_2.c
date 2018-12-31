/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_2.c
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
    �o�̬O�A�ٸ̪��p�׫ǡA�a�W�Q�ۤ@�j������V�A���ӴN�O�ѹA�ҥΨӷ�@��
�E�Ϊ��A�쵡�H�۷L�������@�T�A�t�H���ؿ��M���Pı�C
TEXT);

	set("exits", ([
		//��]�w�X�f�G"north" : "/quest/old_farmer/room_1",
		
		//�H�U��`06-09-07�A�� tzj �E����ܧ󪺦�m�C
		"west"  : "/quest/old_farmer/room_1",
		"north" : ({ 75, 39, 0, "lightforest", 0, 0, "75/39/0/lightforest/0/0" }),
	]));
	
	::reset_objects();
	replace_program(STANDARD_ROOM);
}
