/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _message_npc.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-6
 * Note   : NPC�T���~����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

varargs void receive_msg(string msg, string classes);

/* �� tell_object(), write() ���I�s */
void catch_tell(string msg, string classes)
{
	// �_���@���T��
	if( query_temp("disable/msg") ) return;
	
	// �� receive_msg �Τ@�B�z�T�����e
	receive_msg(msg, classes);
}

