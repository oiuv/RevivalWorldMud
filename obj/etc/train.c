/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : train_ob.c
 * Author : Sinji@RevivalWorld
 * Date   : 2005-04-10
 * Note   : ���� (�Q���쵲��C)
 * Update :
 *  o 2005-04-13 Sinji �ץ��{���Ĳv
 -----------------------------------------
 */
#include <ansi.h>
#include <daemon.h>
#include <inherit.h>
#include <feature.h>
#include <secure.h>
#include <lock.h>

inherit STANDARD_OBJECT;
//inherit VEHICLE_ACTION_MOD;

void do_enter(object me, string arg);
void do_out(object me, string arg);
void do_lookout(object me, string arg);
void do_drive(object me, string arg);

// ���O�ʧ@
nosave mapping actions = 	
([
	"enter"			:	(: do_enter :),
	"out"			:	(: do_out :),
	"lookout"		:	(: do_lookout :),
	"drive" 		:	(: do_drive :),
]);
// �Ϥ�V������
nosave mapping corresponding =
([
	"north"			:	"south",
	"south"			:	"north",
	"east"			:	"west",
	"west"			:	"east",
	"northwest"		:	"southeast",
	"northeast"		:	"southwest",
	"southwest"		:	"northeast",
	"southeast"		:	"northwest",
	"up"			:	"down",
	"down"			:	"up",
]);

varargs int new_train(int n);

nosave object prev_train, next_train;

int clean_up(int inherited)
{
	return 0;
}
void enter_next_train(object me)
{
	if( !objectp(next_train) )
		return tell(me, "�����c�èS���s���U�`���c�C\n");
	me->move(next_train);
}
void enter_prev_train(object me)
{
	if( !objectp(prev_train) )
		return tell(me, "�����c�èS���s���W�`���c�C\n");
	me->move(prev_train);
}
void set_next_train(object ob)
{
	next_train = ob;
	set("exits/next", (: enter_next_train :));
}
void set_prev_train(object ob)
{
	prev_train = ob;
	set("exits/prev", (: enter_prev_train :));
}

object query_prev_train()
{
	return prev_train;
}

object query_next_train()
{
	return next_train;
}

object query_head_train()
{
	if( objectp(prev_train) )
		return prev_train->query_head_train();
	return this_object();
}

object query_last_train()
{
	if( objectp(next_train) )
		return next_train->query_last_train();
	return this_object();
}

void create()
{
	set_idname("train", HIY"R05413 "NOR YEL"�x�K�۱j������");
	set("flag/no_amount", 1);
	set("short", "�����Y");
	set("long", @HELP
�A�b�@�x�����̡A�A�i�H�z�L�|�P���l�ݨ쵡�~������
HELP);
	set("unit", "�x");
	set("mass", -1);

	//new_train(5);
}
string long()
{
	string help = @HELP
R05413 �x�K�۱j������
HELP;
	return help;
}

varargs int new_train(int n)
{
	object head_train, last_train, new_train;
	int train_count;

	head_train = query_head_train();
	//last_train = query_last_train();
	train_count = query("train_count", head_train);
	// new train limit
	if( train_count >= 40 ) return 0;
	if( train_count + n >= 40 ) n = 40 - train_count;
	do
	{
		last_train = query_last_train();
		new_train = new(base_name(this_object()));

		if( !new_train ) return 0;
		train_count++;
		new_train->set_idname("train " + train_count, "�۱j��������" + CHINESE_D->chinese_number(train_count) + "���c");
		set("short", "������" + CHINESE_D->chinese_number(train_count) + "���c", new_train);
		new_train->set_prev_train(last_train);
		last_train->set_next_train(new_train);
	}
	while(--n);

	set("train_count", train_count, head_train);
	set_temp("status", HIY"(���c " + train_count + " �`)"NOR, head_train);
	if( !train_count ) delete_temp("status", head_train);
	return 1;
}
int remove_train()
{
	object head_train, last_train, ptrain;
	int train_count;
	
	head_train = query_head_train();
	last_train = query_last_train();
	if( head_train == last_train ) return 0;

	ptrain = last_train->query_prev_train();
	train_count = query("train_count", head_train);

	all_inventory(last_train)->move(ptrain);

	delete("exits/next", ptrain);

	train_count--;
	set("train_count", train_count, head_train);
	set_temp("status", HIY"(���c " + train_count + " �`)"NOR, head_train);
	if( !train_count ) delete_temp("status", head_train);
	destruct(last_train);
	return 1;
}

void do_out(object me, string arg)
{
	object my_env = environment(me);
	object train_env = environment(this_object());
	string my_idname = me->query_idname();
	string train_idname = this_object()->query_idname();
	array loc;

	if( my_env != this_object() )
		return tell(me, "�A�ä��b�����W�C\n");
	if( !train_env ) train_env = environment(query_head_train());

	loc = query_temp("location", query_head_train());
	broadcast(train_env, my_idname + "�q" + train_idname + "�W���F�U�ӡC\n", 0, me);
	me->move(loc || train_env);
	broadcast(my_env, my_idname + "�q�������F�X�h�C\n", 0, me);
}
void do_enter(object me, string arg)
{
	object my_env = environment(me);
	object train_env = environment(this_object());
	string my_idname = me->query_idname();
	string train_idname = this_object()->query_idname();

	if( my_env == this_object() )
		return tell(me, "�A�w�g�b�����W�F�C\n");

	broadcast(train_env, my_idname + "�q�������F�i�ӡC\n", 0, me);
	me->move(this_object());
	broadcast(my_env, my_idname + "�f�W�F" + train_idname + "�C\n", 0, me);
	
}
void do_drive(object me, string arg)
{
	mixed exit;
	mapping exits;
	string my_idname, from;
	object env, vehicle;

	//���L������U���O
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}
	if( !arg )
		return tell(me, "�A�Q�n�����̨��H\n");

 	vehicle = environment(me);
	env = environment(vehicle);

	if( this_object() != vehicle )
		return tell(me, "�A�ä��b�����W�C\n");
	if( !objectp(env) )
		return tell(me, pnoun(2, me)+"�ثe�L�k�V�����m���ʡA�гq���Ův�B�z�C\n");

	my_idname = vehicle->query_idname();
	
	if( env->is_maproom() )
	{
		switch(MAP_D->valid_move(vehicle, arg))
		{
		case 1:
			if( me->is_fighting() && !COMBAT_D->do_flee(me) ) return;
			return MAP_D->move(vehicle, arg);
		case -1:
			if( wizardp(me))
				return MAP_D->move(vehicle, arg);
			else	return tell(me, "��"+CHINESE_D->to_chinese(arg)+"��V�����Q���F�A"+pnoun(2, me)+"�S��k������p�C\n");
		case -2:
			if( query_temp("go_resist", me) == "WALL_"+arg )
				msg("$ME�������D"+CHINESE_D->to_chinese(arg)+"��O��A�o�̵M�q��p���F�U�h�A���𼲪����إ��D�C\n", vehicle, 0, 1);
			else
			{
				set_temp("go_resist", "WALL_"+arg, me);
				tell(me, "��"+CHINESE_D->to_chinese(arg)+"��O��A"+pnoun(2, me)+"�S��k������p�C\n");
			}
			return;
		default:
			return tell(me, "�� "+arg+" ����V�S���X���C\n");
		}
	}

	if( !mapp(exits = query("exits", env)) || !sizeof(exits) ) return;

	exit = exits[arg];
	from = CHINESE_D->to_chinese(corresponding[arg]);
	if( functionp(exit) ) exit = evaluate(exit, vehicle);

	// �Y�O�y�ХX�f
	if( arrayp(exit) )
	{
		if( (MAP_D->query_map_system(exit))->query_coor_data(exit, "lock") & LOCKED && !wizardp(me) )
			return broadcast(env, my_idname+"�µ۩� "+capitalize(arg)+" ��V�������F�W�h�C\n", 0, vehicle);

		if( !wizardp(me) && (MAP_D->query_map_system(exit))->query_coor_data(exit, "lock") & WIZLOCKED )
			return broadcast(env, my_idname+"�µ۩� "+capitalize(arg)+" ��V�����𼲤F�W�h�C\n", 0, vehicle);
	}
	// �Y�O����W�٥X�f
	else if( stringp(exit) )
	{
		if( query("lock/"+arg, env) & LOCKED && !wizardp(me))					   
			return broadcast(env, my_idname+"�µ۩��@"+capitalize(arg)+" ��V�������F�W�h�C\n", 0, vehicle);

		if( !wizardp(me) && query("lock/"+arg, env) & WIZLOCKED )
			return broadcast(env, my_idname+"�µ۩� "+capitalize(arg)+" ��V�����𼲤F�W�h�C\n", 0, vehicle);
	}
	else return;

	broadcast(env, my_idname+"��"+(CHINESE_D->to_chinese(arg)||" "+capitalize(arg)+NOR" ��V")+"��p�ӥh�C\n", 0, vehicle);

	vehicle->move(exit);

	if( from )
		broadcast(exit, my_idname+"�q"+from+"��p�L�ӡC\n", 0, vehicle);
	else
		broadcast(exit, my_idname+"��p�F�L�ӡC\n", 0, vehicle);
}
void remove()
{
	object last_train;
	while(objectp(last_train = query_last_train()) && objectp(next_train)) destruct(last_train);
}