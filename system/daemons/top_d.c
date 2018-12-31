/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : top_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-06-28
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>

#define DATA_PATH	"/data/daemon/top.o"


#define TOP_MAX		1100
#define TOP_BUF		100

#define TOP_ID		0
#define TOP_NUMBER	1
#define TOP_TIME	2

mapping top = allocate_mapping(0);

/* TOP ��Ʈ榡
top = ([
	�Ʀ�]����1 : ({ // �̼Ʀr�ƦC
			({ �N��1, �Ʀr, �ɶ�, �Ѽ� ... }), // TOP 1
			({ �N��2, �Ʀr, �ɶ�, �Ѽ� ... }), // TOP 2
			({ �N��3, �Ʀr, �ɶ�, �Ѽ� ... }), // TOP 3
			// ...
		}),
	�Ʀ�]����2 : ({ // �̼Ʀr�ƦC
			({ �N��1, �Ʀr, �ɶ�, �Ѽ� ... }), // TOP 1
			({ �N��2, �Ʀr, �ɶ�, �Ѽ� ... }), // TOP 2
			({ �N��3, �Ʀr, �ɶ�, �Ѽ� ... }), // TOP 3
			// ...
		}),
]);
*/

void heart_beat()
{
	save_object(DATA_PATH);	
}

void create()
{
	if( !restore_object(DATA_PATH) )
		save_object(DATA_PATH);
	
	// �Q�����x�s�@��
	set_heart_beat(6000);
}

int remove()
{
	return save_object(DATA_PATH);
}

varargs mixed query_top(string key, int rank)
{
	if( key )
	{
		if( rank )
		{
			if( rank < 0 || rank > sizeof(top[key]) ) return;
			return top[key][rank-1];
		}
		if( undefinedp(top[key]) ) return allocate(0);
		return top[key][0..TOP_MAX-TOP_BUF-1];
	}
	return top;
	
}

// �ˬd TOP �O�_�s�b
int top_exists(string key, string id)
{
	if( undefinedp(top[key]) ) return 0;
		
	foreach(array data in top[key])
		if( data[TOP_ID] == id )
			return 1;
	
	return 0;
}

// �R�� TOP ���
varargs void delete_top(string key, string id)
{
	int pos, top_size;

	if( undefinedp(id) )
	{
		map_delete(top, key);
		return;
	}
	if( undefinedp(top[key]) ) return;

	top_size = sizeof(top[key]);
	for(pos=0; pos<top_size; pos++)
	{
		if( top[key][pos][TOP_ID] == id )
			top[key][pos] = 0;
	}
	top[key] -= ({ 0 });

	return;
}

void delete_top_id(string id)
{
	foreach(string key, array data in top)
		delete_top(key, id);
}

// �i��Ƨǰʧ@
varargs void sort_top(mixed key)
{
	if( undefinedp(key) )
	{
		foreach(key, array data in top)
		{
			top[key] = sort_array(top[key], (: count($1[TOP_NUMBER], "<", $2[TOP_NUMBER]) || (count($1[TOP_NUMBER], "==", $2[TOP_NUMBER]) && $1[TOP_TIME] > $2[TOP_TIME]) ? 1 : -1 :));
			top[key] = top[key][0..TOP_MAX-1];
		}
		return;
	}
	else if( undefinedp(top[key]) ) 
		return;

	top[key] = sort_array(top[key], (: count($1[TOP_NUMBER], "<", $2[TOP_NUMBER]) || (count($1[TOP_NUMBER], "==", $2[TOP_NUMBER]) && $1[TOP_TIME] > $2[TOP_TIME]) ? 1 : -1 :));
	top[key] = top[key][0..TOP_MAX-1];
}

// ��s TOP ���
varargs void update_top(string key, string id, mixed val, mixed arg ...)
{
	if( !key || !id || !(val = big_number_check(val)) ) return;
	if( undefinedp(top[key]) ) top[key] = allocate(0);

	delete_top(key, id);

	top[key] += ({ ({ id, val, time() }) + (arg ? arg: allocate(0)) });
	sort_top(key);

	// ���Ǹ�ƻݭn�ߧY�x�s, �קK�N�~���p�ɭP��ƿ�
	switch(key)
	{
		case "building":
			save_object(DATA_PATH);
			break;
		default:
			break;
	}
	
	return;
}

/* Feature */
void update_top_rich(object ob)
{
	string id, idname, city, assets, all_cash, all_estate, all_stock;
	mapping stockdata;

	if( !ob || !ob->is_user_ob() ) return;
	
	id = ob->query_id(1);
	
	if( SECURE_D->is_wizard(id) ) return;

	idname = ob->query_idname();
	city = query("city", ob);
 	stockdata = query("stock", ob);

	if( mapp(stockdata) && sizeof(stockdata) )
	foreach(string enterprise, mapping data in stockdata)
		all_stock = count(all_stock, "+", count(count(data["amount"], "*", to_int(ENTERPRISE_D->query_enterprise_info(enterprise, "stockvalue")*1000000)), "/", 100));

 	all_cash = MONEY_D->query_all_cash(id);
 	all_estate = ESTATE_D->query_all_estate_value(id, 1);
 
 	assets = count(count(all_cash, "+", all_estate), "+", all_stock);

  	update_top("cash", id, all_cash, idname, city);
 	update_top("estate", id, all_estate, idname, city);
 	update_top("rich", id, assets, idname, city);
 	update_top("stockvalue", id, all_stock, idname, city);
 
 	MONEY_D->set_assets(id, assets);
 	
 	set("totalassets", assets, ob);
}

/* Feature */
void update_top_social_exp(object ob)
{
	string id;

	if( !ob || !ob->is_user_ob() ) return;
	
	id = ob->query_id(1);
	
	if( SECURE_D->is_wizard(id) ) return;
  	
 	update_top("social_exp", id, query("exp/social/tot", ob), ob->query_idname(), query("city", ob));
}

void update_top_skill(object ob)
{
	string id, idname, city;
	int fullskills;
	mapping skills;

	if( !ob || !ob->is_user_ob() ) return;
	
	if( !mapp(skills = query("skills", ob)) ) return;

	id = ob->query_id(1);
	
	if( SECURE_D->is_wizard(id) ) return;

	idname = ob->query_idname();
	city = query("city", ob);	
	
	foreach(string skillname, mapping data in skills)
	{
		/* ��ɭ��ŮɳB�z
		if( !arrayp(top["skill/"+skillname]) )
			top["skill/"+skillname] = allocate(0);

		update_top("skill/"+skillname, id, data["level"], idname, city);
		*/

		if( data["level"] == 100 )
			fullskills++;
	}

	update_top("skill", id, fullskills, idname, city);
}

void calculate_top(object me)
{
	update_top_skill(me);
	update_top_rich(me);
	update_top_social_exp(me);
}

void switch_top(string key_from, string key_to)
{
	top[key_to] = copy(top[key_from]);	
	map_delete(top, key_from);
}

string query_name()
{
	return "�Ʀ�]�t��(TOP_D)";
}

string *query_top_key()
{
	return keys(top);
}

	