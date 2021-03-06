/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _message_npc.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-6
 * Note   : NPC訊息繼承檔
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

varargs void receive_msg(string msg, string classes);

/* 由 tell_object(), write() 等呼叫 */
void catch_tell(string msg, string classes)
{
	// 斷絕一切訊息
	if( query_temp("disable/msg") ) return;
	
	// 由 receive_msg 統一處理訊息內容
	receive_msg(msg, classes);
}

