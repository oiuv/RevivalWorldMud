/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : channel_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-20
 * Note   : �W�D���F
 * Update :
 *  o 2001-07-08 Cookys �ץ� other channel �����~�@�֥[�W���H�N�e�X
 *  o 2001-07-25 Cookys other �e�X�L�k�e�X���W�D�ɷ|�H[����]��ܤ��C
 *
 -----------------------------------------
 */

#define CHAT 		(1<<0)	//����
#define RUMOR		(1<<1)	//����
#define MUSIC		(1<<2)	//����
#define WIZ		(1<<3)	//�Ův
#define SYS		(1<<4)	//�t��
#define NCH		(1<<5)	//�ʴ�
#define NEWS		(1<<6)	//�s�D
#define TWIZ		(1<<7)	//�O�W�Ův
#define GWIZ		(1<<8)	//��ڧŮv
#define OTHER		(1<<9)	//��L
#define AD		(1<<10)	//�s�i
#define CITY		(1<<11)	//����
#define GOD		(1<<12)	//�ѯ�
#define REPORT		(1<<13)	//���i
#define ENT		(1<<14)	//���~
#define STOCK		(1<<15) //�Ѳ�
#define AUC		(1<<16) //���

#include <ansi.h>
#include <secure.h>
#include <daemon.h>
#include <message.h>
#include <gender.h>

#define DEFAULT_REFUSE  ( NCH | MUSIC | OTHER | AD )

class channel_class
{
	int level, number;
	string msg;
	mapping extra;
}

mapping channels;
mapping users;
nosave int water_number;

string help = @HELP
�����W�D���O�A�d���W�D���ϥΤ�k�� help channels�C
HELP;

private array info = 
({
    ({
	REPORT,	
	"report",
	GUEST, 
	RED "�i"NOR HIR"�^��"NOR RED"�j"NOR, 	
	([
	    "time"		:	1, 
	    "msgcolor"	:	NOR HIR,
	]),
    }),
    ({ 
	GOD,
	"god",	
	ADMIN,  
	BLU "�i"HIB"����"NOR BLU"�j"NOR, 
	([

	]),
    }),
    ({ 
	CHAT, 	
	"chat",
	PLAYER,
	CYN "�i"HIC"����"NOR CYN"�j"NOR,
	([

	]),
    }),
    ({
	RUMOR,
	"rumor",
	PLAYER,
	MAG "�i"HIM"����"NOR MAG"�j"NOR,
	([
	    "wizlevel"	:	1,
	]) 
    }),
    ({ 
	MUSIC,
	"music",
	PLAYER,
	GRN "�i"HIG"����"NOR GRN"�j"NOR,
	([

	])
    }),
    ({ 
	WIZ, 
	"wiz",
	GUEST,
	YEL "�i"HIY"�Ův"NOR YEL"�j"NOR,
	([

	]), 
    }),
    ({ 
	SYS,
	"sys",
	GUEST,
	WHT "�i"HIW"�t��"NOR WHT"�j"NOR,
	([
	    "time"		:	1,
	    "msgcolor"	:	NOR WHT,
	]),
    }),
    ({ 
	NCH,
	"nch",
	GUEST,
	WHT "�i"NOR YEL"�ʴ�"NOR WHT"�j"NOR,
	([
	    "time"		:	1,
	    "msgcolor"	:	NOR YEL,
	]),
    }),
    ({ 
	NEWS,
	"news",
	PLAYER,
	GRN "�i"HIG"�s"NOR GRN"�D"HIG"�j"NOR,
	([
	    "time"		:	1,
	    "notalk"	:	1,
	]),
    }),
    ({
	TWIZ,
	"twiz",
	WIZARD,
	WHT "�i"NOR YEL"�O�W" NOR WHT"�j"NOR,
	([
	    "intermud"	:	INTERMUD2_D,
	    "channel"	:	"twiz",
	]),
    }),
    ({ 
	GWIZ,
	"gwiz",
	WIZARD,
	WHT "�i"HIW"����%T"NOR WHT"�j"NOR,
	([
	    "intermud"	:	INTERMUD2_D,
	    "channel"	:	"CREATOR",
	    "subchannel"	:	1,
	    "newtitle"	:	(: stringp($1) ? "-"+$1 : "" :),
	]),
    }),
    ({ 
	OTHER,
	"other",
	PLAYER,
	CYN "�i"NOR WHT"����%T"NOR CYN"�j"NOR,
	([
	    "intermud"	:	INTERMUD2_D,
	    "channel"	:	"",
	    "subchannel"	:	1,
	    "newtitle"	:	(: stringp($1) ? ("-"+HIG+$1+NOR) : "" :),
	]),
    }),
    ({
	AD,
	"ad",
	PLAYER,
	WHT "�i"NOR GRN"�s�i" NOR WHT"�j"NOR,
	([

	]),
    }),
    ({ 
	CITY,
	"city",
	PLAYER,
	HIW"�i"NOR"%T"NOR HIW"�j"NOR,
	([
	    "subchannel"	:	1,
	    "filter"	:	(: query("city",$1) == (stringp($2) ? $2 : query("city", $2)) || wizardp($1) :),
	    "newtitle"	:	(: CITY_D->query_city_name(stringp($1) ? $1 : query("city", $1))||"�y���C��":),
	]),
    }),
    ({ 
	ENT,
	"ent",
	PLAYER,
	HIW"�i"NOR"%T"NOR HIW"�j"NOR,
	([
	    "subchannel"	:	1,
	    "filter"	:	(: query("enterprise",$1) == (stringp($2) ? $2 : query("enterprise", $2)) || wizardp($1) :),
	    "newtitle"	:	(: ENTERPRISE_D->query_enterprise_color_id(stringp($1) ? $1 : query("enterprise", $1))||"�����":),
	]),
    }),
    ({
	STOCK,
	"stock",
	PLAYER,
	YEL "�i"NOR HIY"��"NOR YEL"��" NOR HIY"�j"NOR,
	([
	    "time"		:	1,
	]),
    }),
    ({
	AUC,
	"auc",
	PLAYER,
	BLU "�i"NOR HIB"���" NOR BLU"�j"NOR,
	([
	    "time"	:	1,
	    "notalk"	:	1,
	]),
    }),
});

nosave mapping replace_word = ([
    "�F�A�Q":	"******",
]);

int valid_channel(int level, string channel)
{
	foreach(array data in info)
	{
		if( data[1] == channel && level >= data[2] )
			return 1;
	}

	return 0;
}

varargs private void do_channel(object me, string channel, string msg, mixed raw)
{
	int emote,internal_flag;
	object *fit_users;
	class channel_class ch;
	string channel_title, idname, send_msg, time, my_id, my_idname, color, subchannel;

	if( !channel || channel == "" || !me ) return;

        if( query("ignore", me) )
		return tell( me, sprintf("�z�{�b�L�k�ϥΤ����W�D, �гq���Ův�C\n"));

	// �ˬd�O�_�������O
	if( channel[<1] == '*' ) { emote = 1; channel = channel[0..<2]; }

	if( sscanf(channel, "gwiz-%s", subchannel) ) channel = "gwiz";

	if( !classp(ch = channels[channel]) ) return;

	if( channel == "city" && interactive(me) && !wizardp(me) && !query("city", me) )
		return tell(me, pnoun(2, me)+"�å��[�J���󫰥��C\n");
	else if( channel == "ent" && interactive(me) && !wizardp(me) && !query("enterprise", me) )
		return tell(me, pnoun(2, me)+"�å��[�J������~�C\n");

	my_id = me->query_id(1);
	my_idname = me->query_idname();

	// �Y���l�W�D�аO, �h���R�l�W�D�W��
	if( ch->extra["subchannel"] ) 
		if( msg && sscanf(msg, "[%s] %s", subchannel, msg) ) internal_flag=1;
		// �ˬd���W�D���S������W�D�W
	if( ch->extra["newtitle"] )
	{

		channel_title = replace_string(ch->msg, "%T", evaluate(ch->extra["newtitle"], stringp(raw) ? raw : (wizardp(me)||previous_object(1)==find_object(INTERMUD2_D)) ? subchannel || me : me));
	}
	else
		channel_title = ch->msg;

	// ���p�O�ϥΪ�, �h�ˬd�W�D���U
	if( interactive(me) && !(ch->number & users[me]) )
	{
		if( SECURE_D->level_num(my_id) < ch->level ) return;

		users[me] |= ch->number;
		set("channels", query("channels",me) | ch->number, me);
		tell(me, "�}�� "+channel_title+" �W�D�C\n");

		if( !msg ) return;
	}

	// �Y�S����J�T��
	msg = msg || (emote ? "�ݰ_�Ӫ��״I�C" : "..." );

	// �ˬdID, NAME 
	idname = my_idname || "NO_IDNAME";

	// ��z�X���b�ϥΥ��W�D���W�D�ϥΪ� 
	fit_users = keys(filter(users, (: $(ch->number) & $2 :)));

	// �ˬd���W�D���ϥΤ��L�o����
	if( ch->extra["filter"] )
	{
		if( sizeof(fit_users) )
			fit_users = filter(fit_users, ch->extra["filter"], stringp(raw) ? raw : (wizardp(me) ? subchannel || me : me));
	}


	// �Y���W�D����ܮɶ��аO
	if( ch->extra["time"] == 1 )
		time = HIW+ctime(time())[11..15]+NOR" ";
	else if( ch->extra["time"] == 2 )
		time = HIW+ctime(TIME_D->query_game_time())[11..15]+NOR" ";
	else	
		time = "";

	// �Y�ϥΪ��Ҧ�
	if( emote ) 
	{
		string verb, arg;
		verb = msg;
		sscanf(msg, "%s %s", verb, arg);

		// Added by Ekac
		if( EMOTE_D->is_emote(verb) )
			msg = evaluate(COMMAND_D->find_ppl_command(verb),me,arg,1) || (idname+msg);
		else
			msg = idname + msg;
	}

	// �Y���W�D���]�w�S�w�T���C��
	if( ch->extra["msgcolor"] )
	{
		color = ch->extra["msgcolor"];
		msg = remove_ansi(msg);
		idname = remove_ansi(idname);
	}
	else
		color = "";

	if( query("no_channel", me) )
		return tell(me, pnoun(2, me)+"�ثe���Q���\�ϥΤ��}�W�D�o���C\n");

	// ����~�ù����欰
	if( undefinedp(raw) && previous_object() != this_object() && (interactive(me) || me->is_npc()) )
	{
		int nowtime = time();

		if( nowtime - query_temp("lastchat/time", me) < 1 )
		{
			if( addn_temp("lastchat/warning", 1, me) > 100 )
			{
				tell(me, "�ѩ�"+pnoun(2, me)+"�j�q���й��դ��X�k���W�D�o���ʧ@�A�q�{�b�_�N�۰ʫ���"+pnoun(2, me)+"�����O�\��C\n");
				set("cmdlimit", 30, me);
				CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�j�q���й��դ��X�k���W�D�o���ʧ@�A�q�{�b�_�N�۰ʭ�����O�t�סC\n");
			}
			return tell(me, "���}�W�D����s��o�e�T���C\n");
		}
		if( query_temp("lastchat/msg", me) == msg && nowtime - query_temp("lastchat/time", me) < 300 )
		{
			if( addn_temp("lastchat/repeattimes", 1, me) > 5 )
			{
				tell(me, "���}�W�D����s��o�e�ۦP�T���C\n");

				if( addn_temp("lastchat/warning", 1, me) > 100 )
				{
					tell(me, "�ѩ�"+pnoun(2, me)+"�j�q���й��դ��X�k���W�D�o���ʧ@�A�q�{�b�_�N�۰ʫ���"+pnoun(2, me)+"�����O�\��C\n");
					set("cmdlimit", 30, me);
					CHANNEL_D->channel_broadcast("sys", me->query_idname()+"�j�q���й��դ��X�k���W�D�o���ʧ@�A�q�{�b�_�N�۰ʭ�����O�t�סC\n");
				}
				return;
			}
		}
		else
		{
			set_temp("lastchat/msg", msg, me);
			delete_temp("lastchat/repeattimes", me);
		}

		set_temp("lastchat/time", nowtime, me);
	}
	// ���a�L�k�� News �W�D�o��
	if( ch->extra["notalk"] )
	{	
		if( !wizardp(me) && interactive(me) )
		{
			return tell(me, "���a�L�k�ϥγo���W�D�o���C\n");
		}
	}
	// �Y�O�����W�D
	if( ch->extra["intermud"] && interactive(me) )
	{
		string intermud_msg = msg;
		if( emote ) intermud_msg = replace_string(msg, my_idname, my_idname[0..<2] + "@RW)");

		if( channel == "gwiz" )
			ch->extra["intermud"]->send_gwiz_msg(subchannel, ansi_capitalize(me->query_id()), me->query_name(), intermud_msg, emote);
		else
		if((!internal_flag && subchannel!="") || ( internal_flag && member_array(subchannel,keys(channels))==-1 )) ch->extra["intermud"]->send_gchannel_msg(subchannel, ansi_capitalize(me->query_id()), me->query_name(), intermud_msg, emote);
		else 	subchannel="����";
	}

	// �Y���°T���ѼƩΪ��Ѽ�, send_msg = �W�D�W�� + �T��
	if( !undefinedp(raw) || emote )
		send_msg = channel_title+time+color;
	else
		send_msg = channel_title+time+color+idname+"�G"+color;


	// �B�z��²�r��
	send_msg = send_msg + msg + NOR + (noansi_strlen(msg) > 0 && remove_ansi(msg)[<1] == '\n' ? "" : "\n");


	// replace string
	foreach(string a , string b in replace_word)
	send_msg=replace_string(send_msg,a,b);

	// �ϥΪ��v���B�z, ���Ű����󻡸ܪ̪�����ܦW��
	if( ch->extra["wizlevel"] )
	{
		int my_level = SECURE_D->level_num(my_id);
		string level_send_msg;
		object *level_fit_users = filter(fit_users, (: wizardp($1) && $(my_level) <= SECURE_D->level_num($1->query_id(1)) :));

		if( random(50) || wizardp(me) )
			switch(query("env/rumor", me))
		{
		case 1:
			{
				string random_idname;
				object* fit_users_without_wiz = filter_array(fit_users, (: !wizardp($1) :));

				if( !sizeof(fit_users_without_wiz) )
					random_idname = fit_users[random(sizeof(fit_users))]->query_idname();
				else
					random_idname = fit_users_without_wiz[random(sizeof(fit_users_without_wiz))]->query_idname();

				level_send_msg = replace_string(send_msg, my_idname, "["+my_idname+"]-���_��" + random_idname );
				send_msg = replace_string(send_msg, my_idname, "���_��" + random_idname );
				break;
			}
		case 2:
			{
				if( undefinedp(query_temp("rumor_serial",me)) )
				{
					water_number += 1 + random(50);
					set_temp("rumor_serial", water_number,me);
				}
				level_send_msg = replace_string(send_msg, my_idname, "["+my_idname+"]-"+(query("gender", me) == MALE_GENDER?"Mr.":"Miss")+  query_temp("rumor_serial",me) );
				send_msg = replace_string(send_msg, my_idname, (query("gender", me) == MALE_GENDER?"Mr.":"Miss")+  query_temp("rumor_serial",me));

				break;
			}
		default:
			level_send_msg = replace_string(send_msg, my_idname, "["+my_idname+"]-�Y�H(Someone.)");
			send_msg = replace_string(send_msg, my_idname, "�Y�H(Someone.)");
			break;
		}
		else
		{
			level_send_msg = replace_string(send_msg, my_idname, "["+my_idname+"]-������"+my_idname);
			send_msg = replace_string(send_msg, my_idname, "������"+my_idname);
		}

		fit_users -= level_fit_users;

		// �v���������ϥΪ�
		tell(level_fit_users, level_send_msg, CHNMSG);

		level_fit_users->add_msg_log(channel, level_send_msg);
	}

	tell(fit_users, send_msg, CHNMSG);

	// �����Ǩ�
	if( channel == "city" && previous_object() != this_object() && query("city", me) )
	{
		string city, mycity = query("city", me);
		string no_ansi_send_msg = remove_ansi(send_msg);
		object *citizens;

		foreach(city in CITY_D->query_cities())
		{
			if( city == mycity ) continue;
			if( strsrch(no_ansi_send_msg, remove_ansi(CITY_D->query_city_name(city)||"")) != -1	&& strsrch(lower_case(no_ansi_send_msg), city) != -1 )
			{
				citizens = filter_array(users(), (: query("city", $1) == $(city) :));

				if( sizeof(citizens) )
				{
					this_object()->channel_broadcast("city", "�T���e��"+CITY_D->query_city_idname(city)+"...", me);
					this_object()->channel_broadcast("city", "�u"+send_msg[0..<2]+"�v", citizens[0]);
					//this_object()->channel_broadcast("city", "�{���ǨӮ����A"+CITY_D->query_city_idname(mycity)+"���������G���b�Q�׵ۦ���"+CITY_D->query_city_idname(city)+"���Ʊ�...", citizens[0]);
				}
			}
		}
	}

	fit_users->add_msg_log(channel, send_msg);

	switch(channel)
	{
	case "god":
		break;
	case "chat":
	case "city":
	case "ent":
	case "rumor":
	case "twiz":
	case "gwiz":
	case "music":
	case "wiz":
		log_file("channel/"+channel, send_msg, -1);
		break;
	}
}

/* Called from COMMAND_D */
nomask mapping query_channels_fpointer()
{
	mapping m = allocate_mapping(sizeof(info)*2);

	if( previous_object() != load_object(COMMAND_D) )
		return 0;

	foreach( mixed *c in info )
	{
		string ch = (string) c[1];
		m[ch] = ({ c[2], (: do_channel($1, $(ch), $2, $3) :) });
		m[ch+"*"] = ({ c[2], (: do_channel($1, $(ch+"*"), $2, $3) :) });
	}

	return m;
}

/* ���U�W�D */
nomask void register_channel(object me, int channel)
{
	if( !channel ) return;
	if( undefinedp(users[me]) ) users[me] = channel;
	else users[me] |= channel;
}

/* �������U */
nomask void remove_register(object me, int channel)
{
	if( !channel ) return;
	users[me] ^= channel;
	if( users[me] == 0 ) map_delete(users, me);
}

/* ���������@��ϥΪ� */
nomask void remove_user(object me)
{
	map_delete(users, me);
}

/* �Ҧ����a���s���U�W�D */
nomask void reset_all_register()
{
	users = allocate_mapping(0);
	filter_array( users(), (: register_channel($1, query("channels",$1)) :) );
}

nomask varargs array query_channel_users(string channel, int flag)
{
	class channel_class ch;
	array res = allocate(0);

	if( undefinedp(ch = channels[channel]) ) return res;
	foreach(object ob, int i in users)
	{
		if( !flag && (i & ch->number) ) res += ({ ob });
		if( flag && !(i & ch->number) ) res += ({ ob });
	}
	return res;
}

nomask int query_default_channel(string id)
{
	int default_channel, level = SECURE_D->level_num(id);
	foreach( string name, class channel_class ch in channels )
	{
		if( level < ch->level ) continue;
		default_channel |= ch->number;
	}
	return default_channel &~ DEFAULT_REFUSE;
}

nomask string query_using_channel(object user)
{
	int channel = users[user];
	string using_channel = "";

	if( channel )
		foreach( string name, class channel_class ch in channels )
		if( channel & ch->number ) using_channel += " "+name;

	return using_channel == "" ? "�L" : using_channel;
}

varargs void channel_broadcast(string channel, string msg, mixed arg)
{
	switch(channel)
	{
	case "city":
		msg = NOR GRN"�a��"+HIG+"�s�D "NOR+msg; break;
	case "ent":
		msg = NOR CYN"���~"+HIC+"�s�D "NOR+msg; break;
	default: break;
	}

	if( undefinedp(arg) )
		do_channel(this_object(), channel, msg, 1);
	else if( objectp(arg) )
		do_channel(arg, channel, msg, 1);
	else
		do_channel(this_object(),channel, msg, arg); 

}

varargs void announce(object me, string msg)
{
	shout("\n\a"HIR + (objectp(me) ? me->query_idname() + HIR"�o�G":"") + "�t�Τ��i ("HIW + ctime(time()) + NOR + HIR")�G\n\n"NOR + msg + NOR"\n\n");
}


private void create()
{
	class channel_class ch;

	if( clonep() ) destruct(this_object());

	channels = allocate_mapping(0);
	users = allocate_mapping(0);
	water_number = 10000;
	foreach( mixed *c in info )
	{
		ch = new(class channel_class);
		ch->number	= c[0];		
		ch->level	= c[2];
		ch->msg		= c[3];
		ch->extra	= c[4];
		channels[c[1]] = ch;
	}
	reset_all_register();

	COMMAND_D->do_reset();
}


string query_name()
{
	return "�W�D�t��(CHANNEL_D)";
}
