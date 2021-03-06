/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : labor.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-22
 * Note   : labor 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <location.h>
#include <daemon.h>
#include <npc.h>
#include <buff.h>

inherit COMMAND;

string help = @HELP
    這個指令能讓你查看目前已顧用的所有人力清單，以及徵調清單上的人力到你所
在的地方來，以方便命令他做工作或者其他事情。

labor			- 顯示所有員工列表
labor '員工編號'	- 將該編號之員工呼喚至您的面前
labor salary 1000	- 儲存 1000 元的安全預備薪資(預設為該城市的貨幣)

HELP;

void labor_call(object me, object labor)
{
	if( !objectp(me) || !objectp(labor) )
		return;

	msg("$ME收到了一封來自"+me->query_idname()+"的徵調訊息，立刻出發前往。\n", labor, 0, 1);
	
	labor->move_to_environment(me);
	
	msg("$ME氣喘虛虛地趕到了此處。\n", labor, 0, 1);
	tell(me, labor->query_idname()+"已到達目的地。\n");
}

string labor_list(object me)
{
	int i;
	object labor, env;
   	string place, job;
	string msg = "";
    	string salary, salary_paid, totalsalary, *labors = query("hirelabors", me);
    	string moneyunit = MONEY_D->city_to_money_unit(query("city", me)) || MONEY_D->query_default_money_unit();
    
    	if( !sizeof(labors) )
    		return me->query_idname()+"沒有雇用任何人力。\n";
    			
    	msg = pnoun(2, me)+"總共雇用了 "+sizeof(labors)+" 個人力：\n";
	msg += NOR WHT"─────────────────────────────────────\n"NOR;
	msg +=     HIW"編號 人力名稱          位置                         親密度 工作 月薪($"+moneyunit+")\n"NOR;
	msg += NOR WHT"─────────────────────────────────────\n"NOR;

    	foreach( string file in labors )
    	{
    		if( !file_exist(file+".o") )
    		{
    			set("hirelabors", labors - ({ file }), me);
    			if( !sizeof(query("hirelabors", me)) )
    				delete("hirelabors", me);
    			continue;
    		}

    		if( !objectp(labor = load_object(file)) )
    			error("Can't load "+file+".");
    			
    		if( query("adventure", labor) )
    			place = AREA_D->query_area_name(query("adventure/area", labor), query("adventure/num", labor));
    		else if( objectp(env = environment(labor)) )
    		{
    			if( env->is_maproom() )
    			{
    				array loc = query_temp("location", labor);
    					
    				place = loc_short(loc);
    			}
    			else
    				place = env ? HIG+(env->query_room_name()||query("short", env)||"未知名的地方")+NOR : HIR"在宇宙中飄流"NOR;
    		}
    		else
    			place = HIR"虛幻之中"NOR;

		switch( query("job/cur", labor) )
		{
			case CLERK:
				job = HIC"店"NOR CYN"員"NOR;
				break;
			case WORKER:
				job = HIY"工"NOR YEL"人"NOR;
				break;
			case ENGINEER:
				job = HIR"研"NOR RED"發"NOR;
				break;
			case ADVENTURER:
				job = HIG"探"NOR GRN"險"NOR;
				break;
			default:
				job = NOR WHT"無"NOR;
				break;
		}
		salary = SALARY_D->query_labor_salary(labor);
		totalsalary = count(salary, "+", totalsalary);
    		msg += sprintf(HIY" %-3d"NOR" %-18s%-30s%5s %-4s %s\n", ++i, noansi_strlen(labor->query_idname())<=16?labor->query_idname():labor->query_name(), place, HIG+query("relationship/"+me->query_id(1), labor)+NOR, job, HIY+NUMBER_D->number_symbol(salary)+NOR);
    	}
    	
    	salary_paid = query("salary_paid", me);
    	
    	msg += sprintf("\n "NOR YEL"每月應付薪水總額 "HIY"$%s %s"NOR YEL"，安全預備薪資 "HIY"$%s %s"NOR YEL" ("HIY"%s"NOR YEL" 個月)\n"NOR,  moneyunit, NUMBER_D->number_symbol(totalsalary), moneyunit, NUMBER_D->number_symbol(salary_paid), count(totalsalary, ">", 0) ? NUMBER_D->number_symbol(count(salary_paid, "/", totalsalary)) : "0");
    	msg += NOR WHT"─────────────────────────────────────\n"NOR;
    	
    	return msg;
}

void labor_salary(object me, string arg)
{
	string moneyunit = MONEY_D->city_to_money_unit(query("city", me)) || MONEY_D->query_default_money_unit();

	if( !arg )
		return tell(me, pnoun(2, me)+"想要儲存多少安全預備薪資？\n");

	arg = big_number_check(arg);
	
	if( count(arg, "<=", 0) )
		return tell(me, "請輸入正確的金額數字。\n");
		
	if( !me->spend_money(moneyunit, arg) )
		return tell(me, pnoun(2, me)+"身上沒那麼多 $"+moneyunit+" 錢了！。\n");
		
	set("salary_paid", count(query("salary_paid", me), "+", arg), me);
		
	msg("$ME儲存了 "HIY"$"+moneyunit+" "+NUMBER_D->number_symbol(arg)+NOR" 的安全預備薪資。\n", me, 0, 1);	
	
	me->save();
}

private void do_command(object me, string arg)
{
	int i, num;
	object labor;
	string salary;
	int waiting_time;

    	if( !arg )
    		return tell(me, labor_list(me));
	else if( (num = to_int(arg)) > 0 )
	{
		string *labors = query("hirelabors", me);
		
		if( num > sizeof(labors) )
			return tell(me, "並沒有這麼多位雇用人力。\n");
		
		foreach( string file in labors )
		{
			if( ++i == num )
			{
				if( !file_exist(file+".o") )
	    			{
	    				set("hirelabors", labors - ({ file }), me);
	    				if( !sizeof(query("hirelabors", me)) )
	    					delete("hirelabors", me);
	    				continue;
	    			}

    				if( !objectp(labor = load_object(file)) )
    					error("Can't load "+file+".");

				if( same_environment(me, labor) )
					return tell(me, labor->query_idname()+"已經在這裡了。\n");

				if( query("adventure", labor) && file_exists(query("adventure/from", labor)) && ADVENTURE_D->is_doing_adventure(query("adventure/from", labor)) )
					return tell(me, labor->query_idname()+"正在執行探險任務中，無法召回。\n");

				waiting_time = labor->query_loading()/1000;
				
				// 最少 5 秒鐘
				if( waiting_time < 5 )
					waiting_time = 5;
				
				msg("$ME捎了一封信給了"+labor->query_idname()+"叫"+pnoun(3, labor)+"立刻趕來此處(需要 "+waiting_time+" 秒的時間)。\n", me, 0, 1);
				
				if( query_temp("labor_call_out_handler", labor) )
				{
					remove_call_out(query_temp("labor_call_out_handler", labor));
					delete_temp("labor_call_out_handler", labor);
				}
				set_temp("labor_call_out_handler", call_out((: labor_call, me, labor:), waiting_time), labor);
				return;
			}
		}
	}
	else if( sscanf(arg, "salary %s", salary) == 1 )
	{
		labor_salary(me, salary);
		return;
	}
	else if( wizardp(me) && find_player(arg) )
		return tell(me, labor_list(find_player(arg)));
	
	return tell(me, "請輸入正確的指令格式(help labor)。\n");
}