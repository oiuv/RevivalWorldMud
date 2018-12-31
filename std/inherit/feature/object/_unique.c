/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _unique.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-12-17
 * Note   : �ߤ@���~����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

private void dest()
{
	destruct(this_object());
}

int is_unique()
{
	return 1;
}

void unique_handle()
{
	foreach( object ob in filter_array( children(base_name(this_object())), (: clonep :)) )
	{
		if( ob != this_object() )
			destruct(ob);
		else call_out((:dest:),2);
	}
}