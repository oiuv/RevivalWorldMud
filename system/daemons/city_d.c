/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : city_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-10
 * Note   : �������F, �B�z�Ҧ�������Ʈw
 * Update :
 *  o 2002-09-17 Clode ���s�]�p������Ƶ��c�P�x�s��Ƶ{��
 *  o 2002-09-17 Clode �קﳡ�� code �����e����O
 *
 -----------------------------------------
 */

inherit __DIR__"city_d_main.c";

private void create()
{
	if( clonep() )
		destruct(this_object());

	// �קK�o�ͺG�@
	if( base_name(this_object()) != "/system/daemons/city_d" )
		destruct(this_object());

	this_object()->restore_all_data();
}

int remove()
{
	return this_object()->save_all();
}
string query_name()
{
	return "�����t��(CITY_D)";
}
