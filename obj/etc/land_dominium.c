/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : land_dominium.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-1-21
 * Note   : �g�a�Ҧ��v��
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <map.h>
#include <ansi.h>
#include <inherit.h>
#include <daemon.h>
#include <message.h>
#include <location.h>
#include <roommodule.h>

inherit STANDARD_OBJECT;

string DECLARATION = @TEXT


	             [1;37;43m �g �a �� �� �v �� [m
    �Q�γo�i�g�a�Ҧ��v���i�ۥѻ{��@���L�H�֦����g�a�A�{���
���g�a�K��%^GENDER%^�Ҿ֦��A�{��g�a��%^GENDER%^�K�i�H�}�l�Q�Φ��g�a�ӳгy�]
�ݩ�ۤv���]�I�A��ܤ@����a�� claim �Y�i�{��C      [1;31m�~�w��[m
                                                     [1;31m�x�֢x[m
    �֦��̡G%^ID%^                             [1;31m�x��x[m
    ���Ĵ����G�û�����                               [1;31m���w��[m
    

TEXT;

void do_claim(object me, string arg);

mapping actions = 	
([
	"claim"			:(: do_claim :),
]);

void do_claim(object me, string arg)
{
	object env = environment(me);
	mapping data;
	string owner;
	array loc = query_temp("location", me);
	
	if( !(owner = query_temp("owner")) )
		return tell(me, "�o�O�@�i�o�󪺤g�a�Ҧ��v���A"+pnoun(2, me)+"�L�k�ϥΡC\n", CMDMSG);
		
	if( owner != me->query_id(1) )
		return tell(me, "�o�i�g�a�Ҧ��v���O "+capitalize(owner)+" ���A"+pnoun(2, me)+"�L�v�ϥΡC\n", CMDMSG);
	
	if( !arrayp(loc) || !env || !env->is_maproom() ) 
		return tell(me, pnoun(2, me)+"�L�k�{��o���g�a�C\n", CMDMSG);

	if( !CITY_D->is_city_location(loc) )
		return tell(me, pnoun(2, me)+"�u��b�����ϰ줺�{��g�a�C\n", CMDMSG);

	if( loc[CITY] != query("city", me) )
		return tell(me, pnoun(2, me)+"�u��b�ۤv�������̻{��g�a�C\n", CMDMSG);

	if( !(CITY_D->query_coor_data(loc, "region") & AGRICULTURE_REGION) )
		return tell(me, pnoun(2, me)+"�u��{��w�g�g�L���F���W�����A�~�ϡC\n", CMDMSG);
	
	if( !mapp(data = CITY_D->query_coor_data(loc)) )
		return tell(me, "�g�a��ƿ��~�A�гq���Ův�B�z�C\n", CMDMSG);
	
	if( nullp(owner = data["owner"]) && nullp(data["ownership"]) )
	{
		if( CITY_D->set_coor_data(loc, "owner", me->query_id(1)) )
		{
			CITY_D->set_coor_icon(loc, BWHT"��"NOR);
			CITY_D->set_coor_data(loc, "claimed", 1);
			tell(me, pnoun(2, me)+"�Q�Τg�a�Ҧ��v���{��F�o���g�a�C\n", CMDMSG);
			
			// �]�w�Цa����T
			ESTATE_D->set_land_estate(me->query_id(1), loc);

			delete("flag/land_dominium", me);
			me->save();
			destruct(this_object());
		}
		else if( owner == me->query_id(1) )
			return tell(me, "�o���a�w�g�O"+pnoun(2, me)+"���F�C\n", CMDMSG);
	}
	else tell(me, "�o���a�w�g�O�O�H���F�A"+pnoun(2, me)+"�L�k�{��C\n", CMDMSG);
}

// �b�֤�W�N�O�֪�
void set_owner()
{
	object env = environment(this_object());

	if( env && userp(env) )
	{
		set_idname(env->query_id()+"'s land dominium", env->query_name()+"���g�a�Ҧ��v��");
		set("long", terminal_colour(DECLARATION, (["ID":sprintf("%-12s",ansi_capitalize(env->query_id())), "GENDER":(query("gender", env)=="�k"?"�A":"�p")])));
		set_temp("owner", env->query_id(1));
	}
}

void create()
{
	set_idname("droped land dominium", "�o�󪺤g�a�Ҧ��v��");
	set_temp("keep", 2);
	set("long", terminal_colour(DECLARATION, (["ID":sprintf("%-12s","�w�o��"), "GENDER":"�L"])));
	
	call_out( (: set_owner :), 1 );
	
        if( this_object()->set_shadow_ob() ) return;
	
	set("unit","�i");
	set("mass",10);
	set("value", -1);
	set("flag/no_amount", 1);
        set("flag/no_drop", 1);
        set("flag/no_give", 1);
}
