/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : give.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-01-01
 * Note   : give ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <object.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�N���~�Ϊ��������Y�H

give [�Y�H] [�ƶq|all] [���~] [�Ƨ�]		- �N�Y�ƶq�αƧǪ��Y���~�����Y�H
give [�ƶq|all] [���~] [�Ƨ�] to [�Y�H]		- �N�Y�ƶq�αƧǪ��Y���~�����Y�H
HELP;


private void do_give(object me, object target, string arg)
{
	int mass;
	object ob;
	string amount;

	if( target == me )
		return tell(me, "�F��w�g�b"+pnoun(2, me)+"�����W�F�I\n");
	
	// Delay ������U���O
	if( me->is_delaying() )
		return tell(me,	me->query_delay_msg());

	if( sscanf(arg, "$%s %s", arg, amount) == 2 )
	{
		if( me->is_npc() )
			return me->do_command("say �ڵL�k���������C\n");

		if( !MONEY_D->money_unit_exist(arg) )
		{
			if( !arg || !arg[0] )
				return tell(me, pnoun(2, me)+"�n�����@�سf���H\n");

			tell(me, "�èS�� "+arg+" �o�سf���C\n");
			
			return;
		}
		
		amount = big_number_check(amount);
		if( !amount )
			return tell(me, "�п�J���T���Ʀr�C\n");
		
		if( count(amount, "<=", 0) )
			return tell(me, "���B�����n�j��s�C\n");
		
		if( !me->spend_money(arg, amount) )
			return tell(me, pnoun(2, me)+"���W�S������h�� $"+upper_case(arg)+" ���C\n");
			
		target->earn_money(arg, amount);
		
		if( !wizardp(target) )
		{
			log_file("command/give", me->query_idname()+"��"+target->query_idname()+" "+money(arg, amount));
			CHANNEL_D->channel_broadcast("nch", me->query_idname()+"��"+target->query_idname()+" "+money(arg, amount));
		}
		
		if( target->is_npc() )
			call_out((: call_other :), 0, target, "reply_get_money", me, arg, amount);

		return msg("$ME����$YOU "HIY+money(arg, amount)+NOR"�C\n", me, target, 1);
	}
		
	if( sscanf(arg, "%s %s", amount, arg) == 2 )
	{
		if( amount == "all" ) ;
		// �p�G amount �ä��O�Ʀr���
		else if( !big_number_check(amount) )
		{
			arg = amount + " " + arg;
			amount = "1";	
		}	
		else if( count(amount, "<", 1) ) amount = "1";
	}
	else amount = "1";
	
	if( objectp(ob = present(arg, me)) )
	{
		object base_ob = load_object(base_name(ob));
		string ob_idname = ob->query_idname(), unit = query("unit", ob)||"��";
		
		if( amount != "all" && count(amount, ">", copy(query_temp("amount", ob))||1 ) )
			return tell(me, pnoun(2, me)+"���W�S������h"+unit+ob_idname+"�C\n");
		
		if( amount == "all" )
			amount = query_temp("amount", ob)||"1";
		
		if( ob->is_keeping() )
			return tell(me, pnoun(2, me)+"�������Ѱ�"+ob->query_idname()+"���O�d(keep)���A�C\n");

		if( query("flag/no_give", ob) )
			return tell(me, ob->query_idname()+"�����\����C\n");

		if( target->get_object(ob, amount) )
			msg("$ME��"+QUANTITY_D->obj(ob || base_ob, amount)+"����$YOU�C\n", me, target, 1);
		else
			msg("$ME�Q��"+QUANTITY_D->obj(ob || base_ob, amount)+"����$YOU�A����$YOU�Ө����G�ӭ��F�C\n", me, target, 0);
		
		return;
	}
	
	if( lower_case(arg) == "all" )
	{		
		object *all_ob = all_inventory(me);
		
		if( sizeof(all_ob) )
		{
			int longest, too_heavy, loading, max_loading;
			string msg="", str;
		
			max_loading = target->query_max_loading();
			
			foreach(ob in all_ob)
			{
				mass = query("mass", ob);
				
				if( mass == -1 || ob->is_keeping() || query("flag/no_give", ob) ) continue;
				
				amount = copy(query_temp("amount",ob)) || "1";
				
				loading = max_loading - target->query_loading();
				
				if( count(loading, "<", count(amount,"*",mass) ) )
				{
					if( loading < mass ) continue;
					
					str = (msg==""?"�u ":"   ")+QUANTITY_D->obj(ob, loading/mass, 1)+"\n";
					ob->move(target, loading/mass);
					too_heavy = 1;
				}
				else
				{
					str = (msg==""?"�u ":"   ")+QUANTITY_D->obj(ob, amount, 1)+"\n";
					ob->move(target, amount);
				}
				
				
				msg += str;	
				
				if( noansi_strlen(str) > longest ) longest = noansi_strlen(str);
			}
			
			if( msg == "" )
				return msg("$ME�յ۵�$YOU�Ҧ������~�A��$YOU���W�t���ӭ��F�A���򳣮����ʤF�C\n", me, target, 0);
			
			msg = msg[0..<2]+repeat_string(" ", longest-noansi_strlen(str)+1 );
			
			msg("$ME�⨭�W�൹���F������F$YOU�C\n"+msg+"�v\n\n", me, target, 1);
			
			if( too_heavy )
				msg("�ѤU���F����G�ӭ�$YOU�����ʤF�C\n", me, target, 0);
			
			return;
		}
		return tell(me, pnoun(2, me)+"���W�S�����󪫫~�i�H��"+target->query_idname()+"�C\n");
	}
	return tell(me, pnoun(2, me)+"���W�S�� "+arg+" �o�󪫫~�C\n");
}

private void confirm_give(string input, object me, object target, string arg)
{
	if( input && lower_case(input)[0] == 'y' )
		do_give(me, target, arg);
	else
		tell(me, pnoun(2, me)+"�����N���~����"+target->query_idname()+"���ʧ@�C\n");
		
	me->finish_input();
}

private void do_command(object me, string arg)
{
	object target;
	string target_name;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n���֤���F��H\n");

	if( sscanf(arg, "%s to %s", arg, target_name) != 2 )
		sscanf(arg, "%s %s", target_name, arg);
	
	if( !arg || !target_name )
		return tell(me, pnoun(2, me)+"�Q�n���֤���F��H\n");

	if( !objectp(target = present(target_name, environment(me))) )
		return tell(me, "�o�̨S�� "+target_name+" �o�ӤH�C\n");

	if( !target->is_living() )
		return tell(me, pnoun(2, me)+"���൹"+target->query_idname()+"����F��C\n");
		
	if( userp(target) )
		do_give(me, target, arg);
	else
	{
		if( query("boss", target) == me->query_id(1) )
			tell(me, HIY"�`�N�G�Y�O�������u�����N�L�k�A���^\n"NOR+pnoun(2, me)+"�T�w�n�N���~����"+target->query_idname()+"��(Yes/No)�H\n");
		else
			tell(me, HIY"�`�N�G���������~�Ϊ����N�L�k�A���^\n"NOR+pnoun(2, me)+"�T�w�n�N���~����"+target->query_idname()+"��(Yes/No)�H\n");

		input_to((: confirm_give :), me, target, arg);
	}
}