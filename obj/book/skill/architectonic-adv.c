/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : architectonic-adv.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-23
 * Note   : �����ؿv�ޯ��
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <skill.h>

#define BOOK_SKILL 	"architectonic-adv"
#define BASE_SKILL	([ "architectonic" : 100 ])

inherit __DIR__"_skill_book.c";

void create()
{
	set("skill", BOOK_SKILL);
	set("baseskill", BASE_SKILL);
	
	::create();
}
