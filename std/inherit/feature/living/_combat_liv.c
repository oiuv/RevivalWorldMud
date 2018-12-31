/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _combat_liv.c
 * Author : Sinji@RevivalWorld
 * Date   : 2005-1-8
 * Note   : Refer to /std/inherit/feature/living/usr/_loginload_usr.
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */
 
#ifdef USE_THIS_COMBAT

#include <daemon.h>
#include <message.h>
#include <ansi.h>

nosave mapping limbs = ([
	"head":		"�Y��",
	"neck":		"��l",
	"body":		"����", 
	"hand":		"��",
	"feet":		"�}",
]);
nosave object *weapons = allocate(2);
nosave object *targets = allocate(0);
nosave mapping equipments = allocate_mapping(0);
nosave int target_turns = 0;

object *query_weapons()
{
	return weapons;
}
string *query_limbs()
{
	return keys(limbs);
}
string *query_limb_names()
{
	return values(limbs);
}
string query_limb_name(string limb)
{
	return limbs[limb];
}
object query_equip(string limb)
{
	return equipments[limb];
}
mapping query_equips()
{
	return equipments;
}
string query_limb(object equip)
{
	int i = member_array(equip, values(equipments));
	if( i < 0 ) return 0;
	return keys(equipments)[i];
}

/*
wield_weapon return values: 
 0: no argument.
-1: it's not weapon
-2: you have no hand to take weapon
-3: you already wield the weapon
-4: it's not allowable to wield
*/
int wield_weapon(object weapon)
{
	int weapon_hands, used_hands;

	if( !weapon ) return 0;
	if( !weapon->is_weapon() ) return -1;
	weapon_hands = (weapon->is_singlehand() ? 1:2);
	used_hands = sizeof(weapons - ({ 0 }));
	// Determine whether have hands to use weapon
	if( (used_hands + weapon_hands) > 2 ) return -2;
	if( member_array(weapon, weapons) != -1 ) return -3;
	if( !weapon->allowable_wield(this_object()) ) return -4;
	while(weapon_hands--) weapons[weapon_hands] = weapon;
	weapon->set_wield(1);
	return 1;
}
/*
un-wield_weapon return values: 
 0: no argument.
-1: it's not weapon
-2: you not have any weapons
-3: you have not wield the weapon
-4: it's not allowable to un-wield
*/
int unwield_weapon(object weapon)
{
	int i;

	if( !weapon ) return 0;
	if( !weapon->is_weapon() ) return -1;
	if( !sizeof(weapons) ) return -2;
	if( (i = member_array(weapon, weapons)) == -1 ) return -3;
	if( !weapon->allowable_unwield(this_object()) ) return -4;

	if( !weapons->is_singlehand() )
		weapons = allocate(2);
	else
		weapons[i] = 0;

	weapon->set_wield(0);
	return 1;
}

/*
wear_equip return values: 
 0: no argument.
-1: it's not equipment
-2: you have no such limb to wear equip
-3: the limb already have equipment
-4: it's not allowable to equip
*/
int wear_equip(object equipment)
{
	string limb;

	if( !equipment ) return 0;
	if( !equipment->is_equipment() ) return -1;

	limb = equipment->query_limb();
	if( undefinedp(limbs[limb]) ) return -2;
	if( equipments[limb] ) return -3;
	if( !equipment->allowable_wear(this_object()) ) return -4;
	equipments[limb] = equipment;
	equipment->set_wear(1);
	return 1;
}
/*
remove_equip return values:
 0: no argument.
-1: it's not equipment
-2: you have no such limb to remove equip
-3: the limb have no equipment
-4: it's not allowable to remove
*/
int remove_equip(mixed limb)
{
	object equipment;
	
	if( objectp(limb) ) limb = limb->query_limb();
	if( !limb ) return 0;
	if( undefinedp(limbs[limb]) ) return -2;
	equipment = equipments[limb];
	if( !equipment ) return -3;
	if( !equipment->is_equipment() ) return -1;
	if( !equipment->allowable_remove(this_object()) ) return -4;
	map_delete(equipments, limb);
	equipment->set_wear(0);
	return 1;
}

// Auto-load equipments
void restore_equipments()
{
	foreach(object ob in all_inventory(this_object()))
	{
		if( !ob->is_equipment() ) continue;
		if( ob->is_wear() )
		{
			wear_equip(ob);
			continue;
		}
		if( ob->is_wield() )
		{
			wield_weapon(ob);
			continue;
		}
	}
}

int is_fighting()
{
	return COMBAT_D->is_fighting(this_object());
}

int fightable()
{
	return 1;
}
void fight(object target)
{
	if( !objectp(target) ) return;
	if( !target->fightable() ) return;
	targets += ({ target });
}
varargs void stop_fight(object target, int flag)
{
	int i;

	if( !objectp(target) ) return;
	if( (i = member_array(target, targets)) < 0 ) return;
	targets -= ({ target });
	msg("$ME�������$YOU�C\n", this_object(), target, 0, 1, ENVMSG);
	if( flag ) target->stop_fight(this_object());
}
void attack()
{
	object target, weapon;

	targets -= ({ 0 });
	if( !sizeof(targets) ) return;
	target_turns %= sizeof(targets);
	target = targets[target_turns];
	weapon = weapons[0];
	if( !weapon ) weapon = weapons[1];
	
	if( objectp(weapon) )
	{
		msg("$ME����" + weapon->short() + "����$YOU�A", this_object(), target, 1, ENVMSG);
		weapon->attack(this_object(), target);
		return;
	}
	msg("$ME��������$YOU�A", this_object(), target, 1, ENVMSG);
	target->receive_attack(this_object(), random(10)+1);
	target_turns++;
	return;
}
void receive_attack(object from, int damage)
{
	string limb = keys(limbs)[random(sizeof(limbs))]; // random limb
	object equip;

	if( objectp(from) && member_array(from, targets) < 0 )
	{
		targets += ({ from });
	}
	if( objectp(equip = query_equip(limb)) )
	{
		msg("��! ���@�n����" + equip->short() + "�C\n", this_object(), from, 1, ENVMSG);
		equip->receive_attack(damage);
		return;
	}
	msg("���ꪺ���b" + limbs[limb] + "�W�C\n", this_object(), from, 1, ENVMSG);
	this_object()->receive_damage(from, damage);
}

void receive_damage(object from, int damage)
{
	mapping dbase;
        
	dbase = this_object()->query_database();
	msg("( $ME����F " + damage + " �I�ˮ` )\n", this_object(), from, 0, ENVMSG);
	addn("abi/hp/cur", -damage, this_object());
	if( dbase["abi"]["hp"]["cur"] < 0 )
	{
		targets->stop_fight(this_object());
		targets = allocate(0);
		msg(HIR"\n$ME���F�C\n\n"NOR, this_object(), 0, 1, ENVMSG);
	}
}

#endif
