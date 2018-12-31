/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : valid_bind.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

/*
�W�١G
        valid_bind - �P�_�@�ӫ��w���禡���� (function pointer) �O�_�൲�X
                     (bind) ��@�Ӫ���W�C
�y�k�G
        int valid_bind( object doer, object owner, object victim );

        ��� valid_bind( ���� �@�Ϊ�, ���� �֦���, ���� ���`�� );
�Ϊk�G
        �� doer ���ըϥ� bind() �~���禡���X owner �� victim ���󪺨禡��
        �СA�|�I�s���禡�C�p�G���禡�Ǧ^ 0�A�N�T��X�C
�ѦҡG
        bind
�@�̡G
        Tim Hollebeek  Beek@ZorkMUD, Lima Bean, IdeaExchange, and elsewhere
½Ķ�G
        spock@muds.net          2000.May.21.    v22 ��
*/

int valid_bind(object doer, object owner, object victim) 
{
        return 1;
}
