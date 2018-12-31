/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : valid_readwrite.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

#include <daemon.h>
#include <secure.h>

/*
�W�١G
        valid_read - �ˬd�@�ӤH�O�_���vŪ���@���ɮסC
�y�k�G
        int valid_read( string file, object user, string func );

        ��� valid_read( �r�� �ɮ�, ���� �ϥΪ�, �r�� �禡 );
�Ϊk�G
        �C����@�Ӫ������Ū���ɮ׮ɡA�X�ʵ{���|�I�s�D�_���� valid_read()
        �H�ˬd�O�_���\Ū���C���禡���ѼƬ��ɮצW�١B����Ū�����ɪ�����B�I
        �s���禡�W�١C�p�G valid_read() �Ǧ^�D�s�ȡA�h��\Ū���C
�ѦҡG
        valid_write
�@�̡G
        Tim Hollebeek  Beek@ZorkMUD, Lima Bean, IdeaExchange, and elsewhere
½Ķ�G
        spock@muds.net          2000.May.21.    v22 ��
*/
int valid_read(string file, object from, string func)
{
	return SECURE_D->valid_check(file, from, func, READ);
}

/*
�W�١G
        valid_write - �ˬd�@�Ӫ���O�_���v�g�J�@���ɮסC
�y�k�G
        int valid_write( string file, object ob, string func );

        ��� valid_write( �r�� �ɮ�, ���� ����, �r�� �禡 );
�Ϊk�G
        �C����@�Ӫ�����ռg�J�ɮ׮ɡA�X�ʵ{���|�I�s�D�_���󤤪�
        valid_write() �H�ˬd�O�_���\�g�J�C���禡���ѼƬ��ɮצW�١B���ռg�J
        ���ɪ�����B�I�s���禡�W�١q�q�`�O�Q�ϥΪ��~���禡�r�C�p�G
        valid_write() �Ǧ^�D�s�ȡA�h��\�g�J�C
�ѦҡG
        valid_read
�@�̡G
        Tim Hollebeek  Beek@ZorkMUD, Lima Bean, IdeaExchange, and elsewhere
½Ķ�G
        spock@muds.net          2000.May.21.    v22 ��
*/
int valid_write(string file, object from, string func)
{
	return SECURE_D->valid_check(file, from, func, WRITE);
}
