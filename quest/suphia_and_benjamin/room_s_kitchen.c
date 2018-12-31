/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_s_kitchen.c
 * Author : tzj@RevivalWorld
 * Date   : 2006-09-09
 * Note   : Ĭ��Ȯa���p��
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
	set("short", HIY"�p��"NOR);
	set("long",@TEXT
�@�@�@���i�o�̡A�i�ծɲ��ͪ�����ﭱ�ӨӡA�H�Y�D��L�׬����@�p������A
���l�W��m����l���m�˦a�B�P�@�T�C���B�p�㪺�\�񤫵M���ǡA�Q���g�L�@�f
�����z�C�Ф�����������Hı�o�ŷx���H�H�Z����A���a���l�l������y�L�Q
�����Ħ�F�C
TEXT);


	set("exits", ([
		//���F���O�q��2F���ӱ�Ψ�ӫ��
		"east"    : "/quest/suphia_and_benjamin/room_s_afterroom",
		
		//���n���O���U
		"south"   : "/quest/suphia_and_benjamin/room_s_frontroom",
		
		//���訫�O��
		//"west"  : ,
		
		//���_���O��
		//"north" : ,
		
	]));

/*	�w�d��NPC�Ϊ��~��...
	set("objects", ([
		"/quest/suphia_and_benjamin/npc_suphia" : 1 ,
	]));
*/
	
	::reset_objects();
	replace_program(STANDARD_ROOM);
}
