/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mjai_clode.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-31
 * Note   : �O�W�Q���i�±N�� AI ����
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include "mj.h"

inherit STANDARD_NPC;

#define MJOB	(query_temp("mjobject"))

varargs void create()
{
	set_idname("AI MJer",HIC"�H�u���z"NOR CYN"�±N�L"NOR);
	set("unit", "��");
	startup_living();
}

// �C���@�i�P��I�s AI_pg() �]�w�۰ʸI�b���
void AI_pg()
{
	string *group;
	mapping Pdata = copy(fetch_variable("Pdata", MJOB));
	mapping data = Pdata[this_object()];

	// �Y�w�gť�P�h�����I�b�ˬd
	if( sizeof(data[LISTEN]) ) return;

	foreach( group in MJOB->tile_logic_check(data[TILE]) )
	{
		if( sizeof(group) == 2 && group[0] == group[1] && member_array(group[0], data[AUTOPON]) == -1 )
			data[AUTOPON] += ({ group[0] });

		if( sizeof(group) == 3 && group[0] == group[1] && group[1] == group[2] && member_array(group[0], data[AUTOGON]) == -1 )
			data[AUTOGON] += ({ group[0] });
	}
}

// �W�a���P��I�s AI_eat �P�_�O�_�Y�P
// �^�ǭn�Y����i�P ex:({ "2w", "3w" })
string *AI_eat()
{
	int i;
	string *group;
	array choice = allocate(2, allocate(0));
	mapping MJdata = copy(fetch_variable("MJdata", MJOB));
	mapping Pdata = copy(fetch_variable("Pdata", MJOB));
	mapping data = Pdata[this_object()];

	if( sizeof(data[LISTEN]) || !MJdata[DUMPING] ) return 0;

	foreach( group in MJOB->tile_logic_check(data[TILE]) )
	switch(sizeof(group))
	{
	case 1: choice[0] += group;
		break;
	case 2: if( group[0] != group[1] )
			choice[1] += ({ group });
		break;
	}

	// check ����
	foreach( group in choice[1] )
	{
		// �Y�������S�b���諸�ɭ�
		if( group[0][0] == group[1][0] - 1
		    && MJdata[DUMPING][1] == group[0][1]
		    && (MJdata[DUMPING][0] == group[0][0] - 1 || MJdata[DUMPING][0] == group[1][0] + 1 ))
			return group;
	}

	// check ���O�_�������}
	for(i=0;i<sizeof(choice[0])-1;i++)
	{
		if( choice[0][i][1] == choice[0][i+1][1]
		    && choice[0][i][0] == choice[0][i+1][0] - 2
		    && MJdata[DUMPING][1] == choice[0][i][1]
		    && MJdata[DUMPING][0] == choice[0][i][0] + 1)
			return ({ choice[0][i], choice[0][i+1] });
	}

	return 0;
}

// ���� AI ���P�ɩI�s AI_da()
string AI_da()
{
	string *group;
	array choice = allocate(2, allocate(0));
	string *result = allocate(0);
	mapping MJdata = copy(fetch_variable("MJdata", MJOB));
	mapping Pdata = copy(fetch_variable("Pdata", MJOB));
	mapping data = Pdata[this_object()];

	foreach( group in MJOB->tile_logic_check(data[TILE]) )
	switch(sizeof(group))
	{
		case 1: choice[0] += group;
			break;
		case 2: if( group[0] != group[1] )
				choice[1] += ({ group });
			break;
	}

	// �̷Ӧr�P -> ��i -> ��L, �N�氦�Ƨ�
	choice[0] = sort_array(choice[0],
	    (: member_array($1[0]-48, ({ ({1,9,2,8,3,7,4,6,5}), ({9,1,8,2,7,3,6,4,5}) })[random(2)])
		> member_array($2[0]-48, ({ ({1,9,2,8,3,7,4,6,5}), ({9,1,8,2,7,3,6,4,5}) })[random(2)]) ? 1 : -1 :));

	// ���Hť�P�ɶ}�l�p�ߥ��P
	if( MJdata[LISTENING] || sizeof(MJdata[DUMPED]) > 35 )
	{
		if( sizeof(choice[0]) )
		{
			// �u��Q���L���P
			result = choice[0] & (string *)MJdata[DUMPED];
			
			// �����L���P�h�ƶ���
			if( sizeof(result) )
			{
				// �Ƨǭ�Q���L���P, �̪񪺵P�Ƴ̫e�Y
				result = sort_array(result, (: member_array($1, $(MJdata)[DUMPED]) < member_array($2, $(MJdata)[DUMPED]) ? 1 : -1 :));
			}
			// �Y���O�S�Q���L���P�h���r�P
			else
			{
				result = choice[0] & ({ "ea", "so", "we", "no", "jo", "fa", "ba" });
			}
		}

		if( !sizeof(result) && sizeof(choice[1]) )
		{
			// ��Q���L����
			foreach( group in choice[1] )
			{
				if( member_array(group[0], MJdata[DUMPED]) != -1 )
					result += ({ group[0] });
				if( member_array(group[1], MJdata[DUMPED]) != -1 )
					result += ({ group[1] });
			}

			if( sizeof(result) )
			{
				result = sort_array(result, (: member_array($1, $(MJdata)[DUMPED]) < member_array($2, $(MJdata)[DUMPED]) ? 1 : -1 :));
				
			}
		}
	}
	else
	{
		// �Y�����h�����
		if( sizeof(choice[0]) )
		{
			// �P�_�O�_�b�����}, �����ܫh�������i�P
			foreach( string t in choice[0] )
			{
				if( member_array(t[1], ({ 'w', 's', 't' })) == -1 ||
				(member_array(sprintf("%c%c", t[0]+2, t[1]), choice[0]) == -1 && member_array(sprintf("%c%c", t[0]-2, t[1]), choice[0]) == -1) )
					result += ({ t });
			}
		}
		
		// �ɶq���䥴
		if( !sizeof(result) )
			result = choice[0];

		// �Y�L���, �h�����䥴
		if( !sizeof(result) && sizeof(choice[1]) )
		{
			// ��Q���L����
			foreach( group in choice[1] )
			{
				if( member_array(group[0], MJdata[DUMPED]) != -1 )
					result += ({ group[0] });
				if( member_array(group[1], MJdata[DUMPED]) != -1 )
					result += ({ group[1] });
			}
		}
	}

	// �A�S���N�é�P�M���w���P
	if( !sizeof(result) )
		result = sort_array(data[TILE], (: member_array($1, $(MJdata)[DUMPED]) < member_array($2, $(MJdata)[DUMPED]) ? 1 : -1 :));

	return result[0];
}

// �C�������P��I�s AI_tin() �P�_�O�_ť�P�� au hu
array AI_tin()
{
	int i, j, listencount;
	object ppl;
	string tile, seatile;
	mapping listening = allocate_mapping(0);
	mapping MJdata = copy(fetch_variable("MJdata", MJOB));
	mapping Pdata = copy(fetch_variable("Pdata", MJOB));
	mapping data = Pdata[this_object()];
	string *mytiles = data[TILE];
		
	// �۰ʤ��R�����ǵP�i�Hť
	foreach( tile in Tsort )
	{
		if( MJOB->valid_hu_check(MJOB->tile_sort(mytiles+({ tile })), 0, 0) )
		{
			// �ˬd�ۤv���F�X�i
			i = member_array(tile, mytiles);
			if(i!=-1)
			{
				j++;
				while( i++<sizeof(mytiles)-1 && mytiles[i-1] == mytiles[i] )
					j++;
			}

			// �ˬd�������X�i
			foreach( seatile in MJdata[DUMPED] )
			if( seatile == tile ) j++;

			// �ˬd�Ҧ��H���e���X�i
			foreach( ppl in fetch_variable("Psort", MJOB) )
			{
				i = member_array( tile, fetch_variable("Pdata", MJOB)[ppl][OTILE] );
				if(i!=-1)
				{
					j++;
					while( i++<sizeof(Pdata[ppl][OTILE])-1 && Pdata[ppl][OTILE][i-1] == Pdata[ppl][OTILE][i] )
						j++;
				}
			}

			listening[tile] = 4-j;
		}
	}


	foreach( i in values(listening) )
	listencount += i;

	// ����ť
	if( !sizeof(listening) || !listencount )
		return 0;

	// �Y�uť�@�P, �h��ť, ���]�w�۰ʭJ
	if( sizeof(listening) == 1 )
		return keys(listening); // �@ au hu �ʧ@
	else if( listencount < 3 )
		return keys(listening);
	else
		return ({1});// ť�P
}

void emotion_begin()
{
	set_this_player(this_object());
	switch(random(10))
	{
		case 0:this_object()->do_command("say ���ڷQ�@�U�ӭ��i�P����.....");break;
                case 1:this_object()->do_command("pace �ڬO�Q���K�H�g�X�Ӫ�");break;
		case 2:this_object()->do_command("pace 147 258 369");break;
		case 3:this_object()->do_command("pace 13579 2468");break;
	}
}
void emotion_eat()
{
	set_this_player(this_object());
	switch(random(10))
	{
		case 1:this_object()->do_command("mars");	break;
		case 2:this_object()->do_command("wire �Y");	break;
		case 3:this_object()->do_command("fascinate");	break;
	}
}

void emotion_pon()
{
	set_this_player(this_object());
	switch(random(10))
	{
		case 1: this_object()->do_command("pace �I"); break;
		case 2: this_object()->do_command("sing"); break;
		case 3: this_object()->do_command("excellent �I"); break;
		case 4: this_object()->do_command("fascinate �I"); break;
	}
}

void emotion_mo()
{
	mapping MJdata = copy(fetch_variable("MJdata", MJOB));

	set_this_player(this_object());
	switch(random(20))
	{
		case 1:this_object()->do_command("wahaha");break;
                case 2:this_object()->do_command("pace �ڬO�Q���t�H�g�X�ӣx");break;
		case 3..4:
			if( MJdata[LISTENING] )
			{
				this_object()->do_command("imho "+MJOB->full_chinese(Tsort[random(sizeof(Tsort))])+"��j�����v���Ӧ� "+sprintf("%.2f%%", random(1000)/10.)); break;
			}
			break;
		case 5..8:
			if( MJdata[LISTENING] )
			{
				this_object()->do_command("imho ���Ӧ��H�bť "+MJOB->full_chinese(Tsort[random(sizeof(Tsort))])); break;
			}
			break;
		case 9:
			if( MJdata[LISTENING] )
				this_object()->do_command("imho ���Ӧ��H�bť "HIY"MP"NOR YEL"3"NOR); break;
	}
}
