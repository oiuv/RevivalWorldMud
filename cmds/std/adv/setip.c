/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : setip.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-12
 * Note   : setip ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <ip.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
setip 					- �C�X�ثe IP �]�w���
setip ban 210.59.67.253			- �]�w�� IP �ä[�T��n�J
setip ban 210.59.*.*			- �]�w�� IP �ä[�T��n�J
setip ban 210.59.*.* in 50		- �]�w 50 �p�ɤ��T� IP �n�J
setip ban 210.59.*.* in 50 with '����'	- �]�w 50 �p�ɤ��T� IP �n�J�P�ɵ��Ѹ��
setip -d 210.59.67.253			- ������ IP �Ҧ��]�w

IP ��ܦ��i�� regexp �����W�y�k

setip �ﶵ�p�U
 ban 		- �����T�� IP �n�J
 cmdlimit	- ����Ѧ� IP �n�J�����a�����O����t��(�g�@�ʭ���)
 ok		- �Щ��� IP ���ݦX�k IP, ���ݭn�]�w����(���K�Q���Э���)
HELP;


int valid_ip(string ip)
{
	string *elem = allocate(4);
	
	if( sscanf(ip, "%s.%s.%s.%s", elem[0], elem[1], elem[2], elem[3]) != 4 )
		return 0;

	// �Ĥ@�� * �|�y�� regexp �P�_���~
	if( elem[0] == "*" ) return 0;

	foreach(string e in elem)
	if( e != "*" && to_int(e) < 0 && to_int(e) > 255 )
		return 0;
		
	return 1;
}

private void command(object me, string arg)
{
	int releasetime;
	string option, ip, note;

	if( !is_command() ) return;
	
	if( !arg )
	{
		string msg;
		mapping ipdata = IP_D->query_ipdata();
		mapping data;
		msg  = HIW"IP                �]�w         �]�w�ɶ�         ����ɶ�         ����\n"NOR;
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		
		if( sizeof(ipdata) )
		foreach(ip in sort_array(keys(ipdata), 1))
		{
			data = ipdata[ip];
			switch(data["status"])
			{
				case BAN: 		option = HIR"�T��n�J"NOR; break;
				case CMDLIMIT:		option = HIY"���O�t�׭���"NOR; break;
				case OK:		option = HIG"�X�k"NOR;	break;
			}
			msg += sprintf("%-18s%-13s"HIM"%-17s"NOR MAG"%-17s"NOR"%s\n", ip, option, TIME_D->replace_ctime(data["recordtime"]), data["releasetime"] ? TIME_D->replace_ctime(data["releasetime"]) : "�L��", data["note"]||"");
		}
		else
			msg += "�S������ IP ����]�w�C\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		return me->more(msg);
	}
	
	if( sscanf(arg, "%s %s", option, arg) != 2 )
		return tell(me, "�� help setip �d�ߥ��T���O�榡�C\n");
		
	if( sscanf(arg, "%s in %d with %s", ip, releasetime, note) != 3 && sscanf(arg, "%s in %d", ip, releasetime) != 2 && sscanf(arg, "%s with %s", ip, note) != 2 )
		ip = arg;

	if( !valid_ip(ip) )
		return tell(me, "IP �榡���~�C\n");

	switch(option)
	{
		case "ban":
			if( IP_D->query_ip(ip) )
				return tell(me, "IP�u"+ip+"�v�w���]�w����A�Х��R���]�w�C\n");

			IP_D->set_ip(me, ip, BAN, releasetime*3600);
			
			if( releasetime )
				CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�]�w "+releasetime+" �p�ɤ��T��a�g�ѡu"+ip+"�v�� IP ��m�n�J�C���C");
			else
				CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�]�w�ä[�T��a�g�ѡu"+ip+"�v�� IP ��m�n�J�C���C");
			if( note )
			{
				IP_D->set_ip_note(me, ip, note);
				CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�Х� IP�u"+ip+"�v���]�w���Ѭ��G"+note+"�C");
			}
			break;
		case "cmdlimit":
			if( IP_D->query_ip(ip) )
				return tell(me, "IP�u"+ip+"�v�w���]�w����A�Х��R���]�w�C\n");

			IP_D->set_ip(me, ip, CMDLIMIT, releasetime*3600);
			
			if( releasetime )
				CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�]�w "+releasetime+" �p�ɤ�����g�ѡu"+ip+"�v�� IP ��m�n�J�����a���O����t�סC");
			else
				CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�]�w�ä[����g�ѡu"+ip+"�v�� IP ��m�n�J�����a���O����t�סC");
			if( note )
			{
				IP_D->set_ip_note(me, ip, note);
				CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�Х� IP�u"+ip+"�v���]�w���Ѭ��G"+note+"�C");
			}
			break;
		case "ok":
			if( IP_D->query_ip(ip) )
				return tell(me, "IP�u"+ip+"�v�w���]�w����A�Х��R���]�w�C\n");
				
			IP_D->set_ip(me, ip, OK, releasetime*3600);
			
			if( releasetime )
				CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�]�w "+releasetime+" �p�ɤ��u"+ip+"�v�� IP ��m���ӦX�k�L���C");
			else
				CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�]�w�u"+ip+"�v�� IP ��m���ӦX�k�L���C");
			
			if( note )
			{
				IP_D->set_ip_note(me, ip, note);
				CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�Х� IP�u"+ip+"�v���]�w���Ѭ��G"+note+"�C");
			}
			break;	
		case "-d":
			if( !IP_D->query_ip(ip) )
				return tell(me, "IP�u"+ip+"�v�쥻�N�S�����󭭨�]�w�C\n");
				
			IP_D->delete_ip(me, ip);
			CHANNEL_D->channel_broadcast("sys", me->query_idname()+"���� IP�u"+ip+"�v���Ҧ��]�w����C");
			break;
		default:
			return tell(me, "�S�� "+option+" �o�ӳ]�w�ﶵ�C\n");	
	}	
}