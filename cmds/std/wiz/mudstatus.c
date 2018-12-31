/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mudstatus.c
 * Author : Cookys@RevivalWorld
 * Date   : 2001-07-08
 * Note   : mudstatus ���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <i2d.h>
#include <message.h>
#include <ansi.h>
inherit COMMAND;

string help = @HELP
        �з� mudstatus ���O�C
HELP;
#define I2D "/system/daemons/intermud2_d.c"

//object I2D=load_object(I2D);

nosave array current_status=({ ({}),({"$GRN$GB$NOR$",GB_CODE}),({"$RED$AD$NOR$",ANTI_AD}),({"$BLU$Ig$NOR$",IGNORED}),({}),({}),({"$GRN$GB$NOR$",GB_CODE}),({"$RED$AD$NOR$",ANTI_AD}),({"$BLU$Ig$NOR$",IGNORED}) });

void select_mud(int funct,int si,string *mud,string x)
{
	int input;
	string mudname;
	
		
	if( stringp(x) && x[0]=='q' )
	{
		tell(this_player(), "���}�C\n", CMDMSG);
		return;
	}
	
	if( !sscanf(x,"%d",input) || input<0 || input > si )
	{
		tell(this_player(), "���X�k����J�A�Э��s��J�G\n", CMDMSG);
		input_to((: select_mud,funct,si,mud :));
		return;
	}
	mudname=mud[input];
	switch(funct)
	{
		case 1:if(I2D->set_status(mudname,GB_CODE)) tell(this_player(), "�����C\n", CMDMSG);return;
		case 2:if(I2D->set_status(mudname,ANTI_AD)) tell(this_player(), "�����C\n", CMDMSG);return;
		case 3:if(I2D->set_status(mudname,IGNORED)) tell(this_player(), "�����C\n", CMDMSG);return;
		case 6:if(I2D->set_status(mudname,-GB_CODE)) tell(this_player(), "�����C\n", CMDMSG);return;
		case 7:if(I2D->set_status(mudname,-ANTI_AD)) tell(this_player(), "�����C\n", CMDMSG);return;
		case 8:if(I2D->set_status(mudname,-IGNORED)) tell(this_player(), "�����C\n", CMDMSG);return;
	}
	
}

string test_status(string name,int status)
{
	if( I2D->fetch_data(name)["STATUS"] & status )
		return HIG"ENABLE"NOR;
	else return HIR"DISABLE"NOR;
}

void get_mud_name(int funct,string name)
{
	int si,i;
	string *mud=I2D->fetch_mudname(name);
	
		
	if( stringp(name) && name[0]=='q' )
	{
		tell(this_player(), "���}�C\n", CMDMSG);
		return;
	}
	
	if(!mud)
	{ 
		tell(this_player(), "��p�A�䤣�쯸�x�A�Э��s��J�C\n", CMDMSG);
		input_to((: get_mud_name,funct :));
		return;
	}
	//if( si=sizeof(mud)==1)
	//for(int i=0;i<=si;i++)
        //        printf("[%2d]\t\t%s\n",i,mud[i]);
        si=sizeof(mud);
        if( si == 1 )
        {
        	select_mud(funct,si,mud,"0");
        	return;
        }
        foreach(string x in mud)
        {
        	//tell(this_player(),sprintf("[%4d] %s\n",i,x));
        	tell(this_player(),sprintf("[%4d] %s %s[%s]\n",i,x,ansi(current_status[funct][0]),test_status(x,current_status[funct][1])));
        	i++;	
        }
        tell(this_player(), "�п�� 0~"+(si-1), CMDMSG);
        input_to((: select_mud,funct,si,mud :));
	return;
}

void get_mud_ip(int funct,string name)
{
	int si,i;
	string *mud=I2D->fetch_mudip(name);
	
		
	if( stringp(name) && name[0]=='q' )
	{
		tell(this_player(), "���}�C\n", CMDMSG);
		return;
	}
// fetch mud by ip	
	si=sizeof(mud);
        if( si == 1 )
        {
        	select_mud(funct,si,mud,"0");
        	return;
        }
        foreach(string x in mud)
        {
        	tell(this_player(),sprintf("[%4d] %s %s[%s]\n",i,x,ansi(current_status[funct][0]),test_status(x,current_status[funct][1])));
        	i++;	
        }
        tell(this_player(), "�п�� 0~"+(si-1), CMDMSG);
        input_to((: select_mud,funct,si,mud :));
	return;
	
	
}
void select_target_way(int funct,string x)
{
	int *num=({1,2}),input;
	
		
	if(stringp(x) && x[0]=='q' )
	{
		tell(this_player(), "���}�C\n", CMDMSG);
		return;
	}
	
	if( !sscanf(x,"%d",input) || member_array(input,num)==-1 )
	{
		tell(this_player(), "�п�J[ 1~2 ] ���Ʀr�G\n", CMDMSG);
		input_to((: select_target_way,funct :));
		return;
	}
	
	switch(input)
	{
		case  1:tell(this_player(), "�п�J���x�W�١G\n", CMDMSG);
			input_to((: get_mud_name,funct :));
			return;
		case  2:
			tell(this_player(), "�п�J���x 'IP:PORT'�G\n", CMDMSG);
			input_to((: get_mud_ip,funct :));
			return;
	}
	return;
}

void select_main_menu(string x)
{
	int *num=({1,2,3,6,7,8,9}),input;
	
	if(stringp(x) && x[0]=='q' )
	{
		tell(this_player(), "���}�C\n", CMDMSG);
		return;
	}
	
	if( !sscanf(x,"%d",input) || member_array(input,num)==-1 )
	{
		tell(this_player(), "�п�J[ 1~3,6~9 ] ���Ʀr�G\n", CMDMSG);
		input_to((: select_main_menu :));
		return;
	}
	if( input==9)
	{
		if( fetch_variable("debug",find_object(I2D))==1 )
		{ 
			I2D->debug();
			tell(this_player(), "�����\�������C\n", CMDMSG);
			return;
		} else {
			I2D->debug();
			tell(this_player(), "�����\��}�ҡC\n", CMDMSG);
			return;
		}	
	}
	tell(this_player(), "1.��ʿ�J���x�W��\n2.��ʿ�J IP ��}\n\n", CMDMSG);
	switch(input)
	{
		case  1:tell(this_player(), "�п�ܧA�Q�W�[ #��X# �\�઺���x��J�覡�G\n", CMDMSG);
			input_to((: select_target_way,1 :));break;
		case  2:tell(this_player(), "�п�ܧA�Q�W�[ #�s�i# �\�઺���x��J�覡�G\n", CMDMSG);
			input_to((: select_target_way,2 :));break;
		case  3:tell(this_player(), "�п�ܧA�Q�W�[ #����# �\�઺���x��J�覡�G\n", CMDMSG);
			input_to((: select_target_way,3 :));break;
		case  6:tell(this_player(), "�п�ܧA�Q�Ѱ� #��X# �\�઺���x��J�覡�G\n", CMDMSG);
			input_to((: select_target_way,6 :));break;
		case  7:tell(this_player(), "�п�ܧA�Q�Ѱ� #�s�i# �\�઺���x��J�覡�G\n", CMDMSG);
			input_to((: select_target_way,7 :));break;
		case  8:tell(this_player(), "�п�ܧA�Q�Ѱ� #����# �\�઺���x��J�覡�G\n", CMDMSG);
			input_to((: select_target_way,8 :));break;
		
	}
	return;
}

private void command(object me, string arg)
{
	if( !is_command() ) return;
	if( !arg )
	{
		tell(me, "�w��ϥ� Intermud2 Daemon Tool �I�I\n", CMDMSG);
		tell(me, "�п�J�Q�ϥΪ����ءG\n"+@ITEM
[1]. �W�[ #��X# �\���Y��( BIG5 <-> GB ������ )
[2]. �W�[ #�s�i# �\���Y��( �N�ӯ��Ҧ��W�D�T����� AD Channel )
[3]. �W�[ #����# �\���Y��( �����Ҧ��ӯ��Ұ����ʧ@ )
[4]--(�O�d)
[5]--(�O�d)
[6]. �Ѱ��Y���� #��X# �\��
[7]. �Ѱ��Y���� #�s�i# �\��
[8]. �Ѱ��Y���� #����# �\��
[9]. �����\���ܧ�( DEBUG Mode On/Off )
[q]. ���}�C
�п�J[ 1~3,6~9 ] ���Ʀr�G
ITEM, CMDMSG);	
		input_to((: select_main_menu :));
		return;
	}	
}