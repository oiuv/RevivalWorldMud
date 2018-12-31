/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _reset_room.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-4
 * Note   : �ж����m�~����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <message.h>

void reset_objects()
{
	string basename;
	mixed amount;
	mapping objects, default_objects;
	
	if( !this_object()->query_database() )
		return;

	objects = copy(query("temp_objects")) || allocate_mapping(0);
	
	default_objects = copy(query("objects"));

	if( sizeof(default_objects) )
		foreach( basename, amount in default_objects )
			objects[basename] = count(objects[basename],"+",amount);
			
	if( sizeof(objects) )
	{
		object ob;
		
		// �M���ж����즳�w����������
		destruct(filter_array(all_inventory(), (: $(objects)[base_name($1)] :)));
		
		foreach( basename, amount in objects )
		{
			catch { ob = new(basename) || load_object(basename); };

			if( !objectp(ob) ) continue;
			
			if( !query("flag/no_amount", ob) )
				set_temp("amount", amount, ob);
			else
				while((amount = count(amount,"-",1)) != "0") 
					(new(basename)||load_object(basename))->move(this_object());
				
			ob->move(this_object());
		}
	}
	
	delete("temp_objects");
}

void reset()
{

}
