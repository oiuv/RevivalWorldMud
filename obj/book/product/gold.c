/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : gold.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-15
 * Note   :
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

inherit __DIR__"_product_plan.c";

void create()
{
	set("product", "gold");
	
	::create();
}
