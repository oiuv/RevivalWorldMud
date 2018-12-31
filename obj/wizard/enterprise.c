/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : enterprise.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-08
 * Note   : ���~��
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>
#include <daemon.h>
#include <secure.h>
#include <map.h>
#include <location.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

mapping actions;

void fire_process(object me, object target, int combo, int sec)
{
	if( sec == 0 )
	{
		int damage, faint;
		for(int i=0;i<combo;i++)
		{
			damage = range_random(1, 125);
			
			if( !target->cost_health(damage) )
				faint = 1;
			
			msg("$YOU�Q$ME�o�g�� 12 �������}�C�����A�ɭP "HIR+damage+NOR" ���ͩR�ˮ`("NOR GRN+target->query_health_cur()+NOR"/"HIG+target->query_health_max()+NOR")�C\n", me, target, 1);

			if( faint )
				target->faint();
		}
		
		delete_temp("delay/fire");
	}
	else
	{
		msg("�Z�� 12 �������}�C����$YOU�٦� "+sec+" ����...\n", me, target, 1);
		call_out((: fire_process, me, target, combo, sec-1 :), 1);
	}
}

void fire(object me, string arg)
{
	int combo = random(11)+1;
	object target = find_player(arg) || present(arg);
	
	if( !me->is_equipping_object(this_object()) )
		return tell(me, pnoun(2, me)+"�������˳�"+this_object()->query_idname()+"�C\n");

	if( query_temp("delay/fire") > time() )
		return tell(me, "���Z���L�k�s��o�g�A�е��ݪZ���t�ΧN�o�C\n");

	if( !objectp(target) )
		return tell(me, "�S�� "+arg+" �o�ӤH�C\n");
	
	for(int i=0;i<combo;i++)
	msg("$ME���a"+this_object()->query_idname()+"���$YOU�o�g 12 �������}�C("HIY+combo+NOR YEL" �s�o"NOR")�C\n", me, target, 1);
	set_temp("delay/fire", time() + 5);
	call_out((: fire_process, me, target, combo, 5 :), 1);
}

void shield(object me, string arg)
{
	if( !me->is_equipping_object(this_object()) )
		return tell(me, pnoun(2, me)+"�������˳�"+this_object()->query_idname()+"�C\n");

	if( !query_temp("shield/on") )
	{
		msg("$ME�Ұ�"+this_object()->query_idname()+"�۰ʽ��W���@�n�t�ΡC\n", me, 0, 1);
		set_temp("status", HIR"(���@�n)"NOR);
		set_temp("shield/on", 1);
	}
	else
	{
		msg("$ME����"+this_object()->query_idname()+"�۰ʽ��W���@�n�t�ΡC\n", me, 0, 1);
		delete_temp("status");
		delete_temp("shield/on");
	}
}

void antimatter_process(object me, object target, int sec)
{
	if( sec == 0 )
	{
		msg("$YOU�Q$ME�ұҰʪ��Ϫ��贲���f���������A�ɭP "HIR+1000000+NOR" ���ͩR�ˮ`("NOR RED+"-1000000"+NOR"/"HIG+target->query_health_max()+NOR")�C\n", me, target, 1);

		target->faint();
		
		if( userp(target) )
		LOGOUT_D->quit(target);
		
		delete_temp("delay/antimatter");
	}
	else
	{
		msg("�Z���Ϫ��贲���f�E����q�����٦� "+sec+" ����...\n", me, target, 1);
		call_out((: antimatter_process, me, target, sec-10 :), 10);
	}
}

// �Ϫ��贲���f
void antimatter(object me, string arg)
{
	object target = find_player(arg) || present(arg);
	
	if( !me->is_equipping_object(this_object()) )
		return tell(me, pnoun(2, me)+"�������˳�"+this_object()->query_idname()+"�C\n");

	if( query_temp("delay/antimatter") > time() )
		return tell(me, "���Z���L�k�s��o�g�A�е��ݪZ���t�ΧN�o�C\n");

	if( !objectp(target) )
		return tell(me, "�S�� "+arg+" �o�ӤH�C\n");
		
	msg("$ME���a"+this_object()->query_idname()+"���$YOU�}�ҤϪ��贲���f�C\n", me, target, 1);
	set_temp("delay/antimatter", time() + 60);
	call_out((: antimatter_process, me, target, 60 :), 1);
}

void create()
{
	set_idname(HIW"enter"WHT"prise"NOR, HIW"��"WHT"�~��");
	set_temp("status", HIY"��"NOR YEL"��");

	actions = ([
		"fire" : (: fire :),
		"shield" : (: shield :),
		"antimatter": (: antimatter :),
	]);

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", @LONG
  �t�ƶW���t���檺���t�������P�ھ�Ų�A�֦�����������⪺��O
  
    �Z��
      11 �� 12 �������}�C�A�`��X�q�� 8 �U 5 �d����
      1 �Ӱ��t�q�l���p�ޡA�˳� 50 �T�q�l���p
      4 �� 4 ���s�o�����p�ޡA�`�@�˳Ʀ� 250 �T���p
      �Ϫ��贲���f�C
  
    ���m
      �۰ʽ��W���@�n�t�ΡA�`�e�q�� 459 �U���J
      �����G�׽ƦX�K����P�T�׽ƦX�K������h�~�ߡA�[�W 10 ���̪��ܿN�k�˥�
      ���ŵ��c�O�@�ޡC 
  
    ���t��O
      ���`���覱�t 8 ��
      ���t���覱�t 9.9 ��
      �̤j��O�O�H���t 9.99 ��� 24 �p�ɡC   
   
    ���t��Ӫ�
    ���t(��)     ���t(�C��� 299,792,458 ����/��)
    [1;37m1            [1;30m1
    [1;37m2            [1;30m8
    [1;37m3            [1;30m27
    [1;37m4            [1;30m64
    [1;37m5            [1;30m125
    [1;37m6            [1;30m216
    [1;37m7            [1;30m343
    [1;37m8            [1;30m512
    [1;37m9            [1;30m729      
    [1;37m10           [1;30m1000
    [1;37m11           [1;30m1331
    [1;37m12           [1;30m1728
    [1;37m13           [1;30m2197
    [1;37m14.1         [1;30m2803.221  [0m
LONG
	);

	set("unit", "��");
	set("mass", 0);
	set("value", 0);
	set("flag/no_amount", 1);

	set(BUFF_STR, 100);
	set(BUFF_PHY, 100);
	set(BUFF_INT, 100);
	set(BUFF_AGI, 100);
	set(BUFF_CHA, 100);

	set(BUFF_STAMINA_MAX, 100);
	set(BUFF_HEALTH_MAX, 100);
	set(BUFF_ENERGY_MAX, 100);
	
	set(BUFF_STAMINA_REGEN, 100);
	set(BUFF_HEALTH_REGEN, 100);
	set(BUFF_ENERGY_REGEN, 100);

	set(BUFF_LOADING_MAX, 100);
	set(BUFF_FOOD_MAX, 100);
	set(BUFF_DRINK_MAX, 100);
	
	set(BUFF_SOCIAL_EXP_BONUS, 100);

	set("buff/status", HIW"��"WHT"�~��");
	
	::setup_equipment(EQ_MOUNT, HIG"�r"NOR GRN"�p"NOR);
}
