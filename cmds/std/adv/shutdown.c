/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : shutdown.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : shutdown ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <ansi.h>
#include <mudlib.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	shutdown [-r|-h|-k] [time|now|cancel|force]
HELP;

#define SYNOPSIS		"shutdown [-r|-h|-k] [time|now|cancel|force]\n"

//#define BEEP			"\a"
#define MAX_SHUTDOWN_WAITING	1800

private nosave int shutdowntime;
private nosave string shutdowntype;

private void regular_remove_process()
{
	string systeminfo = SYSTEM_D->query_total_system_info();

	// �x�s���a���
	foreach( object user in users() )
	{
		reset_eval_cost();
		
		if( !objectp(user) || !environment(user) ) continue;
	
		tell(user, "\n"+systeminfo+"\n");

		flush_messages(user);

		if( wizardp(user) )
			LOGOUT_D->save_user_info(user);
		else
			LOGOUT_D->quit(user);
	}

	// �������D�n���
	MONEY_D->save();
	MAP_D->save();
	ESTATE_D->save();
	CITY_D->save_all();
	
	// �ϩҦ����󥿱`����
	foreach(object ob in objects())
	{
		reset_eval_cost();
		ob->remove();
	}
}

void heart_beat()
{
	if( !(shutdowntime % 60) || (shutdowntime <= 20 && !(shutdowntime % 2) ) )
	{
		switch(shutdowntype)
		{
		case "r":
			shout(HIY"\n\n�A�L "+shutdowntime+" ��A"MUD_FULL_NAME HIY"�N�}�l���歫�s�Ұʵ{�ǡC\n\n"NOR);
			break;
		case "h":
			shout(HIY"\n\n�A�L "+shutdowntime+" ��A"MUD_FULL_NAME HIY"�N�}�l���������{�ǡC\n\n"NOR);
			break;
		case "k":
			shout(HIY"\n\n�A�L "+shutdowntime+" ��A"MUD_FULL_NAME HIY"�N�}�l����𰣪��a�{�ǡC\n\n"NOR);
			break;
		}
	}

	if( !shutdowntime-- )
	{
		set_heart_beat(0);
		
		switch(shutdowntype)
		{
			case "r":
				shout(BEEP"\n\n"MUD_FULL_NAME"�}�l���歫�s�Ұʵ{��...\n\n"HIR"�еy�ԴX��ᵥ�ݨt�έ��s�Ұʧ�����A��n�J�C\n\n"NOR);
				regular_remove_process();
				shutdown();
				break;
			case "h":
				shout(BEEP"\n\n"MUD_FULL_NAME"�}�l���������{��...\n\n"HIR"�е��ݨt�αҰʫ�A��n�J�C\n\n"NOR);
				regular_remove_process();
				shutdown(-1);
				break;
			case "k":
				shout(BEEP"\n\n"MUD_FULL_NAME"�}�l����𰣪��a�{��...\n\n"HIR"�еy��X���A��n�J�C\n\n"NOR);
				foreach( object user in users() )
					if( !wizardp(user) ) LOGOUT_D->quit(user);
				break;
			default:
				break;
		}
		
		shutdowntime = 0;
		shutdowntype = 0;
	}
}

void system_auto_reboot()
{
	if( call_stack(1)[1]!=load_object(TIME_D) )
	{
		log_file("command/deny_auto_shutdown", "deny shutdown - "+sprintf("%O\n",call_stack(1)));
		return;
	}
	//tell(find_player("cookys"),sprintf("%O\n",call_stack(1)));
	
	log_file("command/shutdown", "system auto reboot.");
	
	if( query_heart_beat() ) return;
	
	shutdowntime = 300;
	shutdowntype = "r";
	set_heart_beat(10);
	
	shout(BEEP"\n\n"+HIY+MUD_FULL_NAME HIY"�O����ϥιL�q�A�ۦ�ҰʤF���s�Ұʵ{�ǡC\n\n"NOR);
}


private void command(object me, string arg)
{
	int itime;
	string type, time;
	
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, SYNOPSIS);
	
	arg = remove_fringe_blanks(lower_case(arg));
	
	sscanf( arg, "-%s %s", type, time );
	
	if( !type || !time ) 
		return tell(me, SYNOPSIS);
	
	log_file("command/shutdown", me->query_idname()+" shutdown -"+type+" "+time);
	
	switch(time)
	{
		case "force":
			switch(type)
			{
			case "r":
				shout(BEEP"\n\n"+HIR+me->query_idname()+HIR"�j��Ҩt�ΡA�еy��X��ᵥ�ݨt�έ��s�Ұʧ�����A��n�J�C\n\n"NOR);
				shutdown();
				break;
			case "h":
				shout(BEEP"\n\n"+HIR+me->query_idname()+HIR"�j�������t�ΡA�е��ݨt�αҰʫ�A��n�J�C\n\n"NOR);
				shutdown(-1);
				break;
			case "k":
				shout(BEEP"\n\n"+HIR+me->query_idname()+HIR"�j��𰣩Ҧ����a�A�еy��X���A��n�J�C\n\n"NOR);
				foreach( object user in users() )
					if( !wizardp(user) ) destruct(user);
				break;
			default:
				return tell(me, SYNOPSIS);
			}
			break;
			
		case "now":
			switch(type)
			{
			case "r":
				shout(BEEP"\n\n"+HIY+me->query_idname()+HIY"����F"MUD_FULL_NAME HIY"���ߧY���s�Ұʵ{��...\n\n"HIR"�еy��X��ᵥ�ݨt�έ��s�Ұʧ�����A��n�J�C\n\n"NOR);
				regular_remove_process();
				shutdown();
				break;
			case "h":
				shout(BEEP"\n\n"+HIY+me->query_idname()+HIY"����F"MUD_FULL_NAME HIY"���ߧY�����{��...\n\n"HIR"�е��ݨt�αҰʫ�A��n�J�C\n\n"NOR);
				regular_remove_process();
				shutdown(-1);
				break;
			case "k":
				shout(BEEP"\n\n"+HIY+me->query_idname()+HIY"����F"MUD_FULL_NAME HIY"���ߧY�𰣪��a�{��...\n\n"HIR"�еy��X���A��n�J�C\n\n"NOR);
				regular_remove_process();
				foreach( object user in users() )
					if( !wizardp(user) ) destruct(user);
				break;
			default:
				return tell(me, SYNOPSIS);
			}
			break;
		case "cancel":
			if( query_heart_beat() )
			{
				set_heart_beat(0);
				
				switch(shutdowntype)
				{
					case "r":
						shout(BEEP"\n\n"+HIY+me->query_idname()+HIY"�����F"MUD_FULL_NAME HIY"���w�����s�Ұʵ{�ǡC\n\n"NOR);
						break;
					case "h":
						shout(BEEP"\n\n"+HIY+me->query_idname()+HIY"�����F"MUD_FULL_NAME HIY"���w�������{�ǡC\n\n"NOR);
						break;
					case "k":
						shout(BEEP"\n\n"+HIY+me->query_idname()+HIY"�����F"MUD_FULL_NAME HIY"���w���𰣪��a�{�ǡC\n\n"NOR);
						break;
				}
				
				shutdowntime = 0;
				shutdowntype = 0;
				return;
			}
			else
				return tell(me, "�ثe�èS������w�� shutdown �{�ǡC\n");
			break;
		default:
			if( sscanf(time, "%d", itime) )
			{
				if( itime > MAX_SHUTDOWN_WAITING )
					return tell(me, "�̦h�u��]�w "+MAX_SHUTDOWN_WAITING+" ���w���ɶ��C\n");
				
				if( type != "r" && type != "h" && type != "k" )
					return tell(me, SYNOPSIS);
					
				switch(type)
				{
					case "r":
						shout(BEEP"\n\n"+HIY+me->query_idname()+HIY"����F"MUD_FULL_NAME HIY"�� "+itime+" ��w�����s�Ұʵ{�ǡC\n\n"NOR);
						break;
					case "h":
						shout(BEEP"\n\n"+HIY+me->query_idname()+HIY"����F"MUD_FULL_NAME HIY"�� "+itime+" ��w�������{�ǡC\n\n"NOR);
						break;
					case "k":
						shout(BEEP"\n\n"+HIY+me->query_idname()+HIY"����F"MUD_FULL_NAME HIY"�� "+itime+" ��w���𰣪��a�{�ǡC\n\n"NOR);
						break;
					default:
						return tell(me, SYNOPSIS);
				}
				
				shutdowntime = itime;
				shutdowntype = type;
				
				set_heart_beat(10);
			}
			else
				return tell(me, SYNOPSIS);
	}
}