/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : shopping2_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-12
 * Note   : �ʪ����F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <citydata.h>
#include <object.h>
#include <map.h>
#include <npc.h>

mapping shopping;


#define MONEY_PER_RESIDENT			160		// �C��~������ 160 �������O
#define BASE_MONEY				"5000000"	// �s�����򥻮��O��O

int moneyrehashtime;
string *productlist = allocate(0);
string popproduct;

mapping query_shopping()
{
	return shopping;
}

void add_product(string filename)
{
	productlist |= ({ filename });
}

void pop_product_choose()
{
	if( sizeof(productlist) )
	{
		object product;

		for(int i=0;i<100;i++)
		{
			popproduct = productlist[random(sizeof(productlist))];
			product = load_object(popproduct);
			
			if( objectp(product) && !query("badsell", product) )
				break;
		}
		
		CHANNEL_D->channel_broadcast("news", NOR GRN"�y��S���G�ӫ~"+product->query_idname()+NOR GRN"���b���O�����W�޵o�y���ʶR����C"NOR);
	}
}

string query_pop_product()
{
	return popproduct;
}

mixed query_shopping_info(string city, int num, string prop)
{
	if( !shopping[city] || !shopping[city][num] )
		return 0;

	return shopping[city][num][prop];
}

int shopping_cost_ratio(string city, int num)
{
	return shopping[city][num]["ratio"];
}
	
void shopping_cost_money(string city, int num, string cost)
{
	mapping data = shopping[city][num];
	
	shopping[city][num]["money"] = count(data["money"], "-", cost);
	
	if( count(data["moneypersec"], "==", 0) ) return;

	if( count(cost, ">", 5000000) )
	{
		object stack = call_stack(1)[<1];
		CHANNEL_D->channel_broadcast("nch", "�ө��G"+query("owner", stack)+" "+base_name(stack)+" �o�� "+cost+" ���B�� NPC ���O�欰�C");
	}
}

void rehash_money()
{
	int num;
	string city;

	foreach(city in CITY_D->query_cities() )
	{
		if( CITY_D->query_city_info(city, "fallen") )
		{
			shopping[city][0]["moneyrehashtime"] = time();
			continue;
		}

		for(num=0;CITY_D->city_exist(city, num);num++)
		{
			shopping[city][num]["resident"] = CITY_D->query_section_info(city, num, "resident");
			shopping[city][num]["money"] = BASE_MONEY;
			shopping[city][num]["money"] = count(shopping[city][num]["resident"], "*", MONEY_PER_RESIDENT);

			shopping[city][num]["totalmoney"] = shopping[city][num]["money"];
			shopping[city][num]["moneyrehashtime"] = time();
		}
	}
}

void rehash_storelist()
{
	int num;
	string city;

	shopping = allocate_mapping(0);

	foreach(city in CITY_D->query_cities() )
	{
		shopping[city] = allocate_mapping(0);

		for(num=0;CITY_D->city_exist(city, num);num++)
		{
			shopping[city][num] = allocate_mapping(0);	
			shopping[city][num]["resident"] = CITY_D->query_section_info(city, num, "resident");
		}
	}
}

void heart_beat()
{
	int num, lacktime;
	string city, predictmoney, moneypersec;

	if( moneyrehashtime + 3600 - time() < 1 )
	{
		moneyrehashtime = time();
		rehash_money();
	}
	
	foreach(city in CITY_D->query_cities() )
	{
		for(num=0;CITY_D->city_exist(city, num);num++)
		{
			if(!shopping[city] || !shopping[city][num]) continue;

			lacktime = shopping[city][num]["moneyrehashtime"] + 3600 - time();

			moneypersec = count(shopping[city][num]["totalmoney"], "/", 3600);
			predictmoney = count(lacktime, "*", moneypersec);
			
			shopping[city][num]["predictmoney"] = predictmoney;
			shopping[city][num]["moneypersec"] = moneypersec;
			
			if( count(moneypersec, ">", 0) )
			{
				shopping[city][num]["ratio"] = to_int(count(count(shopping[city][num]["predictmoney"], "-", shopping[city][num]["money"]), "/", shopping[city][num]["moneypersec"]));
				
				if( shopping[city][num]["ratio"] < -1 )
					shopping[city][num]["ratio"] = 0;
			}
		}
	}
}

void create()
{
	rehash_storelist();
	heart_beat();
	set_heart_beat(10);
}

int remove()
{

}
string query_name()
{
	return "�ʪ��t��(SHOPPING_D)";
}
