/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : lotto_machine.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-1-21
 * Note   : �ֳz�mñ�`��
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#define DATA_PATH		"/data/object/lotto.o"

#include <ansi.h>
#include <daemon.h>
#include <inherit.h>
#include <secure.h>

#define MAX_LOTTO_AMOUNT                10000
#define MAX_SINGLE_TIME_LOTTO		10000

#define MAX_LOTTO_OPEN_PER_ROUND	2500
#define MAX_LOTTO_PER_ROUND		2500

#define MONEY_PER_LOTTO			500
#define RECYCLE_PERCENT			80	// RECYCLE_PERCENT% �@������, �ѤU�����t�Φ^��

#define FIFTH_REWARD			2000	// �� 5 ������
inherit STANDARD_OBJECT;

private nosave mapping table;
private mapping lotto_info;
private nosave mapping add_number_schedule;

void do_lotto(object me, string arg);

nosave string help = @HELP
    �C�` 500 ���A�C�Q�����۰ʶ}���@���A�}����Ҧ��Ѿl�����L������
�ֿn�ܤU���Y���A���z���B���j���I

lotto				�C�X�A�ثe����`���
lotto 1,2,3,4,5,6		�@���ռƦr 1 �� 42, �Ш̷ӳr�I�榡��J
lotto N				�۰ʹq���︹ N �`
lotto info 			��ܥثe�U�a�U�`���

lotto open			�ߧY�}��(�Ův�v��)

�Y���G�`�m�� 38% + �W���Ѿl���Ҧ�����  (��������)
�G���G�`�m�� 12%                       (���������䤤�@�����S�O��)
�T���G�`�m�� 15%                       (���P)
�|���G�`�m�� 35%                       (�|�P)
�����G�@�� 2000 ��, �� 2000 ���B�~�o�� (�T�P)

�������B�̷Ӥ����H�ƥ������t

���骩���Gv2.4
HELP;

nosave mapping actions = 	
([
    "lotto"			:(: do_lotto :),
]);

// �H�N�D�� 6 �ռƦr
int *random_number()
{
	int *num = allocate(6), i, j, k;

	reset_eval_cost();

	for(j=i=0;j<6;j=++i)
	{
		k = random(42)+1;
		while( j-- && k!=num[j] );

		if( j != -1 )
		{
			i--; continue;
		}
		else num[i] = k;
	}
	return num;
}

void lotto_open_finish(int *open_num, int open_spec_num, mapping prize, int *total_prize, int total_lotto)
{
	string id;
	mapping map;
	string msg, *money = allocate(5);
	object ppl;

	// �Y���m���������m�� 38% + �W���Ѿl�Ҧ��m��

	money[0] = count(count(count(lotto_info["total_prize"],"*",38),"/",100), "+", lotto_info["first_prize"]);
	money[1] = count(count(lotto_info["total_prize"],"*",12),"/",100);
	money[2] = count(count(lotto_info["total_prize"],"*",15),"/",100);
	money[3] = count(count(lotto_info["total_prize"],"*",35),"/",100);
	money[4] = FIFTH_REWARD+"";

	msg =  HIY"\n�� "+lotto_info["time"]+" ���ֳz�m���C���m�y�w�g�����}�X�F�I�I\n"NOR;
	msg += HIW"�}�����X���G"HIC+open_num[0]+","+open_num[1]+","+open_num[2]+","+open_num[3]+","+open_num[4]+","+open_num[5]+HIW" �S�O�����G"HIC+open_spec_num+"\n"NOR;

	msg += HIG"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

	msg += "�`�@��`�G"	+sprintf("%18s",NUMBER_D->number_symbol(total_lotto))+" �`\n";
	msg += "�`�m�����G"	+sprintf("%18s",NUMBER_D->number_symbol(count(lotto_info["total_prize"],"+",lotto_info["first_prize"])))+" ��\n";
	msg += "�Y�m�m���G"	+HIR+sprintf("%18s",NUMBER_D->number_symbol(money[0]))+" ��"HIC"(�W���ֿn "+NUMBER_D->number_symbol(lotto_info["first_prize"])+" ��)\n"NOR;
	msg += "�G���m���G"	+HIG+sprintf("%18s",NUMBER_D->number_symbol(money[1]))+" ��\n"NOR;
	msg += "�T���m���G"	+HIG+sprintf("%18s",NUMBER_D->number_symbol(money[2]))+" ��\n"NOR;
	msg += "�|���m���G"	+HIG+sprintf("%18s",NUMBER_D->number_symbol(money[3]))+" ��\n"NOR;
	msg += "�����m���G"	+HIG+sprintf("%18s",money[4])+" ��\n"NOR;

	msg += HIG"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

	msg += "�����ֳz�m�έp�����H�Ʀp�U�G\n";
	msg += "�Y���G "HIM+sprintf("%6d",total_prize[0])+NOR"�H�A�C�`�i�o "HIG+sprintf("%18s",NUMBER_D->number_symbol(count(money[0],"/",total_prize[0]||1)))+NOR" ��\n";
	msg += "�G���G "HIM+sprintf("%6d",total_prize[1])+NOR"�H�A�C�`�i�o "HIG+sprintf("%18s",NUMBER_D->number_symbol(count(money[1],"/",total_prize[1]||1)))+NOR" ��\n";
	msg += "�T���G "HIM+sprintf("%6d",total_prize[2])+NOR"�H�A�C�`�i�o "HIG+sprintf("%18s",NUMBER_D->number_symbol(count(money[2],"/",total_prize[2]||1)))+NOR" ��\n";
	msg += "�|���G "HIM+sprintf("%6d",total_prize[3])+NOR"�H�A�C�`�i�o "HIG+sprintf("%18s",NUMBER_D->number_symbol(count(money[3],"/",total_prize[3]||1)))+NOR" ��\n";
	msg += "�����G "HIM+sprintf("%6d",total_prize[4])+NOR"�H�A�C�`�i�o "HIG+sprintf("%18s",FIFTH_REWARD+"")+NOR" ��\n";

	msg += HIG"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

	lotto_info["total_prize"] = total_prize[0] ? count(count(lotto_info["total_prize"],"*",62),"/",100) : lotto_info["total_prize"];
	lotto_info["total_prize"] = count(lotto_info["total_prize"], "-", total_prize[1]?money[1]:"0");
	lotto_info["total_prize"] = count(lotto_info["total_prize"], "-", total_prize[2]?money[2]:"0");
	lotto_info["total_prize"] = count(lotto_info["total_prize"], "-", total_prize[3]?money[3]:"0");

	lotto_info["first_prize"] = total_prize[0] ? lotto_info["total_prize"] : count(lotto_info["total_prize"],"+",lotto_info["first_prize"]);

	if( count(lotto_info["first_prize"],"<",0) )
		lotto_info["first_prize"] = "0";

	msg += "�����Ѿl�����G"+HIC+NUMBER_D->number_symbol(lotto_info["first_prize"])+" ��"NOR"�A�N�ֿn�ܤU���Y�������C\n";

	msg(msg, this_object(),0,1);

	reset_eval_cost();

	foreach(id, map in prize)
	{
		string ppl_money;

		if( !sizeof(map) ) continue;

		if( objectp(ppl=find_player(id)) )
		{
			tell(ppl,"\n�� "+lotto_info["time"]+" ���ֳz�m�}���w�g�����A"+pnoun(2, ppl)+"��������T�p�U�G\n"HIY+
			    (map[0]?"�Y�� "+map[0]+" �` ":"")+
			    (map[1]?"�G�� "+map[1]+" �` ":"")+
			    (map[2]?"�T�� "+map[2]+" �` ":"")+
			    (map[3]?"�|�� "+map[3]+" �` ":"")+
			    (map[4]?"���� "+map[4]+" �` ":"")+NOR);
		}
		else 
		{
			ppl = load_user(id);
		}

		if( !objectp(ppl) ) continue;

		ppl_money = count(map[0],"*",count(money[0],"/",total_prize[0]||1));
		ppl_money = count(ppl_money,"+",count(map[1],"*",count(money[1],"/",total_prize[1]||1)));
		ppl_money = count(ppl_money,"+",count(map[2],"*",count(money[2],"/",total_prize[2]||1)));
		ppl_money = count(ppl_money,"+",count(map[3],"*",count(money[3],"/",total_prize[3]||1)));
		ppl_money = count(ppl_money,"+",count(map[4],"*",FIFTH_REWARD));

		if( userp(ppl) )
			tell(ppl, "\n�@�o�������� "HIG"$"+MONEY_D->query_default_money_unit()+" "+NUMBER_D->number_symbol(ppl_money)+NOR" ���I�I\n\n");

		if( map[0] )
		{
			shout("\n"+repeat_string(HIY"�ֳz�}���G"+ppl->query_idname()+HIY"��o�Y�� "+map[0]+" �`�@ "+NUMBER_D->number_symbol(count(map[0],"*",count(money[0],"/",total_prize[0]||1)))+" ���I�I�I�I\n",5)+"\n"NOR);

			ppl->add_title( random(2) ? HIC"�ֳz"NOR CYN"�j��"NOR : HIC"�ֳz"NOR CYN"���P"NOR );
		}
		
		catch {
			if( objectp(ppl) )
			{
				ppl->earn_money(MONEY_D->query_default_money_unit(), ppl_money);
				ppl->save();
			}
		};

		if( objectp(ppl) && !userp(ppl) )
			destruct(ppl);
	}

	lotto_info["time"]++;
	table = allocate_mapping(0);
	lotto_info["lotto_opening"] = 0;
	lotto_info["total_prize"] = "0";

	set_temp("status", HIY"�Y���ֿn�����G"+NUMBER_D->number_symbol(lotto_info["first_prize"])+NOR);
	save_object(DATA_PATH);

	set_heart_beat(6000);
}

void lotto_open_schedule(int *open_num, int *no_sort_open_num, int open_spec_num, mapping prize, int *total_prize, int total_lotto, int sec, int total_sec, int open)
{
	int *num, i, j, fit;
	string id;
	array arr1, arr2;

	if( !(--sec % (total_sec/7)) )
	{
		if( open < 6 )
			msg("�ثe�w�g�}�X�� "+(++open)+" �Ӹ��X�G"+HIY+no_sort_open_num[open-1]+"���I"NOR+"\n", this_object(), 0, 1);
		else if( open == 6 )
		{
			msg("�̫�@�y�S�O���G"+HIY+open_spec_num+" ���I"NOR+"\n", this_object(), 0, 1);
			msg("�ثe���b�p�⤤�����G�A�еy�ԡC\n", this_object(), 0, 1);
			open++;
		}
	}


	foreach( id, arr1 in table )
	{
		if( !sizeof(arr1) )
			continue;

		if( !arrayp(prize[id]) )
			prize[id] = allocate(5);

		reset_eval_cost();

		foreach( arr2 in arr1 )
		{
			foreach( num in arr2 )
			{
				fit = 0;
				foreach( i in num )
				{
					foreach( j in open_num )
					{
						if( i == j )
						{
							fit++;
							break;
						}
					}
				}

				if( fit > 2 )
				{
					if( fit == 3 )
					{
						prize[id][4]++;
						total_prize[4]++;
					}
					else if( fit == 4 )
					{
						prize[id][3]++;
						total_prize[3]++;
					}
					else if( fit == 5 )
					{
						//�ˬd�S�O��
						if( member_array( open_spec_num, num ) != -1 )
						{
							prize[id][1]++;
							total_prize[1]++;
						}
						else
						{
							prize[id][2]++;
							total_prize[2]++;
						}
					}
					else if( fit == 6 ) 
					{
						prize[id][0]++;
						total_prize[0]++;
					}
				}
			}

			table[id] = table[id][1..];

			if( !sizeof(table[id]) )
				map_delete(table, id);

			call_out( (:lotto_open_schedule ,open_num, no_sort_open_num, open_spec_num, prize, total_prize, total_lotto, sec, total_sec, open:), 1); 
			return;
		}
	}

	if( open == 7 )
		lotto_open_finish(open_num, open_spec_num, prize, total_prize, total_lotto);
	else
		call_out( (:lotto_open_schedule ,open_num, no_sort_open_num, open_spec_num, prize, total_prize, total_lotto, sec, total_sec, open:), 1); 
}

int no_save(string file)
{
	return 1;
}

void lotto_open()
{
	int open_sec;
	int *open_num, *no_sort_open_num, open_spec_num, total_lotto;

	if( sizeof(add_number_schedule) )
		return tell(this_player(), "�ثe�U�`��Ʃ|���B�z�έp�����A�еy��A��}���C\n");

	if( lotto_info["lotto_opening"] )
		return tell(this_player(), "�ثe�w�g�b�}���F�C\n");

	//�p��Ҧ���`��
	foreach(string id, array arr1 in table)
	foreach(array arr2 in arr1)
	total_lotto += sizeof(arr2);

	if( !total_lotto )
		return tell(this_player(), "�L�����`��ơA�L�k�}���C\n");

	open_sec = (total_lotto/MAX_LOTTO_OPEN_PER_ROUND) < 7 ? 7 : (total_lotto/MAX_LOTTO_OPEN_PER_ROUND);
	msg(HIY"\n�� "+lotto_info["time"]+" ���ֳz�m�}�l�}���F�I�I\n�ثe�|�Q�G���m�y�w�g�}�l�j�ʡA�Фj�a�@�ߵ��Զ}�����G("+open_sec+" ��)�C\n\n"NOR, this_object(),0,1);

	// �}�����X
	no_sort_open_num = random_number();
	open_num = sort_array(no_sort_open_num, (: $1 > $2 ? 1 : -1 :));
	// �S�O��
	while( member_array((open_spec_num=random(42)+1), open_num) != -1 );

	lotto_info["lotto_opening"] = 1;
	lotto_open_schedule(
	    open_num, 
	    no_sort_open_num,
	    open_spec_num, 
	    allocate_mapping(0), 
	    allocate(5),
	    total_lotto,
	    open_sec,
	    open_sec,
	    0,
	);
}

void lotto_info()
{
	int i, count;
	string info = "";

	foreach(string id, array arr1 in table)
	{
		count = 0;
		foreach(array arr2 in arr1)
		count += sizeof(arr2);

		if( !count ) continue;

		info += sprintf(HIW"%-4s"NOR"%-30s"HIW"%10d"NOR WHT" �`\n"NOR, ++i+".", find_player(id) ? find_player(id)->query_idname() : capitalize(id), count);
	}
	if( sizeof(add_number_schedule) )
		info += "(�U�`��ƥ��b�p�⤤)\n";

	tell(this_player(), info==""?"�ثe�L�H�U�`�C\n\n":"�ثe�@�� "+i+" �H�U�`�G\n"+info+"\n");
}

void lotto_clean()
{
	table = allocate_mapping(0);
	add_number_schedule = allocate_mapping(0);

	lotto_info = ([ 
	    "first_prize":"0",
	    "total_prize":"0", 
	    "history":allocate_mapping(0),
	    "time":1,
	    "lotto_opening":0 
	]);

	save_object(DATA_PATH);

	msg("$ME�b�ֳz�mñ�`���W�i����������ˡA�g�T�{��w�M���Ҧ��m��ñ�`��ơC\n", this_player(),0,1);
}

// �� id �[�J�s���Ʀr��
void add_number()
{
	int count=0;

	foreach( string id, mixed num in add_number_schedule )
	{
		if( intp(num) )
		{
			while(num--)
			{
				if( ++count > MAX_LOTTO_PER_ROUND )
				{
					add_number_schedule[id] = num+1;
					break;
				}

				if( !num )
				{
					if( sizeof(table[id][sizeof(table[id])-1]) >= MAX_LOTTO_PER_ROUND )
						table[id] += allocate(1, allocate(0));
					table[id][<1] += ({ random_number() });
					map_delete(add_number_schedule, id);
					break;
				}

				if( sizeof(table[id][sizeof(table[id])-1]) >= MAX_LOTTO_PER_ROUND )
					table[id] += allocate(1, allocate(0));

				table[id][<1] += ({ random_number() });
			}
		}
		else if( arrayp(num) )
		{
			if( sizeof(table[id][sizeof(table[id])-1]) >= MAX_LOTTO_PER_ROUND )
				table[id] += allocate(1, allocate(0));

			table[id][<1] += ({ num });

			map_delete(add_number_schedule, id);

			if( ++count > MAX_LOTTO_PER_ROUND )
				break;
		}
		else map_delete(add_number_schedule, id);
	}

	if( sizeof(add_number_schedule) )
		call_out( (: add_number :), 1);
}

void do_lotto(object me, string arg)
{
	string money, id = me->query_id(1);

	int *num = allocate(6), i, count, continuing;

	switch( arg )
	{
	case 0:
		{
			string msg="";
			mapping mylotto;

			mylotto = table[id];

			if( !sizeof(mylotto) )
				return tell(me, pnoun(2, me)+"�S�������`��ơC\n");

			foreach(array arr in mylotto)
			msg += implode(map(arr, (: implode($1, (: $1+","+$2 :)) :)), "\n");

			me->more(pnoun(2, me)+"�Ҧ�����`��Ʀp�U�G\n"+msg);
			return;
			break;
		}
	case "info":	
		return lotto_info();
		break;
	case "clean":	
		//if( !wizardp(me) ) 
		//return tell(me, pnoun(2, me)+"�S���v�Q�M���ֳz��ơC\n");
		//else 
		//return lotto_clean();
		break;
	case "open":
		if( lotto_info["lotto_opening"] )
			return tell(me, "�ثe���b�}���A���వ����ơC\n");

		if( !wizardp(me) ) 
			return tell(me, pnoun(2, me)+"�S���v�Q����ֳz�}�����ʡC\n");
		else 
			return lotto_open();
		break;
	default:
		break;
	}

	if( query("lotto_fixing") )
		return tell(me, "���x���פ��A�еy��A���`�C\n");

	if( lotto_info["lotto_opening"] )
		return tell(me, "�ثe���b�}���A���వ����ơC\n");

	if( !arrayp(table[id]) )
		table[id] = allocate(1, allocate(0));

	// �p�� id ��`�ƥ�
	if( arrayp(table[id]) )
	{
		foreach( array answer in table[id] )
		count += sizeof(answer);
	}

	// �Y�O��J���ռƦr
	if( sscanf(arg, "%d,%d,%d,%d,%d,%d", num[0],num[1],num[2],num[3],num[4],num[5]) == 6 
	    || sscanf(arg, "%d,%d,%d,%d,%d,%d -c %d", num[0],num[1],num[2],num[3],num[4],num[5],continuing) == 7)
	{
		if( count+1 > MAX_LOTTO_AMOUNT ) 
			return tell(me, "�ثe�C�H�C���̰���`�Ƭ� "+MAX_LOTTO_AMOUNT+" �`�C\n");

		if( wizardp(me) && count > 0 )
			return tell(me, "�Ův�C���u���@�`�A�קK�z�Z���a�g�٥����C\n");

		num = sort_array(num, (:$1>$2?1:-1:));

		for(i=0;i<6;i++)
		{
			if( num[i] < 1 || num[i] > 42 ) 
				return tell(me, "���X���o�C�� 1 �ΰ��� 42�C\n");
			if( i && num[i] <= num[i-1] )
				return tell(me, "���X���i����ñ�`�C\n");
		}

		if( sizeof(add_number_schedule) )
			return tell(me, "�e�@���`�̪�����٦b�B�z���A�еy��A�աC\n");

		if( !me->spend_money(MONEY_D->query_default_money_unit(), MONEY_PER_LOTTO) )
			return tell(me, pnoun(2, me)+"�� $"+MONEY_D->query_default_money_unit()+" ���R���_����@�i�m��F�I\n");

		lotto_info["total_prize"] = count(MONEY_PER_LOTTO*RECYCLE_PERCENT/100, "+", lotto_info["total_prize"]);

		add_number_schedule[id] = num;

		add_number();

		msg("$ME��n�F���ӼƦr�G�u"+num[0]+","+num[1]+","+num[2]+","+num[3]+","+num[4]+","+num[5]+"�v�A�ê�F $"+MONEY_D->query_default_money_unit()+" "+MONEY_PER_LOTTO+" ���U�`�C\n",me,0,1);
	}
	else if( (big_number_check(arg) && sscanf(arg, "%d", i)) == 1 || sscanf(arg, "%d -c %d", i , continuing) == 2 )
	{
		if( i < 1 || i > MAX_SINGLE_TIME_LOTTO )
			return tell(me, "�q���︹�`�Ƥ��o�C�� 1 �`�άO�@���U�`���� "+MAX_SINGLE_TIME_LOTTO+" �`�C\n");

		if( count+i > MAX_LOTTO_AMOUNT ) 
			return tell(me, "�ثe�C�H�C���̰���`�Ƭ� "+MAX_LOTTO_AMOUNT+" �`�C\n");

		if( wizardp(me) && count+i > 1 )
			return tell(me, "�Ův�C���u���@�`�A�קK�z�Z���a�g�٥����C\n");

		if( sizeof(add_number_schedule) )
			return tell(me, "�e�@���`�̤U�`����٦b�B�z���A�еy��A�աC\n");

		money = count(MONEY_PER_LOTTO, "*", i);

		if( !me->spend_money(MONEY_D->query_default_money_unit(), money) )
			return tell(me, pnoun(2, me)+"���W�� $"+MONEY_D->query_default_money_unit()+" ���R���_ "+i*(continuing||1)+" �i�m���I\n");

		lotto_info["total_prize"] = count(count(count(money, "*", RECYCLE_PERCENT),"/",100), "+", lotto_info["total_prize"]);

		set_temp("status", HIY"�Y���ֿn�����G"+NUMBER_D->number_symbol(count( copy(count(count(lotto_info["total_prize"],"*",38),"/",100)), "+", lotto_info["first_prize"])))+NOR;
		save_object(DATA_PATH);
		add_number_schedule[id] = i;

		add_number();

		msg("$ME�Q�ιq���︹�U�F "+NUMBER_D->number_symbol(i)+" �`�A�@��F $"+MONEY_D->query_default_money_unit()+" "+NUMBER_D->number_symbol(count(i,"*",MONEY_PER_LOTTO))+" ���C\n",me,0,1);
	}
	else return tell(me, "��`���O���~�A�иԲӾ\Ū help lotto�C\n");
}

void heart_beat()
{
	// �Y�|���U�`��Ƭ��B�z�h set_heart_beat(50); 5 ���A�}��
	if( !sizeof(add_number_schedule) )
	{
		if( lotto_info["total_prize"] == "0" )
			return;

		msg(HIY"�Q�����@�����ֳz�m���۰ʶ}���ɶ���F�I�I\n"NOR, this_object(),0,1);
		set_heart_beat(0);
		lotto_open();
	}
	else
		set_heart_beat(50);
}

void create()
{
	restore_object(DATA_PATH);

	if( !mapp(lotto_info) )
		lotto_info = ([ 
		    "first_prize":"0",
		    "total_prize":"0", 
		    "history":allocate_mapping(0),
		    "time":1,
		    "lotto_opening":0,
		]);

	set_idname("lotto machine",HIC"�ֳz�m"NOR CYN"ñ�`��"NOR);


	set_temp("status", HIY"�Y���ֿn�����G"+NUMBER_D->number_symbol(lotto_info["first_prize"])+NOR);
	set("flag/no_amount", 1);
	set("long",help);
	set("unit","�x");
	set("mass",-1);

	table = allocate_mapping(0);
	add_number_schedule = allocate_mapping(0);



	if( file_size(DATA_PATH) < 0 ) save_object(DATA_PATH);

	// �Q���� open �@��
	set_heart_beat(6000);
}
