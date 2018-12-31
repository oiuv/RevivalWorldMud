/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : snoop_shadow.c
 * Author : Cookys@RevivalWorld
 * Date   : 2003-5-27
 * Note   : shadow object
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
#define SNOOP_AGENT "/cmds/std/wiz/snoop.c"

void create(object ob)
{
	if( clonep(this_object()) ) {
 		if( call_stack(1)[3]!=load_object("/cmds/std/wiz/snoop.c") )
	 	{
 			destruct(this_object());
 			return;
	 	}
	 	
	 	if( objectp(ob) && !shadow(ob,0))
 			shadow(ob);
 		else destruct(this_object());
 	}
 	//tell(find_player("cookys"),sprintf("snoop_shadow previous ob:%O\n",call_stack(1)));
 	
}


void catch_tell(string msg, string classes)              
{
	if( classes != "snoop" )
		SNOOP_AGENT->notify_snooper_catch(query_shadowing(this_object()),msg);
	
	return query_shadowing(this_object())->catch_tell(msg,classes);
}
/*
mixed process_input(string msg)
{
	SNOOP_AGENT->notify_snooper_cmd(query_shadowing(this_object()),msg);
	
	return query_shadowing(this_object())->process_input(msg);
}

*/