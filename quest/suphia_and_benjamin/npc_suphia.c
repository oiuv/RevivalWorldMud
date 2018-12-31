/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : npc_suphia.c
 * Author : tzj@RevivalWorld
 * Date   : 2006-09-0
 * Note   : 
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

/*
	�t�ιw�]�� NPC �����禡
	void reply_emote(object me, string verb, string args);
	void reply_say(object me, string msg);
	void reply_catch_tell(object me, string verb, string args);
	void reply_get_object(object me, object ob, mixed amount);
	void reply_get_money(object me, string unit, mixed money);
*/

#include <ansi.h>
#include <inherit.h>
#include <gender.h>
#include <daemon.h>
#include <quest.h>

inherit STANDARD_NPC;

void do_command(string cmd);
void make_money();

void set_behavior()
{
	set("behavior/random", 
	([
		(: do_command("stare �������F��") :)      	: 10,
		(: do_command("ponder �Z�ǩ�... ����") :) 	:  5,
	]));
}
void make_money()
{

}
void delete_behavior()
{

}

void reply_say(object me, string msg)
{
	
	if( objectp(me) && msg && strsrch(msg, "��") != -1 )
		do_command("say ��... �p�Х��b�N��A"+ pnoun(2, me) +"�n�@�_�Y�ܡH");
}

void reply_emote(object me, string verb, string args)
{
	
	//quest�����ݵ�
 	if( verb == "ask" )
 	{
/*
 		if(args == "")
 			do_command("nod" + " " + me->query_id(1) );
 			
 		if( strsrch(args, "�F��") >= 0 )
 		{
	 		if( strsrch(args, "������") >= 0 )
	 		{
	 			do_command("say �c... ��... ���S�����򭫭n����ЧO�b�N��...");
	 			msg("$ME���G���@�N�N���D�~��U�h... \n",this_object(), 0, 1);
	 		}

	 		else if ( strsrch(args, "�Z�ǩ�") >= 0 )
	 		{
	 			do_command("say �O��... �Z�ǩ������j���b�e���F�誺���~�W�a... ");
	 		}
	 		else if ( strsrch(args, "�Ŭu") >= 0 )
	 		{
	 			do_command("say ��ť���L�b�ܻ����F��A���G���ӵۦW�Ŭu���ˤl�H�i�O�ڨS�h�L... ");
	 			msg("$ME�C�n���ۻy�D: " + HIG + "�n�O��h�N�n�F... \n" + NOR ,this_object(), 0, 1);
	 		}
	 		else	
	 		{
	 			switch(random(5))
	 			{
	 				case 0:
	 					do_command("?" + " " + me->query_id(1) );
	 					do_command("say ��... �H"+ pnoun(2, me) +"�O�Q�ݧڪF�観����ܡH");
	 					break;
	 				case 1:
	 				case 2:
	 					do_command("say �F���ˤF�H");
	 					break;
	 				case 3:
	 					do_command("? �F��");
	 					break;
	 				case 4:
	 					do_command("tense" + " " + me->query_id(1) + " " + "�F��ǨӤ���ƶܡI�H");
	 					break;
	 			}
	 		}
	 	}
	 	else if ( strsrch(args, "�Z�ǩ�" ) >= 0 )
	 	{
	 		if( (strsrch(args, "�O") >= 0 ) && (strsrch(args,"��") >= 0 ) )
 			{
				do_command("say �Z�ǩ������O���l�̪��W�H�@�Iť���L�b���H�\�Ŭu���]... ");
 				msg("$ME�ܰ����a��������A���o�ݰ_�ӱo���Ǹ���C\n",this_object(), 0, 1);
 			}
 			else if( strsrch(args, "�Z�ǩ��Ŭu") >= 0 )
 			{
 				do_command("say" + " " + "���O����ǷŬu�W�ڡI");
 				do_command("laugh" + " " +  me->query_id(1));
 			}
 			else if( strsrch(args, "�Z�ǩ����]") >= 0 )
 			{
 				do_command("say" + " " + "���O����Ǯ��]�W�ڡI");
 				do_command("laugh" + " " +  me->query_id(1));
 			}
 			
 			else if( (strsrch(args, "�Ŭu") >= 0 ) || (strsrch(args,"���]") >= 0 ) )
 			{
				do_command("say �O��ť���Z�ǩ������b���H�\�Ŭu���]... ");
 			}
 			else
 			{
 				switch(random(10))
 				{
 					case 0:
 					case 1:
 					case 2:
 						do_command("hand" + " " +  me->query_id(1) + " " + pnoun(2, me) + "�{�ѯZ�ǩ������ܡH");
 						break;
 					case 3:
 						do_command("peer" + " " + me->query_id(1));
 						do_command("say" + " " + pnoun(2, me) + "���D�Z�ǩ������O�ֶܡH");
 						break;
 					case 4:
 					case 5:
 						do_command("boggle" + " " + me->query_id(1));
 						do_command("say" + " "+ "�ڡI�ڪ��ۨ��ۥѳQ"+ pnoun(2, me) +"ť��աI�H");
 						break;
 					case 6:
 					case 7:
 						msg("$ME��M�y���F�_�ӡA������]�S���C\n",this_object(), 0, 1);
 						break;
 					case 8:
 					case 9:
 						msg("$ME���G���J�I�䤧���A���o�@�y�C\n",this_object(), 0, 1);
 						break;
 				}

  			}

	 	}
	 	else if ( strsrch(args, "�Ŭu" ) >= 0 )
	 	{
	 		if ( strsrch(args, "���]" ) >= 0 )
	 		{
	 			do_command("pout");
	 			do_command("say" + " " + "����Ŭu���]���A�ڳ̰Q���Ŭu���]�F�I... �ڡA��p�A��§�F�C");	 		
	 		}
	 		else if ( (strsrch(args, "��" ) >= 0) || (strsrch(args, "��B" ) >= 0) )
	 		{
	 			do_command("hmm" + " " +me->query_id(1));
	 			do_command("say" + " " + "�ڤ��ӲM���Ŭu���ơA����ť���L���G�F�観�ӵۦW�Ŭu���ˤl�H");	 		
	 		}
	 		else
	 		{
		 		do_command("?" + " " + me->query_id(1));
		 		do_command("say" + " "+ "��... �H" + pnoun(2, me) + "�O�Q�ݧڭ��̦��Ŭu�ܡH�H");
	 		}
	 	}
*/
 	}
 	else
 	{
 		switch(verb)
 		{
 			case "dunno"	:
 			case "sob"	:
 			case "cry"	:
 			case "inn"	:
 				switch(random(2))
 				{
 					case 0:
		 				do_command("pat" + " " +  me->query_id(1));
		 				break;
		 			case 1:
		 				do_command("comfort" + " " + me->query_id(1));
		 				break;
 				}
 				break;
 				
 			case "gan"	:
 			case "fuck"	:
 				switch(random(3))
 				{
 					case 0:
		 				do_command("?" + " " +  me->query_id(1));
		 				break;
		 			case 1:
		 				do_command("inn" + " " + me->query_id(1) + " " + "������n����...");
		 				break;
		 			case 2:
		 				do_command("twiddle");
		 				do_command("say" + " " + "�����A�������F�H");
		 				break;
 				}
  				break;
 
  			case "bonk"	:
  			case "kick"	:
 			case "flog"	:
 			case "slap"	:
				switch(random(3))
 				{
 					case 0:
		 				do_command("sob" + " " +  me->query_id(1));
		 				break;
		 			case 1:
		 				do_command("inn" + " " + me->query_id(1) + " " + "�n�h�@...");
		 				break;
		 			case 2:
		 				do_command("draw");
		 				do_command("say" + " " + "�ڰ����F����ܡH");
		 		}		break;
  				break;

  			case "cat"	:
  			case "smile"	:
  			case "hi"	:
  				switch(random(3))
  				{
  					case 0:
  						do_command("hug" + " " + me->query_id(1));
  						break;
  					case 1:
  						do_command("smile" + " " + me->query_id(1));
  						break;
  					case 2:
  						do_command("cuddle" + " " + me->query_id(1));
  						break;
  				}
  				break;
  			
  			case "good"	:
  			case "clap"	:
  			case "cheer"	:
				switch(random(4))
  				{
  					case 0:
  						do_command("blush");
  						break;
  					case 1:
  						do_command("ya" + " " + me->query_id(1));
  						break;
  					case 2:
  						do_command("thank" + " " + me->query_id(1));
  						break;
  					case 3:
  						do_command("bow" + " " + me->query_id(1));
  						break;
  				}
  				break;

  			case "hi"	:
  				do_command("hi" + " " + me->query_id(1));
  				break;

 		}
 	}
}

int target_gone(object me)
{

}

void reply_get_object(object me, object ob, mixed amount)
{

}

void reply_get_money(object me, string unit, mixed money)
{

}

void create()
{
	set_idname("suphia","Ĭ���");
	
	set("nickname", "�ѯu�g�k");

	set("unit", "��");
	
	set("age", 17);

	set("gender", FEMALE_GENDER);

	set("long",@TEXT
�@�@���e���֤k�~�����b�Q�C���A�¦Ⲵ���Ħ�u�v�A�X�𥼲檺�y�W���ǳ\��
�]�C�o�����²�몺�A�a�k�`�諸��A�A���b�Ȥl�W���G�Y���ҫ䪺��V������
�F��C
TEXT);

	// �ҰʥD�ʦ欰
	set_behavior();

	startup_living();
}
