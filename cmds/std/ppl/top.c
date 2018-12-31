/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : top.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-8-15
 * Note   : top ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <skill.h>
#include <map.h>

inherit COMMAND;

string help = @HELP
�C�X�U�رƦ�]�G

top �ﶵ		- �w�]�C�X�@�ɫe 30 �W�C��
top �ﶵ 100 		- �C�X�@�ɫe 100 �W�C��
top �ﶵ all		- �C�X�����ƦW(�̦h 1000 �W)

���ئp�U
top rich		- �@�ɴI���Ʀ�](�{���ϩЦa���`��)
top cash		- �@�ɲ{���Ʀ�]
top estate		- �@�ɩЦa���Ʀ�]
top product		- �@�ɺZ�P���~�Ʀ�]
top store		- �@�ɳ̨ΰө��Ʀ�]
top skill		- �@�ɧޯ�v�v�Ʀ�]
top skill �ޯ�W	- �@�ɳ�W�ޯ൥�űƦ�]
top building		- �@�ɰ��ӫؿv�Ʀ�]
top enterprise		- �@�ɥ��~�Ʀ�]
top stock		- �@�ɪѤ��Ʀ�]
top socialexp		- �@�ɪ��|�g��Ʀ�]
top maintain		- �@�ɤg�a���űƦ�]
top quest		- �@�ɥ��ȧ����ƶq�Ʀ�]
HELP;

private void do_command(object me, string arg)
{
	int i, top;
	string msg, arg1, arg2, arg3;

	if( !arg ) 
		return tell(me, help);

	if( sscanf(arg, "%s %s %s", arg1, arg2, arg3) == 3 )
	{
		if( arg3 == "all" ) 
			i = 1000;
		else 
			i = to_int(arg3);
	}
	else if( sscanf(arg, "%s %s", arg1, arg2) == 2 )
	{
		if( arg2 == "all" )
			i = 1000;
		else if( to_int(arg2) )
		{
			i = to_int(arg2);
			arg2= 0;
		}
		else
			i = 30;
	}
	else
	{
		i = 30;
		arg1 = arg;
	}

	if( i < 1 || i > 1000 )
		return tell(me, "��ܱƦW�ƶq���o�p�� 1 �Τj�� 1000�C\n");

	switch( arg1 )
	{
		case "rich":
			{

				msg = NOR YEL"�@��"HIY"�e "+i+" �I��"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "�ƦW", "���a", "����", "�`�겣($RW)", "����");
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach(array data in TOP_D->query_top("rich"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
						top,
						data[3],
						(CITY_D->query_city_name(data[4]) || ""),
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "cash":
			{

				msg = NOR YEL"�@��"HIY"�e "+i+" �{��"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "�ƦW", "���a", "����", "�{��($RW)", "����");
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach(array data in TOP_D->query_top("cash"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
						top,
						data[3],
						(CITY_D->query_city_name(data[4]) || ""),
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "estate":
			{

				msg = NOR YEL"�@��"HIY"�e "+i+" �Цa��"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "�ƦW", "���a", "����", "�Цa��($RW)", "����");
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach(array data in TOP_D->query_top("estate"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
						top,
						data[3],
						(CITY_D->query_city_name(data[4]) || ""),
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "product":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �Z�P���~"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg += sprintf(HIW"%4s %-33s %8s %11s  %-12s %6s\n"NOR, "�ƦW", "���~", "�P���`��", "����", "�}�o��", "����");
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("product"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-33s"HIW" %8s"HIY" %11s"HIG"  %-12s"NOR YEL" %6s\n"NOR,
						top,
						data[3],
						NUMBER_D->number_symbol(data[1]),
						data[4],
						ansi_capitalize(data[5]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "store":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �̨ΰө��Ʀ�]"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg += sprintf(HIW"%4s %-31s %14s %10s %-10s %6s\n"NOR, "�ƦW", "�ө�", "�P���`��", "����", "����", "����");
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("storesell"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-31s"HIW" %14s"HIY" %10s"HIG" %-10s"NOR YEL" %6s\n"NOR,
						top,
						data[4],
						NUMBER_D->number_symbol(data[1]),
						(CITY_D->query_city_name(data[5]) || ""),
						ansi_capitalize(data[3]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "skill":
			{
				if( !arg2 )
				{
					msg = NOR YEL"�@��"HIY"�e "+i+" �ޯ�v�v"NOR YEL"�Ʀ�]�G\n"NOR;
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
					msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "�ƦW", "���a", "����", "���޼�", "����");
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
					foreach(array data in TOP_D->query_top("skill"))
					{
						if( ++top > i ) break;
						msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
							top,
							data[3],
							(CITY_D->query_city_name(data[4]) || ""),
							NUMBER_D->number_symbol(data[1]),
							TIME_D->replace_ctime(data[2])[0..9],
							);
					}
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				}
				else
				{
					arg2 = lower_case(arg2);

					if( !SKILL_D->skill_exists(arg2) )
						return tell(me, "�èS���o�اޯ�C\n");
					
					msg = NOR YEL"�@��"HIY"�e "+i+" "+(SKILL(arg2))->query_idname()+"�ޯ൥��"NOR YEL"�Ʀ�]�G\n"NOR;
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
					msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "�ƦW", "���a", "����", "�ޯ൥��", "����");
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
					foreach(array data in TOP_D->query_top("skill/"+arg2))
					{
						if( ++top > i ) break;
						msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
							top,
							data[3],
							sizeof(data) > 4 ? (CITY_D->query_city_name(data[4]) || "") : "",
							NUMBER_D->number_symbol(data[1]),
							TIME_D->replace_ctime(data[2])[0..9],
							);
					}
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
					
					
				}
				return me->more(msg+"\n");
			}
			break;
		case "building":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" ���ӫؿv"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg += sprintf(HIW"%4s %-14s %-32s %-10s %4s %10s\n"NOR, "�ƦW", "���a", "�ө�", "����", "�Ӽh", "����");
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach(array data in TOP_D->query_top("building"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."HIY" %-14s "NOR"%-32s %-10s "HIG"%4s"NOR YEL" %10s\n"NOR,
						top,
						data[3],
						data[4],
						(CITY_D->query_city_name(data[5]) || ""),
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;

		case "stock":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �Ѥ�"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          �����Ѳ��{��        �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("stockvalue") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "quest":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" ���ȧ���"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          ���ȧ����ƶq        �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("quest") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						data[1],
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "enterprise":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" ���~"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���~                     �겣�`��		               �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("stock") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s"HIY"%-22s"NOR YEL"%s\n"NOR,
						top,
						data[2],
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "socialexp":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" ���|�g���`��"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          ���|�g���`��        �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("social_exp") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "maintain":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �g�a����"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                      �y��                 ���� ����   �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("maintain") )
				{
					if( ++top > i ) break;
						
					switch(data[4])
					{
						case FARM: arg1 = HIG"�A"NOR GRN"��"NOR; break;
						case PASTURE: arg1 = HIY"��"NOR YEL"��"NOR; break;
						case FISHFARM: arg1 = HIB"�i"NOR BLU"��"NOR; break;
					}
					msg += sprintf(HIW"%-3d."NOR" %-25s%-22s%4s"HIR"%5s"NOR YEL" %s\n"NOR,
						top,
						data[3],
						loc_short(restore_variable(data[0])),
						arg1,
						data[1],
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		default:
			tell(me, help);
			break;
	}
}		
