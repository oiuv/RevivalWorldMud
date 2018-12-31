/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mayor.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-14
 * Note   : ������T
 * Update :
 *  o 2003-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
	��ܫ�����T�C
HELP;

private void do_command(object me, string arg)
{
	int i, j, totalresident, totalflourish;
	string str;
	string *cities, city, mayor, *emblem;
	float rate;
	string GNP, NNP;

	if(!arg)
	{
		cities = sort_array(CITY_D->query_cities() - ({ "wizard" }), (: CITY_D->query_city_info($1, "totalflourish") < CITY_D->query_city_info($2, "totalflourish") ? 1 : -1 :));
	
		str = HIW"����             �ìP���� ���� ����        �`�c�a�� ���� ����ɥN �}����U �g�٦���\n";
		str +=NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		
		foreach(city in cities)
		{
			mayor = CITY_D->query_city_info(city, "government/mayor");

			GNP = CITY_D->query_city_info(city, "GNP");
			NNP = CITY_D->query_city_info(city, "NNP");
			
			if( count(GNP, "<=", 0) )
				rate = 0.;
			else
				rate = to_float(count(count(NNP, "*", 10000), "/", GNP))/100.;

			for(i=0;CITY_D->city_exist(city, i);i++);
			
			if( CITY_D->query_city_info(city, "fallen") ) rate = 0.;
			
			str += sprintf("%-24s"HIM"%d %3s  "HIW"%-12s"HIG"%-9s"HIC"%-5d"HIY"%-9s%-6s"+(rate>0?HIG:HIR)+"%9.2f%%\n"NOR,
				CITY_D->query_city_idname(city),
				i,
				CITY_D->query_city_info(city, "fallen") ? HIR"��"NOR : HIG"��"NOR,
				mayor ? capitalize(mayor) : "",
				NUMBER_D->number_symbol(CITY_D->query_city_info(city, "totalflourish")),
				sizeof(CITY_D->query_city_info(city, "citizens")),
				CITY_D->query_age_name(city),
				CITY_D->query_city_info(city, "fallen") ? "":CITY_D->query_city_info(city, "register_open")?HIG"�}��"NOR:HIR"�ʳ�"NOR,
				rate,
			);
		}
		str +=NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		str +=NOR WHT"mayor '����ID' �d�߲Ӹ`���e\n"NOR;
	}
	else
	{
		string city_idname, moneyunit, area, city_scale_mayor;
		mapping officer;
		
		city = arg;

		if( !CITY_D->city_exist(city) )
			return tell(me, "�S�� "+city+" �o�ӫ����C\n");
		
		//if( CITY_D->query_city_info(city, "fallen") )
		//	return tell(me, "�L�k�d��"+CITY_D->query_city_idname(city)+"����ơC\n");

		emblem = CITY_D->query_city_info(city, "emblem");
		mayor = CITY_D->query_city_info(city, "government/mayor");
		officer = CITY_D->query_city_info(city, "government/officer");
		
		totalresident = 0;
		totalflourish = CITY_D->query_city_info(city, "totalflourish");

		for(i=0;CITY_D->city_exist(city, i);i++)
			totalresident += CITY_D->query_section_info(city, i, "resident");
		
		city_idname = CITY_D->query_city_idname(city);
		str =  "                                                                "+emblem[0]+"\n";
		str += sprintf("%-24s"NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w  "NOR"%10s\n", city_idname, emblem[1]);
		str +=  "                                                                "+emblem[2]+"\n";


		str += "�ɥN "+CITY_D->query_age_name(city)+" "HIY+repeat_string("*", CITY_D->query_city_info(city, "level"))+NOR"\n";
		str += "���� "HIW+(mayor ? capitalize(mayor) : "�L")+"\n"NOR;
		str += "�x�� \n"NOR;

		if( !sizeof(officer) )
			str += "     �L\n";
		else
			foreach( string officerid, string title in officer )
				str += sprintf("     %-12s%s\n", HIW+capitalize(officerid)+NOR, title);
		
		str += "\n�`������      "HIG+sizeof(CITY_D->query_city_info(city, "citizens"))+NOR"\n";
		str += "�`�c�a��      "HIG+NUMBER_D->number_symbol(totalflourish)+NOR"\n";
		str += "�`�~����      "HIG+NUMBER_D->number_symbol(totalresident)+NOR"\n\n";

		moneyunit = MONEY_D->city_to_money_unit(city);
		GNP = CITY_D->query_city_info(city, "GNP");
		NNP = CITY_D->query_city_info(city, "NNP");
		
		if( count(GNP, "<=", 0) )
			rate = 0.;
		else
			rate = to_float(count(count(NNP, "*", 10000), "/", GNP))/100.;

		if( CITY_D->query_city_info(city, "fallen") ) rate = 0.;
		str += sprintf("%-38s%-32s\n", "������      "+(moneyunit ? "$"+moneyunit:"���w"), "�F���겣   "+NUMBER_D->number_symbol(CITY_D->query_city_info(city, "assets")));
		str += sprintf("%-38s%-32s\n", "��ڶײv      "+EXCHANGE_D->query_exchange_data(moneyunit||"0"), "�a���|     "+CITY_D->query_city_info(city, "estatetax"));
		str += sprintf("%-38s%-32s\n\n", "�����`�겣    "+money(MONEY_D->query_default_money_unit(), GNP), "�g�٦����v "+(rate>0?HIG:HIR)+sprintf("%-.2f%%", rate))+NOR;
		
		for(j=0;j<i;j++)
		{
			switch(CITY_D->query_section_info(city, j, "level"))
			{
				case 0: city_scale_mayor = "����"; break;
				case 1: city_scale_mayor = "�m��"; break;
				case 2: city_scale_mayor = "�ƥ���"; break;
				case 3: city_scale_mayor = "�ƭ���"; break;
				case 4: city_scale_mayor = "�ƭ���"; break;
			}

			str += (CITY_D->query_section_info(city, j, "name")||"�� "+(j+1)+" �ìP����")+"\n";
			str += sprintf("%-14s%-22s%-13s%s\n", city_scale_mayor, 	HIC+(capitalize(CITY_D->query_section_info(city, j, "vicemayor"))||"")+NOR, 			HIY"  ���ݸ귽   "NOR, NUMBER_D->number_symbol(CITY_D->query_section_info(city, j, "resource/metal")));
			str += sprintf("%-14s%-22s%-13s%s\n", "�c�a��", 	NUMBER_D->number_symbol(CITY_D->query_section_info(city, j, "flourish")), 			WHT"  ��۸귽   "NOR, NUMBER_D->number_symbol(CITY_D->query_section_info(city, j, "resource/stone")));
			str += sprintf("%-14s%-22s%-13s%s\n", "�~����", 	NUMBER_D->number_symbol(CITY_D->query_section_info(city, j, "resident")), 			HIC"  �M���귽   "NOR, NUMBER_D->number_symbol(CITY_D->query_section_info(city, j, "resource/water")));
			str += sprintf("%-14s%-22s%-13s%s\n", "�W��", 		CITY_D->query_city_scale(city, j), 							YEL"  ���귽   "NOR, NUMBER_D->number_symbol(CITY_D->query_section_info(city, j, "resource/wood")));
			str += sprintf("%-14s%-22s%-13s%s\n\n", "���O��O/��", money(moneyunit, count(24,"*",SHOPPING_D->query_shopping_info(city, j, "totalmoney"))),	HIM"  �U�Ƹ귽   "NOR, NUMBER_D->number_symbol(CITY_D->query_section_info(city, j, "resource/fuel")));
		}
		str += sprintf(HIW"\n%-14s%-22s%-13s%-14s\n"NOR, "���ϦW��", "�x���n�u", "�귽����", "�S���ޱ���" );
		
		foreach(area in AREA_D->query_areas())
		{
			str += sprintf("%-14s%-22s%-13s%-14s\n", AREA_D->query_area_name(area), WHT+NUMBER_D->number_symbol(0)+NOR, HIG+0+"%"NOR, HIR"�L����o�{"NOR);
		}
		str +=NOR WHT"\n�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	}
	me->more(str);
}