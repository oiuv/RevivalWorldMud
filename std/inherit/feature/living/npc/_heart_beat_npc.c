/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _heart_beat_npc.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-10-10
 * Note   : NPC �߸�
 * Update :
 *  o 2005-02-12  �ץ� tick �H���ˬd save �禡�O�_�s�b
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <location.h>
#include <function.h>

private nosave int tick;

nomask void condition_handler()
{
	object me = this_object();
	mapping condition;
	
	if( mapp(condition = query("condition")) )
	{
		foreach( string key, mapping value in condition )
		{
			if( value["time"] <= 0 )
			{
				catch(replace_string(key, "#","/")->stop_effect(this_object()));
				delete("condition/"+key);
				continue;
			}
			
			if( value["heart_beat"] > 0 && !(value["time"] % value["heart_beat"]) )
				catch(replace_string(key, "#","/")->heart_beat_effect(this_object()));
			
			addn("condition/"+key+"/time", -1);
		}
	}

	if( !me->is_stamina_full() )
		me->earn_stamina(this_object()->stamina_regen());
	if( !me->is_health_full() )
		this_object()->earn_health(me->health_regen());
	if( !me->is_energy_full() )
		this_object()->earn_energy(me->energy_regen());

	// �C 25 �����@��
	if( !(tick % 25) )
	{
		// �����P����
		if( me->query_food_cur() > 0 )
			me->cost_food(1);
		if( me->query_drink_cur() > 0 )
			me->cost_drink(1);
	}

	if( query("faint") )
	{		
		float recover_speed = 1 + me->query_skill_level("consciousness")/100.;
		
		if( me->query_stamina_cur()*recover_speed >= me->query_stamina_max()/2 )
			me->wakeup();
	}
}

/*
	NPC �欰��z�� :
		
	*behavior/reply_say	���� say ���O�ɳQ�ʩI�s
	(function) fp(object this_player(), string "���ܤ��e")
	
	*behavior/reply_emote	��������O�ɳQ�ʩI�s
	(function) fp(object this_player(), string "�����O", string "�����[��r")
	
	*behavior/walking	�� heart_beat �D�ʨ̷� walking �]�w��Ʃw������
	(int) ���

	*behavior/shopping	�� heart_beat �D�ʨ̷� shopping �]�w��Ʃw���ʪ�
	(int) ���

	*behavior/get_salary	�� heart_beat �D�ʨ̷� get_salary �]�w��Ʃw�������~��
	(int) ���
	
	*behavior/random	�� heart_beat �C���I�s
	(mapping) random =
	([
		fp 1	:	�C�� heart_beat ���@�������O�����v 1(���:�U�����@),
		fp 2	:	�C�� heart_beat ���@�������O�����v 2(���:�U�����@),
	])
*/
		
void heart_beat()
{
	mapping behavior;

	if( !tick )
		tick = random(1024);
	else
		++tick;

	condition_handler();

	// �C���ɶ� 1 �p��(�Y��ڮɶ� 120 ��)�����@���~��
	if( !(tick % 120) )
	{
		if( addn("age_hour", 1) == 8760 )
			BIRTHDAY_D->grow_up(this_object());
	}

	if( mapp(behavior = query("behavior")) )
	{
		// ���ʦ欰
		if( behavior["walking"] && !(tick % behavior["walking"]) )
			this_object()->behavior_walking();

	
		// ���N���O�欰
		if( sizeof(behavior["random"]) )
		{
			foreach(function fp, int prob in behavior["random"])
				if( prob > random(10000) )
				{
					if( functionp(fp) & FP_OWNER_DESTED )
				        {
				                if( functionp(fp) & FP_NOT_BINDABLE )
				                	error("heart_beat_npc.c reply_say() �禡�L�k�A���s���C\n");
				
				                fp = bind(fp, this_object());
				        }
					evaluate(fp);
				}
		}
	}
	// fixed by sinji
	//if( !(tick%1800) && function_exists("save", this_object()) ) 
	//	this_object()->save();
		
	if( !(tick%1800) ) 
		this_object()->save();
}

int query_tick()
{
	return tick;
}