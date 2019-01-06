/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : postoffice.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-21
 * Note   : �l��
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit ROOM_ACTION_MOD;

private void input_mail_confirm(object me, string receiver, string title, string content, string yn)
{
	int time;
	mapping maildata;
	string file;

	if( yn != "y" && yn != "yes " )
	{
		tell(me, "�����H�X�H��C\n");
		return me->finish_input();
	}

	time = time();
	file = read_file(user_mail(receiver));

	if( stringp(file) )
		maildata = restore_variable(file);

	if( !mapp(maildata) )
		maildata = allocate_mapping(0);

	// �קK key ����
	while( !undefinedp(maildata[time]) ) time++;

	maildata[time] = allocate_mapping(0);

	maildata[time]["time"] = time();
	maildata[time]["sender"] = me->query_id(1);
	maildata[time]["receiver"] = receiver;
	maildata[time]["title"] = kill_repeat_ansi(title+NOR);
	maildata[time]["content"] = kill_repeat_ansi(content+NOR);

	tell(me, "���\�H�X�u"+title+"�v�H��C\n");

	me->finish_input();

	if( find_player(receiver) )
		tell(find_player(receiver), HIY"\n�l������@�ʨӦ�"+me->query_idname()+"���H��A���D���u"+title+"�v�C\n"NOR);

	set_temp("postoffice", time, me);

	write_file(user_mail(receiver), save_variable(maildata), 1);
}

private void input_mail_content(object me, string receiver, string title, string content)
{
	if( !content || !content[0] )
	{
		tell(me, GRN"�п�J"HIG"�H�󤺮e�G\n"NOR);
		me->edit( (: input_mail_content, me, receiver, title :) );
		return;
	}

	tell(me, "�T�w�H�X�H��H(y/n)");
	input_to( (: input_mail_confirm, me, receiver, title, content :) );
}

private void input_mail_title(object me, string receiver, string title)
{
	if( !title || !title[0] )
	{
		tell(me, GRN"�п�J"HIG"�H����D�G"NOR);
		input_to( (: input_mail_title, me, receiver :) );
		return;
	}
	if( noansi_strlen(title) > 40 )
	{
		tell(me, HIR"�D�D���o�W�L 40 �Ӧr���C\n"NOR);
		tell(me, GRN"�п�J"HIG"�H����D�G"NOR);
		input_to( (: input_mail_title, me, receiver :) );
		return;
	}

	if( title && query("encode/gb", me) ) title = G2B(title);

	tell(me, GRN"�п�J"HIG"�H�󤺮e�G\n"NOR);

	me->edit( (: input_mail_content, me, receiver, title :) );
}

// �H�X�H��
private void do_mail(object me, string arg)
{
	if( !arg )
		return tell(me, "�п�J����H�� ID�C\n");

	if( !wizardp(me) && query_temp("postoffice", me) + 60 > time() )
		return tell(me, pnoun(2, me)+"���[�e�~�H�X�H��A�еy��@�����C\n");

	if( !user_exists(arg) )
		return tell(me, "�o�ӥ@�ɤ��ä��s�b�o�Ӫ��a�C\n");

	tell(me, GRN"�п�J"HIG"�H����D�G"NOR);

	input_to( (: input_mail_title, me, arg :) );
}


// �C�X�H��
private void do_list(object me, string arg)
{
	int i;
	string msg;
	string myid = me->query_id(1);
	string file = read_file(user_mail(myid));
	mapping maildata;

	if( !stringp(file) )
		return tell(me, pnoun(2, me)+"�S������H��C\n");

	maildata = restore_variable(file);

	if( !mapp(maildata) || !sizeof(maildata) )
		return tell(me, pnoun(2, me)+"�S������H��C\n");

	msg = me->query_idname()+"���H��p�U�G\n";
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += "    �H��H        ���             ���A  �D�D\n";
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

	foreach(int time in sort_array(keys(maildata), -1))
	{
		msg += sprintf("%-4s%-14s%-17s%-6s%s\n",
			++i+".",
			capitalize(maildata[time]["sender"]),
			HIC+TIME_D->replace_ctime(time)+NOR,
			maildata[time]["read"] ? HIG"�wŪ"NOR : HIR"��Ū"NOR,
			HIY+maildata[time]["title"])+NOR;
	}

	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += HIW"Ū���H��Gread '�s��'�B�R���H��Gdelete '�s��' \n\n"NOR;

	me->more(msg);
}


// Ū���H��
private void do_read(object me, string arg)
{
	int i;
	int selected;
	string msg;
	string myid = me->query_id(1);
	string file = read_file(user_mail(myid));
	mapping maildata;

	if( !stringp(file) )
		return tell(me, pnoun(2, me)+"�S������H��C\n");

	maildata = restore_variable(file);

	if( !mapp(maildata) || !sizeof(maildata) )
		return tell(me, pnoun(2, me)+"�S������H��C\n");

	i = to_int(arg);

	if( i < 1 || i > sizeof(maildata) )
		return tell(me, "��J�H��s�����~�C\n");

	selected = sort_array(keys(maildata), -1)[i-1];

	msg =  "�H��H  �G"+capitalize(maildata[selected]["sender"])+"\n";
	msg += "�H�����G"+HIG+TIME_D->replace_ctime(maildata[selected]["time"])+NOR+"\n";
	msg += "�H��D�D�G"+HIY+maildata[selected]["title"]+NOR+"\n";
	msg += "�H�󤺮e�G\n";
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += "\n"+maildata[selected]["content"]+"\n\n";
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

	me->more(msg);

	if( undefinedp(maildata[selected]["read"]) )
	{
		maildata[selected]["read"] = 1;
		write_file(user_mail(myid), save_variable(maildata), 1);
	}
}

// ���H��
private void do_delete(object me, string arg)
{
	int i;
	string myid = me->query_id(1);
	string file = read_file(user_mail(myid));
	mapping maildata;

	if( !stringp(file) )
		return tell(me, pnoun(2, me)+"�S������H��C\n");

	maildata = restore_variable(file);

	if( !mapp(maildata) || !sizeof(maildata) )
		return tell(me, pnoun(2, me)+"�S������H��C\n");

	i = to_int(arg);

	if( i < 1 || i > sizeof(maildata) )
		return tell(me, "��J�H��s�����~�C\n");

	map_delete(maildata, sort_array(keys(maildata), -1)[i-1]);

	tell(me, "�R���� "+i+" �ʫH�󧹲��C\n");

	write_file(user_mail(myid), save_variable(maildata), 1);
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"lobby"	:
	([
		"short"	: HIG"�l��"NOR GRN"�j�U"NOR,
		"help"	:
			([
"topics":
@HELP
    �l���j�U�t�d�Ҧ��l�F�����~�ȡC
HELP,

"mail":
@HELP
�H�H���Y�쪱�a�A�Ϊk�p�U�G
  mail '���aID'		- �H�H���Y�쪱�a
HELP,

"read":
@HELP
���H�A�Ϊk�p�U�G
  read 2		- Ū���� 2 �ʫH
HELP,

"list":
@HELP
�C�X�Ҧ��H��A�Ϊk�p�U�G
  list			- ��ܩҦ��H��C��
HELP,

"delete":
@HELP
�N�Y�ӫH��R���A�Ϊk�p�U�G
  delete 2		- �R���� 2 �ʫH
HELP,
			]),
		"action":
			([
				"mail"		: (: do_mail :),
				"read"		: (: do_read :),
				"list"		: (: do_list :),
				"delete"	: (: do_delete :),
			]),
	]),
]);


// �]�w�ؿv�����
nosave array building_info = ({

	// �ؿv��������W��
	HIG"�l��"NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION

	// �}�i�����O��
	,"5000000"

	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,30

	// �̰��i�[�\�Ӽh
	,1

	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0

	// �ؿv���ɥN
	,1
});
