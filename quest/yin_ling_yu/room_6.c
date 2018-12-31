/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_6.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : �зǩж�
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <quest.h>

inherit STANDARD_ROOM;

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;

void create()
{
	set("short", HIW"���K"NOR WHT"�s�}"NOR);
	set("long",@TEXT
    �o�̬O�@�ӷ������K���s�}�A�̭��@�����¡A������N�h���O�N�B�B�B���㪺
���z�A�Y���W�]���ɦa�w�U���ӡA���L�����Y�@�ݡA���G�槽�ٮ��西���A�a�W�]
�i�H��������X�西�b�ͪ������s�H�ѡA�Ӧb�o�Ӥ西���s�}�������h���@�ӫ���
�۪�[1;31m���ɥ۬W[m�������b�a�W�A�����L�v���ɥ����H�ݱo�����f�b�C
TEXT);

	set("exits", ([
		"south" 	: "/quest/yin_ling_yu/room_7",
		"north"		: "/quest/yin_ling_yu/room_5",
	]));
	
	::reset_objects();
}

void do_push(object me, string arg)
{
	if( arg != "��" && arg != "�k" )
		return tell(me, pnoun(2, me)+"�Q�n��۬W�����k���Ӥ�V��(��, �k)�H\n");
		
	if( !query_temp("/quest/yin_ling_yu/stone/yellow", me) )
		return tell(me, pnoun(2, me)+"�ϥX�Y�����O�q�]�����ʳo�ӥ۬W�C\n");

	if( lower_case(arg) == "��" )
	{
		set_temp("/quest/yin_ling_yu/stone/red", 1, me);
		msg("$ME�ΤO�N�۬W��������A�u�Pı��a�O�U���G���F��b�_�ʡC\n", me, 0, 1);
		return;
	}
	else if( lower_case(arg) == "�k" )
	{
		set_temp("/quest/yin_ling_yu/stone/red", 2, me);
		msg("$ME�ΤO�N�۬W���k����A�u�Pı��a�O�U���G���F��b�_��...�C\n", me, 0, 1);
		return;
	}
}

mapping actions = 
([
	"push" : (: do_push :),
]);