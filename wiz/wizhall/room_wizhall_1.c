/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_wizhall_1.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-23
 * Note   : �зǩж�
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <message.h>
#include <daemon.h>

inherit STANDARD_ROOM;

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;

// �w�]�X�f
#define DEFAULT_CITY_EXIT(city, num)	({ 49, 49, 0, city, num, 0, "49/49/0/"+city+"/"+num+"/0" })

mapping actions;

array south_exit(object ob)
{
	string city = query("city", ob);
	int num = query("section_num", ob);

	if( !sizeof(CITY_D->query_cities()) )
		return 0;

	if( !CITY_D->city_exist(city) )
	{
		string *cities = CITY_D->query_cities() - ({ "wizard" });
		
		if( sizeof(cities) )
		{	
			do
			{
				city = cities[random(sizeof(cities))];
			}
			while( CITY_D->query_city_info(city, "no_occupy") );
		}
		else
			return 0;
	}
	
	if( CITY_D->city_exist(city, num) )
		return CITY_D->query_section_info(city, num, "wizhall") || DEFAULT_CITY_EXIT(city, num);
	else
		return CITY_D->query_section_info(city, 0, "wizhall") || DEFAULT_CITY_EXIT(city, 0);
}

void do_gocity(object me, string arg)
{
	int num;
	string my_city = query("city", me);
	string *lines;

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�h���y�����H�� help gocity �Ѧҫ��O�Ϊk�èϥ� mayor ���O�d�ߩҦ�������ơC\n");
	
	arg = remove_ansi(lower_case(arg));
	
	lines = explode(arg, " ");
	if(undefinedp(num = to_int(lines[<1]))) {
		num = 1;
		arg = implode(lines, " ");
	}
	else {
		arg = implode(lines[0..<2], " ");
	}
	

	if( !num ) num = 1;

	if( !CITY_D->city_exist(arg, num-1) )
		return tell(me, "�o�ӥ@�ɨèS���o�y�����C\n");

	if( !wizardp(me) )
	{
		if( CITY_D->query_city_info(arg, "no_occupy") )
			return tell(me, "�o�y�����L�k�q�o�̶i�J�C\n");
			
		if( my_city && my_city != arg )
			return tell(me, pnoun(2, me)+"�O"+CITY_D->query_city_idname(my_city)+"�������A�L�k�A�ۥѶi�J��L�����C\n");
		
		if( arg == "wizard" )
			return tell(me, CITY_D->query_city_idname(arg, num)+"�O�Ův�����A�u���Ův����i�J�C\n");
	}
	
	msg("$ME��L�Ův�������߶׻E��q��Ҷ}�Ҫ��q�D�A�e��"+CITY_D->query_city_idname(arg, num-1)+"�C\n", me, 0, 1);
	me->move(CITY_D->query_section_info(arg, num-1, "wizhall") || DEFAULT_CITY_EXIT(arg, (num-1)));
	msg("$ME��L�Ův�����q�D�Ө�F"+CITY_D->query_city_idname(arg, num-1)+"�C\n", me, 0, 1);
}

void create()
{
	set("short", HIW"�Ův"NOR WHT"����"NOR);
	set("long",@TEXT
    ���j���Ův�����ⰼ�U�ݥߵۥ|�䥨���۬W�A�伵�ۼƤQ�ذ��γ��A��������
�k��O�e�F�ƦʤءA�b�o�ɥj�Ѫ��۳y�ؿv���A�i�H�Pı��@��[1;37m���t����q[m���w�w
���i�J����A�P�ɫo�]�i�H�P����ؿv�`�B�w�wŧ�Ӫ�[0;1;30m���N[m���P�A���G�b�o�y����
�̡A���۳\�h�����H�������K�A�Ѧ���[1;33m South [m��V�i�H���V�ۤv���ݪ������A�|
���J�y���󫰥��̩��n���h�|�H�N������@�ӫ����A�άO�Q��[1;33m gocity [m���O�e���z
�ҫ��w�������C�ϥ� [1;33mmayor [m���O�i�H�d�ߦU�ӫ������ԲӸ�ơA�@���J�y�ɪ���
�ҡC
TEXT);

	set("exits", ([
		"east" : "/wiz/wizhall/room_wizhall_2",
		"west" : "/wiz/wizhall/room_wizhall_6",
		"north" : "/wiz/wizhall/room_wizhall_4",
		"south" : (: south_exit($1) :),
		
	]));
	

	set("objects", ([
		"/obj/board/player" : 1,
	]));
	
	set_heart_beat(100);

	actions = ([ "gocity" : (: do_gocity :) ]);
	
	set("help/gocity", 
@HELP
�L���y�̥i�H�ϥΦ����O�ۥѫe�����@�y�����A���O�Ϊk�p�U
    
    gocity '����ID'	- �e���Y����
    
    �Ҧp
    gocity center	- �e�� center ����
    
���� ID �i�� mayor ���O�d��
HELP,
	);

	::reset_objects();
}

void heart_beat()
{
	int heal;
	string roomshort = query("short");
	
	foreach( object ob in all_inventory(this_object()) )
	{
		if( !ob->is_living() ) return;

		heal = 0;
		
		if( !ob->is_stamina_full() )
		{
			heal = 1;
			ob->earn_stamina(20+random(21));
		}

		if( !ob->is_health_full() )
		{
			heal = 1;
			ob->earn_health(20+random(21));
		}

		if( !ob->is_energy_full() )
		{
			heal = 1;
			ob->earn_energy(20+random(21));
		}
		
		if( heal )
			msg(roomshort+"�̯��t����q���w�w�a�`�J��$ME������C\n"NOR, ob, 0, 1);
	}
}

void init(object ob)
{
	if( !ob->is_living() ) return;

	if( query("total_online_time", ob) < 24*60*60 )
	{
		string msg = HIY"\n�s�ⴣ�ܢw�w�w�w�w�w\n"
			NOR YEL"�w��"+pnoun(2, ob)+"�i�J���ͪ��@�ɽu�W Mud �C���C\n"
			"�s��Х��ԥ[�\Ū http://www.revivalworld.org/rw/documents/newbie ���x���оǤ��\n"
			"�ݧ��оǤ���Y�������L���D�w��Q�Ρuchat xxx�v��ѫ��O�V�u�W��L���a�߰�\n"
			"�Y�ɦa�ϽаѦ� http://www.revivalworld.org/rw/online\n"
			HIY"�w�w�w�w�w�w�w�w�w�w\n\n"NOR;
			
		tell(ob, msg);
	}
}

