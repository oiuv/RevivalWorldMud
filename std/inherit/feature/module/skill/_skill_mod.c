/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _skill_mod.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-04
 * Note   : skill module
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <skill.h>
#include <feature.h>

inherit DATABASE;


nomask varargs string query_id(int raw)
{
	return undefinedp(raw) ? query("id") : remove_ansi(query("id"));
}
nomask varargs string query_name(int raw)
{
	return undefinedp(raw) ? query("name") : remove_ansi(query("name"));
}
nomask varargs string query_idname(int raw)
{
	return undefinedp(raw) ? query_name()+"("+ansi_capitalize(query_id())+")" : query_name(1)+"("+capitalize(query_id(1))+")";
}
nomask int query_skill_type()
{
	return query("type");
}
nomask int query_skill_note()
{
	return query("note");
}

// �W�[�Y��
void add_title(object ob, int level)
{
	string title = query("title/"+level);
	
	if( stringp(title) )
		ob->add_title(title);	
}


// �R���Y��
void remove_title(object ob, int level)
{
	string title = query("title/"+level);
	
	if( stringp(title) )
		ob->remove_title(title);		
}
