/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : object.c
 * Author : Clode@RevivalWorld
 * Date	  : 2001-7-2
 * Note	  : included by	Simul_Efun Object
 * Update :
 *  o 2002-01-05 Cookys	�[�W����

 -----------------------------------------
 */

#include <daemon.h>
#include <location.h>


/* �Ǧ^�� ob �b�P�@���ҩΦP�@�y�Ф�������}�C, �]�A ob ���� */
/* �� simul_efun ��� mud �Ĳv���ܤj���v�T, �Y�����n, �ݧ�g���u���� efun */
object *present_objects(object ob)
{
	// env ���ثe����Ҧb���Ŷ�
	object env = environment(ob);

	// �S�����ҡI�H
	if( !env ) return allocate(0);

	// �u���b map_d	���a�Ϩt�Τ~�ݭn�P�_
	if( env->is_maproom() )
		return MAP_D->coordinate_inventory(query_temp("location", ob));

	// �p�G�b map_d	�t�ΥH�~�����Ǧ^ efun:all_inventory ����
	return all_inventory(env);
}


private	int sort_objects_sortfunc(object ob1, object ob2)
{
	if( ob1->is_board() ) return 1;
	if( ob2->is_board() ) return -1;

	if( userp(ob1) ) return	1;
	if( userp(ob2) ) return	-1;

	if( !ob1->is_npc() ) return 1;
	if( !ob2->is_npc() ) return -1;

	return -1;
}

/*
	�Ω�C�����e����Ƨ�
	����ƧǶ��Ǭ� NPC -> ITEM -> PLAYER ->	BOARD
*/
object *sort_objects(object *obarr)
{
	object *retarr = allocate(0);
	array uniarr;

	if( !sizeof(obarr) )	
		return retarr;

	// �i�����	
	uniarr = unique_array(obarr, (:	userp($1) ? 1 :	$1->is_npc() ? 2 : $1->is_board() ? 3 :	4 :));

	// ������Ƨ�
	uniarr = sort_array(uniarr, (: sort_objects_sortfunc($1[0], $2[0]) :));

	foreach( object* unique	in uniarr )
	retarr += unique;

	return retarr;
}

/* �ھ�	str ��M���� */
//note:	object str �R������ (�N�O:�ustr�v�]�i�H�O�@�Ӫ���A�h�ustr�v�O�M��A�ӫD�I�s�禡 id()�C�o��)
// str �O���M�䪺�r�� ,	�Y��ob�h�M�� ob	���t��
varargs	object present(	string str, object ob )
{
	object *inv;

	if( !str || !str[0] ) return 0;

	// ���S�� ob �ǤJ�A���h�ˬd�O�_���S��	map_d system
	if( objectp(ob)	)
	{
		// ���	map_d ���A�ίS�w�� present_object simul_efun.
		if( ob->is_maproom() )
			inv = sort_objects(present_objects(this_player()));
		else
			inv = sort_objects(all_inventory(ob));
	}
	else
		// �S��	ob �ǤJ�A�۰ʫ����W�@�Ӫ���
	{
		object obj = previous_object(-1)[<1];
		object env = environment(obj);
		inv = sort_objects(all_inventory(obj));

		// ���W�Ӫ���Ө��A���ⳡ������A�@�ӬO�Ҧb�Ϫ�����A�t�@�ӬO���W����
		if( objectp(env) )
		{
			// �O�_�b map_d	��
			if( env->is_maproom() )
				inv += sort_objects(present_objects(obj));
			else
				inv += sort_objects(all_inventory(env));
		}
	}

	return object_parse(str, inv);
}


/* �ˬd���� ob �O�_�����O���� */
varargs	int is_command(object ob)
{
	// �p�G�S���ǤJ	ob, �۰ʨ��W�@�өI�s����
	if( !objectp(ob) ) ob =	previous_object();
	// �ˬd�O�_�����O�ؿ��A�åB�S�Q clone	�L
	return file_name(ob)[0..5] == "/cmds/" && !clonep(ob);
}
/* �ˬd���� ob �O�_�����F���� */
varargs	int is_daemon(object ob)
{
	// �p�G�S���ǤJ	ob, �۰ʨ��W�@�өI�s����
	if( !objectp(ob) ) ob =	previous_object();
	// �ˬd�O�_�����F�ؿ��A�åB�S�Q clone	�L
	return file_name(ob)[0..15] == "/system/daemons/" && !clonep(ob);
}

/* �R���ǤJ�� ob , ob �i�� str or object type */
varargs	int destruct(mixed ob, mixed amount)
{
	object env, *obarr;

	if( stringp(ob)	) 
		ob = find_object(ob);

	if( objectp(ob)	)
		obarr =	({ ob });
	else if( arrayp(ob) )
		obarr =	ob;
	else
		return 0;

	foreach( ob in obarr )
	{
		if( !objectp(ob) ) continue;

		/* only for debug */
		/*		if( !clonep(ob) )
				{
					CHANNEL_D->channel_broadcast("nch", sprintf("�D���� %s �D %O �R��",  base_name(ob), previous_object()));
					log_file("root_object", sprintf("�D���� %s �D %O �R��",  base_name(ob), previous_object()));
				}
		*/
		if( ob->query_database() && objectp(env = environment(ob)) ) 
		{
			// �R���ƶq�֩��`��
			if( amount && count(amount, "<", query_temp("amount", ob)||1) && count(amount, ">", 0) )
			{
				set_temp("amount", count(query_temp("amount", ob),"-",amount), ob);

				continue;
			}
			// �����R��
			else
			{
				if( env->is_maproom() )
					MAP_D->leave_coordinate(query_temp("location", ob), ob);

				else if( env->is_living() )
					env->remove_action(ob);
			}
		}
		
		// �Y�����󥿳Q�˳Ƥ�, �j�������˳�
		if( ob->is_equipping() )
		{
			int status;
			object *equippers = ob->query_equipping();
			
			if( sizeof(equippers) )
			{
				foreach(object equipper in equippers)
					if( objectp(equipper) )
						equipper->unequip(ob, &status);	
			}
		}

		catch( ob->remove());

		// �I�s	efun �R������
		efun::destruct(ob);
	}
	return 1;
}

object load_user(string	id)
{
	object user;

	if( !stringp(id) )
		return 0;

	if( objectp(user = find_player(id)) )
		return user;

	if( !objectp(user = load_object(user_file_name(id))) )
		return 0;

	if( user->restore() )
		return user;

	destruct(user);
	return 0;
}

int user_exists(string id)
{
	object user;

	if( !stringp(id) ) return 0;

	user = load_user(id);

	if( !user ) return 0;

	if( !userp(user) )
		destruct(user);

	return 1;
}

object load_module(mixed filename)
{
	if( !filename ) return 0;

	if( arrayp(filename) )
	{
		foreach( string room in this_object()->city_roomfiles(filename) )
		{
			if( sscanf(room, "/city/%*s/%*d/room/%*d_%*d_%*d_%*s") == 6 )
				continue;

			return load_object(room[0..<3]);
		}			
		return 0;
	}			

	if( !stringp(filename) || !file_exists(filename) ) return 0;

	return load_object(filename);
}

int same_environment(object ob1, object ob2)
{
	return objectp(ob1) && objectp(ob2) && member_array(ob2, present_objects(ob1)) != -1;
}

