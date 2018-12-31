/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : score.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-6-29
 * Note   : score ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <buff.h>

inherit COMMAND;

string help = @HELP
�d�ߨ��⪬�A
    
score	- �d�ߨ���򥻪��A
score s	- �ֳt�d�߯�O��(�w�]�O�W���O hp)
score m - �d�ߨ���{���P�s��

score �i�ϥΨt���Y�g sc

HELP;

private string stat_symbol(int cur, int max)
{
	if( cur >= max)
		return NOR RED+cur+" / "HIR+max+NOR;
	else if( cur >= max*3/4 )
		return NOR YEL+cur+" / "HIY+max+NOR;
	else
		return NOR GRN+cur+" / "HIG+max+NOR;
}

private varargs string bonus(int bonus, string unit)
{
	if( bonus > 0 )
		return NOR"("CYN"+"HIC+bonus+(unit||"")+NOR")";
	else if( bonus == 0 )
		return "";
	else
		return NOR"("RED"-"HIR+(-bonus)+(unit||"")+NOR")";
}

private varargs string regen(int r)
{
	if( r > 0 )
		return NOR CYN"+"HIC+r+NOR;
	else if( r == 0 )
		return "";
	else
		return NOR RED"-"HIR+(-r)+NOR;
}

string score_standard(object ob)
{
	string msg;
	mapping dbase = ob->query_database();	

	string mycity = dbase["city"];
	string myenterprise = dbase["enterprise"];
	string mytitle = dbase["title"];
	string mynickname = dbase["nickname"];
	string myidname = ob->query_idname();
	int mytalent = dbase["exp"]["talent"];
	int myage = dbase["age"];
	
	int mystr = ob->query_str();
	int myphy = ob->query_phy();
	int myint = ob->query_int();
	int myagi = ob->query_agi();
	int mycha = ob->query_cha();
	
	int mystrbuff = ob->query_all_buff(BUFF_STR);
	int myphybuff = ob->query_all_buff(BUFF_PHY);
	int myintbuff = ob->query_all_buff(BUFF_INT);
	int myagibuff = ob->query_all_buff(BUFF_AGI);
	int mychabuff = ob->query_all_buff(BUFF_CHA);
	
	int mysocialcur = ob->query_social_exp_cur();
	int mysocialtot = ob->query_social_exp_tot();
	int mysocialexpbuff = myint*2-20+ob->query_all_buff(BUFF_SOCIAL_EXP_BONUS);
	
	int mystaminacur = ob->query_stamina_cur();
	int mystaminamax = ob->query_stamina_max();
	int mystaminamaxbuff = ob->query_all_buff(BUFF_STAMINA_MAX);
	int myhealthcur = ob->query_health_cur();
	int myhealthmax = ob->query_health_max();
	int myhealthmaxbuff = ob->query_all_buff(BUFF_HEALTH_MAX);
	int myenergycur = ob->query_energy_cur();
	int myenergymax = ob->query_energy_max();
	int myenergymaxbuff = ob->query_all_buff(BUFF_ENERGY_MAX);
	
	int myfoodcur = ob->query_food_cur();
	int myfoodmax = ob->query_food_max();
	int myfoodmaxbuff = ob->query_all_buff(BUFF_FOOD_MAX);
	int mydrinkcur = ob->query_drink_cur();
	int mydrinkmax = ob->query_drink_max();
	int mydrinkmaxbuff = ob->query_all_buff(BUFF_DRINK_MAX);

	msg = sprintf(@SCORE_STANDARD
[1;37m�~�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w��[0;1;30m
�U[m%-70s[1;30m�U
�u�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w [1;36m��[0;36m�A [1;30m�t
�U[m �O�q [1;30mStr[0m %-15s[1;30m�U[m �~�� %-4d  ���� %-21s     [1;30m�U
�U[m ��� [1;30mPhy[0m %-15s[1;30m�U[m �ѽ� %-4d  ���� %-21s     [1;30m�U
�U[m ���� [1;30mInt[0m %-15s[1;30m�U                                           [1;30m�U
�U[m �ӱ� [1;30mAgi[0m %-15s[1;30m�U[m                                           [1;30m�U
�U[m �y�O [1;30mCha[0m %-15s[1;30m�U[m ���|�g��%-7s  %-25s[1;30m�U
�U                                                                      �U
�U[m ��O %5d / %-12s%-40s%-4s[1;30m�U
�U[m �ͩR %5d / %-12s%-40s%-4s[1;30m�U
�U[m �믫 %5d / %-12s%-40s%-4s[1;30m�U
SCORE_STANDARD

        ,"�i"+(CITY_D->query_city_name(mycity)||"�L����")+"�j"+(ENTERPRISE_D->query_enterprise_color_id(myenterprise)||"")+(mytitle?" "+mytitle:"")+(mynickname?" "+mynickname:"")+" "+myidname
        ,mystr+bonus(mystrbuff), myage, stat_symbol(myfoodcur, myfoodmax)+bonus(myfoodmaxbuff)
        ,myphy+bonus(myphybuff), mytalent, stat_symbol(mydrinkcur, mydrinkmax)+bonus(mydrinkmaxbuff)
        ,myint+bonus(myintbuff)
        ,myagi+bonus(myagibuff)
        ,mycha+bonus(mychabuff), bonus(mysocialexpbuff, "%"), mysocialcur+" / "+mysocialtot
        ,mystaminacur, mystaminamax+bonus(mystaminamaxbuff), GRAPH_D->graph(mystaminacur,mystaminamax,2,0,20), regen(ob->query_all_buff(BUFF_STAMINA_REGEN))  
	,myhealthcur , myhealthmax+bonus(myhealthmaxbuff), GRAPH_D->graph(myhealthcur,myhealthmax,1,0,20), regen(ob->query_all_buff(BUFF_HEALTH_REGEN))
        ,myenergycur , myenergymax+bonus(myenergymaxbuff), GRAPH_D->graph(myenergycur,myenergymax,4,0,20), regen(ob->query_all_buff(BUFF_ENERGY_REGEN))
        );
        
        if( sizeof(dbase["condition"]) )
        {
        	msg += NOR WHT"�U                                                                      �U\n"NOR;
        	foreach( string condition, mapping data in dbase["condition"] )
        		msg += sprintf(NOR WHT"�U "NOR"%-23s %-45s"NOR WHT"�U\n"NOR, replace_string(condition,"#", "/")->query_condition_name(), HIW+(data["time"]/60)+NOR WHT" min "HIW+(data["time"]%60)+NOR WHT" sec"NOR );
        }
        
        if( sizeof(dbase["stock"]) )
        {
        	float ownstockvalue, curstockvalue;
        	string stockcolor, stockdesc;
        	msg += "�U                                                                      �U\n";
        	foreach( string enterprise, mapping data in dbase["stock"] )
        	{
        		if( wizardp(ob) && !ENTERPRISE_D->enterprise_exists(enterprise) )
        		{
        			delete("stock/"+enterprise, ob);
        			continue;
        		}
        			
        		ownstockvalue = data["value"];
        		curstockvalue = ENTERPRISE_D->query_enterprise_info(enterprise, "stockvalue");
        		
        		if( ownstockvalue > curstockvalue )
        		{
        			stockcolor = HIG;
        			stockdesc = "��";
        		}
        		else if( ownstockvalue < curstockvalue )
        		{
        			stockcolor = HIR;
        			stockdesc = "��";
        		}
        		else
        		{
        			stockcolor = HIW;
        			stockdesc = "";
        		}

        		msg += sprintf(NOR WHT"�U "NOR"%-19s%8s  %-20s%-20s"NOR WHT"�U\n"NOR, ENTERPRISE_D->query_enterprise_color_id(enterprise), WHT +data["amount"] +NOR" �i", "���Ѫѻ� "HIW+sprintf("%.2f", ownstockvalue||0.0)+NOR, "�ثe�ѻ� "+stockcolor+sprintf("%.2f", curstockvalue||0.0)+stockdesc+NOR );
        	}
        }
        msg += "[1;37m���w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"+CITY_D->query_age_name(query("city", ob))+"�w��[m\n";
        
        return msg;
}

#define MAX_MONEY	"999999999999999999999999999999999999999999999"
private string score_money(object ob)
{
	string bodymsg = "", bankmsg = "";
	mapping dbase = ob->query_database();
	mapping moneydata = MONEY_D->query_moneydata(ob->query_id(1));
	mapping cash_info;
	mapping bank_info;
	object room;
	
	if( mapp(moneydata) )
	{
		cash_info = moneydata["cash"];
		bank_info = moneydata["bank"];
	}
	
	if( mapp(cash_info) && sizeof(cash_info) )
		foreach( string unit in sort_array(keys(cash_info), 1) )
		{
			if( count(cash_info[unit], ">", MAX_MONEY) )
				set("money/"+unit, MAX_MONEY, ob);
			bodymsg += sprintf("[1;30m�U   [m$%-2s %-63s[0;1;30m�U\n", unit, NUMBER_D->number_symbol(cash_info[unit]));
		}
	else
		bodymsg += "[1;30m�U   [m���W�S������{��                                                  [0;1;30m �U\n";

	if( mapp(bank_info) && sizeof(bank_info) )
	foreach( string bankfile, mixed data in bank_info )
	{
		if( !file_exist(bankfile+".o") || !objectp(room = load_object(bankfile)) )
		{
			//delete("banks/"+bankfile, ob);
			continue;
		}

		bankmsg += sprintf("[1;30m�U [m%-69s[0;1;30m�U\n", query("short", room) + (MONEY_D->is_bank_autotransfer(ob->query_id(1), bankfile)?" (�۰���b�G"HIG"�}��"NOR")":" (�۰���b�G"HIR"����"NOR")") );
		
		data = query("savings/"+ob->query_id(1), room);
		
		if( mapp(data) && mapp(data = data["money"]) )
		{
			foreach( string unit, string money in data )
				bankmsg += sprintf("[1;30m�U   [m$%-2s %-62s[0;1;30m �U\n", unit, NUMBER_D->number_symbol(money));
		}
	}
	else
		bankmsg += "[1;30m�U   [m�Ȧ�S������s��                                                  [0;1;30m �U\n";

	return sprintf(@SCORE_STANDARD

[1;37m�~�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w��[0;1;30m
�U[m%-70s[1;30m�U
�u�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w [1;33m��[0;33m�� [1;30m�t
�U[m [1;33m�{[0;33m��                                                               [0;1;30m  �U
%s
%s
[1;37m���w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w%s�w��[m

SCORE_STANDARD
,"�i"+(CITY_D->query_city_name(dbase["city"])||"�L����")+"�j"+(ENTERPRISE_D->query_enterprise_color_id(dbase["enterprise"])||"")+(dbase["title"]?" "+dbase["title"]:"")+(dbase["nickname"]?" "+dbase["nickname"]:"")+" "+ob->query_idname()
        ,bodymsg[0..<2], bankmsg[0..<2], CITY_D->query_age_name(dbase["city"]));
}

string score_simple(object ob)
{
	return sprintf("���骬�A�G"HIG"��O[%d/%d]"NOR"�B"HIR"�ͩR[%d/%d]"NOR"�B"HIB"�믫[%d/%d]"NOR"�C\n"
		,ob->query_stamina_cur(), ob->query_stamina_max()
		,ob->query_health_cur(), ob->query_health_max()
		,ob->query_energy_cur(), ob->query_energy_max()); 
}

private void do_command(object me, string arg)
{
        object ob;
        string msg, arg2;

	if( arg && arg != "s" && arg != "m" )
	{
		if( wizardp(me) )
		{
			if( !objectp(ob = find_player(arg) || present(arg)) || !ob->is_living()  )
			{
				if( sscanf(arg, "%s %s", arg, arg2) == 2 )
				{
					if( objectp(ob = find_player(arg) || present(arg)) && ob->is_living() )
						arg = arg2;
					else
						ob = me;
				}
				else
					ob = me;
			}
		}
		else if( objectp(ob = present(arg)) && ob->is_living() )
		{
			if( query("boss", ob) != me->query_id(1) )
				return tell(me, ob->query_idname()+"�ä��O"+pnoun(2, me)+"���Ϊ��Ҥu�C\n");
		}
		else
			ob = me;
	}
	else
		ob = me;	

	switch(arg)
	{
		case "m":
		case "money":
			if( ob->is_npc() )
				return tell(me, "�L�k�d��"+ob->query_idname()+"��������ơC\n");

			msg = score_money(ob);
			break;
		
		case "s":
		case "simple":
			msg = score_simple(ob);
			break;
		default:
			msg = score_standard(ob);
			break;
	}
	
	tell(me, msg);
}                             