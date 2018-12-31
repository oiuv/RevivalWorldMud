#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	�з� children ���O
HELP;

private varargs string memory_expression(int m, int flag)
{ 
        float mem = to_float(m);
        if( mem<1024 )
        {
                if(flag) return sprintf("%.2f  Bytes", mem);
                else return sprintf("%7.2f  Bytes", mem);
        }
        if( mem<1024*1024 )
        {
                if(flag) return sprintf("%.2f KB", (float)mem / 1024);
                return sprintf("%7.2f KB", (float)mem / 1024);
        }
        if(flag) return sprintf("%.3f MB", (float)mem / (1024*1024));
        return sprintf("%7.3f MB", (float)mem / (1024*1024));
}

private void command(object me, string arg)
{
	object ob;
	int flag, mem;
	string item, msg;

	if( !is_command() ) return;

	if( !arg )
		return tell(me, "���O�榡: children -c | [-e] [-n] [-q <����>] <�ɮשΪ���> \n", CMDMSG);
	if( sscanf(arg, "-e %s", arg) == 1 ) flag = 1;
	if( sscanf(arg, "-n %s", arg) == 1 ) flag = 2;
	if( sscanf(arg, "-q %s %s", item, arg) == 2 ) flag = 3;
	if( arg == "-c" )
	{
		mixed object_child = allocate(0);
		mixed unique_child = unique_array(objects(), (: base_name($1) :));
		int i, size;
		foreach(object *child in unique_child)
		{
			mem = 0;
			foreach(ob in child)
			{
				mem += memory_info(ob);
			}
			if( (size = sizeof(child)) > 2)
				object_child += ({ ({ base_name(child[0]), size, mem }) });
		}
		object_child = sort_array(object_child, (: $1[1] < $2[1] ? 1:-1 :));
		msg = "�ƻs����ƶq�ƦW�G\n";
		foreach(array *info in object_child)
		{
			i++;
			msg += sprintf("%2d. %-35s -> %4d (%s)\n", i, info[0], info[1], memory_expression(info[2]));
		}
		me->more(msg);
		return;
	}

	ob = present(arg);
	if( objectp(ob) ) arg = base_name(ob);
	else arg = resolve_path(me, arg);
	{
		object env, clone_ob, *child = children(arg);
		int i, num;
		string sid;

		if( !sizeof(child) ) return tell(me, "���� " + arg + " �S������ƻs����C\n");
		msg = arg + " ���ƻs����p�U�G\n";
		foreach(clone_ob in child)
		{
			if( sscanf(sprintf("%O\n", clone_ob), "%*s#%d", num) != 2 ) num = -1;
			sid = (num < 0 ? "��l����": "#" + num);
			env = environment(clone_ob);
			i++;
			if( flag == 1 )
				msg += sprintf("%3d. %8s\t -> %O\n", i, sid, env);
			else if( flag == 2 )
				msg += sprintf("%3d. %8s\t -> %s\n", i, sid, clone_ob->short());
			else if( flag == 3 )
				msg += sprintf("%3d. %8s\t -> %O\n", i, sid, query_temp(item, clone_ob));
			else
				msg += sprintf("%3d. %8s\n", i, sid);
		}
	}
	me->more(msg);
	return;
}
