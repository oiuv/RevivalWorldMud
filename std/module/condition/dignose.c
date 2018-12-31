/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : dignose.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-05-16
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>

#define CONDITION_ID		"dignose"
#define CONDITION_NAME		NOR WHT"�����"NOR
#define CONDITION_TYPE		"�ˮ`"
#define DEFAULT_TIME		60
#define DEFAULT_HEART_BEAT	5

// �^�Ǫ��A�^��W��
string query_condition_id()
{
	return CONDITION_ID;
}

// �^�Ǫ��A����W��
string query_condition_name()
{
	return CONDITION_NAME;
}

// �^�Ǫ��A���A�W��
string query_condition_type()
{
	return CONDITION_TYPE;
}

// �^�Ǫ��A�w�]����ɶ�
int query_default_condition_time()
{
	return DEFAULT_TIME;
}

// �^�Ǫ��A�w�]�߸��ɶ�
int query_default_condition_heart_beat()
{
	return DEFAULT_HEART_BEAT;
}

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	msg("$ME����u"CONDITION_NAME"�v��"CONDITION_TYPE"�v�T�C\n", ob, 0, 1);
}

// �������A�ɪ��ĪG
void stop_effect(object ob)
{
	msg("$ME�Ѱ��u"CONDITION_NAME"�v��"CONDITION_TYPE"�v�T�C\n", ob, 0, 1);
}

// ���A�i�椤���ĪG
void heart_beat_effect(object ob)
{
	if( !ob->cost_health(20) )
		ob->faint();

	msg("$ME����u"CONDITION_NAME"�v���I�k�ˮ`�A�l�� 20 �I���ͩR��("NOR RED+ob->query_health_cur()+"/"HIR+ob->query_health_max()+NOR")�C\n", ob, 0, 1);	
}