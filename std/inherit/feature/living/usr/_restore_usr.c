/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _restore_usr.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-28
 * Note   : �ϥΪ̸��J
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */
 
nomask varargs string *set_idname(string id, string name);
nomask varargs int restore(int i);

nomask int restore_usr(string id)
{
	return arrayp(set_idname(id)) && restore();
}
