/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : file.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

/* �P�_�@�ӫ��w������ݪ��ϰ� */ //�����D�వԣ by Clode
string domain_file(string str) {}

/* �P�_�@�ӫ��w���󪺧@�̬��� */
/* �ݩw�q PACKAGE_MUDLIB_STATS  */
string author_file(string str) {} //�]�O�����D�వԣ..���D�Ψӧ�chown? :p by Clode

/* ���s�гy��������w�@�����p�r�� */
/* �ݩw�q PRIVS */
string privs_file(string filename) {
    return filename;
}
