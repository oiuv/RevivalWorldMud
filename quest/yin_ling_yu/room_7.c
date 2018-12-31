/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_7.c
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
�۪�[1;34m�ťɥ۬W[m�������b�a�W�A�����L�v���ɥ����H�ݱo�����f�b�C
TEXT);
	
	set("exits", ([
		"north" 	: "/quest/yin_ling_yu/room_6",
	]));

	::reset_objects();
}

void do_push(object me, string arg)
{
	mapping stone;

	if( arg != "��" && arg != "�k" )
		return tell(me, pnoun(2, me)+"�Q�n��۬W�����k���Ӥ�V��(��, �k)�H\n");
		
	if( !query_temp("/quest/yin_ling_yu/stone/red", me) )
		return tell(me, pnoun(2, me)+"�ϥX�Y�����O�q�]�����ʳo�ӥ۬W�C\n");

	if( lower_case(arg) == "��" )
	{
		set_temp("/quest/yin_ling_yu/stone/blue", 1, me);
		msg("$ME�ΤO�N�۬W��������A�u�Pı��a�O�U���G���F��b�_�ʡC\n", me, 0, 1);
	}
	else if( lower_case(arg) == "�k" )
	{
		set_temp("/quest/yin_ling_yu/stone/blue", 2, me);
		msg("$ME�ΤO�N�۬W���k����A�u�Pı��a�O�U���G���F��b�_��...�C\n", me, 0, 1);
	}

	stone = query_temp("/quest/yin_ling_yu/stone", me);
	
	if( stone["green"] == 1 && stone["yellow"] == 2 && stone["red"] == 2 && stone["blue"] == 2 )
	{
		msg("$ME�}���U���۪O��M�P�}�A$ME�N�o�˱��F�U�h�A�۪O�H�Y�S���F�_�ӡC\n", me, 0, 1);
		me->move("/quest/yin_ling_yu/room_8");
		me->set_quest_note(QUEST_YIN_LING_YU_2, QUEST_YIN_LING_YU_2_NAME, "�}�ѤF�s�}���������A�i�J��۬}�����C");
		me->set_quest_step(QUEST_YIN_LING_YU_2, 4);
		delete_temp("/quest/yin_ling_yu/stone", me);
	}
	else
	{
		delete_temp("/quest/yin_ling_yu/stone", me);
		msg("$ME�����e��M�@�}�ҽk�A�����F�C\n", me, 0, 1);
		me->move(({ 81, 71, 0, "lightforest", 8, 0, "81/71/0/lightforest/8/0" }));
		msg("$ME�����e��M�@�}�ҽk�A�����@�G�N�]��F�o�̡C\n", me, 0, 1);
		
		if( !random(5) )
			me->faint();
	}
}

mapping actions = 
([
	"push" : (: do_push :),
]);