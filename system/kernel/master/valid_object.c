/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : valid_object.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

/* 
�W�١G
        valid_object - ���A�౱��O�_�n���J�Y�Ӫ���C
�y�k�G
        int valid_object( object obj );

        ��� valid_object( ���� ���� );
�Ϊk�G
        ���J�@�Ӫ��󤧫�A�X�ʵ{���|�H�s�гy�����󬰰ѼơA�I�s�D�_����
        (master object) �� valid_object()�C�p�G valid_object() �s�b�A�ö�
        �^ 0�A�h�|�R��������èϸ��J���󪺥~���禡 (efun) ���Ϳ��~��X�C�p
        �G���禡���s�b�άO�Ǧ^ 1�A�h���J���󪺹L�{�N�򥭱`�@�ˡC���禡�I�s
        �ɡA�Ӫ����٨S�����|�������{���X�A�]�A create()�b���A�ҥH�Ӫ���
        ���F file_name(obj) ���~�A�S���h�֪F�観�ġC
�ѦҡG
        valid_override
�@�̡G
        Tim Hollebeek  Beek@ZorkMUD, Lima Bean, IdeaExchange, and elsewhere
½Ķ�G
        spock@muds.net          2000.May.21.    v22 ��
*/

int valid_object( object obj )
{
	return 1;
}