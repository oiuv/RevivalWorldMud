/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_1.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-19
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
	set("short", WHT"�}�Z�����a"NOR);
	set("long",@TEXT
    �o���Τl���|�P���񺡤F�U�عA�@����A�ܩ���O�@�ӥ��Z���A�a�A�������Z
���O�a�W����y�����˪׵ۤ@�Өk�l�A�ݤf���ۧ�M�A�q���ݨӦ��G��Q���`��
�[�C�o�Өk�l���M�w�g���`�A���⤤���򴤵ۤ@���Ŧ��A�n�A���G��L�Ө��O�ӫ�
���n���F��A�γ\�i�H�ݬݸ̭�������u���C
TEXT);

	set("exits", ([
		"south" 	: ({ 37, 51, 0, "blueflowers", 0, 0, "37/51/0/blueflowers/0/0" }),
		"west"	: "/quest/yin_ling_yu/room_2",
	]));
	
	set("objects", ([
		"/quest/yin_ling_yu/liang_fan_huan" : 1 ,
	]));
	
	::reset_objects();
}

void do_search(object me, string arg)
{	
	if( !arg || arg=="" )
		return tell(me, pnoun(2, me)+"�Q�n�j�M����H\n");

	if( strsrch(arg, "�A�n") != -1 )
	{
		if( query("total_online_time", me) < 15*24*60*60 )
			return tell(me, pnoun(2, me)+"���W�u�ɶ����� 15 �ѵL�k�i��o�ӥ��ȡC\n");

		if( me->query_quest_step(QUEST_YIN_LING_YU) == 0 )
		{
			if( me->query_quest_finish(QUEST_YIN_LING_YU) && time() < me->query_quest_time(QUEST_YIN_LING_YU) + 60*60*24*7)
				return tell(me, pnoun(2, me)+"���[�e�~½�L�o���A�n�A�ݤ��X�������L�S�O���a��F�C\n");
				
			tell(me, pnoun(2, me)+"���}�F�Ŧ��A�n�A�����Y�o�S������F��A�u���A�n�~¸�ۨ�ӦW�r�A���O�O�Z���P�¦СC\n");
			
			me->set_quest_note(QUEST_YIN_LING_YU, QUEST_YIN_LING_YU_NAME, "�q�Ŧ��A�n���~���o�{�F��ӦW�r�G�u�Z���P�¦Сv�C");
			me->set_quest_step(QUEST_YIN_LING_YU, 1);
		}
		else if( me->query_quest_step(QUEST_YIN_LING_YU) == 2 )
		{
			tell(me, pnoun(2, me)+"�S�J�Ӫ��ݤF���Ŧ��A�n�A��Ӥ�Ũ�]�_�ۤ@�Ǧr�A�W���g�ۡG�u��v�M���t�ߡv�C\n");
			
			me->set_quest_note(QUEST_YIN_LING_YU, QUEST_YIN_LING_YU_NAME, "�q�Ŧ��A�n����Ũ���o�{�F�@�y�֡G�u��v�M���t�ߡv�C");
			me->set_quest_step(QUEST_YIN_LING_YU, 3);
		}
		else
			tell(me, pnoun(2, me)+"�䤣��o���Ŧ��A�n�������L�S�O���a��F�C\n");
	}
	else if( strsrch(arg, "��") != -1 || strsrch(arg, "��") != -1 )
		tell(me, "�a�W�V�ê���⨬�񤤥i�H����X���ƭӤH����n��ӥh�C\n");
	else if( strsrch(arg, "�M") != -1 )
		tell(me, "�@��g����񪺤M�A���ݤM�Φ��G�ä��O�A�a���ϥΪ��M��C\n");
	else if( strsrch(arg, "��") != -1 )
		tell(me, "�D�`������A�n�A�H�Ŧ⪺���u�_�s�Ӧ��C\n");
	else if( strsrch(arg, "�k") != -1 )
		tell(me, "�o�Өk�l���ݤf�Q�M����ӹL�A��M�O�ӭP�R���ˤf�C\n");
	else
		tell(me, pnoun(2, me)+"�Q�n�j�M����H\n");
}

mapping actions = 
([
	"search" : (: do_search :),
]);