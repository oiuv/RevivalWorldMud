/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sysdatabase_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-6-19
 * Note   : �t�θ�Ʈw���F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>

#define DATA_PATH		"/data/daemon/sysdatabase.o"
#define BACKUP_DATA_PATH	"/data/daemon/sysdatabase_bakcup.o"

inherit DATABASE;

object query_ob()
{
	return this_object();
}

int save()
{
	return save_object(DATA_PATH) && save_object(BACKUP_DATA_PATH);
}

int remove()
{
	return save();
}

void create()
{
	if( !restore_object(DATA_PATH) && !restore_object(BACKUP_DATA_PATH) )
		save();
}
string query_name()
{
	return "������Ʈw�t��(SYSDATABASE_D)";
}
