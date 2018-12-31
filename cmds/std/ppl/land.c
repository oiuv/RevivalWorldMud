/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : land.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-08
 * Note   : land ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <map.h>
#include <location.h>

inherit COMMAND;

string help = @HELP
    �g�a��اl�ǡC

land			- �d�ߥثe�w�g�l�Ǫ��g�a���
land absorb		- �l�Ǥg�a���(�ݭn�S���~)
land release '�Ʀr'	- �N�g�a��ئC���� N �ӧl�����g�a��������ثe�Ҧb���g�a

HELP;

private void do_command(object me, string arg)
{
	int i;

	if( !arg )
	{
		string list = "";
		array landdata = query("landdata", me);
		
		if( arrayp(landdata) && sizeof(landdata) )
		{
			list += HIY"�g�a"NOR YEL"���"NOR"\n";
			foreach(array elem in landdata)
			{
				i++;
				switch(elem[0])
				{
					case FARM: 
						list += sprintf("%-3d%-20s "NOR GRN"�g�a���� "HIG"%s"NOR GRN"/%d\n"NOR, i, HIG"�A��"NOR, repeat_string("*", elem[1]), elem[2]);
						break;
					case PASTURE: 
						list += sprintf("%-3d%-20s "NOR YEL"�g�a���� "HIY"%s"NOR YEL"/%d\n"NOR, i, NOR YEL"����"NOR, repeat_string("*", elem[1]), elem[2]);
						break;
					case FISHFARM: 
						list += sprintf("%-3d%-20s "NOR BLU"�g�a���� "HIB"%s"NOR BLU"/%d\n"NOR, i, HIB"�i�޳�"NOR, repeat_string("*", elem[1]), elem[2]);
						break;
				}
			}
			return tell(me, list+"\n");
		}
		else
			return tell(me, "�ثe�S������g�a�l�Ǻ�ءC\n");
	}
	else if( arg == "absorb" )
	{
		int type;
		int growth_level;
		int maintain_time;
		
		object media;
		object env = environment(me);
		array loc = query_temp("location", me);
		array landdata = query("landdata", me) || allocate(0);
		
		if( !env->is_maproom() || !arrayp(loc) || !CITY_D->city_exist(loc[CITY], loc[NUM]) )
			return tell(me, "�L�k�l�ǳo���g�a����ءC\n");
		
		if( CITY_D->query_coor_data(loc, "owner") != me->query_id(1) )
			return tell(me, "�o���g�a�ä��O"+pnoun(2, me)+"���C\n");
			
		foreach(object ob in all_inventory(me))
		{
			if( query("land_absorb", ob) )	
			{
				media = ob;
				break;
			}
		}
		
		if( !objectp(media) )
			return tell(me, pnoun(2, me)+"���W�ʥF�l�Ǥg�a��ت��C�����~�C\n");
		
		type = CITY_D->query_coor_data(loc, TYPE);
		growth_level = CITY_D->query_coor_data(loc, "growth_level");
		maintain_time = CITY_D->query_coor_data(loc, "maintain_time");
	
		if( type != FARM && type != FISHFARM && type != PASTURE )
			return tell(me, "�L�k�l�ǳo�ؤg�a����ءC\n");
	
		if( maintain_time < 1 )
			return tell(me, "�o���g�a�S�������إi�H�l���C\n");

		msg("$ME�N$YOU��m�b�g�a�������A�u��$YOU�o�X�벴��"HIW"�ե�"NOR"�N�o���g�a����ا����l�ǰ_�ӡC\n", me, media, 1);
		msg("�H�ۥե��v�����h�A$YOU�]��ۮ����F�C\n", me, media, 1);
		destruct(media, 1);

		CITY_D->delete_coor_data(loc, "growth_level");
		CITY_D->delete_coor_data(loc, "maintain_time");
		CITY_D->delete_coor_data(loc, "growth");
		CITY_D->delete_coor_data(loc, "status");
		TOP_D->delete_top("maintain", save_variable(loc));

		landdata += ({ ({ type, growth_level, maintain_time }) });
		set("landdata", landdata, me);
		me->save();
	}
	else if( sscanf(arg, "release %d", i) == 1 )
	{
		object env = environment(me);
		array landdata = query("landdata", me);
		array loc = query_temp("location", me);
		array elem;
		
		if( !env->is_maproom() || !arrayp(loc) || !CITY_D->city_exist(loc[CITY], loc[NUM]) )
			return tell(me, "�L�k�N�g�a��������o���g�a�W�C\n");

		if( CITY_D->query_coor_data(loc, "owner") != me->query_id(1) )
			return tell(me, "�o���g�a�ä��O"+pnoun(2, me)+"���C\n");
			
		if( !arrayp(landdata) )
			return tell(me, "�ثe�S�����󪺤g�a��ءC\n");

		if( i < 1 || i > sizeof(landdata) )
			return tell(me, "�п�J���T���Ʀr�C\n");

		elem = landdata[i-1];
		
		if( CITY_D->query_coor_data(loc, TYPE) != elem[0] )
			return tell(me, "�L�k�N�o���������g�a��������o���g�a�W�C\n");
			
		if( CITY_D->query_coor_data(loc, "maintain_time") )
			return tell(me, "�o���g�a�w�g���{�s���g�a��ءA�L�k�A�N�g�a��������o���g�a�W�C\n");

		CITY_D->set_coor_data(loc, "growth_level", elem[1]);
		CITY_D->set_coor_data(loc, "maintain_time", elem[2]);
		GROWTH_D->set_status(loc);
		
		msg("$ME�N�j�q���g�a��������o���g�a�W�A�u���g�a�W�x�_�@�}"HIW"��ծ���~"NOR"�C\n", me, 0, 1);
		TOP_D->update_top("maintain", save_variable(loc), elem[1], me->query_idname(), elem[0]);

		landdata = landdata[0..i-2] + landdata[i..<1];
		
		if( !sizeof(landdata) )
			delete("landdata", me);
		else
			set("landdata", landdata, me);
		
		me->save();
	}
	else return tell(me, "�п�J���T�����O�Ѽ�(help land)�C\n");
}
