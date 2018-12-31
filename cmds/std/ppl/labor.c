/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : labor.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-22
 * Note   : labor ���O
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
    �o�ӫ��O�����A�d�ݥثe�w�U�Ϊ��Ҧ��H�O�M��A�H�μx�ղM��W���H�O��A��
�b���a��ӡA�H��K�R�O�L���u�@�Ϊ̨�L�Ʊ��C

labor			- ��ܩҦ����u�C��
labor '���u�s��'	- �N�ӽs�������u�I��ܱz�����e
labor salary 1000	- �x�s 1000 �����w���w���~��(�w�]���ӫ������f��)

HELP;

void labor_call(object me, object labor)
{
	if( !objectp(me) || !objectp(labor) )
		return;

	msg("$ME����F�@�ʨӦ�"+me->query_idname()+"���x�հT���A�ߨ�X�o�e���C\n", labor, 0, 1);
	
	labor->move_to_environment(me);
	
	msg("$ME��ݵ��a����F���B�C\n", labor, 0, 1);
	tell(me, labor->query_idname()+"�w��F�ت��a�C\n");
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
    		return me->query_idname()+"�S�����Υ���H�O�C\n";
    			
    	msg = pnoun(2, me)+"�`�@���ΤF "+sizeof(labors)+" �ӤH�O�G\n";
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg +=     HIW"�s�� �H�O�W��          ��m                         �˱K�� �u�@ ���~($"+moneyunit+")\n"NOR;
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

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
    				place = env ? HIG+(env->query_room_name()||query("short", env)||"�����W���a��")+NOR : HIR"�b�t�z���Ƭy"NOR;
    		}
    		else
    			place = HIR"��ۤ���"NOR;

		switch( query("job/cur", labor) )
		{
			case CLERK:
				job = HIC"��"NOR CYN"��"NOR;
				break;
			case WORKER:
				job = HIY"�u"NOR YEL"�H"NOR;
				break;
			case ENGINEER:
				job = HIR"��"NOR RED"�o"NOR;
				break;
			case ADVENTURER:
				job = HIG"��"NOR GRN"�I"NOR;
				break;
			default:
				job = NOR WHT"�L"NOR;
				break;
		}
		salary = SALARY_D->query_labor_salary(labor);
		totalsalary = count(salary, "+", totalsalary);
    		msg += sprintf(HIY" %-3d"NOR" %-18s%-30s%5s %-4s %s\n", ++i, noansi_strlen(labor->query_idname())<=16?labor->query_idname():labor->query_name(), place, HIG+query("relationship/"+me->query_id(1), labor)+NOR, job, HIY+NUMBER_D->number_symbol(salary)+NOR);
    	}
    	
    	salary_paid = query("salary_paid", me);
    	
    	msg += sprintf("\n "NOR YEL"�C�����I�~���`�B "HIY"$%s %s"NOR YEL"�A�w���w���~�� "HIY"$%s %s"NOR YEL" ("HIY"%s"NOR YEL" �Ӥ�)\n"NOR,  moneyunit, NUMBER_D->number_symbol(totalsalary), moneyunit, NUMBER_D->number_symbol(salary_paid), count(totalsalary, ">", 0) ? NUMBER_D->number_symbol(count(salary_paid, "/", totalsalary)) : "0");
    	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
    	
    	return msg;
}

void labor_salary(object me, string arg)
{
	string moneyunit = MONEY_D->city_to_money_unit(query("city", me)) || MONEY_D->query_default_money_unit();

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�x�s�h�֦w���w���~��H\n");

	arg = big_number_check(arg);
	
	if( count(arg, "<=", 0) )
		return tell(me, "�п�J���T�����B�Ʀr�C\n");
		
	if( !me->spend_money(moneyunit, arg) )
		return tell(me, pnoun(2, me)+"���W�S����h $"+moneyunit+" ���F�I�C\n");
		
	set("salary_paid", count(query("salary_paid", me), "+", arg), me);
		
	msg("$ME�x�s�F "HIY"$"+moneyunit+" "+NUMBER_D->number_symbol(arg)+NOR" ���w���w���~��C\n", me, 0, 1);	
	
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
			return tell(me, "�èS���o��h�춱�ΤH�O�C\n");
		
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
					return tell(me, labor->query_idname()+"�w�g�b�o�̤F�C\n");

				if( query("adventure", labor) && file_exists(query("adventure/from", labor)) && ADVENTURE_D->is_doing_adventure(query("adventure/from", labor)) )
					return tell(me, labor->query_idname()+"���b���汴�I���Ȥ��A�L�k�l�^�C\n");

				waiting_time = labor->query_loading()/1000;
				
				// �̤� 5 ����
				if( waiting_time < 5 )
					waiting_time = 5;
				
				msg("$ME���F�@�ʫH���F"+labor->query_idname()+"�s"+pnoun(3, labor)+"�ߨ軰�Ӧ��B(�ݭn "+waiting_time+" ���ɶ�)�C\n", me, 0, 1);
				
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
	
	return tell(me, "�п�J���T�����O�榡(help labor)�C\n");
}