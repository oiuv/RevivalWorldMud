/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ipuse.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-10
 * Note   : ipuse ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
ipuse			�d�ߩҦ� IP �ϥΰO��
ipuse -m		�d�ߨϥάۦP IP �n�J���P���a���O��
ipuse -stat		�d�ߨϥάۦP IP �n�J�W�L 3 �줣�P���a���O��
ipuse -i <IP ��m>	�d�ߴ��ϥθ� IP ��m�n�J�����a
			�Ψ�Ҧ��i��ϥιL����L IP
ipuse <ID>		�d�ߴ��ϥθ� ID �n�J�� IP ��m
			�Ψ�Ҧ��i��ϥιL����L ID �� IP
HELP;

string *workstations = ({ "140.137.16.70", "140.134.4.20", "140.126.8.4", "140.134.4.210" });
private nosave mapping caller = allocate_mapping(0);

int sort_ip(mapping idx, string ip1, string ip2)
{
	if( idx[ip1] > idx[ip2] ) return 1;
	return -1;
}
void callback(string address, string resolved, int key)
{
        string msg;

	msg = resolved + " => " + (address ? address : "Unknown") + "\n";

        if( objectp(caller[key]) )
        {
                tell(caller[key], msg, CMDMSG);
                caller[key]->finish_input();
        }
        
        map_delete(caller, key);
}
private void command(object me, string arg)
{
	mapping ipuse = LOGIN_D->query_ip_use();
	int i;

	if( !arg || arg == "-m" )
	{
		string *id = allocate(0), *last_id = allocate(0);
		string *msg = ({});
		string *ipsort = keys(ipuse);
		ipsort = filter_array(ipsort, (: stringp($1) :));
		ipsort = sort_array(ipsort, (: strcmp($1, $2) :));
		foreach(string ip in ipsort)
		{
			last_id = id;
			id = ipuse[ip];
			if( sizeof(id) > 1 )
				msg += ({ sprintf(" %-24s%s"HIY"%s\n"NOR,
					ip,
					sizeof(last_id & id) ? HIR:"",
					implode(id, (: sprintf("%-12s%-12s", capitalize($1), capitalize($2)) :))) });
			else if( arg != "-m" )
				msg += ({ sprintf(" %-24s%s%s\n"NOR,
					ip,
					sizeof(last_id & id) ? HIR:"",
				capitalize(id[0])) });
				
		}
		msg = ({ HIW"�Ҧ� IP �n�J�����C��p�U�G\n"NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR }) +msg+ ({ NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR });
		
		me->more(implode(msg, ""));
		return;
	}
/*
	else if( sscanf(arg, "-ws %d", i) )
	{
		string *used_ip_res = allocate(0);
		foreach(string ip, string *ids in ipuse)
		{
			if( sizeof(ids) < i ) continue;
			used_ip_res |= ({ ip + " (" + sizeof(ids) + ")" });
			tell(me, "Resolving ip: " + ip + "\n");
			caller[resolve(ip, "callback")] = me;;
		}
		tell(me, "�i�ର�u�@���� IP ���G\n"HIY+implode(used_ip_res, ", \n")+NOR"\n");
		
		return;
	}
*/
	else if( sscanf(arg, "-i %s", arg) )
	{
		string *used_ip = allocate(0);
		string *used_ip_res = allocate(0);

		if( !sizeof(ipuse[arg]) )
			return tell(me, "�S���H�ϥιL ["+arg+"] �� IP ��m�n�J�C���C\n");

		tell(me, "�ϥιL IP ��m ["+arg+"] �����a�G\n"HIY+implode(ipuse[arg], ", \n")+NOR"\n");
		
		foreach(string id in ipuse[arg])
			foreach(string ip, string *ids in ipuse)
			{
				if( member_array(ip, workstations) != -1 ) continue;
				if( member_array(id, ids) == -1 ) continue;
				used_ip |= ({ ip });
				used_ip_res  |= ({ ip + " (" + sizeof(ids) + ")" });
			}
		tell(me, "IP ��m ["+arg+"] �i��ϥιL IP�G\n"HIY+implode(used_ip_res, ", \n")+NOR"\n");
		return ;
	}
	else if( arg == "-stat" )
	{
		mapping ip_index = allocate_mapping(0);
		string *ipsort = keys(ipuse);
		string msg;
		//int i;
		foreach(string key, array ids in ipuse)
		{
			if( (i = sizeof(ids)) < 3 ) continue; 
			ip_index[key] = i;
		}
		//foreach(string ip, i in ip_index) tell(me, sprintf("%s %d\n", ip, i));
		//tell(me, "Count: " + sizeof(ip_index) + "\n");
		//return;
		ipsort = sort_array(keys(ip_index), (: sort_ip($(ip_index), $1, $2) :));
		msg = "IP �ϥβέp��Ʀp�U: \n";
		foreach(string ip in ipsort)
		{
			int j=0;
			msg += sprintf("%s (%d): \n"HIY, ip, ip_index[ip]);
			foreach(string id in ipuse[ip])
			{
				msg += sprintf(" %-12s%s", id, (!(++j%7) ? "\n":""));
			}
			msg += NOR"\n";
		}
		tell(me, msg);
		return;
	}
	else
	{
		string msg, *addrs = allocate(0), *addr_res = allocate(0);
		mapping used_id = allocate_mapping(0);
		
		foreach(string ip, string *id in ipuse)
		{
			if( !ip || member_array(arg, id) == -1 ) continue;
			if( member_array(ip, workstations) != -1 ) continue;
			addr_res += ({ ip + " (" + sizeof(id) + ")" });
			addrs += ({ ip });
		}
		if( !sizeof(addrs) )
			return tell(me, "�ثe�S�� " + arg + " �� IP �n�J�����C\n");
		tell(me, "�ϥιL���a ["+arg+"] �� IP ��m�G\n"HIY+implode(addr_res, ", \n")+NOR"\n");

		foreach(string addr in addrs)
		{
			if( !addr ) continue;
			foreach(string id in ipuse[addr])
			{
				if( undefinedp(used_id[id]) ) used_id[id] = allocate(0);
				used_id[id] += ({ addr });
			}
		}
		msg = "���a ["+arg+"] �i��ϥιL�� ID�G\n";
		
		foreach(string id, array addr in used_id)
		{
			string addrs_desc = implode(addr, ", ");
			msg += sprintf(HIY"%-12s"NOR" %s\n", id, addrs_desc);
		}
		tell(me, msg);
		return;
	}
}                                                                