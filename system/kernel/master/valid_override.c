/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : valid_override.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

#include <object.h>

/*
�W�١G
        valid_override - ���� efun:: ���γ~�C
�y�k�G
        int valid_override( string file, string efun_name, string mainfile );
 
        ��� valid_override( �r�� �ɮ�, �r�� �~���禡�W, �r�� �D�ɮ� );
�Ϊk�G
        file �O�өI�s��ڤW�ҳB���ɮסFmainfile �O���sĶ���ɮסq�]��
        #include �����Y�A��̥i��|���P�r�C
 
        �b�D�_���� (master.c) ���[�W valid_override() �i�H���� efun:: �o��
        �e�m�y�k (prefix) ���γ~�C�C�����X�ʵ{�����սsĶ�e���[�W efun:: ��
        �禡�ɡA�|�I�s�D�_���󤤪� valid_override()�C�p�G valid_override()
        �Ǧ^ 0�A�h�sĶ���ѡC�ҥH�@�ǸT��� efun:: �e�m�y�k�W�׭�@�Ϊ��~��
        �禡�Avalid_override() �i�H���Ѥ@�Ӥ�k�ק復�̪��@�ΡC
 
        �p�G�z�Ʊ�ϥέ�� 3.1.2  �� efun:: ���@�ΡA�u�ݭn�b�D�_����
        master.c ���[�W�U���o��G
 
        int valid_override(string file, string efun) { return 1; }
 
        ���U�O�@�ӭ�����h�� valid_override() �d�ҡG
        �qĶ���G���d�һP 21 ���õL���P�A���O�ثe�w�h�F mainfile �Ѽơr

        int valid_override( string file, string name )
        {
                if (file == "/adm/obj/simul_efun") {
                        return 1;
                }

                if (name == "destruct")
                        return 0;
                if (name == "shutdown")
                        return 0;
                if (name == "snoop")
                        return 0;
                if (name == "exec")
                        return 0;

                return 1;
        }
�ѦҡG
        valid_object, function_exists
�@�̡G
        Tim Hollebeek  Beek@ZorkMUD, Lima Bean, IdeaExchange, and elsewhere
½Ķ�G
        spock@muds.net          2000.May.21.    v22 ��

*/

int valid_override(string file, string name, string mainfile)
{
	
	if( file[0..24] == SIMUL_EFUN_OB[0..24] ) return 1;

	switch(name)
	{
		case "memory_summary":
                case "destruct"        :
		case "shutdown"	:
		case "exec"	:	return 0;
		default		:	return 1;
	}
}
