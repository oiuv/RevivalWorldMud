/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : city_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-10
 * Note   : 城市精靈, 處理所有城市資料庫
 * Update :
 *  o 2002-09-17 Clode 重新設計城市資料結構與儲存資料程序
 *  o 2002-09-17 Clode 修改部份 code 提高容錯能力
 *
 -----------------------------------------
 */

inherit __DIR__"city_d_main.c";

private void create()
{
	if( clonep() )
		destruct(this_object());

	// 避免發生慘劇
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
	return "城市系統(CITY_D)";
}
