/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _action_auction.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-11
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>

#define DEFAULT_PAYMENT		10000

void output_object(object env, string database, string basename, string amount);

void do_auction(object me, string arg, string database)
{
	int num, i, j;
	string amount;
	int auc_amount;
	string auc_base_price;
	string auc_direct_price;
	int auc_hour;
	mapping objects;
	string *shelfsort, shelf;
	string basename;
	object ob;
	object env = environment(me);
	object master = env->query_master();
	string money_unit = MONEY_D->query_default_money_unit();
	mapping auc_data = allocate_mapping(0);
	
	if( query("owner", env) != me->query_id(1) )
		return tell(me, "�o�̬O�O�H���ؿv���C\n");

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�����@�Ӫ��~�H\n");
		
	if( sscanf(arg, "%d %d $%s $%s %d", num, auc_amount, auc_base_price, auc_direct_price, auc_hour) != 5 )
		return tell(me, "�п�J���T�������(help auction)�C\n");
		
	if( auc_amount < 1 || auc_amount > 9999999 )
		return tell(me, "�п�J���T���ƶq(1-9999999)�C\n");
		
	auc_base_price = big_number_check(auc_base_price);
	auc_direct_price = big_number_check(auc_direct_price);

	if( count(auc_base_price, "<", 1) || count(auc_base_price, ">", "999999999999") )
		return tell(me, "�п�J���T�����л���(1 - 999999999999)�C\n");
		
	if( count(auc_direct_price, "<", 1) || count(auc_direct_price, ">", "999999999999") )
		return tell(me, "�п�J���T�������ʶR����(1 - 999999999999)�C\n");
		
	if( count(auc_direct_price, "<=", auc_base_price) )
		return tell(me, "�����ʶR�����o����ΧC�󩳻��C\n");

	if( auc_hour < 1 || auc_hour > 7*24 )
		return tell(me, "�v�Юɶ��̤� 1 �Ӥp�ɡA�̦h 168 �p�ɡC\n");
		
	objects = query(database, master);

	if( !sizeof(objects) )
		return tell(me, env->query_room_name()+"�̨èS�����󪫫~�C\n");

	shelfsort = keys(objects) | (query("shelfsort", master) || allocate(0));

	foreach( shelf in shelfsort )
	{
		for(i=0;i<sizeof(objects[shelf]);i+=2)
		{
			if( ++j == num )
			{
				basename = objects[shelf][i];
				amount = objects[shelf][i+1];

				if( catch(ob = load_object(basename)) )
					continue;
	
				// �Y���~�w�g����
				if( !objectp(ob) )
				{
					error(basename+" �ӫ~��ƿ��~�C");
					continue;
				}
				
				if( count(amount, "<", auc_amount) )
					return tell(me, ob->query_idname()+"�S���o��h�ӡC\n");

				if( !me->spend_money(money_unit, DEFAULT_PAYMENT) )
					return tell(me, pnoun(2, me)+"�S����������ú�����O "HIY+money(money_unit, DEFAULT_PAYMENT)+NOR"�C\n");
				else
					tell(me, pnoun(2, me)+"ú��F������O "+HIY+money(money_unit, DEFAULT_PAYMENT)+NOR"�C\n");

				output_object(master, database, basename, ""+auc_amount);
				
				auc_data["seller"] = me->query_id(1);
				auc_data["basename"] = basename;
				auc_data["amount"] = auc_amount;
				auc_data["minute"] = auc_hour*60;
				auc_data["base_price"] = auc_base_price;
				auc_data["direct_price"] = auc_direct_price;
				
				AUCTION_D->add_auction(auc_data);
				
				CHANNEL_D->channel_broadcast("auc", HIB"�s�ӫ~�G"NOR+me->query_idname()+"���u"+QUANTITY_D->obj(ob, auc_amount, 1)+"�v�A�����G"HIY+money(money_unit, auc_base_price)+NOR"�A�����ʶR���G"HIY+money(money_unit, auc_direct_price)+NOR"�A�Ѿl�ɶ��G"HIR+(auc_hour*60)+NOR" ����");
				return;
			}
		}
	}
	
	tell(me, "�ӫ~�s�����~�C\n");
}
