/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : flag.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

/* �B�z�X�ʵ{���Ұʮɩҫ��w�� mudlib �S�w�X�� */
/* �Ψ� debug... ���ɵL�ݸ��J */
void flag( string arg )
{
	string file, func;
	if( previous_object() ) return;
	
	if( sscanf(arg,"load:%s %s",file,func) )
		printf( "MASTER: "+file+"'s "+func+"() �^��"+call_other(file, func) );
	else
		printf( "MASTER: �����X��\n" );
	return;
}