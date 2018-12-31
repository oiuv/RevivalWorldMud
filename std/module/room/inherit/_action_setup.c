/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _setup_action.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-23
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>

void refresh_class(object env, string database);

void do_setup(object me, string arg)
{
	string value;
	object env = environment(me);
	object master = env->query_master();
	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�]�w����F��H\n");
		
	if( !wizardp(me) && query("owner", env) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"�ä��O�o�a��������C\n");
		
	sscanf(arg, "%s %s", arg, value);
	
	switch(arg)
	{
		// �]�w���\�c�涵��
		case "sell":
		{
			int percent, number;
			object ob;
			string amount, basename, price, option, *worldproductlist;
			mapping sell = query("setup/sell", master)||allocate_mapping(0);
			mapping sell_product;
			
			worldproductlist = PRODUCT_D->query_world_product();

			if( !value )
			{			
				string msg;
				
				msg  = "�@�ɩҦ��ӫ~�C��\n";
				msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
				msg += "�s�� ���~                            �����ʼƶq ���ʻ�              ���ʤ�\n";
				msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
				foreach(basename in worldproductlist)
				{
					// ��Ӧ�m�ŤU��
					if( !objectp(ob = load_object(basename)) )
					{
						msg += sprintf("%s%-5s�L�k���J���~\n"NOR, 
							!(number % 2) ? "" : WHT,
							++number+".");
						
						continue;
					}
					
					sell_product = sell[replace_string(basename, "/", "#")]||sell["all"]||allocate_mapping(0);

					msg += sprintf("%s%-5s"NOR"%-34s%s%8s %-20s%6s\n"NOR, 
						!(number % 2) ? "" : WHT,
						++number+".", 
						ob->query_idname(),
						number % 2 ? "" : WHT,
						(sell_product["nosell"] ? "������" : sell_product["amount"]||"�L��")+"",
						NUMBER_D->number_symbol(count(count(query("value", ob), "*", sell_product["percent"]), "/", 100)),
						sell_product["percent"]+"%",
						);
				}
				
				msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
				return me->more(msg+"\n");
			}
			
			if( sscanf(value, "%s with %s for %s", option, price, amount) == 3 || sscanf(value, "%s with %s", option, price) == 2 )
			{
				if( option == "all" )
				{
					if( price[<1] != '%' )
						return tell(me, "�]�w���ʪ��~�ɥu��]�w���ʦʤ���C\n");

					percent = to_int(price[0..<2]);
					
					if( percent < 0 || percent > 999 )
						return tell(me, "�����ҥu�श�� 0% �� 999%�C\n");
					
					if( amount )
					{
						amount = big_number_check(amount);
					
						if( count(amount, "<", 0) || count(amount, ">", "10000000") )
							return tell(me, "�ƶq����C�� 0 �ΰ��� 10000000�C\n");
							
						delete("setup/sell", master);
						set("setup/sell/all/amount", amount, master);
					}
					else
						delete("setup/sell", master);
					

					set("setup/sell/all/percent", percent, master);
						
					if( amount )
						msg("$ME�N�Ҧ����~�����c�����]�w�� "+percent+"%�A�`���ʼƶq�� "+amount+"�C\n", me, 0, 1);
					else
						msg("$ME�N�Ҧ����~�����c�����]�w�� "+percent+"%�A�`���ʼƶq���L���C\n", me, 0, 1);
				}
				else
				{
					number = to_int(option);
					
					if( number <= 0 || number > sizeof(worldproductlist) )
						return tell(me, "�п�J���T���@�ɪ��~�s���C\n");
				
					basename = worldproductlist[number-1];
					ob = load_object(basename);
					basename = replace_string(basename, "/", "#");

					if( price[<1] != '%' )
						return tell(me, "�]�w���ʪ��~�ɥu��]�w���ʦʤ���C\n");

					percent = to_int(price[0..<2]);
					
					if( percent < 0 || percent > 999 )
						return tell(me, "�����ҥu�श�� 0% �� 999%�C\n");
							
					price = count(count(query("value", ob), "*", percent), "/", 100);

					if( !(price = big_number_check(price)) )
						return tell(me, "�п�J���T������C\n");
						
					if( count(price, "<", 0) || count(price, ">", "10000000000") )
						return tell(me, "���椣��C�� 0 �ΰ��� 10000000000�C\n");
							
					percent = to_int(count(count(price, "*", 100), "/", query("value", ob)));
						
					if( percent < 0 || percent > 999 )
						return tell(me, "�����ҥu�श�� 0% �� 999%�C\n");	

					delete("setup/sell/"+basename, master);

					if( amount )
					{
						amount = big_number_check(amount);
					
						if( count(amount, "<", 0) || count(amount, ">", "10000000") )
							return tell(me, "�ƶq����C�� 0 �ΰ��� 10000000�C\n");
						
						set("setup/sell/"+basename+"/amount", amount, master);
					}
					
					set("setup/sell/"+basename+"/percent", percent, master);

					if( amount )
						msg("$ME�N"+ob->query_idname()+"�����ʤ�]�w�� "+percent+"%�A���欰 "+price+"�A�`���ʼƶq�� "+amount+"�C\n", me, 0, 1);
					else
						msg("$ME�N"+ob->query_idname()+"�����ʤ�]�w�� "+percent+"%�A���欰 "+price+"�A�`���ʼƶq���L���C\n", me, 0, 1);
				}
			}
			
			else if( value == "all -d" )
			{
				delete("setup/sell", master);
				msg("$ME����ʩҦ����~�C\n", me, 0, 1);
			}
			else if( sscanf(value, "%d -d", number) == 1 )
			{
				if( number <= 0 || number > sizeof(worldproductlist) )
					return tell(me, "�п�J���T���@�ɪ��~�s���C\n");
					
				basename = worldproductlist[number-1];
				ob = load_object(basename);
				basename = replace_string(basename, "/", "#");
				delete("setup/sell/"+basename, master);
				set("setup/sell/"+basename+"/nosell", 1, master);
				msg("$ME�����"+ob->query_idname()+"�C\n", me, 0, 1);
			}
			
			master->save();
			break;
		}
		case "price":
		{
			int i, j, size;
			string unit, money;
			mapping products = query("products", master);
			string *productlist, basename, amount;
			
			if( !mapp(products) )
				return tell(me, "�o�̨èS���c�����ӫ~�C\n");

			productlist = implode(values(products), (:$1+$2:))||allocate(0);
			
			if( !value )
				return tell(me, pnoun(2, me)+"�Q�n�]�w���ذӫ~�������H\n");
			
			size = sizeof(productlist);
			
			if( !size )
				return tell(me, "�o�̨èS���c�����ӫ~�C\n");

			if( sscanf( value, "%d %s", i, money ) != 2 )
				return tell(me, pnoun(2, me)+"�Q�n�N���@�ӽs�����ӫ~�]�w�����H\n");
				
			unit = env->query_money_unit();
			
			for(j=0;j<size;j+=2)
			{
				basename = productlist[j];
				amount = productlist[j+1];
			
				if( j/2+1 == i )
				{
					object product = load_object(basename);

					if( !objectp(product) )
					{
						error(basename+" �ӫ~��ƿ��~�C");
						continue;
					}

					if( money[<1]=='%' && big_number_check(money[0..<2]) )
						money = count(count(query("value", product), "*", money[0..<2]), "/", 100);
						
					if( !big_number_check(money) || count(money, ">", "999999999") || count(money, "<=", 0) )
						return tell(me, "�ӫ~���楲������ 1 - 999999999 �����C\n");
									
					set("setup/price/"+replace_string(basename, "/", "#"), money, master);
					msg("$ME�N"+product->query_idname()+"�������վ㬰"+HIY+QUANTITY_D->money_info(unit, money)+NOR" ("+count(count(money,"*",100),"/",query("value", product))+"%)�C\n", me, 0, 1);

					master->query_module_file()->calculate_master_data(master, basename);
					master->save();
					return;
				}
			}
			
			return tell(me, "�п�J���T���s���C\n");
		}
		case "class":
		{
			int i, j, size;
			string classname;
			mapping products = query("products", master);
			string *productlist, basename, amount;
			
			if( !mapp(products) )
				return tell(me, "�ثe�S���ӫ~�i�H�]�w�C\n");

			productlist = implode(values(products), (:$1+$2:))||allocate(0);
			
			if( !value )
				return tell(me, pnoun(2, me)+"�Q�n�]�w���ذӫ~�������H\n");
			
			size = sizeof(productlist);
			
			if( !size )
				return tell(me, "�o�̨èS���c�����ӫ~�C\n");

			if( sscanf( value, "%d %s", i, classname ) != 2 )
				return tell(me, pnoun(2, me)+"�Q�n�N���@�ӽs�����ӫ~�]�w�����H\n");
			
			classname = remove_ansi(classname);
			
			if( !is_chinese(classname) || strlen(classname) < 2 || strlen(classname) > 10 )
				return tell(me, "�����W�٥����O����@�줭�Ӫ�����r�C\n");

			for(j=0;j<size;j+=2)
			{
				basename = productlist[j];
				amount = productlist[j+1];
			
				if( j/2+1 == i )
				{
					object product = load_object(basename);
					
					if( !objectp(product) )
					{
						error(basename+" �ӫ~��ƿ��~�C");
						continue;
					}

					switch(classname)
					{
						case "�i��":
							delete("setup/class/"+replace_string(basename, "/", "#"), master);
							break;
						case "�P�P":
						{
							mapping setupclass = query("setup/class", master);
						
							if( mapp(setupclass) )
							foreach(string f, string c in setupclass)
							{
								if( c == "�P�P" )
								{
									delete("setup/class/"+f, master);
									refresh_class(master, "products");
									master->query_module_file()->calculate_master_data(master, replace_string(f, "#", "/"));
								}
							}

							if( query("badsell", product) )
								return tell(me, "�o�ذӫ~�L�k�C���P�P���ӫ~�C\n");							
							
							set("setup/class/"+replace_string(basename, "/", "#"), classname, master);
							break;
						}
						default:
							set("setup/class/"+replace_string(basename, "/", "#"), classname, master);
							break;
					}
					msg("$ME�N"+product->query_idname()+"�������]�w��"+HIY+classname+NOR"�C\n", me, 0, 1);
					refresh_class(master, "products");

					master->query_module_file()->calculate_master_data(master, basename);
					master->save();
					return;
				}
			}
			
			return tell(me, "�п�J���T���s���C\n");
		}
		case "mode":
		{
			if( query("mode", master) )
			{
				delete("mode", master);
				msg("$ME�N���ө��ഫ���u��~�Ҧ��v�C\n", me, 0, 1);
			}
			else
			{
				set("mode", 1, master);
				msg("$ME�N���ө��ഫ���u�޲z�Ҧ��v�C\n", me, 0, 1);
			}
			master->save();
			break;
		}
		default:
			return tell(me, "�п�J���T���]�w����(sell, price, class, mode)�C\n");
	}
	
}


void do_warehouse_setup(object me, string arg)
{
	string value;
	object env = environment(me);
	object master = env->query_master();

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�]�w����F��H\n");
		
	if( !wizardp(me) && query("owner", env) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"�ä��O�o���ܮw���֦��̡C\n");
		
	sscanf(arg, "%s %s", arg, value);
	
	switch(arg)
	{
		case "class":
		{
			int i, j, size;
			string classname;
			mapping products = query("products", master);
			string *productlist, basename, amount;
			
			if( !mapp(products) )
				return tell(me, "�ثe�S�����~�i�H�]�w�����C\n");

			productlist = implode(values(products), (:$1+$2:))||allocate(0);
			
			if( !value )
				return tell(me, pnoun(2, me)+"�Q�n�]�w���ت��~�������H\n");
			
			size = sizeof(productlist);
			
			if( !size )
				return tell(me, "�o�̨èS���c����󪫫~�C\n");

			if( sscanf( value, "%d %s", i, classname ) != 2 )
				return tell(me, pnoun(2, me)+"�Q�n�N���@�ӽs�������~�]�w�����H\n");
			
			classname = remove_ansi(classname);
			
			if( !is_chinese(classname) || strlen(classname) < 2 || strlen(classname) > 10 )
				return tell(me, "�����W�٥����O����@�줭�Ӫ�����r�C\n");

			for(j=0;j<size;j+=2)
			{
				basename = productlist[j];
				amount = productlist[j+1];
				
				if( j/2+1 == i )
				{
					object product = load_object(basename);
				
					if( !objectp(product) )
					{
						error(basename+" �ӫ~��ƿ��~�C");
						continue;
					}

					if( classname == "�i��")
						delete("setup/class/"+replace_string(basename, "/", "#"), master);
					else
						set("setup/class/"+replace_string(basename, "/", "#"), classname, master);

					msg("$ME�N"+product->query_idname()+"�������]�w��"+HIY+classname+NOR"�C\n", me, 0, 1);
					refresh_class(master, "products");

					master->save();
					return;
				}
			}
			
			return tell(me, "�п�J���T���s���C\n");
		}

		default:
			return tell(me, "�п�J���T���]�w����(class)�C\n");
	}
	
}