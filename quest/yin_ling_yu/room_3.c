/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_3.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : �зǩж�
 * Update :
 *  2006-09-07 �E�� by Tzj@RevivalWorld
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
	set("short", HIW"�j"NOR WHT"��"NOR);
	set("long",@TEXT
    �o�̬O�@���o�󪺥j��A��B�}�¤������H�~��A���o�̪��a�W�o�˪׵ۤ@��
�w�g���`���k�l�A�ⴤ�ۤ@��M�l�A�ݤM���W�����P�M���A���G�O�b���e��H��
���A�̫ᤣ�ĳQ���A�k�l���窫���㪺���Q½�ȹL�A�ݨӫܦ��i��O�@�ηm�ܱ��H
�סA���L�k�l�������ۤ@�]�U�l�A�̭����G�˵ۤ@�ǯ��ġA�γ\�i�H�j�M�ݬݳU
�l�̩�Ǥ���F��A���~���y���z���G�٬ݨ�F���񦳨Ǥk�l������C
TEXT);

	set("exits", ([
		//��]�w�X�f�G"north" 	: ({ 72, 63, 0, "lightforest", 0, 0, "72/63/0/lightforest/0/0" }),
		//��]�w�X�f�G"west"	: ({ 71, 64, 0, "lightforest", 0, 0, "71/64/0/lightforest/0/0" }), 
		//�өж����訫����ܮy�ЬO72,65�Acode�]�w�O71,64�C
		
		//�H�U��`06-09-07�A�� tzj �E����ܧ󪺦�m�C
		"east" 		: ({ 16, 60, 0, "lightforest", 0, 0, "16/60/0/lightforest/0/0" }),
		"south" 	: ({ 15, 61, 0, "lightforest", 0, 0, "15/61/0/lightforest/0/0" }),
		"north"		: "/quest/yin_ling_yu/room_9",
	]));
	
	set("objects", ([
		"/quest/yin_ling_yu/yi_ke_wen" : 1 ,
	]));
	
	::reset_objects();
}

void do_search(object me, string arg)
{
	if( !arg || arg=="" )
		return tell(me, pnoun(2, me)+"�Q�n�j�M����H\n");

	if( strsrch(arg, "�U") != -1 )
	{
 		if( me->query_quest_step(QUEST_YIN_LING_YU) == 0 )
		{
			tell(me, "�U�l�u���@�ǯ��ġA"+pnoun(2, me)+"��ı�o�o�ǯ��Ħ�ԣ�S�O�C\n");
		}
		else if( me->query_quest_step(QUEST_YIN_LING_YU) == 1 || time() > me->query_quest_data(QUEST_YIN_LING_YU, "ginseng") + 7200 )
		{
			object ob = new("/quest/yin_ling_yu/wild_ginseng");
			
			msg("$ME���}�F�U�l�A�o��F$YOU�C\n", me, ob, 1);

			me->set_quest_note(QUEST_YIN_LING_YU, QUEST_YIN_LING_YU_NAME, "�q���_�����Ǫ��U�l�����F�@�⳥�s�H�ѡC");
			me->set_quest_step(QUEST_YIN_LING_YU, 2);
			me->set_quest_data(QUEST_YIN_LING_YU, "ginseng", time());
			ob->move(me);
		}
		else
			tell(me, "�U�l�̪����~���G�w�g�Q�����F�C\n");
	}
	else if( strsrch(arg, "�k") != -1 || strsrch(arg, "��") != -1 )
		tell(me, "�o�Ӥk�l��������G�O���n�誺��V�L�h�F�C\n");
	else if( strsrch(arg, "�M") != -1 )
		tell(me, "�o����ê��M�W��B���O��A�᪺�����A��M���O����n�M�C\n");
	else if( strsrch(arg, "��") != -1 )
		tell(me, "�@����o�w�[���Фl�C\n");
	else if( strsrch(arg, "��") != -1 )
		tell(me, "�o�Өk�l���窫�̤w�g�Q�j�A�@�šA���򳣤��ѤF�C\n");
	else if( strsrch(arg, "����") != -1 )
		tell(me, "�q�o��ݤ��M���U�l���F��O����A�u�ݱo�X�����O���Ī��ҼˡC\n");
	else
		tell(me, pnoun(2, me)+"�Q�n�j�M����H\n");	
}

mapping actions = 
([
	"search" : (: do_search :),
]);