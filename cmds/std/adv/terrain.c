/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : terrain.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-09-07
 * Note   : terrain ���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <map.h>
#include <daemon.h>
#include <terrain.h>

inherit COMMAND;

string help = @HELP
�@�@��ʫإߦa�Ϊ����O

�i�H�ϥΪ��ѼƦp�U�G

clear			- �N�ثe��m���y�и�Ƨ����M�����Ŧa
forest			- �إߴ˪L�a��([1;32m��[m)
mountain1		- �إߤs�ߦa��([0;33m��[m)
mountain2		- �إߤs�ߦa��([0;33m��[m)
river1			- �إߪe�y([0;44;1;34m��[m�y�V���F)
river2			- �إߪe�y([0;44;1;34m��[m�y�V����)
river3			- �إߪe�y([0;44;1;34m��[m�y�V���n)
river4			- �إߪe�y([0;44;1;34m��[m�y�V���_)
road			- �إ߹D��
road '�W��'		- �إ߹D���P�D���W��
bridge			- �إ߾���([0;41;1;31m��[m)
bridge '�W��'		- �إ߾��ٻP���٦W��
wall			- �إ�����ݩ�
farm			- �إ߹A��([0;42;1;32m��[m)
pasture			- �إߪ���([0;43;1;33m��[m)
fishfarm		- �إ߾i�޳�([0;44;1;34m��[m)
light			- �إ߸��O([1;33m�F[m)
pool			- �إߤ���([1;36m��[m)
pavilion		- �إ߲D�F([1;37m��[m)
tree			- �إߤj��([1;32m�D[m)
grass			- �إ߯��([0;32m�s[m)
statue			- �إ��J��([1;35m��[m)
icon '�ϧ�'		- ��¦b�ثe�y�ФWø��(�L�ݩʸ��)
icon -d			- �R���ثe�y�ФW���ϧ�
room '�ж���m'		- �]�w�ثe�y�гs�����ж��ɮ�
room -d			- �R���ж��s���]�w
room broken		- �]�w���@��k��ж�
name '�y�ЦW��'		- �]�w�ثe�y�й������W��
name -d			- �R���W�ٳ]�w

HELP;

private void command(object me, string arg)
{
	string arg2;
	array loc = query_temp("location", me);

	if( !is_command() ) return;

	if( !arg )
		return tell(me, help);

	if( !arrayp(loc) || !AREA_D->is_area_location(loc) )
		return tell(me, pnoun(2, me)+"�u��b���ϳ]�w�a�ΡC\n");
		
	sscanf(arg, "%s %s", arg, arg2);

	switch(arg)
	{
		// �M�����a�Ҧ����
		case "clear":
		{
			// �M���y�и��
			AREA_D->delete_coor_data(loc);
			
			// �M���y�йϧ�
			AREA_D->delete_coor_icon(loc);
			
			msg("$ME�N�o���g�a�i�����a�C\n", me, 0, 1);
			
			break;
		}
		// �إߴ˪L�a��
		case "forest":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, FOREST);
			AREA_D->set_coor_icon(loc, HIG"��"NOR);
			
			msg("$ME�b�o���g�a�W�إߤF�˪L�u"HIG"��"NOR"�v���a�ΡC\n", me, 0, 1);
			
			break;
		}
		// �إߤs�ߦa��(��)
		case "mountain1":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, MOUNTAIN);
			AREA_D->set_coor_icon(loc, YEL"��"NOR);
			
			msg("$ME�b�o���g�a�W�إߤF�s�ߡu"YEL"��"NOR"�v���a�ΡC\n", me, 0, 1);
			
			break;
		}
		// �إߤs�ߦa��(�k)
		case "mountain2":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, MOUNTAIN);
			AREA_D->set_coor_icon(loc, YEL"��"NOR);
			
			msg("$ME�b�o���g�a�W�إߤF�s�ߡu"YEL"��"NOR"�v���a�ΡC\n", me, 0, 1);
			
			break;
		}
		// �إߪe�y�a��(�F)
		case "river1":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, RIVER);
			AREA_D->set_coor_icon(loc, BBLU HIB"��"NOR);
			AREA_D->set_coor_data(loc, FLAGS, FLOW_EAST);

			msg("$ME�b�o���g�a�W�إߤF�e�y�u"BBLU HIB"��"NOR"�v(�F�V)���a�ΡC\n", me, 0, 1);
			
			break;
		}
		// �إߪe�y�a��(��)
		case "river2":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, RIVER);
			AREA_D->set_coor_icon(loc, BBLU HIB"��"NOR);
			AREA_D->set_coor_data(loc, FLAGS, FLOW_WEST);

			msg("$ME�b�o���g�a�W�إߤF�e�y�u"BBLU HIB"��"NOR"�v(��V)���a�ΡC\n", me, 0, 1);
			
			break;
		}
		// �إߪe�y�a��(�n)
		case "river3":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, RIVER);
			AREA_D->set_coor_icon(loc, BBLU HIB"��"NOR);
			AREA_D->set_coor_data(loc, FLAGS, FLOW_SOUTH);

			msg("$ME�b�o���g�a�W�إߤF�e�y�u"BBLU HIB"��"NOR"�v(�n�V)���a�ΡC\n", me, 0, 1);
			
			break;
		}
		// �إߪe�y�a��(�_)
		case "river4":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, RIVER);
			AREA_D->set_coor_icon(loc, BBLU HIB"��"NOR);
			AREA_D->set_coor_data(loc, FLAGS, FLOW_NORTH);

			msg("$ME�b�o���g�a�W�إߤF�e�y�u"BBLU HIB"��"NOR"�v(�_�V)���a�ΡC\n", me, 0, 1);
			
			break;
		}
		// �إ߹D���a��
		case "road":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, ROAD);
			AREA_D->set_coor_icon(loc, "�@");

			if( !arg2 )
				msg("$ME�b�o���g�a�W�إߤF�D�����a�ΡC\n", me, 0, 1);
			else
			{
				msg("$ME�b�o���g�a�W�إߤF�D���è��W���u"+(arg2+NOR)+"�v�C\n", me, 0, 1);
				AREA_D->set_coor_data(loc, "short", kill_repeat_ansi(arg2+NOR));				
			}
			break;
		}
		// �إ߾��٦a��
		case "bridge":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, BRIDGE);
			AREA_D->set_coor_icon(loc, BRED HIR"��"NOR);

			if( !arg2 )
				msg("$ME�b�o���g�a�W�إߤF���١u"+BRED HIR"��"NOR+"�v���a�ΡC\n", me, 0, 1);
			else
			{
				msg("$ME�b�o���g�a�W�إߤF���١u"+BRED HIR"��"NOR+"�v�è��W���u"+(arg2+NOR)+"�v�C\n", me, 0, 1);
				AREA_D->set_coor_data(loc, "short", kill_repeat_ansi(arg2+NOR));				
			}
			break;
		}
		// �إ�����ݩ�
		case "wall":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, WALL);
			AREA_D->set_coor_data(loc, FLAGS, NO_MOVE);

			msg("$ME�b�o���g�a�W�إߤF������ݩʡC\n", me, 0, 1);
			
			break;
		}
		// �إ߹A��
		case "farm":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, FARM);
			AREA_D->set_coor_icon(loc, HIG BGRN"��"NOR);
			
			msg("$ME�b�o���g�a�W�إߤF�A�Сu"HIG BGRN"��"NOR"�v�C\n", me, 0, 1);
			
			break;
		}
		// �إߪ���
		case "pasture":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, PASTURE);
			AREA_D->set_coor_icon(loc, HIY BYEL"��"NOR);

			msg("$ME�b�o���g�a�W�إߤF�����u"HIY BYEL"��"NOR"�v�C\n", me, 0, 1);
			
			break;
		}
		// �إ߸��O
		case "light":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, LIGHT);
			AREA_D->set_coor_icon(loc, HIY"�F"NOR);
			
			msg("$ME�b�o���g�a�W�إߤF���O�u"HIY"�F"NOR"�v�C\n", me, 0, 1);
			
			break;
		}
		// �إߤ���
		case "pool":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, POOL);
			AREA_D->set_coor_icon(loc, HIC"��"NOR);
			
			msg("$ME�b�o���g�a�W�إߤF�����u"HIC"��"NOR"�v�C\n", me, 0, 1);
			
			break;
		}
		// �إ߲D�F
		case "pavilion":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, PAVILION);
			AREA_D->set_coor_icon(loc, HIW"��"NOR);
			
			msg("$ME�b�o���g�a�W�إߤF�D�F�u"HIW"��"NOR"�v�C\n", me, 0, 1);
			
			break;
		}
		// �إߤj��
		case "tree":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, TREE);
			AREA_D->set_coor_icon(loc, HIG"�D"NOR);
			
			msg("$ME�b�o���g�a�W�إߤF�j��u"HIG"�D"NOR"�v�C\n", me, 0, 1);
			
			break;
		}
		// �إ߯��
		case "grass":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, GRASS);
			AREA_D->set_coor_icon(loc, NOR GRN"�s"NOR);
			
			msg("$ME�b�o���g�a�W�إߤF��֡u"NOR GRN"�s"NOR"�v�C\n", me, 0, 1);
			
			break;
		}
		// �إ��J��
		case "statue":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, STATUE);
			AREA_D->set_coor_icon(loc, HIM"��"NOR);
			
			msg("$ME�b�o���g�a�W�إߤF�J���u"HIM"��"NOR"�v�C\n", me, 0, 1);
			
			break;
		}
		// �إ߾i�޳�
		case "fishfarm":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, FISHFARM);
			AREA_D->set_coor_icon(loc, HIB BBLU"��"NOR);
			
			msg("$ME�b�o���g�a�W�إߤF�i�޳��u"HIB BBLU"��"NOR"�v�C\n", me, 0, 1);
			
			break;
		}	
		// ���ø��(���B�z�ݩʪ�����)
		case "icon":
		{
			if( !arg2 )
				return tell(me, "�п�J���T�����O�榡�C\n");
	
			if( arg2 == "-d" )
			{
				AREA_D->delete_coor_icon(loc);
				
				msg("$ME�R���F�o���g�a���ϧΡC\n", me, 0, 1);
				
				break;
			}
			AREA_D->set_coor_icon(loc, kill_repeat_ansi(arg2+NOR));
			
			msg("$ME�b�o���g�a�W�إߤF�u"+arg2+NOR+"�v���ϧΡC\n", me, 0, 1);
		
			break;
		}
		// �إ߹������ж����
		case "room":
		{
			if( !arg2 )
				return tell(me, "�п�J���T�����O�榡�C\n");

			if( arg2 == "-d" )
			{
				AREA_D->delete_coor_data(loc, ROOM);
				AREA_D->delete_coor_data(loc, SPECIAL_EXIT);
				
				msg("$ME�R���F�o���g�a���ж��s����ơC\n", me, 0, 1);
				
				break;
			}
			else if( arg2 == "broken" )
			{
				AREA_D->delete_coor_data(loc);	
				AREA_D->set_coor_data(loc, ROOM, "room");
				msg("$ME�b�o���g�a�W�إߤF�@���L�k�i�J���ж��C\n", me, 0, 1);
				return;
			}
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, SPECIAL_EXIT, arg2);
			AREA_D->set_coor_data(loc, ROOM, "room");

			msg("$ME�b�o���g�a�W�إߤF�P�ж��u"+arg2+"�v���s���C\n", me, 0, 1);
		
			break;
		}
		// �إߤg�a���W��
		case "name":
		{			
			if( !arg2 )
				return tell(me, "�п�J���T�����O�榡�C\n");
			
			if( arg2 == "-d" )
			{
				AREA_D->delete_coor_data(loc, "short");
				
				msg("$ME�R���F�o���g�a���R�W��ơC\n", me, 0, 1);
				
				break;
			}

			AREA_D->set_coor_data(loc, "short", kill_repeat_ansi(arg2+NOR));
				
			msg("$ME�N�o���g�a�R�W���u"+arg2+NOR+"�v�C\n", me, 0, 1);
			
			break;
		}
		// ��L���p
		default:
		{
			tell(me, "�п�J���T���ѼơC\n"+help);
			break;
		}
	}
}
