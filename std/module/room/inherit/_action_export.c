/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _export_action.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-17
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>

void output_object(object env, string database, string basename, string amount);

// ���~��X���O
void do_export(object me, string arg, string database)
{
	int i, j, which = 1;
	object env, master, ob, newob;
	mapping objects;
	string amount = "1", oamount, env_city, objectname, basename;
	string *shelfsort, shelf;
	
	env = environment(me);
	master = env->query_master();

	if( query("owner", env) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"�L�k�b�O�H���ؿv����X���~�C\n");

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n��X���򪫫~�H\n");
	
	env_city = env->query_city();
	
	sscanf(arg, "%s %s", amount, objectname);
	
	if( !objectname || (amount != "all" && !(amount = big_number_check(amount))) )
	{
		objectname = arg;
		amount = "1";
	}
	
	if( sscanf( objectname, "%s %d", arg, which ) == 2 )
	{
		if( which < 1 ) which = 1;
		objectname = arg;
	}

	objects = query(database, master);
	
	if( !sizeof(objects) )
		return tell(me, env->query_room_name()+"�̨èS�����󪫫~�C\n");

	shelfsort = keys(objects) | (query("shelfsort", master) || allocate(0));

	foreach( shelf in shelfsort )
	{
		for(i=0;i<sizeof(objects[shelf]);i+=2)
		{
			basename = objects[shelf][i];
			oamount = objects[shelf][i+1];
			
			if( catch(ob = load_object(basename)) )
				continue;

			// �Y���~�w�g����
			if( !objectp(ob) )
			{
				error(basename+" �ӫ~��ƿ��~�C");
				continue;
			}

			j++;
	
			if( j == to_int(objectname) || objectname == "all" || (ob->query_id(1) == lower_case(objectname) && !(--which)) )
			{
				if( amount == "all" || objectname == "all" )
					amount = oamount;
				else if( count(amount,">",oamount) )
					return tell(me, "�o�̨èS���o��h"+(query("unit", ob)||"��")+ob->query_idname()+"�C\n");
				else if( count(amount, "<", 1) )
					return tell(me, "�п�J���T����X�ƶq�C\n");
			
				newob = new(basename);
				
				if( !query("flag/no_amount", newob) )
					set_temp("amount", amount, newob);
	
				msg("$ME�N"+QUANTITY_D->obj(ob, amount)+"��"+env->query_room_name()+"��X�C\n",me,0,1);

				if( !me->get_object(newob, amount) )
				{
					if( count(amount, "==", 1) )
					{
						tell(me, pnoun(2, me)+"���W�����~�Ӧh�Τӭ��L�k�A����h�F��F�A�u�n����b�a�W�C\n");
						newob->move_to_environment(me);
					}
					else
					{
						tell(me, pnoun(2, me)+"���W�����~�Ӧh�Τӭ��L�k�A����h�F��F�C\n");
						destruct(newob);
						return;
					}
				}
	
				output_object(master, database, basename, amount);
				
				if( objectname != "all" )
					return;
			}
		}
	}
	
	if( objectname != "all" )
		return tell(me, "�o�̨èS�� "+objectname+" �o�ذӫ~�C\n");
	else if( master->query_building_type() == "warehouse" )
		master->save();
}
