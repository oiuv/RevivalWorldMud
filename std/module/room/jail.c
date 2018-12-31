/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : jail.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-24
 * Note   : �ʺ�
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
#include <object.h>

inherit ROOM_ACTION_MOD;

private void do_arrest(object me, string arg)
{
	object target;
	object env = environment(me);
	string city = env->query_city();
	string reason;

	if( !wizardp(me) && !CITY_D->is_mayor(city, me) )
		return tell(me, "�u����������e���ǤH�C\n");

	if( !arg || !arg[0] )
		return tell(me, pnoun(2, me)+"�Q�n�e������ǤH�H\n");

	if( sscanf(arg, "%s because %s", arg, reason) != 2 )
		return tell(me, "�п�J���T�����O�榡�C\n");

	target = load_user(arg);

	if( !objectp(target) )
		return tell(me, "�o�ӥ@�ɨèS���o�쪱�a�C\n");
		
	if( !wizardp(me) && query("city", target) != city )
	{
		tell(me, target->query_idname()+"�ä��O�����������C\n");
		
		if( !userp(target) )
			destruct(target);

		return;
	}
	
	if( target == me )
		return tell(me, pnoun(2, me)+"�L�k�e���ۤv�C\n");
	
	if( query("prisoner", target) && same_environment(target, me) )
	{
		tell(me, target->query_idname()+"�w�g�Q���i�ʺ��F�C\n");

		if( !userp(target) )
			destruct(target);

		return;
	}

	if( wizardp(me) )
		CHANNEL_D->channel_broadcast("news", target->query_idname()+HIR"�]���u"HIW + reason + NOR HIR"�v�D��"+me->query_idname()+HIR"���e���A���i�F�Ův�ʺ������լd�C");
	else
		CHANNEL_D->channel_broadcast("city", target->query_idname()+HIR"�]���u"HIW + reason + NOR HIR"�v�D��"+me->query_idname()+HIR"���e���A���i�F�F���ʺ��C", me);
	
	target->move_to_environment(me);
	
	set("prisoner", 1, target);
	set("quit_place", base_name(env), target);
	target->save();

	if( !userp(target) )
		destruct(target);
}

private void do_release(object me, string arg)
{
	object target;
	object env = environment(me);
	string city = env->query_city();
	string reason;
	
	if( !wizardp(me) && !CITY_D->is_mayor_or_officer(city, me) )
		return tell(me, "�u�������Ωx���������ǤH�C\n");

	if( !arg || !arg[0] )
		return tell(me, pnoun(2, me)+"�Q�n�������ǤH�H\n");
		
	if( sscanf(arg, "%s because %s", arg, reason) != 2 )
		return tell(me, "�п�J���T�����O�榡�C\n");

	target = find_player(arg);

	if( !objectp(target) )
		return tell(me, "�u�W�èS���o�쪱�a�C\n");
		
	if( !wizardp(me) && query("city", target) != city )
		return tell(me, target->query_idname()+"�ä��O�����������C\n");
	
	if( target == me )
		return tell(me, pnoun(2, me)+"�L�k����ۤv�C\n");
		
	if( !same_environment(me, target) )
		return tell(me, target->query_idname()+"�ä��b���B�C\n");
		
	if( !query("prisoner", target) )
		return tell(me, target->query_idname()+"�ä��O�}�ǡC\n");
		
	if( wizardp(me) )
	{
		CHANNEL_D->channel_broadcast("news", me->query_idname()+HIR"�]���u"HIW + reason + NOR HIR"�v�N"+target->query_idname()+HIR"�q�Ův�ʺ�������X�ӡC"NOR);
		msg("$ME��F��ۤv���W���ǹСA�^��Ův�����h�F... \n" + NOR , target, 0, 1);
		target->move(load_object(WIZ_HALL));
	}
	else
		CHANNEL_D->channel_broadcast("city", me->query_idname()+HIR"�]���u"HIW + reason + NOR HIR"�v�N"+target->query_idname()+HIR"�q�F���ʺ�������X�ӡC"NOR, me);
	
	delete("prisoner", target);
	target->save();
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"jail"	:
	([
		"short"	: HIR"�c"NOR RED"��"NOR,
		"help"	:
			([
"topics":
@HELP
    �Ψӥ}�T�ǤH���a��C
HELP,

"arrest":
@HELP
�e���Y�쪱�a�A�Ϊk�p�U�G
  arrest '���aID' because '��]'	- �N�Y��u�W�����e��(�������O)
HELP,

"release":
@HELP
����Y�쪱�a�A�Ϊk�p�U�G
  release '���aID' because '��]'	- �N�Y��Q���b�ʺ�������������(�����P�x�����O)
HELP,

			]),
		"action":
			([
				"arrest"	: (: do_arrest :),
				"release"	: (: do_release :),
			]),
	]),
]);


// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIR"�ʺ�"NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,AGRICULTURE_REGION | INDUSTRY_REGION

	// �}�i�����O��
	,"5000000"
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,-100
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});

