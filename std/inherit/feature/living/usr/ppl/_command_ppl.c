/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _command_ppl.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-4
 * Note   : ���a���O�t��
 * Update :
 *  o 2001-07-08 Clode ��z action �{���X
 *
 -----------------------------------------
 */

#include <daemon.h>

/* �Ұʤ��ʩʽ� */
nomask void enable_interactive()
{
	if( !interactive(this_object()) ) destruct(this_object());
	
	CHANNEL_D->register_channel(this_object(), query("channels"));
}
