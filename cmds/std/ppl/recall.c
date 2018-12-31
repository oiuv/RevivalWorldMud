/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : recall.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-2-2
 * Note   : recall ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <object.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�o�ӫ��O�i�H���z�b�g�����ɭԧֳt�a�^��Ův�j�U


HELP;

#define WIZ_HALL_UP	"/wiz/wizhall/room_wizhall_7"

private void do_command(object me, string arg)
{
	object recallroom;
	object env = environment(me);
	object *inv;

	// �O�ǤH
	if( query("prisoner", me) )
	{
		msg("$ME����}�Q�K��R��A�L�k�ۥѦ�ʡC\n", me, 0, 1);
		return;
	}

	//���L������U���O
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	if( wizardp(me) )
		recallroom = load_object(WIZ_HALL_UP);
	else
		recallroom = load_object(WIZ_HALL);
		
	inv = all_inventory(recallroom);

	if( env && env == recallroom ) 
		return tell(me, pnoun(2, me)+"�w�g�b"+query("short", env)+"�F�C\n");

	msg("$ME�|�_�k��b���e���t�e�F�@�Ӷ�A�����}�F���e���Ŷ��A�u���ꤺ���Ŷ��u�űN$ME���l�F�i�h�C\n", me, 0, 1);

	me->move(recallroom);

	inv = filter_array(inv, (: userp($1) :));

	if( sizeof(inv) )
		msg("$ME�q"+query("short", recallroom)+"�������Ŷ����_���p�X�A�~�F$YOU�@���C\n", me, inv[random(sizeof(inv))], 1);
	else
		msg("$ME�q"+query("short", recallroom)+"�������Ŷ����_���p�X�C\n", me, 0, 1);
}