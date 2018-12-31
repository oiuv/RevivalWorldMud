/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chemical.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-16
 * Note   : �ƾǭ�Ƽt
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <npc.h>

inherit ROOM_ACTION_MOD;

inherit __DIR__"inherit/_mod_factory.c";

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"line"	:
	([
		"short"	: HIM"�Ͳ�"NOR MAG"�u"NOR,
		"heartbeat":30,	// ��ڮɶ� 1 �����
		"job": WORKER,
		"master":1,
		"help"	:
			([
"topics":
@HELP
    �Ͳ��u���Ѳ��~���s�y�P��ެ�o�C
HELP,

"line":
@HELP
�Ͳ��u������O�A�Ϊk�p�U�G[�޲z���O]
  line 2			- �]�w�s�y�Ͳ���o���߽s���� 2 �����~
  line start			- �ҰʥͲ��u���B�@
  line stop			- ����Ͳ��u���B�@
HELP,

"setup":
@HELP
�]�w�Ͳ��u��ƪ����O�A�Ϊk�p�U�G[�޲z���O]
  setup reserach 1 21,33 	- �]�w���y��o���߮y�Цb�Ĥ@�ìP������ 21,33
  setup warehouse 1 75,90	- �]�w�ܮw���߮y�Цb�Ĥ@�ìP������ 75,90
HELP,

"list":
@HELP
�C�X�i�Ͳ������~��T�A�Ϊk�p�U�G
  list			 	- �C�X�i�Ͳ������~��T
HELP,
			]),
		"action":
			([
				"line"		: (: do_line_action($1, $2) :),
				"list"		: (: do_list_action($1, $2) :),
				"setup"		: (: do_setup_action($1, $2) :),
			]),
	]),
]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIB"�ƾ�"NOR BLU"��Ƽt"NOR

	// �}�i���ؿv�����̤֩ж�����
	,4

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,INDIVIDUAL

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,INDUSTRY_REGION

	// �}�i�����O��
	,"15000000"

	// �ؿv���������ռаO
	,1

	// �c�a�^�m��
	,9
	
	// �̰��i�[�\�Ӽh
	,2
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,2
});
