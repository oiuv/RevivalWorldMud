/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : look.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-8
 * Note   : look ���O
 * Update :
 *  o 2001-07-03 Clode �W�[����y�ЧP�_
 *
 -----------------------------------------
 */

#include <map.h>
#include <lock.h>
#include <time.h>
#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <location.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        �з� look ���O�C
HELP;

#define MAX_OBJECT_SHOW		30

#define EXITSORT	({"up","down","west","east","north","south","southeast","southwest","northwest","northeast"})

int sort_exits(mapping exits, string e1, string e2)
{
	int idx1, idx2;
	
	if( arrayp(exits[e1]) ) return 1;
	if( arrayp(exits[e2]) ) return -1;

	idx1 = member_array(e1, EXITSORT);
	idx2 = member_array(e2, EXITSORT);
	
	if( idx1 == -1 ) return 1;
	if( idx2 == -1 ) return -1;

	return idx1-idx2;
}

private void do_command(object me, string arg)
{
	string msg="";
	object ob, env = environment(me);
	object *inventories;
	mapping exits, actions;
	mixed targetroom;

	if( !env )
		return tell(me, "�A�����ƨ��b�ŵꤧ���A���񤰻򳣨S���A�@����šC\n");

	if( !arg || arg == "here" )
		inventories = present_objects(me) - ({ me });
	else if( arg == "outside" )
	{
		if( !environment(env) )
			return tell(me, query("short", env)+"���~�����򳣨S���C\n");
		else
		{
			inventories = present_objects(env) - ({ env });
			env = environment(env);
		}
	}
	else
	{
		// Looking At Interactive Object
		if( arg == "me" ) ob = this_player();
		if( !ob ) ob = present(arg);
		if( objectp(ob) )
		{
			if( ob->is_living() )
			{
				if( ob == me )
					msg("$ME�ݤF�ݦۤv����A�@�y�g�Ī��ˤl�C\n", me, 0, 1);
				else
					msg("$ME�������t���L$YOU���W���C�@�Ӥ�ӤաA���G�Q�n�ݳz$YOU���@���C\n", me, ob, 1);
			}
	
			if( !ob->is_living() && mapp(actions = ob->query_actions()) )
				msg += sprintf("�S����O "HIY"%-=80s\n"NOR, implode(keys(actions), ", "));
	
			msg = " "+ob->long()+msg;
			return me->more(msg);
		}
		else if( targetroom = query("exits/" + arg, env) )
		{
			if( !wizardp(me) && query("lock/" + arg, env) )
				return tell(me, "�� "+capitalize(arg)+" ��V���@�D��צ�F"+pnoun(2, me)+"�����u�C\n");
			if( stringp(targetroom) )
			{
				env = load_object(targetroom);
				inventories = all_inventory(env);
				msg("$ME�����F��l�A���Y�� "+capitalize(arg)+" ��V��"+query("short", env)+"�ݥh�C\n", me, 0, 1);
				broadcast(env, "������G���H�����o�Ӧa��ݤF�L�ӡC\n");
			}
			else
				return tell(me, "�q�o�̵L�k�� "+arg+" ����V�ݡC\n");
		}
		else return tell(me, "�o�̨S�� " + arg + " �o�ӪF��ΥX�f�C\n");
	}

	// �a�Ϩt��
	if( env->is_maproom() )
	{
		array loc = arg == "outside" ? query_temp("location", environment(me)) : query_temp("location", me);

		if( query("last_title_screen", me) != MAP_D )
		{
			tell(me, SAVEC + REF + REST);
			set("last_title_screen", MAP_D, me);
		}

		if( sizeof(loc) )
		msg += "�ثe��m"NOR WHT"("+(loc[X]+1)+","+(loc[Y]+1)+")�w�w"NOR"�u"+MAP_D->coor_short_name(loc)+NOR"�v\n";

		if( !query("env/no_map", me) )
			msg += (MAP_D->show_map(loc) || "�a�Ϩt�ο��~�A�гq���Ův�B�z�C\n");
		msg += MAP_D->query_map_system(loc)->query_coor_data(loc, "long")||"";
		msg += MAP_D->query_map_system(loc)->query_coor_data(loc, "status")||"";

	}
	// �@�����Ҫ������
	else	
	{
		
		array loc = env->query_location();
		object master = env->query_master();
		string room_function_short = env->query_room_function_short();
		string longtext = query("long", env);

		msg += (query("short", env)||"�L�W��") + (room_function_short?" "+room_function_short:"") +(loc?" "+HIG+env->query_coor_short()+NOR:"")+ (wizardp(me)?" -> "+NOR WHT+file_name(env)+NOR:"")+"\n\n";
		
		if( longtext && longtext[0] )
			msg += longtext + (longtext[<1] == '\n' ? "\n" : "\n\n");

		msg += query("status", env)||"";

		if( env->is_module_room() )
			msg += (env->query_module_file())->look_room(env) || "";

		if( objectp(master) && sizeof(master->query_slave()) )
			msg += "�s�� "+ (env == master ? HIG"�s��"NOR GRN"����"NOR:query("short", master)+HIG" <"+(master->query_location()[X]+1)+","+(master->query_location()[Y]+1)+">"NOR )+"�A�@ "+HIG+(sizeof(master->query_slave())+1)+NOR" �B\n";

		if( mapp(actions = env->query_actions()) && sizeof(actions))
			msg += "���O "HIY+implode(sort_array(keys(actions),1)," ")+NOR+"\n";

		if( !(exits = query("exits", env)) ) 
			msg += "�o�Ӧa��S�����󪺥X�f�C\n";
		else
		{
			int lockflag;
			string *sort_exits, exitname, exitlong;

			exitlong = "�X�f ";

			sort_exits = sort_array(keys(exits), (: sort_exits($(exits), $1, $2) :));

			foreach( string dir in sort_exits )
			{
				if( arrayp(exits[dir]) )
				{
					exitname = HIW"<"+capitalize(dir)+">"NOR;
					lockflag = MAP_D->query_coor_data(exits[dir], "lock");
				}
				else if( functionp(exits[dir]) )
				{
					exitname = HIG"<"+capitalize(dir)+">"NOR;
					lockflag = query("lock/"+dir, env);
				}
				else
				{
					exitname = "<"+capitalize(dir)+">"NOR;
					lockflag = query("lock/"+dir, env);
				}
				
				if( lockflag & LOCKED )
					exitname = HIR"<"+capitalize(dir)+">"NOR;
				if( lockflag & WIZLOCKED )
					exitname = HIY"<"+capitalize(dir)+">"NOR;
					
				exitlong += exitname+" ";
			}

			msg += multiwrap(exitlong, 72);
			
			msg[<1..<1] = NOR"\n\n";
		}
	}

	foreach(ob in sort_objects(inventories))
		msg += (ob->is_living()||ob->is_board() ? ob->short(1) : " "+remove_fringe_blanks(ob->short(1)))+ (wizardp(me)&&!userp(ob)?" -> "+NOR WHT+file_name(ob)+NOR:"")+"\n";

	tell(me, msg);
}
