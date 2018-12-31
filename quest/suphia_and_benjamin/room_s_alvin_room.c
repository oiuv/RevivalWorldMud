/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_s_alvin_room.c
 * Author : tzj@RevivalWorld
 * Date   : 2006-09-10
 * Note   : Ĭ��Ȫ��̧̡Ъ����媺�׫�
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
	set("short", HIC"�׫�"NOR);
	set("long",@TEXT
�@�@�o�̬OĬ��ȧ̧̡Ъ����媺�׫ǡA�ϥ��N�O�@�ӤQ�G���x�֤p�k�Ī��ж��I
TEXT);


	set("exits", ([
		//���F���O��
		//"east"    : ,
		
		//���n���O��
		//"south" : ,
		
		//����^����U
		"west"    : "/quest/suphia_and_benjamin/room_s_frontroom",
		
		//���_���O��
		//"north" : ,
		
	]));

	set("lock/west", 1);
/*	�w�d��NPC�Ϊ��~��...
	set("objects", ([
		"/quest/suphia_and_benjamin/npc_suphia" : 1 ,
	]));
*/
	
	::reset_objects();
	replace_program(STANDARD_ROOM);
}
