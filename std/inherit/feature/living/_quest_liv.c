/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _quest_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : �����~����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>

// �]�w quest ����
void set_quest_note(string quest, string name, string note)
{
	set("quest/"+quest+"/time", time());
	set("quest/"+quest+"/name", name);
	set("quest/"+quest+"/note", note);
	
	tell(this_object(), HIY"���ȰT����s�A�ЧQ�� quest ���O�d�ߡC\n"NOR);
}

// �]�w quest ���
void set_quest_data(string quest, string key, mixed value)
{
	set("quest/"+quest+"/data/"+key, value);
}

// ���o quest ���
mixed query_quest_data(string quest, string key)
{
	return query("quest/"+quest+"/data/"+key);
}

// �]�w quest �B�J
void set_quest_step(string quest, int step)
{
	set("quest/"+quest+"/step", step);
}

// ���o quest �B�J
int query_quest_step(string quest)
{
	return query("quest/"+quest+"/step");
}

// �]�w�� quest �w�g����
void set_quest_finish(string quest, string name, string note)
{
	int finish_time = addn("quest/"+quest+"/finish", 1);
	
	if( finish_time == 1 && !SECURE_D->is_wizard(this_object()->query_id(1)) )
	{
		int totalquests = sizeof(filter(query("quest"), (: $2["finish"] :)));
		TOP_D->update_top("quest", this_object()->query_id(1), totalquests, this_object()->query_idname(), query("city"));
	}
	
	delete("quest/"+quest+"/step");
	delete("quest/"+quest+"/data");
	set_quest_note(quest, name, note);
}

// �ˬd quest �O�_�w�g����
int query_quest_finish(string quest)
{
	return query("quest/"+quest+"/finish");
}

// ���o�̫�@����s�ɶ�
int query_quest_time(string quest)
{
	return query("quest/"+quest+"/time");
}
