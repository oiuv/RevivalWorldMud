/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : combat_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-26
 * Note   : �԰����F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <equipment.h>
#include <weapon.h>
#include <buff.h>

mapping combat;

//
// �O�_���b�԰�
//
varargs int in_fight(object me, object enemy)
{
	if( !sizeof(combat[me]) )
		return 0;

	if( undefinedp(enemy) )
		return 1;
	else
		return member_array(enemy, combat[me]) != -1;
}

//
// �}�l�԰�
//
void start_fight(object me, object enemy)
{
	if( undefinedp(combat[me]) )
		combat[me] = allocate(0);

	if( undefinedp(combat[enemy]) )
		combat[enemy] = allocate(0);

	// �D�ʭn�D�԰�(�N���ǩԦܲĤ@��)
	combat[me] = ({ enemy }) | combat[me];
			
	// �Q�ʥ[�J�԰�
	combat[enemy] |= ({ me });
}

//
// ����԰�
//
void stop_fight(object me)
{
	if( arrayp(combat[me]) )
	foreach(object enemy in combat[me])
	{
		combat[enemy] -= ({ me });
		
		if( !sizeof(combat[enemy]) )
			map_delete(combat, enemy);
	}
	
	map_delete(combat, me);
}

void damage_message(object attacker, object defender, int damage)
{
	int cur = defender->query_health_cur() - damage;
	int max = defender->query_health_max();
	
	if( cur < 0 ) cur = 0;

	switch(cur * 10 / max)
	{
		case 0:
			msg(RED"$YOU"RED"����D�`�Y�����ˮ`�A����n�n���Y...["+cur+"/"+max+"]\n"NOR, attacker, defender, 1); 
			break;
		case 1..2:
			msg(HIR"$YOU"HIR"���W�A�崳���A�ʧ@�w�M���õL��...["+cur+"/"+max+"]\n"NOR, attacker, defender, 1); 
			break;
		case 3..4:
			msg(YEL"$YOU"YEL"������ЬƲ`�A�����o�Q���j�j...["+cur+"/"+max+"]\n"NOR, attacker, defender, 1); 
			break;
		case 5..6:
			msg(HIY"$YOU"HIY"�y�S�h�W�����A�����ĤO�a�������m...["+cur+"/"+max+"]\n"NOR, attacker, defender, 1); 
			break;
		case 7..8:
			msg(GRN"$YOU"GRN"���W���F�@�ǶˡA���ʧ@���M�Q���ӱ�...["+cur+"/"+max+"]\n"NOR, attacker, defender, 1); 
			break;
		case 9..10000000:
			msg(HIG"$YOU"HIG"���F�@�I���ˡA���h���o...["+cur+"/"+max+"]\n"NOR, attacker, defender, 1); 
			break;
	}
	
	return 0;
}

//
// �i������ʧ@
//
void attack(object attacker, object defender)
{
	string msg;
	int damage;			// �`������
	string armor_buff_type;		// �������
	object weapon, *weapons;
	object armor, *armors;
	object* defended_armors = allocate(0);

	weapons = attacker->query_equipping_object(EQ_HAND[PART_ID]) || attacker->query_equipping_object(EQ_TWOHAND[PART_ID]);
	armors = defender->query_equipping_object(EQ_BODY[PART_ID]);

	// �{�����
	if( !sizeof(weapons) )
	{
		damage = 0;

		msg = "$ME�����Y��$YOU�ĤO���h�A";
		
		// �R��
		if( random(attacker->query_agi()) > random(defender->query_agi()) )
		{			
			damage += attacker->query_str() - random(defender->query_phy());
				
			if( sizeof(armors) )
			{
				int armor_defense;

				foreach(armor in armors)
				{
					armor_defense = random(query(BUFF_ARMOR_INJURY, armor));
					
					if( armor_defense > 0 )
					{
						defended_armors += ({ armor });
						damage -= random(armor_defense+1);
					}
					else if( armor_defense < 0 )
						damage += random(-armor_defense-1);
					
					if( sizeof(defended_armors) )	
						msg += "���Q$YOU��"+defended_armors[random(sizeof(defended_armors))]->query_name()+"�פU�F�@�����������C\n";
					else
						msg += "�����R���C\n";
				}
			}
			else
				msg += "�����R���C\n";
				
			if( damage <= 5 ) damage = random(5)+1;
		}
		else
			msg += "���Q$YOU�{�}�C\n";

		msg(msg, attacker, defender, 1);
		
		if( damage > 0 )
			damage_message(attacker, defender, damage);

		// ���㪺�W�߯S���m
		if( sizeof(defended_armors) )	
			defended_armors[random(sizeof(defended_armors))]->special_defend(attacker, defender);
		
		if( attacker->is_faint() )
			return;
		
		// ���ѼĤH
		if( !defender->cost_health(damage) )
			return;
	}
	// �Z������
	else foreach(weapon in weapons)
	{
		damage = 0;

		switch(query("weapon_type", weapon))
		{
			case WEAPON_TYPE_SLASH: 	armor_buff_type = BUFF_ARMOR_SLASH; break;
			case WEAPON_TYPE_GORE: 		armor_buff_type = BUFF_ARMOR_GORE; break;
			case WEAPON_TYPE_EXPLODE: 	armor_buff_type = BUFF_ARMOR_EXPLODE; break;
			case WEAPON_TYPE_INJURY: 	armor_buff_type = BUFF_ARMOR_INJURY; break;
			case WEAPON_TYPE_FIRE: 		armor_buff_type = BUFF_ARMOR_FIRE; break;
			case WEAPON_TYPE_ICE: 		armor_buff_type = BUFF_ARMOR_ICE; break;
			case WEAPON_TYPE_POISON: 	armor_buff_type = BUFF_ARMOR_POISON; break;
			case WEAPON_TYPE_HEART: 	armor_buff_type = BUFF_ARMOR_HEART; break;
		}			

		msg = "$ME�ϥ�"+weapon->query_name()+"�V$YOU�i������A";

		// �R��
		if( random(attacker->query_agi()) > random(defender->query_agi()) )
		{
			damage += attacker->query_str() - random(defender->query_phy());
				
			if( sizeof(armors) )
			{
				int armor_defense;
				
				foreach(armor in armors)
				{
					armor_defense = random(query(armor_buff_type, armor));
					
					if( armor_defense > 0 )
					{
						defended_armors += ({ armor });
						damage -= random(armor_defense+1);
					}
					else if( armor_defense < 0 )
						damage += random(-armor_defense-1);
					
					if( sizeof(defended_armors) )	
						msg += "���Q$YOU��"+defended_armors[random(sizeof(defended_armors))]->query_name()+"�צ�F�@�����������C\n";
					else
						msg += "�����R���C\n";
				}
			}
			else
				msg += "�����R���C\n";
			
			if( damage <= 5 ) damage = random(5)+1;
		}
		else
			msg += "���Q$YOU�{�}�C\n";
			
		msg(msg, attacker, defender, 1);
			
		if( damage > 0 )
			damage_message(attacker, defender, damage);

		// ���㪺�W�߯S���m
		if( sizeof(defended_armors) )	
			defended_armors[random(sizeof(defended_armors))]->special_defend(attacker, defender);

		if( attacker->is_faint() )
			return;

		// ���ѼĤH	
		if( !defender->cost_health(damage) )
			return;

		// �Z�����W�߯S�����
		weapon->special_attack(attacker, defender);
	}
}

void heart_beat()
{
	object enemy;
	object *stop_fight = allocate(0);

	// �R������������
	map_delete(combat, 0);
	
	foreach(object me, array data in combat)
	{
		if( !arrayp(combat[me]) || !sizeof(combat[me]) )
		{
			stop_fight += ({ me });
			continue;
		}

		combat[me] -= ({ 0 });
		
		foreach(enemy in combat[me])
		if( !same_environment(me, enemy) )
		{
			msg("$ME�P$YOU����԰��C\n", me, enemy, 1);

			combat[me] -= ({ enemy });
			combat[enemy] -= ({ me });
		}

		// �ĤH�q�q�����F�Φۤv�w�ˤF
		if( !sizeof(combat[me]) || me->is_faint() )
		{
			stop_fight += ({ me });
			continue;
		}
		
		attack(me, combat[me][0]);
	}
	
	foreach(object me in stop_fight)
		stop_fight(me);
}

void create()
{
	combat = allocate_mapping(0);
	
	set_heart_beat(30);
}

int remove()
{
	foreach(object ob, mapping data in combat)
		tell(ob, "�԰��t�Χ�s�A����԰��C\n");
}

string query_name()
{
	return "�԰��t��(COMBAT_D)";
}

