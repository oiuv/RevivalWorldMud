/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : birthday_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-12-23
 * Note   : �ͤ���F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <message.h>

void grow_up(object user)
{
	int talent;
	
	addn("age", 1, user);
	delete("age_hour", user);
	
	talent = (random(4)>0) + 2;
	
	tell(user, pnoun(2, user)+"��L�F "+query("age", user)+" �����ͤ�A�o��F "+talent+" �I�ѽ�ȡC\n", SYSMSG);

	addn("exp/talent", talent, user);
	addn("stat/water/max", random(6)+15, user);
	addn("stat/food/max", random(6)+15, user);
	
	user->save();
}

string query_name()
{
	return "�ͤ�t��(BIRTHDAY_D)";
}
