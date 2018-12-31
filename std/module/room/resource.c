/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : resource.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : �귽������
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
		"short"	: NOR WHT"�Ͳ��u"NOR,
		"help"	:
			([
"topics":
@HELP
    �Ͳ��u���Ѳ��~���s�y�P��ެ�o�C
HELP,

"import":
@HELP
��J��ƪ����O�A�Ϊk�p�U�G[�޲z���O]
  import			- �L�I�ثe�Ͳ��u������Ƽƶq
  import all			- �N���W�Ҧ���������~��J�Ͳ��u
  import all stone		- ��J�Ҧ��� stone ���
  import 3 stone		- ��J 3 �� stone ���
  import stone			- ��J 1 �� stone ���
HELP,

"export":
@HELP
��X��ƪ����O�A�Ϊk�p�U�G[�޲z���O]
  export all			- �N�Ҧ��Ͳ��u�W����ƿ�X
  export all stone		- ��X�Ҧ��� stone ���
  export 3 stone		- ��X 3 �� stone ���
  export stone			- ��X 1 �� stone ���
HELP,

"plist":
@HELP
�C�X���Ͳ��u�i�H�Ͳ������~�����A�Ϊk�p�U�G
  plist				- �C�X���Ͳ��u�i�H�Ͳ������~����
HELP,

"design":
@HELP
�]�w���s���~�����O�A�Ϊk�p�U�G[�޲z���O]
  design			- ��ܥثe�����~�]�p���
  design type chair		- �]�p�s���~�������� chair ����
  design id big chair		- �]�p�s���~���^��W�٥s�� 'big chair'
  design name �j�Ȥl		- �]�p�s���~������W�٥s�� '�j�Ȥl'
  design sample			- ���եͲ��˫~�A�խY�˫~�Ͳ����\�A�Y�i�ߨ�i�沣�~���q��
HELP,

"line":
@HELP
�Ͳ��u������O�A�Ϊk�p�U�G[�޲z���O]
  line				- ��ܥثe�Ͳ��u���Ͳ�����
  line start			- �ҰʥͲ��u���B�@
  line stop			- ����Ͳ��u���B�@
  line cancel			- �û��R���ð���Ͳ����b�Ͳ������~
HELP,

"setup":
@HELP
�]�w�Ͳ��u��ƪ����O�A�Ϊk�p�U�G[�޲z���O]
  setup reserach 21,33 		- �]�w�����Ͳ��u��������o���ߩж��y�Цb 21,33
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job": WORKER,
		"action":
			([
				//"import"	: (: ::do_import_action($1, $2) :),
				//"plist"		: (: ::do_plist_action($1, $2) :),
				//"design"	: (: ::do_design_action($1, $2) :),
				//"line"		: (: ::do_line_action($1, $2) :),
				//"setup"		: (: ::do_setup_action($1, $2) :),
			]),
	]),
	
	"research":
	([
		"short"	: NOR HIW"��o����"NOR,
		"help"	:
			([
"topics":
@HELP
   ��o���߭t�d�i���ު���o�A���ݭn�j�q�u�{�v�C
HELP,

"info":
@HELP
�d�ݬ�o���ߪ���o���G�A�Ϊk�p�U�G
  info			- ��ܬ�o���ߪ���o���G
HELP,

"research":
@HELP
��o�Y���������~�A�Ϊk�p�U�G
  research		- �w��Y���������~�i���o
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job"	: ENGINEER,
		"action":
			([
				//"research"	: (: ::do_research_action($1, $2) :),
				//"plist"		: (: ::do_plist_action($1, $2) :),
				//"info"		: (: ::do_research_info_action($1, $2) :),
			]),
	]),
]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIY"�귽"NOR YEL"�Ķ���"NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,INDIVIDUAL

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,INDUSTRY_REGION

	// �}�i�����O��
	,"5000000"

	// �ؿv���������ռаO
	,1

	// �c�a�^�m��
	,5
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,1
	
	// �ؿv���ɥN
	,2
});
