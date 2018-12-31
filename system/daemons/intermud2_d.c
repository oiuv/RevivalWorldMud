/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : intermud2_d.c
 * Author : Cookys@RevivalWorld
 * Date   : 2001-01-22
 * Note   : �өM�ª� Intermud2 ���q���s���{�� , �ﵽ�즳 DNS_MASTER
 *	    ���� Bug , �j�T��g, �w�p�N�H intersys.c ��X��( I2, I3, 
 *          new protocol ).
 * Update :
 *  o 2001-01-22 Cookys �V�| , �n�����|�� DNS_FAKE �X�{�F�O :Q
 *  o 2001-01-22 Cookys �۰ʥ[�J�F������ mud �ΧR���T�ѨS�^����mud
 *  o 2001-01-24 Cookys ���C�i�J mudlist �з�, �ç��� mudlist a/q ,
 *			�{�b�@�{������ mudlist �H�~������ service
 *			request.(ping ���~) 
 *  o 2001-01-24 Cookys gwizmsg , warning , startup ����
 *  o 2001-01-28 Cookys affirmation gtell support locate ����
 *  o 2001-02-03 Cookys �ѨM incoming_mudlist �y���L���j��eping���D
 *  o 2001-07-25 Cookys �W�[�T���i�J�����᪺�L�o[�ϥ� ansi package]
 * 
 * �`�N   : ���קK��ۤv mud �L���e ping �����ΡA�Цb OS �t�Τ��� /etc/hosts �����T�]�w�C
 *
 -----------------------------------------
 */

/*	�� i2d.c �t���̷� Intermud 2 Protocol �Ҩ�w���榡�ӶǰT�����F
**	�ΨӳB�z MUD ����~�q�T ,�ѩ� DNS_MASTER �L����� ,�ҥH����g�F
**	�ӷs�� ,���L�ѩ� UDP �����ݩ󤣥i�a�ʪ��ǿ�覡 ,��g�ᤣ�z����
**	�k�����N�O�@�ӳ̤j���|�} ,���M�������άY�ǤH�����覡�ȮɳB�z ,
**	���L�ڥ����ѨM���D�٬O���W TCP �̮ڥ�...
**	PS. I3 ���G�O�ĥ��� IP �[�c, ���L router �t��ӭ�...
**	    �ثe���⦳�ũw�X�@�M�� TWMUD �q���w�����q�T��w :P
**	mud_status �ĥΦ줸�B��, �`�ٮį�.
**
*/

/*********************<   i2d.h     >*********************
#define MUD                     0
#define STREAM                  1
#define DATAGRAM                2
#define STREAM_BINARY           3
#define DATAGRAM_BINARY         4
//-----------------------------------------------
#define I2D_MUD_SERVER 		({"203.71.88.240",4004})
#define MUDLIST_UPDATE_INTERVAL 600
#define REFRESH_INCOMING_TIME 	3*60*60
//----------------------�줸�B���-------------------------
#define GB_CODE 	1
#define ANTI_AD 	2
#define IGNORED 	4
#define SHUTDOWN 	8
#define ONLINE 		16
#define DISCONNECT 	32
***********************************************************/

#include <ansi.h>
#include <i2d.h>
#include <socket_err.h>
#include <origin.h>
#include <mudlib.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit DATABASE;

#define NCH_CHANNEL(x) CHANNEL_D->channel_broadcast("nch", "I2D ���F�G"+(string)x)

mapping mudlist=([]);
nosave mapping incoming_mudlist=([]);	
string save_path = "/data/network/";
nosave string my_address;
nosave int udp_port=4004;
int udp_socket,refresh_limit=time(),debug;

// prototype
varargs string SAVE_PATH(string name,int flag);
varargs string *fetch_mudname(string arg,int flag);
string get_mud_name(mapping info);
int compare_mud_info(mapping info);

void receive_mudlist_request(mapping info);
void receive_mudlist_answer(mapping info);
void receive_ping_request(mapping info);
void receive_ping_answer(mapping info);
void receive_gchannel_msg(mapping info);
void receive_shutdown(mapping info);
void receive_gwiz_msg(mapping info);
void receive_warning(mapping info);
void receive_startup(mapping info);
void receive_gtell(mapping info);
void receive_affirmation(mapping info);
void receive_support_request(mapping info);
void receive_support_answer(mapping info);
void receive_locate_request(mapping info);
void receive_locate_answer(mapping info);
void receive_rwho_answer(mapping info);
void receive_rwho_request(mapping info);


void send_ping_request(string target, int port);
void send_mudlist_request(string target, int port);
void read_callback(int socket,string info,int address);
void send_shutdown(string target, int port);
void send_startup(string targ,int port);
varargs void send_warning(string targ,int port,string msg,string fakehost);
void refresh_ping_buffer();
void refresh_buffers();
private void set_mudlist(string name,mapping info);
private void set_incoming_mudlist(string name,mapping info);
private void resolve_callback(string addr, string resolved, int key);

//-------------------------------------------------------------------------------------------
nosave mapping service_handler = ([
    "mudlist_q" : 	(: receive_mudlist_request :),
    "mudlist_a" : 	(: receive_mudlist_answer :),
    "ping_q" : 		(: receive_ping_request :),
    "ping_a" : 		(: receive_ping_answer :),
    "gchannel" :		(: receive_gchannel_msg :),
    "shutdown" :		(: receive_shutdown :),
    "gwizmsg" :		(: receive_gwiz_msg :),
    "warning" :		(: receive_warning :),
    "startup" :		(: receive_startup :),
    "gtell"	:	(: receive_gtell :),
    "affirmation_a":	(: receive_affirmation :),
    "support_q" :		(: receive_support_request :),
    "support_a" : 	(: receive_support_answer :),
    "locate_q" :		(: receive_locate_request :),
    "locate_a" :		(: receive_locate_answer :),
    "rwho_a" :		(: receive_rwho_answer :),
    "rwho_q" :		(: receive_rwho_request :),

]);	
nosave mapping ping_buffer=([]);
nosave mapping affirmation_buffer=([]);
nosave mapping rwho_buffer=([]);
nosave mapping gtell_buffer=([]);
nosave mapping msg_buffer=([]);
//-------------------------------------------------------------------------------------------

#define GTELL_B 	1
#define PING_B 		2
#define AFFIRMATION_B 	3
#define RWHO_B 		4

void restore()
{
	restore_object(SAVE_PATH("i2d"));
}

int save()
{
	return save_object(SAVE_PATH("i2d"));	
}

// �N�S status  �������]�� 0
/*
void make_status()
{
	foreach(string t , mapping ref d in mudlist)
	if(undefinedp(d["STATUS"])) d["STATUS"]=0;

	foreach(string t , mapping ref d in incoming_mudlist)
	if(undefinedp(d["STATUS"])) d["STATUS"]=0;
}
*/
int check_dns_fake(string name)
{
	return sizeof(fetch_mudname(name))-1;	
}

// �۰ʧ�s incoming_mudlist �� mudlist
private void refresh_incoming()
{
	string name;
	mapping info;
	foreach(name , info in incoming_mudlist)
	if(info["CONNECTION"] > (REFRESH_INCOMING_TIME/MUDLIST_UPDATE_INTERVAL)-5 )
	{
		map_delete(info,"CONNECTION");
		mudlist[name]=info;
		map_delete(incoming_mudlist,name);
	}
	foreach( name,info in mudlist)
	if( info["HOSTADDRESS"]!=my_address && ((time()-info["LASTESTCONTACT"]> 259200) || 
		(time()-info["LASTESTCONTACT"]>5*60*60 && check_dns_fake(info["NAME"])>0 ) ))
		map_delete(mudlist,name);
	incoming_mudlist=([]);
	refresh_limit=time();		

}
/*
void ooxx()
{
	evaluate((:refresh_incoming :));	
}
*/
/*void gan(string mud)
{
	map_delete(mudlist,mud);	
}*/

void send_all_shutdown()
{
	string mud;
	mapping info;
	// ���o mudlist �ҧt����T
	if( mapp(mudlist) )
		foreach(mud, info in mudlist)
		send_shutdown(
		    info["HOSTADDRESS"],
		    info["PORTUDP"]
		);

	// ���o incoming_mudlist �ҧt��T    	
	if( mapp(incoming_mudlist) )
		foreach(mud, info in incoming_mudlist)
		send_shutdown(
		    info["HOSTADDRESS"],
		    info["PORTUDP"]
		);
}

// �w�ɧ�s mud ��T
private void update_info()
{
	string mud;
	mapping info;
	// ���o mudlist �ҧt����T
	if( mapp(mudlist) )
		foreach(mud, info in mudlist)
		send_ping_request(
		    info["HOSTADDRESS"],
		    info["PORTUDP"]
		);
	// ���o incoming_mudlist �ҧt��T    	
	if( mapp(incoming_mudlist) )
		foreach(mud, info in incoming_mudlist)
		send_ping_request(
		    info["HOSTADDRESS"],
		    info["PORTUDP"]
		);
	refresh_buffers();
	// �b REFRESH_INCOMING_TIME �ҳ]���ɶ���۰ʥ[�J�ŦX���� mud
	if( time()- refresh_limit > REFRESH_INCOMING_TIME )
	{
		refresh_incoming();

		// �A�����o incoming_mudlist
		foreach(string t, mapping d in mudlist) 
		send_mudlist_request(d["HOSTADDRESS"],d["PORTUDP"]);
	}

	// �w�� MUDLIST_UPDATE_INTERVAL ���s�@����T
	call_out((: update_info :),MUDLIST_UPDATE_INTERVAL);
}

// �@�}�l�q�o�Q�I�s , �o�S���D�a :Q
void create()
{
	// ���^ save �U�Ӫ����
	restore();
	resolve( query_host_name(), (: resolve_callback :) );
	// ��l�� UDP port
	if( (udp_socket = socket_create(DATAGRAM, (: read_callback :) )) < 0 )
	{
		NCH_CHANNEL("UDP socket created fail.\n");
		error("Unable to create UDP socket.\n");
	}
	while( EEADDRINUSE==socket_bind(udp_socket, udp_port) )
		udp_port++;

	NCH_CHANNEL("��l�Ƨ��� , �ϥ� UDP port: "+udp_port);
	refresh_buffers();
	// �I�s�w�ɧ�s���禡
	refresh_ping_buffer();
	// �e�X startup 
	foreach(string mud,mapping info in mudlist)
	{
		mudlist[mud]["STATUS"]&=~ENCODE_CONFIRM;
		send_startup(info["HOSTADDRESS"],info["PORTUDP"]);
	}
		
	// �e�X ping �n�D�� server        	
	//send_ping_request(I2D_MUD_SERVER[0],I2D_MUD_SERVER[1]);
	// �V�w�] server ���o mudlist
	//send_mudlist_request(I2D_MUD_SERVER[0],I2D_MUD_SERVER[1]);
	// ����@�� incoming_mudlist
	foreach(string t, mapping d in mudlist) 
	send_mudlist_request(d["HOSTADDRESS"],d["PORTUDP"]);
	// �}�l�w����s��T
	evaluate((: update_info :));
}

// destruct �� simul_efun �|�� call �o��
int remove()
{
	send_all_shutdown();
	socket_close(udp_socket);
	return save();
}


private void resolve_callback(string addr, string resolved, int key)
{
	my_address = resolved;
}

// UDP port ���i�Ӫ��F��|���g�L�o
private void read_callback(int socket,string info,int address)
{
	string *infoclip,title,datas;
	mixed handler;
	mapping infomap=([]);
	
#ifdef __PACKAGE_ANSI__
	info=ansi(info);
#endif	
	
	NCH_CHANNEL( sprintf( "socket %d\ninfo %s\naddress %d", socket, info, address ));
	//NCH_CHANNEL(info+"\t\t"+address);
	// �ˬd�ʥ]�榡�O�_���T
	if(!sscanf(info,"@@@%s@@@%*s",info)) return;
	// ��@�B��M�ݪ� service ���R
	infoclip = explode(info,"||");
	// �����L�k�B�z���A�Ȧb�o�L�o
	//NCH_CHANNEL(sprintf("%O\n",service_handler[infoclip[0]]));
	if(!infoclip || undefinedp(handler = service_handler[infoclip[0]]) )
	{
		NCH_CHANNEL(sprintf("Unknow I2 service %s from %s ",infoclip[0],address));
		return;
	}	
	// �N string -> mapping �H�K���U�ǻ�		
	foreach(string data in infoclip)
	if( sscanf(data, "%s:%s", title, datas)==2 ) infomap[title] = datas;
	// �N�o�e�� package �� IP ������ mapping info ��
	sscanf(address, "%s %*s", address);
	if( (address+"" == my_address || address+""=="127.0.0.1")&& infomap["PORTUDP"]==udp_port+"" )
		return;
	infomap["HOSTADDRESS"] = address;
	if(debug) NCH_CHANNEL(sprintf("����%s\n%O\n",address,info));
	// ��I�� service �M�Ϊ� function        
	//call_other(this_object(),handler,infomap);
	
	evaluate(handler,infomap);

}

// �e�X�T���Ϊ��禡
void send_udp(string targ, mixed port, string service, mapping info)
{
	int socket;
	string title,data,msg;

	// ���ǰe�ڭ̨S�����A��
	if( undefinedp(service_handler[service]) )
	{
		NCH_CHANNEL("Unknow Service.");
		error("Invalid intermud Services.\n");
	}	

	// ���ǰe��Ƶ��ۤv.
	//if( targ == my_address && port == fetch_variable("udp_port") ) return;
	//NCH_CHANNEL(sprintf("ip:%s port:%s services:%s",targ,port,service));
	//NCH_CHANNEL(sprintf("%O",typeof(port)));
	if( (targ == my_address || targ=="127.0.0.1") && port == udp_port )
		//	if( service!="ping_a" && service!="ping_q" )
		return;


	// �إ� socket �H�}�l�ǰe 	
	socket = socket_create(DATAGRAM, "read_callback");
	if( !socket ) return;

	// �N mapping -> string
	msg = service;
	if( mapp(info) ) {
		foreach(title, data in info) {
			if( !stringp(title) || !stringp(data) ) continue;
			msg += sprintf("||%s:%s",title,data);
		}
	}
	if( !undefinedp(mudlist[targ+":"+port]) && ( (mudlist[targ+":"+port]["STATUS"]& GB_CODE) || ( !undefinedp(mudlist[targ+":"+port]["ENCODING"]) && lower_case(mudlist[targ+":"+port]["ENCODING"])=="gb") ))
		//if( (mudlist[targ+":"+port]["STATUS"]& GB_CODE) || ( !undefinedp(mudlist[targ+":"+port]["ENCODING"]) && lower_case(mudlist[targ+":"+port]["ENCODING"])=="gb") )
		msg = B2G(msg);
	// �e�X�T��.
	socket_write(socket, "@@@" + msg + "@@@", targ + " " + port);
	// debug msg
	if(debug) NCH_CHANNEL(sprintf("�e�X��%s:\n%s\n"+HIY+"%O\n"+NOR,targ,port+"",msg));
	// �����ǰe������ socket
	socket_close(socket);


}
// �� mapping package �����o mud name
string get_mud_name(mapping info)
{
	return info["HOSTADDRESS"]+":"+info["PORTUDP"];	
}

// �� mud name ��{���ĴX�Ÿ��
int get_info_level(string mudname)
{
	if( member_array(mudname,keys(mudlist))==-1)	
	{
		if(member_array(mudname,keys(incoming_mudlist))==-1)
			return -1;
		else 	return 0;
	}
	else return 1;
}

void analyze_mud_info(mapping info)
{
	int info_level=get_info_level(get_mud_name(info));

	switch(info_level) {
	case -1: set_incoming_mudlist(get_mud_name(info),info);break;
	case  0: set_incoming_mudlist(get_mud_name(info),info);break;
	case  1: set_mudlist(get_mud_name(info),info);break;
	}		
}
// �Ω�j�����禡, ���R Mud �O�_���w�q�L�{��, �|���{�Ұe pin_q
int compare_mud_info(mapping info)
{
	string mudname=get_mud_name(info);
	int level=get_info_level(mudname);

	if( level < 1 )
		send_ping_request(info["HOSTADDRESS"],info["PORTUDP"]);
	else if( mudlist[mudname]["STATUS"] &(SHUTDOWN | DISCONNECT))
	{
		mudlist[mudname]["STATUS"]&=(~(SHUTDOWN | DISCONNECT));
		mudlist[mudname]["STATUS"]|=ONLINE;
	}	
	return level;
}


// �]�w or ��s mudlist ��T
private void set_mudlist(string name,mapping info)
{
	mapping map;
	string t,d;

	// �� IPADDRESS:PORT �� index , ���� fake , �Y��Ƥ��X�h�ױ�
	if( sscanf(name, "%*d.%*d.%*d.%*d:%*d") != 5 ) return;
	// �٨S������ mud ���?
	if( undefinedp(map = mudlist[name]) ) 
	{
		// ���w mudname
		mudlist[name] = info;
		// ��l�� mud STATUS
		mudlist[name]["STATUS"]=0;
		return;
	}

	// �w�s�b���,��s��T ...
	foreach(t, d in info) mudlist[name][t]=d;

}
// �]�w or ��s incoming mudlist ��T
private void set_incoming_mudlist(string name,mapping info)
{
	mapping map;
	string t,d;

	if( sscanf(name, "%*d.%*d.%*d.%*d:%*d") != 5 ) return;
	if( undefinedp(map = incoming_mudlist[name]) ) 
	{
		incoming_mudlist[name] = info;
		incoming_mudlist[name]["CONNECTION"]=1;
		return;
	}

	// ��s��T 
	foreach(t, d in info) incoming_mudlist[name][t]=d;
	// �C����s��T�s�u�^���p�� + 1
	incoming_mudlist[name]["CONNECTION"]+=1;	
}

// �M���Ȧs��
void refresh_buffers()
{
	rwho_buffer=([]);
	gtell_buffer=([]);
	affirmation_buffer=([]);
	//refresh_ping_buffer();
	msg_buffer=([]);
}

/********************************************************************************************
 * �ѥ~�ɯ�]�w�������p�����q�禡..							    *
 ********************************************************************************************/

// SAVE_PATH �Y�ǤJ flag , �Y�ΥH��s SAVE_PATH
varargs string SAVE_PATH(string name,int flag)
{
	if(!flag) return save_path+name;
	else
	{
		save_path = name;
		save();
		return "SAVE_PATH "+save_path+" updated successfully.\n";
	}
}

void debug()
{
	if(!debug)
		debug =1;
	else debug=0;
}

//--------------------------------------------------------------------------------------------
// Status �t�C
//--------------------------------------------------------------------------------------------

// ��"ip:port"�ӴM�� mud data
mixed fetch_data(string name)
{
	int level;
	level = get_info_level(name);
	switch(level){
	case -1: return 0;
	case 1: return mudlist[name];
	case 0: return incoming_mudlist[name];
	}
	return 0;
}

// �έ^��W�M��n�O�W , �Ǧ^�ŦX���}�C
varargs string *fetch_mudname(string arg,int flag)
{
	mapping info;
	string name,*result=({});
	if( flag ) arg=lower_case(arg);
	foreach(name , info in (mudlist+incoming_mudlist))
	if(!flag)
	{
		if( arg==info["NAME"] )
			result+=({name});
	}		
	else	if( strsrch(lower_case(info["NAME"]),arg)!=-1 )
		result+=({name});
	return result;
}
// �� mud ip �ӴM��n�O�W , �Ǧ^�ŦX�}�C
varargs string *fetch_mudip(string arg)
{
	mapping info;
	string name,*result=({}),ip;
	//if( flag ) arg=lower_case(arg);
	foreach(name , info in (mudlist+incoming_mudlist))
	{
		sscanf(name,"%s:%*s",ip);
		if( arg == ip )
			result+=({name});
	}
	return result;
}

// �]�w mudlist �̪� mud �]�w
int set_status(string mud,int num)
{
	int level;
	level = get_info_level(mud);
	if(level!=1 ) return 0;

	if( num > 0)
	{	
		// �w�g�������]�w
		if(mudlist[mud]["STATUS"] & num)
			return 0;
		else mudlist[mud]["STATUS"]+=num;
	}
	else
	{	// �w�g�������]�w	
		if(!mudlist[mud]["STATUS"]&num)
			return 0;
		else mudlist[mud]["STATUS"]-=(-num);
	}
	NCH_CHANNEL(sprintf("%s (%s) �[�J�ݩ� [%d]",mudlist[mud]["NAME"],mud,num));
	save();
	return 1;
}

class channel_class
{
	int level, number;
	string msg;
	mapping extra;
}

int accept_channel(string channel)
{
	class channel_class ch;
	mapping channels=fetch_variable("channels",load_object(CHANNEL_D));
	if( !undefinedp(channels[channel]) && !undefinedp((ch=channels[channel])->extra["intermud"]) )
		return 1;
}

void add_buffer(string mudname,int kind)
{
	//if(get_info_level(mudname)<1)
	//	return;
	switch(kind)
	{
	case GTELL_B:	
		if(undefinedp(gtell_buffer[mudname]))
			gtell_buffer[mudname]=1;
		else 	
			gtell_buffer[mudname]++;
		break;
	case PING_B:	
		if(undefinedp(ping_buffer[mudname]))
			ping_buffer[mudname]=({1,time()});
		else 	
		{
			ping_buffer[mudname][0]++;
			ping_buffer[mudname][1]=time();
		}
		break;
	case RWHO_B:
		if(undefinedp(rwho_buffer[mudname]))
			rwho_buffer[mudname]=1;
		else 	
			rwho_buffer[mudname]++;
		break;
	case AFFIRMATION_B:
		if(undefinedp(affirmation_buffer[mudname]))
			affirmation_buffer[mudname]=1;
		else 	
			affirmation_buffer[mudname]++;
	}
}


void sub_buffer(string mudname,int kind)
{
	//if(get_info_level(mudname)<1)
	//	return;
	switch(kind)
	{
	case GTELL_B:	
		if(undefinedp(gtell_buffer[mudname]))
			return;
		else
		{ 	
			gtell_buffer[mudname]--;
			if(gtell_buffer[mudname]<1)
				map_delete(gtell_buffer,mudname);
		}

		break;
	case PING_B:	
		if(undefinedp(ping_buffer[mudname]))
			return;
		else 
		{	
			ping_buffer[mudname][0]--;
			if( ping_buffer[mudname][0] < 1)
				map_delete(ping_buffer,mudname);
		}
		break;
	case RWHO_B:
		if(undefinedp(rwho_buffer[mudname]))
			return;
		else 	
		{
			rwho_buffer[mudname]--;
			if( rwho_buffer[mudname] < 1)
				map_delete(ping_buffer,mudname);
		}
		break;
	case AFFIRMATION_B:
		if(undefinedp(affirmation_buffer[mudname]))
			return;
		else 	
		{
			affirmation_buffer[mudname]--;
			if(affirmation_buffer[mudname]<1)
				map_delete(affirmation_buffer,mudname);
		}
	}
}

int test_buffer(string mudname,int kind)
{
	switch(kind){
	case GTELL_B:
		if( !undefinedp(gtell_buffer[mudname]) ) return 1;
		break;
	case PING_B:
		if( !undefinedp(ping_buffer[mudname]) ) return 1;
		break;
	case RWHO_B:
		if( !undefinedp(rwho_buffer[mudname]) ) return 1;
		break;
	case AFFIRMATION_B:
		if( !undefinedp(affirmation_buffer[mudname]) ) return 1;
		break;
	}
	return 0;
}
/********************************************************************************************
 * �Ѧ��B�}�l , �H�U�����U�تA�����O							    *
 ********************************************************************************************/

//--------------------------------------------------------------------------------------------
// Ping �t�C
//--------------------------------------------------------------------------------------------
/* Intermud2 ping_q �ʥ]�榡
	([
		"NAME":		(string)�o Mud �������W��[�V��]
				�ª� DNS_MASTER �p�G�J��ۦP�| DNS_FAKE.
		"PORTUDP":      (int)(��᳣̫�|�Q�]��string)
				�o��mud��intermud2 daemon ��ť�� udp port.
	])

    Intermud2 ping_a �ʥ]�榡	
	([
		"NAME":         (string)�o Mud �������W��
		"MUDGROUP":	(string)�����s��?
		"HOST":         (string)Mud �D���W��?(�n���S��, ��~�@��)
		"LOCATION":	(string)�Ҧb�a?
		"MUDLIB":       (string)mudlib, �p�G���OEastern Stories�|������ es channel.
		"VERSION":      (string)mudlib ����?
		"ENCODING":     (string)�s�X�覡�� BIG5/GB,
		"PORTUDP":      (int) UDP port,
	])
*/


// ping_buffer �ΨӾױ������n�� ping request
// �קK�Q save ���O�귽�Ŧ� nosave


// �o�X Ping �n�D
void send_ping_request(string target, int port)
{
	send_udp(target, port, "ping_q", ([
		"NAME":         INTERMUD_MUD_ENGLISH_NAME,
		"PORTUDP":      ""+fetch_variable("udp_port"),
	    ]));
	//if(get_info_level(target+":"+port)>0)
	add_buffer(target+":"+port,PING_B);
	//NCH_CHANNEL("�e�X PING �n�D�� " + target + ":" + port + "�C");
}

void refresh_ping_buffer()
{
	string t;
	int *d;
	// �A�����榹�禡�ɤ��n�� callout �٦b�]
	remove_call_out("refresh_ping_buffer");

	foreach(t,d in ping_buffer)
	if(time()-d[1] > 90)
	{
		int flag;
		//NCH_CHANNEL("peer...."+sprintf("%O%O\n",t,mudlist[t]["STATUS"]));
		if( get_info_level(t)==1 && !(mudlist[t]["STATUS"]&SHUTDOWN))
		{

			mudlist[t]["STATUS"]&=(~ONLINE);
			mudlist[t]["STATUS"]|=DISCONNECT;

			// 2 hr�����������@�� ping -.-
			if( time() - mudlist[t]["LASTESTCONTACT"] < 60*60*2 )
				flag=1;
		}
		map_delete(ping_buffer,t);
		if( flag )
		{
			send_ping_request(mudlist[t]["HOSTADDRESS"],mudlist[t]["PORTUDP"]);
			flag=0;
		}
	}
	call_out("refresh_ping_buffer",60);
}

// ���� ping request
void receive_ping_request(mapping info)
{
	int port;

	// UDP_PORT �榡���~
	if( undefinedp(info["PORTUDP"]) || !sscanf(info["PORTUDP"],"%d",port) ) return;
	// �p�G�ڭ� mudlist ���٨S������ mud , �ڭ̤]�n�D�L�^ ping.
	if( info["HOSTADDRESS"]!="127.0.0.1" )
		if(get_info_level(get_mud_name(info))<0)
			compare_mud_info(info);
		// �e�X PING Answer
	send_udp(info["HOSTADDRESS"], port, "ping_a", 
	    ([
		"NAME":         INTERMUD_MUD_ENGLISH_NAME,
		"MUDNAME":	INTERMUD_MUD_CHINESE_NAME,
		"MUDGROUP":	MUD_GROUP,
		"HOST":         MUD_HOST_NAME,
		"LOCATION":	MUD_LOCATION,
		"DRIVER":	__VERSION__,
		"MUDLIB":       MUDLIB_NAME,
		"VERSION":      MUDLIB_VERSION_NUMBER,
		"ENCODING":     ENCODING,
		"USERS":	sizeof(users()),
		"PORTUDP":      ""+fetch_variable("udp_port"),
		"PORT":		""+__PORT__,
	    ]));

	//NCH_CHANNEL("���� PING �n�D , �e�X�^���� "+info["HOSTADDRESS"]+":"+port+" .");
}

// ���� PING �^��
void receive_ping_answer(mapping info)
{
	string mudname=get_mud_name(info);
	// ���n�D���^ ping ��?
	if( !test_buffer(mudname,PING_B) ) 
	{
		//if( !sscanf(info["PORTUDP"],"%*d") ) return;
		NCH_CHANNEL("���줣�� Ping Answer From: "+info["HOSTADDRESS"]+":"+info["PORTUDP"]+" [ "+info["NAME"]+" ] ");
		
		if(get_info_level(mudname)<1)
			receive_ping_request(info);
			
		send_warning(info["HOSTADDRESS"],info["PORTUDP"],"\aWe didn't ask for this ping request.","Sorry,");
		//compare_mud_info(info);
		return;
	}
	//if(get_info_level(mudname)>0)
	sub_buffer(mudname,PING_B);
	info["LASTESTCONTACT"]=time();
	analyze_mud_info(info);
	if( get_info_level(mudname) == 1)
	{
		if( mudlist[mudname]["STATUS"] &(SHUTDOWN | DISCONNECT))
			mudlist[mudname]["STATUS"]&=(~(SHUTDOWN | DISCONNECT));
		mudlist[mudname]["STATUS"]|=ONLINE;
	}	
	//NCH_CHANNEL("���� "+info["HOSTADDRESS"]+" �e�^�� PING ANSWER .");
}

//--------------------------------------------------------------------------------------------
// Mudlist �t�C
//--------------------------------------------------------------------------------------------


// �e�X Mudlist �T���M�Ϊ��禡 -.-
void send_mudlist_udp(string targ, int port,string info)
{
	int socket;

	// ���ǰe��Ƶ��ۤv.
	if( targ == my_address && port == fetch_variable("udp_port") ) return;
	//if( targ == "140.117.201.101" ) return;

	// �إ� socket �H�}�l�ǰe 	
	socket = socket_create(DATAGRAM, "read_callback");
	if( !socket ) return;

	// �e�X�T��.
	socket_write(socket, "@@@mudlist_a||" + info + "@@@\n", targ + " " + port);
	// �����ǰe������ socket
	socket_close(socket);
}

// ������^���� mudlist �C��
void receive_mudlist_answer(mapping info)
{
	string name,clip;

	foreach(name,clip in info)
	{
		string *prop_list;
		mapping mudinfo=([]);
		if( strsrch(clip,"NAME")==-1 ) continue;

		prop_list=explode(clip,"|")-({""});
		foreach(string prop in prop_list) {
			string t, d;
			if( sscanf(prop, "%s:%s", t, d)==2 ) mudinfo[t] = d;
		}
		// ����S�b�M��W�� mud �N�e ping �n�D
		//if(mudinfo["HOSTADDRESS"]!=my_address && mudinfo["HOSTADDRESS"]!="127.0.0.1")
		if( get_info_level(get_mud_name(mudinfo))<1 )
			compare_mud_info(mudinfo);
	}

	//NCH_CHANNEL("���� "+info["HOSTADDRESS"]+" �e�^�� Mudlist Answer.");
}

// �n�D���o�e mudlist �C��
void send_mudlist_request(string targ, int port)
{
	send_udp(targ, port, "mudlist_q", ([
		"NAME":INTERMUD_MUD_ENGLISH_NAME,
		"HOSTADDRESS":  fetch_variable("my_address"),
		"PORTUDP":      "" + fetch_variable("udp_port"),
	    ]));
}

// ����n�D�ðe�X mudlist �C��
void receive_mudlist_request(mapping info)
{
	string name;
	mapping mudinfo;
	int i=1;
	if(!info["PORTUDP"]) return;
	if(get_info_level(get_mud_name(info))<1) return;

	foreach(name,mudinfo in mudlist)
	{
		// �S���Y�ɦ^�����N���i�ण�b, ���e�X�H�K�s�y�����U��
		if(!(mudinfo["STATUS"]&ONLINE)) continue;
		i++;
		send_mudlist_udp(info["HOSTADDRESS"],info["PORTUDP"],
		    sprintf("%d:|NAME:%s|HOST:%s|HOSTADDRESS:%s|PORT:%s|PORTUDP:%s",i,mudinfo["NAME"],mudinfo["HOST"],mudinfo["HOSTADDRESS"],mudinfo["PORT"],mudinfo["PORTUDP"])

		);
	}
	//NCH_CHANNEL("���� "+info["HOSTADDRESS"]+":"+info["PORTUDP"]+" �� Mudlist Request �æ^����.");
}

//--------------------------------------------------------------------------------------------
// Shutdown �t�C
//--------------------------------------------------------------------------------------------

void send_shutdown(string target, int port)
{
	send_udp(target, port, "shutdown", 
	    ([
		"NAME":INTERMUD_MUD_ENGLISH_NAME,
		"PORTUDP":""+fetch_variable("udp_port"),
	    ]));

	NCH_CHANNEL("�e�X Shutdown �T���� " + target + ":" + port + "�C");
}

void receive_shutdown(mapping info)
{
	//compare_mud_info(info);
	if( compare_mud_info(info)==1 )
	{
		string name=get_mud_name(info);

		mudlist[name]["STATUS"]&=(~(ONLINE|DISCONNECT));
		mudlist[name]["STATUS"]|=SHUTDOWN;
	}
	NCH_CHANNEL("����� "+info["HOSTADDRESS"]+":"+info["PORTUDP"]+" �Ӫ� Shutdown �T��.");
}

//--------------------------------------------------------------------------------------------
// Gchannel �t�C
//--------------------------------------------------------------------------------------------


void compare_last_msg(string mudname,string last_msg,string sender)
{
	if( undefinedp(msg_buffer[mudname]) || (msg_buffer[mudname][0]!=last_msg && msg_buffer[mudname][3]!=sender ))
	{
		//if( undefinedp(msg_buffer[mudname]) || msg_buffer[mudname][2] >= time()+15 ) 
		msg_buffer[mudname]=({ last_msg,1,time(),sender });
		
	}	else {
		if(msg_buffer[mudname][0]==last_msg || msg_buffer[mudname][3]==sender)
			msg_buffer[mudname][1]++;
		msg_buffer[mudname][2]=time();
		msg_buffer[mudname][3]=sender;
	}
	
	if( msg_buffer[mudname][1] > 100 )
	{
		set_status(mudname,ANTI_AD);
		NCH_CHANNEL("�N[ " + fetch_data(mudname)["MUDNAME"] +"] �[�J ANTI_AD �W��C");
	}
}


// ���� gchannel �T��
void receive_gchannel_msg(mapping info)
{
	string msg,id,mudname;
	int status;

	// �@�w�����n�������
	if( undefinedp(info["NAME"])
	    ||  undefinedp(info["PORTUDP"])
	    ||  undefinedp(info["USRNAME"])
	    ||  undefinedp(info["CHANNEL"])
	    ||  undefinedp(msg = info["MSG"])
	)   return;
	// �h���̫�@�� \n
	if(msg[<1..<0] == "\n") msg[<1..<0] = "";
	// �s�Ӫ������ڨĨıƶ��^ ping :Q
	if( compare_mud_info(info)<1 ) return;
	
	// �]�w id
	//id = info["USRNAME"] + "@"+ ((mudlist[mudname=get_mud_name(info)]["MUDNAME"])||"")+HBGRN"-"+mudlist[mudname]["NAME"]+"-"NOR;
	id = info["USRNAME"];
	// ���L���� name ?
	if( info["CNAME"] ) id = info["CNAME"] + "(" + id + ")";
	// �O�_�� Emote ?
	if( !info["EMOTE"] )
		set("channel_id",id);
	// GB ��X ?
	/*if(member_array(get_mud_name(info),gb_muds)!=-1)
	{
		id=G2B(id);
		msg=G2B(msg);
	}*/
	mudname = get_mud_name(info);
	status = fetch_data(get_mud_name(info))["STATUS"];
	
	if( !(status & ENCODE_CONFIRM) && strlen(msg) > 20 )
	{
		string mud_ip_port = info["HOSTADDRESS"]+":"+info["PORTUDP"];
		int con = LANGUAGE_D->encode_conjecture(msg);
		
		if( con >= 0 && con <= 15 ) mudlist[mud_ip_port]["STATUS"]|=GB_CODE;
		else if( con > 30 ) mudlist[mud_ip_port]["STATUS"]&=~GB_CODE;
		
		if( con != -1 ) mudlist[mud_ip_port]["STATUS"]|=ENCODE_CONFIRM;	
	}
	
	if( status & GB_CODE )
	{
		id=G2B(id);
		msg=G2B(msg);
	}
	if( status & ANTI_AD ) info["CHANNEL"]="ad";
	if( status & IGNORED ) return;
	if( info["CHANNEL"]!="ad")
	compare_last_msg(mudname,info["MSG"],id);
	
	// ��I�� CHANNEL_D �B�z
	if( accept_channel(info["CHANNEL"]) || info["CHANNEL"]=="ad")
		CHANNEL_D->channel_broadcast(info["CHANNEL"], info["EMOTE"]?("{"+HBGRN+mudlist[mudname]["NAME"]+NOR+"} "+ msg):("{"+HBGRN+mudlist[mudname]["NAME"]+NOR+"} "+ id+"�R"+msg));
	else 
		CHANNEL_D->channel_broadcast("other", info["EMOTE"]?("["+info["CHANNEL"]+"] "+"{"+HBGRN+mudlist[mudname]["NAME"]+NOR+"} "+msg):("["+info["CHANNEL"]+"] {"+HBGRN+mudlist[mudname]["NAME"]+NOR+"} "+id+"�R"+msg));
}

// �o�X gchannel �T��
void send_gchannel_msg(string channel, string id, string name, string msg, int emoted)
{
	mapping info,mudinfo;
	string t;

	if(msg[<1..<0] == "\n") msg[<1..<0] = "";

	msg = replace_string(msg, "|", "|\033[1m");
	msg = replace_string(msg, "@", "@\033[1m");
	info = ([
	    "NAME":         INTERMUD_MUD_ENGLISH_NAME,
	    "PORTUDP":      "" + fetch_variable("udp_port"),
	    "USRNAME":      id,
	    "CNAME":        name,
	    "CHANNEL":      channel,
	    "ENCODING":     ENCODING,
	    "MSG":          msg     			
	]);
	

	if( emoted ) info["EMOTE"] = "YES";

	foreach(t, mudinfo in (mudlist+incoming_mudlist))
	send_udp(mudinfo["HOSTADDRESS"], mudinfo["PORTUDP"],"gchannel", info);
}


//--------------------------------------------------------------------------------------------
// Gwizmsg �t�C
//--------------------------------------------------------------------------------------------

/*
info = ([
		"NAME" : INTERMUD_MUD_ENGLISH_NAME,
		"PORTUDP" : "" + fetch_variable("udp_port"),
		"WIZNAME" : id,
		"CNAME" : name,
		"CHANNEL" : ch,
		"ENCODING" : CHARACTER_SET,
		"GWIZ" : msg,
	]);

*/
void receive_gwiz_msg(mapping info)
{
	string msg,id,mudname;
	int status;

	// �@�w�����n�������
	if( undefinedp(info["NAME"])
	    ||  undefinedp(info["PORTUDP"])
	    ||  undefinedp(info["WIZNAME"])
	    ||  undefinedp(info["CHANNEL"])
	    ||  undefinedp(msg = info["GWIZ"])
	)   return;
	// �h���̫�@�� \n
	if(msg[<1..<0] == "\n") msg[<1..<0] = "";
	// �s�Ӫ������ڨĨıƶ��^ ping :Q
	if( compare_mud_info(info)<1 ) return;

	// �]�w id
	id = info["WIZNAME"];
	//id = info["WIZNAME"] + "@" + info["NAME"];
	// ���L���� name ?
	if( info["CNAME"] ) id = info["CNAME"] + "(" + id + ")";
	// �O�_�� Emote ?
	if( !info["EMOTE"] )
		set("channel_id",id);
	if( status & IGNORED ) return;
	// GB ��X ?
	mudname=get_mud_name(info);
	
	status = fetch_data(mudname)["STATUS"];
	
	if( !(status & ENCODE_CONFIRM) && strlen(msg) > 20 )
	{
		string mud_ip_port = info["HOSTADDRESS"]+":"+info["PORTUDP"];
		int con = LANGUAGE_D->encode_conjecture(msg);
		
		if( con >= 0 && con <= 15 ) mudlist[mud_ip_port]["STATUS"]|=GB_CODE;
		else if( con > 30 ) mudlist[mud_ip_port]["STATUS"]&=~GB_CODE;
		
		if( con != -1 ) mudlist[mud_ip_port]["STATUS"]|=ENCODE_CONFIRM;	
	}
		
	if( status & GB_CODE )
	{
		id=G2B(id);
		msg=G2B(msg);
	}
	if( status & ANTI_AD )
	{
		info["CHANNEL"]="ad";
		CHANNEL_D->channel_broadcast(info["CHANNEL"], info["EMOTE"]?("{"+HBGRN+mudlist[mudname]["NAME"]+NOR+"} "+ msg):("{"+HBGRN+mudlist[mudname]["NAME"]+NOR+"} "+ id+"�R"+msg));
		
		return;
	}

	// ��I�� CHANNEL_D �B�z
	CHANNEL_D->channel_broadcast("gwiz","["+info["CHANNEL"]+"] "+(info["EMOTE"]?("{"+HBGRN+mudlist[mudname]["NAME"]+NOR+"} "+msg):("{"+HBGRN+mudlist[mudname]["NAME"]+NOR+"} "+ id+"�R"+msg)));
	
	//NCH_CHANNEL("���� gwizmsg "+info["CHANNEL"]+" �T��");
}
void send_gwiz_msg(string channel, string id, string name, string msg, int emoted)
{
	mapping info,mudinfo;
	string t;

	if(msg[<1..<0] == "\n") msg[<1..<0] = "";
	msg = replace_string(msg, "|", "|\033[1m");
	msg = replace_string(msg, "@", "@\033[1m");	
	info = ([
	    "NAME":         INTERMUD_MUD_ENGLISH_NAME,
	    "PORTUDP":      "" + fetch_variable("udp_port"),
	    "WIZNAME":      id,
	    "CNAME":        name,
	    "CHANNEL":      channel,
	    "ENCODING":     ENCODING,
	    "GWIZ":          msg,
	]);
	if( emoted ) info["EMOTE"] = "YES";

	foreach(t, mudinfo in (mudlist+incoming_mudlist))
	send_udp(mudinfo["HOSTADDRESS"], mudinfo["PORTUDP"],"gwizmsg", info);
}

//--------------------------------------------------------------------------------------------
// Warning �t�C
//--------------------------------------------------------------------------------------------

void receive_warning(mapping info)
{
	string msg;

	if(compare_mud_info(info)<1)
		return;

	msg=sprintf("�� %s(%s) �i����ĵ�i: %s �� %s",info["NAME"],info["HOSTADDRESS"],info["MSG"],info["FAKEHOST"]);

	send_warning(info["HOSTADDRESS"],info["PORTUDP"],
	    sprintf("Thank For Your Warning [%s], we have loged it , and will try to solve it soon :).",msg));

	//write_file("/log/i2d_warning",msg+"\n");
	log_file("daemon/intermud2", "[warn]"+msg);
	NCH_CHANNEL(msg);
}

varargs void send_warning(string targ,int port,string msg,string fakehost)
{
	send_udp(targ, port, "warning", 
	    ([
		"NAME":INTERMUD_MUD_ENGLISH_NAME,
		"PORTUDP":""+fetch_variable("udp_port"),
		"MSG":msg,
		"FAKEHOST":fakehost?fakehost:"",
	    ]));

}

//--------------------------------------------------------------------------------------------
// Startup �t�C
//--------------------------------------------------------------------------------------------

// �e�X startup ,  �ϥ����W�N�e ping �F, �e�򥻪��N�n.
void send_startup(string targ,int port)
{
	send_udp(targ, port, "startup", 
	    ([
		"NAME":INTERMUD_MUD_ENGLISH_NAME,
		"PORTUDP":""+fetch_variable("udp_port"),
	    ]));
	// ��覬�� startup �|�^ ping_a
	add_buffer(targ+":"+port,PING_B);
	//NCH_CHANNEL("�e�X startup �T���� " + targ + ":" + port + "�C");
}

void receive_startup(mapping info)
{
	receive_ping_request(info);
	NCH_CHANNEL("����� "+info["HOSTADDRESS"]+":"+info["PORTUDP"]+" �Ӫ� startup �T��.");
}

//--------------------------------------------------------------------------------------------
// affirmation_a �t�C
//--------------------------------------------------------------------------------------------


void add_affirmation_buffer(mapping info,string service)
{
	string name=get_mud_name(info);

	service=lower_case(service);

	if(!undefinedp(affirmation_buffer[name+"-"+service]) )
		affirmation_buffer[name+"-"+service]++;
	else affirmation_buffer[name+"-"+service]=1;
}

void receive_affirmation(mapping info)
{
	string name=get_mud_name(info);
	object ob;

	if(compare_mud_info(info)<1)	
		return;
	if( undefinedp(info["NAME"])
	    ||  undefinedp(info["PORTUDP"])
	    ||  undefinedp(info["WIZTO"])
	    ||  undefinedp(info["MSG"])
	    ||  undefinedp(info["MSG"])
	)   return;

	if( undefinedp(affirmation_buffer[name+"-"+info["TYPE"]]))
	{

		send_warning(info["HOSTADDRESS"],info["PORTUDP"],
		    "Affirmation warning : Your mud send unrequest Affirmation_a Please improve it.");
		NCH_CHANNEL(sprintf("Unrequest affirmation answer from %s:%s(%s)   \n%O\n",info["HOSTADDRESS"],info["PORTUDP"]+"",info["NAME"],info));
		return ;

	}
	affirmation_buffer[name+"-"+info["TYPE"]]--;
	if( !ob=find_player(info["WIZTO"]) )
	{
		send_warning(info["HOSTADDRESS"],info["PORTUDP"],
		    "Affirmation warning : No such user or he is offlin now.");
		return;
	}
	if( info["MSG"][<1..<0] != '\n' ) info["MSG"] += "\n";

	tell(ob, sprintf(GRN "[ %s ] %s[%s] �i�D�A �R%s\n"NOR,undefinedp(info["TYPE"])?" ":info["TYPE"],capitalize(info["WIZFROM"]),name,info["MSG"] ), TELLMSG);

}

void send_affirmation(string targ,int port, string to,string msg, string type)
{
	mapping info;
	info = ([
	    "NAME" : INTERMUD_MUD_ENGLISH_NAME,
	    "PORTUDP" : "" + fetch_variable("udp_port"),
	    "WIZFROM" : type+"@"+INTERMUD_MUD_ENGLISH_NAME,
	    "WIZTO"	: to,
	    "TYPE" : type,
	    "MSG" : msg,
	]);
	send_udp(targ,port,"affirmation_a",info);
	//NCH_CHANNEL("send affirmation msg to "+to+"@"+targ+":"+port);
}

//--------------------------------------------------------------------------------------------
// Gtell �t�C
//--------------------------------------------------------------------------------------------



void receive_gtell(mapping info)
{
	string name=get_mud_name(info);
	object ob;

	if(compare_mud_info(info)<1)	
		return;
	if( undefinedp(info["NAME"])
	    ||  undefinedp(info["PORTUDP"])
	    ||  undefinedp(info["WIZTO"])
	    ||  undefinedp(info["WIZFROM"])
	    ||  undefinedp(info["MSG"])
	)   return;
	if( !test_buffer(name,GTELL_B))
	{
		if(time()-gtell_buffer[name] < 1)
		{
			send_affirmation(info["HOSTADDRESS"],info["PORTUDP"],info["WIZFROM"],
			    "Gtell warning : Your mud send too many GTELL request in the same time , Please try again later.","gtell");
			//NCH_CHANNEL(sprintf("%s[%s:%s] gtell ���W�c , ignore",info["NAME"],info["HOSTADDRESS"],info["PORTUDP"]));
			return ;
		}
	}
	gtell_buffer[name]=time();

	if( !ob=find_player(info["WIZTO"]) )
	{
		send_affirmation(info["HOSTADDRESS"],info["PORTUDP"],info["WIZFROM"],
		    "GTELL warning : No such user or he is offlin now.","gtell");
		return;
	}
	if( info["MSG"][<1..<0] != '\n' ) info["MSG"] += "\n";
	if( undefinedp(info["CNAME"]) )
		tell(ob, sprintf(GRN "%s@%s(%s) �i�D�A�R%s"NOR,capitalize(info["WIZFROM"]),info["NAME"],name,info["MSG"] ), TELLMSG);
	else 	tell(ob, sprintf(GRN "%s(%s@%s[%s]) �i�D�A�R%s"NOR,info["CNAME"],capitalize(info["WIZFROM"]),info["NAME"],name,info["MSG"] ), TELLMSG);

	send_affirmation(info["HOSTADDRESS"],info["PORTUDP"],info["WIZFROM"],
	    "GTELL Respond : "+info["WIZTO"]+" has received your message.","gtell");

}

void send_gtell(string targ,int port,string my_id,string my_cname,string targ_id,string msg)
{
	mapping info;
	info = ([
	    "NAME" : INTERMUD_MUD_ENGLISH_NAME,
	    "PORTUDP" : "" + fetch_variable("udp_port"),
	    "WIZFROM" : my_id,
	    "WIZTO"	: targ_id,
	    "CNAME" : my_cname,
	    "MSG" : msg,
	]);
	add_affirmation_buffer(mudlist[targ+":"+port],"gtell");
	send_udp(targ,port,"gtell",info);

	NCH_CHANNEL(sprintf("%s send gtell msg to %s@%s:%s",my_id,targ_id,targ,port+""));
}

//--------------------------------------------------------------------------------------------
// Support �t�C
//--------------------------------------------------------------------------------------------

//void send_support_answer(string targ,int port,)

void receive_support_request(mapping info)
{
	string answer;

	if(compare_mud_info(info)<1)	
		return;		

	if( undefinedp(info["NAME"])
	    ||  undefinedp(info["PORTUDP"])
	    ||  undefinedp(info["CMD"])
	    ||  undefinedp(info["ANSWERID"])
	)   return;

	if( info["CMD"]="tcp" )
		answer="no";
	else
	{
		string cmd=info["CMD"];
		if( undefinedp(service_handler[cmd]) )
			answer="no";
		else answer = "yes";
	}

	if( answer="yes" )
	{
		send_udp(info["HOSTADDRESS"],info["PORTUDP"],"support_a",([
			"NAME" : INTERMUD_MUD_ENGLISH_NAME,
			"PORTUDP" : "" + fetch_variable("udp_port"),
			"CMD" : info["CMD"]+(!undefinedp(info["PARAM"]) ? "||PARAM:"+info["PARAM"] : ""),
			"ANSWERID" : info["ANSWERID"],
			"SUPPORTED" : "yes",
		    ]));	
	} else
	{
		send_udp(info["HOSTADDRESS"],info["PORTUDP"],"support_a",([
			"NAME" : INTERMUD_MUD_ENGLISH_NAME,
			"PORTUDP" : "" + fetch_variable("udp_port"),
			"CMD" : info["CMD"]+(!undefinedp(info["PARAM"]) ? "||PARAM:"+info["PARAM"] : ""),
			"ANSWERID" : info["ANSWERID"],
			"NOTSUPPORTED" : "yes",
		    ]));
	}
}


//--------------------------------------------------------------------------------------------
// Locate �t�C
//--------------------------------------------------------------------------------------------

void receive_locate_request(mapping info)
{
	string answer;

	if(compare_mud_info(info)<1)	
		return;		

	if( undefinedp(info["NAME"])
	    ||  undefinedp(info["PORTUDP"])
	    ||  undefinedp(info["ASKWIZ"])
	    ||  undefinedp(info["TARGET"])
	)   return;

	if( find_player(lower_case(info["TARGET"])) )
		answer="YES";
	else answer="NO";

	send_udp(info["HOSTADDRESS"],info["PORTUDP"],"locate_a",([
		"NAME" : INTERMUD_MUD_ENGLISH_NAME,
		"PORTUDP" : "" + fetch_variable("udp_port"),
		"TARGET" : info["TARGET"],
		"ASKWIZ" : info["ASKWIZ"],
		"LOCATE" : answer,
	    ]));
}

void send_locate_request(string my_name,string t_name)
{
	string t;
	mapping mudinfo;

	foreach(t, mudinfo in (mudlist+incoming_mudlist))
	send_udp(mudinfo["HOSTADDRESS"],mudinfo["PORTUDP"],"locate_q",([
		"NAME" : INTERMUD_MUD_ENGLISH_NAME,
		"PORTUDP" : "" + fetch_variable("udp_port"),
		"TARGET" : t_name,
		"ASKWIZ" : my_name,
	    ]));
}

void receive_locate_answer(mapping info)
{
	object ob;
	if(compare_mud_info(info)<1)	
		return;		

	if( undefinedp(info["NAME"])
	    ||  undefinedp(info["PORTUDP"])
	    ||  undefinedp(info["LOCATE"])
	    ||  undefinedp(info["ASKWIZ"])
	    ||  undefinedp(info["TARGET"])
	)   return;

	switch(info["TARGET"])
	{
	case "NO":return;
	case "YES":
		if( ob = find_player(lower_case(info["ASKWIZ"])))
			tell(ob,sprintf("%s was located on %s [%s:%s] now.\n",info["TARGET"],info["NAME"],info["HOSTADDRESS"],info["PORTUDP"]), ETCMSG);	
		else return;
	}
	NCH_CHANNEL("���� locate_answer");
}



//--------------------------------------------------------------------------------------------
// RWho �t�C
//--------------------------------------------------------------------------------------------



void receive_rwho_request(mapping info)
{
	string str;
	if( undefinedp(info["NAME"])
	    ||  undefinedp(info["PORTUDP"])
	    ||  undefinedp(info["ASKWIZ"])
	)   return;

	if(compare_mud_info(info)<1)	
		return;		

        str = "/cmds/std/ppl/who.c"->who(0, info["VERBOSE"] ? "-l": "-i");

	send_udp(info["HOSTADDRESS"],info["PORTUDP"],"rwho_a",([
		"NAME" : INTERMUD_MUD_ENGLISH_NAME,
		"PORTUDP" : "" + fetch_variable("udp_port"),
		"ASKWIZ" : info["ASKWIZ"],
		"RWHO"	: str,
	    ]));
}

void receive_rwho_answer(mapping info)
{
	object ob;
	string mudname=get_mud_name(info);

	if( undefinedp(info["NAME"])
	    ||  undefinedp(info["PORTUDP"])
	    ||  undefinedp(info["ASKWIZ"])
	    ||  undefinedp(info["RWHO"])
	)   return;

	if( !test_buffer(mudname,RWHO_B) )
	{
		compare_mud_info(info);
		return;
	}else sub_buffer(mudname,RWHO_B);

	if(!ob=find_player(info["ASKWIZ"]))
		return;
	tell(ob, info["RWHO"]+"\n", CMDMSG);
}

varargs void send_rwho_request(string ip,int port,string player,string arg)
{
	if(arg)
		send_udp(ip,port,"rwho_q",([
			"NAME" : INTERMUD_MUD_ENGLISH_NAME,
			"PORTUDP" : "" + fetch_variable("udp_port"),
			"ASKWIZ" : player,
			"VERBOSE": arg,
		    ]));
	else
		send_udp(ip,port,"rwho_q",([
			"NAME" : INTERMUD_MUD_ENGLISH_NAME,
			"PORTUDP" : "" + fetch_variable("udp_port"),
			"ASKWIZ" : player,
		    ]));
	add_buffer(ip+":"+port,RWHO_B);
}

string query_name()
{
	return "INTERMUD2 �t��(INTERMUD2_D)";
}

