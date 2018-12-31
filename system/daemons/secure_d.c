/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : secure_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-7
 * Note   : Ū�g�v���P�ϥΪ̵��ź��F
 * Update :
 *  o 2001-02-08 Clode �� trigger_object �i��|���@�ǷN�~���p, ���L
 *  o 2002-09-11 Clode ���s��z Secure_d
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <secure.h>

#define DATA    "/system/kernel/data/secure"+__SAVE_EXTENSION__
#define LOG     "daemon/secure"

/* �Ův�W�� */
private mapping wizards = 
([
    // �]�w�w�]�Ův, �]�w����R�� DATA ��, ���s�Ұʫ�K�i�󴫷s���Ův�C��
    "npc"   :   "wizard",
    "mud"   :   "advisor",
    "ivy"   :   "admin",
]);

// �ϥΪ̵��żƦr�P�W�ٹ��
private nosave mapping wizlevel_contrast =
([
    PLAYER  :   "player",
    GUEST   :   "guest",
    WIZARD  :   "wizard",
    ADVISOR :   "advisor",
    ADMIN   :   "admin",
]);


/* ���榡 : ({ WIZ_LEVEL, "ID 1", "ID 2", ... }).... */

/* ���\Ū���ؿ�����, ��������歭��̥iŪ���Ҧ��ɮץؿ� */
private nosave mapping ALLOW_READ =
([
    "log"       :   ({ ADMIN | ADVISOR }),
    "system/kernel" :       ({ ADMIN | ADVISOR | WIZARD }),
    "doc/driver"    :       ({ ADMIN | ADVISOR | WIZARD }),
]);

/* ���\�g�J�ؿ�����, ��������歭��̥i�g�J�Ҧ��ɮץؿ� */
private nosave mapping ALLOW_WRITE =
([
    "cmds"      :   ({ ADMIN | ADVISOR }),
    "doc"       :   ({ ADMIN | ADVISOR }),
    "include"       :   ({ ADMIN | ADVISOR }),
    "obj"       :   ({ ADMIN | ADVISOR }),
    "std"       :   ({ ADMIN | ADVISOR }),
    "system"        :   ({ ADMIN | ADVISOR }),
    "data/user"     :   ({ ADMIN | ADVISOR }),
]);

/* ���\�sĶ�ؿ�����, ��������歭��̥i�sĶ���J�Ҧ��ɮץؿ� */
private nosave mapping ALLOW_COMPILE = 
([
    "system"        :   ({ ADMIN | ADVISOR }),
    "system/kernel" :   ({ ADMIN | ADVISOR }),
    "system/daemons"    :   ({ ADMIN | ADVISOR }),
]);

nomask int is_wizard(string id)
{
    return !undefinedp(wizards[id]);
}

/* �x�s�ɮפγƥ� */
private nomask int save_data()
{
    return save_object(DATA) && cp(DATA, DATA+"_bak");
}

/* �^�� level_num �۹����� level_name */
nomask string level_num_to_level_name(int num)
{
    return wizlevel_contrast[num] || wizlevel_contrast[PLAYER];
}

/* �^�� level_name �۹����� level_num */
nomask int level_name_to_level_num(string name)
{
    foreach(int level_num, string level_name in wizlevel_contrast)
    if( name == level_name ) 
        return level_num;

    return 0;
}

/* �^�ǨϥΪ̪��Ův���ŦW�� */
nomask string level_name(mixed userid)
{
    if( objectp(userid) )
        userid = userid->query_id(1);
    
    if( !stringp(userid) )
        return 0;

    return wizards[userid] || wizlevel_contrast[PLAYER];
}

/* �^�ǨϥΪ̪��Ův���żƦr */
nomask int level_num(mixed userid)
{
    string name;
    
    if( objectp(userid) )
        userid = userid->query_id(1);
    
    if( !stringp(userid) )
        return 0;
    
    name = level_name(userid);

    foreach(int level_num, string level_name in wizlevel_contrast)
    if( name == level_name ) 
        return level_num;
}

nomask varargs int permission(string file, string id, int option, string func)
{

    int i = strlen(file);
    array value;
    string *apart = explode(file, "/");
    mapping limit;

    switch(option)
    {
    case READ:  limit = ALLOW_READ;     break;
    case WRITE: limit = ALLOW_WRITE;    break;
    case COMPILE:   limit = ALLOW_COMPILE;  break;
    default:    return 0;
    }

    /* �ѪR�ؿ��v�� */
    while(i--)
        if( !undefinedp(value = limit[implode(apart[0..i], "/")]) )
            return level_num(id) & value[0] || member_array(id, value[1..]) != -1;

        /* �����S���ŦX�� return 1 */
    return 1;
}

nomask varargs int valid_check(string file, object ob, string func, int option)
{
    object tri_ob;
    string my_id;

    //printf("\nvalidcheck:\tfile:%s\t func:%s\n",file,func);
    //printf("origin: %s\n statck: %O\n",origin(),call_stack(1)[<1])

    if( !objectp(ob) || ob == this_object() || !intp(option) ) return 1;

    if( !file || !file[0] ) 
        file = "/";
    else if( file[0] != '/' ) 
        file = "/" + file;

    /* �ק� Driver �� this_player(), �� this_player(1) �i�Ǧ^ call_out �� this_player() by Clode */
    tri_ob = this_player(1) || call_stack(1)[<1];

    if( func == "save_object")
    {
        // �u�� secure_d �i�H�� secure.o
        if( ob != this_object() && file == DATA )
            return 0;
    }

    /* �Y�OĲ�ʪ��󬰤��ʪ��� */
    if( objectp(tri_ob) && interactive(tri_ob) )
    {
        string your_id;

        /* ���a�@�� return 1 */
        if( !wizardp(tri_ob) ) return 1;

        /* �d�ߤ��� id �� return 0 */
        if( !stringp(my_id = tri_ob->query_id(1)) ) return 0;

        /* ����Ū��ۤv���Ű����Ův���ɮ� */
        if( sscanf(file, "/wiz/home/%s/%*s", your_id) == 2 || sscanf(file, "/wiz/home/%s", your_id) == 1 )
            return level_num(my_id) >= ADVISOR || level_num(your_id) <= level_num(my_id);

        if( option == WRITE && (sscanf(file, "/data/user/%*s/%s/%*s", your_id) == 3 || sscanf(file, "/data/user/%*s/%s", your_id) == 2) )
                        return level_num(my_id) >= level_num(your_id) || your_id == my_id || !is_wizard(your_id);

        return permission(file, my_id, option, func);
    }

    /* �|�� /wiz/home ���ɮ�Ĳ�o�����Τj���u���b call_out ���������Χa? */
    if( tri_ob && sscanf(file_name(tri_ob), "/wiz/home/%s/%*s", my_id) )
        return permission(file, my_id, option, func);

    /* �Y���O�� /wiz/home �����ɮש�Ĳ�o�� valid_read
       �h�@�� return 1, �]�N�O������L�ؿ��U���ɮצs��
       �����浹�ȱo�H�����H, �o�̥H���٥i�H�ɨǪF��.. */
    return 1;

}

nomask mapping query_wizards()
{
    return copy(wizards);
}

nomask int change_wizard_level(string id, string level_name)
{
    object this_player = this_player(1);

    /* �ˬd�O�_�����O���� && �קK this_object �Q�~�� */
    if( !level_name 
        || !id
        || !objectp(this_player)
        || level_num(this_player->query_id(1)) < level_name_to_level_num(level_name)
        || !is_command(previous_object()) 
        || this_object() != load_object(SECURE_D) 
        || !level_name_to_level_num(level_name) ) 
        return 0;

    /* �p�G level �� player */
    if( level_name == "player" )
        map_delete(wizards, id);
    else 
        wizards[id] = level_name;

    log_file(LOG, sprintf("%s �ק� %s �v���� %s�C\n", this_player?this_player->query_idname():"", id, level_name));

    return save_data();
}

private void create()
{
    if( clonep() ) return;

    if( !restore_object(DATA, 1) )
    {
        save_data();
        log_file(LOG, "�L�k���J�s�ɡC\n");
    }
}

string query_name()
{
    return "�w���t��(SECURE_D)";
}
