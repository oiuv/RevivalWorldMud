/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chicken_stud.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : 種雞
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <nature.h>
#include <map.h>

inherit STANDARD_OBJECT;

void create()
{
        set_idname("chicken stud","種雞");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "用來交配生產的種畜。\n");
        set("unit", "隻");
        set("mass", 1000);
        set("value", 500);
        set("badsell", 1);

	// 種子資訊
	set("growth",
	([
		//種類
		"type":		PASTURE,

		//名稱
		"idname":	this_object()->query_idname(),
		
		//開始生產時的訊息
		"start_msg":"雞群們咕咕咕地叫著，各自吃著面前的飼料。\n" ,
			
	       	//每單位需要的原料種類與數量
	       	"material":
	       	([
	       		"/obj/materials/water":5,
	       		"/obj/farming/product/rice":5,
	       	]),
			
	       	//產品的數量設定與產品路徑
	       	"harvest":
	       	([
	       		"/obj/pasture/product/egg":50,
	       		"/obj/pasture/product/chicken":20,
	       	]),

		//生成時的訊息
		"harvest_msg":"$ME歡喜地屠宰並取出新鮮的雞肉。\n",

		//種植此類產品所需要的技能與其等級
		"skill":
		([
			"pasture":0,
		]),

		//成長時的訊息
		"growing":
		([
			5:	"進入了濃情蜜意的交配期，種雞之間開始互相交配。\n",
			50:	"種雞們恢復到過去的生活，但可以注意到部份的母雞行為有些不同。\n",
			100:	"部份的母雞生下了幾顆雞蛋，並開始孵起蛋來。\n",
			200:	"小雞們一隻隻地破蛋而出，只見小雞們不斷地吱吱叫，似乎對眼前的新世界充滿著好奇心。\n",
			300:	"小雞逐漸的成長，愈長愈壯。\n",
			400:	"小雞們全部健康的長大成為成雞，可以進行收成了。\n",
		]),
	
		// 天氣影響敘述
		"weather":
		([
			SNOWY:	"大雪連連嚴重使得雞群們陸續的凍死。\n",
			SHOWER:	"連日來的雨量使得部份的雞群們受涼病死。\n",
			TYPHOON:"颱風來襲使得部份的雞群們被強風吹來的木頭砸死。\n",
			FOEHN:	"焚風的吹襲讓部份的雞群們過熱中暑而死。\n",
		]),
	]));
}
