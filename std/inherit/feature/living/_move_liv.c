/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _move_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-1
 * Note   : �ͪ������~����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <type.h>
#include <daemon.h>
#include <object.h>
#include <location.h>
#include <message.h>
#include <envvar.h>
#include <login.h>
#include <buff.h>

private nomask void process_command(string verb, string args, string input);

varargs nomask void do_look(string args)
{
	process_command("look", args, "look "+args);
}

varargs nomask void move(mixed where)
{	
	array originalloc = query_temp("location");
	object env = environment();

	if( !where ) return;

	switch(typeof(where))
	{
	case ARRAY:	
		{
			object new_env = env;

			where[CODE] = where[X]+"/"+where[Y]+"/"+where[Z]+"/"+where[PLACE]+"/"+where[NUM]+"/"+where[EXTRA];

			// leave()
			present_objects(this_object())->leave(this_object());
			
			set("section_num", where[NUM]);
			MAP_D->leave_coordinate(originalloc, this_object());

			// �ˬd�O�_�n���ܷs���a�Ϫ���
			if( arrayp(originalloc) )
			{
				// �����H�ιϹ�
				if( userp(this_object()) )
					MAP_D->remove_player_icon(this_object(), originalloc);

				if( !env || !env->is_maproom() || originalloc[NUM] != where[NUM] || originalloc[PLACE] != where[PLACE] )
					new_env = MAP_D->query_maproom(where);			
			}
			else
			{
				if( userp(this_object()) && !(query("env/flag") & FLAG_NO_MAP) )
					startup_title_screen(this_object(), 11);

				new_env = MAP_D->query_maproom(where);
			}

			if( objectp(env) && !env->is_maproom() )
				
				env->leave(this_object());
			
			if( objectp(new_env) && env != new_env )
			{
				move_object(new_env);
				set_temp("maproom", new_env);
			}

			// �����ϥ� copy(), �_�h�y�з|�V��
			set_temp("location", copy(where));
			MAP_D->enter_coordinate(where, this_object());
			
			if( userp(this_object()) ) do_look();

			// init()
			present_objects(this_object())->init(this_object());

			// �]�w�H�ιϹ� 
			if( userp(this_object()) )
				MAP_D->set_player_icon(this_object(), where);

			break;
		}
	case STRING:
		where = load_object(where);
		
		if( !objectp(where) )
		{
			tell(this_object(), "�W�@�����u�ɪ��Ҧb�a�w�g���s�b�o�ӥ@�ɤW�F�A�^��Ův�j�U...�C\n");
			where = load_object(STARTROOM);
		}
	case OBJECT:
		{
			// leave()
			present_objects(this_object())->leave(this_object());

			if( objectp(env) )
			{
				if( env->is_maproom() )
				{
					if( userp(this_object()) )
					{
						MAP_D->remove_player_icon(this_object(), originalloc);
						reset_screen(this_object());
					}
					delete_temp("location");

					MAP_D->leave_coordinate(originalloc, this_object());
				}
				else
					env->leave(this_object());
			}

			move_object(where);

			if( where == find_object(VOID_OB) ) return;
			
			if( userp(this_object()) ) do_look();
			// init()
			where->init(this_object());
			present_objects(this_object())->init(this_object());
			break;
		}
	default:
		error(sprintf("�L�k���θ��J���� %O �C\n", where));
		break;
	}
}

nomask varargs void move_to_environment(object ob, mixed amount)
{
	object env = environment(ob);

	if( !env ) return;

	if( env->is_maproom() )
		move( query_temp("location", ob) );
	else
		move( env );
}

// ��²��ʪ���ܥt�@����W
nomask void move_to_object(object where)
{
	if( where == this_object() || where == environment() ) return;

	move_object(where);
}

// �ϸ��H�̸�۲���
nomask void follower_move(mixed origin, mixed target)
{
	int moving_stamina_cost;
	string this_idname;
	object follower, *followers = query_temp("follower");

	if( !arrayp(followers) || !origin || !target ) return;

	this_idname = this_object()->query_idname();

	followers -= ({ 0 });
	
	if( !sizeof(followers) )
		delete_temp("follower");

	foreach( follower in followers )
	{
		if( !objectp(follower) ) continue;

		if( arrayp(origin) && save_variable(query_temp("location", follower)) != save_variable(origin) ) continue;
		if( objectp(origin) && origin != environment(follower) ) continue;
		
		// �O�ǤH
		if( query("prisoner", follower) )
		{
			msg("$ME����}�Q�K��R��A�L�k�ۥѦ�ʡC\n", follower, this_object(), 1);
			continue;
		}

		if( follower->is_delaying() )
		{
			tell(follower, follower->query_delay_msg());
			msg("$ME���b���L���A�L�k�~����H$YOU�C\n", follower, this_object(), 1);
			continue;
		}
		if( follower->is_faint() )
		{
			msg("$ME���ˤF�A�L�k�~����H$YOU�C\n", follower, this_object(), 1);
			continue;	
		}
			
		moving_stamina_cost = -follower->query_all_buff(BUFF_MOVING_STAMINA);
		if( moving_stamina_cost > 0 && !follower->cost_stamina(moving_stamina_cost) )
		{
			msg("$ME�w�g�S����O�~��e�i�F�C\n", follower, this_object(), 1);
			continue;
		}
		
		if( follower->query_loading() > follower->query_max_loading() )
		{
			msg("$ME��ۨI��������e�i�A���רs�O�O���q�ߡC\n", follower, this_object(), 1);
			follower->faint();
			continue;
		}

		msg("$ME���H��"+this_idname+"���F�L�h�C\n", follower, 0, 1);
		follower->move(target);
		msg("$ME���H��$YOU���F�L�ӡC\n", follower, this_object(), 1, 0, ({ follower }));

		follower->follower_move(origin, target);
	}
}

private int move_or_destruct( object dest )
{
	tell(this_object(), "�A�Ҧb������"+(dest?"[ "+dest->query_name()+" ]":"")+"�Q�R���F�C\n", ENVMSG);
	move(VOID_OB);
	tell(this_object(), "�A���b�@�����t�����A�|�P���Ŷ����G�L�������C\n", ENVMSG);
}
