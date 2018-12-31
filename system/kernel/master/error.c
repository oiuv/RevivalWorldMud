/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : error.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

#include <daemon.h>

#define TRACE_DETAIL_LENGTH_LIMIT	300
#define SPRINTF_OVERFLOW		65535

/* �B�z���~�T�����禡 */
string tracert_error(mapping error, int caught)
{
        int count;
        string err_msg;
        mapping trace;

        err_msg="\n"+sprintf(@ERR
�w�w�w�w�w�w�w�w�w�w�w�w�w�w<Bugs Report>�w�w�w�w�w�w�w�w�w�w�w�w�w�w
[ ���~�ɶ� ]: %-s
[ ���~���e ]: %-s[m
[ ���~�ɮ� ]: %-s
[ ���~��� ]: %-d[m
[ ��Ʀ^�� ]:
ERR,
        ctime(time()),
        replace_string(error["error"], "\n", " "),
        error["file"],
        error["line"]);

        foreach(trace in error["trace"])
        {
                count++;
                err_msg +=sprintf(@ERR
    -- �� %|3d �� --
        [ Ĳ�ʪ��� ]: %O     
        [ �{���ɮ� ]: %s
        [ �禡�W�� ]: %s(%s)
        [ �I�s��� ]: %s
ERR,
                count,
                trace["object"],
                trace["program"]||"",
                trace["function"]||"",
                trace["arguments"] ? implode(map(trace["arguments"], (:typeof($1):)), ", ") : "",
                (trace["line"] || "����")+"");

                if( trace["arguments"] )
                {
                        err_msg += "        [ �ǤJ�Ѽ� ]:\n";
                        err_msg += implode(map(trace["arguments"], (: "                   ** ("+typeof($1)+")"+implode(explode(sprintf("%."+TRACE_DETAIL_LENGTH_LIMIT+"O\n", $1)+(strlen(sprintf("%O", $1)) > TRACE_DETAIL_LENGTH_LIMIT ? "... �T���L���ٲ�\n" : ""), "\n"),"\n                      ") :)), "\n")+"\n";
                }
                if( trace["locals"] )
                {
                        err_msg += "        [ �{���ܼ� ]:\n";
                        err_msg += implode(map(trace["locals"], (: "                   ** ("+typeof($1)+")"+implode(explode(sprintf("%."+TRACE_DETAIL_LENGTH_LIMIT+"O\n", $1)+(strlen(sprintf("%O", $1)) > TRACE_DETAIL_LENGTH_LIMIT ? "... �T���L���ٲ�\n" : ""), "\n"),"\n                      ") :)), "\n")+"\n";
                }
        }
        err_msg+="�w�w�w�w�w�w�w�w�w�w�w�w�w�w<Bugs Report>�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n\n";
        return err_msg;
}

void error_handler(mapping error, int caught)
{
	object me;
	string msg;
	
	me = this_interactive() || this_player() || previous_object();

	// �קK���J�L�����~�^���j��
	if( previous_object() == this_object() )
	{
		if( find_object(CHANNEL_D) )
			CHANNEL_D->channel_broadcast("sys", "Master object �o�͵L�����~�j��A"+error["file"]+" �� "+error["line"]+" ��A���~�G"+error["error"]+"�A�w�j���C");
		return;
	}

        msg = tracert_error(error, caught);
        
        if( caught )
        	log_file("catch", msg);
        else
        	log_file("run", msg);
        	
        if( objectp(me) && userp(me) )
        {
        	string *command = query_temp("command", me);

		if( strlen(msg) >= SPRINTF_OVERFLOW-50000 ) msg = msg[0..SPRINTF_OVERFLOW-50000] + "\n\n[�T���L���ٲ�...]\n";
		
		if( member_array("temp_database", variables(me)) != -1 ) 
			set_temp("bug_msg", msg, me);

		if( !wizardp(me) )
		{
			tell(me, "[1;5;33mWARNING[m �o�ӵ{���@�~����L�ħY�N�����A�p�����D�Ь��{���]�p�v�ά����H��...\n", "ERRMSG");			
			
			// �קK CHANNEL_D �����o�Ϳ��~�ɪ����D
			if( find_object(CHANNEL_D) )
			{
				CHANNEL_D->channel_broadcast("sys", me->query_idname()+"("+(sizeof(command)==2?command[0]+" "+command[1]:"Unknown")+")����ɬq���~"+(caught ? "(Catch)" : "")+"�G"+error["file"]+" �� "+error["line"]+" ��C");
				CHANNEL_D->channel_broadcast("sys", "���~�G"+replace_string(error["error"],"\n", " ")+"�C");
			}
			tell(me, "[1;32m�w�۰ʱN���~�q���u�W�Ův�B�z�A�еy�����A�Y�L�H�B�z�ХD�ʳq���Ův�C[m\n", "ERRMSG");
		}
		else
			tell(me, "[1;33m����ɬq���~[m�G�а��� bug �ˬd�Բӿ��~�^���C\n"+"[1;33m........�ɮ�[m�G"+error["file"]+"\n[1;33m........���[m�G�� "+error["line"]+" ��\n[1;33m........���~[m�G"+replace_string(error["error"],"\n", " ")+"\n", "ERRMSG");

		//me->finish_input();
	}
	else
	{
		// �קK CHANNEL_D �����o�Ϳ��~�ɪ����D
		if( find_object(CHANNEL_D) )
		{
			CHANNEL_D->channel_broadcast("sys", (objectp(me) ? base_name(me)+" " : "")+"����ɬq���~"+(caught ? "(Catch)" : "")+"�G"+error["file"]+" �� "+error["line"]+" ��C");
			CHANNEL_D->channel_broadcast("sys", "���~�G"+replace_string(error["error"],"\n", " ")+"�C");
		}
	}

}


// Write an error message into a log file. The error occured in the object
// 'file', giving the error message 'message'.
/* ������ܽsĶ���~���T�� */
void log_error(string, string message)
{
	string error_type;
	object user = this_interactive() || this_player() || previous_object();

    	log_file("comp", message);

	error_type = strsrch(message, "Warning") == -1 ? "���~" : "ĵ�i";

	if( objectp(user) && userp(user) )
	{
		if( !wizardp(user) )
			CHANNEL_D->channel_broadcast("sys", user->query_idname()+"�sĶ�ɬq"+error_type+"�G"+message);

		tell(user, "[1;33m�sĶ�ɬq"+error_type+"[m�G" + message, "ERRMSG");
	}
	else
		CHANNEL_D->channel_broadcast("sys", (objectp(user) ? base_name(user)+" " : "")+"�sĶ�ɬq"+error_type+"�G"+message);
}