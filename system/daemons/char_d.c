/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : char_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2000-12-29
 * Note   : ������F
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <gender.h>

void create_char(object user, int gender)
{
	if( previous_object() != find_object(PPL_LOGIN_D) ) return;

	// ��l�~��
	set("age", 15, user);

	// �ʧO
	set("gender", gender, user);

	// ���j�ݩ�
	set("attr/str", 10, user);
	set("attr/phy", 10, user);
	set("attr/int", 10, user);
	set("attr/agi", 10, user);
	set("attr/cha", 10, user);

	// �U����O��
	set("abi/stamina/max", 500, user);
	set("abi/stamina/cur", 500, user);
	set("abi/health/max", 250, user);
	set("abi/health/cur", 250, user);
	set("abi/energy/max", 250, user);
	set("abi/energy/cur", 250, user);

	// �U���g���
	set("exp/social/cur", 0, user);
	set("exp/social/tot", 0, user);

	// ���A��
	set("stat/water/max", 100, user);
	set("stat/water/cur", 100, user);
	set("stat/food/max", 100, user);
	set("stat/food/cur", 100, user);

	set("createtime", time(), user);

	/* �]�w�з��W�D */
	set("channels", CHANNEL_D->query_default_channel(user->query_id(1)), user);

}

void create_npc(object npc)
{
	if( !npc->is_npc() ) return;

	// ��l�~��
	if( !query("age", npc) ) set("age", 15, npc);

	// �U���ݩ�
	if( !query("attr", npc) )
	{
		set("attr/str", 10, npc);
		set("attr/phy", 10, npc);
		set("attr/int", 10, npc);
		set("attr/agi", 10, npc);
		set("attr/cha", 10, npc);
	}

	// �U����O��
	if( !query("abi", npc) )
	{
		set("abi/stamina/max", 500, npc);
		set("abi/stamina/cur", 500, npc);
		set("abi/health/max", 100, npc);
		set("abi/health/cur", 100, npc);
		set("abi/energy/max", 100, npc);
		set("abi/energy/cur", 100, npc);
	}
	// �U���g���
	set("exp/social/cur", 0, npc);
	set("exp/social/tot", 0, npc);

	// ���A��
	set("stat/water/max", 100, npc);
	set("stat/water/cur", 50, npc);
	set("stat/food/max", 100, npc);
	set("stat/food/cur", 50, npc);
}

int destruct_char(string id)
{
	string path = user_path(id);
	string backup_path;

	if( file_size(path) != -2 ) return 0;

	// �R���Ҥu��T
	LABOR_D->del_labors(id);

	// �����K�X��T
	PASSWORD_D->del_password(id);

	// �����Цa����T
	ESTATE_D->remove_estate(id);

	// ����������T
	MONEY_D->clear_money(id);

	// ����������T
	CITY_D->remove_citizen(id);

	// �R���Ʀ�]����
	TOP_D->delete_top_id(id);

	// ���P���~����
	ENTERPRISE_D->unregister_member(id);

	backup_path = user_deleted_path(id);
	intact_path(backup_path);

	rmtree(user_backup_path(id));
	rmtree(user_deleted_path(id));

	return !rename(path, backup_path);
}

void create()
{

}

string query_name()
{
	return "����t��(CHAR_D)";
}
