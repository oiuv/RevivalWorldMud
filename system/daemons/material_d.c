/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : material_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-4-15
 * Note   : ���ƺ��F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <material.h>
#include <map.h>
#include <daemon.h>
#include <inherit.h>

#define INITIAL_RESOURCE_AMOUNT	200


string percentage_color(int p)
{
	switch(p)
	{
		case 0..50:
			return HIY+p+"%"+NOR;
		case 51..99:
			return HIG+p+"%"+NOR;
		case 100..999999:
			return HIW+"100%"+NOR;
		default:
			return HIR+p+"%"+NOR;
	}
}

string material_percentage(mapping mtr_required, mapping mtr_exerted)
{
	int num;
	string mtr_name, msg = "";
	
	foreach(mtr_name, num in mtr_required)
		msg += MATERIAL_CHINESE[mtr_name]+"�G"+percentage_color(mtr_exerted[mtr_name]*100/mtr_required[mtr_name])+"�B";

	return msg[0..<3];		
}
	
int is_accomplished(mapping mtr_required, mapping mtr_exerted)
{
	foreach(string mtr_name, int num in mtr_required)
		if( mtr_exerted[mtr_name] < mtr_required[mtr_name] )
			return 0;

	return 1;
}

string material_required_info(mapping mtr_required)
{
	string msg = "";
	
	foreach(string mtr_name, int num in mtr_required)
		msg += MATERIAL_CHINESE[mtr_name]+" "+num+" ���B";

	return msg[0..<3];
}

private void create()
{
	if( clonep() )
	{
		destruct(this_object());
		return;
	}
}

string query_name()
{
	return "���ƨt��(MATERIAL_D)";
}
