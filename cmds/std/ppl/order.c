/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : order.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-19
 * Note   : �R�O���O
 * Update :
 *  o 2003-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <npc.h>
#include <skill.h>
#include <secure.h>

inherit COMMAND;

string help = @HELP
   �o�i�H���A�U�R�O���ۤv���H�O�A�ثe�}�񪺩R�O�p�U�G

�S��R�O���O
  order '���u' to name '����'		- �����u������W�r
  order '���u' to id '�^��'		- �����u���^��W�r

  order '���u' to work clerk		- �R�O���u�q�Ʃ������u�@(�u�@������ clerk/worker/engineer)
  order '���u' to work -d		- �R�O���u����u�@

  order '���u' to learn '�ޯ�'		- �R�O���u�V�z�ǲߧޯ�(�z����������Ƥf�~�ޯ�P�оǧޯ�)
  order '���u' to learn '�ޯ�' 50%	- �@���ӶO 50% ���믫�ӱоɭ��u

  order '���u' to heal			- �Φۤv����O�ӱϧU�w�˪����u(�Ȯɫ��O)

�зǨt�Ϋ��O
  order '���u' to command		- �d�߭��u�i�H�ϥΪ��t�Ϋ��O
  order '���u' to '���O'		- �R�O���u����Y�Өt�Ϋ��O(���O�Ϊk�P���a���`���Ϊk�ۦP)


�������O: labor
HELP;

private void do_command(object me, string arg)
{
	string id, key, value;
	object npc, env;

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�R�O�֡H\n");

	if( sscanf(arg, "%s to %s", id, key) != 2 )
		return tell(me, pnoun(2, me)+"�Q�n�R�O�ְ�����ơH(�� order boy to name �k��)\n");
		
	env = environment(me);

	if( !objectp(npc = present(id, env)) )
		return tell(me, "�o�̨S�� "+id+" �o�ӤH�C\n");
	
	if( !npc->is_living() )
		return tell(me, pnoun(2, me)+"�L�k�R�O�@�ӵL�ͩR���F��C\n");
	
	if( !npc->is_npc() || (!wizardp(me) && query("boss", npc) != me->query_id(1)) )
		return tell(me, npc->query_idname()+"�ä��O"+pnoun(2, me)+"���Ϊ��C\n");

	sscanf(key, "%s %s", key, value);

	if( query("faint", npc) && key != "heal" )
		return tell(me, pnoun(2, me)+"�L�k�R�O���ˤ������u�@����Ʊ��C\n");

	switch(lower_case(key))
	{
		case "name":
		{
			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n��"+npc->query_idname()+"�����򤤤�W�r�H\n");

			value = kill_repeat_ansi(value+NOR);

			if( !is_chinese(value) )
				return tell(me, "��"+pnoun(2, me)+"�Ρu����v���W�r�C\n");
	
			if( noansi_strlen(value) > 12 || noansi_strlen(value) < 2 )
				return tell(me, "����W�r�����b 1 �� 6 �Ӥ���r�����C\n");

			msg("$ME�h�J$YOU�D�G"+pnoun(2, npc)+"�H�ᤤ��W�l�N��W�s���u"+value+"�v�a�I\n", me, npc, 1);
			npc->set_idname(0, value);
			npc->do_command("say ���¦������ڨ��s������W�r�A�ڤ@�w�|���t�z������...");

			npc->save();				
			break;
		}
		case "id":
		{
			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n��"+npc->query_idname()+"������^��W�r�H\n");
			
			value = kill_repeat_ansi(lower_case(value)+NOR);
			
			if( !is_english(value) )
				return tell(me, "��"+pnoun(2, me)+"�Ρu�^��v���W�r�C\n");
			
			if( noansi_strlen(value) > 16 || noansi_strlen(value) < 3 )
				return tell(me, "�^��W�r�����b 3 �� 16 �ӭ^��r�Ƥ����C\n");
				
			if( user_exists(remove_ansi(value)) )
				return tell(me, "���o���W���{�����a���W�r�C\n");
				
			msg("$ME�h�J$YOU�D�G"+pnoun(2, npc)+"�H��^��W�l�N��W�s���u"+value+"�v�a�I\n", me, npc, 1);
			npc->set_idname(value, 0);
			npc->do_command("say ���¦������ڨ��s���^��W�r�A�ڤ@�w�|���t�z������...");
			
			npc->save();
			break;
		}
		case "work":
		{
			int job;
			string job_name;
			mapping action_info;

			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n�s"+npc->query_idname()+"������˪��u�@(engineer, worker, clerk, adventurer)�H\n");
			
			//if( !wizardp(me) && query("owner", env) != me->query_id(1) )
			//	return tell(me, "�o�̨ä��O"+pnoun(2, me)+"���ؿv���A�L�k�����u�@�C\n");

			switch( value )
			{
				case "-d":
					if( !query("job", npc) )
						return tell(me, npc->query_idname()+"�쥻�N�S���b�i��u�@�C\n");
					
					msg("$ME�s$YOU�������Ҧ��u�@�C\n", me, npc, 1);
					delete("job", npc);
					delete("position", npc);
					npc->do_command("say �O������C");
					return;
				case "engineer":
					job = ENGINEER;		
					job_name = HIR"�u"NOR RED"�{�v"NOR;
					break;
				
				case "worker":
					job = WORKER;
					job_name = HIY"�u"NOR YEL"�H"NOR;
					break;
				
				case "clerk":
					job = CLERK;
					job_name = HIC"��"NOR CYN"��"NOR;
					break;
				
				case "adventurer":
					job = ADVENTURER;
					job_name = HIG"��"NOR GRN"�I��"NOR;
					break;
				default:
					return tell(me, "�n����ؤu�@(engineer, worker, clerk, adventurer)�H\n"NOR);
			}
			
			//if( query("job/cur", npc) == job )
			//	return tell(me, npc->query_idname()+"�w�g�b��"+job_name+"���u�@�F�C\n");

			action_info = env->query_action_info(query("function", env));
			
			//if( !mapp(action_info) || !(action_info["job"] & job) )
			//	return tell(me, "�o�̨ä��ݭn"+job_name+"�o�ؤu�@�H�O�C\n");
			
			//if( env->query_master() != env )
			//	return tell(me, "���u�u��b�s�ꤤ�ߤu�@�A�b��L�a��u�@�O�S���Ϊ��C\n");

			msg("$ME�h�J$YOU�D�G�q���Ѱ_"+pnoun(2, npc)+"�N��"+job_name+"���u�@�a�I\n", me, npc, 1);
			npc->do_command("say �n���I�ڤ@�w�|�ɤߺɤO���u�@���I");			
			npc->do_command("follow -d");

			//broadcast(env, "�u��"+npc->query_idname()+"�}�l�ۤ��u�@.....\n");
			
			set("position", job_name, npc);
			set("job/cur", job, npc);
			set("job/efficiency", 500, npc);
			
			break;
		}

		case "learn":
		{
			int percent;
			int energy, costsocial;
			int max_level;
			int npc_level;

			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n"+npc->query_idname()+"�ǲߤ���ޯ�H\n");

			if( sscanf(value, "%s %d%%", value, percent) == 2 )
			{
				if( percent <= 0 || percent > 100 )
					return tell(me, "�п�J���T���믫�ӶO�ʤ���C\n");
					
				energy = percent * me->query_energy_max() / 100;
			}
			else
				energy = 50;

			value = lower_case(value);
			
			npc_level = npc->query_skill_level(value);
			
			if( !SKILL_D->skill_exists(value) )
				return tell(me, "�èS�� "+value+" �o�اޯ�C\n");

			if( npc_level >= me->query_skill_level("teach") )
				return tell(me, pnoun(2, me)+"��"+(SKILL("teach"))->query_idname()+"���Ť����A"+npc->query_idname()+"��"+(SKILL(value))->query_idname()+"���Ť���A���W���ɡC\n");

			max_level = (SKILL(value))->max_level(npc);
			
			if( npc_level >= max_level )
				return tell(me, npc->query_idname()+"��"+(SKILL(value))->query_idname()+"�ޯ൥�Ťw�g��F�W���C\n");

			if( !(SKILL(value))->allowable_learn(npc) )
				return tell(me, npc->query_idname()+"�L�k�ǲ߳o���ޯ�C\n");
				
			if( !me->cost_energy(energy) )
				return tell(me, pnoun(2, me)+"���믫���� "+energy+"�A�L�k�A�Ǳ§ޯ�F�C\n");
				
			costsocial = energy / 10;
			
			if( costsocial < 5 )
				costsocial = 5;
			
			if( !me->cost_social_exp(costsocial) )
				return tell(me, pnoun(2, me)+"�����|�g��Ȥ��� "+costsocial+"�A�L�k�A�Ǳ§ޯ�F�C\n");
				
			msg("$ME�ӶO "+energy+" ���믫�J�Ӧa�Ǳ�$YOU����"+(SKILL(value))->query_idname()+"���g��C\n"+npc->query_idname()+"��"+(SKILL(value))->query_idname()+"���F��i�@�B���A�ѡC\n", me, npc, 1);
			
			npc->add_skill_exp(value, pow((me->query_skill_level("eloquence")+1) * energy, 1.001));
			
			npc_level = npc->query_skill_level(value);
			
			if( npc_level >= max_level )
			{
				set("skills/"+value+"/level", max_level, npc);
				set("skills/"+value+"/exp", (SKILL(value))->level_exp(max_level), npc);
				//tell(me, npc->query_idname()+"��"+(SKILL(value))->query_idname()+"�ޯ൥�Ťw�g����A���W���ɡC\n");
			}
			break;
		}
		case "command":
		{
			npc->do_command("say �ڥثe�i�H��檺�t�Ϋ��O���G"+implode(COMMAND_D->query_available_command(STANDARD), ", "));
			break;
		}
		case "heal":
		{
			msg("$ME������K�b$YOU�I��A�H���ͤ��O�N�ۤv���ͩR���O�`�J$YOU�餤�C\n", me, npc, 1);
			
			npc->set_all_ability_full();
			
			if( npc->is_faint() )
				npc->wakeup();
			else
				msg("$ME�Pı�믫�ʭ��C\n", npc, 0, 1);

			npc->save();
		
			me->faint();
			break;
		}
		default:
		{
			msg("$ME�R�O$YOU�G�u"HIY+(value?key+" "+value:key)+NOR"�v�C\n", me, npc, 1);

			if( !wizardp(me) && (COMMAND_D->find_chn_command(key) || COMMAND_D->find_emt_command(key)) )
				return npc->do_command("say �ڤ���b�W�D�W�o���]����@��������O");

			npc->do_command(value?key+" "+value:key);
			break;
		}
	}
}