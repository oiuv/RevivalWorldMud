/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _object_database_mod.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-17
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

 /*
���~����x�s�榡
([
	"����1" :	({"basename_1", "amount_1", "basename_2", "amount_2", ... }),
	"����2" :	({"basename_1", "amount_1", "basename_2", "amount_2", ... }),
	...
])
*/

/*
�w�p�ק令�p�U���c
({
	"����1",
	({
		"basename_1", "amount_1",
		"basename_2", "amount_2",
		"basename_3", "amount_3",
	}),
	"����2",
	({
		"basename_1", "amount_1",
		"basename_2", "amount_2",
		"basename_3", "amount_3",
	}),
})


//
// �ˬd����J�����~�O�_�|�y���e�q�W�L
//
int over_capacity2(object master, string database, mixed amount, mixed capacity)
{
	int size;
	array objects;
	array objectslist;
	string totalamount;

	// �e�q�L��	
	if( capacity == -1 ) return 0;

	objects = query(database, master) || allocate(0);
	objectslist = implode(filter_array(objects, (: arrayp($1) :)), (:$1+$2:));

	capacity = count(capacity, "*", sizeof(master->query_slave())+1);

	size = sizeof(objectslist);
	
	for(int i=0;i<size;i+=2)
		totalamount = count(totalamount, "+", objectlist[i+1]);
		
	if( count(count(amount, "+", totalamount), ">", capacity) )
		return 1;

	return 0;
}
*/

// �W�L�e�q����
int over_capacity(object env, string database, mixed amount, mixed capacity)
{
	int size;
	mapping objects;
	array objectlist;
	string totalamount;
	
	if( capacity == -1 )
		return 0;

	objects = query(database, env) || allocate_mapping(0);
	objectlist = implode(values(objects), (:$1+$2:)) || allocate(0);
	
	capacity = count(capacity, "*", (sizeof(query("slave", env))+1));
	
	size = sizeof(objectlist);
	
	for(int i=0;i<size;i+=2)
		totalamount = count(totalamount, "+", objectlist[i+1]);

	if( count(count(amount, "+", totalamount), ">", capacity) )
		return 1;
	
	return 0;
}



// ��J���~
void input_object(object env, string database, string basename, mixed amount)
{
	mapping objects;
	array objectlist;
	string classname;
	
	objects = query(database, env) || allocate_mapping(0);
	objectlist = implode(values(objects), (:$1+$2:))||allocate(0);

	classname = query("setup/class/"+replace_string(basename, "/", "#"), env);
	
	// �p�G�쥻�S���o�ذӫ~
	if( member_array(basename, objectlist) == -1 )
	{
		if( !arrayp(objects[classname]) )
			objects[classname] = ({ basename, amount+"" });
		else
			objects[classname] += ({ basename, amount+"" });
	}
	else
	{
		int idx;
		string shelf, *data;
		foreach(shelf, data in objects)
		{
			idx = member_array(basename, data);
			
			if( idx != -1 )
			{
				objects[shelf][idx+1] = count(data[idx+1], "+", amount);
				break;
			}
		}
	}
	
	set(database, objects, env);
}

// ��X���~
void output_object(object env, string database, string basename, mixed amount)
{
	int idx;
	string shelf, *data;
	mapping objects;
	
	objects = query(database, env) || allocate_mapping(0);
	
	foreach(shelf, data in objects)
	{
		idx = member_array(basename, data);
		
		if( idx != -1 )
		{
			// �����R��
			if( amount == "all" || count(data[idx+1], "==", amount) )
			{
				objects[shelf] = objects[shelf][0..idx-1] + objects[shelf][idx+2..];
				
				if( !sizeof(objects[shelf]) )
					map_delete(objects, shelf);
			}
			else
				objects[shelf][idx+1] = count(objects[shelf][idx+1], "-", amount);
			
			break;
		}
	}
	
	if( sizeof(objects) )
		set(database, objects, env);
	else
		delete(database, env);
}

// �j�M module �W���ର basename
varargs string query_module_basename(object env, string database, string module, string shelflimit)
{
	int idx;
	string shelf, *data;
	mapping objects;
	object ob;
	
	objects = query(database, env) || allocate_mapping(0);
	
	foreach(shelf, data in objects)
	{
		if( !undefinedp(shelflimit) && shelf != shelflimit ) continue;

		for(idx=0;idx<sizeof(data);idx+=2)
		{
			catch( ob = load_object(data[idx]) );
		
			if( !objectp(ob) ) continue;
	
			if( ob->query_module() == module )
				return base_name(ob);
		}
	}
	
	return 0;
}


// �^�Ǫ��~�ƶq
varargs string query_object_amount(object env, string database, string basename, string shelflimit)
{
	int idx;
	string shelf, *data;
	mapping objects;
	
	objects = query(database, env) || allocate_mapping(0);
	
	foreach(shelf, data in objects)
	{
		if( !undefinedp(shelflimit) && shelf != shelflimit ) continue;

		idx = member_array(basename, data);
		
		if( idx != -1 )
			return data[idx+1];
	}
	
	return 0;
}


// �ಾ���~
void transfer_object(object from, object to, string fromdb, string todb, string basename, mixed amount)
{
	output_object(from, fromdb, basename, amount);
	input_object(to, todb, basename, amount);
}

// �����������
void refresh_class(object env, string database)
{
	int i, size;
	string newshelf, shelf, *data, basename;
	mapping objects;
	mapping new_objects = allocate_mapping(0);

	objects = query(database, env) || allocate_mapping(0);
	
	foreach(shelf, data in objects)
	{
		size = sizeof(data);

		for(i=0;i<size;i+=2)
		{
			basename = data[i];
			
			newshelf = query("setup/class/"+replace_string(basename, "/", "#"), env);
			
			if( undefinedp(new_objects[newshelf]) )
				new_objects[newshelf] = allocate(0);
				
			new_objects[newshelf] += data[i..i+1];				
		}
	}
	
	set(database, new_objects, env);
}
