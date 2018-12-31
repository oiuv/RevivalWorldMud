/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : quest_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : ���Ⱥ��F
 * Update :
 *
 -----------------------------------------
 */
 
#include <quest.h>

#define QUEST_NAME		0
#define QUEST_STEP		1
#define QUEST_OPEN		2

mapping quests = ([
	QUEST_OLD_FARMER 		: ({ QUEST_OLD_FARMER_NAME, QUEST_OLD_FARMER_STEP, 1 }),
	QUEST_YIN_LING_YU 		: ({ QUEST_YIN_LING_YU_NAME, QUEST_YIN_LING_YU_STEP, 1 }),
	QUEST_YIN_LING_YU_2 		: ({ QUEST_YIN_LING_YU_2_NAME, QUEST_YIN_LING_YU_2_STEP, 1 }),
	QUEST_CITY_WALKER		: ({ QUEST_CITY_WALKER_NAME, QUEST_CITY_WALKER_STEP, 1 }),
	QUEST_BOHR			: ({ QUEST_BOHR_NAME, QUEST_BOHR_STEP, 1 }),
	QUEST_EPIC_FARMING		: ({ QUEST_EPIC_FARMING_NAME, QUEST_EPIC_FARMING_STEP, 0 }),
	QUEST_EPIC_FISHFARM		: ({ QUEST_EPIC_FISHFARM_NAME, QUEST_EPIC_FISHFARM_STEP, 0 }),
	QUEST_EPIC_PASTURE		: ({ QUEST_EPIC_PASTURE_NAME, QUEST_EPIC_PASTURE_STEP, 0 }),
]);

int query_quest_steps(string quest)
{
	if( !undefinedp(quests[quest]) )
		return quests[quest][QUEST_STEP];

	return -1;
}

int query_quest_name(string quest)
{
	if( !undefinedp(quests[quest]) )
		return quests[quest][QUEST_NAME];
		
	return -1;
}

int query_total_quests_amount()
{
	return sizeof(quests);
}

int query_total_opened_quests_amount()
{
	return sizeof(filter(quests, (: $2[QUEST_OPEN] :)));
}
string query_name()
{
	return "���Ȩt��(QUEST_D)";
}
