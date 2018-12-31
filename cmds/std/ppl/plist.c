/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : plist.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : ���a���O�{��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <skill.h>
#include <feature.h>
#include <daemon.h>

#define PRODUCT_MODULE_PATH "/std/module/product/"

inherit COMMAND;

private void do_command(object me, string arg)
{
	mapping info, subinfo;
	object materialob;
	string msg;
	string *products = get_dir(PRODUCT_MODULE_PATH);
	string *learned_products = query("learned_products", me) || allocate(0);
	if( !arg )
	{
		msg = "���~�C��A�ثe�@�i�Ͳ� "+sizeof(products)+" �ز��~\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg +=    "�W��                    �Ͳ��u�t      �]�p�� �s�y�Ӯ� �̧C����\n"NOR;
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		
		foreach(string product in products)
		{
			info = (PRODUCT_MODULE_PATH+product+"/module.c")->query_product_info();
			
			msg += sprintf("%-24s%-14s%-7s%-9d%s\n",
				info["name"]+"("+info["id"]+")",
				BUILDING_D->query_building(info["factory"])[0],
				member_array(product, learned_products) == -1 ? HIR"�L"NOR:HIG"��"NOR,
				info["timecost"],
				info["value"],
				);
		}
		
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += "��J plist '���~ID' �i�d�߲Ӹ`���\n\n";
	}
	else if( member_array(arg, products) != -1 )
	{
		info = (PRODUCT_MODULE_PATH+arg+"/module.c")->query_product_info();
		
		msg = info["name"]+"("+capitalize(info["id"])+")���ԲӲ��~��Ʀp�U�G\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += WHT"���~�W��"NOR" "+info["name"]+"("+capitalize(info["id"])+")\n";
		msg += WHT"�Ͳ��u�t"NOR" "+BUILDING_D->query_building(info["factory"])[0]+"\n";
		msg += WHT"���~����"NOR" $"+info["value"]+"\n";
		msg += WHT"���~���q"NOR" "+info["mass"]+" g\n";
		msg += WHT"�s�y�Ӯ�"NOR" "+info["timecost"]+" ��/���\n";
		msg += WHT"��ޯӶO"NOR" "+info["technology"]+" ��ޭ�\n";
		msg += WHT"��o�O��"NOR" $"+NUMBER_D->number_symbol(info["cost"])+"\n";

		msg += WHT"�s�y���\n"NOR;
		foreach( string material, int amount in info["material"] )
		{
			if( !objectp(materialob = load_object(material)) )
			{
				subinfo = (PRODUCT_MODULE_PATH+material+"/module.c")->query_product_info();
				msg += sprintf("         %-30s %d\n", subinfo["name"]+"("+capitalize(subinfo["id"])+")", amount);
			}
			else
				msg += sprintf("         %-30s %d\n", materialob->query_idname(), amount);
		}

		msg += WHT"�ޯ�ݨD\n"NOR;
		foreach( string skill, int level in info["skill"] )
			msg += sprintf("         %-30s �ܤ� %-3d ��\n", (SKILL(skill))->query_idname(), level);
		
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	}
	tell(me, msg);
}