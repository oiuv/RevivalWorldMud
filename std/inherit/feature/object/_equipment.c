/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _equipment.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-21
 * Note   :
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

private nosave object* equipping = allocate(0);

//
// �O�_���˳ƪ���
//
int is_equipment()
{
	return 1;
}

//
// �R���򥢪��˳ƪ̪���
//
void remove_null_equipping()
{
	foreach(object ob in equipping)
		if( !objectp(ob) )
			equipping -= ({ ob });
			
	if( !sizeof(equipping) )
		delete_temp("equipping");
}

//
// �^�ǩҦ����b�˳Ʀ����󪺨ϥΪ�
//
object *query_equipping()
{
	remove_null_equipping();

	return equipping;	
}


//
// �O�_���Q�˳Ƥ�
//
int is_equipping()
{
	return query_temp("equipping");
}


//
// �]�w ob �˳ƤW������
// set_temp("equipping") �� _loginload_usr �i�H�x�s�˳ƪ��A
//
void set_equipping(object ob)
{
	equipping |= ({ ob });
	
	// �]�w��w
	this_object()->set_keep();

	set_temp("equipping", 1);
	
	remove_null_equipping();
}


//
// �Ѱ� ob �˳Ʀ����󪺪��A
// �u���� _equipment_liv �I�s, �����ѥ~�ɩI�s
//
varargs void delete_equipping(object ob)
{
	remove_null_equipping();

	if( undefinedp(ob) )
		equipping = allocate(0);
	else
		equipping -= ({ ob });

	// �Ѱ���w
	if( !sizeof(equipping) )
	{
		delete_temp("equipping");
	}
}


//
// ���o����]�w
//
array query_part_set()
{
	return query("equip/part/set");
}


//
// ���o����W��
//
string query_part_name()
{
	return query("equip/part/name");
}


//
// �ˬd�O�_���\�Q user �˳�
//
int valid_equip(object user)
{
	return 1;
}


//
// �ˬd�O�_���\�Q user ���U�˳�
//
int valid_unequip(object user)
{
	return 1;
}


//
// ��l�˳Ƴ]�w
// �Q�� set() ������]�w�ȳQ�]�w�� shadow_object ���`�� memory �ϥ�
//
void setup_equipment(array set, string name)
{
	set("equip/part/set", set);
	set("equip/part/name", name);
}

