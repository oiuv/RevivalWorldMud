/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : recycle.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-03
 * Note   : �U���^�����ʧ@����
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

inherit ROOM_ACTION_MOD;

#define RECYCLE_MONEY	10

void do_recycle(object me, string arg)
{
	//mixed value;
	//string money = "0", moneyunit;
	string msg="", amount;
	object ob, env = environment(me);

	if( !arg )
		return tell(me, "�ثe�^���`�ơG"+NUMBER_D->number_symbol(query("numbers", env))+" �ӡC\n", CMDMSG);

	if( sscanf(arg, "%s %s", amount, arg) == 2 )
	{
		if( amount == "all" ) ;
		// �p�G amount �ä��O�Ʀr���
		else if( !big_number_check(amount) )
		{
			arg = amount + " " + arg;
			amount = "1";	
		}	
		else if( count(amount, "<", 1) ) amount = "1";
	}
	else amount = "1";
	
	//moneyunit = MONEY_D->city_to_money_unit(env->query_city());
	
	if( objectp(ob = present(arg, me)) )
	{
		if( amount != "all" && count(amount, ">", query_temp("amount", ob)||1) ) 
			return tell(me, pnoun(2, me)+"���W�u�� "+(query_temp("amount",ob)||1)+" "+(query("unit",ob)||"��")+ob->query_idname()+"�C\n", CMDMSG);
		
		if( ob->is_keeping() )
			return tell(me, pnoun(2, me)+"�������Ѱ�"+ob->query_idname()+"���O�d(keep)���A�C\n");

		if( amount == "all" )
			amount = (query_temp("amount", ob)||"1")+"";
		
		// �]�׮t�L�j�|�y���R����Q, �Ȯ� mark
		//value = query("value", ob);
		//if( count(value, ">", RECYCLE_MONEY) || count(value, "<=", 0) )
		//	money = count(amount, "*", RECYCLE_MONEY);
		//else
		//	money = count(amount, "*", count(value, "/", 2));
		
		//me->earn_money(moneyunit, money);

		set("numbers", count(query("numbers", env), "+", amount), env);
		msg("$ME��"+QUANTITY_D->obj(ob, amount)+"��i�귽�^����A�ثe�^���`�ơG"+NUMBER_D->number_symbol(query("numbers", env))+" �ӡC\n", me, 0, 1);
		
		destruct(ob, amount);
		return;
	}
	
	if( lower_case(arg) == "all" )
	{
		object *all_ob = all_inventory(me);
		
		if( sizeof(all_ob) )
		{
			foreach(ob in all_ob)
			{
				amount = query_temp("amount",ob)||"1";
				
				if( ob->is_keeping() || (count(query("value", ob),">",0) && query_temp("endurance", ob)>=0) ) continue;
			
				msg += HIG"�D"NOR+QUANTITY_D->obj(ob, amount, 1)+"\n";
				
				//value = query("value", ob);

				//if( count(value, ">", RECYCLE_MONEY) || count(value, "<=", 0) )
				//	money = count(money, "+", count(amount, "*", RECYCLE_MONEY));
				//else
				//	money = count(money, "+", count(amount, "*", count(value, "/", 2)));
				
				set("numbers", count(query("numbers", env), "+", amount), env);
				destruct(ob);
			}

			//me->earn_money(moneyunit, money);

			return msg("$ME�⨭�W����^�����F�������i�귽�^����F�C�ثe�^���`�ơG"+NUMBER_D->number_symbol(query("numbers", env))+" �ӡC\n"NOR, me, 0, 1);
		}
		return tell(me, pnoun(2, me)+"���W�S���F��i�H�^���I\n", CMDMSG);
	}
	
	return tell(me, pnoun(2, me)+"���W�S�� "+arg+" �o�󪫫~�C\n", CMDMSG);
}
	

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"recycle"	:
	([
		"short"	: NOR GRN"�^����"NOR,
		"help"	:
			([
"topics":
@HELP
    �^�����ߥi�H�^���Ҧ����󤣨���Ȫ����~�A�P�ɬ��F���y���O�A�C�^���@�󪫫~�i�o 10 �������C
HELP,

"recycle":
@HELP
�^���귽�����O�A�Ϊk�p�U�G
  recycle		- ��ܥثe�w�^���`��
  recycle all		- �^�����W�Ҧ�������Ȫ����~
  recycle garbage	- �^�� 1 �� garbage ���~
  recycle 3 garbage	- �^�� 3 �� garbage ���~
HELP,

			]),
		"action":
			([
				"recycle"	: (: do_recycle :),
			]),
	]),
]);


// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIG"�U���^����"NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION

	// �}�i�����O��
	,"5000000"

	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,30
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});
