/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : enterprise.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-03-31
 * Note   : ���~�j��
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
#include <condition.h>
#include <delay.h>

inherit ROOM_ACTION_MOD;

void do_invest(object me, string arg)
{
	string moneyunit, money, enterprise, default_moneyunit;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n���άO�h��h�ֿ��H\n");
		
	enterprise = query("enterprise", me);
	default_moneyunit = MONEY_D->query_default_money_unit();
	
	if( sscanf(arg, "$%s %s", moneyunit, money) == 2 )
	{
		moneyunit = upper_case(moneyunit);

		if( !(money = big_number_check(money)) )
			return tell(me, "�п�J���T���Ʀr�C\n");

		if( moneyunit != default_moneyunit )
			return tell(me, "���~���Ϊ��W��ΰh��ȱ��� $"+default_moneyunit+" �������f���C\n");
				
		if( count(money, ">", 0) )
		{
			if( !me->spend_money(moneyunit, money) )
				return tell(me, pnoun(2, me)+"�S���o��h���{���C\n");
			
			ENTERPRISE_D->change_assets(enterprise, money);
			ENTERPRISE_D->change_invest(enterprise, me->query_id(1), money);
			
			CHANNEL_D->channel_broadcast("ent", me->query_idname()+"�W�� "HIY+money(default_moneyunit, money)+NOR" �@�����~����C\n", me);
		}
		else if( count(money, "<", 0) )
		{
			money = money[1..];

			if( !ENTERPRISE_D->change_assets(enterprise, "-"+money) )
				return tell(me, "�ثe���~��´���èS������h������i�ѩP��C\n");
			
			me->earn_money(default_moneyunit, money);
			ENTERPRISE_D->change_invest(enterprise, me->query_id(1), "-"+money);

			CHANNEL_D->channel_broadcast("ent", me->query_idname()+"�q���~���Τ��h�� "HIY+money(default_moneyunit, money)+NOR"�C\n", me);
		}
		else 
			return tell(me, pnoun(2, me)+"�Q�n���ΰh��h�ֿ��H\n");
	}
	else
		return tell(me, "�� help invest ��J���T�����O�榡�C\n");
	
}

void do_read(object me, string arg)
{
	object env = environment(me);
	
	if( query("enterprise", me) != query("owner", env)[11..] )
		return tell(me, pnoun(2, me)+"���O�o�����~�������L�k�ϥγo���\Ū�ǡC\n");

	msg("$ME�H�⮳�_�@���]�g���x�}�l�\Ū...�C\n", me, 0, 1);
	me->start_delay(REST_DELAY_KEY, 10, pnoun(2, me)+"���b�\Ū���x�C\n", "�\Ū�����C\n");
        me->start_condition(FINANCEKNOWLEDGE);
}

void do_release(object me, string arg)
{
	string stockamount;
	string available_stockamount;
	string enterprise;
	object env = environment(me);
	
	if( !wizardp(me) )
		return tell(me, "���դ��C\n");

	enterprise = query("owner", env)[11..];
	
	if( !ENTERPRISE_D->is_president(enterprise, me->query_id(1)) )
		return tell(me, pnoun(2, me)+"���O�o�ӥ��~���`���C\n");

	if( count(arg, "<=", 0) )
		return tell(me, "�п�J�j�� 0 ���Ʀr�C\n");
		
	stockamount = count(arg, "+", ENTERPRISE_D->query_enterprise_info(enterprise, "stockamount"));
	available_stockamount = count(arg, "+", ENTERPRISE_D->query_enterprise_info(enterprise, "available_stockamount"));
	CHANNEL_D->channel_broadcast("stock", ENTERPRISE_D->query_enterprise_color_id(enterprise)+"���~���Ϋť����X�Ѳ� "HIC+NUMBER_D->number_symbol(arg)+NOR" �i�A�`���X�Ѳ� "HIC+NUMBER_D->number_symbol(stockamount)+NOR" �i�C\n");
	
	ENTERPRISE_D->set_enterprise_info(enterprise, "stockamount", stockamount);
	ENTERPRISE_D->set_enterprise_info(enterprise, "available_stockamount", available_stockamount);
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"fund"	:
	([
		"short"	: WHT"����B�z��"NOR,
		"help"	:
			([
"topics":
@HELP
    �B�z������a��C
invest $WZ 10000	�N�ӤH������i�J���~����
invest $RW -10000	�h�^�ӤH�����
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job": 0,
		"action":
			([
				"invest" : (: do_invest :),
			]),
	
	]),
	"reading"	:
	([
		"short"	: WHT"�\Ū��"NOR,
		"help"	:
			([
"topics":
@HELP
    ���~�������ϮѫǡC
read			�\Ū���y
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job": 0,
		"action":
			([
				"read" : (: do_read :),
			]),
	
	]),	
	"directorate"	:
	([
		"short"	: WHT"���Ʒ|"NOR,
		"help"	:
			([
"topics":
@HELP
    ���~���ƿ줽���a��C
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job": 0,
		"action":
			([
				"release" : (: do_release :),
			]),
	
	]),	
	"president"	:
	([
		"short"	: HIW"�`��"WHT"�줽��"NOR,
		"help"	:
			([
"topics":
@HELP
    ���~�`���줽���a��C
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job": 0,
		"action":
			([

			]),
	]),
]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	WHT"���~�j��"NOR

	// �}�i���ؿv�����̤֩ж�����
	,4

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,5

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,ENTERPRISE

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,COMMERCE_REGION

	// �}�i�����O��
	,"30000000"
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,0
	
	// �̰��i�[�\�Ӽh
	,5
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});
