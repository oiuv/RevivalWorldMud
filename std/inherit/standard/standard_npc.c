/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : standard_npc.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-11-5
 * Note   : �з� NPC �~����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>

int is_npc()
{
	return 1;
}

/* ��� */
inherit DATABASE;

/* �M�� */
inherit CLEAN_UP;

/* �ͪ� */
inherit LIVING_LIV;

/* ���A */
inherit CONDITION_LIV;

/* ���� */
inherit DELAY_LIV;

/* ���� */
inherit MOVE_LIV;

/* �����B�z */
inherit MONEY_LIV;

/* �԰� */
inherit COMBAT_LIV;

/* �ޯ� */
inherit SKILL_LIV;

/* �ݩ� */
inherit ATTRIBUTION_LIV;

/* �˳� */
inherit EQUIPMENT_LIV;

/* �W��ID */
inherit IDNAME_NPC;

/* ���O */
inherit COMMAND_NPC;

/* �ʧ@ */
inherit ACTION_NPC;

/* �߸� */
inherit HEART_BEAT_NPC;

/* �T�� */
inherit MESSAGE_NPC;

/* �T���欰 */
inherit BEHAVIOR_MESSAGE_NPC;

/* ���ʦ欰 */
inherit BEHAVIOR_WALKING_NPC;
