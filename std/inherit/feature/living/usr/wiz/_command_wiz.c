/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _command_wiz.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-4
 * Note   : �Ův���O�t��
 * Update :
 *  o 2002-07-08 Clode ��z action �{���X
 *
 -----------------------------------------
 */

#include <daemon.h>

private nosave int commands_authority;

/* �Ұʤ��ʩʽ� */
nomask void enable_interactive()
{
	if( !interactive(this_object()) || !is_daemon(previous_object()) )
		error(sprintf("���ʥؼбҰ��v������ %O(Call Stack)\n", call_stack(1)));

	enable_wizard();

	/* �V�w���t�ΤΫ��O�t�ή������O�v�� */	
	commands_authority = SECURE_D->level_num(this_object()->query_id(1));
	
	/* �V Channel_D �n���W�� */
	CHANNEL_D->register_channel(this_object(), query("channels"));
}

int query_commands_authority()
{
	return commands_authority;
}
