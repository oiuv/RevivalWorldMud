/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _skill_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-04
 * Note   : �ޯ��~����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <skill.h>
#include <daemon.h>

int query_skill_level(string skill)
{
	return query(SKILL_LV(skill));
}

float query_skill_exp(string skill)
{
	return query(SKILL_EXP(skill));
}

int remove_skill(string skill)
{
	if( !query(SKILL_KEY(skill)) ) return 0;
	delete(SKILL_KEY(skill));
	return 1;
}


// �i��ɯ�
int skill_level_up(string skill)
{
	int new_lv;
	object me = this_object();
	
	new_lv = addn(SKILL_LV(skill), 1);

	// ��ܤɯŰT��
	tell(me, sprintf(NOR YEL"%s��"HIY"%s"NOR YEL"���Ŵ��ɬ� "HIY"%d"NOR YEL" �šC\n"NOR,
		pnoun(2, me), (SKILL(skill))->query_idname(), new_lv));
	
	// ����ɯŬ����ʧ@, ���ɯŦA���ͤɯŨƥ�
	(SKILL(skill))->after_level_up(me, new_lv);
	
	// �B�z�ޯ��Y��
	(SKILL(skill))->add_title(me, new_lv);
	
	return new_lv;
}

// �i�歰��
int skill_level_down(string skill)
{
	int new_lv, cur_lv;
	object me = this_object();
	
	cur_lv = query_skill_level(skill);
	
	// ���歰�Ŭ����ʧ@, �����ͭ��Ũƥ�A����
	(SKILL(skill))->before_level_down(me, cur_lv);

	// �B�z�ޯ��Y��
	(SKILL(skill))->remove_title(me, cur_lv);

	new_lv = addn(SKILL_LV(skill), -1);
	
	// ��ܭ��ŰT��
	tell(me, sprintf(NOR YEL"%s��"HIY"%s"NOR YEL"���Űh�B�� "HIY"%d"NOR YEL" �šC\n"NOR,
		pnoun(2, me), (SKILL(skill))->query_idname(), new_lv));

	return new_lv;
}

// �g��ȭp���ˬd�O�_�ɯ�
int exp_calculate(string skill)
{
	object me = this_object();
	int cur_exp = query_skill_exp(skill);
	int cur_lv, max_lv;
	int level_change;

	max_lv = (SKILL(skill))->max_level(this_object());

	// �� while �@�s��ɯŰʧ@
	while(1)
	{
		cur_lv = query_skill_level(skill);

		if( cur_exp < (SKILL(skill))->level_exp(cur_lv + 1) || cur_lv >= max_lv ) break;
			
		skill_level_up(skill);
		
		level_change = 1;
	}

	// �� while �@�s�򭰯Űʧ@	
	while(1)
	{
		cur_lv = query_skill_level(skill);

		if( cur_exp >= (SKILL(skill))->level_exp(cur_lv) || cur_lv <= 0 ) break;
		
		skill_level_down(skill);
		
		level_change = 1;
	}

	if( level_change )
	{
		if( userp(me) && !SECURE_D->is_wizard(me->query_id(1)) )
			TOP_D->update_top("skill/"+skill, me->query_id(1), cur_lv, me->query_idname(), query("city"));
		
		me->save();
	}
}

// �W�[�g���
int add_skill_exp(string skill, mixed exp)
{
	float new_exp, max_exp;
	object skillob = load_object(SKILL(skill));

	// �ˬd�ޯ�O�_�s�b
	if( !objectp(skillob) ) return 0;
	
	exp = to_float(exp);

	// �g��ȭ���
	exp *= skillob->exp_multiple(this_object());

	new_exp = query_skill_exp(skill) + exp;
	max_exp = to_float(skillob->level_exp(skillob->max_level(this_object())));

	if( new_exp < 0 )
		set(SKILL_EXP(skill), 0.);
	else if( new_exp > max_exp )
		set(SKILL_EXP(skill), max_exp);
	else
		set(SKILL_EXP(skill), new_exp);

	// �g��ȼW�[���᪺�ƥ�
	skillob->added_exp(this_object(), exp);
	
	// �p��g����ˬd�O�_�n�ɯ�
	exp_calculate(skill);
}


// �^�ǤU�@���Żݭn�g���
int next_level_exp(string skill)
{
	return (SKILL(skill))->level_exp(query_skill_level(skill) + 1);
}


// �^�ǳ̰����Ť��ޯ�
string query_highest_skill()
{
	mapping skills = query("skills");
	string *sortskill;
	
	if( !mapp(skills) )
		return 0;
	
	sortskill = sort_array(keys(skills), (: $(skills)[$1]["level"] >  $(skills)[$2]["level"] ? -1 : 1 :));
	
	return sortskill[0];
}
