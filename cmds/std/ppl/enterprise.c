/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : enterprise.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : ���~��T
 * Update :
 *  o 2003-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>

inherit COMMAND;

#define REGISTER_ENTERPRISE_MONEY	"10000000"

string help = @HELP
���~�޲z���O�A�Ϊk�p�U�G
  enterprise			- �C�X�ثe�Ҧ����~��T
  enterprise ���~�s��		- �d�߬Y���~���ԲӸ��
  enterprise register �W��	- ��O $RW 10,000,000 �n�O���U��ڥ��~����(�W�٥i�ϥΦ�m)
  enterprise unregister �W��	- ���P�Ѵ���ڥ��~����(�`�����O)
  enterprise invite 'ID'	- �ܽШ�L���a�i�J���~����(�`�����O)
  enterprise kickout 'ID'	- �N�Y����~������X���~����(�`�����O)
  enterprise join		- �����ܽжi�J���~����
  enterprise leave		- ���}���~��´
HELP;

private void do_command(object me, string arg)
{
	int strlen, i;
	string enterprise_color_id, enterprise_id;
	string inviteid, kickoutid;
	string myid = me->query_id(1);
	string myenterprise = query("enterprise", me);
	string *enterprise_stockvalue_sort;
	if( !arg )
	{
		
		string msg = "";
		msg += "   ���~�W��            �`��           ����    �ѻ�     �����겣�`��\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		
		enterprise_stockvalue_sort = sort_array(ENTERPRISE_D->query_all_enterprises(), (: ENTERPRISE_D->query_enterprise_info($2, "stockvalue") > ENTERPRISE_D->query_enterprise_info($1, "stockvalue") ? 1 : -1 :));
		
		foreach(enterprise_id in ENTERPRISE_D->query_all_enterprises())
		{	
			msg += sprintf("%-3s%-20s"HIM"%-15s"HIG"%-4d"HIC"%8.2f"NOR RED"("HIR"%-2d"NOR RED") "HIY"%s\n"NOR
				,++i+"."
				,ENTERPRISE_D->query_enterprise_color_id(enterprise_id)
				,capitalize(ENTERPRISE_D->query_enterprise_info(enterprise_id, "president"))
				,sizeof(ENTERPRISE_D->query_enterprise_info(enterprise_id, "member"))
				,ENTERPRISE_D->query_enterprise_info(enterprise_id, "stockvalue")+0.
				,member_array(enterprise_id, enterprise_stockvalue_sort)+1
				,money("RW", ENTERPRISE_D->query_enterprise_allmembers_assets(enterprise_id)));
		}
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += "��J enterprise '���~�s��' �d�߲Ӹ`���\n";
		return tell(me, msg);	
	}

	if( (i = to_int(arg)) )
	{
		array estates;
		string president, msg, membermsg;
		string *allenterprises = ENTERPRISE_D->query_all_enterprises();
		
		if( i<=0 || i>sizeof(allenterprises) )
			return tell(me, "��J�s�����~�C\n");
		
		arg = allenterprises[i-1];

		estates = ESTATE_D->query_player_estate("ENTERPRISE/"+arg);
		president = ENTERPRISE_D->query_president(arg);
		msg = "�u"+ENTERPRISE_D->query_enterprise_color_id(arg)+"�v���~���Ϊ��ԲӸ�Ʀp�U�G\n";
		membermsg = "";

		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += "�`��     "HIW+(president ? capitalize(president) : "�L")+NOR"\n";
		msg += "���߮ɶ� "HIC+TIME_D->replace_ctime(ENTERPRISE_D->query_enterprise_info(arg, "registertime"))[0..9]+NOR+"\n";
		msg += "���~�겣 "HIY+money("RW", ENTERPRISE_D->query_enterprise_assets(arg))+"\n"NOR;
		msg += "�i�θ�� "HIY+money("RW", ENTERPRISE_D->query_money(arg))+"\n"NOR;
		msg += "���~����\n"HIG"  �W��         ����        ���~���            �ӤH�겣            �[�J�ɶ�\n"NOR;

		foreach(string memberid, mapping data in ENTERPRISE_D->query_enterprise_info(arg, "member"))
		{
			if( memberid == president )
				membermsg = sprintf("  %-13s%-12s%-20s%-20s%s\n"NOR, capitalize(memberid), CITY_D->query_city_id(CITY_D->query_player_city(memberid)), NUMBER_D->number_symbol(data["invest"]), NUMBER_D->number_symbol(MONEY_D->query_lazy_assets(memberid)), TIME_D->replace_ctime(data["jointime"])[0..9]) + membermsg;
			else
				membermsg += sprintf("  %-13s%-12s%-20s%-20s%s\n"NOR, capitalize(memberid), CITY_D->query_city_id(CITY_D->query_player_city(memberid)), NUMBER_D->number_symbol(data["invest"]), NUMBER_D->number_symbol(MONEY_D->query_lazy_assets(memberid)), TIME_D->replace_ctime(data["jointime"])[0..9] );
		}

		msg += membermsg;
		msg += "\n"+("/cmds/std/ppl/estate"->show_estate("ENTERPRISE/"+arg, estates, ENTERPRISE_D->query_enterprise_color_id(arg), 1));

		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		return tell(me, msg);
	}
	
	if( sscanf(arg, "register %s", enterprise_color_id) )
	{
		strlen = noansi_strlen(enterprise_color_id);
		if( strlen < 4 || strlen > 14 )
			return tell(me, "���~�W�٦r�ƥ������� 4 - 14 �Ӧr���C\n");

		enterprise_id = replace_string(remove_ansi(enterprise_color_id), "\n", "");
		enterprise_color_id = kill_repeat_ansi(replace_string(enterprise_color_id, "\n", "")+NOR);
		
		if( ENTERPRISE_D->enterprise_exists(enterprise_id) )
			return tell(me, ENTERPRISE_D->query_enterprise_color_id(enterprise_id)+"�w�g�s�b�A����ϥάۦP�����~�W�١C\n");
		
		if( ENTERPRISE_D->in_enterprise(myid) )
			return tell(me, pnoun(2, me)+"�w�g�[�J"+ENTERPRISE_D->query_enterprise_color_id(ENTERPRISE_D->in_enterprise(myid))+"�A�L�k�A�t�~�ӽЦ��ߥ��~���β�´�C\n");
	
		if( !me->spend_money(MONEY_D->query_default_money_unit(), REGISTER_ENTERPRISE_MONEY) )
			return tell(me, "�n�O���U�ݤ�I $"+MONEY_D->query_default_money_unit()+" "+NUMBER_D->number_symbol(REGISTER_ENTERPRISE_MONEY)+"�C\n");
	
		ENTERPRISE_D->register_enterprise(enterprise_id, enterprise_color_id, myid);
	
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"�n�O���ߤF��ڥ��~���Ρu"+ENTERPRISE_D->query_enterprise_color_id(enterprise_id)+"�v�C");
		
		set("enterprise", enterprise_id, me);
		
		me->save();
	
		return tell(me, pnoun(2, me)+"��O $"+MONEY_D->query_default_money_unit()+" "+NUMBER_D->number_symbol(REGISTER_ENTERPRISE_MONEY)+" �n�O���ߤF��کʥ��~���Ρu"+ENTERPRISE_D->query_enterprise_color_id(enterprise_id)+"�v�C\n");
	}
	else if( sscanf(arg, "unregister %s", enterprise_color_id) )
	{
		enterprise_id = replace_string(remove_ansi(enterprise_color_id), "\n", "");
		
		if( !ENTERPRISE_D->enterprise_exists(enterprise_id) )
			return tell(me, "�o�ӥ��~�ä��s�b�C\n");
			
		if( !wizardp(me) && !ENTERPRISE_D->is_president(enterprise_id, myid) )
			return tell(me, pnoun(2, me)+"�ä��O"+ENTERPRISE_D->query_enterprise_color_id(enterprise_id)+"���`���C\n");
		
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"�ť��Ѵ���ڥ��~���Ρu"+ENTERPRISE_D->query_enterprise_color_id(enterprise_id)+"�v�C");
		tell(me, pnoun(2, me)+"�ť��Ѵ���کʥ��~���Ρu"+ENTERPRISE_D->query_enterprise_color_id(enterprise_id)+"�v�C\n");
		
		ENTERPRISE_D->unregister_enterprise(enterprise_id);
		return;
	}
	else if( sscanf(arg, "invite %s", inviteid) )
	{
		object target;
		
		if( !objectp(target = find_player(inviteid)) )
			return tell(me, "�ثe�u�W�èS�� "+inviteid+" �o�쪱�a�C\n");
		
		if( !myenterprise )
			return tell(me, pnoun(2, me)+"�å��զ�������~�C\n");

		if( !ENTERPRISE_D->is_president(myenterprise, myid) )
			return tell(me, pnoun(2, me)+"�ä��O"+ENTERPRISE_D->query_enterprise_color_id(myenterprise)+"���`���A�L�k�ܽЦ����C\n");

		if( ENTERPRISE_D->in_enterprise(inviteid) )
			return tell(me, target->query_idname()+"�w�g�O"+ENTERPRISE_D->query_enterprise_color_id(ENTERPRISE_D->in_enterprise(inviteid))+"�������C\n");
		
		if( !same_environment(me, target) )
			return tell(me, target->query_idname()+"�����Ө즹�a�~���ܽСC\n");

		CHANNEL_D->channel_broadcast("ent", me->query_idname()+"�ܽ�"+target->query_idname()+"�������~���Ρu"+ENTERPRISE_D->query_enterprise_color_id(myenterprise)+"�v�������C", me);
		
		msg("\n$ME�ܽ�$YOU������ڥ��~���Ρu"+ENTERPRISE_D->query_enterprise_color_id(myenterprise)+"�v���@���C\n", me, target, 1);
		set_temp("enterprise_invite", myenterprise, target);
		tell(me, "���b����"+target->query_idname()+"���^���C\n");
		tell(target, "�Y�@�N�[�J�����~�п�J enterprise join�C\n");
		return;
	}
	else if( sscanf(arg, "kickout %s", kickoutid) )
	{
		object target = load_user(kickoutid);

		if( !myenterprise )
			return tell(me, pnoun(2, me)+"�å��զ�������~�C\n");

		if( !ENTERPRISE_D->is_president(myenterprise, myid) )
			return tell(me, pnoun(2, me)+"�ä��O"+ENTERPRISE_D->query_enterprise_color_id(myenterprise)+"���`���A�L�k�ܽЦ����C\n");

		if( !ENTERPRISE_D->in_enterprise(kickoutid) )
			return tell(me, target->query_idname()+"�쥻�N���O"+ENTERPRISE_D->query_enterprise_color_id(ENTERPRISE_D->in_enterprise(inviteid))+"�������C\n");
		
		CHANNEL_D->channel_broadcast("ent", me->query_idname()+"�N"+target->query_idname()+"��X���~���ΡC", me);
		
		tell(target, pnoun(2, target)+"�Q���~�`��"+me->query_idname()+"�}���F�������C\n");
		ENTERPRISE_D->unregister_member(kickoutid);

		delete("enterprise", target);
		target->save();
		
		if( !userp(target) )
			destruct(target);

		return;
	}
	else if( arg == "leave" )
	{
		if( !myenterprise )
			return tell(me, pnoun(2, me)+"�å��[�J������~�C\n");
		
		if( ENTERPRISE_D->is_president(myenterprise, myid) )
			return tell(me, pnoun(2, me)+"�O"+ENTERPRISE_D->query_enterprise_color_id(myenterprise)+"���`���A�L�k�զ����}���~�C\n");

		CHANNEL_D->channel_broadcast("ent", me->query_idname()+"���P�F���~���Ρu"+ENTERPRISE_D->query_enterprise_color_id(myenterprise)+"�v���������C", me);
		tell(me, pnoun(2, me)+"���P�F���~���Ρu"+ENTERPRISE_D->query_enterprise_color_id(myenterprise)+"�v���������C\n");
		delete("enterprise", me);
		me->save();
		ENTERPRISE_D->unregister_member(myid);
		return;
	}
	else if( arg == "join" )
	{
		myenterprise = query_temp("enterprise_invite", me);
		
		if( !myenterprise )
			return tell(me, pnoun(2, me)+"�|�����������~���ܽСC\n");
		
		delete_temp("enterprise_invite", me);
			
		if( ENTERPRISE_D->in_enterprise(myid) )
			return tell(me, pnoun(2, me)+"�w�g�O"+ENTERPRISE_D->query_enterprise_color_id(ENTERPRISE_D->in_enterprise(myid))+"�������C\n");

		if( !ENTERPRISE_D->enterprise_exists(myenterprise) )
			return tell(me, pnoun(2, me)+"���[�J�����~�w�g�Ѵ��C\n");
		
		set("enterprise", myenterprise, me);
		me->save();

		ENTERPRISE_D->register_member(myenterprise, myid);
		CHANNEL_D->channel_broadcast("ent", me->query_idname()+"�����ܽЦ������~���Ρu"+ENTERPRISE_D->query_enterprise_color_id(myenterprise)+"�v�������C", me);
		tell(me, pnoun(2, me)+"�����ܽЦ������~���Ρu"+ENTERPRISE_D->query_enterprise_color_id(myenterprise)+"�v�������C\n");

		return;
	}

	return tell(me, "�п�J���T�����~�޲z���O�榡�A�аѦ� help enterprise�C\n");
}