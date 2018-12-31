/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_wizhall_5.c
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
#include <time.h>
#include <daemon.h>

inherit STANDARD_ROOM;

string help = @HELP
�b�o�ӥ��Z�L�_���ж����èS������S����T�C
HELP;

#define HOR 	24
#define VER	14

void online_user_record();

void create()
{
	set("short", HIW"�Ův"NOR WHT"���� "NOR YEL"��_��"NOR);
	set("long",@TEXT
    �o�̬O�Ův������_�䪺�ж��A�o�̪ŵL�@���A�P��L���ж��۸��_�ӡA���G
���Գ\�h�C
TEXT);

	set("exits", ([
		"east" : "/wiz/wizhall/room_wizhall_4",
	    ]));
	set("objects", ([
		"/obj/etc/mj/mj" : 1,
	    ]));
	online_user_record();
	set_heart_beat(600);
	::reset_objects();
}

//�����ɮɥN code �����ޥ�
void online_user_record()
{
	int *realtime = TIME_D->query_realtime_array();
	int i, j, k, hour = realtime[HOUR];
	string str,last_str="",b="�i",*sections=({});
	string s="                                                                         ";
	string *list=({s,s,s,s,s,s,s,s,s,s,s,s,s,s});
	mapping record;	
	float w, max_num=0., min_num=10000.;
	string list_name = HIW"          �� "NOR WHT"�� �� �@ �� �� �� �W �u �H �� �� �p ��";
	string time = "       0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23\n";
	int *num= allocate(HOR);

	record = query("login/maxusers/hour/", SYSDATABASE_D->query_ob());

	if( !mapp(record) ) return;

	foreach( string hourstr, int sizeofusers in record  )
	{
		num[to_int(hourstr)] = sizeofusers;
		if( sizeofusers > max_num ) max_num = sizeofusers;
		if( sizeofusers < min_num ) min_num = sizeofusers;
	}

	w = ((max_num-min_num)/(VER-2));
	
	if( w < 0.001 )
		w = 0.001;
		
	str = NOR WHT"   ��"+list_name+"("+(realtime[MON]+1)+" �� "+(realtime[MDAY]+1)+" ��)"+NOR+"\n";
	for(i=0;i<VER;i++)
	{
		for(j=0;j<HOR;j++)
		{
			if((num[j]-min_num)/w > VER-3-i)
			{
				if(list[i][j*3+1..j*3+2] != b)
					list[i][j*3..j*3+2] = sprintf("%3d",num[j]);
				if( i < VER-1 )
					list[i+1][j*3+1..j*3+2]=b;
			}
			//if(j==hour)
			//	list[i][j*3+3..j*3+3] = "|";
		}
		k = w*(VER-2-i)+min_num;
		str +=HIC+(i%2==0 ? sprintf("%3d", k) : "   ")+NOR WHT"��"NOR+list[i]+"\n";
	}

	str += HIC"  0"NOR WHT"�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w��\n"NOR;
	str += HIC+time[0..(hour*3+4)]+HIM+"["+HIR+sprintf("%2d",hour)+HIM+"]"+HIC+time[(hour*3+9)..<0]+NOR;

	// �� | �Ÿ��ᤧ��ƥ[�W WHY �C��
	sections = explode( str, "\n" );
	for(i=0;i<sizeof(sections);i++)
	{
		if(i>1 && i<16)
		{
			j = strsrch(sections[i],"|");
			sections[i]=sections[i][0..j]+NOR WHT+sections[i][j+1..<0];
		}
		last_str += sections[i]+"\n";
	}	

	set("long", last_str);
}

void heart_beat()
{
	online_user_record();
}
