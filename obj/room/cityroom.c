/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cityroom.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-02
 * Note   : ��¦��������
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_MAPROOM;

string query_city()
{
	return query("city");
}

int query_num()
{
	return query("num");
}
