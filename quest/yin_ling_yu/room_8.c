/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_8.c
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
#include <daemon.h>

inherit STANDARD_ROOM;

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;

void create()
{
	set("short", HIW"���K"NOR WHT"�s�}�}��"NOR);
	set("long",@TEXT
    �o�̬O�@�ӷ������K���s�}�A�̭��@�����¡A������N�h���O�N�B�B�B���㪺
���z�A�Y���W�]���ɦa�w�U���ӡA���L�����Y�@�ݡA���G�槽�ٮ��西���A�a�W�]
�i�H��������X�西�b�ͪ������s�H�ѡA�Ӧb�o�Ӥ西���s�}�������h���@�ӫ���
�۪��եɥۻO�A�ۻO���W����G�i�H��@�ǪF��C
TEXT);
	
	::reset_objects();
}

void do_put(object me, string arg)
{
	object ob;
	
	if( !arg || !arg[0] )
		return tell(me, pnoun(2, me)+"�Q�n�񤰻�F��b�ۻO�W�H\n");
		
	ob = present(arg);
	
	if( !objectp(ob) )
		return tell(me, "�S���o�˪F��C\n");
		
	if( base_name(ob) != "/quest/yin_ling_yu/tear_wild_ginseng" )
		return tell(me, pnoun(2, me)+"�N"+ob->query_idname()+"���ۻO�W�n�@�}�l�A���G�Sԣ�����]���S���F�U�ӡC\n");

	tell(me, pnoun(2, me)+"�N"+ob->query_idname()+"���ۻO�W�C\n");
	
	destruct(ob, 1);
	ob = 0;

	tell(me, WHT+@TEXT

    �ж��̬�M�ǤF�@�ӲӲӪ��n���G�H�����ܱ��ܷR�K�O�p���A���H�ର�F�R��
�����A���H�ର�F�R�Ӯ��_��{���Z���A�󦳤H�ର�F�R�ӥ��h�ͩR�A���M��
��u�d�U�F���\�����\�A���o�N�O�H��...
    �ڪ����F�Ѥw�g�����A�D�`�P�±z�����@���A�ڳo�̨S������S�O���F��A�N
���\�ڱN�@���������F�Ѯ��������z���y�M�a�A�p�G�z�S���y�M���ܧڴN�N�ڧQ��
���s�H�Ѫ����}�i���y�M�e���z�a

TEXT+NOR);

	foreach(object inv in all_inventory(me))
	{
		switch(base_name(inv))
		{	
			case "/obj/pasture/treasure/giant_eagle":
				ob = new("/obj/pasture/treasure/true_giant_eagle");
				destruct(inv);
				break;
			case "/obj/pasture/treasure/fly_horse":
				ob = new("/obj/pasture/treasure/true_fly_horse");
				destruct(inv);
				break;
			case "/obj/pasture/treasure/kylin":
				ob = new("/obj/pasture/treasure/true_kylin");
				destruct(inv);
				break;
			case "/obj/pasture/treasure/mammoth":
				ob = new("/obj/pasture/treasure/true_mammoth");
				destruct(inv);
				break;
			case "/obj/pasture/treasure/pack_animal":
				ob = new("/obj/pasture/treasure/true_pack_animal");
				destruct(inv);
				break;
		}
		
		if( objectp(ob) )
			break;
	}
	
	if( !objectp(ob) )
		ob = new("/obj/pasture/treasure/true_horse");

	msg("$ME�o��F$YOU�C\n", me, ob, 1);
	ob->set_keep();
	ob->move(me);
	
	me->set_quest_finish(QUEST_YIN_LING_YU_2, QUEST_YIN_LING_YU_2_NAME, "�q�L�F��������A�N���F�ѥ�^�D�H�⤤�A�ño��@�Ǽ��y�C");
	
	if( !wizardp(me) )
		CHANNEL_D->channel_broadcast("city", me->query_idname()+"�Ѷ}�F�u"+QUEST_YIN_LING_YU_2_NAME+"�v", me);
}

mapping actions = 
([
	"put" : (: do_put :),
]);
