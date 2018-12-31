/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_wizhall_6.c
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
#include <daemon.h>

inherit STANDARD_ROOM;

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;

mapping actions;

void init(object ob);

void do_join(object me, string arg)
{
	if( query("newbiemaster", me) )
		return tell(me, pnoun(2, me)+"�w�g�O�s����ɭ��F�C\n");

	if( query("total_online_time", me) < 6*30*24*60*60 )
		return tell(me, pnoun(2, me)+"���C���ɶ����W�L���Ӥ�A�L�k����s����ɭ��C\n");

	if( wizardp(me) )
		return tell(me, "�Ův�L�k����s����ɭ��C\n");

	me->add_title(HIY"�s��"NOR YEL"���ɭ�"NOR);
	set("newbiemaster", 1, me);

	CHANNEL_D->channel_broadcast("news", me->query_idname()+"���@����q�ȩʪ�"HIY"�s��"NOR YEL"���ɭ�"NOR"�A�Фj�a���x�A�s�⦳������D�ҥi�V"+pnoun(3, me)+"�߰ݡC\n");

	me->save();

	init(0);
}

void do_leave(object me, string arg)
{
	if( !query("newbiemaster", me) )
		return tell(me, pnoun(2, me)+"�쥻�N���O�s����ɭ��C\n");

	me->remove_title(HIY"�s��"NOR YEL"���ɭ�");
	delete("newbiemaster", me);

	CHANNEL_D->channel_broadcast("news", me->query_idname()+"�����s����ɭ���¾�ȡA�Фj�a��"+pnoun(3, me)+"���e���^�m���x�C\n");

	me->save();

	init(0);
}

void create()
{
	set("short", HIW"�Ův"NOR WHT"���� "NOR YEL"��n��"NOR);

	set("exits", ([
		"east" : "/wiz/wizhall/room_wizhall_1",
	    ]));

	set("objects", ([
	    ]));

	init(0);

	actions = ([
	    "join" : (: do_join :),
	    "leave": (: do_leave :),
	]);

	::reset_objects();
}

void init(object ob)
{
	object master_player;

	string roomlong = @TEXT
    �o�̬O�s�⪺���ɭ��𮧫ǡA��B�\���F�U�ظ޲����оǹD��A�ݨӨC�����
�����ܨ��W�����ɷs��i�J�o�Ӽs�j���@�ɡA�����ި��W�A���ɭ��̫o�O�֦����h
�A�P�ɧ�O����F�\�h�n�͡A�u������W���q�l�ݪO���C�X�ثe���u�W�s����ɭ�
�W��A������C���W�����D�ݥL�̴N��F�A�U�F���O�utell "id" XXXX�v�K�i�H�P
�L�̷��q�͸ܡA��M�L�̤j�h�Ƥ]�O�j���H�A���@�w�|�^���A���͸ܡA�i�H�h�մX
����ɭ��άO������J�uchat XXXX�v�b���@�W�D�W�o�ݡC
TEXT;

	roomlong += HIY"\n����                     �u�W�s����ɭ� ID\n"NOR;

	foreach( master_player in users() )
	{
		if( query("newbiemaster", master_player) )
			roomlong += sprintf(HIY"%-24s"NOR" %-30s\n", CITY_D->query_city_idname(query("city", master_player))||"�L���y", master_player->query_idname());
	}

	set("long", roomlong);
}
