/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : valid_shadow.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

/*

        valid_shadow - ������Ǫ���i�H�Q��v (shadow)�C
�y�k�G
        int valid_shadow( object ob );

        ��� valid_shadow( ���� ���� );
�Ϊk�G
        ��@�Ӫ�����է�v ob �ɡq�ϥ� shadow() �~���禡 (efun)�r�A�|�I�s
        �D�_���󤤪� valid_shadow() �禡�C�@�Ӫ������ӧ@�����禡���ѼơC��
        ����O previous_object() ���է�v������C�p�G�����v�A
        valid_shadow() ���Ǧ^ 0�A�ɭP shadow() �I�s���ѨöǦ^ 0�C�p�G
        valid_shadow() �Ǧ^ 1�A�h���\��v�C
�ѦҡG
        shadow, query_shadowing
�@�̡G
        Tim Hollebeek  Beek@ZorkMUD, Lima Bean, IdeaExchange, and elsewhere
½Ķ�G
        spock@muds.net          2000.May.21.    v22 ��
*/
int valid_shadow(object ob)
{
        return 1;
}
