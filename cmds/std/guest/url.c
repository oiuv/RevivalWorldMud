
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP

���O�榡 : url <URL: http://abcd.com>
            
���o�Ӻ��}������
HELP;

private string html_remove(string str)
{
	string head, tag, tail;

	if( !str ) return "";
	while(sscanf(str, "%s<%s>%s", head, tag, tail) == 3)
	{
		switch(lower_case(tag))
		{
		case "br":	tag = "\n"; break;
		case "p":	tag = "\n\n"; break;
		default:	tag = "";
		}
		str = head + tag + tail;
	}
	str = replace_string(str, "\n\r", "\n");
	str = replace_string(str, "\t", "");
	while(strsrch(str, " \n") != -1)
		str = replace_string(str, " \n", "\n");
	while(strsrch(str, "\n\n\n") != -1)
		str = replace_string(str, "\n\n\n", "\n\n");
	str = replace_string(str, "&amp;", "&");
	str = replace_string(str, "&lt;", "<");
	str = replace_string(str, "&gt;", ">");
	str = replace_string(str, "&nbsp;", " ");
	str = replace_string(str, "&copy;", "(C)");
	
	return str;
}
void get_html(object me, string str)
{
	tell(me, html_remove(iconv_string("UTF-8", "BIG5", str)));
}
private void command(object me, string arg)
{
	if( !is_command() ) return;
	if( !arg ) return tell(me, help, CMDMSG);
	if( strsrch(arg, "/") == -1 ) arg += "/";
	if( sscanf(arg, "http://%*s") == 0 ) arg = "http://" + arg;
	
        tell(me, "���J " + arg + " �������A�еy��...\n", CMDMSG);

	HTTP_CLIENT_D->get_url(arg, (: get_html, me :));
	return;
}

