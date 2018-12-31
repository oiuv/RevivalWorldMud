/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : area_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-18
 * Note   : �ϰ���F, �B�z�Ҧ��ϰ��Ʈw
 * Update :
 *
 -----------------------------------------
 */

inherit __DIR__"area_d_main.c";

private void create()
{
	if( clonep() )
		destruct(this_object());

	// �קK�o�ͺG�@
	if( base_name(this_object()) != "/system/daemons/area_d" )
		destruct(this_object());

	this_object()->restore_all_data();
}

int remove()
{
	return this_object()->save_all();
}

string query_name()
{
	return "���Ϩt��(AREA_D)";
}
