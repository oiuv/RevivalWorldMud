/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : product_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-05-03
 * Note   : ���~���F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <daemon.h>
#include <ansi.h>
#include <terrain.h>

string *world_product =
({
	"/obj/materials/metal",
	"/obj/materials/stone",
	"/obj/materials/water",
	"/obj/materials/wood",
	"/obj/materials/fuel",

	"/obj/fishing/rod/toy_rod",
	"/obj/fishing/rod/bamboo_rod",
	"/obj/fishing/rod/wood_rod",
	"/obj/fishing/rod/plastic_rod",
	"/obj/fishing/rod/carbon_fiber_rod",
	"/obj/fishing/rod/microelectronic_rod",

	"/obj/fishing/bait/bread_bug",
	"/obj/fishing/bait/prawn",
	"/obj/fishing/bait/tsou_chip",
	"/obj/fishing/bait/worm",

	"/obj/fishing/fish/fish0",
	"/obj/fishing/fish/fish1",
	"/obj/fishing/fish/fish2",
	"/obj/fishing/fish/fish3",
	"/obj/fishing/fish/fish4",
	"/obj/fishing/fish/fish5",
	"/obj/fishing/fish/fish6",
	"/obj/fishing/fish/fish7",
	"/obj/fishing/fish/fish8",
	
	"/obj/fishing/adv_fish/fish0",
	"/obj/fishing/adv_fish/fish1",
	"/obj/fishing/adv_fish/fish2",
	"/obj/fishing/adv_fish/fish3",

	"/obj/fishing/sp_fish/fish0",
	"/obj/fishing/sp_fish/fish1",
	"/obj/fishing/sp_fish/fish2",

	"/obj/fishing/other/box",
	
	"/obj/collection/tool/coarse_pickax",
	"/obj/collection/tool/coarse_drill",
	"/obj/collection/tool/coarse_bucket",
	"/obj/collection/tool/coarse_saw",
	"/obj/collection/tool/coarse_shovel",

	"/obj/etc/newspaper",
	
	"/obj/gem/gem0",
	"/obj/gem/gem1",
	"/obj/gem/gem2",
	"/obj/gem/gem3",
	"/obj/gem/gem4",
	"/obj/gem/gem5",
	"/obj/gem/gem6",
	
	"/obj/farming/seed/barley_seed",
	"/obj/farming/seed/corn_seed",
	"/obj/farming/seed/herbage_seed",
	"/obj/farming/seed/lily_seed",
	"/obj/farming/seed/rice_seedling",
	"/obj/farming/seed/rose_seed",
	"/obj/farming/seed/sunflower_seed",

	"/obj/farming/product/barley",
	"/obj/farming/product/corn",
	"/obj/farming/product/herbage",
	"/obj/farming/product/lily",
	"/obj/farming/product/rice",
	"/obj/farming/product/rose",
	"/obj/farming/product/sunflower",

	"/obj/pasture/breed/cattle_stud",
	"/obj/pasture/breed/chicken_stud",
	"/obj/pasture/breed/deer_stud",
	"/obj/pasture/breed/horse_stud",
	"/obj/pasture/breed/pig_stud",
	"/obj/pasture/breed/sheep_stud",

	"/obj/pasture/product/antler",
	"/obj/pasture/product/beef",
	"/obj/pasture/product/chicken",
	"/obj/pasture/product/egg",
	"/obj/pasture/product/horseflesh",
	"/obj/pasture/product/mutton",
	"/obj/pasture/product/pork",
	"/obj/pasture/product/venison",
	"/obj/pasture/product/wool",
	
	"/obj/fishfarm/breed/clam",
	"/obj/fishfarm/breed/young_crab",
	"/obj/fishfarm/breed/young_shrimp",

	"/obj/fishfarm/product/pearl",
	"/obj/fishfarm/product/big_crab",
	"/obj/fishfarm/product/big_shrimp",
	
	"/obj/systemproducts/flatbed_tricycle",
	
});

string *query_world_product()
{
	return world_product;
}

void create()
{


}
string query_name()
{
	return "���~�t��(PRODUCT_D)";
}
