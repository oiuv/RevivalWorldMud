/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_s_afterroom.c
 * Author : tzj@RevivalWorld
 * Date   : 2006-09-09
 * Note   : Ĭ��Ȯa���m����
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
	set("short", CYN"�m����"NOR);
	set("long",@TEXT
�@�@�o�̸m��F�@�ǲ�������δX�U���s���}�ơA�]��m�F�A�ѩһݭn�Ψ쪺�U
�ؤu��C�ѩ�g�L��z�A�ҥH���M���B�F��ܦh�S���A���ݰ_�ӫo�@�I�]���áC
�q�o�̩��Τl�`�B�ݹL�h�A�i�H�ݨ���G�O�p�Ъ��a��A���������𤣮ɱq�ӳB
�ǹL�ӡC
TEXT);


	set("exits", ([
		//���F���O��
		"east"    : ({ 82, 66, 0, "lightforest", 0, 0, "82/66/0/lightforest/0/0" }),
		
		//���n���O��
		//"south" : ,
		
		//���訫
		"west"    : "/quest/suphia_and_benjamin/room_s_kitchen",
		
		//���_���O��
		"north" : ({ 81, 65, 0, "lightforest", 0, 0, "81/65/0/lightforest/0/0" }),
			
		//���B���q��2F���ӱ�
		//"up"    : ,
		
	]));

/*	�w�d��NPC�Ϊ��~��...
	set("objects", ([
		"/quest/suphia_and_benjamin/npc_suphia" : 1 ,
	]));
*/
	
	::reset_objects();
	replace_program(STANDARD_ROOM);
}
