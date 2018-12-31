/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : master.c
 * Author : Clode@RevivalWorld
 * Date   : 2000-12-18
 * Note   : �D�_����
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <object.h>

//-----------------------------------------------------------------------------

#include "master/flag.c";/* �Ψ� Debug , ���ɵL�ݸ��J */
#include "master/compile_object.c";/* �ΨӸ��J MudOS �������� */
#include "master/crash.c";/* �ΨӳB�z Crash �ɪ���污�p */
#include "master/preload.c";/* �w�����J��l�ưʧ@ */
#include "master/error.c";/* �B�z���~�T�����禡 */
#include "master/file.c";/* ���� domain_file , author_file , privs_file */
#include "master/ed.c";/* ���� Edit ���@�ǳ]�w */
#include "master/parse.c";/* ��k(�襤��Mud�Ө����G�S���γB?) */
#include "master/object.c";/* ���ҾD�췴����, �Ҧ����b���󳣷|�I�s���禡 */

//-----------------------------------------------------------------------------

#include "master/valid_asm.c";
#include "master/valid_author.c";
#include "master/valid_bind.c";/* �ˬd�禡���лP���󪺵��X */
#include "master/valid_compile_to_c.c";/* �P�_�O�_�i�H�� LPC->C �sĶ */
#include "master/valid_domain.c";
#include "master/valid_hide.c";/* �ˬd���ܯ�O */
#include "master/valid_link.c";/* ���� link �~���禡���γ~ */
#include "master/valid_object.c";/* ���A�౱��O�_�n���J�Y�Ӫ��� */
#include "master/valid_override.c";/* ���� efun:: ���γ~ */
#include "master/valid_readwrite.c";/* �ˬd�O�_���vŪ���g�J�ɮ� */
#include "master/valid_save_binary.c";/* ����@�Ӫ���O�_�i�H�x�s���w���J���{�� */
#include "master/valid_shadow.c";/* ������Ǫ���i�H�Q��v */
#include "master/valid_socket.c";/* �O�@ socket �~���禡 */
#include "master/valid_database.c";

//-----------------------------------------------------------------------------

/* �^�ǳs�u���� */
//        connect() �Ǧ^������A�|��@��l�ϥΪ̪��� (initial user object)�C
//        �Ъ`�N�A����A�i�H�ϥ� exec() �~���禡�N���ܨϥΪ̪���C

private object connect( int port )
{
        object ob;
        
        if( !catch(ob = new(LOGIN_OB) ) )
        	return ob;
        else
        	printf("\n�n���{�����b�ק襤�A�еy��A�աC\n\n");
}

private void create()
{
        printf("Master Object(/system/kernel/master.c) ���J���\\n");
}
