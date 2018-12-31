/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _remove_room.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-9-14
 * Note   : �ж������~����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

int remove()
{
	string basename, amount;
	mapping objects;
	mapping temporary_objects = allocate_mapping(0);
	
	if( !this_object()->query_database() ) return 0;

	objects = copy(query("objects"));

	if( sizeof(objects) )
	foreach( basename, amount in objects )
		if( basename[<2..<1] == ".c" )
		{
			objects[basename[0..<3]] = amount;
			map_delete(objects, basename);
			set("objects", objects);
		}

	foreach( object ob in all_inventory() )
	{
		if( ob->is_user_ob() ) continue;

		basename = base_name(ob);
		amount = query_temp("amount", ob) || "1";

		if( mapp(objects) && objects[basename] && count(amount, ">=", objects[basename]) )
		{
			temporary_objects[basename] = count(count(temporary_objects[basename],"+",amount), "-", objects[basename]);

			if( temporary_objects[basename] == "0" )
				map_delete(temporary_objects, basename);
		}	
		else
			temporary_objects[basename] = count(temporary_objects[basename], "+", amount);
	}
			
	if( sizeof(temporary_objects) )
		set("temp_objects", temporary_objects);
	
	return this_object()->save();
}
