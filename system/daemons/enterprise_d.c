/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : enterprise_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-05
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <mudlib.h>

#define DATA_PATH	"/data/daemon/enterprise.o"
#define MRTGDATA	"/data/daemon/mrtg/stock/"

#define STOCK_MAX_VALUE		1000.0
#define STOCK_MIN_VALUE		1.0
#define STOCK_DEFAULT_VALUE	50.0
#define STOCK_DEFAULT_AMOUNT	5000

mapping enterprises;
nosave private mapping special_stockvalue;

int enterprise_exists(string id)
{
	return !undefinedp(enterprises[id]);
}
	
string query_enterprise_color_id(string id)
{	
	if( undefinedp(enterprises[id]) ) return 0;
	
	return enterprises[id]["color_id"];
}


// �^�Ǧb���ӥ��~���
string in_enterprise(string id)
{
	string enterprise_id, member_id;
	mapping enterprise_data, member_data;

	foreach(enterprise_id, enterprise_data in enterprises)
		foreach(member_id, member_data in enterprise_data["member"])
			if( id == member_id )
				return enterprise_id;
	
	return 0;
}

// ���U���~����
void register_member(string enterprise_id, string member_id)
{
	mapping member_data = allocate_mapping(0);
	
	member_data["jointime"] = time();
	
	enterprises[enterprise_id]["member"][member_id] = member_data;
	
	save_object(DATA_PATH);
}

// ���P���~����
void unregister_member(string unregister_id)
{
	string enterprise_id, member_id;
	mapping enterprise_data, member_data;

	foreach(enterprise_id, enterprise_data in enterprises)
	{
		foreach(member_id, member_data in enterprise_data["member"])
		{
			if( unregister_id == member_id )
				map_delete(enterprises[enterprise_id]["member"], unregister_id);
		}
	}
	
	save_object(DATA_PATH);
}

// �]�w���~��T
mixed set_enterprise_info(string id, string prop, mixed info)
{
	if( undefinedp(enterprises[id]) ) return 0;

	return enterprises[id][prop] = info;
}

// �d�ߥ��~��T
varargs mixed query_enterprise_info(string id, string prop)
{
	if( undefinedp(enterprises[id]) ) return 0;

	if( undefinedp(prop) )
		return enterprises[id];

	return enterprises[id][prop];
}

// �R�����~��T
void delete_enterprise_info(string id, string prop)
{
	if( undefinedp(enterprises[id]) ) return 0;

	map_delete(enterprises[id], prop);
}

// ���U���~
void register_enterprise(string id, string color_id, string president)
{	
	enterprises[id] = allocate_mapping(0);
	
	enterprises[id]["president"] = president;
	enterprises[id]["color_id"] = color_id;
	enterprises[id]["member"] = allocate_mapping(0);
	enterprises[id]["registertime"] = time();
	enterprises[id]["stockvalue"] = STOCK_DEFAULT_VALUE;
	enterprises[id]["stockpredictvalue"] = STOCK_DEFAULT_VALUE;
	enterprises[id]["stockamount"] = STOCK_DEFAULT_AMOUNT;
	enterprises[id]["available_stockamount"] = STOCK_DEFAULT_AMOUNT;

	register_member(id, president);	
	
	save_object(DATA_PATH);
}

// ���P���~
void unregister_enterprise(string enterprise_id)
{
	object member_ob;
	string member_id;
	mapping member_data;

	if( undefinedp(enterprises[enterprise_id]) ) return;

	ESTATE_D->remove_estate("ENTERPRISE/"+enterprise_id);

	//MRTG_D->removemrtg("stock_"+enterprises[enterprise_id]["enterprise_id_number"]);
	
	foreach(member_id, member_data in enterprises[enterprise_id]["member"])
	{
		member_ob = load_user(member_id);
		
		if( objectp(member_ob) )
		{
			delete("enterprise", member_ob);
			
			member_ob->save();
			
			if( !userp(member_ob) )
				destruct(member_ob);
		}	
	}
	
	map_delete(enterprises, enterprise_id);
	
	save_object(DATA_PATH);
}

// �ˬd�O�_���`��
int is_president(string enterprise_id, string id)
{
	if( undefinedp(enterprises[enterprise_id]) ) return 0;
	
	return enterprises[enterprise_id]["president"] == id;
}

string query_president(string enterprise_id)
{
	if( undefinedp(enterprises[enterprise_id]) ) return 0;
	
	return enterprises[enterprise_id]["president"];
}

// �ˬd�O�_������
int is_member(string enterprise_id, string id)
{
	if( undefinedp(enterprises[enterprise_id]) ) return 0;
	
	return !undefinedp(enterprises[enterprise_id]["member"][id]);
}

// �B�z���
string change_invest(string enterprise_id, string id, mixed money)
{
	string result;
	
	if( undefinedp(enterprises[enterprise_id]) || undefinedp(enterprises[enterprise_id]["member"][id]) ) return 0;
	
	result = count(enterprises[enterprise_id]["member"][id]["invest"], "+", money);
	
	enterprises[enterprise_id]["member"][id]["invest"] = result;

	return result;
}

// �B�z�겣
string change_assets(string enterprise_id, mixed money)
{
	string result;
	
	if( undefinedp(enterprises[enterprise_id]) ) return 0;
	
	result = count(enterprises[enterprise_id]["assets"], "+", money);
	
	if( count(result, "<", 0) ) return 0;

	enterprises[enterprise_id]["assets"] = result;

	//if( SECURE_D->is_wizard(enterprises[enterprise_id]["president"]) )
	//	enterprises[enterprise_id]["assets"] = "0";

	return result;
}

//
// �ˬd��H�O�_�b�P�@�ӥ��~��
//
int same_enterprise(string member_id1, string member_id2)
{
	if( !member_id1 || !member_id2 )
		return 0;

	foreach(string enterprise_id, mapping enterprise_data in enterprises)
	{
		if( !undefinedp(enterprise_data["member"][member_id1]) &&
			!undefinedp(enterprise_data["member"][member_id2]) )
			return 1;
	}
	
	return 0;
}

string query_money(string enterprise_id)
{
	if( undefinedp(enterprises[enterprise_id]) ) return 0;
	
	return enterprises[enterprise_id]["assets"];
}

mapping query_enterprises()
{
	return copy(enterprises);
}

string query_enterprise_assets(string enterprise_id)
{
	string totalassets;
	
	if( undefinedp(enterprises[enterprise_id]) ) return 0;
	
	// �C�p�ɧ@�@�� cache
	if( enterprises[enterprise_id]["assets_cache_time"] + 3600 < time() )
	{
		enterprises[enterprise_id]["assets_cache"] = ESTATE_D->query_all_estate_value("ENTERPRISE/"+enterprise_id);
		enterprises[enterprise_id]["assets_cache_time"] = time();
	}

	totalassets = count(enterprises[enterprise_id]["assets"], "+", enterprises[enterprise_id]["assets_cache"]);

	//foreach(string id, mapping data in enterprises[enterprise_id]["member"])
		//totalassets = count(totalassets, "+", MONEY_D->query_lazy_assets(id));
	
	return totalassets;
}


//
// �^�ǩҦ��������`�겣
//
string query_enterprise_allmembers_assets(string enterprise_id)
{
	string totalassets;
	
	if( undefinedp(enterprises[enterprise_id]) ) return 0;
		
	foreach(string id, mapping data in enterprises[enterprise_id]["member"])
		totalassets = count(totalassets, "+", MONEY_D->query_lazy_assets(id));
		
	return totalassets;
}

string *query_all_enterprises()
{
	return sort_array(keys(enterprises), (: count(query_enterprise_allmembers_assets($1), "<", query_enterprise_allmembers_assets($2)) ? 1 : -1 :));
}

int save()
{
	return save_object(DATA_PATH);
}

int remove()
{
	return save();
}

void fix_enterprise_database()
{
//	foreach(string enterprise_id, mapping data1 in enterprises)
}

void add_enterprise_to_mrtg(string enterprise_id, int enterprise_id_number, float new_stockvalue)
{
/*
	mapping info = ([
		"Directory":"stock/"+enterprise_id_number,
		"Target":"`cat \""LIBRARY_PATH+MRTGDATA+enterprise_id_number+"\"`",
		"YLegend":"Value",
		"LegendI":"�ѻ�",
		"LegendO":"",
		"Legend1":"���~�ӪѨ���",
		"Legend2":"",
		"ShortLegend":"��",
		"MaxBytes":50,
		"MaxAbs":1500,
		"Options":"gauge, growright, nopercent, noinfo",
		"Title": enterprise_id+"���~�ӪѨ���",
		"PageTop":enterprise_id+"���~�ӪѨ���"
	]);

	MRTG_D->addmrtg("stock_"+enterprise_id_number, info);
	write_file(MRTGDATA+enterprise_id_number, sprintf("%.2f", new_stockvalue)+"\n"+0, 1);
*/
}


void set_special_stockvalue(string reason, int value)
{
	if( !mapp(special_stockvalue) )
		special_stockvalue = allocate_mapping(0);
		
	special_stockvalue[reason] = value;
}

void heart_beat()
{
	string enterprise_id;
	mapping enterprise_data;
	float new_stockvalue;
	float stockvalue_change;
	int randomvalue;
	int assets_scale;
	int global_randomvalue;

	global_randomvalue = range_random(-30, 30);
	
	if( sizeof(special_stockvalue) > 0 )
	{
		foreach(string reason, int svalue in special_stockvalue )
		{
			CHANNEL_D->channel_broadcast("stock", reason);
			global_randomvalue += svalue;
		}
		special_stockvalue = allocate_mapping(0);
	}

	if( global_randomvalue >= 25 )
		CHANNEL_D->channel_broadcast("stock", "�ѥ�����U��"HIR"�Q"NOR RED"�h"NOR"�v�T�A�U�Ѵ��M�e�{�k�ɨ��աC");
	else if( global_randomvalue <= -25 )
		CHANNEL_D->channel_broadcast("stock", "�ѥ�����U��"HIG"�Q"NOR GRN"��"NOR"�v�T�A�U�Ѵ��M��{�h�z�C");

	foreach(enterprise_id, enterprise_data in enterprises)
	{
		if( undefinedp(enterprise_data["enterprise_id_number"]) )
			enterprises[enterprise_id]["enterprise_id_number"] = SERIAL_NUMBER_D->query_serial_number("enterprise_id_number");

		// �i��w���ѻ����վ�
		if( !random(500) )
		{
			randomvalue = range_random(-1000, 1000);
			
			// �C�����̰ܳ� 10%
			stockvalue_change = enterprise_data["stockpredictvalue"] * randomvalue * 0.0001;

			// �����q��
			if( !random(2) )
			switch(randomvalue)
			{
				case 800..1000:
					CHANNEL_D->channel_broadcast("stock", "���R�v�w�����~����"+query_enterprise_color_id(enterprise_id)+"���ѻ������Ө��i��|���j�T�W���C");
					break;
				case 500..799:
					CHANNEL_D->channel_broadcast("stock", "���R�v�w�����~����"+query_enterprise_color_id(enterprise_id)+"���ѻ������Ө��i��|���p�T�W���C");
					break;
				case -799..-500:
					CHANNEL_D->channel_broadcast("stock", "���R�v�w�����~����"+query_enterprise_color_id(enterprise_id)+"���ѻ������Ө��i��|���p�T�U�^�C");
					break;	
				case -1000..-800:
					CHANNEL_D->channel_broadcast("stock", "���R�v�w�����~����"+query_enterprise_color_id(enterprise_id)+"���ѻ������Ө��i��|���p�T�U�^�C");
					break;	
			}

			new_stockvalue = enterprise_data["stockpredictvalue"] + stockvalue_change;
			
			assets_scale = sizeof(query_enterprise_assets(enterprise_id));

			// �ѻ��[�W���~�`�� scale
			//new_stockvalue += pow(to_float(assets_scale+"."+to_string(assets_scale)[0..0]), 1.3);
			
			if( new_stockvalue < STOCK_MIN_VALUE )
				new_stockvalue = STOCK_MIN_VALUE;
			else if( new_stockvalue > STOCK_MAX_VALUE )
				new_stockvalue = STOCK_MAX_VALUE;
			
			enterprises[enterprise_id]["stockpredictvalue"] = new_stockvalue;
		}


		// �ѻ��C��w���ѻ��� 90%
		if( enterprise_data["stockvalue"]  < enterprise_data["stockpredictvalue"] * 0.9 )
			randomvalue = range_random(-40, 80);
		// �ѻ�����w���ѻ��� 110%
		else if( enterprise_data["stockvalue"] > enterprise_data["stockpredictvalue"] * 1.1 )
			randomvalue = range_random(-80, 40);
		else
			randomvalue = range_random(-80, 80);
		
		randomvalue += global_randomvalue;

		// �C���ܰʳ̰� 1.5%
		stockvalue_change = enterprise_data["stockvalue"] * randomvalue * 0.0001;
		
		new_stockvalue = enterprise_data["stockvalue"] + stockvalue_change;
		

		if( new_stockvalue < STOCK_MIN_VALUE )
			new_stockvalue = STOCK_MIN_VALUE;
		else if( new_stockvalue > STOCK_MAX_VALUE )
			new_stockvalue = STOCK_MAX_VALUE;

		if( randomvalue >= 200 )
			CHANNEL_D->channel_broadcast("stock", sprintf("���~����%s�ѻ��j�� "HIR"��%.2f"NOR" �I(%.2f%%)�A���� "HIW"%.2f"NOR" �I�C",
				query_enterprise_color_id(enterprise_id),
				stockvalue_change,
				randomvalue/100.,
				new_stockvalue));
		else if( randomvalue <= -200 )
			CHANNEL_D->channel_broadcast("stock", sprintf("���~����%s�ѻ����� "HIG"��%.2f"NOR" �I(%.2f%%)�A���� "HIW"%.2f"NOR" �I�C",
				query_enterprise_color_id(enterprise_id),
				-stockvalue_change,
				-randomvalue/100.,
				new_stockvalue));

/*
		if( stockvalue_change > 0. )
			CHANNEL_D->channel_broadcast("stock", sprintf("%-20s �ѻ� "HIR"��%5.2f"NOR"�A���� "HIW"%7.2f"NOR" �I�C", query_enterprise_color_id(enterprise_id), stockvalue_change, new_stockvalue));
		else if( stockvalue_change < 0. )
			CHANNEL_D->channel_broadcast("stock", sprintf("%-20s �ѻ� "HIG"��%5.2f"NOR"�A���� "HIW"%7.2f"NOR" �I�C", query_enterprise_color_id(enterprise_id), -stockvalue_change, new_stockvalue));
		else
			CHANNEL_D->channel_broadcast("stock", sprintf("%-20s �ѻ� "HIW"  %5.2f"NOR"�A���� "HIW"%7.2f"NOR" �I�C", query_enterprise_color_id(enterprise_id), stockvalue_change, new_stockvalue));
*/
		enterprises[enterprise_id]["stockvalue_change"] = stockvalue_change;	
		enterprises[enterprise_id]["stockvalue"] = new_stockvalue;
		
		if( undefinedp(enterprises[enterprise_id]["stockvalue_minimum"]) )
			enterprises[enterprise_id]["stockvalue_minimum"] = new_stockvalue;
		else if( new_stockvalue < enterprises[enterprise_id]["stockvalue_minimum"]+10. )
			enterprises[enterprise_id]["stockvalue_minimum"] = new_stockvalue+10.;

		add_enterprise_to_mrtg(enterprise_id, enterprises[enterprise_id]["enterprise_id_number"], new_stockvalue);
	}
}

void set_enterprise_stockvalue(string enterprise_id, float value)
{
	enterprises[enterprise_id]["stockvalue"] = value;
}

void create()
{
	if( !restore_object(DATA_PATH) )
	{
		enterprises = allocate_mapping(0);
		save();
	}

	set_heart_beat(6000);
}


string query_name()
{
	return "���~�t��(ENTERPRISE_D)";
}
