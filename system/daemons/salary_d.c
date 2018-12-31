/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : salary_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-02-24
 * Note   : 
 * Update :
 *  o 2005-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <location.h>
#include <daemon.h>
#include <message.h>
#include <money.h>
#include <skill.h>

#define UNIT	0
#define MONEY	1

#define LEVEL_SALARY	"50"
#define BASE_SALARY	"20000"

int in_paying_salary = 0;
int current_call_out = 0;
string *ids;

string query_labor_salary(object labor)
{
	int level;

	mapping skills = query("skills", labor);
	
	if( !mapp(skills) )
		return BASE_SALARY;

	foreach( string sk, mapping data in skills )
	{
		if( !SKILL_D->skill_exists(sk) )
			continue;

                level += data["level"]/(SKILL(sk))->exp_multiple();
	}
	
	return count(BASE_SALARY, "+", count(LEVEL_SALARY, "*", to_int(pow(level, 1.1))));
}

// �@�Ӥ�I�@���~��
private int pay_salary(object boss)
{
	int leadership_exp;
	object labor;
	string bossid, *labor_files, labor_file, msg, moneyunit;
	string salary, salary_paid;
	string total_salary_pay;

	labor_files = query("hirelabors", boss);
	
	if( !labor_files || !sizeof(labor_files) ) return 0;

	bossid = boss->query_id(1);

	moneyunit = MONEY_D->city_to_money_unit(query("city", boss)) || MONEY_D->query_default_money_unit();

	foreach( labor_file in labor_files )
	{
		reset_eval_cost();

		if( !objectp(labor = load_object(labor_file)) ) continue;
	
		salary = query_labor_salary(labor);
		salary_paid = query("salary_paid", boss);

		// �Y���~��w�I
		if( count(salary_paid, ">=", salary) )
			set("salary_paid", count(salary_paid, "-", salary), boss);

		// �S�~��w�I�B�I���_�{��
		else if( !(boss->spend_money(moneyunit, salary, BANK_PAYMENT)) )
		{
			// �˱K�׶W�L 300 ���|���}
			if( query("relationship/"+bossid, labor) > 300 || random(40) )
			{			
				if( userp(boss) )
					tell(boss, labor->query_idname()+"�y�Ӥ@�ʫH���D�G���M"+pnoun(2, boss)+"�I���X�ڪ� "+money(moneyunit, salary)+" �~���A���ڤ��M�@�N�d�b"+pnoun(2, boss)+"�����䰵�ơC\n");
			
				// ������Y
				addn("relationship/" + bossid, -100, labor);
				leadership_exp -= 300+random(300);

			}
			else
			{
				if( userp(boss) )
					tell(boss, pnoun(2, boss)+"�L�k��I"+labor->query_idname()+"����� "+money(moneyunit, salary)+" ���~���A"+labor->query_idname()+"�ݵM���M�a���}�F�C\n");

				leadership_exp -= 3000+random(300);

				// �n���� LABOR_D, ����L�H�o�H����
				LABOR_D->add_labor(labor, 1);

				log_file("command/fire", sprintf("%s �I���_�~���ϱo %O ��¾\n", bossid, labor ));
				
				destruct(labor);
				
				continue;
			}
		}

		total_salary_pay = count(total_salary_pay, "+", salary);

		// �W�[���u�����˱K��
		if( addn("relationship/"+bossid, 30, labor) >= 10000 )
			set("relationship/"+bossid, 9999, labor);
	
		// �W�[���󪺻�ɧޯ�
		leadership_exp += 600+random(600);
		labor->save();

	}
	
	msg = HIW"\n�S��F�멳�o���~�����ɭԤF�A������`�@�o�X�~��(�]�A�w�I���B)�G\n"HIY;

	msg += "�D"+money(moneyunit, total_salary_pay)+"\n";
	
	tell(boss, msg+NOR"\n");

	boss->add_social_exp(leadership_exp);	

	return 1;
}

private void distributed_pay_salary()
{
	object user;
	
	if( !sizeof(ids) )
	{
		CHANNEL_D->channel_broadcast("news", "�Ҥu�~��B�z�o�񧹲��C");
		in_paying_salary = 0;
		return;
	}

	user = load_user(ids[0]);
	
	ids = ids[1..];

	if( objectp(user) )
	{
		catch
		{
			if( pay_salary(user) )
				user->save();
						
			if( !userp(user) )
				destruct(user);
		};
	}

	current_call_out = call_out((: distributed_pay_salary :), 1);
}

void game_month_process()
{
	ids = all_player_ids();

	if( in_paying_salary ) return;

	in_paying_salary = 1;

	CHANNEL_D->channel_broadcast("news", "�o��Ҥu�~�ꪺ�ɭԨ�F�A���b�p��B�z�Ҧ��~����(�@ "+sizeof(ids)+" �����)�C");

	current_call_out = call_out((: distributed_pay_salary :), 1);
}

int remove()
{
	remove_call_out(current_call_out);
}
	
void create()
{


}
string query_name()
{
	return "�~���t��(SALARY_D)";
}
