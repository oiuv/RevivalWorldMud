/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : skill_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-13
 * Note   : �ޯ���F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <skill.h>

string *skills;
string *sorted_skills;

#define EXP_SHIFT		7700

string *manual_skill_sort =
({
	HIG"�ݩʯ�O"NOR GRN"���i"NOR,
	"strength",
	"strength-adv",
	"physique",
	"physique-adv",
	"intelligence",
	"intelligence-adv",
	"agility",
	"agility-adv",
	"charisma",
	"charisma-adv",
	"stamina",
	"health",
	"energy",
	"feature",

	HIG"��X�ޥ�"NOR GRN"�V�m"NOR,
	"technology",
	"leadership",
	"leadership-adv",
	"architectonic",
	"architectonic-adv",
	"exchange",
	"price",
	"estaterebate",
	"sport",
	"consciousness",
	"teach",
	"eloquence",
	"idle",
	
	HIG"�򥻭��"NOR GRN"�Ͳ�"NOR,
	"metal",
	"stone",
	"water",
	"wood",
	"fuel",

	HIG"�S����"NOR GRN"�Ͳ�"NOR,
	"fishing",
	"farming",
	"pasture",
	"fishfarm",
	
	
	HIG"�@���u�t"NOR GRN"�Ͳ�"NOR,
	"metalclassify",
	"stoneclassify",
	"waterclassify",
	"woodclassify",
	"fuelclassify",

	HIG"�G���u�t"NOR GRN"�Ͳ�"NOR,
	"food-fac",
	"drink-fac",
	"clothing-fac",
	"instrument-fac",
	"furniture-fac",
	"hardware-fac",
	"chemical-fac",
	"machinery-fac",
	"electrics-fac",
	"transportation-fac",
	"entertainment-fac",
	"medicine-fac",
	"adventure-fac",
	"shortrange-fac",
	"longrange-fac",
	"armor-fac",
	"heavyarmor-fac",
	"magic-fac",
	
	
	HIY"���u�u�@"NOR YEL"�ޯ�"NOR,
	"storemanage",
	"factorymanage",
	"researchmanage",
	"security",

});

int query_level_exp(int lv)
{
	if( lv < 1 ) return 0;
	
	// 0 - 100 �Ť@�غ�k
	if( lv <= 100 )
	{
		lv = 100 - lv;
		return to_int((10000000 + EXP_SHIFT)/pow(1+(((lv+39)/10)/250.), lv/0.7) - EXP_SHIFT);
	}
	// 101 - 200 �t�@�غ�k
	else if( lv <= 200 )
	{
		lv = 200 - lv;
		return to_int((1000000000 + EXP_SHIFT)/pow(1+(((lv+39)/10)/250.), lv/0.7) - EXP_SHIFT)+query_level_exp(100);
	}
	else
		return 0;
}


string *query_skills()
{
	return skills;
}

string *query_sorted_skills()
{
	return sorted_skills;
}

int skill_exists(string skill)
{
	return member_array(skill, skills) == -1 ? 0 : 1;
}

void create()
{	
	skills = map(get_dir(SKILL_ROOT), (: $1 = $1[0..<3] :));
	sorted_skills = manual_skill_sort | skills;//sort_array(skills, (: member_array($1, manual_skill_sort) - member_array($2, manual_skill_sort) :));
}

string query_name()
{
	return "�ޯ�t��(SKILL_D)";
}
