/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : creator.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-1-18
 * Note   : �Ѩt�Φ۰ʲ��ͥ������ɮ�
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
string header = @LEAD
/* This program is a part of RW mudlib 
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : %^FILENAME%^
 * Author : �� creator ����
 * Date   : 
 * Note   : Include File.
 * Update :
 *  o 2005-00-00
 *
 -----------------------------------------
 */

LEAD;

private int valid_file_name(string file)
{
	foreach( int a in file )
	{
		if( !(a>='0' && a<='9') && !(a>='A' && a<='Z') && !(a>='a' && a<='z') && a!='_' )
			return 0;
	}
	
	return 1;
}


private void create_feature_include()
{
	string sf, file = terminal_colour(header, ([ "FILENAME":"feature.h" ]));
	string *files = deep_path_list("/std/inherit/feature/");
	
	foreach( string f in files )
	{
		if( f[<2..<1] != ".c" ) continue;

		sf = f[strsrch(f,"/",-1)+2..<3];
		
		if( valid_file_name(sf) )
			file += sprintf("%s%-40s%s", "#define ",upper_case(sf)," \""+f+"\"\n");
	}
	
	write_file("/include/feature.h", file+"\n", 1);
}	


private void create_inherit_include()
{
	
	string sf, file = terminal_colour(header, ([ "FILENAME":"inherit.h" ]));
	string *files = deep_path_list("/std/inherit/standard/");
	
	foreach( string f in files )
	{
		if( f[<2..<1] != ".c" ) continue;

		sf = f[strsrch(f,"/",-1)+1..<3];
		
		if( valid_file_name(sf) )
			file += sprintf("%s%-40s%s", "#define ",upper_case(sf)," \""+f+"\"\n");
	}
	
	write_file("/include/inherit.h", file+"\n", 1);
}

private void create_daemon_include()
{
	string sf, file = terminal_colour(header, ([ "FILENAME":"daemon.h" ]));
	
	foreach( string f in get_dir("/system/daemons/") )
	{
		if( f[<2..<1] != ".c" ) continue;
		
		f = "/system/daemons/"+f;
		
		if( file_size(f) < 1 ) continue;
			
		sf = f[strsrch(f,"/",-1)+1..<3];
		
		
		if( valid_file_name(sf) )
			file += sprintf("%s%-40s%s", "#define ",upper_case(sf)," \""+f+"\"\n");
	}
	
	write_file("/include/daemon.h", file+"\n", 1);
}

private void create_condition_include()
{
	string sf, file = terminal_colour(header, ([ "FILENAME":"condition.h" ]));
	
	foreach( string f in get_dir("/std/module/condition/") )
	{
		if( f[<2..<1] != ".c" ) continue;
		
		f = "/std/module/condition/"+f;
		
		if( file_size(f) < 1 ) continue;
			
		sf = f[strsrch(f,"/",-1)+1..<3];
		
		
		if( valid_file_name(sf) )
			file += sprintf("%s%-40s%s", "#define ",upper_case(sf)," \""+f[0..<3]+"\"\n");
	}
	
	write_file("/include/condition.h", file+"\n", 1);
}

void create_all_include()
{
	// �۰ʲ��� feature.h
	create_feature_include();
	
	// �۰ʲ��� inherit.h
	create_inherit_include();
	
	// �۰ʲ��� daemon.h
	create_daemon_include();
	
	// �۰ʲ��� condition.h
	create_condition_include();
}	

void create()
{
	create_all_include();
}

string query_name()
{
	return "���Y�ɦ۰ʫظm�t��(CREATOR)";
}
