/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : labor_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-6-19
 * Note   : �H�O���F
 * Update :
 *  o 2000-05-22  
 *
 -----------------------------------------
 */

#include <object.h>
#include <daemon.h>

#define DATA_PATH		"/data/daemon/labor.o"

private mapping labors;


#define LABOR_ID	"ID"
#define LABOR_IDNAME	"IDNAME"
#define LABOR_AGE	"AGE"
#define LABOR_SKILLS	"SKILLS"
#define LABOR_ADDTIME	"ADDTIME"

void remove_labor(string file);

int save_data()
{
	// �M���W�L 30 �ѨS�H���Ϊ����u
	foreach(string basename, mapping data in labors)
	{
		if( time() - data[LABOR_ADDTIME] > 30*24*60*60 )
		{
			destruct(find_object(basename));
			rm(basename+".o");
			remove_labor(basename);	
		}
	}

	return save_object(DATA_PATH);
}

int restore_data()
{
	return restore_object(DATA_PATH);
}

varargs void add_labor(object npc, int nodestruct_boss)
{
	string bossid;
	object bossob;

	if( !npc->is_module_npc() ) return;

	labors[base_name(npc)] = ([
		LABOR_ID : npc->query_id(1),
		LABOR_IDNAME : npc->query_idname(),
		LABOR_AGE : query("age", npc),
		LABOR_SKILLS : query("skills", npc) ,
		LABOR_ADDTIME : time(),
	]);
	
	bossid = query("boss", npc);
	
	if( bossid )
		bossob = load_user(bossid);
		
	if( objectp(bossob) )
	{
		string *hirelabors = query("hirelabors", bossob);
		
		if( arrayp(hirelabors) )
		{
			set("hirelabors", hirelabors - ({ base_name(npc) }), bossob);
			bossob->save();
		}
		
		if( environment(bossob) )
			bossob->earn_money(environment(bossob)->query_money_unit(), count(query("salary_paid", npc), "*", 15000));
		else
			bossob->earn_money(MONEY_D->query_default_money_unit(), count(query("salary_paid", npc), "*", 15000));
		if( !nodestruct_boss && !userp(bossob) )
			destruct(bossob);
	}
	
	delete("boss", npc);
	delete("relationship", npc);
	delete("job", npc);
	delete("salary_paid", npc);
	
	save_data();
}

void remove_labor(string file)
{
	map_delete(labors, file);

	save_data();
}

mapping query_labors()
{
	return copy(labors);
}

void del_labors(string id)
{
	string *hirelabors;
	object labor, boss = load_user(id);	
	
	if( !objectp(boss) ) return;
	
	hirelabors = query("hirelabors", boss);
	
	if( !sizeof(hirelabors) ) return;
	
	foreach( string laborfile in hirelabors )
	{
		if( !objectp(labor = load_object(laborfile)) )
			continue;

		delete("job", labor);
		delete("boss", labor);
		add_labor(labor);
	}
	
	delete("hirelabors", boss);
	boss->save();
	
	if( !userp(boss) )
		destruct(boss);
}
void create()
{
	if( !restore_data() )
	{
		labors = allocate_mapping(0);
		save_data();
	}
}

int remove()
{
	return save_data();
}

string query_name()
{
	return "���u�t��(LABOR_D)";
}
