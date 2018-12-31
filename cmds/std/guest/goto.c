/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : goto.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : goto ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� goto ���O�C
HELP;

private void command(object me, string arg)
{
	object ppl, newenv;
	
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�h���̡H\n", CMDMSG);
	if( ppl = find_player(arg) )
		newenv = environment(ppl);
	else
	{
		arg = resolve_path(me, arg);
		intact_file_name(ref arg);
		
		if( find_object(arg) || file_exist(arg) )
			newenv = load_object(arg);
	}
	
	if( !newenv ) return tell(me, "�S���o�Ӧa��C\n", CMDMSG);
	
	msg("$ME���i�@�}"HIW"����"NOR"������F�C\n", me, 0, 1, ENVMSG);

	if( me == ppl )
		return tell(me, "�e��"+pnoun(2, me)+"�ثe�Ҧb���a��H\n", CMDMSG);

	if( same_environment(me, ppl) )
		return tell(me, pnoun(2, me)+"�P"+ppl->query_idname()+"�w�g�b�P�@�Ӧa��C\n", CMDMSG);

	if( objectp(ppl) && userp(ppl) && newenv->is_maproom() )
		me->move(query_temp("location", ppl));
	else
		me->move(newenv);
	
	msg(HIW"��������"NOR"������M�X�{$ME�����v�C\n", me, 0, 1, ENVMSG);
}