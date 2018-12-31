/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : warehouse.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-05-26
 * Note   : �Ҳ� - �ܮw
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <npc.h>
#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit ROOM_ACTION_MOD;

inherit __DIR__"inherit/_object_database_mod.c";
inherit __DIR__"inherit/_action_list";
inherit __DIR__"inherit/_action_import";
inherit __DIR__"inherit/_action_export";
inherit __DIR__"inherit/_action_transport";
inherit __DIR__"inherit/_action_setup";
inherit __DIR__"inherit/_action_auction";

#define WAREHOUSE_CAPACITY	-1

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"warehouse"	:
	([
		"short"	: HIR"�x�îw��"NOR,
		"help"	: 
			([ 
"topics":
@HELP
    �ܮw�O���a�x�ê��~���a��C
HELP,

"import":
@HELP
�N�ӫ~�W�[�����O�A�Ϊk�p�U�G[�޲z���O]
  import all		- �N�Ҧ����W�����~�s�J�ܮw
  import all rice	- �N�Ҧ��� rice ���~�s�J�ܮw
  import 3 rice		- �N 3 �� rice ���~�s�J�ܮw
  import rice		- �N 1 �� rice ���~�s�J�ܮw
HELP,

"export":
@HELP
�N�ӫ~�U�[�����O�A�Ϊk�p�U�G[�޲z���O]
  export all		- �N�Ҧ������~�q�ܮw�����X
  export all rice	- �N�Ҧ� rice ���~�q�ܮw�����X
  export 3 rice		- �N 3 �� rice ���~�q�ܮw�����X
  export rice		- �N 1 �� rice ���~�q�ܮw�����X
HELP,

"transport":
@HELP
�B�骫�~�����O�A�Ϊk�p�U�G
  transport rice to 1 51,22	- �N 1 �� rice ���~�B��ܲ� 1 �ìP�����y��(51,22)�B���ؿv��
  transport 3 rice to 2 51,22	- �N 3 �� rice ���~�B��ܲ� 2 �ìP�����y��(51,22)�B���ؿv��
  transport 2 3 to 1 51,22	- �N 2 ��s���� 3 �����~�B��� �� 1 �ìP�����y��(51,22)�B���ؿv��
HELP,

"setup":
@HELP
�]�w�ܮw�\�઺���O�A�Ϊk�p�U�G[�޲z���O]
  setup class 2 ���~ 		- �N�s���� 2 �����~�����쭹�~�����[�l�W�A�i�ۭq����
HELP,

"auction":
@HELP
��檫�~�����O�A�Ϊk�p�U�G

auction '���~�s��' '�ƶq' $'����' $'�����ʶR��' '���ɶ�'

�Ҧp:
	
auction 1 500 $20000 $1000000 5		- ���s���� 1 �����~ 500 �ӡA���л��� $20000�A
					  �����ʶR���� $1000000�A���ɶ��� 5 �p��

���ȳ��@�߬� $RW

HELP,

			]),
		"master":1,
		"action":
			([
				"import"	: (: do_import($1, $2, "products", WAREHOUSE_CAPACITY) :),
				"export"	: (: do_export($1, $2, "products") :),
				"list"		: (: do_warehouse_list($1, $2, "products", WAREHOUSE_CAPACITY) :),
				"transport"	: (: do_transport($1, $2, "products") :),
				"setup"		: (: do_warehouse_setup($1, $2) :),
				"auction"	: (: do_auction($1, $2, "products") :),
			]),
	]),

]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIR"�ܮw"NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,INDIVIDUAL

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,INDUSTRY_REGION

	// �}�i�����O��
	,"2000000"
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,2
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,5
	
	// �ؿv���ɥN
	,1
});

