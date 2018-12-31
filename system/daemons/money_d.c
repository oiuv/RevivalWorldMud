/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : money_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-7-3
 * Note   : �������F, �@�����Ĥ��Ϩt��, �Τ@�޲z���ȳ��, �óB�z���������ഫ
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <bank.h>
#include <ansi.h>
#include <map.h>
#include <citydata.h>
#include <daemon.h>
#include <money.h>
#include <roommodule.h>
#include <location.h>

#define DATA		"/data/daemon/money.o"

#define DEFAULT_EXCHANGE	0.7
#define DEFAULT_PRICE_INDEX	1.

#define CITY_ELEM		0
#define NUM_ELEM		1
#define BUILDING_SHORT		2
#define BUILDING_TYPE		3
#define BUILDING_WALL_TABLE	4
#define BUILDING_ROOM_TABLE	5

#define CONVERT_PRECISION	100000000
#define DEFAULT_MONEY_UNIT	"RW"

// �ֳt city ==> money_unit ��Ӫ�
mapping C2M;

// �ֳt money_unit ==> city ��Ӫ�
nosave mapping M2C;

// �Ҧ��������y�q���
private mapping moneydata;

int money_unit_exist(string money_unit)
{
	return !undefinedp(M2C[upper_case(money_unit)]);
}
	
string city_to_money_unit(string city)
{
	return C2M[city];
}

string money_unit_to_city(string money_unit)
{
	return M2C[upper_case(money_unit)];
}

int save()
{
	return save_object(DATA);
}

// �إ߷s��������t
int create_new_finance_system(string city, string money_unit)
{
	if( !stringp(city) || !stringp(money_unit) || !CITY_D->city_exist(city) ) return 0;
	
	money_unit = remove_fringe_blanks(upper_case(money_unit));
	
	if( strlen(money_unit) != 2 ) return 0; 
	
	if( !undefinedp(M2C[money_unit]) )
		return 0;

	// �إ߰�ڶײv��T
	if( !EXCHANGE_D->create_new_exchange(money_unit, DEFAULT_EXCHANGE) )
		return 0;

	// �إߪ������Ƹ�T
	CITY_D->set_city_info(city, "price_index", DEFAULT_PRICE_INDEX);

	M2C[money_unit] = city;
	C2M[city] = money_unit;
	
	return save();
}
	
// ������������ city ��������t, �]�A����ݤ��������
int remove_finance_system(string city)
{
	if( undefinedp(C2M[city]) ) return 0;
	
	EXCHANGE_D->remove_exchange(C2M[city]);
	CITY_D->delete_city_info(city, "price_index");
	
	map_delete(M2C, C2M[city]);
	map_delete(C2M, city);
	
	save();
}


/* �վ㪫������ */
float change_price_index(string city, int num, int percent)
{
	float current_price_index, new_price_index;
	if( !CITY_D->city_exist(city, num) || !percent ) return 0.;

	if( percent > 10 ) percent = 10;
	if( percent < -10 ) percent = -10;
	
	current_price_index = CITY_D->query_section_info(city, num, "price_index")||DEFAULT_PRICE_INDEX;
	
	new_price_index = current_price_index * (1. + (percent / 100.));
	
	if( new_price_index < 0.01 )
	{
		new_price_index = 0.01;
		percent = to_int(abs(100-current_price_index/0.01*100));
	}
	
	CHANNEL_D->channel_broadcast("news", sprintf("%s"HIY"����������%s�� %.6f�A%s�T�׬� %d%%�A�ϱo��a���O�N�@%s�C"NOR,
		CITY_D->query_city_idname(city, num),
		percent > 0 ? "�W��" : "�U��",
		new_price_index,
		percent > 0 ? "�W��" : "�U��",
		to_int(abs(percent)),
		percent > 0 ? "���C" : "����"
	));
		
	return CITY_D->set_section_info(city, num, "price_index", new_price_index);
}

// �Ǧ^�����[�v�᪺����
string real_prices(string city, int num, mixed amount)
{
	float price_index;
	
	if( !CITY_D->city_exist(city, num) || count(amount, "==", 0) ) return 0;
		
	price_index = CITY_D->query_section_info(city, num);
	
	return count(count(amount, "*", price_index*CONVERT_PRECISION), "/", CONVERT_PRECISION);
}

// �p��@�ɤ��ӳf�����`��
string query_world_cash(string query_unit)
{
	string id;
	mapping data;
	string totalcash;
	string bank;
	object bankroom;
	
	foreach(id, data in moneydata)
	{
		if( SECURE_D->is_wizard(id) ) continue;

		totalcash = count(totalcash, "+", data["cash"][query_unit]);
		
		foreach(bank in keys(data["bank"]))
		{
			if( !file_exists(bank) ) continue;
			
			if( !objectp(bankroom = load_object(bank)) ) continue;
				
			totalcash = count(totalcash, "+", query("savings/"+id+"/"+query_unit, bankroom));
		}
	}

	return totalcash;
}

// �^�ǩҦ��{�����
varargs string query_all_cash(string id, string query_unit)
{
	mixed data;
	mapping money, banks;
	object bankroom;
	string all_cash;

	if( undefinedp(moneydata[id]) )
	{
		moneydata[id] = allocate_mapping(0);
		moneydata[id]["cash"] = allocate_mapping(0);
		moneydata[id]["bank"] = allocate_mapping(0);
		return 0;
	}

	if( mapp(money = moneydata[id]["cash"]) )
	{
		if( !undefinedp(query_unit) )
			all_cash = money[query_unit];
		else
		foreach( string unit, string m in money )
			all_cash = count(all_cash, "+", EXCHANGE_D->convert(m, unit, DEFAULT_MONEY_UNIT));
	}
	
	if( mapp(banks = moneydata[id]["bank"]) )
	{
		foreach( string bank in keys(banks) )
		{	
			if( !file_exists(bank) )
			{
				map_delete(moneydata[id]["bank"], bank);
				continue;
			}
	
			if( !(bankroom = load_object(bank)) )
				continue;
			
			data = query("savings/"+id, bankroom);
			
			if( mapp(data) && mapp(data = data["money"]) )
			{
				if( !undefinedp(query_unit) )
					all_cash = count(all_cash, "+", data[query_unit]);
				else
				foreach( string unit, string m in data )
					all_cash = count(all_cash, "+", EXCHANGE_D->convert(m, unit, DEFAULT_MONEY_UNIT));
			}
		}
	}
	return all_cash;
}

// �H RW ���зǳ��p��]�I
string query_assets(string id)
{
	return count(query_all_cash(id), "+", ESTATE_D->query_all_estate_value(id, 1));
}

// �p�����`�Ͳ����B(�ন $RW)
string calculate_GNP(string city)
{
	string GNP;
	string EST;
	string unit;
	string *citizens = CITY_D->query_citizens(city);
	
	if( !sizeof(citizens) ) return 0;

	unit = C2M[city];

	// �����Ҧ��]��
	foreach( string citizen in citizens )
	{
		if( SECURE_D->is_wizard(citizen) ) continue;
	
		GNP = count(GNP, "+", query_all_cash(citizen));
		GNP = count(GNP, "+", ESTATE_D->query_all_estate_value(citizen, 1));
		EST = count(EST, "+", ESTATE_D->query_all_estate_value(citizen));
	}
	
	// �[�W���F���겣
	GNP = count(GNP, "+", EXCHANGE_D->convert(CITY_D->query_city_info(city, "assets"), C2M[city], DEFAULT_MONEY_UNIT));
	
	CITY_D->set_city_info(city, "GNP", GNP);
	CITY_D->set_city_info(city, "EST", EST);
	CITY_D->set_city_info(city, "world_cash", query_world_cash(unit));
	return GNP;
}

string calculate_NNP(string city)
{
	string old_GNP = CITY_D->query_city_info(city, "GNP");
	string NNP = count(calculate_GNP(city), "-", old_GNP);

	CITY_D->set_city_info(city, "NNP", NNP);
	return NNP;
}

void broadcast_GNP()
{
	float rate;
	string GNP, NNP;
	string *cities = CITY_D->query_cities(), city_idname;
	
	if( !sizeof(cities) )
		return;
	
	cities -= ({ "wizard" });

	foreach( string city in sort_array(cities, (: count(CITY_D->query_city_info($1, "GNP"), "<", CITY_D->query_city_info($2, "GNP")) ? 1 : -1 :) ))
	{
		if( CITY_D->query_city_info(city, "fallen") ) continue;
		
		GNP = CITY_D->query_city_info(city, "GNP");
		NNP = CITY_D->query_city_info(city, "NNP");
		rate = to_float(count(count(NNP, "*", 10000), "/", GNP))/100.;

		city_idname = CITY_D->query_city_idname(city);
		CHANNEL_D->channel_broadcast("news", sprintf("%-25s "HIW"�����`�겣��"HIY" $RW %20s "HIW"�A����g�٦����v��"+(rate>0?HIG:HIR)+"%6.2f%%"HIW"�C"NOR, city_idname, NUMBER_D->number_symbol(GNP), rate));
	}
}

varargs int spend_money(string id, string unit, mixed money, int payment)
{
	if( !id || !unit || !money ) return 0;
	
	unit = upper_case(unit);
	money = big_number_check(money);
	
	if( !moneydata[id] || count(money, "<=", 0) ) return 0;

	// �{�������h��I�{��
	if( count( moneydata[id]["cash"][unit], ">=", money ) )
	{
		moneydata[id]["cash"][unit] = count( moneydata[id]["cash"][unit], "-", money );
		
		// ����n����F
		if( count( moneydata[id]["cash"][unit], "==", 0 ) )
			map_delete(moneydata[id]["cash"], unit);
			
		return 1;
	}
	else
	{
		int bankflag;
		string bank, bankmoney, allmoney;
		object bankob;

		// ���i��Ȧ���b���I�ڰʧ@
		if( !payment ) return 0;

		allmoney = moneydata[id]["cash"][unit];
		
		// ���˩Ҧ��Ȧ�s��, �ˬd�O�_����b
		foreach( bank, bankflag in moneydata[id]["bank"] )
		{
			// �Y�O�j���I�O || (��b�I�O && ��b�Ȧ�)
			if( payment & FORCED_PAYMENT || (payment & BANK_PAYMENT && bankflag & AUTOTRANSFER) )
			{
				if( !file_exists(bank) )
				{
					map_delete(moneydata[id]["bank"], bank);
					continue;
				}

				bankob = load_object(bank);
				
				if( !objectp(bankob) ) continue;
				
				bankmoney = query("savings/"+id+"/money/"+unit, bankob);
				
				allmoney = count( allmoney, "+", bankmoney );
			}
		}
		
		// �����{�� + �s�ڪ���������
		if( count( allmoney, "<", money ) ) return 0;
		
		// �������{��
		money = count( money, "-", moneydata[id]["cash"][unit] );
		map_delete(moneydata[id]["cash"], unit);
		
		// ���˩Ҧ��Ȧ�s��, �ˬd�O�_����b
		foreach( bank, bankflag in moneydata[id]["bank"] )
		{
			if( payment & FORCED_PAYMENT || (payment & BANK_PAYMENT && bankflag & AUTOTRANSFER) )
			{
				if( !file_exists(bank) )
				{
					map_delete(moneydata[id]["bank"], bank);
					continue;
				}
				
				bankob = load_object(bank);
				
				if( !objectp(bankob) ) continue;
				
				bankmoney = query("savings/"+id+"/money/"+unit, bankob);
				
				if( count( bankmoney, ">=", money ) )
				{
					bankmoney = count( bankmoney, "-", money );
					
					// ��n�᧹
					if( count( bankmoney, "==", 0 ) )
						delete("savings/"+id+"/money/"+unit, bankob);
					else
						set("savings/"+id+"/money/"+unit, bankmoney, bankob);
						
					return 1;
				}
				else
				{
					money = count( money, "-", bankmoney );
					delete("savings/"+id+"/money/"+unit, bankob);
				}
			}
		}
	}

	return 1;
}

// �ˬd�� bank �O�_���Ұ� autotransfer
int is_bank_autotransfer(string id, string bank)
{
	if( undefinedp(moneydata[id]) ) return 0;
		
	if( !mapp(moneydata[id]["bank"]) ) return 0;
		
	return moneydata[id]["bank"][bank] & AUTOTRANSFER;
}

varargs string earn_money(string id, string unit, mixed money, int payment)
{
	if( !id || !unit || !money ) return 0;
	
	unit = upper_case(unit);
	money = big_number_check(money);
	
	if( count(money, "<=", 0) ) return 0;
	
	if( !user_exists(id) ) return 0;

	if( undefinedp(moneydata[id]) )
	{
		moneydata[id] = allocate_mapping(0);
		moneydata[id]["cash"] = allocate_mapping(0);
		moneydata[id]["bank"] = allocate_mapping(0);
	}
	
	moneydata[id]["cash"][unit] = count( moneydata[id]["cash"][unit], "+", money );
	
	return moneydata[id]["cash"][unit];
}

varargs int set_bank(string id, string option, string bank, int flag)
{
	if( !id || !option || !bank || !moneydata[id] ) return 0;

	if( option == "set" )
	{
		if( undefinedp(moneydata[id]) )
		{
			moneydata[id] = allocate_mapping(0);
			moneydata[id]["cash"] = allocate_mapping(0);
			moneydata[id]["bank"] = allocate_mapping(0);
		}
		
		moneydata[id]["bank"][bank] = flag;
	}
	else if( option == "delete" )
	{
		if( !undefinedp(moneydata[id]) )
			map_delete(moneydata[id]["bank"], bank);
	}
}

string query_lazy_assets(string id)
{
	if( undefinedp(moneydata[id]) )
		return 0;
	
	return moneydata[id]["assets"];
}

string set_assets(string id, mixed money)
{
	if( !stringp(id) ) return 0;

	if( undefinedp(moneydata[id]) )
		moneydata[id] = allocate_mapping(0);
	
	return moneydata[id]["assets"] = money;
}

varargs void clear_money(string id, string unit)
{
	if( !mapp(moneydata[id]) ) return;

	if( !undefinedp(unit) && stringp(unit) )
		map_delete(moneydata[id], unit);
	else
		map_delete(moneydata, id);
}

varargs mapping query_moneydata(string id)
{
	if( undefinedp(id) )
		return moneydata;
	
	return moneydata[id];
}

void heart_beat()
{
	save();
}

private void create()
{
	if( clonep() ) 
		destruct(this_object());
		
	C2M = allocate_mapping(0);
	M2C= allocate_mapping(0);
	moneydata = allocate_mapping(0);

	if( file_size(DATA) < 1 )
		save();
	
	restore_object(DATA);

	if( !mapp(moneydata) )
		moneydata = allocate_mapping(0);
	
	foreach( string city, string money_unit in C2M )
	{
		if( !CITY_D->city_exist(city) )
			map_delete(C2M, city);
		else
			M2C[money_unit] = city;
	}
	save();
	
	C2M[DEFAULT_MONEY_UNIT] = DEFAULT_MONEY_UNIT;
	M2C[DEFAULT_MONEY_UNIT] = DEFAULT_MONEY_UNIT;

	set_heart_beat(300);
}

string query_default_money_unit()
{
	return DEFAULT_MONEY_UNIT;
}

int remove()
{
	return save();
}

string query_name()
{
	return "�����t��(MONEY_D)";
}
