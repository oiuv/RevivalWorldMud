/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : update.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-02-06
 * Note   : update ���O
 * Update :
 *  o 2002-11-05 Clode rewrite command update
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <secure.h>
#include <object.h>
#include <message.h>

inherit COMMAND;

#define DEEP_INHERIT_UPDATE	1
#define BINARY_DATA		"/data/binaries/"

string help = @HELP
	�з� update ���O
HELP;

varargs private void update(object me, string *update_list, int largenum)
{
	mixed err, env;
	int reset_command, shadowing, timeexpression;
	object ob, void_ob, child, *children, *inv, childenv, inv_elem;
	array childloc;
	mixed childamount;
	int childkeep;
	int childequip;
	string file;
	int status;
	
	void_ob = load_object(VOID_OB);
	
	if( largenum )
		CHANNEL_D->channel_broadcast("news", "�t�ζ}�l�i���s�A�еy��...");

	timeexpression = time_expression {
	foreach( file in update_list )
	{
		inv = allocate(0);
		children = allocate(0);
		
		reset_eval_cost();

		/*
		if( !SECURE_D->permission(file, my_id, WRITE) || !SECURE_D->permission(file, my_id, READ))
		{	
			tell(me, pnoun(2, me)+"�S�����s�sĶ "+file+" ���v�Q�C\n");
			continue;
		}
		*/
		
		if( !file_exists(file) )
		{
			tell(me, "�S�� "+file+" �o���ɮסC\n");
			continue;
		}

		if( objectp(ob = find_object(file)) )
		{
			if( ob == void_ob )
			{
				tell(me, pnoun(2, me)+"�S�����s�sĶ VOID_OB ���v�Q�C\n");
				continue;
			}
			
			if( ob == this_object() )
			{
				tell(me, "�Ф�� reset_cmd�C\n");
				tell(me, "�sĶ�P���J "+file+" �����C\n");
				
				if( !query_temp("no_update_msg", me) )
					CHANNEL_D->channel_broadcast("sys", "�sĶ�P���J "+file+" �����C");
		
				destruct(this_object());
				continue;
			}
			
			inv = all_inventory(ob);
			
			inv->move(void_ob);
			
			children = children(file) - ({ ob });
			
			foreach( child in children )
			{
				if( !environment(child) || userp(child) )
				{
					children -= ({ child });
					destruct(child);
					continue;
				}
				if( child->query_shadow_ob() == ob )
					set_temp("shadowing_ob", 1, child);
			}
			
			env = environment(ob);
			
			if( objectp(env) && env->is_maproom() )
				env = query_temp("location", ob);

			destruct(ob);
			
			rm(BINARY_DATA+file[0..<3]+".b");
		}
		
		if( (err = catch( load_object(file) )) )
		{
			tell(me, sprintf( "�o�Ϳ��~�R%s\n", err ));
			
			if( !query_temp("no_update_msg", me) )
				CHANNEL_D->channel_broadcast("sys", "�sĶ "+file+" �o�Ϳ��~�C");
			
			continue;
		}

		if( !largenum )
		{
			tell(me, "�sĶ�P���J "+file+" �����C\n");
			
			if( !query_temp("no_update_msg", me) )
				CHANNEL_D->channel_broadcast("sys", "�sĶ�P���J "+file+" �����C");
		}
		
		if( file[0..5] == "/cmds/" ) reset_command = 1;
		
		ob = find_object(file);
		
		inv->move(ob);
		
		foreach(inv_elem in inv)
			if( inv_elem && inv_elem->is_equipping() )
				ob->equip(inv_elem, &status);
			
		tell(inv, NOR GRN"["HIG"��s"NOR GRN"]"NOR+"�o�̪��ж����Ҥw��s�̷ܳs�������C\n");

		foreach( child in children )
		{
			shadowing = query_temp("shadowing_ob", child);
			childenv = environment(child);
			childloc = query_temp("location", child);
			childamount = query_temp("amount", child);
			childkeep = child->is_keeping();
			childequip = child->is_equipping();
			
			if( childequip )
				childenv->unequip(child, &status);

			if( userp(childenv) )
				tell(environment(child), NOR GRN"["HIG"��s"NOR GRN"]"NOR+pnoun(2, childenv)+"���W��"+child->query_idname()+"�w��s�̷ܳs�������C\n");
			
			// �Y�O�a�Ϫ���, �h�Ѧa�Ϫ���޲z�t�Υh����
			if( MAPOB_D->is_mapob(child) )
			{
				destruct(child);
				continue;
			}
			else
			{
				destruct(child);
			
				child = clone_object(file);
				
				if( childamount )
					set_temp("amount", childamount, child);
	
				if( childkeep )
					child->set_keep(childkeep);
	
				if( shadowing )
					set("shadow_ob", ob, child);
	
				if( childequip )
					childenv->equip(child, &status);
	
				if( childloc )
					child->move(childloc);
				else
					child->move(childenv);
			}
		}
		
		ob->move(env);
	}};
	
	if( largenum )
	{
		tell(me, "�sĶ�P���J "+sizeof(update_list)+" �ɧ����C\n");
		CHANNEL_D->channel_broadcast("sys", "�sĶ�P���J "+sizeof(update_list)+" �ɧ����A��O "+sprintf("%2.5f", timeexpression/1000000.)+" ��C");
		CHANNEL_D->channel_broadcast("news", "��s�����A�`�@��s "+sizeof(update_list)+" ���{���A��O "+sprintf("%2.5f", timeexpression/1000000.)+" ��C");
	}

	set("current_work/file", file, me);
	
	if( reset_command )
		COMMAND_D->do_reset();
	
	me->remove_status(query_temp("last_time_update_status", me));
	me->add_status(set_temp("last_time_update_status", HIW"��"WHT"�s"NOR, me), 30);
}

private void command(object me, string arg)
{
	int option, time;
	string *parse, *update_list, arg2;
	object ob;
	
	if( !is_command() ) return;
	
	if( !arg ) arg = "";
	
	parse = single_quote_parse(arg);
	
	switch( sizeof(parse) )
	{
		case 2:
			arg = parse[0];
			arg2 = parse[1];
			if( parse[1][0] != '-' ) break;
			
			foreach( int a in parse[1][1..] )
			{
				switch(a)
				{
					case 'd':	option |= DEEP_INHERIT_UPDATE;	break;
					default: break;
				}
			}
			break;
		case 0:
			arg = query("current_work/file", me);
			break;
		default:
			arg = parse[0];
			break;
	}

	if( !arg ) arg = query("current_work/file", me);
	if( !arg ) return tell(me, "�п�J�����s�sĶ�θ��J���ɮשΪ���W�١C\n");

	// �ɤO��s�Ҧ� module_room
	if( arg == "all" )
	{
		update_list = allocate(0);

		if( arg2 == "module_room-master" ) 
		{
			foreach( ob in objects((: $1->is_module_room() && query("function", $1)=="front" :)) )
				update_list += ({ base_name(ob) });
		}
		else if( sscanf(arg2, "module_room-%s", arg) == 1 )
		{
			foreach( ob in objects((: $1->is_module_room() && sscanf(base_name($1), "/city/"+$(arg)+"/%*s") :)) )
				update_list += ({ base_name(ob) });
		}
		else 
		switch( arg2 )
		{
			// ��s�Ҧ��з��~�Ӿ�
			case "standard":
				foreach( ob in objects() )
				{
					if( sscanf(base_name(ob), "/std/inherit/standard/%*s") )
					{
						update_list |= deep_inherit_list(ob);
						update_list |= ({ base_name(ob) });
					}
				}
				break;
			case "module_room":
				foreach( ob in objects() )
				{
					if( ob->is_module_room() )
					update_list += ({ base_name(ob) });
				}
				break;
			case "module_object":
				foreach( ob in objects() )
				{
					if( ob->is_module_object() )
					update_list += ({ base_name(ob) });
				}
				break;
			case "module_npc":
				foreach( ob in objects() )
				{
					if( ob->is_module_npc() )
					update_list += ({ base_name(ob) });
				}
				break;
			case "standard_module":
				foreach( ob in objects() )
				{
					if( sscanf(base_name(ob), "/std/module/%*s") )
					{
						update_list |= deep_inherit_list(ob);
						update_list |= ({ base_name(ob) });
					}
				}
				break;
			default:
				return tell(me, "�п�J���T����s���ءC\n");
			
		}
		
		if( sizeof(update_list) )
		{
			CHANNEL_D->channel_broadcast("sys", "�@������}�l�i�������s "+arg2+" ����A�@ "+NUMBER_D->number_symbol(sizeof(update_list))+" �Ӫ���C");
			CHANNEL_D->channel_broadcast("news", "�@������t�ηǳƶi��j�W�ҧ�s�A�w�Ƨ�s "+NUMBER_D->number_symbol(sizeof(update_list))+" �Ӫ���A��s�L�{���N�|���y�Ƭ����ܼƤQ�����C");
			call_out((: CHANNEL_D->channel_broadcast("news", "�}�l�i��t�Χ�s...") :), 59);
			call_out((:update, me, update_list, 1:), 60);
			return;
		}
		else
			return tell(me, "�ثe�å����J����ŦX���ت�����C\n");
	}

	// ��s���⨭��
	if( objectp(ob = find_player(arg)) || arg == "-me" || arg == "-alluser")
	{
		object *ob_arr;
		
		if( ob ) ob_arr = ({ ob });
		else if( arg == "-me" ) ob_arr = ({ me });
		else if( arg == "-alluser" ) ob_arr = filter_array(users(), (: $1->is_user_ob() && environment($1) :));
			
		foreach( object update_ob in ob_arr )
		{
			msg("�@�}"NOR WHT"�¼v"NOR"��M���L$ME������.....�C\n", update_ob, 0, 1);
			call_out( (: call_other, LOGIN_D, "update_player", update_ob :), 1 );
		}
		return;
	}

	
	arg = resolve_path(me, arg);
	
	intact_file_name(ref arg);

	switch(file_size(arg))
	{
		case -1:
			arg = arg[strsrch(arg, "/", -1)+1..];
			if( objectp(ob = find_object(arg) || present(arg, me) || present(arg, environment(me))) )
			{
				string basename = base_name(ob);
				
				intact_file_name(ref basename);
				
				if( file_exists(basename) )
				{
					if( basename[<2..<1] == ".o" )
						basename = basename[0..<3];

					update_list = ({ basename });
				}
			}
			break;
		case -2:
			add_slash(ref arg);
			update_list = allocate(0);
			
			foreach( string file in get_dir(arg) )
				if( file_size(arg+file) > 0 )
					update_list += ({ arg+file });
			break;
		default:
		
			if( arg[<2..<1] == ".o" )
				arg = arg[0..<3];
				
			if( option & DEEP_INHERIT_UPDATE )
				ob = find_object(arg);

			update_list = ({ arg });
	}

	if( objectp(ob) && option & DEEP_INHERIT_UPDATE )
	{
		update_list += deep_inherit_list(ob);

		reverse_array(ref update_list);
	}
	
	if( !arrayp(update_list) )
		return tell(me, "�S���o���ɮשάO����C\n");
		
	time = time_expression 
	{
		evaluate((:update, me, update_list:));
	};
	
	if( time > 1000000 )
		CHANNEL_D->channel_broadcast("news", sprintf("�{����s�ɭP�C�����y(>1 ��)�A�@��O %.2f ��C", time/1000000. ));
}