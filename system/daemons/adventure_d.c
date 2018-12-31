/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : adventure_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-22
 * Note   : ���I�t��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <message.h>

#define DATA		"/data/daemon/adventure.o"
#define NPC_TYPE	({ "leader", "prospector", "explorer", "guard", "scout" })
mapping adventure;


int save()
{
	save_object(DATA);
}

//
// �O�_���b�i�汴�I
//
int is_doing_adventure(mixed env)
{
	if( stringp(env) )
		return !undefinedp(adventure[env]);
	else if( objectp(env) )
		return !undefinedp(adventure[base_name(env)]);
	else
		return 0;
}

//
// �|�ݭn���ɶ�
//
int time_still(object env)
{
	return adventure[base_name(env)]["time"];
}

//
// �}�l���I����
//
void start_adventure(object env, mapping data)
{
	adventure[base_name(env)] = data;
}

//
// �߸��禡
//
void heart_beat()
{
	object env;
	object ob;
	string npctype;
	string filename, filename2;
	mapping data, data2;
	
	// ���X�Ҧ� NPC ����, �ˬd���
	foreach(filename, data in adventure)
	{
		foreach(npctype in NPC_TYPE)
		{
			if( data[npctype] && file_exists(data[npctype]) )
				adventure[filename][npctype+"_ob"] = load_object(data[npctype]);
			else
				map_delete(adventure[filename], npctype+"_ob");
		}
		
		// ���I��a�����F -> �������I����
		if( !file_exist(filename) )
		{
			foreach(npctype in NPC_TYPE)
			{
				if( !objectp(ob = adventure[filename][npctype+"_ob"]) ) continue;
				delete("adventure", ob);
				ob->save();
			}

			map_delete(adventure, filename);
		}
	}
	
	// �i�汴�I����
	foreach(filename, data in adventure)
	{
		adventure[filename]["time"]--;

		// ���I����
		if( adventure[filename]["time"] <= 0 )
		{
			env = load_object(filename);
			
			foreach(npctype in NPC_TYPE)
			{
				if( !objectp(ob = adventure[filename][npctype+"_ob"]) ) continue;

				delete("adventure", ob);
				ob->move(env);
				ob->save();
				msg("$ME�����F�b�u"+AREA_D->query_area_idname(data["area"], data["num"])+"�v�����I���ʡA�h�Φa�^�즹�a�C\n", ob, 0, 1);
			}
			
			map_delete(adventure, filename);
			continue;
		}

		foreach(filename2, data2 in adventure)
		{
			// ���P�ϰ줣�i��԰�
			if( data["area"] != data2["area"] || data["num"] != data2["num"] || filename == filename2 ) continue;

			// ���d�L��O�U���U���קK�԰�
			if( data["scout_ob"] && random(data["scout_ob"]->query_agi()) > 5 ) continue;
			
			// ������Ҧ��@��, �i��԰�
			if( data["guard_ob"] && data2["guard_ob"] )
			{
				if( random(data["guard_ob"]->query_phy()) > random(data2["guard_ob"]->query_phy()) )
					CHANNEL_D->channel_broadcast("nch", "�����ǨӦb�u"+AREA_D->query_area_idname(data["area"], data["num"])+"�v���A"+capitalize(data["boss"])+" �P "+capitalize(data2["boss"])+" ��䱴�I����o���Y���Ĭ�C\n");				
				
				map_delete(adventure, filename);
			}
			// �Y�u���ڤ観�@��
			else if( data["guard_ob"] )
			{
				if( random(data["guard_ob"]->query_phy()) > 5 )
					CHANNEL_D->channel_broadcast("nch", "�����ǨӦb�u"+AREA_D->query_area_idname(data["area"], data["num"])+"�v���A"+capitalize(data["boss"])+" �P "+capitalize(data2["boss"])+" ��䱴�I����o���Y���Ĭ�C\n");	
			}
			// �Y�u����観�@��
			else if( data2["guard_ob"] )
			{
				if( random(data2["guard_ob"]->query_phy()) > 5 )
					CHANNEL_D->channel_broadcast("nch", "�����ǨӦb�u"+AREA_D->query_area_idname(data["area"], data["num"])+"�v���A"+capitalize(data["boss"])+" �P "+capitalize(data2["boss"])+" ��䱴�I����o���Y���Ĭ�C\n");	
			}
			// �Y���賣�S���@��
			else
			{
				if( !random(3) )
					CHANNEL_D->channel_broadcast("nch", "�����ǨӦb�u"+AREA_D->query_area_idname(data["area"], data["num"])+"�v���A"+capitalize(data["boss"])+" �P "+capitalize(data2["boss"])+" ��䱴�I����o���Y���Ĭ�C\n");	
			}
		}
	}
	
	save();
}


int remove()
{
	return save();
}

void create()
{
	if( !restore_object(DATA) )
	{
		adventure = allocate_mapping(0);

		save();
	}
	
	set_heart_beat(600);
}

string query_name()
{
	return "���I�t��(ADVENTURE_D)";
}
