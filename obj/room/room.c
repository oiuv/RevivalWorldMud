/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-21
 * Note   : �зǩж�
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_ROOM;

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;

void create()
{
	set("short","�ũж�");
	set("long",@TEXT
    �o�O�@�Ӥ��򳣨S�����ж��A�u���ж��|�P���F�}�ª�����~, ��b�ݤ��X�o�өж�������S�O���B�C

TEXT);
}