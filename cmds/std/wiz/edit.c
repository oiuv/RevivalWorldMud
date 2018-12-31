/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : edit.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-6
 * Note   : edit ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>


inherit COMMAND;

string help = @HELP
���O�榡 : edit <�ɦW>, edit here

�Q�Φ��@���O�i�����b�u�W�s���ɮסC
�b edit �U�`�Ϊ����O���[��:

�� �W�[(a), ���N(c) �Ұ� 'edit mode'.

�n���^ 'command mode', �h�b�@�檺�_�I��J '.' �A�� 'enter' ��.

$ - �o�ӲŸ��b command mode �û����N��̫�@�檺�渹.
:�Ʀr(n),(m)p ---- �C�X�ɮפ��e, n ���}�l�渹, m �������渹. �p�G�S��
                   m,n �h�C�X �{�b �Ҧb�檺���e. �p�G 'p' �e�u���@�ӼƦr
                   �h�C�X�Ʀr�ҫ��渹�����e.
:�Ʀr(n),(m)d ---- �R���� n �� m �檺���e. �p�G 'd' �e�u���@�ӼƦr, �h
                   �R���Ʀr�ҫ��渹�����e.
:�Ʀr(n)a     ---- �b n ���[�J�F��. ��O�i�J 'edit mode'
:�Ʀr(n)i     ---- �b n ��e�[�J�F��. ��O�i�J 'edit mode'
:�Ʀr(n)c     ---- �ק�� n �檺���e. ��O�i�J 'edit mode', ����
                   �A�ҥ��J���F�賣�|���N�� n ��즳�����e.
              [��: �o�ӫ��O�� �u���ġv�d��O�@��, �W�L�@��ä��|���A�ק�
                     n+1 �檺�F��.]
�p�G�S�����w�渹, �h�H�ثe�Ҧb�欰��.
:x            ---- �s�ɨ����} edit�C
:q            ---- ���} edit. [��: �����O�u�A�Ω���ɮפw�g�x�s�L��, �άO�ɮ�
                   �S���Q��ʹL.]
:Q            ---- �p�G�ɮ׳Q�ʹL, ���A���Q�g�J�Q��ʪ����e, �A�i�H�γo�ӫ�
                   �O���} edit.
:!<cmd>       ---- �b edit �U����Y�� mud ���O.
HELP;
private void exit_edit()
{
        this_player(1)->show_prompt();
}

private void command(object me, string arg)
{
	if( !is_command() ) return;
	
	if( !arg ) 
		return tell(me, "���O�榡�Redit <�ɦW>\n", CMDMSG);

	arg = resolve_path(me, arg);

	switch( file_size(arg) )
	{
		case -1:
			tell(me, "�}�l�s��s�ɮ� "+arg+"�C\n", CMDMSG);
			break;
		case -2:
			return tell(me, "�L�k�s��@�ӥؿ��C\n", CMDMSG);
		default:
			tell(me, "�}�l�s�����ɮ� "+arg+"�C\n", CMDMSG);
	}
        set("current_work/file", arg, me);
        ed(arg, "exit_edit");
}
