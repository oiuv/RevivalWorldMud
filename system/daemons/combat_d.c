/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : combat_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-26
 * Note   : 戰鬥精靈
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
// 是否正在戰鬥
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
// 開始戰鬥
//
void start_fight(object me, object enemy)
{
	if( undefinedp(combat[me]) )
		combat[me] = allocate(0);

	if( undefinedp(combat[enemy]) )
		combat[enemy] = allocate(0);

	// 主動要求戰鬥(將順序拉至第一位)
	combat[me] = ({ enemy }) | combat[me];
			
	// 被動加入戰鬥
	combat[enemy] |= ({ me });
}

//
// 停止戰鬥
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
			msg(RED"$YOU"RED"受到非常嚴重的傷害，身體搖搖欲墜...["+cur+"/"+max+"]\n"NOR, attacker, defender, 1); 
			break;
		case 1..2:
			msg(HIR"$YOU"HIR"身上鮮血斑斑，動作已然雜亂無章...["+cur+"/"+max+"]\n"NOR, attacker, defender, 1); 
			break;
		case 3..4:
			msg(YEL"$YOU"YEL"身體受創甚深，行動顯得十分勉強...["+cur+"/"+max+"]\n"NOR, attacker, defender, 1); 
			break;
		case 5..6:
			msg(HIY"$YOU"HIY"臉露痛苦的表情，但仍奮力地攻擊防禦...["+cur+"/"+max+"]\n"NOR, attacker, defender, 1); 
			break;
		case 7..8:
			msg(GRN"$YOU"GRN"身上受了一些傷，但動作仍然十分敏捷...["+cur+"/"+max+"]\n"NOR, attacker, defender, 1); 
			break;
		case 9..10000000:
			msg(HIG"$YOU"HIG"受了一點輕傷，不痛不癢...["+cur+"/"+max+"]\n"NOR, attacker, defender, 1); 
			break;
	}
	
	return 0;
}

//
// 進行攻擊動作
//
void attack(object attacker, object defender)
{
	string msg;
	int damage;			// 總攻擊值
	string armor_buff_type;		// 防具種類
	object weapon, *weapons;
	object armor, *armors;
	object* defended_armors = allocate(0);

	weapons = attacker->query_equipping_object(EQ_HAND[PART_ID]) || attacker->query_equipping_object(EQ_TWOHAND[PART_ID]);
	armors = defender->query_equipping_object(EQ_BODY[PART_ID]);

	// 徒手攻擊
	if( !sizeof(weapons) )
	{
		damage = 0;

		msg = "$ME握緊拳頭往$YOU奮力揮去，";
		
		// 命中
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
						msg += "但被$YOU的"+defended_armors[random(sizeof(defended_armors))]->query_name()+"擋下了一部份的攻擊。\n";
					else
						msg += "完全命中。\n";
				}
			}
			else
				msg += "完全命中。\n";
				
			if( damage <= 5 ) damage = random(5)+1;
		}
		else
			msg += "但被$YOU閃開。\n";

		msg(msg, attacker, defender, 1);
		
		if( damage > 0 )
			damage_message(attacker, defender, damage);

		// 防具的獨立特殊防禦
		if( sizeof(defended_armors) )	
			defended_armors[random(sizeof(defended_armors))]->special_defend(attacker, defender);
		
		if( attacker->is_faint() )
			return;
		
		// 擊敗敵人
		if( !defender->cost_health(damage) )
			return;
	}
	// 武器攻擊
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

		msg = "$ME使用"+weapon->query_name()+"向$YOU進行攻擊，";

		// 命中
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
						msg += "但被$YOU的"+defended_armors[random(sizeof(defended_armors))]->query_name()+"擋住了一部份的攻擊。\n";
					else
						msg += "完全命中。\n";
				}
			}
			else
				msg += "完全命中。\n";
			
			if( damage <= 5 ) damage = random(5)+1;
		}
		else
			msg += "但被$YOU閃開。\n";
			
		msg(msg, attacker, defender, 1);
			
		if( damage > 0 )
			damage_message(attacker, defender, damage);

		// 防具的獨立特殊防禦
		if( sizeof(defended_armors) )	
			defended_armors[random(sizeof(defended_armors))]->special_defend(attacker, defender);

		if( attacker->is_faint() )
			return;

		// 擊敗敵人	
		if( !defender->cost_health(damage) )
			return;

		// 武器的獨立特殊攻擊
		weapon->special_attack(attacker, defender);
	}
}

void heart_beat()
{
	object enemy;
	object *stop_fight = allocate(0);

	// 刪除消失的物件
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
			msg("$ME與$YOU停止戰鬥。\n", me, enemy, 1);

			combat[me] -= ({ enemy });
			combat[enemy] -= ({ me });
		}

		// 敵人通通不見了或自己暈倒了
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
		tell(ob, "戰鬥系統更新，停止戰鬥。\n");
}

string query_name()
{
	return "戰鬥系統(COMBAT_D)";
}

