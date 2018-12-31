/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : virtual_d.c
 * Author : Cookys@RevivalWorld
 * Date   : 2000-12-18
 * Note   : ����������F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <object.h>
#include <daemon.h>
#include <citydata.h>
#include <inherit.h>

void create()
{
	if( clonep() ) 
		destruct(this_object());
}

nomask varargs object compile_object(string file, int cloning)
{
	string filename, option;
	
	if( !file || !file[0] || file[<2..<1] == __SAVE_EXTENSION__ ) return 0;

	// ������������
	if( sscanf(file, CITY_ROOT"%*s/%*d/%s/%s", option, filename) == 4 )
	{
		switch(option)	
		{
			// �����ж���������
			case "room":
			{
				string type;
				
				// �ж������\���ͽƻs����
				if( cloning || (sscanf(filename, "%*d_%*d_%*d_%s", type) != 4 && sscanf(filename, "%*d_%*d_%s", type) != 3) ) 
					return 0;
				
				return new(STANDARD_MODULE_ROOM, file, type);
			}
			default:
				return 0;
		}
	}

	// �ӫ~
	if( sscanf(file, "/product/%s/%*d", option) == 2 )
	{
		if( !file_exists(file) && previous_object(1) != find_object("/std/module/room/research") )
			return 0;

		return new("/std/module/product/"+option+"/object.c", file, option);
	}

	// �����~(souvenir)
	if( sscanf(file, "/product/souvenir_%s", option) == 1 )
		return new("/obj/systemproducts/souvenir.c", file, option);
				
	// �Ҥu
	if( sscanf(file, "/npc/%*d") )
	{
		if( cloning ) return 0;

		return new(STANDARD_MODULE_NPC, file);
	}

	// �ϥΪ̪���
	if( !cloning && sscanf(file, "/data/user/%s/%s", option, filename) == 2 )
	{
		// �ĥθ��Y�Ԫ��ˬd
		if( !is_english(filename) || filename[0..0] != option ) return 0;

		if( SECURE_D->is_wizard(filename) && (call_stack(1)[3]==load_object(WIZ_LOGIN_D) || (call_stack(1)[3]==load_object(LOGIN_D))) )
			return new(WIZ_OB);
		
		return new(PPL_OB);
	}


	
	return 0;
}

string query_name()
{
	return "��������t��(VIRTUAL_D)";
}
