/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : trading_post.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-4-13
 * Note   : �ۥѶR��Ұʧ@�~�Ӫ���
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit ROOM_ACTION_MOD;

inherit __DIR__"inherit/_object_database_mod.c";
inherit __DIR__"inherit/_action_list";
inherit __DIR__"inherit/_action_buy";
inherit __DIR__"inherit/_action_sell";

#define TRADING_POST_CAPACITY	-1

nosave mapping allowable_trade = 
([

	"�A�@":
		({
			"/obj/farming/seed/rice_seedling", "-1",
			"/obj/farming/seed/herbage_seed", "-1",
			"/obj/farming/seed/barley_seed", "-1",
			"/obj/farming/seed/corn_seed", "-1",
			"/obj/farming/seed/rose_seed", "-1",
			"/obj/farming/seed/sunflower_seed", "-1",
			"/obj/farming/seed/lily_seed", "-1",
		}),
	"����":
		({
			"/obj/pasture/breed/chicken_stud", "-1",
			"/obj/pasture/breed/pig_stud", "-1",
			"/obj/pasture/breed/sheep_stud", "-1",
			"/obj/pasture/breed/horse_stud", "-1",
			"/obj/pasture/breed/cattle_stud", "-1",
			"/obj/pasture/breed/deer_stud", "-1",
		}),
	"�i��":
		({
			"/obj/fishfarm/breed/clam", "-1",
			"/obj/fishfarm/breed/young_crab", "-1",
			"/obj/fishfarm/breed/young_shrimp", "-1",
		}),
	"����":
		({
			"/obj/fishing/rod/toy_rod", "-1",
			"/obj/fishing/rod/bamboo_rod", "-1",
			"/obj/fishing/rod/wood_rod", "-1",
			"/obj/fishing/rod/plastic_rod", "-1",
			"/obj/fishing/rod/carbon_fiber_rod", "-1",
			"/obj/fishing/rod/microelectronic_rod", "-1",
			"/obj/fishing/bait/bread_bug", "-1",
			"/obj/fishing/bait/prawn", "-1",
			"/obj/fishing/bait/tsou_chip", "-1",
			"/obj/fishing/bait/worm", "-1",
		}),
	"�Ķ�":
		({
			"/obj/collection/tool/coarse_pickax", "-1",
			"/obj/collection/tool/coarse_drill", "-1",
			"/obj/collection/tool/coarse_bucket", "-1",
			"/obj/collection/tool/coarse_saw", "-1",
			"/obj/collection/tool/coarse_shovel", "-1",
			
		}),
	"��L":
		({
                        "/obj/etc/newspaper", "-1",
                        "/obj/etc/firework", "-1",
                        "/obj/systemproducts/flatbed_tricycle", "-1",
		}),

]);

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"lobby"	:
	([
		"short"	: HIW"�R��j�U"NOR,
		"help"	:
			([
"topics":
@HELP
    �R��j�U�O���a�ʶR�P�c��ӫ~���a��A�ѩ�O�ѬF���}�]�A�ȷ|�H��������V���a���ʰӫ~�C
HELP,

"list":
@HELP
�C�X�Ҧ��c�椤���ӫ~�����O�A�Ϊk�p�U�G
  list			- �C�X�Ҧ��c�椤���ӫ~
HELP,

"buy":
@HELP
�ʶR�ӫ~�����O�A�Ϊk�p�U�G
  buy all rice			- �R�U�Ҧ��� rice �ӫ~
  buy 3 rice			- �R�U 3 �� rice �ӫ~
  buy rice			- �R�U 1 �� rice �ӫ~
  buy 3 2			- �R�U 3 ��s���� 2 ���ӫ~
  buy 2				- �R�U 1 ��s���� 2 ���ӫ~
  buy rice to center 1 51,22	- �R�U 1 �� rice �ӫ~�ðe�F center ������ 1 �ìP�����y��(51,22)�B���ؿv��
HELP,

"sell":
@HELP
�c��ӫ~�����O�A�Ϊk�p�U�G
  sell all		- ��X�Ҧ����W�i�c�檺�ӫ~
  sell all rice		- ��X�Ҧ��� rice �ӫ~
  sell 3 rice		- ��X 3 �� rice �ӫ~
HELP,
			]),
		"action":
			([
				//"sell"		: (: do_trading_post_sell($1, $2, 50) :),
				"list"		: (: do_list($1, $2, "products", TRADING_POST_CAPACITY) :),
				"buy"		: (: do_buy($1, $2, "products", "GOVERNMENT") :),
			]),
	]),
]);


// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	NOR YEL"���y����"NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION

	// �}�i�����O��
	,"5000000"
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,10
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});
