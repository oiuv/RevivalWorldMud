/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : get.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-10-10
 * Note   : get ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <location.h>

inherit COMMAND;

string help = @HELP
�N���~�Ϊ����߬B�_��

get [�ƶq|all] [���~] [�Ƨ�]	- �N�Y�ƶq�αƧǪ��Y���~�߬B�_��

HELP;

private void do_command(object me, string arg)
{
	int mass, from_environment=1;
	object ob, target, env;
	string from, msg="", amount;

	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say ����Q�n�ھߤ���F��H\n");
		else
			return tell(me, pnoun(2, me)+"�Q�n�ߤ���F��H\n");
	}
	
	// Delay ������U���O
	if( me->is_delaying() )
		return tell(me,	me->query_delay_msg());

	env = environment(me);
	
	if( sscanf(arg, "%s from %s", arg, from) == 2 )
	{
		if( !objectp(target = present(from)) )
			return tell(me, "�o�̨S�� "+from+" �o�ӤH�Ϊ��~�C\n");
		else
			from_environment = 0;
	}
	else target = env;

	if( sscanf(arg, "$%s %s", arg, amount) == 2 )
	{
		if( me->is_npc() )
			return me->do_command("say �ڵL�k�߬B�����C\n");

		if( !MONEY_D->money_unit_exist(arg) )
			return tell(me, "�èS�� "+arg+" �o�سf���C\n");
		
		amount = big_number_check(amount);
		if( !amount )
			return tell(me, "�п�J���T���Ʀr�C\n");
		
		if( count(amount, "<=", 0) )
			return tell(me, "���B�����n�j��s�C\n");
		
		if( !objectp(ob = present(arg, target)) )
			return tell(me, "�o�̨èS�� "+arg+" �o�سf���C\n");

		if( count(amount, ">", query("money", ob)) )
			return tell(me, "�o�̨èS���o��h�� "+arg+" �����C\n");
		
		ob->move(me, amount);
		
		log_file("command/get", me->query_idname()+"�߰_ "+money(arg, amount));
		CHANNEL_D->channel_broadcast("nch", me->query_idname()+"�߰_ "+money(arg, amount));

		return msg("$ME�� "HIY+money(arg, amount)+" "NOR+(from_environment?"":"�q"+target->query_idname()+"��")+"�ߤF�_�ӡC\n", me,0,1);
	}
	
	if( sscanf(arg, "%s %s", amount, arg) == 2 )
	{
		if( amount == "all") ;
		
		// �p�G amount �ä��O�Ʀr���
		else if( !big_number_check(amount) )
		{
			arg = amount + " " + arg;
			amount = "1";	
		}	
		else if( count(amount, "<", 1) ) amount = "1";
	}
	else amount = "1";
	
	if( objectp(ob = present(arg, target)) )
	{
		object base_ob = load_object(base_name(ob));

		if( me == ob )
			return tell(me, pnoun(2, me)+"�p�G�u���i�H��ۤv���_�ӡA�ЦܧŮv�j�U����Q�U�������C\n");
		
		if( me == target )
			return tell(me, ob->query_idname()+"�w�g�b"+pnoun(2, me)+"���W�F�I�I�C\n");
			
		if( target->is_living() )
			return msg("$ME�b$YOU���W�F�N��N�A�յۧ�"+QUANTITY_D->obj(ob, amount)+"�������I\n", me, target, 1);
		
		if( ob->is_living() )
			return msg("$ME�b$YOU���W�F�N��N�A�յۧ��I�O�I��$YOU�����_�ӡI�I\n", me, ob,1);
		
		if( ob->is_money() )
		{
			if( amount != "all" && count(amount, ">", query("money", ob)) )
				return tell(me, "�o�̪����S������h�C\n");
			
			if( amount == "all" )
				amount = query("money", ob);
	
			msg("$ME�� "HIY+money(query("unit", ob), amount)+" "NOR+(from_environment?"":"�q"+target->query_idname()+"��")+"�ߤF�_�ӡC\n", me,0,1);
			
			log_file("command/get", me->query_idname()+"�߰_�F "+money(query("unit", ob), amount));
			CHANNEL_D->channel_broadcast("nch", me->query_idname()+"�߰_�F "+money(query("unit", ob), amount));
			ob->move(me, amount);
			
			return;
		}
		
		if( amount != "all" && count(amount,">", query_temp("amount",ob)||1 ) )
		{
			if( me->is_npc() )
				return me->do_command("say "+"�o�̪�"+ob->query_idname()+"�S������h�ӡC\n");
			else
				return tell(me, "�o�̪�"+ob->query_idname()+"�S������h�ӡC\n");
		}
		
		if( query("mass", ob) == -1 )
		{
			msg("$ME�H��N"+QUANTITY_D->obj(ob, amount)+"�@���|�_�A�u��$ME�����a���F�@�y�u�ګ��I�n���ӭ��F...�v�C\n",me,0,1);
			return me->faint();
		}
		
		if( amount == "all" )
		{
			amount = ((me->query_max_loading() - me->query_loading()) / (query("mass", ob)||1)) + "";
			
			if( count(amount, ">", query_temp("amount", ob)||1) )
				amount = query_temp("amount", ob)||"1";
				
			if( count(amount, "==", 0) )
				return tell(me, pnoun(2, me)+"������"+ob->query_idname()+"�F�C\n");
		}
		
		if( me->get_object(ob, amount) )
			return msg("$ME��"+QUANTITY_D->obj(ob || base_ob, amount)+(from_environment?"":"�q"+target->query_idname()+"��")+"�ߤF�_�ӡC\n", me,0,1);
		else
			return msg("$ME��"+QUANTITY_D->obj(ob || base_ob, amount)+(from_environment?"":"�q"+target->query_idname()+"��")+"���F�_�ӡA�uť��j�L�@�n�ذաA�j�L�����_�F�I�S������C\n",me,0,1);
	}
	
	if( lower_case(arg) == "all" )
	{
		object *all_ob = filter_array(present_objects(me), (: !$1->is_living() && query("mass",$1) >= 0 :));
		
		if( userp(target) )
			return msg("$ME�b$YOU���W�F�N��N�A�յۧ�$YOU�Ҧ����F����������I\n",me,target,1);
		
		if( sizeof(all_ob) )
		{
			int too_heavy, loading, max_loading;
			
			max_loading = me->query_max_loading();
			
			foreach(ob in all_ob)
			{
				mass = query("mass",ob);

				if( mass == -1 ) continue;
		
				if( ob->is_money() )
				{
					msg += HIR"�D"NOR+ob->query_idname()+NOR"\n";
					
					log_file("command/get", me->query_idname()+"�߰_�F"+QUANTITY_D->money_info(query("unit", ob), query("money", ob)));
					CHANNEL_D->channel_broadcast("nch", me->query_idname()+"�߰_�F"+QUANTITY_D->money_info(query("unit", ob), query("money", ob)));
					ob->move(me);	
					continue;
				}
				
				amount = copy(query_temp("amount",ob)) || "1";
				
				loading = max_loading - me->query_loading();
				
				if( count(loading, "<", count(amount,"*",mass)) )
				{
					if( loading < mass ) continue;
					
					msg += HIR"�D"NOR+QUANTITY_D->obj(ob, loading/mass, 1)+"\n";
					
					ob->move(me, loading/mass);

					too_heavy = 1;
				}
				else
				{
					msg += HIR"�D"NOR+remove_fringe_blanks(QUANTITY_D->obj(ob, amount))+"\n";
					ob->move(me , amount);
				}
			}
			
			if( msg == "" )
			{
				if( me->is_npc() )
					return me->do_command("say �ھߤ��_�ӥ��󪺪F��C\n");
				else
					return tell(me, pnoun(2, me)+"�ߤ��_�ӥ��󪺪F��C\n");
			}
			
			msg("$ME��"+(from_environment?"�a�W�߱o�ʪ��F�賣�ߤF�_��":"$YOU���W�ள���F��q�q���F�X��")+"�C\n"+msg+"\n", me, target,1);
			
			if( too_heavy ) tell(me, "�ѤU���F��ӭ������ʤF�C\n\n");
			
			return;
		}
		return tell(me, (from_environment?"�o�Ӧa��":target->query_idname())+"�S�����󪫫~�i�H�߭C...�C\n");
	}
	
	if( me->is_npc() )
		return me->do_command("say �o�̨S�� "+arg+" �o�󪫫~�C\n");
	else
		return tell(me, "�o�̨S�� "+arg+" �o�󪫫~�C\n");
}