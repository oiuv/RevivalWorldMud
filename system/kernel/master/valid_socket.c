/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : valid_socket.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

/*
�W�١G
	valid_socket - �O�@ socket �~���禡 (efunctions)�C
�y�k�G
	int valid_socket( object caller, string function, mixed array info );

	��� valid_socket( ���� �I�s��, �r�� �禡, �V�X �}�C ��T );
�Ϊk�G
	�C�@�� socket �~���禡���椧�e�A�|���I�s valid_socket()�C�p�G
	valid_socket() �Ǧ^ 0�A�h�� socket �~���禡���ѡC�Ϥ��A�Ǧ^ 1 �h��
	�\�C�Ĥ@�ӰѼ� caller �O�I�s�� socket �~���禡������C�ĤG�ӰѼ�
	function �O�Q�I�s�� socket �~���禡�W�١q�Ҧp socket_write() ��
	socket_bind()�r�C�ĤT�ӰѼƬO�@�Ӹ�T���}�C�C�p�G����T���ܡA�o��
	�}�C���Ĥ@�Ӥ��� (element) �O�Q�ѦҪ��ɮױԭz�� (file descriptor
	being referenced)�C�ĤG�Ӥ����O�֦��� socket ������q���󫬺A�r�C
	�ĤT�Ӥ����O�� socket �����ݦ�} (address of the remote end)�q�r��
	���A�r�C�ĥ|�Ӥ����O�P socket ������ (associated) �s���𸹽X (port
	number)�C
�@�̡G
	Tim Hollebeek  Beek@ZorkMUD, Lima Bean, IdeaExchange, and elsewhere
½Ķ�G
	spock@muds.net          2000.May.21.    v22 ��
*/
int valid_socket(object caller, string func, mixed *info)
{
	return 1;
	// caller �b login_ob �̥i�ण�|�ǤJ����
	if( !objectp(caller) )
		return 0;

	return is_daemon(caller) || is_command(caller) || caller->is_login_ob();
}
