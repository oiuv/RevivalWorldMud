/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_wizhall_2.c
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
	set("short", HIW"�Ův"NOR WHT"���� "NOR YEL"�F�n��"NOR);
	set("long",@TEXT
    �o���ж����|�P��ۤ@�s����ճT���z�Ǯa�B�ƾǮa�B���Ǯa�B�o�ǩ_�Ǫ��H
�Ҵ©ж����������|�_����A�u�����t�������v���b�ж������@�P�a�{�{�ۡA���
�o�Ǽv�������e���O�o�ǤH�ҳгy�X�Ӫ��A�o�Ǧ�m�}�ɪ��e�������G���t�ۦU��
�q���Q�쪺�c�Q�A�����|�P���o�ǤH���R�R�a���ж�������ۡA���������ǤH���v
�T�C
TEXT);

	set("exits", ([
		"west" : "/wiz/wizhall/room_wizhall_1",
	    ]));

	set("objects", ([
		"/obj/etc/mine.old" : 1,
	    ]));

	::reset_objects();
}
