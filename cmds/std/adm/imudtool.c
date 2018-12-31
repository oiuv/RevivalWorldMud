//inherit STD_BIN_OB;
#include <ansi.h>
//#include <http.h>
#include <i3.h>
#include <daemon.h>
#include <feature.h>

inherit COMMAND;

#define HTTP_PORT 4010
#define FTP_PORT 4100
#define I3_D		"/system/daemons/i3_d.c"
#define wiz_level(x) SECURE_D->level_num(x)
#define PRINT(x)  printf("%O\n", x)

#define write(x)  PRINT(x)

nosave string tmp_channel;
nosave string tmp_dname;

private string return_menu();
private string return_services_menu();
void main_menu_input(string input);
private void toggle_services();
int main();
private void set_security();

string main_menu = @END
-------------------------------------------------------------------------
%|72s
-------------------------------------------------------------------------
  %sCommand                             Status%s

   [1] Enable/Disable system          %s
   [2] Enable/Disable services
   [3] List received channels
   [4] List all available channels
   [5] Set/Change listening channels
   [6] Remove listening channels
   [7] Set Intermud security
   [8] Set Intermud password          %s

END;

string security_menu = @END
-------------------------------------------------------------------------
%|72s
-------------------------------------------------------------------------
  %sCommand                             Security Protection%s

   [1] Tell                           %s
   [2] Emote to                       %s
   [3] Who                            %s
   [4] Finger                         %s
   [5] Locate                         %s
   [6] Channels                       %s
   [7] News                           %s
   [8] Mail                           %s
   [9] File                           %s
  [10] Auth                           %s
  [11] Ucache                         %s
  [12] Mudlist                        %s

END;

string services_menu = @END
-------------------------------------------------------------------------
%|72s
-------------------------------------------------------------------------
  %sCommand                             Status%s

   [1] Tell                           %s
   [2] Emote to                       %s
   [3] Who                            %s
   [4] Finger                         %s
   [5] Locate                         %s
   [6] Channels                       %s
   [7] News                           %s
   [8] Mail                           %s
   [9] File                           %s
  [10] Auth                           %s
  [11] Ucache                         %s
  [12] Mudlist                        %s

END;

string list_channels_menu = @END
-------------------------------------------------------------------------
%|72s
-------------------------------------------------------------------------
  %sCommand%s

  [1] List received channel names
  [2] List received channel details

END;

private string main_prompt = "Enter your command [1-8], or [Q] to quit: ";
private string main_prompt2 = "Enter your command [1-8], or [B] to go back: ";
private string services_prompt = 
                            "Toggle service [1-12], or [ENTER] to go back: ";
private string channels_prompt = 
                            "Enter your command [1-2], or [ENTER] to go back: ";
private string security_prompt = "Set security [1-12], or [ENTER] to go back: ";
private mapping return_mapping = ([]);


int clean_up(int num) {
    //this_object()->remove();
    return 1;
}

//void remove() {
//    destruct(this_object());
//}

int query_prevent_shadow() {
    return 1;
}


private void toggle_system() {
    int stat;

    stat = I3_D->query_enabled();
    I3_D->set_enabled(!stat);
}

private string return_services_menu() {
    string ret, tell, emoteto, who, finger, locate, channels, news, mail,
           file, auth, ucache, mudlist;
    mapping s, services;
    string enabled = GRN+BOLD+"Enabled"+NOR;
    string disabled = YEL+BOLD+"Disabled"+NOR;
    string unavailible = RED+BOLD+"Unavailible"+NOR;

    s = SERVICES;
    services = I3_D->query_services();

    if (!services) return "Press [ENTER] to go back: ";

    if (s["tell"] == 0) 
        tell = unavailible;
    else if(services["tell"]->enabled == 0)
        tell = disabled;
    else
        tell = enabled;

    if (s["emoteto"] == 0) 
        emoteto = unavailible;
    else if(services["emoteto"]->enabled == 0)
        emoteto = disabled;
    else
        emoteto = enabled;

    if (s["who"] == 0) 
        who = unavailible;
    else if(services["who"]->enabled == 0)
        who = disabled;
    else
        who = enabled;

    if (s["finger"] == 0) 
        finger = unavailible;
    else if(services["finger"]->enabled == 0)
        finger = disabled;
    else
        finger = enabled;

    if (s["locate"] == 0) 
        locate = unavailible;
    else if(services["locate"]->enabled == 0)
        locate = disabled;
    else
        locate = enabled;

    if (s["channel"] == 0) 
        channels = unavailible;
    else if(services["channel"]->enabled == 0)
        channels = disabled;
    else
        channels = enabled;

    if (s["news"] == 0)
        news = unavailible;
    else if(services["news"]->enabled == 0)
        news = disabled;
    else
        news = enabled;

    if (s["mail"] == 0) 
        mail = unavailible;
    else if(services["mail"]->enabled == 0)
        mail = disabled;
    else
        mail = enabled;

    if (s["file"] == 0)
        file = unavailible;
    else if(services["file"]->enabled == 0)
        file = disabled;
    else
        file = enabled;

    if (s["auth"] == 0)
        auth = unavailible;
    else if(services["auth"]->enabled == 0)
        auth = disabled;
    else
        auth = enabled;

    if (s["ucache"] == 0)
        ucache = unavailible;
    else if(services["ucache"]->enabled == 0)
        ucache = disabled;
    else
        ucache = enabled;

    if (s["mudlist"] == 0)
        mudlist = unavailible;
    else if(services["mudlist"]->enabled == 0)
        mudlist = disabled;
    else
        mudlist = enabled;

    ret = sprintf(services_menu, MUD_NAME+" Intermud Admin Tool", BOLD, NOR,
                  tell, emoteto, who, finger, locate, channels,
                  news, mail, file, auth, ucache, mudlist);
    ret += services_prompt;
    return ret;
}

private void services_menu_input(string input) {
    mapping s, sd;

    s = I3_D->query_services();
    sd = SERVICES;

    switch(input) {
        case "": case 0:
            main();
            return;
        case "1":
            if (sd["tell"] == 1)
                I3_D->set_service("tell", !s["tell"]->enabled);
            break;
        case "2":
            if (sd["emoteto"] == 1)
                I3_D->set_service("emoteto", !s["emoteto"]->enabled);
            break;
        case "3":
            if (sd["who"] == 1)
                I3_D->set_service("who", !s["who"]->enabled);
            break;
        case "4":
            if (sd["finger"] == 1)
                I3_D->set_service("finger", !s["finger"]->enabled);
            break;
        case "5":
            if (sd["locate"] == 1)
                I3_D->set_service("locate", !s["locate"]->enabled);
            break;
        case "6":
            if (sd["channel"] == 1)
                I3_D->set_service("channel", !s["channel"]->enabled);
            break;
        case "7":
            if (sd["news"] == 1)
                I3_D->set_service("channel", !s["news"]->enabled);
            break;
        case "8":
            if (sd["mail"] == 1)
                I3_D->set_service("mail", !s["mail"]->enabled);
            break;
        case "9":
            if (sd["file"] == 1)
                I3_D->set_service("file", !s["file"]->enabled);
            break;
        case "10":
            if (sd["auth"] == 1)
                I3_D->set_service("auth", !s["auth"]->enabled);
            break;
        case "11":
            if (sd["ucache"] == 1)
                I3_D->set_service("ucache", !s["ucache"]->enabled);
            break;
        case "12":
            if (sd["mudlist"] == 1)
                I3_D->set_service("mudlist", !s["mudlist"]->enabled);
            break;
        default:
            write( "Invalid selection.\n");
            write( services_prompt);
            input_to( (: services_menu_input :) );
            return;
    }
    toggle_services();
}

private string return_security_menu() {
    string output;
    mapping services;
    
    services = I3_D->query_services();
    output = sprintf(security_menu+security_prompt,
                     MUD_NAME+" Intermud Admin Tool", BOLD, NOR,
                     BOLD+GRN+services["tell"]->security+NOR,
                     BOLD+GRN+services["emoteto"]->security+NOR,
                     BOLD+GRN+services["who"]->security+NOR,
                     BOLD+GRN+services["finger"]->security+NOR,
                     BOLD+GRN+services["locate"]->security+NOR,
                     BOLD+GRN+services["channel"]->security+NOR,
                     BOLD+GRN+services["news"]->security+NOR,
                     BOLD+GRN+services["mail"]->security+NOR,
                     BOLD+GRN+services["file"]->security+NOR,
                     BOLD+GRN+services["auth"]->security+NOR,
                     BOLD+GRN+services["ucache"]->security+NOR,
                     BOLD+GRN+services["mudlist"]->security+NOR);
    return output;
}

private void toggle_services() {
    write( return_services_menu());
    input_to( (: services_menu_input :) );
}

private void pause(string str) {
    main();
}

private void more_return() {
    write( "\nPress [ENTER] to continue: ");
    input_to( (: pause :) );
}

private void list_received_channels_name() {
    string output;
    string *channels;
    int i;

    output = BOLD+"Received Intermud 3 Channels:"+NOR+"\n\n";
    channels = sort_array(I3_D->query_received_channels(), 1);
    foreach (string chan in channels) {
        if (i == 3) { output += "\n"; i = 0; } else i++;
        output += sprintf("        %-16s", chan);
    }

    //MORE_D->more(output, this_player(), (: more_return :));
    this_player()->start_more(output);
}

private void list_received_channels_detail() {
    string output;
    string *channels;
    int i;
    mapping chanmap;
    class channel c;

    output = BOLD+"Received Intermud 3 Channels:"+NOR+"\n\n";
    channels = sort_array(I3_D->query_received_channels(), 1);
    chanmap = I3_D->query_received_mapping();
    output += BOLD+sprintf("%-15s %-15s %-11s %-15s %-6s  %-11s",
                           "Channel", "C Name", "Security",
                           "Format", "On/Off",
                           "Exceptions")+NOR+"\n";
    foreach (string chan in channels) {
        c = chanmap[chan];
        output += sprintf("%-15s %-15s %-11s %-15s %-6s ", chan, c->c_name,
                          c->sec == "0" ? "None" : c->sec, c->format,
                          c->off_on_create ? "Off" : "On",);
        if (sizeof(c->exceptions) > 0) {
            i = 1;
            foreach (string ex in c->exceptions)
                if (i == 1) {
                    output += sprintf(" %-11s\n",
                    capitalize(ex)); i = 0;
                } else output += sprintf("%-15s %-15s %-11s %-15s %-6s  %s\n",
                                         "","","","","", capitalize(ex));
        } else output += "\n";
    }

    //MORE_D->more(output, this_player(), (: more_return :));
    this_player()->start_more(output);
}

private void select_channel(string sel) {
    switch (sel) {
        case "": case 0:
            main();
            return;
        case "1":
            list_received_channels_name();
            return;
        case "2":
            list_received_channels_detail();
            return;
        default:
            write( "Invalid selection.\n");
            write( channels_prompt);
            input_to( (: select_channel :) );
    }
}

private void list_received_channels() {
    write( sprintf(list_channels_menu+channels_prompt,
                 MUD_NAME+" Intermud Admin Tool", BOLD, NOR));
    input_to( (: select_channel :) );
}

private void get_off(string line, string format, string cls,
                     string *exceptions, string alias, string yn) {
    int i;

    if (lower_case(yn) == "y" || lower_case(yn) == "yes")
        i = 1;
    else
        i = 0;

    I3_D->set_listening(line, 1, cls, alias, format, exceptions, i);
    write( "Now listening to "+line+".\n");
    write( "\nPress [ENTER] to continue: ");
    input_to( (: pause :) );
}

private void get_alias(string line, string format, string cls,
                       string *exceptions, string alias) {
    write(
                 "Do you want this line to be turned off by default? [y/n]: ");
    input_to( (: get_off($(line), $(format), $(cls),
                                    $(exceptions), $(alias), $1) :) );
}

private void get_exceptions(string line, string format, string cls,
                            string exceptions) {
    string *players;

    if (!exceptions || exceptions == "") {
        main();
        return;
    }

    if (exceptions != ".")
        players = explode(lower_case(exceptions), " ");
    else
        players = ({});

    write( "Enter alias for line, or [ENTER] to go back: ");
    input_to( (: get_alias($(line), $(format), $(cls),
                                    $(players), $1) :) );
}

private void get_security(string line, string format, string cls) {

    if (!cls || cls == "") {
        main();
        return;
    }

    if (wiz_level(cls) < 0 && cls != "0") {
        write( "Invalid security class.\n");
        write( "Enter security class restriction, 0 to disable "
                     "class restrictions, or [ENTER] to go back: ");
        input_to( (: get_security($(line),
                                         $(format), $1) :) );
        return;
    }

    write( "Enter people who can listen to this line separated "
                 "by spaces, or (.) for none, or [ENTER] to go back: ");
    input_to( (: get_exceptions($(line), $(format),
                                     $(cls), $1) :) );
}

private void get_format(string line, string input) {
    string tmp, tmp2;
    int i;

    if (!input || input == "") {
        main();
        return;
    }

    tmp = input;
    while (1) {
        tmp2 = replace_string(tmp, "%s", "", 1);
        if (tmp == tmp2) {
            break;
        }
        tmp = tmp2;
        i++;
    }

    if (i != 3) {
        write( "Invalid line format.\n");
        write(
                     "Select format for line (e.g. [%s] %s: %s]), or [ENTER] "
                     "to go back: ");
        input_to( (: get_format($(line), $1) :) );
        return;
    }

    write( "Enter security class restriction, 0 to disable "
                 "class restrictions, or [ENTER] to go back: ");
    input_to( (: get_security($(line), $(input), $1) :) );
}

private void get_channel(string str) {

    if (!str || str == "") {
        main();
        return;
    }

    if (member_array(str, I3_D->query_channels()) < 0) {
        write( "Invalid line.\n");
        write(
                     "Enter channel to receive, or [ENTER] to go back: ");
        input_to( (: get_channel :) );
        return;
    }

    write(
                 "Select format for line (e.g. [%s] %s: %s]), or [ENTER] to go "
                 "back: ");
    input_to( (: get_format($(str), $1) :) );
}

private void set_channels() {
    string output;
    string *channels;
    
    output = BOLD+"Availible Intermud 3 channels:"+NOR+"\n\n";
    channels = sort_array(I3_D->query_channels(), 1);
    foreach (string chan in channels)
        output += chan+", ";
    output = output[0..<3]+"\n";
    write( output+
                 "\nEnter channel to receive, or [ENTER] to go back: ");
    input_to( (: get_channel :) );
}

private void get_rchannel(string str) {

    if (!str || str == "") {
        main();
        return;
    }

    if (member_array(str, I3_D->query_received_channels()) < 0) {
        write( "Invalid line.\n");
        write(
                     "Enter channel to remove, or [ENTER] to go back: ");
        input_to( (: get_rchannel :) );
        return;
    }

    I3_D->set_listening(str, 0);
    write( "No longer receiving "+str+".\n");
    write( "\nPress [ENTER] to continue: ");
    input_to( (: pause :) );
}

private void remove_channels() {
    string output;
    string *channels;

    output = BOLD+"Received intermud 3 channels:"+NOR+"\n\n";
    channels = sort_array(I3_D->query_received_channels(), 1);
    foreach (string chan in channels)
        output += chan+", ";
    output = output[0..<3]+"\n";
    write( output+
                 "\nEnter channel to remove, or [ENTER] to go back: ");
    input_to( (: get_rchannel :) );
}

private void list_channels() {
    string output;
    string *channels;
    int i;

    output = BOLD+"All Intermud 3 Channels:"+NOR+"\n\n";
    channels = sort_array(I3_D->query_channels(), 1);
    foreach (string chan in channels) {
        if (i == 2) { output += "\n"; i = 0; } else i++;
        output += sprintf("        %-16s", chan);
    }

    //MORE_D->more(output, this_player(), (: more_return :));
    this_player()->start_more(output);
}

private void get_password(string pw) {
    int p;

    if (!pw || pw == "") {
        main();
        return;
    }

    if (sscanf(pw, "%d", p) < 1) {
        write( "Invalid password.\n");
        write( "Please enter your new password, or [ENTER] "
                     "to go back: ");
        input_to( (: get_password :) );
    }

    I3_D->set_password(p);
    write( "\nPress [ENTER] to continue: ");
    input_to( (: pause :) );
}

private void set_password() {
    write( "DO NOT CHANGE YOUR PASSWORD UNLESS YOU KNOW "
                           "WHAT YOU'RE DOING!\n");
    write( sprintf("Your current password is: %d\n",
                 I3_D->query_password()) );
    write( "Please enter your new password, or [ENTER] "
                 "to go back: ");
    input_to( (: get_password :) );
}

private void get_class(string service, int status, string sec) {

    if (!sec || sec == "") {
        set_security();
        return;
    }

    sec = upper_case(sec);
    if (wiz_level(sec) < 0) {
        write( "Invalid security class.\n");
        write( security_prompt);
        input_to( (: get_class($(service), $(status), $1) :) );
        return;
    }

    I3_D->set_service(service, status, sec);
    set_security();
}

private void get_security2(string num) {
    string s;
    int i;

    switch (num) {
        case "": case 0:
            main();
            return;
        case "1":
            s = "tell"; break;    
        case "2":
            s = "emoteto"; break;
        case "3":
            s = "who"; break;
        case "4":
            s = "finger"; break;
        case "5":
            s = "locate"; break;
        case "6":
            s = "channel"; break;
        case "7":
            s = "news"; break;
        case "8":
            s = "mail"; break;
        case "9":
            s = "file"; break;
        case "10":
            s = "auth"; break;
        case "11":
            s = "ucache"; break;
        case "12":
            s = "mudlist"; break;
        default:
            write( "Invalid service.\n");
            write( security_prompt);
            input_to( (: get_security2 :) );
            return;
    }
    i = I3_D->query_services()[s]->enabled;
    write( "Please enter security class, or [ENTER] to go back: ");
    input_to( (: get_class($(s), $(i), $1) :) );
}

private void set_security() {
     write( return_security_menu());
     input_to( (: get_security2 :) );
}

private void main_menu_input(string input) {
    switch(input) {
        case "": case "0":
            main();
            return;
        case "B": case "b":
            if (!return_mapping[this_player()]) {
                write( "Invalid selection.\n");
                write(
                undefinedp(return_mapping[this_player()]) ? main_prompt :
                           main_prompt2 );
                input_to( (: main_menu_input :) );
                return;
            } else {
                return_mapping[this_player()]->main();
                map_delete(return_mapping, this_player());
                break;
            }
        case "q": case "Q":
            if (return_mapping[this_player()]) {
                write( "Invalid selection.\n");
                write(
                undefinedp(return_mapping[this_player()]) ? main_prompt :
                           main_prompt2 );
                input_to( (: main_menu_input :) );
                return;
            } else
                return;
        case "1":
            toggle_system();
            main();
            return;
        case "2":
            toggle_services();
            return;
        case "3":
            list_received_channels();
            return;
        case "4":
            list_channels();
            return;
        case "5":
            set_channels();
            return;
        case "6":
            remove_channels();
            return;
        case "7":
            set_security();
            return;
        case "8":
            set_password();
            return;
        default:
            write( "Invalid selection.\n");
            write(
            undefinedp(return_mapping[this_player()]) ? main_prompt :
                                                        main_prompt2 );
            input_to( (: main_menu_input :) );
            return;
    }
}

private string return_menu() {
    string ret;

    ret = sprintf(main_menu, MUD_NAME+" Intermud Admin Tool", BOLD, NOR,
                  I3_D->query_enabled() ? GRN+BOLD+"Enabled"NOR :
                  YEL+BOLD+"Disabled"+NOR, GRN+BOLD+
                  sprintf("%d", I3_D->query_password())+NOR);
    ret += undefinedp(return_mapping[this_player()]) ? main_prompt : 
                                                       main_prompt2;
    return ret;
}





string help = @HELP
        �з� wizcmd_example ���O�C
HELP;

private void command(object me, string arg)
{
    if( !is_command() ) return;
 	
 	main();       
}
int main()
{

    /*if (!security_check(ADMIN_BIN_PRIV)) {
        write( "Security violation!\n");
        return;
    } */

    //if (previous_object())
    //    if (file_name(previous_object()) == "/cmds/adm/imudtool")
    //        return_mapping[this_player()] = previous_object();

    write( return_menu());
    //write(HIG"shit shit !!\n"NOR);
    //input_to( (: main_menu_input() :) );
    input_to( (: main_menu_input :) );
    return 1;
}
