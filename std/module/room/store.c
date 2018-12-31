/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : store.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-4-13
 * Note   : �Ҳ� - �@��ө�
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
#include <condition.h>

inherit ROOM_ACTION_MOD;

inherit __DIR__"inherit/_object_database_mod.c";
inherit __DIR__"inherit/_action_import.c";
inherit __DIR__"inherit/_action_export.c";
inherit __DIR__"inherit/_action_list.c";
inherit __DIR__"inherit/_action_buy.c";
inherit __DIR__"inherit/_action_sell.c";
inherit __DIR__"inherit/_action_gain.c";
inherit __DIR__"inherit/_action_setup.c";
inherit __DIR__"inherit/_action_transport";
inherit __DIR__"inherit/_heart_beat_store.c";
inherit __DIR__"inherit/_action_auction";

#define	POBJECT		0
#define PSHORT		1
#define PIDNAME		2
#define PAMOUNT		3
#define PPRICE		4

#define STORE_CAPACITY	100000

void do_gowork(object me, string arg)
{
	object env = environment(me);
	object master = env->query_master();
	
	if( !query_heart_beat(master) )
		CHANNEL_D->channel_broadcast("sys", me->query_idname()+"���խ״_ "+base_name(master)+" �ө����D(��l�߸� "+query_heart_beat(master)+")");
		
	master->startup_heart_beat();

	msg("$ME�����@�l�⥴���u�G�u���ڥΤO�u�@�I�I�v\n", me, 0, 1);
}

void do_confirm_sightsee(object me, object env, string moneyunit, mixed money, int directly, string arg)
{
	int floor = env->query_floor();

	if( !directly && lower_case(arg) != "y" && lower_case(arg) != "yes" )
	{
		tell(me, pnoun(2, me)+"�����I�O�C�������Y�C\n");
		me->finish_input();
		return;
	}

	if( !directly )
	{
		if( count(money, ">", 0) && !me->spend_money(moneyunit, money) )
			return tell(me, pnoun(2, me)+"���W����������I�C���O�ΡC\n");
		else
		{
			msg("$ME��I�F $"+moneyunit+" "+NUMBER_D->number_symbol(money)+" ���C���O�ΡC\n", me, 0, 1);
			set("money", copy(count(query("money", env),"+",money)), env);
			env->save();
		}
	}

	msg("$ME�q���Ѥj�Ӫ�"HIW"�["WHT"���x"NOR"���~�s���A�u��"HIC"��"NOR CYN"��"NOR"�ƶh�b�|�g�A���j�������ǤJ�����A�`�`�l�@�f��A�Pı�����e��F�\�h�C\n", me, 0, 1);

	if( floor == 100 )
       		me->start_condition(SIGHTSEEBLESS);
       	else if( floor == 120 )
       		me->start_condition(SIGHTSEEBLESS_ADV1);	
       	else if( floor == 140 )
       		me->start_condition(SIGHTSEEBLESS_ADV2);	
       	else if( floor == 160 )
       		me->start_condition(SIGHTSEEBLESS_ADV3);
}

void do_sightsee(object me, string arg)
{
	object env = environment(me);
	string city = env->query_city();
	string moneyunit = MONEY_D->city_to_money_unit(city);
	mixed money = query("sightseeing_price", env);
	
	if( count(money, ">", 0) && me->query_id(1) != query("owner", env) )
	{
		tell(me, pnoun(2, me)+"�C���@���ݤ�I $"+moneyunit+" "+NUMBER_D->number_symbol(money)+" ���O�ΡC\n�O�_�T�w�n�I�O�C���H(Yes/No)�G\n");
		input_to((: do_confirm_sightsee, me, env, moneyunit, money, 0 :));
	}
	else
		do_confirm_sightsee(me, env, moneyunit, money, 1, "y");
}

void do_scenery_setup(object me, string arg)
{
	int money;
	object env = environment(me);
	string moneyunit = MONEY_D->city_to_money_unit(env->query_city());

	if( !arg )
		return tell(me, "�ثe���C���O�ά� $"+moneyunit+" "+NUMBER_D->number_symbol(query("sightseeing_price", env))+"�C\n");
	
	if( me->query_id(1) != query("owner", env) )
		return tell(me, pnoun(2, me)+"���O���ؿv�����֦��̡C\n");

	money = to_int(arg);
		
	if( !money )
	{
		set("sightseeing_price", 0, env);
		msg("$ME�N�[���x���C���O�νվ㬰�K�O�C\n", me, 0, 1);
		env->save();
		return;
	}
	
	if( money <= 0 )
		return tell(me, "�п�J���T���ƭȡC\n");

	if( money > 100000000 )
		return tell(me, "�̤j�O�Τ��o�W�L $"+moneyunit+" "+NUMBER_D->number_symbol(100000000)+"�C\n");

	set("sightseeing_price", money, env);
	
	msg("$ME�N�[���x���C���O�νվ㬰 $"+moneyunit+" "+NUMBER_D->number_symbol(arg)+"�C\n", me, 0, 1);
	env->save();
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"front"	:
	([
		"short"	: HIC"����"NOR,
		"help"	: 
			([ 
"topics":
@HELP
    �����O�c�⪱�a�ӫ~���a��A�P�ɥ�i�H���ʯS�w�ӫ~�C
HELP,

"import":
@HELP
�N�ӫ~�W�[�����O�A�Ϊk�p�U�G[�޲z���O]
  import all		- �N�Ҧ����W���ӫ~�W�[
  import all rice	- �N�Ҧ��� rice �ӫ~�W�[
  import 3 rice		- �N 3 �� rice �ӫ~�W�[
  import rice		- �N 1 �� rice �ӫ~�W�[
HELP,

"export":
@HELP
�N�ӫ~�U�[�����O�A�Ϊk�p�U�G[�޲z���O]
  export all		- �N�Ҧ����ӫ~�U�[
  export all rice	- �N�Ҧ� rice �ӫ~�U�[
  export 3 rice		- �N 3 �� rice �ӫ~�U�[
  export rice		- �N 1 �� rice �ӫ~�U�[
  export 3 1		- �N 3 ��s���� 1 ���ӫ~�U�[
  export 2		- �N 1 ��s���� 2 ���ӫ~�U�[
HELP,

"list":
@HELP
�C�X�Ҧ��c�椤���ӫ~�����O�A�Ϊk�p�U�G
  list			- �C�X�Ҧ��c�椤���ӫ~
HELP,

"buy":
@HELP
�ʶR�ӫ~�����O�A�Ϊk�p�U�G
  buy all rice			- �R�U�Ҧ��� rice �ӫ~
  buy 3 rice			- �R�U 3 �� rice �ӫ~
  buy rice			- �R�U 1 �� rice �ӫ~
  buy 3 2			- �R�U 3 ��s���� 2 ���ӫ~
  buy 2				- �R�U 1 ��s���� 2 ���ӫ~
  buy rice to center 1 51,22	- �R�U 1 �� rice �ӫ~�ðe�F center ������ 1 �ìP�����y��(51,22)�B���ؿv��
HELP,

"gain":
@HELP
�����c��ӫ~�ұo����Q�A�Ϊk�p�U�G[�޲z���O]
  gain			- �����Ҧ���Q
HELP,

"setup":
@HELP
�]�w�ө��\�઺���O�A�Ϊk�p�U�G[�޲z���O]
  setup mode				- �����u�޲z�Ҧ��v�Ρu��~�Ҧ��v
  
  setup sell				- ��ܥ@�ɪ��~�C��H�Τ��\�c��ӫ~���ƶq�P�����
  setup sell all with 50%		- �]�w�Ҧ����~�����ʻ���ʤ��� 50%, ���ʼƶq�L��
  setup sell all with 50% for 100	- �]�w�Ҧ����~�����ʻ���ʤ��� 50%, ���w�u���� 100 ���
  setup sell all -d			- �������ʩҦ����~
  setup sell 2 with 50%			- �]�w�@�ɪ��~�C��W�s�� 2 ���~�����ʻ��欰 50%, ���ʼƶq�L��
  setup sell 2 with 50% for 100		- �]�w�@�ɪ��~�C��W�s�� 2 ���~�����ʻ��欰 50%, ���� 100 ���
  setup sell 2 -d			- �������ʥ@�ɪ��~�C��W�s�� 2 ���ӫ~

  setup price 2 120 			- �]�w�s���� 2 ���ӫ~���欰 120
  setup price 2 150%			- �]�w�s���� 2 ���ӫ~���欰����� 150%
  
  setup class 2 ���~ 			- �N�s���� 2 ���ӫ~�����쭹�~�����[�l�W�A�i�ۭq����
  setup class 2 �P�P			- '�P�P'�O�ө����S������[�A�����즹�[�l�W���ӫ~�N�i�H���ɾP��O
  setup class 2 �i��			- '�i��'�O�ө����S������[�A�����즹�[�l�W���ӫ~�N�¨Ѯi�ܡA�L�k��X
HELP,

"sell":
@HELP
�c��ӫ~�����O�A�Ϊk�p�U�G
  sell			- ��ܥثe���\�c��ӫ~�������P�ƶq
  sell all		- ��X�Ҧ����W�i�c�檺�ӫ~
  sell all rice		- ��X�Ҧ��� rice �ӫ~
  sell 3 rice		- ��X 3 �� rice �ӫ~
HELP,

"transport":
@HELP
�B�骫�~�����O�A�Ϊk�p�U�G
  transport rice to 1 51,22	- �N 1 �� rice ���~�B��ܲ� 1 �ìP�����y��(51,22)�B���ؿv��
  transport 3 rice to 2 51,22	- �N 3 �� rice ���~�B��ܲ� 2 �ìP�����y��(51,22)�B���ؿv��
  transport 2 3 to 1 51,22	- �N 2 ��s���� 3 �����~�B��� �� 1 �ìP�����y��(51,22)�B���ؿv��
HELP,

"gowork":
@HELP
��ө����ɶ����c�檫�~�ɡA�i�H�ϥΦ����O���խ״_���D
  gowork		- �״_�ө����W�䧮����c�檫�~�����D
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
		"job"	: CLERK,
		"heartbeat":5,	// ��ڮɶ� 1 �����
		"master":1,
		"action":
			([
				"import"	: (: do_import($1, $2, "products", STORE_CAPACITY) :),
				"export"	: (: do_export($1, $2, "products") :),
				"list"		: (: do_list($1, $2, "products", STORE_CAPACITY) :),
				"buy"		: (: do_buy($1, $2, "products", "ENVIRONMENT") :),
				"sell"		: (: do_sell($1, $2, "products", STORE_CAPACITY) :),
				"gain"		: (: do_gain($1, $2) :),
				"setup"		: (: do_setup($1, $2) :),
				"transport"	: (: do_transport($1, $2, "products") :),
				"auction"	: (: do_auction($1, $2, "products") :),
				"gowork"	: (: do_gowork($1, $2) :),
			]),
	]),

	"scenery":
	([
		"short"	: HIW"�["WHT"���x"NOR,
		"help"	: 
			([ 
"topics":
@HELP
    �o�̬O���ӤW���[���x�A��������R�������i�H�C���C
HELP,

"sightsee":
@HELP
�C���������O
  sightsee		- �C������
HELP,

"setup":
@HELP
�O�γ]�w���O
  setup			- �d�ݥثe�O��
  setup 1000		- �]�w�C���O�ά� 1000 ��
HELP,

"gain":
@HELP
������Q���O
  gain			- �����Ҧ���Q
HELP,

			]),
		"action":
			([
				"sightsee"	: (: do_sightsee :),
				"setup"		: (: do_scenery_setup :),
				"gain"		: (: do_gain :),
			]),
	]),
]);


string look_room(object room)
{
	string msg = "";
	object master = room->query_master();

	if( query("function", room) == "front" )
	{
		msg += "���a�R�� "HIC+query("shopping/desire", room)+NOR;
		msg += " �����R�� "HIC+query("shopping/average_desire", master)+NOR;
		msg += " �P���`�� "HIY+NUMBER_D->number_symbol(query("totalsell", master))+NOR;
		msg += " ���ޫO�� "HIG+query("shopping/security", master)+NOR;
		msg += " �ө��޲z "HIY+(sizeof(master->query_slave())+1)+"/"+query("shopping/management", master)+NOR"\n";
	}
	return msg;
}

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIC"��"NOR CYN"��"NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,INDIVIDUAL

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION

	// �}�i�����O��
	,"200000"
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,5
	
	// �̰��i�[�\�Ӽh
	,300
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,10
	
	// �ؿv���ɥN
	,1
});
