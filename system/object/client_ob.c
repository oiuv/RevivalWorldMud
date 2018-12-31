/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : client_ob.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-02-06
 * Note   : Client ����D��
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit CLEAN_UP;
inherit DATABASE;

int is_client_ob()
{
	return 1;
}

private nomask void dest()
{
	object player = query_temp("player");
	
	if( objectp(player) )
	{
		tell(player, pnoun(2, player)+"���Ȥ�ݳn��w�g���u�C\n");
		CHANNEL_D->channel_broadcast("sys", player->query_idname()+"�n�X�Ȥ�ݳn��");
	}
	destruct(this_object());
}

private nomask void net_dead()
{
	call_out( (: dest :), 0 );
}

nomask void catch_tell(string msg, string classes)		
{
	if( stringp(msg) ) msg = remove_ansi(msg);
	
	receive("%^"+classes+"|"+msg+"%^");

	// �Ҧ����T�������W�e�X, �����ݫʥ]�զX, �H�Q client ��ƶǿ�
	flush_messages(this_object());
}

nomask void directly_receive(string msg)
{
	receive(msg);
}

nomask void process_input(string input)
{
	string arg;
	
	object player = query_temp("player");
	
	sscanf(input, "%s %s", input, arg);

	switch(input)
	{
		case "BASICINFO":
			catch_tell("bgmidi4.mid", PLAYMUSIC);
			catch_tell((CITY_D->query_city_idname(query("city", player))||"�L���y")+" "+player->query_idname(), MYIDNAME);

			if( wizardp(player) )
				catch_tell("DEBUG", DEBUG);

			player->tell_client_str();
			player->tell_client_hp();
			player->tell_client_ep();
			
			player->tell_client_attr();
			
			break;
		case "CLIENTVERSION":
			set("version", arg);
			CHANNEL_D->channel_broadcast("sys", player->query_idname()+"�n�J�Ȥ�ݳn�骩���� "+arg);
			break;
		case "CHANNELSYS":
			CHANNEL_D->channel_broadcast("sys", player->query_idname()+"�Ȥ�ݳn��^���T���G"+arg);
			break;
		default:break;
	}
}

private void create()
{

}
