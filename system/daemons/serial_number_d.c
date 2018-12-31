/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : serial_number_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-6-19
 * Note   : �y�������F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#define DATA_PATH		"/data/daemon/serial_number.o"

private mapping serial_number;

string query_serial_number(string key)
{
	if( !stringp(key) )
		return 0;
	
	serial_number[key] = count(serial_number[key], "+", 1);
	
	save_object(DATA_PATH);

	return serial_number[key];
}

void remove_serial_number(string key)
{
	if( !stringp(key) )
		return;
		
	map_delete(serial_number, key);
	
	save_object(DATA_PATH);
}

void create()
{
	if( !restore_object(DATA_PATH) )
	{
		serial_number = allocate_mapping(0);
		save_object(DATA_PATH);
	}
}

string query_name()
{
	return "�Ǹ��t��(SERIAL_NUMBER_D)";
}
