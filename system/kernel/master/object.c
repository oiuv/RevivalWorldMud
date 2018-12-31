/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : object.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

// When an object is destructed, this function is called with every
// item in that room.  We get the chance to save users from being destructed.
/* ���ҾD�췴����, �Ҧ����b���󳣷|�I�s���禡 */
void destruct_env_of(object ob)
{
	if( interactive(ob) )
	{
		tell(ob, "�A�Ҧb���Ŷ������F�A�A�{�b���B�b�ŵ��e�z�����C\n", "ENVMSG");
		ob->move(VOID_OB);
	}
}