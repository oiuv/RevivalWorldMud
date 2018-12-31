/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _condition_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-04
 * Note   : ���A
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

// �߰ݪ��A
nomask int in_condition(string condition_file)
{
	return mapp(query("condition/"+replace_string(condition_file, "/","#")));
}

// �Ұʪ��A, 1 �����
varargs nomask void start_condition(string condition_file, int time, int heart_beat)
{
	mapping condition_setup = allocate_mapping(0);

	// �Y�w�g�b�ۦP���A���h��������e���A
	if( in_condition(condition_file) )
		condition_file->stop_effect(this_object());

	// �]�w���A����ɶ��Ψϥιw�]�ɶ�
	if( !undefinedp(time) && time > 0 )
		condition_setup["time"] = time;
	else
		condition_setup["time"] = condition_file->query_default_condition_time();
		
	// �]�w���A�߸��ɶ��Ψϥιw�]�߸��ɶ�
	if( !undefinedp(heart_beat) && heart_beat > 0 )
		condition_setup["heart_beat"] = heart_beat;
	else
		condition_setup["heart_beat"] = condition_file->query_default_condition_heart_beat();

	set("condition/"+replace_string(condition_file, "/","#"), condition_setup);
	
	// ���檬�A�Ұʮɪ��ĪG
	condition_file->start_effect(this_object());
}


// �������A
nomask void stop_condition(string condition_file)
{
	delete("condition/"+replace_string(condition_file, "/","#"));
	
	// ���檬�A�����ɪ��ĪG
	condition_file->stop_effect(this_object());
}

// ���ܪ��A�ɶ�
nomask void change_condition_time(string condition_file, int time)
{
	if( !in_condition(condition_file) ) return;
		
	addn("condition/"+replace_string(condition_file, "/","#")+"/time", time);
}

// �^�Ǫ��A���
varargs nomask mapping query_condition(string condition_file)
{
	if( undefinedp(condition_file) )
		return query("condition");

	return query("condition/"+replace_string(condition_file, "/","#"));
}
