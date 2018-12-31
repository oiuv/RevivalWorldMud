/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _object_mod.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-14
 * Note   : object module
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#define PRODUCT_MODULE_PATH		"/std/module/product/"

// ������O��
nosave mapping actions;

int is_module_product()
{
	return 1;
}

// �d�� Module Filename
string query_module_file()
{
	return query_temp("module_file");
}

string query_module()
{
	return query_temp("module");
}

// �� Action File �d�ߥ\����
varargs mapping query_action_info()
{
	return fetch_variable("action_info", load_object(query_module_file()));
}

// �Ұ� Action
int enable_action()
{
	mapping action_info = query_action_info();
	
	if( !mapp(action_info) ) return 0;
	
	// ���J Actions
	actions = action_info["action"];

	return 1;
}

void init(object ob)
{
	object module = load_object(query_module_file());
	
	if( objectp(module) )
		module->init(ob, this_object());
}

void leave(object ob)
{
	object module = load_object(query_module_file());
	
	if( objectp(module) )
		module->leave(ob, this_object());
}

mapping query_product_info()
{
	return fetch_variable("product_info", load_object(query_module_file()));
}

// �� Virtual_D �I�s�ðe�J��������W�� file
varargs void create(string file, string type)
{
	string module_file;

	if( !stringp(file) || !stringp(type) ) return;
	
	module_file = PRODUCT_MODULE_PATH+type+"/module.c";

	// �b Virtual_D �̫K�ˬd�O�_�ʤ� Action File
	if( !file_exist(module_file) )
		error("�ݭn Module File �� "+module_file+"�C\n");
	
	// ���� Action File
	set_temp("module_file", module_file);
	set_temp("module", type);

	// �Y�O�Ĥ@���Хߪ���, �h�]�w�򥻸��
	if( !this_object()->restore(file) ) 
	{
		mapping info = query_product_info();

		this_object()->set_idname(info["id"], info["name"]);
	}
	else
		enable_action();
}
