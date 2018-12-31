/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : unarm.c
 * Author : Cookys@RevivalWorld
 * Date   : 2003-5-13
 * Note   : 
 * Update :
 *  o 
 -----------------------------------------
*/


/*
	�C�� handler �̭����n�]�p���F��
	
	array query_numerical(object)
		return type: ({ heal_point , attack_point , defend_point , hit_rate , speed_point  })
	int query_frequency(object player)
		return type: int
		
	int costhp(object player,int costpoint);
	
	void show_score(object player);
	
	int strike_back(int speed);
		return int;

	add_flee_exp(player);
	add_sb_exp(opponent);
	add_att_exp()
	add_def_exp()
	add_hit_exp()
	add_dodge_exp()	
*/

#include <combat.h>
#include <ansi.h>

void add_flee_exp(object player)
{
	player->add_skill_exp("flee",random(5));
}

void add_sb_exp(object player)
{
	player->add_skill_exp("speed",random(5));
}

void add_att_exp(object player)
{
	player->add_skill_exp("attack",random(5));
}

void add_def_exp(object player)
{
	player->add_skill_exp("defend",random(5));
}

void add_hit_exp(object player) 
{
	player->add_skill_exp("hunting",random(5));
}

void add_dodge_exp(object player)
{
	player->add_skill_exp("dodge",random(5));
}

void kill_enemy_reward(object player,int exp)
{
	string *skill_array=({"flee","speed","attack","defend","hunting","dodge"});
	
	string skill=skill_array[random(sizeof(skill_array))];
	exp=exp*50+random(exp);
	
	tell(player,"�A���A�F�ĤH�A�o�����["+ HIG+ exp+ NOR+"]�I "+HIG+ skill+ NOR+" �g��C\n");
	
	player->add_skill_exp(skill,exp);
}

array npc_numerical(object npc)
{
	int attack_point=4, defend_point=3, hit_rate=50, speed_point=20,dodge_rate=20,lv=query("combat/unarm/lv",npc);
	
	if( !lv ) lv = 5;
	
	attack_point=attack_point*lv;
	defend_point=defend_point*lv;
	hit_rate=70+lv*2;
	speed_point+=lv*5;
	dodge_rate=dodge_rate+lv*3;
	
	return ({ dodge_rate,attack_point,defend_point,hit_rate,speed_point,"$ME�������٤F$YOU�@���A%^DAMAGE%^�C\n" });
}

array query_numerical(object player)
{
	int attack_point, defend_point, hit_rate, speed_point,dodge_rate;
	
	if( userp(player) )
	{
		dodge_rate   = query("combat/unarm/dodge_rate",player);
		attack_point = query("combat/unarm/attack_point",player);
		defend_point = query("combat/unarm/defend_point",player);
		hit_rate     = query("combat/unarm/hit_rate",player);
		speed_point  = query("combat/unarm/speed_point",player);
		
		attack_point = attack_point*2/3+random(attack_point/3);
		defend_point = defend_point*3/4+random(defend_point/3);
		
		return ({ query("combat/unarm/dodge_rate",player),attack_point,defend_point,hit_rate,speed_point,"$ME�������~�F$YOU�@���A%^DAMAGE%^�C\n" });
	}
	
	else return npc_numerical(player);
}


int query_frequency(object player)
{
        if( player->query_id(1)=="cookys" ) return 4;
	
	return 10; 
} 

int costhp(object player,int costpoint,int mode)
{
	if( costpoint < 0 ) return 0;
	
	if( query("abi/hp/cur",player) - costpoint < 1 )
	{
		// �p�G�O���, hp = 1 , str =0 , faint()
		if( mode == FIGHT )
		{
			set("abi/hp/cur",1,player);
			set("abi/str/cur",1,player);
                        //player->tell_client_hp();
			player->faint();
		}
		return 1;
	} else {
		//return 0;
		addn("abi/hp/cur",-costpoint,player);
                //player->tell_client_hp();
	}
	return 0;
}


string show_score(object player)
{
	return "�]�p��";
}

int strike_back(object player,int speed)
{
	//tell(find_player("cookys"),sprintf("%O %O\n",player,speed));
	if( speed < 1 ) return 0;
	if( random(speed) - random(speed/2) > 0 )
		return 1;
	
	return 0;
}                          

int do_flee(object me)
{
	return random(50)+query("combat/unarm/flee",me);
}

int guard_flee(object me)   
{
	return random(50);
}
