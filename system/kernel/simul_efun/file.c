/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : file.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-27
 * Note   : included by Simul_Efun Object
 * Update :
 *
 *
 -----------------------------------------
 */

private string *unremovable_dir_list = 
({
    "/",
    "/system/",
    "/system/daemons/",
    "/system/kernel/",
    "/system/kernel/master/",
    "/system/kernel/simul_efun/",
    "/system/object/",

    "/std/",
    "/std/feature/",
    "/std/inherit/",
    "/std/module/",

    "/cmds/",
    "/cmds/std/adm/",
    "/cmds/std/adv/",
    "/cmds/std/arch/",
    "/cmds/std/wiz/",
    "/cmds/std/guest/",
    "/cmds/std/ppl/",
    "/cmds/std/",

    "/log/",
    "/log/command/",
    "/log/MudOS/",
    "/log/system/",

    "/data/",
    "/data/user/",
    "/data/backup/",

    "/city/",
    "/include/",
    "/wiz/",
    "/doc/",

});

#define SUCCESS		0
#define FAIL		1

array f_rmtree(string dir, array &res)
{	
	if( dir[<1] != '/' ) dir += "/";
	
	if( file_size(dir) != -2 ) return 0;
	if( member_array(dir, unremovable_dir_list) != -1 ) return 0;

	foreach(string file in get_dir(dir))
	{
		if( file_size(dir+file) == -2 )
			f_rmtree(dir+file, &res);
		else if( rm(dir+file) )
			res[SUCCESS] += ({ dir+file });
		else
			res[FAIL] += ({ dir+file });
	}
	
	if( rmdir(dir) )
		res[SUCCESS] += ({ dir });
	else
		res[FAIL] += ({ dir });
}

array rmtree(string dir)
{
	array res = allocate(2, allocate(0));
	
	f_rmtree(dir, &res);
	
	return res;
	
}	

array f_cptree(string dir, string dst, array &res)
{
	if( dir[<1] != '/' ) dir += "/";
	
	if( file_size(dir) != -2 ) return 0;
	
	if( file_size(dst) != -2 )
	{
		if( mkdir(dst) )
			res[SUCCESS] += ({ dst });
		else
			res[FAIL] += ({ dst });
	}
	
	foreach(string file in get_dir(dir))
	{
		if( file_size(dir+file) == -2 )
			f_cptree(dir+file, dst+file, ref res);
		else if( cp(dir+file, dst+file) )
			res[SUCCESS] += ({ dst+file });
		else
			res[FAIL] += ({ dst+file });
	}
}

array cptree(string dir, string dst)
{
	array res = allocate(2, allocate(0));
	
	f_cptree(dir, dst, &res);
	
	return res;
	
}
// ��޸��y�k�ѪR
string *single_quote_parse(string p)
{
	int i, j, size;
	string *ret = allocate(0), *tmp = explode(p, " ");

	size = sizeof(tmp);

	for(i=0;i<size;i++,j++)
	{
		if( tmp[i][0] == '\'' )
		{
			ret += ({ tmp[i][1..] });

			while( ++i < size )
			{
				ret[j] += " "+tmp[i];

				if( tmp[i][0] && tmp[i][<1] == '\'' )
				{
					ret[j] = ret[j][0..<2];
					break;
				}
			}
		}
		else ret += ({ tmp[i] });
	}

	return ret - ({ "" });
}



#define FILE_TYPE       ({ "", ".c", ".o", ".h" })

void intact_file_name(string ref filename)
{
	if( !filename ) return;
	
	foreach( string type in FILE_TYPE )
	{
		if( file_size(filename+type) >= 0 )
		{
			filename += type;
			break;
		}
	}
}

int file_exists(string filename)
{
	if( !stringp(filename) ) return 0;
	
	intact_file_name(ref filename);
	
	return file_size(filename) > 0;
}

// temp
int file_exist(string filename)
{
	return file_exists(filename);
}

