/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : firework.c
 * Author : 
 * Date   : 2000-00-00
 * Note   : �Ϥ�
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <feature.h>
#include <object.h>
#include <daemon.h>
#include <message.h>
#include <inherit.h>
#include <ansi.h>

inherit STANDARD_OBJECT;

int do_light( object me, string arg );

mapping actions;

string *color = ({
    RED"", YEL"", GRN"", BLU"", MAG"", CYN"", WHT"",
    HIR"", HIG"", HIY"", HIB"", HIM"", HIC"", HIW""
});

string *msg = ({
    "���m�}�ɪ������b�ѪŤ����}�A�n�����R�C\n",
    "�b���ѪųQ�C�m���~�E���A�O�H�����áC\n",
    "����⪺���K���W���g�A�S�z�X�L�ƻȦ⪺�a������C\n",
    "�|�g������S�ߨ�U���z�}�ӡA�o�X�R�R��窺�ӸH�z���C\n",
    "����X�d�U�����V�A�V�|�B�ɭ��ӥh�A���A�ؤ��v���C\n",
    "���@���r���뼻�U�C�m������A�w�w�a�q�ѳ����ۦӭ��C\n"
});

void create()
{
        set_idname( "firework", "�Ϥ�" );

        if( clonep() ) 
                set("shadow_ob", find_object(base_name(this_object())));
        else
        {
                set("long", "�o�O�@�ΨӼy�����Ϥ��A�A�i�H�I�U��(light firework)�C\n");
                set("unit", "��");
                set("value", 100);
                set("mass", 2);
                set("badsell", 1);
        }

        actions = 
        ([ 
            "light"     : (: do_light :)
        ]);
}

int do_light( object me, string arg)
{
        string reason;
        int random_msg, random_color;

        if( !arg ) 
                return tell( me, "�A�Q�I�U����O�H\n");
        else if( arg == this_object()->query_id() ) 
        {
                random_msg = random(sizeof(msg));
                reason = msg[random_msg];
        } 
        else if( sscanf(arg, this_object()->query_id() + " %s", reason) == 1 ) 
        {
                reason = "����M�z�Ϧb�Ť��ƥX�y" + reason + "�z\n";
        } 
        else return tell( me, "�A�Q�I�U����O�H\n");

	if( time() - query_temp("firework/lasttime", me) < 3600 )
	{
		if( addn_temp("firework/times", 1, me) > 10 )
			return tell(me, "�u�ɶ�������A�I�U�Ϥ��F�C\n");
	}
	else
	{
		set_temp("firework/lasttime", time(), me);
		delete_temp("firework/times", me);
	}

        random_color = random(sizeof(color));

        shout( me->query_idname() + "�I�U�F�@�ڷϤ�, �����@�n���W�Ѫ�...\n" + color[random_color] + "�@�D�{�����L�ѻڡA�@�ӷϤ��y�I�z�a�@�n�b�Ť��z�}�E�E�E\n" + reason + NOR);
	
        destruct( this_object(), 1);
        return 1;
}
