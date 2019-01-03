/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : occupy.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-02-27
 * Note   : ������O
 * Update :
 *  o 2005-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <location.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
    ���⫰�����O�A�L���y�����a�i����æ��߷s�����A�����y�����a�h�ݥѥ���
�Ӧ��⫰������L�ìP�����C

occupy city	- ���⦹�y����(����ɻݤ�I 100,000,000 ����y�O��)

HELP;

string WARNING = @LONG

    �нT�w�z�w�g�ԲӾ\Ū�������äF�Ѧ����إ߷s������i��J�쪺�U�ت��p
�]�A����ɻݭn��I $RW 100,000,000 ���d�B��y�O�ΡA�H�Ϋ����¦�س]�ݭn��
�U����X�A����g�礣���N�e���ɭP���������C
LONG;


// ����إߥ��s����
void confirm_occupy_new_city(object me, mapping info, string arg)
{
	if( !arg || (lower_case(arg) != "y" && lower_case(arg) != "yes") )
	{
		tell(me, pnoun(2, me)+"�����F����{�ǡC\n");
		return me->finish_input();
	}

	CITY_D->occupy_new_city(me, info["id"], info["name"], info["moneyunit"]);

	me->finish_input();
}


// ���߽ìP����
void confirm_occupy_section_city(object me, string arg)
{
	int needed_flourish;
	int total_num;
	string city;

	if( !arg || (lower_case(arg) != "y" && lower_case(arg) != "yes") )
	{
		tell(me, pnoun(2, me)+"�����F����{�ǡC\n");
		return me->finish_input();
	}

	city = query("city", me);

	total_num = CITY_D->query_city_num(city);
	needed_flourish = 50000 + (total_num-1)*5000;

	for(int num=0;num<total_num;num++)
	{
		if( CITY_D->query_section_info(city, num, "flourish") < needed_flourish )
			return tell(me, "�{���ìP������������F "+needed_flourish+" ���c�a�פ~��A����s�����C\n");
	}

	CITY_D->occupy_section_city(me);

	me->finish_input();
}


void input_money_unit(object me, mapping info, string arg)
{
	if( !arg || !arg[0] )
	{
		tell(me, pnoun(2, me)+"�����F����{�ǡC\n");
		return me->finish_input();
	}

	arg = ansi(arg);

	info["moneyunit"] = arg;
	tell(me, pnoun(2, me)+"�N�s�������f�����]�w���u"+arg+NOR"�v\n");

	tell(me, NOR CYN"\n�ĥ|�B�G"HIC"�H�W�]�w�O�_���T�L�~�H(Yes/No)"NOR CYN"�G"NOR);
	input_to((: confirm_occupy_new_city, me, info :));
}

void input_color_id(object me, mapping info, string arg)
{
	if( !arg || !arg[0] )
	{
		tell(me, pnoun(2, me)+"�����F����{�ǡC\n");
		return me->finish_input();
	}

	arg = ansi(arg);
	arg = replace_string(arg, " ", "");

	info["id"] = arg;
	tell(me, pnoun(2, me)+"�N�s�������m��^��W�٨��W���u"+arg+NOR"�v\n");

	tell(me, NOR CYN"\n�ĤT�B�G"HIC"�д�"+pnoun(2, me)+"���s�����]�w�f�������(2 �ӭ^��r)\n"NOR CYN"�G"NOR);
	input_to((: input_money_unit, me, info :));
}


void input_color_name(object me, mapping info, string arg)
{
	if( !arg || !arg[0] )
	{
		tell(me, pnoun(2, me)+"�����F����{�ǡC\n");
		return me->finish_input();
	}

	if( query("encode/gb", me) )
		arg = G2B(arg);

	arg = ansi(arg);
	arg = replace_string(arg, " ", "");

	info["name"] = arg;
	tell(me, pnoun(2, me)+"�N�s�������m�⤤��W�٨��W���u"+arg+NOR"�v\n");

	tell(me, NOR CYN"\n�ĤG�B�G"HIC"�д�"+pnoun(2, me)+"���s�������@�ӱm��^��W��(3 - 12 �ӭ^��r)\n"NOR CYN"�G"NOR);
	input_to((: input_color_id, me, info :));
}

private void do_command(object me, string arg)
{
	string mycity;
	object env = environment(me);
	array loc = query_temp("location", me);

	if( arg != "city" )
		return tell(me, pnoun(2, me)+"�Q�n���⤰��H\n");

	if( !env->is_maproom() || !CITY_D->query_city_info(loc[CITY], "fallen") )
		return tell(me, pnoun(2, me)+"�u�����o�󪺫����C\n");

	if( CITY_D->query_city_info(loc[CITY], "no_occupy") )
		return tell(me, "�ثe�L�k���⦹�����C\n");

	mycity = query("city", me);

	if( !mycity )
	{
		mapping info = allocate_mapping(0);
/*
		if( query("total_online_time", me) < 30*24*60*60 )
			return tell(me, pnoun(2, me)+"���W�u�ɶ��ܤ֥����W�L�@�Ӥ�~�����s�����C\n");
*/
		if( count(MONEY_D->query_all_cash(me->query_id(1), MONEY_D->query_default_money_unit()), "<", "10000000000") )
			return tell(me, pnoun(2, me)+"�������֦��ܤ� $"+MONEY_D->query_default_money_unit()+" 100 �����{���~�����s�����C\n");

		CHANNEL_D->channel_broadcast("news", me->query_idname()+"���b�Ҽ{��"+CITY_D->query_city_idname(loc[CITY])+"�إߥ��s����");
		tell(me, WARNING+NOR CYN"\n�Ĥ@�B�G"HIC"�д�"+pnoun(2, me)+"���s�������@�ӱm�⤤��W��(1 - 5 �Ӥ���r)\n"NOR CYN"�G"NOR);
		input_to((: input_color_name, me, info :));
	}
	else if( CITY_D->is_mayor(mycity, me) )
	{
		CHANNEL_D->channel_broadcast("news", CITY_D->query_city_idname(mycity)+"����"+me->query_idname()+"���b�Ҽ{����"+CITY_D->query_city_idname(loc[CITY])+"�@���ìP����");
		tell(me, WARNING+NOR CYN"\nĵ�i�G"HIC"����ìP������N�L�k�ӧO���ìP�����A�Y�g�礣���N�ɭP���"+CITY_D->query_city_idname(mycity)+HIC"���`\n�O�_�T�w���⦹�y�����@��"+CITY_D->query_city_idname(mycity)+HIC"���ìP�����H(Yes/No)"NOR CYN"�G"NOR);
		input_to((: confirm_occupy_section_city, me :));
	}
	else
		tell(me, pnoun(2, me)+"�O"+CITY_D->query_city_idname(mycity)+"�������A�Y�n�إߥ��s����"+pnoun(2, me)+"���������P�������y�A�Y�O�Q����ìP�����h�����ѥ�������C\n");
}
