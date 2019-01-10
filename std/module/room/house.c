/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : house.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-03
 * Note   : ��v�ʧ@�~�Ӫ���
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <condition.h>
#include <delay.h>
#include <material.h>

inherit ROOM_ACTION_MOD;

#define REGEN	180

// �s�@�����~
void do_make(object me, string arg)
{
	int num;
	object env = environment(me);
	string option;

        if( query("owner", env) != me->query_id(1) )
        	return tell(me, "�o�̤��O"+pnoun(2, me)+"���a�C\n");

	if( !arg )
		return tell(me, pnoun(2, me)+"�n�s�@�����u���~�H(souvenir)\n");

	if( sscanf(arg, "%d %s", num, option) == 2 )
	{
		if( num <= 0 )
			return tell(me, "�ƶq����p��s�C\n");
	}
	else
	{
		num = 1;
		option = arg;
	}

	if( option == "souvenir" )
	{
		object ob;
		object metal;
		object stone;
		object wood;
		object water;
		object fuel;

		foreach(ob in all_inventory(me) | all_inventory(env) )
		{
			switch(base_name(ob))
			{
				case MATERIAL(METAL):	if( !objectp(metal) ) metal = ob;	break;
				case MATERIAL(STONE):	if( !objectp(stone) ) stone = ob;	break;
				case MATERIAL(WOOD):	if( !objectp(wood) ) wood = ob;		break;
				case MATERIAL(WATER):	if( !objectp(water) ) water = ob;	break;
				case MATERIAL(FUEL):	if( !objectp(fuel) ) fuel = ob;		break;
				default: break;
			}
		}

		if( !objectp(metal) || count(query_temp("amount", metal)||1, "<", num) )
			return tell(me, pnoun(2, me)+"����S��������"+MATERIAL(METAL)->query_idname()+"�C\n");
		if( !objectp(stone) || count(query_temp("amount", stone)||1, "<", num) )
			return tell(me, pnoun(2, me)+"����S��������"+MATERIAL(STONE)->query_idname()+"�C\n");
		if( !objectp(wood) || count(query_temp("amount", wood)||1, "<", num) )
			return tell(me, pnoun(2, me)+"����S��������"+MATERIAL(WOOD)->query_idname()+"�C\n");
		if( !objectp(water) || count(query_temp("amount", water)||1, "<", num) )
			return tell(me, pnoun(2, me)+"����S��������"+MATERIAL(WATER)->query_idname()+"�C\n");
		if( !objectp(fuel) || count(query_temp("amount", fuel)||1, "<", num) )
			return tell(me, pnoun(2, me)+"����S��������"+MATERIAL(FUEL)->query_idname()+"�C\n");

		if( !me->cost_energy(50 * num) )
			return tell(me, pnoun(2, me)+"�S���������믫�i�H�Ψӻs�@�����~�F�C\n");

		destruct(metal, num);
		destruct(stone, num);
		destruct(wood, num);
		destruct(water, num);
		destruct(fuel, num);

		ob = new("/product/souvenir_"+env->query_city());

		if( num > 1 )
			set_temp("amount", to_string(num), ob);

		msg("$ME�a�۲ӿ�����u�J�Ӧa�s�@�F "+num+" ��"+ob->query_idname()+"�C\n", me, 0, 1);

		ob->move(me);
	}
	else
		return tell(me, pnoun(2, me)+"�n�s�@�����u���~�H(souvenir)\n");
}

// �\Ū���x
void do_readmagazine(object me, string arg)
{
        object env = environment(me);

        if( query("owner", env) != me->query_id(1) )
        	return tell(me, "�o�̤��O"+pnoun(2, me)+"���a�C\n");

        msg("$ME���b���U��½�}���x�A���P�a�\Ū�Ӧۥ@�ɦU�a�������A�R��F�\�h���ѡA�]���@�����w���٦׻��P�U�ӡC\n", me, 0, 1);

	me->start_condition(SELFCHARGE);
}

// ��ı
void do_sleep(object me, string arg)
{
	int stamina_cost;
	int health_cost;
	int energy_cost;
	int stamina_time;
	int health_time;
	int energy_time;
	int max_time;
	object env = environment(me);

        if( query("owner", env) != me->query_id(1) )
        	return tell(me, "�o�̤��O"+pnoun(2, me)+"���a�C\n");

	if( me->is_delaying() )
		return tell(me, me->query_delay_msg());

	stamina_cost = me->query_stamina_max() - me->query_stamina_cur();
	health_cost = me->query_health_max() - me->query_health_cur();
	energy_cost = me->query_energy_max() - me->query_energy_cur();

	msg("$ME�ΪA�a���b�ɤW�A�@�w���N�εۤF(��_ "+stamina_cost+" ��O�B"+health_cost+" �ͩR�B"+energy_cost+" �믫)�C\n", me, 0, 1);

	stamina_time = 2 * stamina_cost / (me->stamina_regen() + REGEN) + 1;
	health_time = 2 * health_cost / (me->health_regen() + REGEN) + 1;
	energy_time = 2 * energy_cost / (me->energy_regen() + REGEN)+ 1;

	max_time = stamina_time;
	if( health_time > max_time ) max_time = health_time;
	if( energy_time > max_time ) max_time = energy_time;

	if( max_time <= 0 )
		tell(me, pnoun(2, me)+"�C�}�������F�_�ӡA�Pı�믫�ʭ��C\n");
	else
	{
		set_temp("rest_regen/stamina", REGEN, me);
		set_temp("rest_regen/health", REGEN, me);
		set_temp("rest_regen/energy", REGEN, me);
		me->start_delay(REST_DELAY_KEY, max_time, pnoun(2, me)+"���b��ı�C\n", pnoun(2, me)+"�C�}�������F�_�ӡA�Pı�믫�ʭ��C\n", bind((: delete_temp("rest_regen", $(me)), $(me)->set_stamina_full(), $(me)->set_health_full(), $(me)->set_energy_full() :), me));
	}
}

// OGC
void do_ogc(object me, string arg)
{
	object env = environment(me);

        if( query("owner", env) != me->query_id(1) )
        	return tell(me, "�o�̤��O"+pnoun(2, me)+"���a�C\n");

	if( me->is_delaying() )
		return tell(me, me->query_delay_msg());

        msg("$ME�q��P���X�_�Ǫ��D��A�]��ɤW�}�l���_�_�Ǫ��ʧ@...��...�@...�z...��...�C�C\n", me, 0, 1);

        me->faint();
}


// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
        "livingroom"    :
        ([
                "short" : HIW"���U"NOR,
                "help"  :
                        ([
"topics":
@HELP
    ���U�i�H���ѳíѪ��\�]�P�\Ū�Z���C
HELP,

"readmagazine":
@HELP
�\Ū���x�����O�A�Ϊk�p�U�G

readmagazine		- ���P�a�\Ū���x

HELP,

"make":
@HELP
�s�@�u���~�����O�A�Ϊk�p�U�G

make souvenir		- �s�@�¤�u���������~(�ݯӶO���j��ƦU�@��)
make '�ƶq' souvenir	- �@���s�@�j�q�������~

HELP,

                        ]),
                "action":
                        ([
                                "readmagazine"	: (: do_readmagazine :),
                                "make"		: (: do_make :),
                        ]),
        ]),
        "bedroom"       :
        ([
                "short" : HIW"�׫�"NOR,
                "help"  :
                        ([
"topics":
@HELP
    �׫ǥi�H�ΨӦn�n�a�Τ@ı�C
HELP,

"sleep":
@HELP
��ı�����O�A�Ϊk�p�U�G

sleep			- ����ɤW�I�I�j�ΡA���������𮧮Ĳv

HELP,

                        ]),
                "action":
                        ([
                                "sleep"       : (: do_sleep :),
                                "ogc"	      : (: do_ogc :),
                        ]),
        ]),
]);

// �]�w�ؿv�����
nosave array building_info = ({

        // �ؿv��������W��
        HIG"��"NOR GRN"�v"NOR

        // �}�i���ؿv�����̤֩ж�����
        ,1

        // �������̤j�ۦP�ؿv�ƶq����(0 �N������)
        ,0

        // �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
        ,INDIVIDUAL

        // �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
        ,AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION

        // �}�i�����O��
        ,"100000"

        // �ؿv���������ռаO
        ,0

        // �c�a�^�m��
        ,1

        // �̰��i�[�\�Ӽh
        ,10

        // �̤j�ۦP�ؿv���ƶq(0 �N������)
        ,1

        // �ؿv���ɥN
        ,1
});
