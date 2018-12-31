/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mj.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-12-10
 * Note   : �O�W�Q���i�±N��
 * Update :
 *  o 2003-04-15 Clode Update v1.05 to v1.06
    o 2003-04-17 Clode Update v1.06 to v1.07
    o 2003-04-19 Clode Update v1.07 to v1.08
    o 2003-05-25 Sinji Update v1.08 to v1.09
    o 2003-06-25 Ekac �� onlooker �]�ݪ��쥴�P�T��
    o 2004-12-28 Clode Update v1.10 to v1.11 �ɱj AI
    o 2004-12-31 Clode Update v1.11 to v1.12 AI �{���W��, �i�ۦ�o�i�U�� AI
    o 2005-03-06 Clode Update v1.12 to v1.13 �״_�������j bug
    o 2006-11-14 Kyo   �s�W���O skyeye �Ѳ��q�A�Ův����ť�P�]�i�H�ݩҦ��H�PXD
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <inherit.h>
#include <message.h>
#include "mj.h"

inherit STANDARD_OBJECT;

#define STATUS	HIY"��"NOR YEL"�N"NOR
#define OSTATUS	HIY"��"NOR YEL"�["NOR

nosave string help = @HELP
�� �O�W���v�Q���i�±N���O����

[�@����O] -
join			- �[�J�P��
leave			- ���}�P��
start			- �}�l�s�P��
restart 		- ���s�}�l�P���A���a�Ҥ����ܡA���}�P��
reset			- �M���P���A�Ҧ����a���}�P��
redeal			- ���s�~�P�A�S���p�ɨϥ�
next			- �~��U�@��
bill			- �ݪ��a�w�X
top			- �Ʀ�]�A�ƦW�i�̤U�C�ѼƱƦC [mo][hu][gun][win][lose][bonus]
redraw			- ���s�Ұʵe���榡�A�e�����é��_�u���s�̾A��
fixmj			- �ɦ�״_�P���A�Y���H�_�u�άO����

[�C�����O] -
mo			- �N�P
da <�P> 		- ���P
pon			- �I�P
eat <�P1> <�P2> 	- �Y�P�A�Ȥ@�ئY�P�զX�ɥi���ݿ�J�P�W
gon <�P>		- �b�P�A�t�b�ɻݿ�J�t�b�P�W
tin			- ť�P
hu			- �J�P
au pon <�P>		- �۰ʸI�P
au gon <�P>		- �۰ʺb�P
au hu <�P>		- �۰ʭJ�P
au tin			- �۰�ť�P
.			- �����D�e��/�����e��
lookall 		- �[�ݨ�L���a���P
substitute <���a>	- ����

[��L���O] -
lookmj			- �[��
bonus			- �x�ƭp���
HELP;

mapping score = allocate_mapping(0);

// �Y�ɤ��±N���
nosave mapping MJdata;

// ���a�y��ƧǸ�� ({ OB1, OB2, OB3, OB4 })
nosave object *Psort;

// �C�������a��� ([ PLAYER:([...]), ...])
nosave mapping Pdata;

// �~�P�᪺�P�l���Ǹ��
nosave string Ctile; 

// �O�W�Q���i�±N�P�����
nosave mapping MJtile=
([
    // �U
    "1w":({"�@�U",4,HIR,RED}),
    "2w":({"�G�U",4,HIR,RED}),
    "3w":({"�T�U",4,HIR,RED}),
    "4w":({"�|�U",4,HIR,RED}),
    "5w":({"��U",4,HIR,RED}),
    "6w":({"���U",4,HIR,RED}),
    "7w":({"�C�U",4,HIR,RED}),
    "8w":({"�K�U",4,HIR,RED}),
    "9w":({"�E�U",4,HIR,RED}),
    // ��
    "1s":({"�@��",4,HIG,GRN}),
    "2s":({"�G��",4,HIG,GRN}),
    "3s":({"�T��",4,HIG,GRN}),
    "4s":({"�|��",4,HIG,GRN}),
    "5s":({"���",4,HIG,GRN}),
    "6s":({"����",4,HIG,GRN}),
    "7s":({"�C��",4,HIG,GRN}),
    "8s":({"�K��",4,HIG,GRN}),
    "9s":({"�E��",4,HIG,GRN}),
    // ��
    "1t":({"�@��",4,HIC,CYN}),
    "2t":({"�G��",4,HIC,CYN}),
    "3t":({"�T��",4,HIC,CYN}),
    "4t":({"�|��",4,HIC,CYN}),
    "5t":({"�",4,HIC,CYN}),
    "6t":({"����",4,HIC,CYN}),
    "7t":({"�C��",4,HIC,CYN}),
    "8t":({"�K��",4,HIC,CYN}),
    "9t":({"�E��",4,HIC,CYN}),
    // �r�P
    "ea":({"�F��",4,HIY,YEL}),
    "so":({"�n��",4,HIY,YEL}),
    "we":({"�護",4,HIY,YEL}),
    "no":({"�_��",4,HIY,YEL}),
    "jo":({"����",4,HIR,RED}),
    "fa":({"�C�o",4,HIG,GRN}),
    "ba":({"�ժO",4,HIW,WHT}),
    "9t":({"�E��",4,HIC,CYN}),
    // ��P
    "f1":({"1 �K",1,HIW,HIR}),
    "f2":({"2 �L",1,HIW,HIR}),
    "f3":({"3 ��",1,HIW,HIR}),
    "f4":({"4 �V",1,HIW,HIR}),
    "f5":({"1 ��",1,HIR,WHT}),
    "f6":({"2 ��",1,HIR,WHT}),
    "f7":({"3 ��",1,HIR,WHT}),
    "f8":({"4 ��",1,HIR,WHT}),
]);

void seat_select();
void master_select();
void wash_mj();
void begining_select();
void deal_tile();
void initialize_player(object player);
void win_process(object winner, int cheat);
int valid_hu_check(string *tile, string *otile, string *flower);
void show_tile(object me);
void show_sea(object me);
void show_all(object me);
mapping bonus_calculate(object winner);

void do_join(object me, string arg);
void do_leave(object me, string arg);
void do_start(object me, string arg);
void do_restart(object me, string arg);
void do_reset(object me, string arg);
void do_redeal(object me, string arg);
void do_redraw(object me, string arg);
void do_next(object me, string arg);
void do_look(object me, string arg);
void do_eat(object me, string arg);
void do_da(object me, string arg);
void do_mo(object me, string arg);
void do_pon(object me, string arg);
void do_gon(object me, string arg);
void do_au(object me, string arg);
void do_tin(object me, string arg);
void do_hu(object me, string arg);
void do_lookmj(object me, string arg);
void do_lookall(object me, string arg);
void do_skyeye(object me, string arg);
void do_substitute(object me, mixed arg);
void do_top(object me, string arg);
void do_bill(object me, string arg);
void do_bonus(object me, string arg);
void do_fixmj(object me, string arg);

nosave mapping actions = 
([
    "join"	:	(:do_join:),
    "leave"	:	(:do_leave:),
    "start"	:	(:do_start:),
    "restart"	:	(:do_restart:),
    "reset"	:	(:do_reset:),
    "redeal"	:	(:do_redeal:),
    "redraw"	:	(:do_redraw:),
    "." 	:	(:do_look:),
    "eat"	:	(:do_eat:),
    "da"	:	(:do_da:),
    "mo"	:	(:do_mo:),
    "pon"	:	(:do_pon:),
    "gon"	:	(:do_gon:),
    "au"	:	(:do_au:),
    "tin"	:	(:do_tin:),
    "hu"	:	(:do_hu:),
    "lookmj"	:	(:do_lookmj:),
    "lookall"	:	(:do_lookall:),
    "next"	:	(:do_next:),
    "skyeye"    :       (:do_skyeye:),
    "substitute":	(:do_substitute:),
    "top"	:	(:do_top:),
    "bill"	:	(:do_bill:),
    "bonus"	:	(:do_bonus:),
    "fixmj"	:	(:do_fixmj:),
]);

// �P���Ƨ�
string *tile_sort(string *tile)
{
	return sort_array(tile, (: member_array($1, Tsort) > member_array($2, Tsort) ? 1 : -1 :));
}

// �ˬd�O�_�����a
int is_player(object me)
{
	return !undefinedp(Pdata[me]);
}

// ���㪺�P�r�ԭz
string full_chinese(string tile)
{
	if( member_array(tile, Tsort) == -1 ) return "ERROR";
	return kill_repeat_ansi(NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+NOR);
}

// �͹��n
varargs void beep(object me)
{
	if( undefinedp(me) )
		foreach( object ppl in Psort )
		tell(ppl, "\a", 0);
	else
		tell(me, "\a", 0);
}

// �ˬd�۰ʥ��P�\��, �۰ʨ����h�l���۰ʸI�b�\��
void auto_function_check(object ppl, string tile)
{
	if( member_array(tile, Pdata[ppl][AUTOPON]) != -1 )
	{
		tell(ppl, "�����۰ʸI"+full_chinese(tile)+"�C\n", 0);
		Pdata[ppl][AUTOPON] -= ({tile});
	}

	if( member_array(tile, Pdata[ppl][AUTOGON]) != -1 )
	{
		tell(ppl, "�����۰ʺb"+full_chinese(tile)+"�C\n", 0);
		Pdata[ppl][AUTOGON] -= ({tile});
	}
}

void clear_last_data()
{
	Ctile			= "";
	MJdata[DUMPED]		= allocate(0);
	MJdata[DUMPING] 	= 0;
	MJdata[TILELIMIT]	= 16;
	MJdata[FINISH]		= 0;
	MJdata[HUMYSELF]	= 0;
	MJdata[HUFIRST] 	= 0;
	MJdata[NINEYOUNGS]	= 0;
	MJdata[MO]		= 0;
	MJdata[TURN]		= 0;
	MJdata[WINNER]		= 0;
	MJdata[LOSER]		= 0;
	MJdata[KEYTILE] 	= 0;
	MJdata[LISTENING]	= 0;

	foreach( object player in Psort )
	{
		if( !objectp(player) )
			Psort -= ({ player });

		Pdata[player][TILE]		= allocate(0);
		Pdata[player][OTILE]		= allocate(0);
		Pdata[player][FLOWER]		= allocate(0);
		Pdata[player][AUTOPON]		= allocate(0);
		Pdata[player][AUTOGON]		= allocate(0);
		Pdata[player][AUTOHU]		= allocate(0);
		Pdata[player][SECRETGON]	= allocate(0);
		Pdata[player][LISTEN]		= allocate(0);
		Pdata[player][PDUMPED]		= allocate(0);
		Pdata[player][PLISTEN]		= 0;
		Pdata[player][LISTENTYPE]	= 0;
		Pdata[player][AUTOOFF]		= 0;
		Pdata[player][FLOWERNUM]	= 0;
		Pdata[player][LOOKALL]		= 0;
	}
}

// ���s���P��
void new_MJ()
{
	broadcast(environment(), HIW"\n\n�i"NOR WHT"���s�P���}�l\n"NOR);
	beep();

	Psort -= ({ 0 });	// �M���Ū��� (���u���a)
	clear_last_data();	// �M���C�����
	MJdata[PLAYING] = 1;	// �C���аO
	MJdata[ROUND]	= 1;	// �_�l���
	MJdata[ROUNDWIND] = 0;	// �_�l��
	seat_select();		// ��ܮy��
	master_select();	// ��ܲ��a
	begining_select();	// ��ܶ}��
	deal_tile();		// �o�P
}

// �U�@��
void next_MJ()
{
	if( MJdata[CFINISH] )
		return tell(this_player(), "�����P���w�g�����F�A���s�}�l�P���Ы� restart�C\n");

	if( !sizeof(Psort) )
		return;

	broadcast(environment(), HIW"\n\n�i"NOR WHT"��"+CHINESE_D->chinese_number(++MJdata[ROUND])+"��P���}�l\n"NOR);

	// ����
	if( MJdata[LOSER] == Psort[MASTER] || (MJdata[WINNER] && MJdata[WINNER] != Psort[MASTER]) )
	{
		// ���Y�������ˬd�O�_�w�����|��±N
		if( !(++MJdata[MROUND] % sizeof(Psort)) && !(++MJdata[ROUNDWIND] % sizeof(Psort)) )
		{
			broadcast(environment(), HIG"\n�w�����|��P���A�P�����s�}�l�C\n"NOR);
			new_MJ();
			return;
		}

		// ���U�a��
		Psort = Psort[1..] + Psort[0..0];

		broadcast(environment(), HIG"�i"NOR GRN"�����G"NOR+Pdata[Psort[MASTER]][IDNAME]+"\n");

		MJdata[CMASTER] = 0;
	}
	// �s��
	else
	{
		MJdata[CMASTER]++;
		broadcast(environment(), HIG"�i"NOR GRN"�s���s"+CHINESE_D->chinese_number(MJdata[CMASTER])+"�G"NOR+Pdata[Psort[MASTER]][IDNAME]+"\n");
	}

	// �M���W����T
	clear_last_data();

	// �}�����
	begining_select();

	// �o�P
	deal_tile();
}

// �����M���P�����
void reset_all_data()
{
	Psort			= allocate(0);
	Pdata			= allocate_mapping(0);
	MJdata			= allocate_mapping(0);
	MJdata[ONLOOKER]	= allocate(0);

	delete_temp("status", this_object());
}

// ��l���a���
void initialize_player(object player)
{
	Pdata[player]		= allocate_mapping(0);
	Pdata[player][PLAYER]	= player;
	Pdata[player][ID]	= player->query_id(1);
	Pdata[player][IDNAME]	= player->query_idname();
        Pdata[player][MONEY]    = 200000;

	// ��l�����t�θ�T
	if( !mapp(score[Pdata[player][ID]]) )
		score[Pdata[player][ID]] = allocate_mapping(0);
}

// �}�l�P��
void do_start(object me, string arg)
{
	int playernum = sizeof(Psort);
	int playerneed = 4-playernum;
	object AIob;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( MJdata[PLAYING] )
		return tell(me, "�P���w�g�}�l�F�C\n");

	if( playernum < 4 )
	{
		while(playerneed--)
		{
			AIob = new(MJAI_FILE);
			AIob->move(environment());
			AIob->do_command("join");
		}
	}

	set_temp("status", HIY"�P"NOR YEL"���i�椤"NOR, this_object());

	// �}�l�s�P��
	new_MJ();
}

// ��ܶi�C���e��
void show_title()
{
	int i;
	object ppl, ppl2;
	foreach( ppl in Psort )
	{
		clear_screen(ppl);
		spec_line_msg(ppl,     HIY"					   ';;;;;;;;;;;;;;;.", 1);
		spec_line_msg(ppl,     HIY"					     ';;;;;;;;;;;;;;;;.", 2);
		spec_line_msg(ppl,     HIY"    �O "NOR YEL"�W �� �v �Q �� �i �� �N �w "HIY"v"+VERSION+"     ';;;;;;;;;;;;;;;.", 3);
		spec_line_msg(ppl,     HIY"						';;;;;;;;;;;;;;;.", 4);
		spec_line_msg(ppl, NOR CYN"   �z�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�s�w�w�w�w�w�{", 5);
		spec_line_msg(ppl, NOR CYN"   ��"HIC"�е��ݤH���B���۰ʶ}�l�P���A�Ϋ� start �����}�l "NOR CYN"��"HIC"�ثe�H�� "+NOR CYN+sizeof(Psort)+"��", 6);
		spec_line_msg(ppl, NOR CYN"   �|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�r�w�w�w�w�w�}"NOR, 7);
		spec_line_msg(ppl,     HIY"						.;;;;;;;;;;;;;;;;.", 8);
		spec_line_msg(ppl,     HIY"					       .;;;;;;;;;;;;;;;'.", 9);
		spec_line_msg(ppl,     HIY"					      .;;;;;;;;;;;;;;'.", 10);
		spec_line_msg(ppl,     HIY"					    .;;;;;;;;;;;;;'.", 11);
		spec_line_msg(ppl,     HIY"					 .;;;;;;;;;;;;;'.", 12);
		spec_line_msg(ppl,     HIY"				      .;;;;;;;;;;;;;'", 13);
		spec_line_msg(ppl,     HIY"				  .;;;;;;;;;;;;'", 14);
		spec_line_msg(ppl,     HIY"			       ''''''"NOR, 15);
		i=8;
		foreach( ppl2 in Psort )
		spec_line_msg(ppl, sprintf(HIW"     �D%-20s "HIG"�w�X $200,000"NOR, ppl2->query_idname()), i++);
	}
}

// ��ܱƦ�]
void show_billboard(object me)
{
	int i;

	clear_screen(me);

	spec_line_msg(me,     HIC"					    ';;;;;;;;;;;;;;;.", 1);
	spec_line_msg(me,     HIC"					     ;;;;;;;;;;;;;;;;.", 2);
	spec_line_msg(me,     HIY"    �O "NOR YEL"�W �� �v �Q �� �i �� �N �w "HIY"v"+VERSION+HIC"     ';;;;;;;;;;;;;;;.", 3);
	spec_line_msg(me,     HIC"						';;;;;;;;;;;;;;;.", 4);
	spec_line_msg(me, NOR CYN"   �z "HIC"���a�w�X"NOR CYN" �w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{", 5);
	spec_line_msg(me, NOR CYN"   �� 							   "NOR CYN"��"NOR, 6);
	spec_line_msg(me, NOR CYN"   �� 							   "NOR CYN"��"NOR, 7);
	spec_line_msg(me, NOR CYN"   �� 							   "NOR CYN"��"NOR, 8);
	spec_line_msg(me, NOR CYN"   �� 							   "NOR CYN"��"NOR, 9);
	spec_line_msg(me, NOR CYN"   �|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}", 10);
	spec_line_msg(me,     HIC"					   .;;;;;;;;;;;;;'.", 11);
	spec_line_msg(me,     HIC"					.;;;;;;;;;;;;;'.", 12);
	spec_line_msg(me,     HIC"				     .;;;;;;;;;;;;;'", 13);
	spec_line_msg(me,     HIC"				 .;;;;;;;;;;;;'", 14);
	spec_line_msg(me,     HIC"			      ''''''"NOR, 15);
	i=6;
	foreach( object ppl in Psort )
	spec_line_msg(me, sprintf(NOR CYN"   ��"HIW"�D%-20s		    "HIG"�w�X $%s"NOR, Pdata[ppl][IDNAME], NUMBER_D->number_symbol(Pdata[ppl][MONEY])) , i++);
}

void do_bill(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	Pdata[me][LOOKSEA] = 1;

	show_billboard(me);
}

void do_top(object me, string arg)
{
	array kings = allocate(4);
	string id, str, *idsort;

	switch( arg )
	{
	case "mo"    : idsort = sort_array(keys(score), (: score[$1][HUMYSELVES] < score[$2][HUMYSELVES] ? 1 : -1 :)); break;
	case "hu"    : idsort = sort_array(keys(score), (: score[$1][WINTIMES] < score[$2][WINTIMES] ? 1 : -1 :)); break;
	case "gun"   : idsort = sort_array(keys(score), (: score[$1][GUNTIMES] < score[$2][GUNTIMES] ? 1 : -1 :)); break;
	case "win"   : idsort = sort_array(keys(score), (: (score[$1][HUMYSELVES]+score[$1][WINTIMES])*1./(score[$1][ROUNDS]||1) < (score[$2][HUMYSELVES]+score[$2][WINTIMES])*1./(score[$2][ROUNDS]||1) ? 1 : -1 :)); break;
	case "lose"  : idsort = sort_array(keys(score), (: score[$1][GUNTIMES]*1./(score[$1][ROUNDS]||1) < score[$2][GUNTIMES]*1./(score[$2][ROUNDS]||1) ? 1 : -1 :)); break;
	case "bonus" : idsort = sort_array(keys(score), (: score[$1][BONUSES] < score[$2][BONUSES] ? 1 : -1 :)); break;
	default:
		idsort = sort_array(keys(score), (: score[$1][SCORE] < score[$2][SCORE] ? 1 : -1 :)); break;
	}

	str =	   HIW"���a	     ����  ����       �ۺN	 ��j �J�P	 Ĺ��	    �駽 Ĺ�x ��x\n"NOR;
	str += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

	foreach( id in idsort )
	{
		//if( score[id][ROUNDS] < 10 )
		//{
		//	idsort -= ({ id });
		//	continue;
		//}

		str += sprintf(HIW"%-10s"HIG"%8d"HIW"%6d"HIY"%4d(%4.1f%%)"HIR"%4d(%4.1f%%)"HIC"%5d"HIW"%4d(%4.1f%%)"NOR WHT"%4d(%4.1f%%)"HIM"%5d"HIM"%5d\n"NOR
		    , capitalize(id)
		    , score[id][SCORE]
		    , score[id][ROUNDS]
		    , score[id][HUMYSELVES]
		    , score[id][HUMYSELVES]*100./(score[id][ROUNDS]||1)
		    , score[id][GUNTIMES]
		    , score[id][GUNTIMES]*100./(score[id][ROUNDS]||1)
		    , score[id][WINTIMES]
		    , score[id][HUMYSELVES]+score[id][WINTIMES]
		    , (score[id][HUMYSELVES]+score[id][WINTIMES])*100./(score[id][ROUNDS]||1)
		    , score[id][LOSETIMES]
		    , score[id][LOSETIMES]*100./(score[id][ROUNDS]||1)
		    , score[id][BONUSES]
		    , score[id][LOSEBONUSES]);

		if( score[id][ROUNDS] < 100 )
			idsort -= ({ id });

	};
	str += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

	foreach( id in idsort )
	{
		kings[0] = sort_array(unique_array(idsort, (: (score[$1][HUMYSELVES]+score[$1][WINTIMES])*1./(score[$1][ROUNDS]||1) :) ), (: (score[$1[0]][HUMYSELVES]+score[$1[0]][WINTIMES])*1./(score[$1[0]][ROUNDS]||1) < (score[$2[0]][HUMYSELVES]+score[$2[0]][WINTIMES])*1./(score[$2[0]][ROUNDS]||1) ? 1:-1 :) );
		kings[1] = sort_array(unique_array(idsort, (: score[$1][LOSETIMES]*1./(score[$1][ROUNDS]||1) :) ), (: score[$1[0]][LOSETIMES]*1./(score[$1[0]][ROUNDS]||1) < score[$2[0]][LOSETIMES]*1./(score[$2[0]][ROUNDS]||1) ? 1:-1 :) );
		kings[2] = sort_array(unique_array(idsort, (: score[$1][HUMYSELVES]*1./(score[$1][ROUNDS]||1) :) ), (: score[$1[0]][HUMYSELVES]*1./(score[$1[0]][ROUNDS]||1) < score[$2[0]][HUMYSELVES]*1./(score[$2[0]][ROUNDS]||1) ? 1:-1 :) );
		kings[3] = sort_array(unique_array(idsort, (: score[$1][GUNTIMES]*1./(score[$1][ROUNDS]||1) :) ), (: score[$1[0]][GUNTIMES]*1./(score[$1[0]][ROUNDS]||1) < score[$2[0]][GUNTIMES]*1./(score[$2[0]][ROUNDS]||1) ? 1:-1 :) );
	}
	str += HIW"�| �j "NOR WHT"�� ��"NOR"\n";
	if( sizeof(kings[0]) )
	str += sprintf(NOR CYN"�Ӳv�̰�   "HIC"�±N�L�G"HIW"%-10s"NOR WHT"%5.2f%%\n"NOR,capitalize(kings[0][0][0]),(score[kings[0][0][0]][HUMYSELVES]+score[kings[0][0][0]][WINTIMES])*100./(score[kings[0][0][0]][ROUNDS]));
	if( sizeof(kings[1]) )
	str += sprintf(NOR YEL"�Ѳv�̰�   "HIY"�b�t�t�G"HIW"%-10s"NOR WHT"%5.2f%%\n"NOR,capitalize(kings[1][0][0]),score[kings[1][0][0]][LOSETIMES]*100./(score[kings[1][0][0]][ROUNDS]));
	if( sizeof(kings[2]) )
	str += sprintf(NOR GRN"�ۺN�v�̰� "HIG"�ۺN���G"HIW"%-10s"NOR WHT"%5.2f%%\n"NOR,capitalize(kings[2][0][0]),score[kings[2][0][0]][HUMYSELVES]*100./(score[kings[2][0][0]][ROUNDS]));
	if( sizeof(kings[3]) )
	str += sprintf(NOR RED"�񬶲v�̰� "HIR"��j���G"HIW"%-10s"NOR WHT"%5.2f%%\n"NOR,capitalize(kings[3][0][0]),score[kings[3][0][0]][GUNTIMES]*100./(score[kings[3][0][0]][ROUNDS]));
	str += NOR WHT"���� 100 ���H�W								      v"VERSION"\n"NOR;

	str += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	me->more("\n"+kill_repeat_ansi(str)+"\n");
}

// �[�J�P��
void do_join(object me, string arg)
{
	if( is_player(me) )
		return tell(me, pnoun(2,me)+"�w�g�[�J�P���F�C\n");

	if( sizeof(Psort) == 4 )
	{
		foreach(object ob in Psort)
		{
			if( Pdata[ob][AI] )
			{
				do_substitute(ob, me);
				destruct(ob);
				return;
			}
		}

		return tell(me, "��p�P���w�g�Ⱥ��F�I\n");
	}

	if( MJdata[PLAYING] && sizeof(Psort) != 4 )
		return tell(me, "�ثe�L�k�[�J�P���C\n");

	// �M�����u���a
	Psort -= ({ 0 });

	Psort += ({ me });
	initialize_player(me);

	if( !userp(me) )
	{
		Pdata[me][AI] = 1;
		set_temp("mjobject", this_object(), me);
	}

	// �Ұ� title screen
	startup_title_screen(me, TITLE_SCREEN_LENGTH);

	msg("$ME�b�±N��ǧ��F�U�ӡC\n", me, 0, 1);
	MJdata[ONLOOKER] -= ({ me });
	me->add_status(STATUS);

	beep();

	if( sizeof(Psort) < 4 )
		msg("$ME�|�B�ۤ�G"+HIG+"�٤�"+CHINESE_D->chinese_number(4-sizeof(Psort))+"�}���֨ӭ��\n"NOR, me, 0, 1);

	// �|�H����Y�}�l�P��
	if( sizeof(Psort) == 4 )
	{
		msg("�H�Ƥw���A�P���}�l�C\n");
		do_start(me, arg);
		return;
	}

	// ��� Title �e��
	if( !MJdata[PLAYING] )
		show_title();

	set_temp("status", HIY"��"+NOR YEL+CHINESE_D->chinese_number(4-sizeof(Psort))+HIY"�}"NOR, this_object());
}

// ���}�P��
void do_leave(object me, string arg)
{
	int playernum;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	foreach( object ppl in Psort )
		if( objectp(ppl) && !Pdata[ppl][AI] ) playernum++;

	if( playernum == 1 )
		return do_reset(me, arg);
	else
	{
		object AIob = new(MJAI_FILE);
		AIob->move(environment());
		do_substitute(me, AIob);
		
		msg("$ME���}�±N�ण���F�C\n", me, 0, 1);
		do_mo(AIob, arg);
		return;
	}
}

// ���s�}�l�P�� (���a����)
void do_restart(object me, string arg)
{
	object player, *players = copy(Psort-({0}));

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	reset_all_data();

	Psort = players;

	foreach( player in Psort )
	initialize_player(player);

	msg(HIY"\n$ME�N�P�������~�F�@�M�A�P�����s�}�l�C\n"NOR,me, 0, 1);

	do_start(me, arg);
}

// ���s�]�m�±N��
void do_reset(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	foreach( object ppl in Psort )
	{
		if( !objectp(ppl) ) continue;

		ppl->remove_status(STATUS);
		ppl->remove_status(OSTATUS);
		reset_screen(ppl);

		if( !userp(ppl) )
			destruct(ppl);
	}

	foreach( object onlooker in MJdata[ONLOOKER] )
	{
		onlooker->remove_status(OSTATUS);
		reset_screen(onlooker);
	}

	reset_all_data();

	msg("$ME�N�±N������M�ŤF�C\n",me,0,1);
}

//���o
void do_redeal(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	if( arrayp(MJdata[NINEYOUNGS]) && member_array(me, MJdata[NINEYOUNGS]) != -1 )
	{
		msg("\n$ME�G�s�@�n�G"HIG"�E�\�E�M�աA���~�I�I\n"NOR, me, 0, 1);

		broadcast(environment(), HIG"�P�����s�o�P�C\n"NOR);

		next_MJ();
		return;
	}

	tell(me, "�S���S���Τ���n�D���s�~�P�C\n");
}

//���e�e��
void do_redraw(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	reset_screen(me);
	startup_title_screen(me, TITLE_SCREEN_LENGTH);

	show_tile(me);

	tell(me, "���s�Ұʵe�������C\n");
}

//�i��U�@��
void do_next(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	if( !MJdata[FINISH] )
		return tell(me, "����P���|�������A����i��U�@��P���C\n");

	if( MJdata[FINISH] > time() )
		return tell(me, "�Щ� "+(MJdata[FINISH]-time())+" ���A�i��U�@���C\n");

	if( sizeof(Psort) < 2 )
		return tell(me, "�ܤ֭n����쪱�a�~�i�H�}�l�P���C\n");

	next_MJ();
}

// �H���w�Ʈy��
void seat_select()
{
	int i, j;
	object *temp = copy(Psort);

	broadcast(environment(), HIY"�i"NOR YEL"���X�u"+implode(Dsort[0..sizeof(temp)-1], "�B")+"�v�H����P�M�w�y��\n"NOR);

	Psort = allocate(0);

	while(sizeof(temp))
	{
		i = random(sizeof(temp));
		Psort += ({ temp[i] });
		broadcast(environment(), HIY"�i "NOR YEL+Dsort[j++]+"�a�G"NOR+Pdata[temp[i]][IDNAME]+"\n");
		temp = temp[0..i-1] + temp[i+1..];
	}
}

// ���a���
void master_select()
{
	int i = random(16)+3;

	msg(HIG"�i"NOR GRN"�F�a$ME"GRN"�Y��l�M�w�_���A�Y�X��l�� "+i+" �I\n"NOR, Psort[0], 0, 1);

	i = ((i-1) % sizeof(Pdata));

	// �������s�̷ӭ춶�ǱƲ��a�쬰 index 0
	Psort = Psort[i..]+Psort[0..i-1];

	broadcast(environment(), HIG"�i "NOR GRN"���a�G"NOR+Pdata[Psort[MASTER]][IDNAME]+"\n");

	MJdata[TURN] = 0;
}

// ���P��m���
void begining_select()
{
	int i = random(16)+3;
	object *temp;

	msg(HIC"�i"NOR CYN"���a$ME"CYN"�Y��l�M�w���P��m�A�Y�X��l�� "+i+" �I\n"NOR, Psort[MASTER], 0, 1);

	// �����}������
	MJdata[OPENDOOR] = (i-1)%sizeof(Psort);

	broadcast(environment(), HIC"�i"NOR CYN+Dsort[MJdata[OPENDOOR]]+"���}���A�� "+(++i)+" �l�}�l���P�C\n"NOR);

	// �P�����ǽվ�
	Ctile = Ctile[i*2..]+Ctile[0..(i*2-1)];

	temp = Psort[MJdata[OPENDOOR]..]+Psort[0..MJdata[OPENDOOR]-1];

	for(i=0;i<sizeof(Psort);i++)
		Pdata[temp[i]][FLOWERNUM] = i;
}

// ���P�P���ƦC
void otile_display(string ref *arr, mapping data, int secretgon)
{
	string tile;

	arr = allocate(2, "");

	if( sizeof(data[FLOWER]) )
	{
		arr[0] += NOR WHT"��";
		arr[1] += NOR WHT"��";
		foreach( tile in data[FLOWER] )
		{
			arr[0] += NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+NOR WHT"��";
			arr[1] += NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+NOR WHT"��";
		}
	}
	if( sizeof(data[OTILE]) )
	{
		arr[0] += NOR WHT"��";
		arr[1] += NOR WHT"��";
		foreach( tile in data[OTILE] )
		{
			if( secretgon && member_array( tile, data[SECRETGON] ) != -1 )
			{
				arr[0] += HIW+"�t"+NOR WHT"��";
				arr[1] += NOR WHT"�b"+NOR WHT"��";
			}
			else
			{
				arr[0] += NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+NOR WHT"��";
				arr[1] += NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+NOR WHT"��";
			}
		}
	}
}


void show_dumping(object me)
{
	string tile;

	spec_line_msg(me, sprintf("	   "NOR WHT"��½ "HIW"%d"NOR WHT" �i�A���� "HIW"%d"NOR WHT" �i�A"HIW"%s"NOR WHT"����A"HIW"%s"NOR WHT"�a�}��"NOR, strlen(Ctile)/2, sizeof(MJdata[DUMPED])+stringp(MJdata[DUMPING]), Dsort[MJdata[ROUNDWIND]], Dsort[MJdata[OPENDOOR]]), 1);

	// �ثe���X���P����T
	if( tile = MJdata[DUMPING] )
	{
		spec_line_msg(me, HIW"�z�w�{"NOR, 1);
		spec_line_msg(me, HIW"��"+NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+NOR HIW"��", 2);
		spec_line_msg(me, HIW"��"+NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+NOR HIW"��", 3);
		spec_line_msg(me, HIW"�|"+NOR+MJtile[tile][COLOR1]+tile+HIW"�}"NOR, 4);
	}
}

// ��ܥD�e��
void show_tile(object me)
{
	string tile, str, *arr;
	mapping mydata, lastdata, nextdata, oppodata;

	// �C�����ݴN�M���@���e��
	clear_screen(me);

	if( Pdata[me][LOOKALL] )
	{
		show_all(me);
		return;
	}
	if( Pdata[me][LOOKSEA] )
	{
		show_sea(me);
		return;
	}

	// �ۮa
	mydata = Pdata[me];

	if( sizeof(mydata[FLOWER]+mydata[OTILE]) )
	{
		otile_display(ref arr, mydata, 0);

		str = "";
		if( sizeof(mydata[FLOWER]) ) str = HIW"  �z"+repeat_string("�w�s",sizeof(mydata[FLOWER])-1)+"�w�{";
		if( sizeof(mydata[OTILE]) ) str += (sizeof(mydata[FLOWER])?"":HIW"  ")+"�z"+repeat_string("�w�s",sizeof(mydata[OTILE])-1)+"�w�{"NOR;
		spec_line_msg(me, str, 9);
		spec_line_msg(me, NOR WHT"  "+kill_repeat_ansi(replace_string(arr[0], WHT, NOR)), 10);
		spec_line_msg(me, NOR WHT"  "+kill_repeat_ansi(arr[1])+NOR, 11);
		str = replace_string(str, "�s", "�r");
		str = replace_string(str, "�z", "�|");
		str = replace_string(str, "�{", "�}");
		spec_line_msg(me, replace_string(str, HIW, NOR WHT), 12);
	}

	arr = allocate(2, "��");

	foreach( tile in mydata[TILE] )
	{
		arr[0] += NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+HIW"��";
		arr[1] += NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+HIW"��";
	}

	spec_line_msg(me, HIW"�z"+repeat_string("�w�s",sizeof(mydata[TILE])-1)+"�w�{"NOR,12);
	spec_line_msg(me, HIW+arr[0]+" "+NOR+(me==Psort[0]?HIG"��"+(MJdata[CMASTER]?"�s"+CHINESE_D->chinese_number(MJdata[CMASTER]):"")+" "NOR:"")+(sizeof(mydata[LISTEN])?HIC"ť("+implode(mydata[LISTEN],",")+") "NOR:"")+NOR, 13);
	spec_line_msg(me, HIW+arr[1]+" "+NOR+HIM+Dsort[member_array(me, Psort)]+"�a "+(mydata[FLOWERNUM]+1)+" ��"+NOR, 14);
	spec_line_msg(me, HIW"�|"+implode(mydata[TILE], "�r")+"�}"+NOR+(member_array(me, Psort)==MJdata[TURN]?HBCYN" "NOR HIREV HIC+remove_ansi(mydata[IDNAME])+NOR HBCYN" ":" "+mydata[IDNAME]+" ")+NOR" ", 15);

	// ����T
	switch(sizeof(Psort))
	{
	case 2:
		oppodata = Pdata[(Psort - ({ me }))[0]];
		break;
	case 3:
		switch( member_array(me, Psort) )
		{
		case 0:
			nextdata = Pdata[Psort[1]];
			lastdata = Pdata[Psort[2]];
			break;
		case 1:
			nextdata = Pdata[Psort[2]];
			lastdata = Pdata[Psort[0]];
			break;
		case 2:
			nextdata = Pdata[Psort[0]];
			lastdata = Pdata[Psort[1]];
			break;
		}
		break;
	case 4:
		switch( member_array(me, Psort) )
		{
		case 0:
			nextdata = Pdata[Psort[1]];
			oppodata = Pdata[Psort[2]];
			lastdata = Pdata[Psort[3]];
			break;
		case 1:
			nextdata = Pdata[Psort[2]];
			oppodata = Pdata[Psort[3]];
			lastdata = Pdata[Psort[0]];
			break;
		case 2:
			nextdata = Pdata[Psort[3]];
			oppodata = Pdata[Psort[0]];
			lastdata = Pdata[Psort[1]];
			break;
		case 3:
			nextdata = Pdata[Psort[0]];
			oppodata = Pdata[Psort[1]];
			lastdata = Pdata[Psort[2]];
			break;
		}
		break;
	}

	// �U�a
	arr = allocate(2, "");
	if( mapp(nextdata) )
	{
		otile_display(ref arr, nextdata, 1);

		str = " "+(nextdata[PLAYER]==Psort[MASTER]?HIG"��"+(MJdata[CMASTER]?"�s"+CHINESE_D->chinese_number(MJdata[CMASTER]):"")+" "NOR:"") + (sizeof(nextdata[LISTEN])?HIC"ť "NOR:"");

		arr[0] = kill_repeat_ansi(arr[0]);
		arr[1] = kill_repeat_ansi(arr[1]);

		spec_line_msg(me, sprintf("%66s"NOR"%-s", repeat_string(" ",strlen(sizeof(nextdata[TILE])+""))+arr[0], str), 7);
		spec_line_msg(me, sprintf("%66s"NOR"%-s", HIW+sizeof(nextdata[TILE])+NOR+arr[1], (member_array(nextdata[PLAYER], Psort)==MJdata[TURN] ?HBCYN" "NOR HIREV HIC+remove_ansi(nextdata[IDNAME])+NOR HBCYN" ":" "+nextdata[IDNAME]+" ")+NOR" "), 8);
	}

	// �W�a
	if( mapp(lastdata) )
	{
		otile_display(ref arr, lastdata, 1);

		str = " "+(lastdata[PLAYER]==Psort[MASTER]?HIG"��"+(MJdata[CMASTER]?"�s"+CHINESE_D->chinese_number(MJdata[CMASTER]):"")+" "NOR:"") + (sizeof(lastdata[LISTEN])?HIC"ť "NOR:"");

		arr[0] = kill_repeat_ansi(arr[0]);
		arr[1] = kill_repeat_ansi(arr[1]);

		spec_line_msg(me, str+repeat_string(" ",noansi_strlen(lastdata[IDNAME])-noansi_strlen(str)+2)+arr[0]+NOR, 5);
		spec_line_msg(me, (member_array(lastdata[PLAYER], Psort)==MJdata[TURN]?HBCYN" "NOR HIREV HIC+remove_ansi(lastdata[IDNAME])+NOR HBCYN" ":" "+lastdata[IDNAME]+" ")+repeat_string(" ",noansi_strlen(str)-noansi_strlen(lastdata[IDNAME])-1)+NOR+arr[1]+HIW+sizeof(lastdata[TILE])+NOR, 6);
	}

	// ��a
	if( mapp(oppodata) )
	{
		otile_display(ref arr, oppodata, 1);

		str = " "+(oppodata[PLAYER]==Psort[MASTER]?HIG"��"+(MJdata[CMASTER]?"�s"+CHINESE_D->chinese_number(MJdata[CMASTER]):"")+" "NOR:"") + (sizeof(oppodata[LISTEN])?HIC"ť "NOR:"");

		arr[0] = kill_repeat_ansi(arr[0]);
		arr[1] = kill_repeat_ansi(arr[1]);

		spec_line_msg(me, "	   "+str+repeat_string(" ",noansi_strlen(oppodata[IDNAME])-noansi_strlen(str)+2)+arr[0]+NOR, 2);
		spec_line_msg(me, "	   "+(member_array(oppodata[PLAYER], Psort)==MJdata[TURN]?HBCYN" "NOR HIREV HIC+remove_ansi(oppodata[IDNAME])+NOR HBCYN" ":" "+oppodata[IDNAME]+" ")+repeat_string(" ",noansi_strlen(str)-noansi_strlen(oppodata[IDNAME])-1)+NOR+arr[1]+HIW+sizeof(oppodata[TILE])+NOR, 3);
	}

	show_dumping(me);
}
// �ݮ����P
void show_sea(object me)
{
	int i, j;
	string str1, str2;

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	clear_screen(me);

	spec_line_msg(me, HIW" ���ثe�b�ୱ�W�Ҧ����P�G"NOR, 2);

	str1 = str2 = HIW"��"NOR;

	if( arrayp(MJdata[DUMPED]) )
	{
		foreach( string tile in MJdata[DUMPED]+ (MJdata[DUMPING]?({MJdata[DUMPING]}):({})) )
		{
			if( !(++j%20) )
			{
				str1 += NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+HIW"��"NOR;
				str2 += NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+HIW"��"NOR;
				spec_line_msg(me, HIW"�z"+repeat_string("�w�s", 19)+"�w�{"NOR, i+3);
				spec_line_msg(me, kill_repeat_ansi(str1), i+4);
				spec_line_msg(me, kill_repeat_ansi(str2), i+5);
				spec_line_msg(me, HIW"�|"+repeat_string("�w�r", 19)+"�w�}"NOR, i+6);
				i+=4;
				str1 = str2 = HIW"��"NOR;
			}
			else
			{
				str1 += NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+HIW"��"NOR;
				str2 += NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+HIW"��"NOR;
			}
		}

		if( j%20 )
		{
			spec_line_msg(me, HIW"�z"+repeat_string("�w�s", j%20-1)+"�w�{"NOR, i+3);
			spec_line_msg(me, kill_repeat_ansi(str1), i+4);
			spec_line_msg(me, kill_repeat_ansi(str2), i+5);
			spec_line_msg(me, HIW"�|"+repeat_string("�w�r", j%20-1)+"�w�}"NOR, i+6);
		}
	}
}

// �ݦۤv�P
void do_look(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	if( Pdata[me][LOOKSEA] || Pdata[me][LOOKALL] )
	{
		Pdata[me][LOOKSEA] = 0;
		Pdata[me][LOOKALL] = 0;
		show_tile(me);
	}
	else
	{
		Pdata[me][LOOKSEA] = 1;
		show_sea(me);
	}
}

void show_all_tile()
{
	foreach( object ppl in Psort )
	show_tile(ppl);

	foreach( object ppl in MJdata[ONLOOKER] )
	{
		if( !objectp(ppl) )
		{
			MJdata[ONLOOKER] -= ({ ppl });
			continue;
		}
		show_all(ppl);
	}
}

// �i��ɪ�ʧ@
void check_flower(mapping ref data, string tile)
{
	while( sscanf(tile, "f%*d") == 1 )
	{
		msg("$ME��X�@�i"+full_chinese(tile)+"�P�ɻ��D�G"HIG"�ɪ�C\n"NOR, data[PLAYER], 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

		data[FLOWER] = tile_sort(data[FLOWER]+({ tile }));
		data[TILE] -= ({ tile });

		// �ɪ�ѵP����
		tile = Ctile[<2..<1];
		Ctile = Ctile[0..<3];

		msg("$ME�ѵP���N�F�@�i�s���P�C\n", data[PLAYER], 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
		tell(data[PLAYER], pnoun(2, data[PLAYER])+"�N��F�@�i"+full_chinese(tile)+"�C\n", 0);

		data[TILE] += ({ tile });

		auto_function_check(data[PLAYER], tile);

		show_all_tile();

		// �ɪ�J�P
		if( valid_hu_check(tile_sort(data[TILE]), data[OTILE], data[FLOWER]) )
		{
			beep();
			msg("$ME�ܾ������j�s�@�n�G"HIY"�y�ۺN�z�աI�z����������\n"NOR, data[PLAYER], 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

			// �b�W�}��
			data[TAILTILE] = 1;
			MJdata[HUMYSELF] = 1;
			MJdata[KEYTILE] = tile;

			// �ˬd�O�_�ѭJ
			if( !sizeof(MJdata[DUMPED]) )
				MJdata[HUFIRST] = 1;

			win_process(data[PLAYER], 0);

			if( sizeof(Psort) < 4 )
				set_temp("status", HIY"��"+NOR YEL+CHINESE_D->chinese_number(4-sizeof(Psort))+HIY"�}"NOR, this_object());
			MJdata[FINISH] = time()+20;
			return;
		}
	}
}

// �}�l�o�P
void deal_tile()
{
	int i, j;
	string tile, *youngs;
	object ppl;

	set_temp("status", HIY"�P"NOR YEL"���i�椤"NOR, this_object());

	// �~�P
	wash_mj();

	// ���]���a���
	foreach( ppl in Psort )
	Pdata[ppl][TILE] = allocate(0);

	// �C�|�l���y���P
	for(i=0;i<4;i++)
		foreach( ppl in Psort )
	{
		// �ѵP�e�}�l���P, �C�|�i�@��
		for(j=0;j<4;j++)
			Pdata[ppl][TILE] += ({ Ctile[j*2..j*2+1] });

		Ctile = Ctile[8..];
	}

	// �̫���a�h���@�i
	Pdata[Psort[MASTER]][TILE] += ({ Ctile[0..1] });
	Ctile = Ctile[2..];

	show_all_tile();

	broadcast(environment(), HIM"�i"NOR MAG"���P�����A�Ѳ��a�}�l�ɪ�G\n"NOR);

	foreach( ppl in Psort )
	{
		foreach( tile in Pdata[ppl][TILE] )
		check_flower(ref Pdata[ppl], tile);

		msg("$ME���D�G"HIG"�L�ɰաI\n"NOR, ppl, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
		Pdata[ppl][TILE] = tile_sort(Pdata[ppl][TILE]);
	}

	//�E�\�E�M
	foreach( ppl in Psort )
	{
		i = 0;
		youngs = ({"1w","9w","1s","9s","1t","9t","ea","we","so","no","jo","fa","ba"});

		foreach( tile in Pdata[ppl][TILE] )
		{
			if( member_array(tile, youngs) != -1 ) i++;
			youngs -= ({ tile });
		}
		if( i >= 9 )
		{
			tell(ppl, "\n"HIG+pnoun(2, ppl)+"���P���E�\�E�M�A�i�n�D���o(redeal)�C\n"NOR, 0);
			if( !arrayp(MJdata[NINEYOUNGS]) )
				MJdata[NINEYOUNGS] = ({ ppl });
			else
				MJdata[NINEYOUNGS] += ({ ppl });
		}

		// ��l�۰ʸI�b�]�w
		if( Pdata[ppl][AI] )
			ppl->AI_pg();
	}

	show_all_tile();

	// ���a�ѭJ ?
	if( valid_hu_check(Pdata[Psort[MASTER]][TILE], Pdata[Psort[MASTER]][OTILE], Pdata[Psort[MASTER]][FLOWER]) )
	{
		beep();
		msg("$ME����Y���j�s�@�n�G"HIY"�ڡ���y�ѭJ�z�աI�z������������������\n"NOR, ppl, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
		MJdata[HUFIRST] = 1;
		win_process(ppl, 0);

		if( sizeof(Psort) < 4 )
			set_temp("status", HIY"��"+NOR YEL+CHINESE_D->chinese_number(4-sizeof(Psort))+HIY"�}"NOR, this_object());
		MJdata[FINISH] = time()+20;
		return;
	}

	if( Pdata[Psort[MASTER]][AI] )
	{
		Psort[MASTER]->emotion_begin();
		call_out((:do_da, Psort[MASTER], call_other(Psort[MASTER], "AI_da"):), 5);
	}
}

// �üƬ~�P
void wash_mj()
{
	int i, num;
	string str = "";
	mapping temp = copy(MJtile);

	foreach( string eng, array data in temp )
	while(data[AMOUNT]--) str += eng;

	// �g�~�Q���P, ���Ӱ��ça
	for(i=0;i<10;i++)
	{
		while(sizeof(str))
		{
			num = random(sizeof(str))/2*2;
			Ctile = random(2) ? Ctile + str[num..num+1] : str[num..num+1] + Ctile;
			str = str[0..num-1] + str[num+2..];
		}
		str = Ctile;
		Ctile = "";
	}

	Ctile = str;
}

// ���P
void do_da(object me, string arg)
{
	int i;
	mapping data;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	if( MJdata[FINISH] )
		return tell(me, "�w�����@��P���A�Y�n�~��i��U�@��P���Ы� next�C\n");

	if( !arg ) return tell(me, pnoun(2, me)+"�Q�n�����i�P�H\n");

	arg = lower_case(arg);

	if( Psort[MJdata[TURN]] != me )
		return tell(me, "�٨S������"+pnoun(2, me)+"�I\n");

	if( MJdata[MO] )
		return tell(me, pnoun(2, me)+"�٨S�N�P�I\n");

	data = Pdata[me];

	if( sizeof(data[LISTEN]) && !data[AUTOOFF] && previous_object() )
		return tell(me, pnoun(2, me)+"�w�gť�P�F�A����ۤv����P�ʧ@�C\n");

	i = member_array(arg, data[TILE]);

	if( i == -1 ) return tell(me, pnoun(2, me)+"�S�� "+arg+" �o�i�P�C\n");

	if( stringp(MJdata[DUMPING]) )
		MJdata[DUMPED] += ({ copy(MJdata[DUMPING]) });

	MJdata[DUMPING] = arg;

	data[TILE] = data[TILE][0..i-1] + data[TILE][i+1..];

	msg("$ME���X�F�@�i"+full_chinese(arg)+"�C\n", data[PLAYER], 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

	data[AUTOOFF] = 0;

	// check �O�_���Hť�P�B�J�F, �B�U�a���J
	foreach( object ppl in Psort[MJdata[TURN]+1..]+Psort[0..MJdata[TURN]-1] )
	{
		if( member_array(arg, Pdata[ppl][LISTEN]) != -1 || member_array(arg, Pdata[ppl][AUTOHU]) != -1 )
		{
			MJdata[LOSER] = me;
			MJdata[KEYTILE] = arg;
			do_hu(ppl, 0);
			return;
		}
	}
	MJdata[MO] = 1;

	Psort -= ({ 0 });

	if( MJdata[TURN] == sizeof(Psort) - 1 )
		MJdata[TURN] = 0;
	else
		MJdata[TURN]++;

	MJdata[NINEYOUNGS] = 0;

	// ���쪺�H beep
	beep(Psort[MJdata[TURN]]);

	show_all_tile();

	// ���쪺�H beep
	beep(Psort[MJdata[TURN]]);

	// �p�G�ۤv�N�۰ʸI�b�P���X, �h�����۰�
	auto_function_check(me, arg);

	// �����P�� check �O�_�n���� AUTOHU
	foreach( string tile in data[AUTOHU] )
	if( !valid_hu_check(tile_sort(data[TILE]+({tile})), data[OTILE], data[FLOWER]) )
	{
		tell(me, "�����۰ʭJ"+full_chinese(tile)+"�C\n", 0);
		data[AUTOHU] -= ({ tile });
	}

	// check �O�_���H�n�۰ʸI�b�έJ
	foreach( object ppl in Psort )
	{
		if( ppl == me ) continue;

		if( arrayp(Pdata[ppl][AUTOGON]) && member_array(arg, Pdata[ppl][AUTOGON]) != -1 )
		{
			Pdata[ppl][AUTOGON] -= ({ arg });
			do_gon(ppl, 0);
			return;
		}
		if( arrayp(Pdata[ppl][AUTOPON]) && member_array(arg, Pdata[ppl][AUTOPON]) != -1 )
		{
			Pdata[ppl][AUTOPON] -= ({ arg });
			do_pon(ppl, 0);
			return;
		}
	}

	// AI �����P���ˬd�I�b���A�P�O�_ť�P, ��ť���P, ��ť���
	if( Pdata[me][AI] )
	{
		if( !sizeof(Pdata[me][LISTEN]) )
		{
			array tin = me->AI_tin();
	
			me->AI_pg();
	
			if( arrayp(tin) )
			{
	
				if( !intp(tin[0]) )
				{
					foreach(string tile in tin )
					if( member_array(tile, Pdata[me][AUTOHU]) != -1 )
						tin -= ({ tile });
	
					if( sizeof(tin) )
					{
						do_au(me, "hu "+implode(tin, " "));
					}
				}
				else
				{
					do_tin(me, 0);
				}
			}
		}
		
		if( MJdata[LISTENING] )
		{
			Pdata[me][AUTOPON] = allocate(0);
			Pdata[me][AUTOGON] = allocate(0);
		}
	}

	// �}�lť�P
	if( Pdata[me][PLISTEN] )
	{
		Pdata[me][PLISTEN] = 0;
		do_tin(me, 0);
	}

	Pdata[me][PDUMPED] += ({ arg });

	// �Y�U�a�O AI
	if( Pdata[Psort[MJdata[TURN]]][AI] )
	{
		string *eat_tiles;

		// AI �ˬd�O�_�ӦY
		if( arrayp(eat_tiles = Psort[MJdata[TURN]]->AI_eat()) )
			do_eat(Psort[MJdata[TURN]], implode(eat_tiles, " "));
	}

	// check �U�ať�P�۰ʺN
	if( sizeof(Pdata[Psort[MJdata[TURN]]][LISTEN]) || Pdata[Psort[MJdata[TURN]]][AI] )
		call_out((:do_mo, Psort[MJdata[TURN]], 0:), 1);
}

void do_eat(object me, string arg)
{
	int i1, i2, *s;
	string t1, t2, b1, b2;
	mapping data;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	if( MJdata[FINISH] )
		return tell(me, "�w�����@��P���A�Y�n�~��i��U�@��P���Ы� next�C\n");

	if( !MJdata[DUMPING] )
		return tell(me, "�{�b�S���P�i�H�Y�I\n");

	if( !MJdata[MO] )
		return tell(me, "�{�b�L�k�Y�P�C\n");

	if( Psort[MJdata[TURN]] != me )
		return tell(me, "�٨S������"+pnoun(2, me)+"�I\n");

	data = Pdata[me];

	// �۰ʤ��R�Y�P�P��
	if( !arg )
	{
		int choices;

		if( member_array(MJdata[DUMPING][1], ({ 'w', 's', 't' })) == -1 )
			return tell(me, "�S����k�o�˦Y��I\n");

		if( MJdata[DUMPING][0] == '1' )
		{
			t1 = "2"+MJdata[DUMPING][1..1];
			t2 = "3"+MJdata[DUMPING][1..1];
		}
		else if( MJdata[DUMPING][0] == '9' )
		{
			t1 = "7"+MJdata[DUMPING][1..1];
			t2 = "8"+MJdata[DUMPING][1..1];
		}
		else
		{
			if( MJdata[DUMPING][0] >= '3' )
			{
				b1 = ""+(MJdata[DUMPING][0]-50)+MJdata[DUMPING][1..1];
				b2 = ""+(MJdata[DUMPING][0]-49)+MJdata[DUMPING][1..1];
				if( member_array(b1, data[TILE]) != -1 && member_array(b2, data[TILE]) != -1 )
				{
					t1 = b1;
					t2 = b2;
					choices++;
				}
			}

			b1 = ""+(MJdata[DUMPING][0]-49)+MJdata[DUMPING][1..1];
			b2 = ""+(MJdata[DUMPING][0]-47)+MJdata[DUMPING][1..1];
			if( member_array(b1, data[TILE]) != -1 && member_array(b2, data[TILE]) != -1 )
			{
				t1 = b1;
				t2 = b2;
				choices++;
			}

			if( MJdata[DUMPING][0] <= '7' )
			{
				b1 = ""+(MJdata[DUMPING][0]-47)+MJdata[DUMPING][1..1];
				b2 = ""+(MJdata[DUMPING][0]-46)+MJdata[DUMPING][1..1];
				if( member_array(b1, data[TILE]) != -1 && member_array(b2, data[TILE]) != -1 )
				{
					t1 = b1;
					t2 = b2;
					choices++;
				}
			}

			if( choices > 1 )
				return tell(me, "�ثe���h�ئY�P�զX�A�Цۦ��J�䤤��i�P�W�C\n");

			if( !choices )
				return tell(me, "�S���P�i�H�Y�I\n");
		}
	}
	else if( sscanf(arg, "%s %s", t1, t2) != 2 )
		return tell(me, pnoun(2, me)+"�Q�έ���i�P�ӦY�P�H\n");

	if( strlen(t1) == 1 )
		t1 += MJdata[DUMPING][1..1];
	if( strlen(t2) == 1 )
		t2 += MJdata[DUMPING][1..1];

	i1 = member_array(t1, data[TILE]);
	if( i1 == -1 ) return tell(me, pnoun(2, me)+"�S�� "+t1+" �o�i�P�C\n");
	i2 = member_array(t2, data[TILE]);
	if( i2 == -1 ) return tell(me, pnoun(2, me)+"�S�� "+t2+" �o�i�P�C\n");

	if( t1[1] == t2[1] && t2[1] == MJdata[DUMPING][1] )
	{
		s = sort_array( ({ to_int(t1[0..0]), to_int(t2[0..0]), to_int(MJdata[DUMPING][0..0]) }), 1 );

		if( s[0] != 0 && s[0] == s[1] - 1 && s[1] == s[2] - 1 )
		{
			msg("$ME���F�@�n�G"HIG"�Y�I\n"NOR, me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
			msg("$ME���X"+full_chinese(t1)+"�M"+full_chinese(t2)+"�N"+full_chinese(MJdata[DUMPING])+"�Y�U�C\n", me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

			data[OTILE] += ({ sort_array(({t1,t2}),1)[0], MJdata[DUMPING], sort_array(({t1,t2}),1)[1] });

			data[TILE] = data[TILE][0..i1-1] + data[TILE][i1+1..];
			i2 = member_array(t2, data[TILE]);
			data[TILE] = data[TILE][0..i2-1] + data[TILE][i2+1..];

			MJdata[DUMPING] = 0;
			MJdata[MO] = 0;

			auto_function_check(me, t1);
			auto_function_check(me, t2);
			show_all_tile();

			if( data[AI] )
			{
				me->emotion_eat();
				call_out((:do_da, me, call_other(me, "AI_da"):), 2);
			}
			return;
		}
	}

	tell(me, "�S����k�o�˦Y��I\n");
}


void do_pon(object me, string arg)
{
	int i;
	mapping data;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	if( MJdata[FINISH] )
		return tell(me, "�w�����@��P���A�Y�n�~��i��U�@��P���Ы� next�C\n");

	if( !MJdata[DUMPING] )
		return tell(me, "�{�b�S���P�i�H�I�I\n");

	if( !MJdata[MO] )
		return tell(me, "�{�b�L�k�I�P�C\n");

	data = Pdata[me];

	i = member_array(MJdata[DUMPING], data[TILE]);

	if( i != -1 && i < sizeof(data[TILE])-1 && data[TILE][i+1] == MJdata[DUMPING] )
	{
		beep();
		msg("$ME�j�ۤ@�n�G"HIG"�I�աI\n"NOR, me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
		msg("$ME�N�T�i"+full_chinese(MJdata[DUMPING])+"�\����e�C\n", me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

		data[OTILE]	+= allocate(3, MJdata[DUMPING]);
		data[TILE]	= data[TILE][0..i-1] + data[TILE][i+2..];
		data[AUTOPON]	-= ({ MJdata[DUMPING] });
		MJdata[DUMPING] = 0;
		MJdata[MO]	= 0;
		MJdata[TURN]	= member_array(me, Psort);

		show_all_tile();

		if( data[AI] )
		{
			me->emotion_pon();
			call_out((:do_da, me, call_other(me, "AI_da"):), 2);
		}
	}
	else
		return tell(me, pnoun(2,me)+"�S���P�i�H�I�աI\n");
}

void do_gon(object me, string arg)
{
	int i, j;
	mapping data;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	if( MJdata[FINISH] )
		return tell(me, "�w�����@��P���A�Y�n�~��i��U�@��P���Ы� next�C\n");

	data = Pdata[me];

	if( sizeof(data[LISTEN]) && !data[AUTOOFF] )
		return tell(me, "�ثe��ť�P���A�A�����H�N�b�P�C\n");

	// �t�b�θI�P�b
	if( arg )
	{
		if( Psort[MJdata[TURN]] != me )
			return tell(me, "�٨S������"+pnoun(2, me)+"�I\n");

		if( MJdata[MO] )
			return tell(me, "�{�b�L�k�t�b�C\n");

		// �ۺN�P�t�b
		i = member_array(arg, data[TILE]);
		if( i != -1 && i < sizeof(data[TILE]) - 3 && data[TILE][i] == data[TILE][i+3] )
		{
			msg("$ME�j�ۤ@�n�G"HIG"�b�աI\n"NOR, me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
			msg("$ME���h�n�N���N�|�i�t�b�P�\����e�C\n", me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

			data[OTILE]	+= allocate(4, arg);
			data[AUTOGON]	-= ({ arg });
			data[TILE]	= data[TILE][0..i-1] + data[TILE][i+4..];
			data[SECRETGON] += ({ arg });
			data[TAILTILE]	= 1;
			MJdata[MO]	= 1;
			MJdata[TILELIMIT]++;
			show_all_tile();

			if( data[AI] )
				call_out((:do_mo, me, 0:), 1);

			return;
		}

		// �ۤv���I�P�A�b
		i = member_array(arg, data[TILE]);
		j = member_array(arg, data[OTILE]);
		if( i != -1 && j != -1 && j < sizeof(data[OTILE]) - 2 && data[OTILE][j] == data[OTILE][j+2] )
		{
			msg("$ME�j�ۤ@�n�G"HIG"�b�աI\n"NOR, me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
			msg("$ME�N"+full_chinese(arg)+"�\����e�C\n", me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

			// �Q�m�b
			foreach( object ppl in Psort[MJdata[TURN]+1..]+Psort[0..MJdata[TURN]-1] )
			if( member_array(arg, Pdata[ppl][LISTEN]) != -1 )
			{
				MJdata[LOSER] = me;
				MJdata[KEYTILE] = arg;
				MJdata[DUMPING] = arg; // fixed by sinji �m�b�ɷ|�B�J
				do_hu(ppl, 0);
				return;
			}

			data[AUTOGON]	-= ({ arg });
			data[OTILE]	= data[OTILE][0..j+2]+({arg})+data[OTILE][j+3..];
			data[TILE]	= data[TILE][0..i-1] + data[TILE][i+1..];
			data[TAILTILE]	= 1;
			MJdata[MO]	= 1;
			MJdata[TILELIMIT]++;
			show_all_tile();
			return;
		}
	}

	if( !MJdata[DUMPING] )
		return tell(me, "�{�b�S���P�i�H�b�I\n");

	if( !MJdata[MO] )
		return tell(me, "�{�b�L�k�b�P�C\n");

	/* ť������o�˺b -.-
		// �b�W�a�I�P
		i = member_array(arg, data[OTILE]);
		if( i != -1 && i < sizeof(data[OTILE]) - 2 && data[OTILE][i] == data[OTILE][i+2] && data[OTILE][i] == MJdata[DUMPING] )
		{
			msg("$ME�j�ۤ@�n�G"HIG"�b�աI\n"NOR, me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
			msg("$ME�N"+full_chinese(arg)+"�\����e�C\n", me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

			data[OTILE] = data[OTILE][0..i+2]+({arg})+data[OTILE][i+3..];
			MJdata[DUMPING] = 0;
			data[TAILTILE]	= 1;
			MJdata[MO]	= 1;
			MJdata[TILELIMIT]++;
			show_all_tile();
			return;
		}
	*/
	// ���b
	i = member_array(MJdata[DUMPING], data[TILE]);
	if( i != -1 && i < sizeof(data[TILE]) - 2 && data[TILE][i] == data[TILE][i+2] && data[TILE][i] == MJdata[DUMPING])
	{
		if( Psort[MJdata[TURN]] == me )
		{
			if( Pdata[me][AI] )
			{
				call_out((:do_mo, me, call_other(me, "AI_da"):), 1);
				return;
			}

			return tell(me, "����b�W�a���P��C\n");
		}

		beep();
		msg("$ME�j�ۤ@�n�G"HIG"�b�աI\n"NOR, me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);
		msg("$ME�N�|�i"+full_chinese(MJdata[DUMPING])+"�\����e�C\n", me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

		data[OTILE]	+= allocate(4, MJdata[DUMPING]);
		data[TILE]	= data[TILE][0..i-1] + data[TILE][i+3..];
		data[AUTOGON] -= ({ MJdata[DUMPING] });
		MJdata[DUMPING] = 0;
		MJdata[TURN]	= member_array(me, Psort);
		MJdata[TILELIMIT]++;
		data[TAILTILE]	= 1;

		show_all_tile();

		if( data[AI] )
			call_out((:do_mo, me, 0:), 1);

		return;
	}
	return tell(me, pnoun(2,me)+"�S���P�i�H�b�աI\n");
}

int tile_logic_seperate_1(array ref group, int i)
{
	int j, sep;

	// sep ���� 1
	j=i+1;
	while( j < sizeof(group) && sizeof(group[j]) == 1
	    && group[i][0][1] == group[j][0][1]
	    && group[j][0][0] - group[i][0][0] == j-i ) j++;

	if( !((j-i) % 3) )
		sep = 1;

	// sep ���� 2
	if( !sep && i < sizeof(group) - 5
	    && group[i+1][0][0] == group[i+5][0][0] - 4
	    && sizeof(unique_array(group[i+1..i+5], (: $1[0][1] :))) == 1
	    && sizeof(group[i+1]) == sizeof(group[i+5]) && sizeof(group[i+1]) == 1
	    && sizeof(unique_array(group[i+2..i+4], (: sizeof($1) :))) == 1
	    && sizeof(group[i+2]) > 1 )
		sep = 1;

	// sep ���� 3
	if( !sep && i < sizeof(group) - 4
	    && group[i+1][0][0] == group[i+4][0][0] - 3
	    && sizeof(unique_array(group[i+1..i+4], (: $1[0][1] :))) == 1
	    && sizeof(group[i+1]) == 1
	    && sizeof(group[i+2]) == sizeof(group[i+3]) + 1
	    && sizeof(group[i+3]) == sizeof(group[i+4]) )
		sep = 1;

	// sep ���� 4
	if( !sep && i < sizeof(group) - 4
	    && group[i+1][0][0] == group[i+4][0][0] - 3
	    && sizeof(unique_array(group[i+1..i+4], (: $1[0][1] :))) == 1
	    && sizeof(group[i+1]) == 2
	    && sizeof(group[i+2]) == sizeof(group[i+3]) + 1
	    && sizeof(group[i+3]) == sizeof(group[i+4]) + 1)
		sep = 1;

	// sep ���� 5
	if( !sep && i < sizeof(group) - 3
	    && group[i+1][0][0] == group[i+3][0][0] - 2
	    && sizeof(unique_array(group[i+1..i+3], (: $1[0][1] :))) == 1
	    && sizeof(group[i+1]) == 2 && sizeof(group[i+2]) == 2 && sizeof(group[i+3]) == 1 )
		sep = 1;

	// sep ���� 6
	if( !sep && i < sizeof(group) - 2 && sizeof(group[i+1]) == 4)
		sep = 1;

	if( sep )
		group = group[0..i-1]+({ group[i][0..<2] }) + ({ group[i][<1..<1] })+group[i+1..];

	return sep;
}


// �P���޿���R�P����
array tile_logic_check(string *tiles)
{
	int i, j;
	array group;

	group = unique_array(tiles, (:$1:));

	//--(1)
	//1 22 33 4	-> 12 2 33 4	 -> 123 2 3 4	->(1)
	//1 2 33 4 5	-> 12 33 4 5	 -> 123 3 4 5	->(1)
	//1 222 333 44	-> 12 22 333 44  -> 123 22 33 44->(1)
	//1 2222 3	-> 12 222 3	 -> 123 222	->(3)
	//1 2 3333	-> 12 3333	 -> 123 333	->(3)
	//1 2 333	-> 12 333	 -> 123 33	->(2)
	//1 222 3	-> 12 22 3	 -> 123 22	->(2)

	//--(2)
	//11 22 33	    -> 12 1 2 33	    -> 123 1 2 3	->(3)
	//11 222 333 4	    -> 12 1 22 333 4	    -> 123 1 22 33 4	->(1)
	//11 2222 3333 44   -> 12 1 222 3333 44     -> 123 1 222 333 44 ->(1)
	//11 22 33 44	    -> 12 1 2 33 44	    -> 123 1 2 3 44	->(1)
	//11 2 3 4	    -> (P)
	//11 2 33 44 5	    -> (P)
	//11 2 333 444 55   -> (P)
	//11 22 333 444 5   -> (P)
	//11 2 3333 4444 555-> (P)
	//11 22 3333 4444 55-> (P)
	//11 222 3333 4444 5-> (P)

	//--(3)
	//111 2 3		 -> 11 1 2 3		 ->(1)
	//111 2 3 4 5 6 7 8 9	 -> 11 1 2 3 4 5 6 7 8 9 ->(1)

	//111 2 3 4 5 6 	 -> 11 1 2 3 4 5 6	 ->(1)
	//111 2 33 44 55 6	 -> 11 1 2 33 44 55 6	 ->(1)
	//111 2 333 444 555 6	 -> 11 1 2 333 444 555 6 ->(1)
	//111 2 3333 4444 5555 6 -> 11 1 2 3333 4444 5555 6 ->(1)

	//111 2 33 4 5		 -> 11 1 2 33 4 5	 ->(1)
	//111 2 333 44 55	 -> 11 1 2 333 44 55	 ->(1)
	//111 2 3333 444 555	 -> 11 1 2 3333 444 555  ->(1)

	//111 22 33 4		 -> 11 1 22 33 4	 ->(1)
	//111 22 333 44 5	 -> 11 1 22 333 44 5	 ->(1)
	//111 22 3333 444 55	 -> 11 1 22 3333 444 55  ->(1)

	//111 2222 3333 444
	//111 2222 3

	//111 222 3333 444 5	 -> (P)
	//111 2 3 4		 -> (P)
	//111 2 3 4 5 6 7	 -> (P)
	//111 22 33 44		 -> (P)
	//111 222 333 444	 -> (P)
	//111 2222 3333 4444	 -> (P)
	//111 222 333		 -> (P)
	//111 222 33		 -> (P)
	//111 2 33 44 5 	 -> (P)
	//111 2 333 444 55	 -> (P)
	//111 22 333 444 5	 -> (P)
	//111 2 3333 4444 555	 -> (P)
	//111 22 3333 4444 55	 -> (P)
	//111 222 3333 4444 5	 -> (P)
	//111 2 333 4444 555 6	 -> (P)
	//111 2222 3333 4	 -> (P)
	//123			 -> (P)

	//--(4)
	// 1111 2 3		-> 111 1 2 3
	// 1111 2 3 4 5 6	->
	// 1111 2 3 4 5 6 7 8 9 ->
	// 1111 2 33 44 5	->

	// 1111 22 33		-> 11 11 22 33
	// 1111

	for(i=0;i<sizeof(group);i++)
	{
		// ���O���l�P
		if( group[i][0][0] < '0' || group[i][0][0] > '9' ) continue;

		// �Ĥ@�P���Ĥ@�Ʀr�j�� 8
		if( group[i][0][0] >= '8' ) continue;

		switch( sizeof(group[i]) )
		{
			// �u���@�i�P, ��©��U�@�i�P���ˬd
		case 1	:

			// �ܤ��٭n��դ~�����եi��
			if( i >= sizeof(group) - 2 ) continue;

			// ���P�����l
			if( group[i][0][1] != group[i+1][0][1] ) continue;

			// �Ʀr���s��
			if( group[i][0][0] != group[i+1][0][0] - 1 ) continue;

			// ���յP�l
			if( sizeof(group[i+1]) == 1 )
			{
				j = 1;
				group[i] += group[i+1];
				group = group[0..i]+group[i+2..];
			}
			else
			{
				j = 2;
				group[i] += group[i+1][0..0];
				group[i+1] = group[i+1][1..];
			}

			// ���s�B�z�ثe���P�l
			i--;
			continue;
			break;
			// ����i�P, �ݩ��U��յP���ˬd
		case 2:
			// 12
			if( group[i][0][0] == group[i][1][0] - 1 )
			{
				// �ܤ��٭n���@�դ~�����եi��
				if( i >= sizeof(group) - 1 ) continue;

				// ���P�����l
				if( group[i][0][1] != group[i+j][0][1] ) continue;

				// �Ʀr���s��
				if( group[i][<1][0] != group[i+j][0][0] - 1 ) continue;

				// ���յP�l
				if( sizeof(group[i+j]) == 1 )
				{
					group[i] += group[i+j];
					group = group[0..i+j-1]+group[i+j+1..];
				}
				else
				{
					group[i] += group[i+j][0..0];
					group[i+j] = group[i+j][1..];
				}

				// �զX 123 ����, ���� i-- ���s��z

			}
			// 11 ���թ���t��ﶶ�l
			else
			{
				// �ܤ��٭n����դ~�����եi��
				if( i >= sizeof(group) - 2 ) continue;

				// �᭱���P�դ����H�t��ﶶ
				if( sizeof(group[i+1]) < 2 || sizeof(group[i+2]) < 2 ) continue;

				// ���P�����l
				if( group[i][0][1] != group[i+1][0][1] || group[i][0][1] != group[i+2][0][1] ) continue;

				// �Ʀr���s��
				if( group[i][<1][0] != group[i+1][0][0] - 1 || group[i][<1][0] != group[i+2][0][0] - 2 ) continue;

				// �S�� Pass ���A
				if( i < sizeof(group) - 4
				    && sizeof(unique_array(group[i+1..i+3], (: $1[0][1] :)))==1
				    && group[i+3][0][0] - group[i+1][0][0] == 2
				    && sizeof(group[i+2]) == sizeof(group[i+3])
				    && sizeof(group[i+1])+sizeof(group[i+4]) == sizeof(group[i+2]) ) continue;

				j = 3;
				group = group[0..i-1]+({ group[i][0..0] })+({ group[i][1..1] })+group[i+1..];
				group[i] += group[i+2][0..0];
				group[i+2] = group[i+2][1..];
				i--;

			}
			continue;
			break;
		case 3:
			// �w�g�O���l���N���L
			if( group[i][0][0] == group[i][2][0] - 2 ) continue;

			// �ܤ��٭n����դ~�����եi��
			if( i >= sizeof(group) - 2 ) continue;

			// ���P�����l
			if( group[i][0][1] != group[i+1][0][1] || group[i][0][1] != group[i+2][0][1] ) continue;

			// �Ʀr���s��
			if( group[i][<1][0] != group[i+1][0][0] - 1 || group[i][<1][0] != group[i+2][0][0] - 2 ) continue;

			tile_logic_seperate_1(ref group, i);
			continue;
			break;
		case 4:
			// �ܤ��٭n����դ~�����եi��
			if( i >= sizeof(group) - 2 ) continue;

			// ���P�����l
			if( group[i][0][1] != group[i+1][0][1] || group[i][0][1] != group[i+2][0][1] ) continue;

			// �Ʀr���s��
			if( group[i][<1][0] != group[i+1][0][0] - 1 || group[i][<1][0] != group[i+2][0][0] - 2 ) continue;

			// �Y������}�@��, �h�������}���, �� size 2 �ˬd
			if( !tile_logic_seperate_1(ref group, i) )
				group = group[0..i-1]+({ group[i][0..1] }) + ({ group[i][2..3] })+group[i+1..];
			continue;
			break;
		}
	}
	return group;
}

void do_bonus(object me, string arg)
{
	string bonus = @BONUS

���O�W�Q���i�±N�x�ƭp��

���J	[ 0 �x] �m�b	[ 1 �x] �M�@��	[ 8 �x]
���a	[ 1 �x] �T�P�x	[ 1 �x] �r�@��	[ 8 �x]
�ۺN	[ 1 �x] ���J	[ 2 �x] �|�b�P	[ 8 �x]
���M	[ 1 �x] �T�t��	[ 2 �x] �ať	[ 8 �x]
���M�ۺN[ 1 �x] �P�ժ�	[ 2 �x] �p�|��	[ 8 �x]
���P�x	[ 1 �x] ���D�H	[ 2 �x] �K�P�L��[ 8 �x]
��P�x	[ 1 �x] �I�I�J	[ 4 �x] �C�m�@	[ 8 �x]
��Q	[ 1 �x] �p�T�P	[ 4 �x] �j�|��	[16 �x]
�J���}�P[ 1 �x] �V�@��	[ 4 �x] ��ť	[16 �x]
�J��i�P[ 1 �x] �|�t��	[ 5 �x] �a�J	[16 �x]
��������[ 1 �x] ���t��	[ 8 �x] �ѭJ	[24 �x]
�b�W�}��[ 1 �x] �j�T�P	[ 8 �x]
BONUS;

	tell(me, bonus);
}

// �x�ƭp��
mapping bonus_calculate(object winner)
{
	int i, j;
	string t;
	array group, allgroup;
	mapping bonus = allocate_mapping(0);
	mapping data = Pdata[winner];
	string *flower = data[FLOWER];
	string *tile = data[TILE];
	string *otile = data[OTILE];
	string *listen = data[LISTEN];

	// ���Rť���X�i�P
	if( !sizeof(listen) )
	{
		string *original_tile = tile - ({ MJdata[KEYTILE] });
		
		foreach( t in Tsort )
		{
			i = member_array(t, original_tile );
	
			// ���L��W�w�g�����b�P
			if( i != -1 && i < sizeof(original_tile)-3 && original_tile[i] == original_tile[i+3] ) continue;
	
			if( valid_hu_check(tile_sort(original_tile + ({ t })), 0, 0) )
				listen += ({ t });
		}
	}
	allgroup = tile_logic_check(tile);
	if( sizeof(otile) )
	{
		for(i=0;i<sizeof(otile);i++)
		{
			// �b�l�Ƥ@��
			if( i < sizeof(otile) - 3 && otile[i] == otile[i+3] )
			{
				allgroup += ({ otile[i..i+3] });
				i+=3;
				continue;
			}
			else
			{
				allgroup += ({ tile_sort(otile[i..i+2]) });
				i+=2;
				continue;
			}
		}
	}

	// ���a
	if( Psort[MASTER] == winner || MJdata[LOSER] == Psort[MASTER] )
		bonus["���a"] = 1;

	if( MJdata[CMASTER] )
	{
		if( Psort[MASTER] == winner || MJdata[LOSER] == Psort[MASTER] )
//			bonus["�s"+CHINESE_D->chinese_number(MJdata[CMASTER])] = MJdata[CMASTER];

//		else if( MJdata[LOSER] == Psort[MASTER] )
			bonus["�s"+CHINESE_D->chinese_number(MJdata[CMASTER])+"��"+CHINESE_D->chinese_number(MJdata[CMASTER])] = MJdata[CMASTER]*2;
	}

	// �ۺN
	if( MJdata[HUMYSELF] )
		bonus["�ۺN"] = 1;

	// ���M
	i=0;
	if( !sizeof(otile) )
		bonus["���M"] = 1;
	else
		foreach( t in otile )
	{
		if( member_array(t, data[SECRETGON]) == -1 )
			i = 1;
	}
	if( i==0 )
		bonus["���M"] = 1;

	if( bonus["���M"] && bonus["�ۺN"] )
	{
		map_delete(bonus, "���M");
		map_delete(bonus, "�ۺN");
		bonus["���M�@�N�T"] = 3;
	}

	// ���P
	foreach( group in allgroup )
	{
		if( sizeof(group) >= 3 && (i = member_array(group[0], ({"ea","so","we","no"}))) != -1 )
		{

			// �魷
			if( i == MJdata[ROUNDWIND] )
				bonus[Dsort[i]+"��"]++;

			// ���a��
			if( i == member_array(winner, Psort) )
				bonus[Dsort[i]+"��"]++;

			if( bonus[Dsort[i]+"��"] == 2 )
			{
				map_delete(bonus, Dsort[i]+"��");
				bonus[Dsort[i]+"��"+Dsort[i]] = 2;
			}
		}
	}

	// ��P
	foreach( t in flower )
	if( t[1]-49 == data[FLOWERNUM] || t[1]-53 == data[FLOWERNUM] ) 
	{
		string flowername = CHINESE_D->chinese_number(data[FLOWERNUM]+1)+"��";
		if( ++bonus[flowername] == 2 )
		{
			map_delete(bonus, flowername);
			bonus["��"+flowername] = 2;
			break;
		}
	}

	// ��Q
	if( sizeof(listen) == 1 )
		bonus["��Q"] = 1;


		// �J��i�P
	if( sizeof(listen) && !bonus["��Q"])
		foreach( group in tile_logic_check(tile) )
		if( sizeof(group) == 3 && group[0][0] == group[2][0] - 2 )
		{
			if(( MJdata[KEYTILE] == group[2] && group[2][0] == '3' )
			    || ( MJdata[KEYTILE] == group[0] && group[0][0] == '7' ))
				bonus["�J��i�P"] = 1;
		}

		// �J���}�P
	if( sizeof(listen) && !bonus["��Q"] && !bonus["�J��i�P"] )
		foreach( group in tile_logic_check(tile) )
		if( sizeof(group) == 3 && group[0][0] == group[2][0] - 2 && MJdata[KEYTILE] == group[1] )
			bonus["�J���}�P"] = 1;

		// �����N��
	if( MJdata[HUMYSELF] && (strlen(Ctile)/2) == MJdata[TILELIMIT] )
		bonus["��������"] = 1;

	// �b�W�}��
	if( data[TAILTILE] )
		bonus["�b�W�}��"] = 1;

	// �m�b
	foreach( object ppl in Psort-({winner}) )
	foreach( group in tile_logic_check(Pdata[ppl][TILE]) )
	if( sizeof(group) == 3 && group[0] == MJdata[KEYTILE] && member_array(group[0], Pdata[ppl][AUTOGON]) != -1)
	{
		bonus["�m�b"] = 1;
		break;
	}

	// ���J
	i=0;
	if( !sizeof(flower) && !MJdata[HUMYSELF] && !MJdata["��Q"] && !bonus["�J���}�P"] )
	{
		foreach( group in allgroup )
		{
			if( sizeof(group) >= 3 && group[0][0] != group[2][0] - 2 )
				i = 1;
			else if( sizeof(group) == 2
			    && (member_array(group[0], ({"ea","so","we","no"})) == MJdata[ROUNDWIND]
				|| member_array(group[0], ({"ea","so","we","no"})) == member_array(winner, Psort) ))
				i = 1;
		}


		if( i == 0 )
			bonus["���J"] = 2;
	}

	// �T,�|,���t��
	i = sizeof(data[SECRETGON]);
	foreach( group in tile_logic_check(tile) )
	{
		if( sizeof(group) == 3 && group[0] == group[2] && member_array(group[0], listen) == -1 )
			i++;
	}
	if( i == 3 )
		bonus["�T�t��"] = 2;
	else if( i == 4 )
		bonus["�|�t��"] = 5;
	else if( i == 5 )
		bonus["���t��"] = 8;

	// �P�ժ�
	if( sizeof(flower) >= 4 )
	{
		if( sizeof(flower & ({"f1","f2","f3","f4"})) == 4 )
			bonus["�K�L��V"] += 2;
		if( sizeof(flower & ({"f5","f6","f7","f8"})) == 4 )
			bonus["�����˵�"] += 2;
	}

	// ���D�H
	if( sizeof(tile) == 2 )
		bonus["���D�H"] = 2;


	// �I�I�J
	i=0;
	foreach( group in allgroup )
	{
		if( sizeof(group) >= 3 && group[0] != group[2] )
			i = 1;
	}
	if( i == 0 )
		bonus["�I�I�J"] = 4;

	// �p�T�P, �j�T�P, �T�P�x
	i=j=0;
	foreach( group in allgroup )
	{
		if( sizeof(group) >= 3 && group[0] == group[2] && member_array(group[0], ({"jo","ba","fa"})) != -1 ) 
		{
			i++;
			bonus[MJtile[group[0]][0]] = 1;
		}
		else if( sizeof(group) == 2 && group[0] == group[1] && member_array(group[0], ({"jo","ba","fa"})) != -1 )
			j++;
	}
	
	if( i == 3 || (i == 2 && j == 1) ) 
	{
		map_delete(bonus, "�C�o");
		map_delete(bonus, "����");
		map_delete(bonus, "�ժO");

		if( i == 3 )
			bonus["�j�T�P"] = 8;
		else if( i == 2 && j == 1 )
			bonus["�p�T�P"] = 4;
	}

	// �M�@��
	i=j=0;
	foreach( group in allgroup )
	{
		if( !i ) i = group[0][1];
		else if( i != group[0][1] ) j = 1;
	}
	if( j == 0 )
		bonus["�M�@��"] = 8;

	// �V�@��, �βM�@��(�r�P)
	i=j=0;
	foreach( group in allgroup )
	{
		if( member_array(group[0], ({"jo","ba","fa","ea","so","we","no"})) != -1 ) continue;

		if( !i ) i = group[0][1];
		else if( i != group[0][1] ) j = 1;
	}
	if( i == 0 )
		bonus["�r�@��"] = 8;
	else if( j == 0 && !bonus["�M�@��"] )
		bonus["�V�@��"] = 4;

	// �|�b�P
	i=j=0;
	for(i=0;i<sizeof(otile)-3;i++)
		if( otile[i] == otile[i+3] )
			j++;
	if( j >= 4 )
		bonus["�|�b�P"] = 8;

	// �p�|��, �j�|��
	i=j=0;
	foreach( group in allgroup )
	{
		if( sizeof(group) >= 3 && group[0] == group[2] && member_array(group[0], ({"ea","so","we","no"})) != -1 )
			i++;
		else if( sizeof(group) == 2 && group[0] == group[1] && member_array(group[0], ({"ea","so","we","no"})) != -1 )
			j++;
	}
	if( i == 4 )
		bonus["�j�|��"] = 16;
	else if( i == 3 && j == 1 )
		bonus["�p�|��"] = 8;

	// �ať
	if( data[LISTENTYPE] == 2 )
		bonus["�ať"] = 8;

	// �K�P�L��
	if( sizeof(flower) == 8 )
		bonus["�K�P�L��"] = 8;

	// �C�m�@
	if( sizeof(flower) == 7 )
		foreach( object ppl in Psort )
	{
		if( ppl == winner ) continue;
		if( sizeof(Pdata[ppl][FLOWER]) )
		{
			bonus["�C�m�@"] = 8;
			break;
		}
	}

	// ��ť
	if( data[LISTENTYPE] == 1 )
		bonus["��ť"] = 16;

	// �ѭJ
	if( MJdata[HUFIRST] )
		bonus["�ѭJ"] = 24;

	else if( sizeof(MJdata[DUMPED]) <= 8 )
	{
		i = 0;
		foreach( object ppl in Psort )
		if( sizeof(Pdata[ppl][OTILE]) )
			i = 1;

		if( i == 0 )
			bonus["�a�J"] = 16;
	}

	if( !sizeof(bonus) )
		bonus["���J"] = 0;

	return bonus;
}

// Ĺ�a�ƥ�B�z
void win_process(object winner, int cheat)
{
	int AIs, bonus, money, loss;
	string bonus_str = "", str, *arr;
	mapping data = Pdata[winner];
	object ppl;

	foreach( string bonus_type, int bonus_number in bonus_calculate(winner) )
	{
		bonus += bonus_number;
		bonus_str = sprintf(HIC"%-12s%2d �x\n", bonus_type, bonus_number) + bonus_str;
	}
	bonus_str += NOR;

	if( cheat )
	{
		bonus = 0;
		bonus_str += HIR"�B�J "+bonus+" �x\n"NOR;
		MJdata[LOSER] = 0;
	}

	if( file_size(DATA_PATH) > 0 )
	{
		restore_object(DATA_PATH);
		set_idname("mj table", HIY"�O"NOR YEL"�W�Q���i�±N��"NOR);
	}

	foreach( ppl in Psort )
	{
		if( undefinedp(score[Pdata[ppl][ID]]) )
			score[Pdata[ppl][ID]] = allocate_mapping(0);

		clear_screen(ppl);

		if( sizeof(data[FLOWER]+data[OTILE]) )
		{
			otile_display(ref arr, data, 0);

			str = "";
			if( sizeof(data[FLOWER]) ) str = HIW"  �z"+repeat_string("�w�s",sizeof(data[FLOWER])-1)+"�w�{";
			if( sizeof(data[OTILE]) ) str += (sizeof(data[FLOWER])?"":HIW"	")+"�z"+repeat_string("�w�s",sizeof(data[OTILE])-1)+"�w�{"NOR;
			spec_line_msg(ppl, str, 2);
			spec_line_msg(ppl, NOR WHT"  "+kill_repeat_ansi(replace_string(arr[0], WHT, NOR)), 3);
			spec_line_msg(ppl, NOR WHT"  "+kill_repeat_ansi(arr[1])+NOR, 4);
			str = replace_string(str, "�s", "�r");
			str = replace_string(str, "�z", "�|");
			str = replace_string(str, "�{", "�}");
			spec_line_msg(ppl, replace_string(str, HIW, NOR WHT), 5);
		}

		arr = allocate(2, "��");
		foreach( string tile in data[TILE] )
		{
			arr[0] += NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+HIW"��";
			arr[1] += NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+HIW"��";
		}
		spec_line_msg(ppl, HIW"�z"+repeat_string("�w�s",sizeof(data[TILE])-1)+"�w�{"NOR, 5);
		spec_line_msg(ppl, HIW+arr[0]+NOR+(data[PLAYER]==Psort[MASTER]?HIG"�� "NOR:"")+(sizeof(data[LISTEN])?HIC"ť("+implode(data[LISTEN],",")+") "NOR:""), 6);
		spec_line_msg(ppl, HIW+arr[1]+NOR+HIM+Dsort[member_array(winner, Psort)]+"�a "+(data[FLOWERNUM]+1)+" ��"NOR, 7);
		if( MJdata[KEYTILE] )
			spec_line_msg(ppl, HIW"�|"+replace_string(implode(data[TILE],"�r"),MJdata[KEYTILE],HIR+MJdata[KEYTILE]+HIW,1,1)+"�}"+NOR+data[IDNAME]+NOR, 8);
		else
			spec_line_msg(ppl, HIW"�|"+implode(data[TILE],"�r")+"�}"+NOR+data[IDNAME]+NOR, 8);
		// fixed by sinji at 2003/6/25 *Bad argument 2 to replace_string() - Expected: string Got: 0.
		// ���ˬd����P�O�_�s�b

		if( MJdata[KEYTILE] )
			spec_line_msg(ppl, HIG" ����P�G"NOR+full_chinese(MJdata[KEYTILE]), 10);

		spec_line_msg(ppl, HIC" �J�P�G"NOR+Pdata[winner][IDNAME]+NOR, 12);

		if( MJdata[LOSER] )
			spec_line_msg(ppl, HIR" ��j�G"NOR+Pdata[MJdata[LOSER]][IDNAME]+NOR, 13);

		spec_line_msg(ppl, HIW" �x�ƭp��G�`�p "+bonus+" �x"NOR, 15);
	}

//	broadcast(environment(), HIW"�x�ƭp��G�`�p "+bonus+" �x\n"NOR+bonus_str+"\n");
		broadcast(environment(), HIW"�w�w�w�w�w�w�w�w�w�w�w�w\n"+NOR+bonus_str+HIW"�w�w�w�w�w�w�w�w�w�w�w�w\n"HIW"�x�ƭp��G�`�p "+bonus+" �x\n\n");

	if( cheat )
	{
		msg("\n$ME�Q���H�o�{"HIR"��B�J��\n\n"NOR, winner, 0, 1);

		money = BASEMONEY;

		if( Pdata[winner][MONEY] < money*(sizeof(Psort)-1) )
		{
			money = Pdata[winner][MONEY] / (sizeof(Psort)-1);
			Pdata[winner][MONEY] = 0;
			loss = 1;
		}
		else
			Pdata[winner][MONEY] -= money;

		score[Pdata[winner][ID]][SCORE] -= money*(sizeof(Psort)-1);

		foreach( ppl in Psort-({winner}) )
		{
			msg(HIW"$ME"HIW"�I��$YOU"HIG"�w�X $"+NUMBER_D->number_symbol(money)+"�C\n"NOR, winner, ppl, 1);
			Pdata[ppl][MONEY] += money;
			score[Pdata[ppl][ID]][SCORE] += money;
		}

		MJdata[LOSER] = winner;
		score[Pdata[winner][ID]][GUNTIMES]++;
		score[Pdata[winner][ID]][LOSETIMES]++;
	}
	else
	{
		MJdata[WINNER] = winner;

		money = bonus*BONUSMONEY+BASEMONEY;

		// ����j��
		if( MJdata[LOSER] )
		{
			score[Pdata[winner][ID]][WINTIMES]++;

			if( Pdata[MJdata[LOSER]][MONEY] < money )
			{
				money = Pdata[MJdata[LOSER]][MONEY];
				Pdata[MJdata[LOSER]][MONEY] = 0;
				loss = 1;
			}
			else
				Pdata[MJdata[LOSER]][MONEY] -= money;

			score[Pdata[MJdata[LOSER]][ID]][SCORE] -= money;

			msg(HIW"$ME"HIW"�I��$YOU"HIG"�w�X $"+NUMBER_D->number_symbol(money)+"�C\n"NOR, MJdata[LOSER], winner, 1);

			Pdata[winner][MONEY] += money;
			score[Pdata[winner][ID]][SCORE] += money;
			score[Pdata[winner][ID]][BONUSES] += bonus;

			score[Pdata[MJdata[LOSER]][ID]][LOSEBONUSES] += bonus;
			score[Pdata[MJdata[LOSER]][ID]][GUNTIMES]++;
			score[Pdata[MJdata[LOSER]][ID]][LOSETIMES]++;

		}
		// �ۺN����
		else
		{
			score[Pdata[winner][ID]][HUMYSELVES]++;

			foreach( ppl in Psort-({winner}) )
			{
				money = bonus*BONUSMONEY+BASEMONEY;
				if( Pdata[ppl][MONEY] < money )
				{
					money = Pdata[ppl][MONEY];
					Pdata[ppl][MONEY] = 0;
					loss = 1;
				}
				else
					Pdata[ppl][MONEY] -= money;

				// �ˬd���a�s��
				if( ppl == Psort[MASTER] )
				{
					score[Pdata[ppl][ID]][LOSEBONUSES] += 1;
					score[Pdata[winner][ID]][BONUSES] ++;

					money += BONUSMONEY;
					msg(HIG"$ME"HIG"���a�h�� 1 �x�C\n"NOR, ppl, 0, 1);

					// �s X �� X
					if( MJdata[CMASTER] )
					{
						score[Pdata[ppl][ID]][LOSEBONUSES] += MJdata[CMASTER]*2;
						score[Pdata[winner][ID]][BONUSES] += MJdata[CMASTER]*2;

						msg(HIG"$ME"HIG"���a�s"+CHINESE_D->chinese_number(MJdata[CMASTER])+"��"+CHINESE_D->chinese_number(MJdata[CMASTER])+"�A�� "+(MJdata[CMASTER]*2)+" �x�C\n"NOR, ppl, 0, 1);
						money += MJdata[CMASTER]*2*BONUSMONEY;
					}
				}

				score[Pdata[ppl][ID]][SCORE] -= money;
				score[Pdata[ppl][ID]][LOSEBONUSES] += bonus;
				score[Pdata[ppl][ID]][LOSETIMES]++;

				msg(HIW"$ME"HIW"�I��$YOU"HIG"�w�X $"+NUMBER_D->number_symbol(money)+"�C\n"NOR, ppl, winner, 1);

				Pdata[winner][MONEY] += money;
				score[Pdata[winner][ID]][SCORE] += money;
				score[Pdata[winner][ID]][BONUSES] += bonus;
			}
		}
	}

	broadcast(environment(), "\n");

	foreach( ppl in Psort )
	{
		if( Pdata[ppl][AI] )
		{
			loss = 0;
			Pdata[ppl][MONEY] = 200000;
			AIs++;
		}

		score[Pdata[ppl][ID]][ROUNDS]++;

		if( Pdata[ppl][SCORE] == 0 )
			msg(HIR"$ME�w�g�� $"+NUMBER_D->number_symbol(money)+" �w�X�骺����A�L�k�~��U�@���P���F�C\n"NOR, ppl, 0, 1);

		else if( !loss )
			tell(ppl, "�~��i��U�@�P���Ы� next�C\n", 0);
	}

	if( AIs == 4 )
		call_out((:next_MJ:), 10);

	if( loss )
		MJdata[CFINISH] = 1;


	save_object(DATA_PATH);
}

// �ˬd�O�_�w�g�J�P
int valid_hu_check(string *tile, string *otile, string *flower)
{
	int gon4, valid = 1;
	int i, *form = allocate(6, 0);

	foreach( array tmp in tile_logic_check(tile) )
	{
		if( sizeof(tmp) == 2 && tmp[0] != tmp[1] )
			valid = 0;

		form[sizeof(tmp)-1]++;
	}

	if( form[1] > 1 || form[0] || form[3] ) valid = 0;

	if( arrayp(flower) )
	{
		// �C�m�@�J�P
		if( sizeof(flower) == 7 )
			foreach( object ppl in Psort )
			if( sizeof(Pdata[ppl][FLOWER]) == 1 )
			{
				MJdata[LOSER] = ppl;
				MJdata[KEYTILE] = Pdata[ppl][FLOWER][0];
				valid = 1;
				break;
			}

			// �K�P�L���J�P
			else if( sizeof(flower) == 8 )
				valid = 1;
	}

	// �|�b�P
	for(i=0;i<sizeof(otile)-3;i++)
		if( otile[i] == otile[i+3] )
			gon4++;

	if( gon4 >= 4 )
		valid = 1;

	return valid;
}

// �J�P���O
void do_hu(object me, string arg)
{
	int cheat;
	mapping data;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	if( MJdata[FINISH] )
		return tell(me, "�w�����@��P���A�Y�n�~��i��U�@��P���Ы� next�C\n");

	// ��X��j�̩��ˬd�O�_��J
	if( MJdata[TURN] == 0 )
	{
		if( Psort[<1] == me && MJdata[MO] )
			return tell(me, pnoun(2, me)+"�Q�B�J�ܡI�H\n");

		if( !MJdata[LOSER] )
			MJdata[LOSER] = Psort[<1];
	}
	else
	{
		if( Psort[MJdata[TURN]-1] == me && MJdata[MO] )
			return tell(me, pnoun(2, me)+"�Q�B�J�ܡI�H\n");

		if( !MJdata[LOSER] )
			MJdata[LOSER] = Psort[MJdata[TURN]-1];
	}
	data = Pdata[me];

	// �B�J
	if( !valid_hu_check(data[TILE], data[OTILE], data[FLOWER]) )
	{
		if( MJdata[DUMPING] )
		{
			if( valid_hu_check( tile_sort(data[TILE]+({MJdata[DUMPING]})), data[OTILE], data[FLOWER]) )
				data[TILE] = tile_sort(data[TILE]+({MJdata[DUMPING]}));
			else cheat = 1;
		}
		else cheat = 1;
	}

	if( cheat && Pdata[me][AI] )
		return;

	MJdata[KEYTILE] = MJdata[DUMPING];

	beep();
	msg("$ME�ܾ������j�s�@�n�G"HIY"�ڭJ�F�աI�z����������\n"NOR, me, 0, 1);

	if( cheat )
		MJdata[LOSER] = me;

	win_process(me, cheat);

	if( sizeof(Psort) < 4 )
		set_temp("status", HIY"��"+NOR YEL+CHINESE_D->chinese_number(4-sizeof(Psort))+HIY"�}"NOR, this_object());

	
	MJdata[FINISH] = time()+20;
	
	//set_idname("mj table", HIY"�O"NOR YEL"�W�Q���i�±N��"NOR);
}

void do_mo(object me, string arg)
{
	string tile;
	mapping data;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	if( MJdata[FINISH] )
		return tell(me, "�w�����@��P���A�Y�n�~��i��U�@��P���Ы� next�C\n");

	if( !MJdata[MO] )
		return tell(me, "�{�b����N�P�C\n");

	if( Psort[MJdata[TURN]] != me )
		return tell(me, "�٨S������"+pnoun(2, me)+"�I\n");

	data = Pdata[me];

	// �ѧ��i���P
	if( data[TAILTILE] )
	{
		tile = Ctile[<2..<1];
		Ctile = Ctile[0..<3];
		tell(me, pnoun(2, me)+"�ѵP���N��F�@�i"+full_chinese(tile)+"�C\n", 0);
		broadcast(environment(), data[IDNAME]+"�ѵP������N�F�@�i�P�C\n", ENVMSG, all_inventory(environment(this_object()))-Psort+({me})-MJdata[ONLOOKER]);
	}
	else
	{
		tile = Ctile[0..1];
		Ctile = Ctile[2..];
		tell(me, pnoun(2, me)+"�N��F�@�i"+full_chinese(tile)+"�C\n", 0);
		broadcast(environment(), data[IDNAME]+"����N�F�@�i�P�C\n", ENVMSG, all_inventory(environment(this_object()))-Psort+({me}));
	}

	data[TILE] += ({ tile });

	auto_function_check(me, tile);

	check_flower(ref data, tile);

	tile = data[TILE][<1];

	data[TILE] = tile_sort(data[TILE]);

	foreach( object ppl in Psort )
	{
		if( ppl == me )
		{
			show_tile(me);
			continue;
		}

		if( Pdata[ppl][LOOKALL] )
			show_all(ppl);
		else if( !Pdata[ppl][LOOKSEA] )
			show_dumping(ppl);
	}

	MJdata[MO] = 0;

	if( MJdata[FINISH] ) return;

	// �ۺN
	if( valid_hu_check(data[TILE], data[OTILE], data[FLOWER]) )
	{
		beep();
		msg("$ME�ܾ������j�s�@�n�G"HIY"�y�ۺN�z�աI�z����������\n"NOR, me, 0, 1);
		MJdata[HUMYSELF] = 1;
		MJdata[KEYTILE] = tile;
		win_process(me, 0);

		if( sizeof(Psort) < 4 )
			set_temp("status", HIY"��"+NOR YEL+CHINESE_D->chinese_number(4-sizeof(Psort))+HIY"�}"NOR, this_object());
		MJdata[FINISH] = time()+20;
		return;
	}

	data[TAILTILE] = 0;

	// �y��
	if( strlen(Ctile)/2 <= (MJdata[TILELIMIT]+(4-sizeof(Psort))*18) )
	{
		int AIs;

		beep();
		broadcast(environment(), HIG"�P�l�u�ѤU "+(strlen(Ctile)/2)+" �i�A�����ŧi�y���A�Ы� next �~��U�@���C\n"NOR);

		if( sizeof(Psort) < 4 )
			set_temp("status", HIY"��"+NOR YEL+CHINESE_D->chinese_number(4-sizeof(Psort))+HIY"�}"NOR, this_object());

		MJdata[FINISH] = time()+5;

		foreach( object ppl in Psort )
		{
			if( !score[Pdata[ppl][ID]] )
				score[Pdata[ppl][ID]] = allocate_mapping(0);
			if( Pdata[ppl][AI] ) AIs++;
			score[Pdata[ppl][ID]][ROUNDS]++;
		}

		if( AIs == 4 )
			call_out((:next_MJ:), 10);
		return;
	}

	// ť�P�N�P�᪺�b�P�P���P����
	if( sizeof(Pdata[me][LISTEN]) )
	{
		int i;

		if( !Pdata[me][AI] )
		{
			// �ۤv�P�w�O�_�t�b
			i = member_array(tile, data[TILE]);
			if( i != -1 && i < sizeof(data[TILE]) - 3 && data[TILE][i] == data[TILE][i+3] )
			{
				Pdata[me][AUTOOFF] = 1;
				return tell(me, "�ШM�w�O�_�n�t�b�Agon [�P�W]�C\n");
			}
			i = member_array(tile, data[OTILE]);
			if( i != -1 && i < sizeof(data[OTILE]) - 2 && data[OTILE][i] == data[OTILE][i+2] )
			{
				Pdata[me][AUTOOFF] = 1;
				return tell(me, "�ШM�w�O�_�n�b�P�Agon [�P�W]�C\n");
			}
		}

		call_out((:do_da, me, tile:), 2);
		return;
	}

	if( Pdata[me][AI] )
	{
		// AI �t�b
		int i = member_array(tile, data[TILE]);
		
		me->emotion_mo();
		
		if( i != -1 && i < sizeof(data[TILE]) - 3 && data[TILE][i] == data[TILE][i+3] )
			call_out( (:do_gon, me, tile:), 2);
		else
			call_out((:do_da, me, call_other(me, "AI_da"):), 2);
	}

	foreach( object ppl in MJdata[ONLOOKER] )
	{
		if( !objectp(ppl) )
		{
			MJdata[ONLOOKER] -= ({ ppl });
			continue;
		}
		show_all(ppl);
	}

}

void do_au(object me, string arg)
{
	int i;
	string str, arg1, arg2, *args;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	if( MJdata[FINISH] )
		return tell(me, "�w�����@��P���A�Y�n�~��i��U�@��P���Ы� next�C\n");

	if( !arg )
	{
		if( !sizeof(Pdata[me][AUTOPON]+Pdata[me][AUTOGON]+Pdata[me][AUTOHU]) )
			return tell(me, pnoun(2, me)+"�ثe�S���]�w����۰ʥ\��C\n");

		if( sizeof(Pdata[me][AUTOPON]) )
		{
			str = "";
			foreach( string tile in Pdata[me][AUTOPON] )
			str += full_chinese(tile)+"�B";

			tell(me, "�۰ʸI���P���G"+str[0..<3]+"�C\n");
		}
		if( sizeof(Pdata[me][AUTOGON]) )
		{
			str = "";
			foreach( string tile in Pdata[me][AUTOGON] )
			str += full_chinese(tile)+"�B";
			tell(me, "�۰ʺb���P���G"+str[0..<3]+"�C\n");
		}
		if( sizeof(Pdata[me][AUTOHU]) )
		{
			str = "";
			foreach( string tile in Pdata[me][AUTOHU] )
			str += full_chinese(tile)+"�B";
			tell(me, "�۰ʭJ���P���G"+str[0..<3]+"�C\n");
		}
		return;
	}

	if( arg == "tin" || arg == "t" )
	{
		tell(me, "�]�w�۰�ť�P�����C\n");
		Pdata[me][PLISTEN] = 1;
		return;
	}
	if( sscanf(arg, "%s %s", arg1, arg2) != 2 )
		return tell(me, pnoun(2, me)+"�Q�n�N����ʧ@�۰ʤơH\n");

	switch( arg1 )
	{
	case "h":
	case "hu":
		args = explode(arg2, " ");
		foreach( arg in args )
		{
			if( member_array(arg, Tsort) == -1 )
				return tell(me, "�S�� "+arg+" �o�i�P�C\n");

			if( member_array(arg, Pdata[me][AUTOHU]) != -1 )
			{
				tell(me, "�����۰ʭJ"+full_chinese(arg)+"�C\n");
				Pdata[me][AUTOHU] -= ({ arg });
			}
			else if( !valid_hu_check(tile_sort(Pdata[me][TILE]+({ arg })), 0, 0) )
				return tell(me, pnoun(2, me)+"�S����k�a"+full_chinese(arg)+"�J�P�C\n");
			else
			{
				auto_function_check(me, arg);
				tell(me, "�]�w�۰ʭJ"+full_chinese(arg)+"�����C\n");
				Pdata[me][AUTOHU] += ({ arg });
			}
		}
		break;
	case "p":
	case "pon":

		if( member_array(arg2, Tsort) == -1 )
			return tell(me, "�S�� "+arg2+" �o�صP�C\n");

		i = member_array(arg2, Pdata[me][TILE]);
		if( i == -1 ) return tell(me, pnoun(2, me)+"��W�S��"+full_chinese(arg2)+"�C\n");

		if( i < sizeof(Pdata[me][TILE]) - 1 && Pdata[me][TILE][i+1] == arg2 )
		{
			if( member_array(arg2, Pdata[me][AUTOPON]) != -1 )
			{
				Pdata[me][AUTOPON] -= ({ arg2 });
				return tell(me, pnoun(2, me)+"�����۰ʸI"+full_chinese(arg2)+"�C\n");
			}

			Pdata[me][AUTOPON] += ({ arg2 });
			Pdata[me][AUTOGON] -= ({ arg2 });

			tell(me, "�]�w�۰ʸI"+full_chinese(arg2)+"�����C\n", 0);
		}
		else
			tell(me, pnoun(2, me)+"��W��"+full_chinese(arg2)+"�����I�աC\n");
		break;
	case "g":
	case "gon":
		if( member_array(arg2, Tsort) == -1 )
			return tell(me, "�S�� "+arg2+" �o�صP�C\n");

		i = member_array(arg2, Pdata[me][TILE]);
		if( i == -1 ) return tell(me, pnoun(2, me)+"��W�S��"+full_chinese(arg2)+"�C\n");

		if( i < sizeof(Pdata[me][TILE]) - 2 && Pdata[me][TILE][i+2] == arg2 )
		{
			if( member_array(arg2, Pdata[me][AUTOGON]) != -1 )
			{
				Pdata[me][AUTOGON] -= ({ arg2 });
				return tell(me, pnoun(2, me)+"�����۰ʺb"+full_chinese(arg2)+"�C\n");
			}

			Pdata[me][AUTOGON] += ({ arg2 });
			Pdata[me][AUTOPON] -= ({ arg2 });
			tell(me, "�]�w�۰ʺb"+full_chinese(arg2)+"�����C\n", 0);
		}
		else
			tell(me, pnoun(2, me)+"��W��"+full_chinese(arg2)+"�����b�աC\n");
		break;
	default:
		return tell(me, "���O�榡�Gau [hu|pon|gon]\n");
	}
}

void do_tin(object me, string arg)
{
	int i;
	string str = "";
	string *listening, *mytiles;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	if( MJdata[FINISH] )
		return tell(me, "�w�����@��P���A�Y�n�~��i��U�@��P���Ы� next�C\n");

	if( sizeof(Pdata[me][LISTEN]) )
	{
		foreach( string tile in Pdata[me][LISTEN] )
		str += full_chinese(tile)+"�B";
		tell(me, "�ثe��ť���P���G"+str[0..<3]+"�C\n");
		return;
	}

	if( Psort[MJdata[TURN]] == me && !MJdata[MO] )
		return tell(me, "�{�b����"+pnoun(2, me)+"���P�A�����ť�P�C\n");

	mytiles = Pdata[me][TILE];

	listening = allocate(0);

	// �۰ʤ��R�nť���X�i�P
	foreach( string tile in Tsort )
	{
		i = member_array(tile, mytiles);

		// ���L��W�w�g�����b�P
		if( i != -1 && i < sizeof(mytiles)-3 && mytiles[i] == mytiles[i+3] ) continue;

		if( valid_hu_check(tile_sort(mytiles+({ tile })), 0, 0) )
			listening += ({ tile });
	}

	if( sizeof(listening) )
	{
		beep();

		msg("$ME�u�K�B�K�B�K�v�a�l���F�X�n�A�N�N�a���G"HIG"��ť�P�F��C\n"NOR, me, 0, 1, 0, all_inventory(environment(this_object()))-Psort-MJdata[ONLOOKER]);

		if( Psort[MASTER] == me && sizeof(MJdata[DUMPED]) <= 1 )
		{
			msg(HIG"\n$ME"HIG"�ثe����ť���A�C\n\n"NOR, me, 0, 1);
			Pdata[me][LISTENTYPE] = 1;
		}

		else if( sizeof(MJdata[DUMPED]) <= 8 )
		{
			i = 0;
			foreach( object ppl in Psort )
			if( sizeof(Pdata[ppl][OTILE]) )
				i = 1;
			if( i==0 )
			{
				msg(HIG"\n$ME"HIG"�ثe���ať���A�C\n\n"NOR, me, 0, 1);
				Pdata[me][LISTENTYPE] = 2;
			}
		}

		Pdata[me][AUTOPON] = allocate(0);
		Pdata[me][AUTOGON] = allocate(0);
		Pdata[me][AUTOHU]  = allocate(0);
		Pdata[me][LISTEN]  = listening;

		show_all_tile();

		MJdata[LISTENING] = 1;

		if( Psort[MJdata[TURN]] == me )
			do_mo(me, 0);
	}
	else
		return tell(me, pnoun(2, me)+"�o�صP�����ť�P�աI\n");
}

void show_all(object me)
{
	int i=1;
	string arr, tile, str="";

	clear_screen(me);

	foreach( object ppl in Psort )
	{
		otile_display(ref arr, Pdata[ppl], 0);

		arr[0] += HIW"��";
		arr[1] += HIW"��";

		foreach( tile in Pdata[ppl][TILE] )
		{
			arr[0] += NOR+MJtile[tile][COLOR1]+MJtile[tile][CHINESE][0..1]+HIW"��";
			arr[1] += NOR+MJtile[tile][COLOR2]+MJtile[tile][CHINESE][2..3]+HIW"��";
		}

		str = "";
		str += sizeof(Pdata[ppl][FLOWER])?NOR WHT"�z"+repeat_string("�w�s", sizeof(Pdata[ppl][FLOWER])-1)+"�w�{"NOR:"";
		str += sizeof(Pdata[ppl][OTILE])?NOR WHT"�z"+repeat_string("�w�s", sizeof(Pdata[ppl][OTILE])-1)+"�w�{"NOR:"";
		str += HIW"�z"+repeat_string("�w�s", sizeof(Pdata[ppl][TILE])-1)+"�w�{"NOR;
		spec_line_msg(me, str, i++);
		spec_line_msg(me, arr[0]+" "+NOR+(ppl==Psort[0]?HIG"��"+(MJdata[CMASTER]?"�s"+CHINESE_D->chinese_number(MJdata[CMASTER]):"")+" "NOR:"")+(sizeof(Pdata[ppl][LISTEN])?HIC"ť("+implode(Pdata[ppl][LISTEN],",")+") "NOR:"")+NOR, i++);
		spec_line_msg(me, arr[1]+NOR+(member_array(ppl, Psort)==MJdata[TURN]?HBCYN" "NOR HIREV HIC+remove_ansi(Pdata[ppl][IDNAME])+NOR HBCYN" ":" "+Pdata[ppl][IDNAME]+" ")+NOR" ", i++);

		str = replace_string(str, "�z","�|");
		str = replace_string(str, "�{","�}");
		str = replace_string(str, "�s","�r");

		spec_line_msg(me, str, i++);
	}
}

void do_lookall(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	if( !sizeof(Pdata[me][LISTEN]) && !MJdata[FINISH] )
		return tell(me, "ť�P��~�i�H�ݩҦ��H���P�C\n");

	Pdata[me][LOOKALL] = 1;
	Pdata[me][LOOKSEA] = 0;

	show_all(me);
}

/* �S��@�����O�Ѳ��q */
void do_skyeye(object me, string arg)
{
	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	if( !wizardp(me) )
		return tell(me, "�����O���Ův�M�Ϋ��O�C\n");

	Pdata[me][LOOKALL] = 1;
	Pdata[me][LOOKSEA] = 0;

	show_all(me);
}


void do_lookmj(object me, string arg)
{
	if( is_player(me) )
		return tell(me, pnoun(2, me)+"�O�䤤�@�쪱�a�A�L�k�ݧO�H���P�C\n");

	if( !MJdata[PLAYING] )
		return tell(me, "�P���٨S�}�l�C\n");

	if( member_array(me, MJdata[ONLOOKER]) == -1 )
	{
		MJdata[ONLOOKER] += ({ me });
		startup_title_screen(me, TITLE_SCREEN_LENGTH);

		if( !MJdata[FINISH] )
			show_all(me);

		me->add_status(OSTATUS);
		msg("$ME�}�l�b�±N����[���C\n", me, 0, 1);
		return;
	}

	switch(arg)
	{
	case 0:
	case "cancel":
		reset_screen(me);
		me->remove_status(OSTATUS);
		MJdata[ONLOOKER] -= ({ me });
		msg("$ME�����F�ǤH�[���C\n", me, 0, 1);
		break;
	case "sea":show_sea(me);break;
	case "all":show_all(me);break;
	default:show_all(me);break;
	}
}

// ��H���ɦ�l
void do_substitute(object me, mixed arg)
{
	object target;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q��ִ��ɡH\n");

	if( objectp(arg) )
		target = arg;
	else if( !objectp(target = find_player(arg) || present(arg, environment()) ) )
		return tell(me, "�o�̨S�� "+arg+" �o�ӤH�C\n");

	if( is_player(target) )
		return tell(me, pnoun(3, target)+"�w�g�[�J�P���F�A�L�k����"+pnoun(2, me)+"�C\n");

	startup_title_screen(target, TITLE_SCREEN_LENGTH);
	
	target->add_status(STATUS);
	set_temp("mjobject", this_object(), target);
	
	Psort[member_array(me, Psort)] = target;

	Pdata[target] = copy(Pdata[me]);
	Pdata[target][PLAYER]	= target;
	Pdata[target][ID]	= target->query_id(1);
	Pdata[target][IDNAME]	= target->query_idname();

	if( !userp(target) )
		Pdata[target][AI] = 1;
	else
		Pdata[target][AI] = 0;

	map_delete(Pdata, me);

	me->remove_status(STATUS);
	reset_screen(me);

	MJdata[ONLOOKER] -= ({ target });

	beep();

	if( !MJdata[PLAYING] )
		show_title();
	else if( !MJdata[FINISH] )
		show_all_tile();

	msg("$ME���}�±N��A�@��N$YOU�ԤF�L�ӡA�ñj��$YOU����±N��e�û��D�G"HIG"�浹"+pnoun(2, target)+"�F�I\n"NOR, me, target, 1);

	if( !userp(me) )
		destruct(me);
}

// �״_�l�a���P��
void do_fixmj(object me, string arg)
{
	mapping tempdata;
	object AIob;

	if( !is_player(me) )
		return tell(me, pnoun(2, me)+"�èS���[�J�P���C\n");

	foreach( object ppl, mapping data in Pdata )
	{
		if( objectp(ppl) ) continue;
		
		foreach(data in values(Pdata))
			if( !objectp(data[PLAYER]) )
				tempdata = copy(data);

		AIob = new(MJAI_FILE);

		Pdata[AIob] = tempdata;
		Pdata[AIob][PLAYER]	= AIob;
		Pdata[AIob][ID]		= AIob->query_id(1);
		Pdata[AIob][IDNAME]	= AIob->query_idname();
		Pdata[AIob][AI] 	= 1;
		
		map_delete(Pdata, ppl);

		Psort[member_array(ppl, Psort)] = AIob;

		AIob->move(environment());

		msg("$ME��M�Ĩ�±N��e�û��D�G"HIG"�ڨӱϤj�a�F�I\n"NOR, AIob, 0, 1);
		
		set_temp("mjobject", AIob);
		do_mo(AIob, arg);
	}
}

void heart_beat()
{
	int player;
	
	if( sizeof(Pdata) )
	{
		foreach( object ppl in keys(Pdata) )
			if( objectp(ppl) && userp(ppl) )	
				player++;
		if( !player )
		{
			foreach(object ppl in keys(Pdata))
				destruct(ppl);
			
			reset_all_data();
		}
	}
}
	
	
void create()
{
	if( file_size(DATA_PATH) > 0 )
		restore_object(DATA_PATH);

	set_idname("mj table", HIY"�O"NOR YEL"�W�Q���i�±N��"NOR);
	set("long", HIY"�O"NOR YEL"�W�Q���i�±N��Ahelp mj �ݫ��O�C��"NOR);
	set("unit", "��");
	set("mass", -1);
	set("flag/no_amount", 1);
	
	save_object(DATA_PATH);

	reset_all_data();
	
	set_heart_beat(3000);
}

void delete_data(string id)
{
	restore_object(DATA_PATH);
	map_delete(score, id);
	save_object(DATA_PATH);
}

int remove()
{
	foreach( object ppl in Psort )
	{
		if( !objectp(ppl) ) continue;

		ppl->remove_status(STATUS);
		reset_screen(ppl);

		if( !userp(ppl) )
			destruct(ppl);
	}

	foreach( object onlooker in MJdata[ONLOOKER] )
	{
		if( !onlooker ) continue;

		onlooker->remove_status(OSTATUS);
		// fixed by sinji on 2003/6/25 - *Bad argument 2 to delete_temp() Expected: object Got: 0.
		// ���ˬd����O�_�s�b
		reset_screen(onlooker);
	}
}
