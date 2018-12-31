/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_wizhall_4.c
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
#include <condition.h>
#include <daemon.h>

inherit STANDARD_ROOM;

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;

mapping actions;

void do_read(object me, string arg)
{
        if( query("total_online_time", me) > 86400*45 )
                return tell(me, pnoun(2, me)+"���`�W�u�ɶ��w�g�W�L 45 �ѡA�L�k�A�ɥγo�ӻ��U�k�N�F�C\n");

        msg("$ME�J�Ӧa½�F½�u"HIG"�s��"NOR GRN"���|�N"NOR"�v�A��M�����W�x�_�@�}����A���|�g��ȻP�԰��g�����o 50% ���[���C\n", me, 0, 1);

        me->start_condition(NEWBIE);
}

void create()
{
        set("short", HIW"�Ův"NOR WHT"���� "NOR YEL"���_��"NOR);
        set("long",@TEXT
    �o�̬O�Ův�������_�䪺�ж��A�o���ж��D�`�����j�A���ӤW�ݥh���G���@�s
�H�����b���Q�׵ۨƱ��A�����|�B�d�ݫo�䤣��q���ӤW�����C�o���ж��|�P����
���W�e���ۦU���U�˪����ˡA�P��L���ж��۸��_�ӡA���G���Գ\�h�C�𨤫h���
�@���}�ª����y�A�W���g�ۡu�s����|�N�v�A�γ\�i�HŪŪ��(read)�C
TEXT);

        set("exits", ([
                "west" : "/wiz/wizhall/room_wizhall_5",
                "east" : "/wiz/wizhall/room_wizhall_3",
                "south" : "/wiz/wizhall/room_wizhall_1",
                "up" : "/wiz/wizhall/room_wizhall_7",
        ]));

        set("lock", ([
                "up" : 6,
        ]));

        actions = ([
                "read": (: do_read :),
        ]);

        ::reset_objects();
}
