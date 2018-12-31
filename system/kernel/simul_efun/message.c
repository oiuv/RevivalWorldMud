/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : message.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-7-2
 * Note   : included by Simul_Efun Object
 * Update :
 *  o 2003-02-14 Clode �T�j�T���禡 tell(), broadcast(), msg()

 -----------------------------------------
 */

#include <location.h>
#include <message.h>
#include <daemon.h>

//
// write(), tell_object(), ����H�T���禡
// ������� tell() �B�z, �H����䴩�a�Ϩt�λP Client �T���ǿ�
//
varargs void tell(mixed target, string msg, string classes)
{
	if( objectp(target) || arrayp(target) )
		target->catch_tell(msg, classes);
}

/* ��Ҧ��ϥΪ̩�X�T�� */
void shout(string msg)
{
	tell(filter_array(users(), (: !$1->is_login_ob() :)), msg, SYSMSG);
}

//
// say(), tell_room() ���h�H�T���禡
// ������� broadcast() �B�z, �H����䴩�a�Ϩt�λP Client �T���ǿ�
//
varargs void broadcast(mixed target, string msg, string classes, mixed exclude)
{
	object *receivers = allocate(0);

	if( !target ) return;

	if( stringp(target) )
		target = load_object(target);

	// �s����H�Y���@�Ӫ���, �h��䤺�e����P�P���Ҫ���ǰe�T��
	if( objectp(target) )
	{
		//object env = environment(target);

		//if( objectp(env) )
		//{
		//	if( env->is_maproom() )
		//		return broadcast(query_temp("location", target), msg, classes, exclude);

		//	receivers += all_inventory(env);
		//}
		receivers += all_inventory(target);
	}
	// �Y�s����H���a�Ϩt�ήy��, �h�B�z�y�йL�o
	else if( arrayp(target) )
		receivers += MAP_D->coordinate_inventory(target);

	// ���~��H
	if( !undefinedp(exclude) )
	{
		if( arrayp(exclude) )
			receivers -= exclude;
		else if( objectp(exclude) )
			receivers -= ({ exclude });
	}

	// ��Ҧ������̼s���T��
	tell(receivers, msg, classes);
}


/* �Τ@�N�W���ഫ���T���o�e�禡 */
varargs void msg(string msg, object me, object you, int onlooker, string classes, object *exclude, mapping translate)
{
	string my_msg, your_msg;
	string my_idname, your_idname;

	if( !msg || !objectp(me) ) return;

	// ���o Me ������
	my_idname = me->query_idname() || "�L�W��";


	// �B�~��J����r���ഫ, �Y���F $ME �P $YOU �H�~���ഫ
	if( mapp(translate) )
		foreach(string pattern, string replace in translate)
		msg = replace_string(msg, pattern, replace);

	// Me �T�����ĤG�H���ഫ
	my_msg = replace_string(msg, "$ME", pnoun(2, me));

	// �Y����H���� you, �h�� you �@�T���B�z
	if( objectp(you) )
	{
		your_idname = you->query_idname() || "�L�W��";

		// Me �T�����ĤT�H���ഫ
		my_msg = replace_string(my_msg, "$YOU", your_idname, 1);
		my_msg = replace_string(my_msg, "$YOU", pnoun(3, you));

		// You �T�����ĤG�H���ഫ
		your_msg = replace_string(msg, "$ME", my_idname, 1);
		your_msg = replace_string(your_msg, "$ME", pnoun(3, me));
		your_msg = replace_string(your_msg, "$YOU", pnoun(2, you));


		// �磌�� you �e�X�T��
		if( !arrayp(exclude) || member_array(you, exclude) == -1 )
			tell(you, your_msg, classes);
	}

	// �磌�� Me �e�X�T��
	if( !arrayp(exclude) || member_array(me, exclude) == -1 )
		tell(me, my_msg, classes);

	// �Y�� onlooker(���[��) �Ѽ�, �h��P me �� you �P�@���Ҥ����ͪ��o�X�T��
	if( onlooker )
	{
		string onlookers_msg;
		object *onlookers = present_objects(me) + ({environment(me)})||({});

		if( objectp(you) && environment(me) != environment(you) ) 
			onlookers += present_objects(you) + ({environment(you)})||({});

		onlookers -= ({ me, you }) + (exclude || ({}));

		onlookers_msg = replace_string(msg, "$ME", my_idname);

		if( objectp(you) ) 
			onlookers_msg = replace_string(onlookers_msg, "$YOU", your_idname);

		// ��Ҧ����[�̪���e�X�T��
		tell(onlookers, onlookers_msg, classes);
	}
}

void write(mixed args...)
{
	error("write() �w�g�o��ϥ�, �Ч�� tell()");
}

void tell_object(mixed args...)
{
	error("tell_object() �w�g�o��ϥ�, �Ч�� tell()");
}

void say(mixed args...)
{
	error("say() �w�g�o��ϥ�, �Ч�� broadcast()");
}

void tell_room(mixed args...)
{
	error("tell_room() �w�g�o��ϥ�, �Ч�� broadcast()");
}

void message(mixed args...)
{
	error("message() �w�g�o��ϥ�, �Ч�� msg()");
}

void printf(string format, mixed args...)
{
	efun::printf("printf():\n"+format, args...);
}
