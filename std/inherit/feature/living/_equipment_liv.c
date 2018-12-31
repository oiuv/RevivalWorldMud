/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _equipment_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-21
 * Note   :
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <equipment.h>

private nosave mapping equipments = allocate_mapping(0);

//
// �^�ǩҦ��˳Ƹ�T
//
mapping query_equipments()
{
	return equipments;
}



//
// �^�ǩҦ��˳ƪ���
//
object *query_equipment_objects()
{
	return implode(values(equipments), (: $1+$2 :)) || allocate(0);
}



//
// 1: �����󤣬O�˳�
// 2: �L�k�˳Ʀb������W
// 3: ����������Ӹ˳Ʀb��B
// 4: �w�g���P�������˳�
// 5: �w�g�˳Ʀb��L������W
//
int equip(object ob, int ref status)
{
	array partset;
	int partamount;
	string partid;
	

	// �����󤣬O�˳�
	if( !ob->is_equipment() )
	{
		status = 1;
		return 0;	
	}

	// �L�k�˳Ʀb������W
	if( !ob->valid_equip(this_object()) )
	{
		status = 2;
		return 0;
	}

	// ����������Ӹ˳Ʀb��B
	if( !arrayp(partset = ob->query_part_set()) )
	{
		status = 3;
		return 0;
	}

	partid = partset[PART_ID];
	partamount = partset[PART_AMOUNT];
	
	// �w�g�L�k�A�˳Ƨ�h�P�������˳�
	if( !undefinedp(equipments[partid]) && sizeof(equipments[partid]) >= partamount )
	{
		status = 4;
		return 0;
	}

	foreach(string p, object *eqs in equipments)
	{
		if( member_array(ob, eqs) != -1 )
		{
			status = 5;
			return 0;
		}
	}
	
	if( undefinedp(equipments[partid]) )
		equipments[partid] = allocate(0);
	
	equipments[partid] |= ({ ob });

	ob->set_equipping(this_object());

	return 1;
}


//
// �Ѱ��Y�Ӫ���˳�
//
// 1: �õL�˳Ʀ�����
// 2: �L�k�Ѱ������˳�
//
int unequip(object ob, int ref status)
{
	foreach(string partid, object *eqs in equipments)
	{
		if( member_array(ob, eqs) != -1 )
		{
			// �L�k�Ѱ������˳�
			if( !ob->valid_unequip(this_object()) )
			{
				status = 2;
				return 0;
			}
			
			ob->delete_equipping(this_object());
			
			equipments[partid] -= ({ ob });
			
			if( !sizeof(equipments[partid]) )
				map_delete(equipments, partid);

			return 1;
		}
	}

	// �õL�˳Ʀ�����
	status = 1;
	return 0;
}



//
// �Ѱ��Ҧ��˳�
//
object *unequip_all()
{
	object ob;
	object *all_equipments = allocate(0);
	
	foreach(string partid, object *eqs in equipments)
	{
		foreach(ob in eqs)
			ob->delete_equipping(this_object());
		
		all_equipments += eqs;
	}
	
	equipments = allocate_mapping(0);
	
	return all_equipments;
}



//
// �Ѱ��Y�ӳ��쪺�Ҧ��˳�
//
// 1: ������õL����˳�
// 2: �L�k�Ѱ������˳�
//
object *unequip_part(string partid)
{
	object *unequipped_equipments = allocate(0);

	if( undefinedp(equipments[partid]) )
		return 0;
	
	foreach( object ob in equipments[partid] )
	{
		// �L�k�Ѱ��o���˳�
		if( !ob->valid_unequip(this_object()) )
			continue;
	
		ob->delete_equipping(this_object());
		
		unequipped_equipments |= ({ ob });
	}
	
	map_delete(equipments, partid);
	
	return unequipped_equipments;
}



//
// �ˬd������O�_���b�˳�
//
int is_equipping_object(object ob)
{
	foreach(string part, object *eqs in equipments)
		if( member_array(ob, eqs) != -1 )
			return 1;
	
	return 0;
}


//
// �ˬd�Y����O�_���˳�
//
int is_equipping_part(string part)
{
	if( !undefinedp(equipments[part]) )
		return 1;

	return 0;
}



//
// �^�ǬY���쪺�˳ƪ���
//
object *query_equipping_object(string partid)
{
	if( !undefinedp(equipments[partid]) )
		return equipments[partid];

	return 0;
}



//
// �^�ǬY�˳ƪ�����
//
string query_equipping_part(object ob)
{
	foreach(string part, object *eqs in equipments)
		if( member_array(ob, eqs) != -1 )
			return part;
	return 0;
}


