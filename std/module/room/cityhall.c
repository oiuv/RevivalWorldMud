/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cityhall.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-2-24
 * Note   : ���F�j�Ӱʧ@�~�Ӫ���
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit ROOM_ACTION_MOD;

#define LAND_DOMINIUM	"/obj/etc/land_dominium"

#define UNREGISTER_MONEY		"1000000"
#define DEFAULT_NEWBIE_MONEY		"10000"

#define CITY_IDNAME_CHANGE_COST		"500000000"

// �n�O�������O
void do_register(object me, string arg)
{
	string city, money_unit;
	object env = environment(me);

	city = env->query_city();

	if( CITY_D->is_citizen(me->query_id(1), city) )
		return tell(me, pnoun(2, me)+"�w�g�O"+CITY_D->query_city_idname(city)+"�������F�C\n");

	if( CITY_D->is_citizen(me->query_id(1), query("city", me))  )
		return tell(me, pnoun(2, me)+"���������쥻�b"+CITY_D->query_city_idname(query("city", me))+"�����y�C\n");

	if( wizardp(me) && city != "wizard" )
		return tell(me, "�Ův����J�y���a�������K�y���C�������C\n");

	if( CITY_D->register_citizen(me->query_id(1), city) )
	{
		set("city", city, me);

		msg("$ME�N��n�����a��ƪ����d�x�p�j�A�V"+CITY_D->query_city_idname(city)+"���F�������n�O���������C\n",me,0,1);

		if( !sizeof(filter_array(all_inventory(me), (: base_name($1) == LAND_DOMINIUM :))) )
			new(LAND_DOMINIUM)->move(me);

		money_unit = MONEY_D->city_to_money_unit(city);

		me->earn_money(money_unit, DEFAULT_NEWBIE_MONEY);

		msg("�d�x�p�j���_���ө^�W�F�@���g�a�Ҧ��v����$ME�A�õo�� "HIY+money(money_unit, DEFAULT_NEWBIE_MONEY)+NOR" �������ϧU����$ME�C\n", me, 0, 1);

		CHANNEL_D->channel_broadcast("news", me->query_idname()+"�V"+CITY_D->query_city_idname(city)+"���F�������n�O���������C");
		CHANNEL_D->channel_broadcast("city", me->query_idname()+"�V"+CITY_D->query_city_idname(city)+"���F�������n�O���������C", me);
		me->save();
		set_temp("city_register_time", time(), me);
		return;
	}

	return tell(me, "�n�O���ѡC\n");
}

void do_confirm_unregister(object me, string city, string arg)
{
	string money_unit = MONEY_D->city_to_money_unit(city);

	if( !arg || !arg[0] )
		return tell(me, pnoun(2, me)+"�����F���P���y���ʧ@�C\n");

	arg = lower_case(arg);

	if( arg != "yes" && arg[0..0] == "y" )
	{
		tell(me, HIY"�Y"+pnoun(2, me)+"�T�w�n���P�����������ܽп�J'yes'�C\n"+pnoun(2, me)+"�T�w�n���P"+CITY_D->query_city_idname(city)+HIY"�����y�H(Yes/No)"NOR);
		input_to((: do_confirm_unregister, me, city:));
		return;
	}

	if( arg != "yes" )
	{
		tell(me, pnoun(2, me)+"�����F���P���y���ʧ@�C\n");
		me->finish_input();
		return;
	}

	if( query("total_online_time", me) > 7*24*60*60 && !me->spend_money(money_unit, UNREGISTER_MONEY) )
	{
		tell(me, pnoun(2, me)+"���P���y�ݭnú�� $"+money_unit+" "+UNREGISTER_MONEY+"�C\n");
		me->finish_input();
		return;
	}

	if( CITY_D->remove_citizen(me->query_id(1)) )
	{
		delete("city", me);

		msg("$ME�V�d�x�p�j�n�O���P�F�b"+CITY_D->query_city_idname(city)+"�����������C\n", me, 0, 1);

		if( present("land dominium", me) )
			destruct(present("land dominium", me));

		me->spend_money(money_unit, DEFAULT_NEWBIE_MONEY);

		ESTATE_D->remove_estate(me->query_id(1));
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"���P�F�b"+CITY_D->query_city_idname(city)+"�����y�C");

		me->save();
		me->finish_input();

		return;
	}
	me->finish_input();
	return tell(me, "���P���ѡC\n");
}

// ���P��������
void do_unregister(object me, string arg)
{
	string city = query("city", me);

	if( !CITY_D->city_exist(city) )
		return tell(me, pnoun(2, me)+"���ݩ����@�y�����C\n");

	if( !CITY_D->is_citizen(me->query_id(1), city) )
		return tell(me, pnoun(2, me)+"���ӴN���O"+CITY_D->query_city_idname(city)+"�������C\n");

	if( CITY_D->is_mayor(city, me) )
		return tell(me, pnoun(2, me)+"�O����������P���������C\n");

	if( time() - query_temp("city_register_time", me) < 600 )
		return tell(me, pnoun(2, me)+"�������� "+(time() - query_temp("city_register_time", me))+" ���~����P���������C\n");

	tell(me, HIR"\n\n���P���������N�|��"+pnoun(2, me)+"�ॢ�b�����������Ҧ��g�a�P�ؿv���åB�o���������v�C\n"HIY+pnoun(2, me)+"�T�w�n���P"+CITY_D->query_city_idname(city)+HIY"�����y�H(Yes/No)"NOR);

	input_to((: do_confirm_unregister, me, city:));
}

void do_citizen(object me, string arg)
{
	int i;
	string msg="";
	string city = environment(me)->query_city();

	msg = CITY_D->query_city_idname(city)+"�������W��p�U(�̷ӤJ�y����)�G\n";

	foreach( string id in CITY_D->query_citizens(city) )
		msg += sprintf("%-15s%s", capitalize(id), ++i%6 ? "" : "\n");

	return tell(me, msg+"\n\n");
}

void reset_election_roomlong(object env)
{
	int i;
	mapping election = query("election", env);
	string msg = "";

	if( election )
	{
		msg += HIW"\n�벼�D�D�G"HIC+election["title"]+NOR WHT"\n�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += HIG"�}��n�O�ѿ�G"NOR+(election["candidate"] ? "�O" : "�_")+"\n";

		if( election["end"] )
			msg += HIG"�벼�}�l�ɶ��G"NOR+(election["start"] ? election["start"]/60/60/24+" �� "+election["start"]/60/60%24+" �� "+election["start"]/60%60+" ������" : "�w�g�}�l")+"\n";

		if( election["announced"] )
			msg += HIG"�벼�����ɶ��G"NOR+(election["end"] ? (election["end"]/60/60/24)+" �� "+(election["end"]/60/60%24)+" �� "+(election["end"]/60%60)+" ������" : "�w�g����")+"\n";

		msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"HIW"�벼���ءG\n\n"NOR;

		if( election["option"] )
		{
			foreach(string option, mapping data in election["option"] )
				msg += sprintf(HIR"%2d"NOR RED" - "NOR"%-50s %d ��\n", ++i, data["candidate"]||option, sizeof(data["vote"]));
		}
		else
			msg += "    �L����ﶵ\n";

		msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		set("long", msg, env);
	}
	else
		set("long", "�ثe�S������벼���ʡC", env);
}

string query_pcolor(int percent)
{
	switch(percent)
	{
		case 0..30:
			return HIR+percent+"%"NOR;
		case 31..70:
			return HIY+percent+"%"NOR;
		case 71..100:
			return HIG+percent+"%"NOR;
		case 101..120:
			return HIC+percent+"%"NOR;
		default:
			return HIW+percent+"%"NOR;
	}
}

void heart_beat(object env)
{
	switch( query("function", env) )
	{
		case "election":
		{
			int time = time();
			mapping election = query("election", env) || allocate_mapping(0);

			if( !((time-120)%1800) )
			{
				set("city", env->query_city(), env);

				if( election["announced"] && !election["ended"] )
					CHANNEL_D->channel_broadcast("city", "���F�����b�|��u"+election["title"]+"�v���벼���ʡA�п��D�ѻP�벼�C", env);
			}

			if( !(time%1800) )
				env->save();

			if( election["announced"] )
			{
				if( !election["started"] && addn("election/start", -1, env) <= 0 )
				{
					set("election/started", 1, env); // �w�}�l
					delete("election/start", env);
					env->save();
				}

				if( !election["ended"] && addn("election/end", -1, env) <= 0 )
				{
					set("election/ended", 1, env); // �w����
					delete("election/end", env);
					env->save();
				}
				reset_election_roomlong(env);
			}
			break;
		}
	}
}

void do_refresh(object me, string arg)
{
	object env = environment(me);
	string msg, estatetax;
	string city = env->query_city();
	string moneyunit = MONEY_D->city_to_money_unit(city);
	int num = CITY_D->query_city_num(city);
	mapping fund = CITY_D->query_city_info(city, "fund") || allocate_mapping(0);
	mapping facilities = CITY_D->query_public_facility(city);
	mapping money = CITY_D->query_city_fund(city);
	string totalreceipt, totalexpense, estatecost;

	estatetax = TAX_D->query_city_tax(city);

	totalreceipt = count(estatetax, "+", totalreceipt);

	foreach( string id, mixed m in money )
		totalexpense = count( totalexpense, "+", m );

	foreach( string citizen in CITY_D->query_citizens(city) )
		estatecost = count( estatecost, "+", count(ESTATE_D->query_all_estate_value(citizen), "/", 15000));

	totalexpense = count( totalexpense, "+", estatecost);

	msg  = "�z�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w "HIG"��s�ɶ� "+TIME_D->replace_ctime(time())+NOR"  �{\n";
	msg += "�U"+sprintf("%-60s", CITY_D->query_city_idname(city)+"���]�F��T")+"�U\n";
	msg += "�u�w�w�w�w�w�w�w�w�w�w�w�w�w"HIR"���F"NOR RED"��X"NOR"�w�w�w�w�w�w�w�w�w�w�w�w�w�t\n";

	msg += "�U"HIW"�۵M�귽          �ƶq       �۵M�O�|             �һݪ��B"NOR"  �U\n";
	msg += sprintf("�U  %-16s%-12s%7s"HIR"%21s"NOR"  �U\n", NOR YEL"�s��"NOR, num+" �y��", query_pcolor(fund["�s��"]), money(moneyunit, money["�s��"]));
	msg += sprintf("�U  %-16s%-12s%7s"HIR"%21s"NOR"  �U\n", NOR HIG"�˪L"NOR, num+" �y��", query_pcolor(fund["�˪L"]), money(moneyunit, money["�˪L"]));
	msg += sprintf("�U  %-16s%-12s%7s"HIR"%21s"NOR"  �U\n", NOR HIB"�e�y"NOR, num+" �y��", query_pcolor(fund["�e�y"]), money(moneyunit, money["�e�y"]));
	msg += "�U                                                            �U\n";
	msg += "�U"HIW"���@��X          �ƶq       ���@�O�i             �һݪ��B"NOR"  �U\n";
	msg += sprintf("�U  %-16s%-12d%7s"HIR"%21s"NOR"  �U\n", NOR WHT"�D��"NOR, facilities["�D��"], query_pcolor(fund["�D��"]), money(moneyunit, money["�D��"]));
	msg += sprintf("�U  %-16s%-12d%7s"HIR"%21s"NOR"  �U\n", NOR RED"����"NOR, facilities["����"], query_pcolor(fund["����"]), money(moneyunit, money["����"]));
	msg += sprintf("�U  %-16s%-12d%7s"HIR"%21s"NOR"  �U\n", HIY"��a��v"NOR, facilities["�Ŧa"], "- ", money(moneyunit, money["�Ŧa"]));
	msg += "�U                                                            �U\n";
	msg += "�U"HIW"��B��X          �ƶq       ��B�޲z             �һݪ��B"NOR"  �U\n";
	msg += sprintf("�U  %-16s%-12d%7s"HIR"%21s"NOR"  �U\n", HIM"�F���ؿv��"NOR, 	facilities["���@�ؿv��"], "- ", money(moneyunit, money["���@�ؿv��"]));
	msg += sprintf("�U  %-16s%-12s%7s"HIR"%21s"NOR"  �U\n", HIC"�����a���޲z"NOR, 	"- ", "- ", money(moneyunit, estatecost));
	msg += "�U                                                            �U\n";
	msg += sprintf("�U  "HIR"�`��X     %45s"NOR"  �U\n", money(moneyunit, totalexpense));
	msg += "�u�w�w�w�w�w�w�w�w�w�w�w�w�w"HIG"���F"NOR GRN"���J"NOR"�w�w�w�w�w�w�w�w�w�w�w�w�w�t\n";
	msg += "�U"HIW"�|�����J                         �|�v             �x���|�B"NOR"  �U\n";
	msg += sprintf("�U  %-28s%7d"HIG"%21s"NOR"  �U\n", "�a���|", CITY_D->query_city_info(city, "estatetax"), money(moneyunit, estatetax));
	msg += "�U                                                            �U\n";
	msg += sprintf("�U  "HIG"�`���J     %45s"NOR"  �U\n", money(moneyunit, totalreceipt));
	msg += "�u�w�w�w�w�w�w�w�w�w�w�w�w�w"HIY"����"NOR YEL"�έp"NOR"�w�w�w�w�w�w�w�w�w�w�w�w�w�t\n";
	msg += sprintf("�U  �W��]�F���� "HIY"%43s"NOR"  �U\n", money(moneyunit, CITY_D->query_city_info(city, "last_total_gain")));
	msg += sprintf("�U  ����w�p���� "HIY"%43s"NOR"  �U\n", money(moneyunit, count(totalreceipt, "-", totalexpense)));
	msg += "�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}\n";
	msg += NOR YEL" �п�J "HIY"refresh"NOR YEL" ���O�ӧ�s��̷s���]�F�έp�Ʀr�C\n"NOR;

	set("long", msg, env);
	tell(me, "��s�]�F��T�����C\n");

	env->save();
}

void do_election_manage(object me, string arg)
{
	mixed option;
	object env = environment(me);
	string city = env->query_city();
	mapping election = query("election", env);

	if( !arg )
		return tell(me, "�аѦ� help manage ���O�����C\n");

	if( !CITY_D->is_mayor_or_officer(city, me) )
		return tell(me, pnoun(2, me)+"���O�o�y�����������Ωx���A�L�k�޲z�벼�c�C\n");

	if( sscanf(arg, "new %s", option) )
	{
		if( noansi_strlen(option) > 30 )
			return tell(me, "�D�D�W�٤���W�L 30 �Ӧr���C\n");

		if( sizeof(election) )
			return tell(me, "�ثe�w�g���t�@�ӧ벼�D�D�b�B�@���C\n");

		option = kill_repeat_ansi(option+NOR);
		set("election/title", option, env);
		tell(me, pnoun(2, me)+"�]�w�벼�D�D���u"+option+"�v�C\n");
	}
	else if( arg == "delete" )
	{
		if( !election )
			return tell(me, "�ثe�S������벼���ʡC\n");

		if( election["announced"] )
			CHANNEL_D->channel_broadcast("city", NOR YEL+me->query_idname()+NOR YEL"�������F���벼���ʡu"HIY+election["title"]+NOR YEL"�v�C"NOR, me);

		tell(me, "�R���Ҧ��벼��ơC\n");
		delete("election", env);
	}
	else if( arg == "candidate" )
	{
		if( !election["title"] )
			return tell(me, "�Х��]�w�벼�D�D�C\n");

		if( election["announced"] )
			return tell(me, "�벼���ʤw�g���i�A�L�k�A�ק�]�w�C\n");

		if( election["candidate"] )
		{
			tell(me, "�����\�Կ�H�n�O�ѿ�C\n");
			delete("election/candidate", env);
		}
		else
		{
			tell(me, "�}��Կ�H�n�O�ѿ�C\n");
			set("election/candidate", 1, env);
		}
	}
	else if( sscanf(arg, "option %s", option) )
	{
		if( !election["title"] )
			return tell(me, "�Х��]�w�벼�D�D�C\n");

		if( election["announced"] )
			return tell(me, "�벼���ʤw�g���i�A�L�k�A�ק�]�w�C\n");

		if( query("election/option/"+option, env) )
		{
			tell(me, "�R���벼���ءu"+option+"�v�C\n");
			delete("election/option/"+option, env);
		}
		else
		{
			tell(me, "�W�[�벼���ءu"+option+"�v�C\n");
			set("election/option/"+option+"/candidate", 0, env);
		}
	}
	else if( sscanf(arg, "start %d", option) )
	{
		if( !election["title"] )
			return tell(me, "�Х��]�w�벼�D�D�C\n");

		if( election["announced"] )
			return tell(me, "�벼���ʤw�g���i�A�L�k�A�ק�]�w�C\n");

		if( option < 0 || option > 240 )
			return tell(me, "�벼�}�l�ɶ��ûݦb 0 �� 240 �p�ɤ����C\n");

		if( election["end"] && option*3600 >= election["end"] )
			return tell(me, "�벼�}�l�ɶ������n�b�벼�����ɶ����e�C\n");

		set("election/start", option*3600, env);
		tell(me, pnoun(2, me)+"�]�w�b���G�벼���ʪ� "+option+" �Ӥp�ɫ�}�l�벼�C\n");
	}
	else if( sscanf(arg, "end %d", option) )
	{
		if( !election["title"] )
			return tell(me, "�Х��]�w�벼�D�D�C\n");

		if( election["announced"] )
			return tell(me, "�벼���ʤw�g���i�A�L�k�A�ק�]�w�C\n");

		if( option < 0 || option > 480 )
			return tell(me, "�벼�}�l�ɶ��ûݦb 0 �� 480 �p�ɤ����C\n");

		if( option*3600 <= election["start"] )
			return tell(me, "�벼�����ɶ������n�b�벼�}�l�ɶ�����C\n");

		set("election/end", option*3600, env);
		tell(me, pnoun(2, me)+"�]�w�b���G�벼���ʪ� "+option+" �Ӥp�ɫᵲ���벼�C\n");

	}
	else if( arg == "announce" )
	{
		if( !election["title"] )
			return tell(me, "�L�벼�D�D�C\n");
		if( !election["end"] )
			return tell(me, "���]�벼�����ɶ��C\n");
		if( election["announced"] )
			return tell(me, "�벼���ʤw�g���i�C\n");

		if( !election["start"] )
			set("election/started", 1, env);

		set("election/announced", 1, env);

		heart_beat(env);

		CHANNEL_D->channel_broadcast("city", NOR YEL+me->query_idname()+NOR YEL"�b���F���|�쪺�벼���ʡu"HIY+election["title"]+NOR YEL"�v�����}�l�C"NOR, me);
	}
	else tell(me, "�аѦ� help manage ���O�����C\n");

	reset_election_roomlong(env);
	env->save();
}

void do_election_register(object me, string arg)
{
	object env = environment(me);
	string city = env->query_city();
	string myid = me->query_id(1);
	mapping election = query("election", env);

	if( !CITY_D->is_citizen(myid, city) )
		return tell(me, pnoun(2, me)+"���O"+CITY_D->query_city_idname(city)+"�������C\n");

	if( !election )
		return tell(me, "�ثe�S���벼���ʡC\n");

	if( query("total_online_time", me) < 30*24*60*60 )
		return tell(me, pnoun(2, me)+"���`�W�u�ɶ��|���W�L 30 �ѡA�L�k�n�O���Կ�H�C\n");

	if( !election["candidate"] )
		return tell(me, "�����벼���}��Կ�H�n�O�C\n");

	if( query("election/option/"+myid, env) )
	{
		delete("election/option/"+myid, env);
		reset_election_roomlong(env);
		CHANNEL_D->channel_broadcast("city", NOR YEL+me->query_idname()+NOR YEL"���ѿ�u"HIY+election["title"]+NOR YEL"�v�C"NOR, me);
		return;
	}

	if( election["ended"] )
		return tell(me, "�벼�w�g�����C\n");

	if( election["started"] )
		return tell(me, "�w�g�}�l�벼�A�L�k�A�i��n�O�C\n");

	set("election/option/"+myid+"/candidate", me->query_idname(), env);

	reset_election_roomlong(env);
	CHANNEL_D->channel_broadcast("city", NOR YEL+me->query_idname()+NOR YEL"�n�O�ѿ�u"HIY+election["title"]+NOR YEL"�v�벼���ʡC"NOR, me);

	env->save();
}

void do_election_vote(object me, string arg)
{
	int i;
	object env = environment(me);
	string city = env->query_city();
	string myid = me->query_id(1);
	mapping election = query("election", env);

	if( !arg || !to_int(arg) )
		return tell(me, "�п�J"+pnoun(2, me)+"�Q�n�벼���ﶵ�s���C\n");

	if( !CITY_D->is_citizen(myid, city) )
		return tell(me, pnoun(2, me)+"���O"+CITY_D->query_city_idname(city)+"�������C\n");

	if( !election )
		return tell(me, "�ثe�S���벼���ʡC\n");

	if( query("total_online_time", me) < 20*24*60*60 )
		return tell(me, pnoun(2, me)+"���`�W�u�ɶ��|���W�L 20 �ѡA�L�k�벼�C\n");

	if( election["ended"] )
		return tell(me, "�벼�w�g�����C\n");

	if( !election["started"] )
		return tell(me, "�٥��}�l�벼�C\n");

	if( !election["option"] )
		return tell(me, "�ثe�S������벼�ﶵ�C\n");

	foreach(string option, mapping data in election["option"] )
		if( query("election/option/"+option+"/vote/"+myid, env) )
			return tell(me, pnoun(2, me)+"�w�g��L���F�C\n");

	foreach(string option, mapping data in election["option"] )
		if( ++i == to_int(arg) )
			set("election/option/"+option+"/vote/"+myid, 1, env);

	reset_election_roomlong(env);
	CHANNEL_D->channel_broadcast("city", me->query_idname()+"��U�u"+election["title"]+"�v���t���@���C", me);
	env->save();
}

void do_officer(object me, string arg)
{
	int num;
	int maxofficers;
	string id, title, city = environment(me)->query_city();
	mapping officers;
	object officer;

	officers = CITY_D->query_city_info(city, "government/officer") || allocate_mapping(0);

	if( CITY_D->is_officer(city, me) && arg == "quit" )
	{
		title = officers[me->query_id(1)];
		map_delete(officers, me->query_id(1));
		CITY_D->set_city_info(city, "government/officer", officers);

		CHANNEL_D->channel_broadcast("city", me->query_idname()+"�V���F����h�F�u"+title+"�v���x��¾�ȡC", me);
		msg("$ME�V���F����h�F�u"+title+"�v���x��¾�ȡC\n", me, 0, 1);

		for(int i=0;CITY_D->city_exist(city, i);i++)
		{
			if( CITY_D->query_section_info(city, i, "vicemayor") == me->query_id(1) )
				CITY_D->delete_section_info(city, i, "vicemayor");
		}

		return;
	}

	if( !CITY_D->is_mayor(city, me) )
		return tell(me, pnoun(2, me)+"���O��������]�w"+pnoun(2, me)+"���x���C\n");

	if( !arg )
		return tell(me, "�ثe�x���W�欰�G"+(sizeof(officers) ? implode(keys(officers), (: capitalize($1)+", "+capitalize($2) :)) : "�L")+"\n");

	if( sscanf(arg, "%s vicemayor %d", id, num) != 2 && sscanf(arg, "%s %s", id, title) != 2 )
		return tell(me, "���O�榡���~�A�� help officer�C\n");

	if( num > 0 )
	{
		num--;

		if( !CITY_D->city_exist(city, num) )
			return tell(me, "�o�ӫ����S���� "+(num+1)+" �ìP�����C\n");

		if( !objectp(officer = find_player(id)) )
			return tell(me, "�u�W�S�� "+capitalize(id)+" �o�쪱�a�C\n");

		if( !CITY_D->is_citizen(id, city) )
			return tell(me, officer->query_idname()+"�ä��O�������������A�L�k���R���ƥ����C\n");

		maxofficers = 2+3*CITY_D->query_sections(city);
		if( undefinedp(officers[id]) && sizeof(officers) >= maxofficers )
			return tell(me, "�ثe�̦h�u��P�ɥ��R "+maxofficers+" ��x���C\n");

		if( !undefinedp(officers[id]) )
			return tell(me, pnoun(2, me)+"�������Ѱ���쥻���x��¾��C\n");

		if( CITY_D->query_section_info(city, num, "vicemayor") )
			return tell(me, pnoun(2, me)+"�������Ѱ� "+capitalize(CITY_D->query_section_info(city, num, "vicemayor"))+" �ƥ�����¾��C\n");

		officers[id] = CITY_D->query_section_info(city, num, "name")+"�ƥ���";

		CITY_D->set_city_info(city, "government/officer", officers);

		CITY_D->set_section_info(city, num, "vicemayor", id);

		CHANNEL_D->channel_broadcast("city", "����"+me->query_idname()+"���R"+officer->query_idname()+"����"+CITY_D->query_city_name(city, num)+"���ƥ����C", me);

		return;
	}

	if( title == "-d" )
	{
		officer = find_player(id);

		if( undefinedp(officers[id]) )
			return tell(me, (objectp(officer) ? officer->query_idname() : capitalize(id)+" ")+"�쥻�N���O�������x���C\n");

		for(int i=0;CITY_D->city_exist(city, i);i++)
		{
			if( CITY_D->query_section_info(city, i, "vicemayor") == id )
				CITY_D->delete_section_info(city, i, "vicemayor");
		}

		map_delete(officers, id);
		CITY_D->set_city_info(city, "government/officer", officers);
		CHANNEL_D->channel_broadcast("city", "����"+me->query_idname()+"�Ѱ��F"+(objectp(officer) ? officer->query_idname() : " "+capitalize(id)+" ")+"���x��¾�ȡC", me);
		return;
	}

	title = kill_repeat_ansi(title+NOR);

	if( !objectp(officer = find_player(id)) )
		return tell(me, "�u�W�S�� "+capitalize(id)+" �o�쪱�a�C\n");

	if( !CITY_D->is_citizen(id, city) )
		return tell(me, officer->query_idname()+"�ä��O�������������A�L�k���R���x���C\n");

	maxofficers = 2+3*CITY_D->query_sections(city);
	if( undefinedp(officers[arg]) && sizeof(officers) >= maxofficers )
		return tell(me, "�ثe�̦h�u��P�ɥ��R "+maxofficers+" ��x���C\n");

	if( title == officers[id] )
		return tell(me, officer->query_idname()+"�w�g�Q���R���u"+title+"�v�C\n");

	officers[id] = title;
	CITY_D->set_city_info(city, "government/officer", officers);
	CHANNEL_D->channel_broadcast("city", "����"+me->query_idname()+"���R"+officer->query_idname()+"����"+CITY_D->query_city_name(city)+"���u"+title+"�v�x���C", me);
}

void do_confirm_abdicate(object me, object target, string city, string arg)
{
	if( lower_case(arg) != "yes" )
	{
		tell(me, pnoun(2, me)+"��������¾�쪺�汵����C\n");
		me->finish_input();
		return;
	}

	CHANNEL_D->channel_broadcast("city", me->query_idname()+"�����N����¾��汵��"+target->query_idname()+"�A"+target->query_idname()+"�������������s�����C", me);
	CHANNEL_D->channel_broadcast("city", "�ѩ󥫪��汵�A�¦������x�����������A�s���F�ζ��N�ѷs�����i�歫�աC", me);
	CHANNEL_D->channel_broadcast("news", CITY_D->query_city_idname(city)+"������"+me->query_idname()+"�����N����¾��汵��"+target->query_idname()+"�A"+target->query_idname()+"����"+CITY_D->query_city_idname(city)+"���s�����C");

	msg(me, "$ME�N�����Lâ�ˤ�浹�F$YOU�A�é��F$YOU���ӻH���D�G�u�o�y�����N�浹"+pnoun(2, target)+"�F�A���n���ڥ���v�C\n", me, target, 1);

	CITY_D->set_city_info(city, "government/mayor", target->query_id(1));
	CITY_D->set_city_info(city, "government/officer", allocate_mapping(0));
}

void do_abdicate(object me, string arg)
{
	object targetob;
	string city = environment(me)->query_city();

	if( !CITY_D->is_mayor(city, me) )
		return tell(me, pnoun(2, me)+"���O���������C\n");

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�N�ä[������¾�������֡H\n");

	if( !objectp(targetob = find_player(arg)) )
		return tell(me, "�u�W�èS�� "+arg+" �o�쪱�a�C\n");

	if( !CITY_D->is_citizen(arg, city) )
		return tell(me, targetob->query_idname()+"�ä��O�������������A�L�k����������ä[�����C\n");

	if( !same_environment(me, targetob) )
		return tell(me, targetob->query_idname()+"���b"+pnoun(2, me)+"�����ǡA�L�k�汵�ä[�����Lâ�C\n");

	tell(me, HIY"�T�w�N�ä[����¾��浹"+targetob->query_idname()+HIY"�ܡH(Yes/No)�G"NOR);

	input_to((: do_confirm_abdicate, me, targetob, city:));
}

void do_denominate(object me, string arg)
{
	int num;
	string msg;
	string city = environment(me)->query_city();
	string moneyunit = environment(me)->query_money_unit();

	if( !arg )
	{
		msg = CITY_D->query_city_idname(city)+"�U�ìP�����W�٦p�U�G\n";

		while(CITY_D->city_exist(city, num))
		{
			msg += sprintf("%-4s%s\n", (num+1)+".", CITY_D->query_section_info(city, num, "name")||"�� "+(num+1)+" �ìP����");
			num++;
		}

		return tell(me, msg+"\n");
	}

	if( !CITY_D->is_mayor(city, me) )
		return tell(me, pnoun(2, me)+"���O�����L�k�ϥΦ��\��C\n");

	if( sscanf(arg, "id %s", arg) == 1 )
	{
		if( lower_case(remove_ansi(arg)) != city )
			return tell(me, "�^��r�������P�{���������^��r���ۦP�A"+pnoun(2, me)+"�u����j�p�g�P��m�C\n");

		if( !CITY_D->change_assets(city, "-"+CITY_IDNAME_CHANGE_COST) )
			return tell(me, "���F�����겣���� "HIY+money(moneyunit, CITY_IDNAME_CHANGE_COST)+NOR" �L�k�ܧ�^��W�١C\n");

		arg = remove_fringe_blanks(kill_repeat_ansi(arg+NOR));

		CHANNEL_D->channel_broadcast("news", CITY_D->query_city_idname(city)+"����"+me->query_idname()+"�o�G�����G�ۤ���_���������^��W�٧�W���u"+arg+"�v�C\n");
		tell(me, pnoun(2, me)+"��O�����겣 "HIY+money(moneyunit, CITY_IDNAME_CHANGE_COST)+NOR" �ܧ󫰥����^��W�١C\n");

		CITY_D->set_city_info(city, "color_id", arg);

		return;
	}

	if( sscanf(arg, "name %s", arg) == 1 )
	{
		int strlen = noansi_strlen(arg);

		if( strlen < 2 || strlen > 10 )
			return tell(me, "����������W�٦r�ƥ������� 1 - 5 �Ӥ���r�C\n");

		if( !is_chinese(arg) )
			return tell(me, "����������W�٥u��ϥΤ���C\n");

		if( !CITY_D->change_assets(city, "-"+CITY_IDNAME_CHANGE_COST) )
			return tell(me, "���F�����겣���� "HIY+money(moneyunit, CITY_IDNAME_CHANGE_COST)+NOR" �L�k�ܧ󤤤�W�١C\n");

		arg = remove_fringe_blanks(kill_repeat_ansi(arg+NOR));

		CHANNEL_D->channel_broadcast("news", CITY_D->query_city_idname(city)+"����"+me->query_idname()+"�o�G�����G�ۤ���_������������W�٧�W���u"+arg+"�v�C\n");
		tell(me, pnoun(2, me)+"��O�����겣 "HIY+money(moneyunit, CITY_IDNAME_CHANGE_COST)+NOR" �ܧ󫰥�������W�١C\n");

		CITY_D->set_city_info(city, "name", arg);

		return;
	}

	if( sscanf(arg, "-symbol %d %s", num, arg) == 2 )
	{
		int strlen;
		string *emblem = CITY_D->query_city_info(city, "emblem");

		if( num < 1 || num > 3 )
			return tell(me, "�u��w�� 1 �� 3 �檺���O�@�ܧ�C\n");

		arg = kill_repeat_ansi(replace_string(arg, "\n", "")+NOR);

		strlen = noansi_strlen(arg);

		if( strlen > 10 )
			return tell(me, "�C�����O�̦h 10 �Ӧr���C\n");

		if( strlen < 10 )
		{
			switch(strlen)
			{
				case 1:arg = "    "+arg+"     ";break;
				case 2:arg = "    "+arg+"    ";break;
				case 3:arg = "   "+arg+"    ";break;
				case 4:arg = "   "+arg+"   ";break;
				case 5:arg = "  "+arg+"   ";break;
				case 6:arg = "  "+arg+"  ";break;
				case 7:arg = " "+arg+"  ";break;
				case 8:arg = " "+arg+" ";break;
				case 9:arg = arg+" ";break;
			}

		}
		emblem[num-1] = arg;

		CITY_D->set_city_info(city, "emblem", emblem);

		msg("$ME�﫰�����O�@�F�u"+arg+"�v�������ܧ�C\n", me, 0, 1);
		return;
	}



	if( sscanf(arg, "%d %s", num, arg) != 2 )
		return tell(me, "�аѦ� help denominate ���T���O�榡�C\n");

	if( !CITY_D->city_exist(city, num-1) )
		return tell(me, "�������èS�� "+num+" �����ìP�����C\n");

	arg = kill_repeat_ansi(replace_string(arg, "\n", "")+NOR);

	if( noansi_strlen(arg) > 10 )
		return tell(me, "�ìP�����W�٪��׳̦h 10 �Ӧr���C\n");

	CITY_D->set_section_info(city, num-1, "name", arg);
	CHANNEL_D->channel_broadcast("city", me->query_idname()+"�N"+CITY_D->query_city_idname(city)+"�� "+num+" �ìP�������W�ٳ]�w���u"+arg+"�v�C", me);
	return tell(me, pnoun(2, me)+"�N"+CITY_D->query_city_idname(city)+"�� "+num+" �ìP�������W�ٳ]�w���u"+arg+"�v�C\n");
}

void do_donate(object me, string arg)
{
	string myid = me->query_id(1);
	object env = environment(me);
	string city = env->query_city();
	string moneyunit = MONEY_D->city_to_money_unit(city);

	if( !CITY_D->is_citizen(myid, city) )
		return tell(me, pnoun(2, me)+"���O�����������L�k���m�C\n");

	if( !arg )
	{
		string msg;
		mapping donate = query("donate", env);

		if( !sizeof(donate) )
			return tell(me, "�ثe�|�L���󮽴ڡC\n");

		msg = "���F�g�O���m�W��p�U�G\n";
		foreach(string id, mapping data in donate)
			msg += sprintf("%-40s%s\n", data["idname"], "$"+moneyunit+" "+NUMBER_D->number_symbol(data["money"]));

		return me->more(msg+"\n");
	}

	else if(!(arg = big_number_check(arg)))
		return tell(me, "�п�J���T�����ڪ��B�C\n");

	if( count(arg, "<", 10000) )
		return tell(me, "�̤֮��ڪ��B�� $"+moneyunit+" 10,000 ���C\n");

	if( !me->spend_money(moneyunit, arg) )
		return tell(me, pnoun(2, me)+"���W�S���o��h�� $"+moneyunit+" �{���C\n");

	set("donate/"+myid, (["idname" : me->query_idname(), "money" : count(arg, "+", query("donate/"+myid+"/money", env)) ]), env);

	env->save();
	CITY_D->change_assets(city, arg);
	CHANNEL_D->channel_broadcast("city", me->query_idname()+"���m�F $"+moneyunit+" "+NUMBER_D->number_symbol(arg)+" �@�����F�g�O���ΡC", me);

	return tell(me, pnoun(2, me)+"���m�F $"+moneyunit+" "+NUMBER_D->number_symbol(arg)+" �@�����F�g�O���ΡC\n");
}

void do_fire(object me, string arg)
{
	object env = environment(me);
	string city = env->query_city();

	if( !CITY_D->is_mayor(city, me) )
		return tell(me, pnoun(2, me)+"���O�����L�k�ϥΦ��\��C\n");

	if( !arg )
		return tell(me, pnoun(2, me)+"�n�}�����@�ӥ��������y�H�C\n");

	arg = lower_case(arg);

	if( !CITY_D->is_citizen(arg, city) )
		return tell(me, capitalize(arg)+" �ä��O�������������C\n");

	if( sizeof(ESTATE_D->query_player_estate(arg)) )
		return tell(me, capitalize(arg)+" �b�o�������٦��Цa���A����}���䥫�y�C\n");

	if( CITY_D->remove_citizen(arg) )
	{
		object player = find_player(arg) || load_user(arg);

		if( !objectp(player) )
			CHANNEL_D->channel_broadcast("news", me->query_idname()+"�}���F "+capitalize(arg)+" �b"+CITY_D->query_city_idname(city)+"�����������C");
		else
		{
			delete("city", player);
			CHANNEL_D->channel_broadcast("news", me->query_idname()+"�}���F"+player->query_idname()+"�b"+CITY_D->query_city_idname(city)+"�����������C");
			player->save();

			if( !interactive(player) )
				destruct(player);
		}
	}
	else
		error("���P���ѡA�гq���Ův�B�z�C\n");
}

void do_fund(object me, string arg)
{
	int percent;
	int taxrate;
	object env = environment(me);
	string tax, city = env->query_city();
	mapping fund = CITY_D->query_city_info(city, "fund") || allocate_mapping(0);

	if( !arg )
		return me->more(query("long", env));

	if( !CITY_D->is_mayor(city, me) )
		return tell(me, pnoun(2, me)+"���O�����L�k�ϥΦ��\��C\n");

	if( sscanf(arg, "%s�| %d", tax, taxrate) == 2 )
	{
		if( taxrate < 0 || taxrate > 10000 )
			return tell(me, "�|�v���i�C�� 0 �ΰ��� 10000�C\n");

		switch(tax)
		{
			case "�a��":
				CITY_D->set_city_info(city, "estatetax", taxrate);
				break;
			default:
				return tell(me, "�ثe�õL"+tax+"�|�C\n");
		}

		CHANNEL_D->channel_broadcast("city", me->query_idname()+"�N"+CITY_D->query_city_idname(city)+"��"+tax+"�|�վ㬰 "+taxrate+"�C\n", me);
		return tell(me, pnoun(2, me)+"�N"+CITY_D->query_city_idname(city)+"��"+tax+"�|�վ㬰 "+taxrate+"�C\n", me);
	}

	if( sscanf(arg, "%s %d", arg, percent) == 2 )
	{
		if( percent < 0 || percent > 120 )
			return tell(me, "�g�O���i�C�� 0% �ΰ��� 120%�C\n");

		switch(arg)
		{
			case "�s��":
			case "�˪L":
			case "�e�y":
			case "�D��":
			case "����":
				fund[arg] = percent;
				break;
			default:
				return tell(me, "�п�J���T���I�F�ﶵ�C\n");
		}

		CITY_D->set_city_info(city, "fund", fund);

		CHANNEL_D->channel_broadcast("city", me->query_idname()+"�N�u"+arg+"�v�������]�F�g�O�վ㬰 "+query_pcolor(percent)+"�C", me);

		return tell(me, pnoun(2, me)+"�N�u"+arg+"�v�������]�F�g�O�վ㬰 "+query_pcolor(percent)+"�C\n", me);
	}
	return tell(me, "�аѦ� help fund ��J���T�榡�C\n");
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"lobby"	:
	([
		"short"	: HIC"�j�U"NOR,
		"help"	:
			([
"topics":
@HELP
    ���F�j�U�]�A�U�إ�����y�\��C
HELP,

"donate":
@HELP
���m�����@�����F������ϥΡA�Ϊk�p�U�G
  donate '����'		- ���m�����@�����F������ϥ�
  donate		- ���m�W��
HELP,
			]),
		"action":
			([
				"donate"	: (: do_donate :),
			]),
	]),

	"register" :
	([
		"short"	: HIY"��F�ưȩ�"NOR,
		"help"	:
			([
"topics":
@HELP
    ��F�ưȩҭt�d�U�����y�P���~�޲z�~�ȡC
HELP,

"register":
@HELP
�n�O���U�������������A�Ϊk�p�U�G
  register		- �n�O���U������������
HELP,

"unregister":
@HELP
���P���y�A�Ϊk�p�U�G
  register		- ���P���y
HELP,

			]),
		"action":
			([
				"register"	: (: do_register :),
				"unregister"	: (: do_unregister :),
				"citizen"	: (: do_citizen :),
			]),
	]),

	"science"	:
	([
		"short"	: HIR"��޵o�i��"NOR,
		"help"	: "�L�������C",
		"action":
			([

			]),
	]),

	"election"	:
	([
		"short"	: HIG"���|�e���|"NOR,
		"help"	:
			([
"topics":
@HELP
    ���|�e���|�t�d�U���벼�����~�ȡC
HELP,

"register":
@HELP
�n�O�Ψ����ѿ�A�Ϊk�p�U�G
  register		- �n�O�ѿ糧�����|(�A���@������)
HELP,

"vote":
@HELP
��U���t���@���A�Ϊk�p�U�G
  vote '����'		- ��U���t���@��
HELP,

"manage":
@HELP
���|�޲z���O�A�Ϊk�p�U�G
  manage new '�D�D'		- �s�W�벼�D�D
  manage delete 		- �R�������벼���
  manage candidate		- �]�w�����|�O�_�}��Կ�H�n�O(���۳]�벼����)
  manage option '�ﶵ'		- �۳]�벼����(���}��Կ�H�n�O)�A�A�]�w�@���h�R��
  manage start '�p��'		- �]�w�X�p�ɫ�벼�}�l�A�Y�]�w�� 0 �Y�ߨ�}�l�벼(�}��Կ�H�n�O�ɤ��i�]�� 0)
  manage end '�p��'		- �]�w�X�p�ɫ�벼����
  manage announce		- �o�G�벼�T���ö}�l�Ұʧ벼����
HELP,
			]),
		"heartbeat":1,
		"action":
			([
				"manage"	: (: do_election_manage :),
				"register"	: (: do_election_register :),
				"vote"		: (: do_election_vote :),
			]),
	]),

	"finance"	:
	([
		"short"	: HIM"�]�F����"NOR,
		"help"	:
			([
"topics":
@HELP
    �]�F�����t�d�����U���]�F�~�ȡC
HELP,

"fund":
@HELP
�]�w�]�F�g�O�A�Ϊk�p�U�G
  fund			- �d�Ұ]�F���
  fund �e�y 80		- �]�w�e�y���ҫO�|�g�O�� 80%
  fund �a���| 20	- �N�a���|�վ㬰 20
HELP,
			]),
		"heartbeat":0,
		"action":
			([
				"fund"		: (: do_fund :),
				"refresh"	: (: do_refresh :),
			]),
	]),

	"mayor"	:
	([
		"short"	: HIW"�����줽��"NOR,
		"help"	:
			([
"topics":
@HELP
    �����줽�ǡC
HELP,

"officer":
@HELP
���R�θѰ��x��¾��A�Ϊk�p�U�G
  officer			- �˵��ثe�x�����
  officer 'ID' vicemayor 1	- ���R�Y���������Ĥ@��ó����ƥ���
  officer 'ID' '¾��'		- ���R�Y���������x���A¾�٥ѥ����]�p
  officer 'ID' -d		- �Ѱ��Y��x����¾��
  officer quit			- �x������¾��
HELP,

"abdicate":
@HELP
����¾��汵���O�A�Ϊk�p�U�G
  abdicate 'ID'		- �N����¾��汵���t�@�쥫��
HELP,

"denominate":
@HELP
�����R�W���O�A�Ϊk�p�U�G
  denominate -symbol 1 '���O'	- �ܧ󫰥����O�� 1 ��
  denominate -symbol 2 '���O'	- �ܧ󫰥����O�� 2 ��
  denominate -symbol 3 '���O'	- �ܧ󫰥����O�� 3 ��
  denominate '�s��' '�W��'	- �]�w�Y�s���ìP�������U�O�W��
  denominate id '�^��'		- �]�w�s�������^��W�� (�u���ܧ��m�P�j�p�g)(�ݪ�O 5 ���������겣)
  denominate name '����'	- �]�w�s����������W�� (�ݪ�O 5 ���������겣)
HELP,

"fire":
@HELP
�}���Y�쥫�������O�A�Ϊk�p�U�G
fire '����ID'			- �N�Y��S���Цa���������}��
HELP,
			]),
		"action":
			([
				"officer"	: (: do_officer :),
				"abdicate"	: (: do_abdicate :),
				"denominate"	: (: do_denominate :),
				"fire"		: (: do_fire :),
			]),
	]),
]);


// �]�w�ؿv�����
nosave array building_info = ({

	// �ؿv��������W��
	HIW"���F"NOR WHT"�j��"NOR

	// �}�i���ؿv�����̤֩ж�����
	,6

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,1

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION

	// �}�i�����O��
	,"0"

	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,50

	// �̰��i�[�\�Ӽh
	,1

	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0

	// �ؿv���ɥN
	,1
});
