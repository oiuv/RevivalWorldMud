/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : valid_hide.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

/*
�W�١G

       valid_hide - ���\�θT��@�Ӫ���㦳���� (hide) �άݨ���ܪ��󪺯�O�C
�y�k�G
	int valid_hide( object ob );

	��� valid_hide( ���� ���� );
�Ϊk�G
	�b�D�_���� (master.c) �[�W valid_hide �i�H������㦳�ۧڰ��ܩM��
	���L���ܪ��󪺯�O�C��@�Ӫ���ϥ� set_hide() �禡�H�i�J���ܪ��A
	�ɡA������N�� valid_hide() ���ߤ@�ѼƩI�s���禡�C���\���������
	�����Ǧ^ 1�A�Ϥ��h�Ǧ^ 0�C��@�Ӫ���Q�ݨ���ܪ��A������ɡA�]�H�P
	�˪��覡�I�s���禡�C
�ѦҡG
	set_hide
�@�̡G
	Tim Hollebeek  Beek@ZorkMUD, Lima Bean, IdeaExchange, and elsewhere
½Ķ�G
	spock@muds.net          2000.May.21.    v22 ��
*/
int valid_hide(object ob) 
{
	//"/system/daemons/channel_d.c"->channel_broadcast("sys",sprintf("1.%O\n this_player():%O\n this_player(1):%O\n",call_stack(1),this_player(),this_player(1)));	
	//"/system/daemons/channel_d.c"->channel_broadcast("sys",sprintf("1.%O\n\n",find_player("cookys")));
	//if( call_stack(1)[<1]==this_object() ) return 1;
	if( this_player() && wizardp(this_player()) ) return 1;
	return 0;
}
