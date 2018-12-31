/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ls.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : ls ���O
 * Update :
 *  o 2001-10-08 Clode �s�W�Ѽ� -a -l
 *
 -----------------------------------------
 */

#include <feature.h>
#include <daemon.h>
#include <secure.h>
#include <ansi.h>
#include <message.h>

#define	SHOW_ALL	1
#define SHOW_LONG	2
 
inherit COMMAND;
 
string help = @HELP
���O�榡�Rls [<���|�W>]
 
�C�X�ؿ��U�Ҧ����l�ؿ����ɮסM�p�G�S�����w�ؿ��M�h�C�X�Ҧb�ؿ�
�����e�M�ҦC�X���ɮפ��e���Х� * �����O�w�g���J������C
 
�d��:
'ls /' �|�C�X�Ҧ����ڥؿ��U���ɮפΤl�ؿ��C
 
�������O�Rcd�Bpwd�C
HELP;


mapping color_tran =
([
HIC	:	CYN,
HIY	:	YEL,
HIR	:	RED,
WHT	:	WHT,
GRN	:	GRN,
HIG	:	GRN,
]);


private void command(object me, string arg)
{
	int i, j, width, col, len, opt;
	string arg2, dir, color, english, chinese, note, fn, show_files="", show_dir="", auth;
	array file, files;
 	mapping notes = ([]);
 
 	if( !is_command() ) return;
 
 	
 	if( arg )
 	{
 		sscanf(arg, "%s %s", arg, arg2);
 		
 		if( arg[0] == '-' )
 		{
 			if( strsrch(arg, "a") != -1 ) opt |= SHOW_ALL;
 			if( strsrch(arg, "l") != -1 ) opt |= SHOW_LONG;
 			
 			arg = 0;
 		}
 		else if( arg2 && arg2[0] == '-' )
 		{
 			if( strsrch(arg2, "a") != -1 ) opt |= SHOW_ALL;
 			if( strsrch(arg2, "l") != -1 ) opt |= SHOW_LONG;
 			
 			arg2 = 0;
 		}
 	}
 	
	dir = resolve_path(me, arg || arg2);
	
        if( dir[<1] != '/' && file_size(dir) == -2) dir += "/";
	
	if( !sizeof(files = get_dir(dir, -1)) )
	{
		if (file_size(dir)==-2) 	
			tell(me, "���ؿ����L�����ɮסC\n", CMDMSG);
		else 
			tell(me, "�S���o�ӥؿ��C\n", CMDMSG);
			
		return;
	}
	
	dir = dir[0..strsrch(dir,"/",-1)];

	if( file_size(dir+".dir")>0 )
	foreach( note in explode(read_file(dir+".dir"), "\n") )
	{
		if( !sscanf(note, "%s %s", english, chinese) ) continue;
		notes[english] = chinese;
	}
		
	foreach( file in files )
	{ 	
		fn = file[0];
		if( !(opt & SHOW_ALL) && fn[0] == '.' ) files -= ({ file });
		else if( (len=strlen(fn+notes[fn])) > width-1 ) width = len + 1;
	}
	
	col = (query_temp("windowsize/width", me) || 80) / (width+5);
	
 	foreach( file in files )
 	{
 		fn = file[0];
 		note = notes[fn] || "";
 		
 		if( file[1] == -2 )
 		{
 			auth = "d";
 			color = HIC;
 			
 			if( !SECURE_D->valid_check(dir+fn, me, 0, READ) )
 				color = HIR, auth += "-";
 			else
 				auth += "r";
 			
 			if( !SECURE_D->valid_check(dir+fn, me, 0, WRITE) )
 				color = HIR, auth += "-";
 			else
 				auth += "w";
 			
 			if( !SECURE_D->valid_check(dir+fn, me, 0, COMPILE) )
 				color = HIR, auth += "-";
 			else
 				auth += "x";	
 				
 			
 			if( opt & SHOW_LONG )
 			{
 				i++;
 				show_dir += sprintf("%-5s %10s %s %-20s%-20s\n",auth ,"<DIR>", TIME_D->replace_ctime(file[2]),color + fn +"/"+ NOR, (color_tran[color]||"")+(notes[fn]||"")+NOR);
 			}
 			else
 			{
 				show_dir += sprintf("   * %-*s%s", 
					width,
                        		color+fn+"/"+NOR+(color_tran[color]||"")+note+NOR,
					++i%col ? "" : "\n" );
			}
 		}
 		else
		{	
			if( find_object(dir+fn) ) color = HIG;
			else if( fn[<2..] == ".c" ) color = GRN;
			else if( fn[<2..] == ".o" ) color = HIY;
			else if( fn[0] == '.' )	color = WHT;
			else color = NOR;
			
			auth = "-";
			
 			if( !SECURE_D->valid_check(dir+fn, me, 0, READ) )
 				color = HIR, auth += "-";
 			else
 				auth += "r";
 			
 			if( !SECURE_D->valid_check(dir+fn, me, 0, WRITE) )
 				color = HIR, auth += "-";
 			else
 				auth += "w";
 			
 			if( !SECURE_D->valid_check(dir+fn, me, 0, COMPILE) )
 				color = HIR, auth += "-";
 			else if( fn[<2..] == ".c" )
 				auth += "x";
 			else
 				auth += "-";
 				
			if( opt & SHOW_LONG )
			{
				j++;
				show_files += sprintf("%-5s %10s %s %-20s%-20s\n",auth ,file[1]+"",TIME_D->replace_ctime(file[2]),color + fn + NOR, (color_tran[color]||"")+(notes[fn]||"")+NOR);
			}
			else
			{
				show_files += sprintf("%4s %-*s%s", 
					WHT+(file[1]/1024+1),
					width,
                       			NOR+color+fn+NOR" "+(color_tran[color]||"")+note+NOR,
					++j%col ? "" : "\n" );
			}
		}
	}

	// �ڦb�F��...-.-
	if( dir == "/" ) 
		dir += " "NOR CYN+"�ڥؿ�";
	else if( file_size(dir[0..strsrch(dir[0..<2], "/", -1)]+".dir")>0 )
	foreach( note in explode(read_file(dir[0..strsrch(dir[0..<2], "/", -1)]+".dir"), "\n") )
	{
		if( !sscanf(note, "%s %s", english, chinese) ) continue;
		if( english == dir[(strsrch(dir[0..<2], "/",-1)+1)..<2] )
		{
			dir += " "NOR CYN+chinese;
			break;
		}
	}
	
	me->more(kill_repeat_ansi(HIC+"�ؿ�:"+dir+NOR + "\n�@ "+i+" �Ӹ�Ƨ��A"+j+" ���ɮ�\n" + show_dir + (!(opt & SHOW_LONG)&& i%col?"\n":"") + show_files + "\n"));
}
