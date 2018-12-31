/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : stock.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-05-31
 * Note   : �Ҩ�����
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

#define PROCEDURE_FUND	50		// 2/100 ������O
#define MAX_OWN_STOCK_TYPE 10		// �̤j�i�P�ɫ������Ѳ�����

string stockvalue_change_description(float change)
{
	change = to_float(change);

	if( change > 0 )
		return sprintf(HIR"��%5.2f"NOR, change);
	else if( change < 0 )
		return sprintf(HIG"��%5.2f"NOR, -change);
	else
		return sprintf(HIW"  %5.2f"NOR, change);
}


void do_list(object me, string arg)
{
	int i, sort;
	string amount, totalvalue;
	float value;
	string enterprise;
	string sortdesc;
	mapping data;
	string *enterprise_stockvalue_sort;
	string msg = "���~�W��                   �ѻ� �ƦW �̷s���^   �i�ʱi��   �ثe����  ���Ѫѻ�  �����`�� \n";
	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";

	enterprise_stockvalue_sort = sort_array(ENTERPRISE_D->query_all_enterprises(), (: ENTERPRISE_D->query_enterprise_info($2, "stockvalue") > ENTERPRISE_D->query_enterprise_info($1, "stockvalue") ? 1 : -1 :));
		
	foreach(enterprise in ENTERPRISE_D->query_all_enterprises())
	{
		data = ENTERPRISE_D->query_enterprise_info(enterprise);
		
		value = to_float(query("stock/"+enterprise+"/value", me));
		amount = query("stock/"+enterprise+"/amount", me);
		totalvalue = count(count(amount, "*", to_int(value*1000000)), "/", 100);
		
		sort = member_array(enterprise, enterprise_stockvalue_sort)+1;
		
		switch(sort)
		{
			case 1: 	sortdesc = HIY BLINK + sort + NOR; break;
			case 2..10: 	sortdesc = HIY+ sort + NOR; break;
			default: 	sortdesc = NOR YEL + sort +NOR; break;
		}
				
		msg += sprintf("%-3d %-20s%7.2f %-4s %-8s %10s %10s   %7.2f  %s\n",
			++i,
			data["color_id"], 
			data["stockvalue"], 
			sortdesc,
			stockvalue_change_description(data["stockvalue_change"]),
			NUMBER_D->number_symbol(data["available_stockamount"]),
			NUMBER_D->number_symbol(amount),
			value,
			NUMBER_D->number_symbol(totalvalue)
			);
	}
	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
	msg += "�Ѳ���쬰 10,000 ��/�i�A�������O�� 2%�A�ѥ��C 10 �����ܰʤ@��\n\n";
	
	
	return me->more(msg);
}

void confirm_buystock(object me, string enterprise, string cost, string cost_extra, string new_amount, float new_value, float cur_value, int amount, string new_available_stockamount, float percent, string arg)
{
	if( !arg )
		return me->finish_input();
		
	arg = lower_case(arg);
	
	if( arg != "y" && arg != "yes" )
	{
		tell(me, "�����ʤJ�Ѳ��C\n");
		return me->finish_input();
	}

	if( !me->spend_money(MONEY_D->query_default_money_unit(), count(cost, "+", cost_extra)) )
	{
		tell(me, pnoun(2, me)+"���W�S�� "+money(MONEY_D->query_default_money_unit(), count(cost, "+", cost_extra))+"�C\n");
		return me->finish_input();
	}
	
	set("stock/"+enterprise+"/amount", new_amount, me);
	set("stock/"+enterprise+"/value", new_value, me);
		
	ENTERPRISE_D->set_enterprise_info(enterprise, "available_stockamount", new_available_stockamount);
	CHANNEL_D->channel_broadcast("stock", me->query_idname()+"�H "+sprintf("%.2f", cur_value)+" �ѻ��ʤJ"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"�Ѳ� "+NUMBER_D->number_symbol(amount)+" �i�A���Ѥ�Ҥɬ� "+sprintf("%.2f", percent)+"%");
	me->finish_input();
	
	TOP_D->update_top_stock(me);
	me->save();
}

void do_buystock(object me, string arg)
{
	int i, num, amount;
	string enterprise;
	mapping data;
	float old_value;
	float new_value;
	float cur_value;
	string old_amount;
	string new_amount;
	string new_available_stockamount;
	float percent;
	string cost;
	string cost_extra;
	
	if( !arg || sscanf(arg, "%d %d", num, amount) != 2 )
		return tell(me, "�п�J���T���榡�C\n");

	if( num <= 0 || amount <= 0 )
		return tell(me, "�п�J�j�� 0 ���Ʀr�C\n");

	i=0;
	foreach(enterprise in ENTERPRISE_D->query_all_enterprises())
	{
		if( ++i != num ) continue;
		
		data = ENTERPRISE_D->query_enterprise_info(enterprise);
		
		if( wizardp(me) )
		{
			if( !SECURE_D->is_wizard(data["president"]) )
				return tell(me, "�ʶR�Ѳ��|�ɭP�Y���~��Q�y�������z�w�A�Ův�u���ʶR�Ův�ҫإߪ����~�Ѳ��C\n");
		}
		else if( SECURE_D->is_wizard(data["president"]) )
			return tell(me, "�L�k�ʶR�Ův�إߪ����~�Ѳ��C\n");
		
		if( count(amount, ">", data["available_stockamount"]) )
			return tell(me, "�ثe�i�ʶR���i�ƥu�� "+NUMBER_D->number_symbol(data["available_stockamount"])+" �i�C\n");
			
		new_available_stockamount = count(data["available_stockamount"], "-", amount);

		cost = count(count(amount, "*", to_int(data["stockvalue"]*1000000)), "/", 100);
		cost_extra = count(cost, "/", PROCEDURE_FUND);

		old_amount = query("stock/"+enterprise+"/amount", me);
		new_amount = count(amount, "+", old_amount);
		
		cur_value = data["stockvalue"];
		old_value = to_float(query("stock/"+enterprise+"/value", me));
		new_value = to_float(count(count(count(to_int(old_value*1000000), "*", old_amount), "+", count(to_int(cur_value*1000000), "*", amount)), "/", new_amount))/1000000.;
		
		percent = to_float(count(count(new_amount, "*", 10000), "/", data["stockamount"]))/100.;
		
		if( !wizardp(me) && percent > 20 )
			return tell(me, "�ӤH�����Ѥ�Ҥ��o�W�L 20%�C\n");
		
		if( sizeof(query("stock", me)) >= MAX_OWN_STOCK_TYPE )
		{
			if( undefinedp(query("stock/"+enterprise, me)) )
				return tell(me, "�P�ɫ��Ѻ������o�W�L "+MAX_OWN_STOCK_TYPE+" �ءC\n");
		}
		tell(me,
			HIY"�Ѳ��ثe�ѻ�  "NOR YEL+sprintf("%.2f", cur_value)+"\n"
			HIY"���ѶR�J����  "NOR YEL+sprintf("%.2f", old_value)+"\n"
			HIY"�����������  "NOR YEL+sprintf("%.2f", new_value)+"\n"
			HIY"���ʪѲ��i��  "NOR YEL+NUMBER_D->number_symbol(amount)+"\n"
			HIY"���ʪѲ��`��  "NOR YEL+money(MONEY_D->query_default_money_unit(), cost)+"\n"
			HIY"�������O��  "NOR YEL+money(MONEY_D->query_default_money_unit(), cost_extra)+" (2%)\n"
			HIY"�ʤJ�`�@��O  "NOR YEL+money(MONEY_D->query_default_money_unit(), count(cost, "+", cost_extra))+"\n"
			HIY"���Ѥ�Ҥɬ�  "NOR YEL+sprintf("%.2f", percent)+"%\n"
			HIY"�O�_�T�w�ʤJ�Ѳ�?"NOR YEL"(Yes/No)"NOR
		);

		input_to((: confirm_buystock, me, enterprise, cost, cost_extra, new_amount, new_value, cur_value, amount, new_available_stockamount, percent :));
			
		return;
	}
	
	tell(me, "�èS���o�ӽs�����Ѳ��C\n");
}

void confirm_sellstock(object me, string enterprise, string earn, string earn_extra, string new_amount, float old_value, float cur_value, int amount, string new_available_stockamount, float percent, string arg)
{
	if( !arg )
		return me->finish_input();
		
	arg = lower_case(arg);
	
	if( arg != "y" && arg != "yes" )
	{
		tell(me, "�����c��Ѳ��C\n");
		return me->finish_input();
	}
	
	me->earn_money(MONEY_D->query_default_money_unit(), count(earn, "-", earn_extra));
	
	if( count(new_amount, "<=", 0) )
		delete("stock/"+enterprise, me);
	else
		set("stock/"+enterprise+"/amount", new_amount, me);
		
	ENTERPRISE_D->set_enterprise_info(enterprise, "available_stockamount", new_available_stockamount);
	CHANNEL_D->channel_broadcast("stock", me->query_idname()+"�H "+sprintf("%.2f", cur_value)+" �ѻ���X"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"�Ѳ� "+NUMBER_D->number_symbol(amount)+" �i�A���Ѥ�ҭ��� "+sprintf("%.2f", percent)+"%");	
	me->finish_input();
	
	TOP_D->update_top_stock(me);
	me->save();
}

void do_sellstock(object me, string arg)
{
	int i, num, amount;
	string old_amount;
	string new_amount;
	float cur_value;
	float old_value;
	string earn;
	string earn_extra;
	float percent;
	string enterprise;
	mapping data;
	string new_available_stockamount;

	if( !arg || sscanf(arg, "%d %d", num, amount) != 2 )
		return tell(me, "�п�J���T���榡�C\n");

	if( num <= 0 || amount <= 0 )
		return tell(me, "�п�J�j�� 0 ���Ʀr�C\n");
	
	i=0;
	foreach(enterprise in ENTERPRISE_D->query_all_enterprises())
	{
		if( ++i != num ) continue;
		
		data = ENTERPRISE_D->query_enterprise_info(enterprise);
		
		old_amount = copy(query("stock/"+enterprise+"/amount", me));
		new_amount = count(old_amount, "-", amount);
		
		if( count(amount, ">", old_amount) )
			return tell(me, pnoun(2, me)+"�ثe�֦�"+data["color_id"]+"���Ѳ��i�ƥu�� "+NUMBER_D->number_symbol(old_amount)+" �i�C\n");
			
		new_available_stockamount = count(data["available_stockamount"], "+", amount);
		
		cur_value = data["stockvalue"];
		old_value = to_float(query("stock/"+enterprise+"/value", me));
		earn = count(count(amount, "*", to_int(cur_value*1000000)), "/", 100);
		earn_extra = count(earn, "/", PROCEDURE_FUND);
		
		percent = to_float(count(count(new_amount, "*", 10000), "/", data["stockamount"]))/100.;

		tell(me,
			HIY"�Ѳ��ثe�ѻ�  "NOR YEL+sprintf("%.2f", cur_value)+"\n"
			HIY"���ѥ�������  "NOR YEL+sprintf("%.2f", old_value)+"\n"
			HIY"����Ѳ��i��  "NOR YEL+NUMBER_D->number_symbol(amount)+"\n"
			HIY"����Ѳ��`��  "NOR YEL+money(MONEY_D->query_default_money_unit(), earn)+"\n"
			HIY"�������O��  "NOR YEL+money(MONEY_D->query_default_money_unit(), earn_extra)+" (2%)\n"
			HIY"��X�`�@��o  "NOR YEL+money(MONEY_D->query_default_money_unit(), count(earn, "-", earn_extra))+"\n"
			HIY"���Ѥ�ҭ���  "NOR YEL+sprintf("%.2f", percent)+"%\n"
			HIY"�O�_�T�w��X�Ѳ�?"NOR YEL"(Yes/No)"NOR
		);
		
		//tell(me, "�O�_�T�w�H "+sprintf("%.2f", cur_value)+" �ѻ���X"+data["color_id"]+"�Ѳ� "+NUMBER_D->number_symbol(amount)+" �i\n�o�� "+money(MONEY_D->query_default_money_unit(), earn)+"�A���Ѥ�ұN�� "+sprintf("%.2f", percent)+"%(Yes/No)�G");
		
		input_to((: confirm_sellstock, me, enterprise, earn, earn_extra, new_amount, old_value, cur_value, amount, new_available_stockamount, percent :));
			
		return;
	}
	
	tell(me, "�èS���o�ӽs�����Ѳ��C\n");
}


// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"hall"	:
	([
		"short"	: NOR HIW"����Ҥj�U"NOR,
		"help"	:
			([
"topics":
@HELP
    �Ҩ������a��C
HELP,

"list":
@HELP
��ܪѥ���T
list		�C�X�Ҧ��ѥ����
HELP,

"buy":
@HELP
�R�J�Ѳ������O
buy 2 300	�R�J�� 2 �ӪѲ� 300 �i
HELP,

"sell":
@HELP
��X�Ѳ������O
sell 3 100	��X�� 3 �ӪѲ� 100 �i
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job": 0,
		"action":
			([
				"list" : (: do_list :),
				"buy" : (: do_buystock :),
				"sell" : (: do_sellstock :),
			]),
	]),
]);


// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIG"�Ҩ�"NOR GRN"�����"NOR

	// �}�i���ؿv�����̤֩ж�����
	,2

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,5

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,ENTERPRISE

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,COMMERCE_REGION

	// �}�i�����O��
	,"75000000"
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,0
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});
