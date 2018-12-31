/* *********************�����Y�ɳq�T�t��***********************
*  
*                                                  By Whatup *
*                                               
* ���u
:whatup_!whatup@mail2000.com.tw QUIT :Leaving...
�W�u
:whatup_!whatup@mail2000.com.tw JOIN :#bitlbee
    :root!root@localhost.localdomain PRIVMSG #bitlbee :Question on MSN connection (handle yukang.tw@yahoo.com.tw):
    :root!root@localhost.localdomain PRIVMSG #bitlbee :The user whatup.tw@gmail.com (��-���) wants to add you to his/her buddy list. Do you want to allow this?
    :root!root@localhost.localdomain PRIVMSG #bitlbee :You can use the yes/no commands to answer this question.

*                                     ==���g�P�N�A�Y�T�y�X==  *
**************************************************************/

/* ���Y�ޤJ�� */
#include <ansi.h>
#include <message.h>
#include <feature.h>
#include <daemon.h>

/* �����]�w */
#include <socket.h>
#include <socket_err.h>

#define PING_TIME 240
#define TELL(x) CHANNEL_D->channel_broadcast("nch", "IM ���F�G"+(string)x)
#define BITLBEE_SERVER "62.75.129.41 7777"

#undef DEBUG

/* �ҩl�]�w */
private mapping users = ([]);

private int process_identify(object me,string pass);
void process_who(int fd,string str);
protected void close_socket(int fd);
varargs void send_who(int fd,string id);
void send_ping();

/* �ɮץD�� */
void create()
{
	CHANNEL_D->channel_broadcast("sys", "IM ���F�G�ҩl�����C");
	call_out((:send_ping:),PING_TIME);
}

void remove()
{
	foreach(int fd,mapping m in users)
	{
		if(m["obj"])
			tell(m["obj"],"���s��s im �t�ΡA�Э��s�n�J�C\n");

		socket_write(fd,"QUIT\r\n");
		socket_close(fd);
	}
}

void reset()
{
	foreach(int fd,mapping m in users)
	{
		if(!m["obj"]) 
		{
			close_socket(fd);			
			map_delete(users,fd);
		}
	}
}

void send_ping()
{
	foreach(int fd,mapping m in users)
	{
		socket_write(fd,"PING\r\n");
	}

	call_out((:send_ping:),PING_TIME);
}

/* �}�l�n�J */
void login_irc(object user)
{
	int err,fd;
	string id;

	if( !user ) return tell( user, "����\n");

	id = user->query_id(1);

	if(!id)
	{
		return tell(user,"�A�S���n�J ID ");
	}

	fd = socket_create( STREAM, "read_callback","close_socket");

	if ( fd < 0 )
	{
		return tell(user,"�s�����ѡA�i��O�D���S���}�ҩάO�����L�k�s�u�C");
	}

	users[fd] = allocate_mapping(5);
	users[fd]["id"] = id;
	users[fd]["steps"]   = 0;
	users[fd]["obj"]   = user;
	users[fd]["list"] = allocate_mapping(1);
	set_temp( "im_fd", fd, user);

	err = socket_connect(fd,BITLBEE_SERVER,"read_callback","write_callback" );

	if( err==EESUCCESS )
	{
#ifdef DEBUG
		TELL(sprintf("[%s] %s ���\�Ұ� Socket ,�}�l�ǳƵn�J irc.fd = %d",
			TIME_D->replace_ctime(time()) ,id,fd));
#endif 
	} else {
#ifdef DEBUG
		TELL(sprintf("[%s]�Ұ� Socket ����,�L�k�e�s�������D��.",
			TIME_D->replace_ctime(time()) ));
#endif 
		return ;
	}
	call_out((:process_identify:),3,user,crypt(id,id)[0..10]);
	return;
}

private int process_identify(object me,string pass)
{
	int fd = query_temp("im_fd", me);
	if( fd < -1 ) 
	{
		tell( me,"[IM Message]:�s�u���ѡA�еy��A�աI\n");
		return 1;
	}
	if(users[fd]["obj"] == me)
		socket_write(fd,"PRIVMSG #bitlbee :identify "+pass+"\r\n");
}

protected void write_callback(int fd)
{
	TELL(sprintf("%d",fd));
	socket_write(fd,"NICK "+users[fd]["id"]+"\r\n");
	return ;
}

protected void read_callback(int fd,mixed message)
{
	string str;
#ifdef DEBUG
	TELL(sprintf("[%s] %d,%s",
		TIME_D->replace_ctime(time()),fd,message ));
#endif DEBUG
	if(regexp(message ,"PING :PinglBee") ) //PING :PinglBee
	{
		socket_write(fd,"PONG\r\n");
	} 

	if( !mapp(users[fd]) ) return;

	switch(users[fd]["steps"])
	{
	case 0:
		str = sprintf("USER %s %s %s :%s\r\n",users[fd]["id"],
		    users[fd]["id"],query_ip_name(users[fd]["obj"]),
		    users[fd]["obj"]->query_name(1));
		socket_write(fd,str);
		break;
	case 1:
		str = sprintf("JOIN #bitlbee\r\n");
		socket_write(fd,str);
		break;
	case 2:
		str = sprintf("MODE #bitlbee\r\n");
		socket_write(fd,str);
		break;
	case 3:
		str = sprintf("WHO #bitlbee\r\n");
		socket_write(fd,str);
		str = sprintf("PRIVMSG #bitlbee :set charset=BIG5\r\n");
		socket_write(fd,str);
		break;
	default:
		{
			string name,mail,msg,id;

			foreach(string m in explode(message,"\r\n")) 
			{ 
				if(sscanf(m,":%s!%s PRIVMSG #bitlbee :%s",name,mail,msg) == 3 )
				{
					if(sscanf(msg,"The user %s wants to add you to his/her buddy list. Do you want to allow this?",mail) == 1)
					{
						tell(users[fd]["obj"],"\n[IM Message]:"+mail+"�Q�n��A�[�J�L���n�ͦW��A�P�N���ܥ� im yes�A���P�N���ܡA�� im no�C\n");
					}
					else if(sscanf(msg,"%s: %s",id,msg) == 2  && users[fd]["obj"]
					    && id == users[fd]["id"])
						tell(users[fd]["obj"],sprintf(HIW HBBLU"%s �ΧY�ɰT���ǵ��A : %s\n"NOR,
							users[fd]["list"][name][4]+"("+users[fd]["list"][name][0]
							+"@"+users[fd]["list"][name][1]+")\n["+name+"]",msg));
					else 
						switch(msg) {
					case "The nick is (probably) not registered":
						tell(users[fd]["obj"],"\n[IM Message]:�Ĥ@���ϥθӺ��F�A�۰ʵ��U�b���I\n");
						socket_write(fd,"PRIVMSG #bitlbee :register "+crypt(users[fd]["id"],users[fd]["id"])[0..10]+"\r\n");
						break;
					case "Incorrect password":
						tell(users[fd]["obj"],"\n[IM Message]:�n�J�K�X���~(�гq���t�κ޲z��)�I\n");
						break;
					case "YAHOO - Login error: Error 99 (Logged in on a different machine or device)":
						tell(users[fd]["obj"],"\n[IM Message]:�A�w�b��L�q���n�J YAHOO�I\n");
						break;
					case "MSN - Logged out: Someone else logged in with your account":
						tell(users[fd]["obj"],"\n[IM Message]:�A�w�b��L�q���n�J MSN �I\n");
						break;
					case "YAHOO - Signing off..":
						tell(users[fd]["obj"],"\n[IM Message]:�A�n�X YAHOO �I\n");
						break;
					case "MSN - Signing off..":
						tell(users[fd]["obj"],"\n[IM Message]:�A�n�X MSN �I\n");
						break;
					case "MSN - Logged in":
						tell(users[fd]["obj"],"\n[IM Message]:MSN �n�J���\�I\n");
						break;
					case "YAHOO - Logged in":
						tell(users[fd]["obj"],"\n[IM Message]:YAHOO �n�J���\�I\n");
						break;
					case "No accounts known. Use 'account add' to add one.":
						tell(users[fd]["obj"],"\n[IM Message]:�Шϥ� im register msn �άO im register yahoo ���U�b���I\n");
						break;
					case "MSN - Login error: Error during Passport authentication":
						tell(users[fd]["obj"],"\n[IM Message]:MSN�K�X���~�I\n");
						break;
					default:
					}
				}

				// �W���T�� :paiting!paiting@hotmail.com JOIN :#bitlbee
				if(sscanf(m,":%s!%s JOIN :#bitlbee",id,name) == 2)
				{
					send_who(fd,id);
					tell(users[fd]["obj"],"\n[IM Message]:"+id+"�W�u�F �I\n");
					continue;
				}

				// �U���T�� :paiting!paiting@hotmail.com QUIT :Leaving...
				if(sscanf(m,":%s!%s QUIT :Leaving...",id,name) == 2)
				{
					map_delete(users[fd]["list"],id);
					tell(users[fd]["obj"],"\n[IM Message]:"+id+"���u�F �I\n");
					continue;
				}

				// ���p�ܽu�W :root!root@rw.twku.net MODE #bitlbee +v paiting
				if(sscanf(m,":root!root@rw.twku.net MODE #bitlbee +v %s",id) == 1)
				{
					send_who(fd,id);
					continue;
				}

				// ���p��away�Φ��� :root!root@rw.twku.net MODE #bitlbee -v paiting
				if(sscanf(m,":root!root@rw.twku.net MODE #bitlbee -v %s",id) == 1)
				{
					send_who(fd,id);
					continue;
				}

				if(sscanf(m,":%*s 352 %*s %*s %*s %*s %*s %*s %*s :%*d %*s") == 10) 
				{
					process_who(fd,m);
				} else 
				if(sscanf(m,":%*s 352 %*s #bitlbee %*s %*s %*s %*s %*s :%*d %*s") == 9) 
				{
					process_who(fd,m);
				}


			}
		}
	}
	users[fd]["steps"]++;   
}

#ifdef DEBUG
void socket_write(int fd,mixed message)
{
	int i;

	TELL(sprintf(HIR"[%s] %d,%s"NOR,
		TIME_D->replace_ctime(time()),fd,message ));

	i = efun::socket_write(fd,message);

	switch(i)
	{
	case EEFDRANGE:       TELL("�y�z�� (descriptor) �W�X�d��C"); break;
	case EEBADF:          TELL("�L�Ī��y�z�ȡC"); break;
	case EESECURITY:      TELL("���ϹH�Ϧw���C"); break;
	case EENOADDR:        TELL("socket �����w��}�C"); break;
	case EEBADADDR:       TELL("��}�榡�����D�C"); break;
	case EENOTCONN:       TELL("socket �|���s���C"); break;
	case EEALREADY:       TELL("�ާ@�w�b�i�椤�C"); break;
	case EETYPENOTSUPP:   TELL("���䴩�����󫬺A�C"); break;
	case EESENDTO:        TELL("sendto �����D�C"); break;
	case EEMODENOTSUPP:   TELL("���䴩�� socket ���A�C"); break;
	case EEWOULDBLOCK:    TELL("�ާ@�N�|���� (block)�C"); break;
	case EESEND:          TELL("�e�X (send) �����D�C"); break;
	case EECALLBACK:      TELL("���ݦ^�I (callback) ���C"); break;
	default:

	}
}
#endif

protected void close_socket(int fd)
{
	if(users[fd]["obj"])
		delete_temp("im_fd", users[fd]["obj"]);

	map_delete(users,fd);
	socket_close(fd);
#ifdef DEBUG
	TELL(sprintf("[%s] %d,�s�u����",
		TIME_D->replace_ctime(time()),fd ));
#endif
}

void send_command(int fd,mixed message)
{
	socket_write(fd,message+"\r\n");
#ifdef DEBUG    
	TELL(sprintf("[%s] %d,%s",
		TIME_D->replace_ctime(time()),fd,message ));
#endif
}

// �B�z irc �u�W�ϥΪ�
varargs void send_who(int fd,string id)
{
	if(undefinedp(id))   
		socket_write(fd,"WHO #bitlbee\r\n");
	else    
		socket_write(fd,"WHO "+id+"\r\n");
}

void process_who(int fd,string str)
{
	string id,ip,status,nick,name;

	if(sscanf(str,":%*s 352 %*s #bitlbee %s %s %*s %s %s :%*d %s",
		id,ip,name,status,nick) == 9) 
		users[fd]["list"][name] = ({ id,ip,name,status,nick});

	if(sscanf(str,":%*s 352 %*s %*s %s %s %*s %s %s :%*d %s",
		id,ip,name,status,nick) == 10) 
		users[fd]["list"][name] = ({ id,ip,name,status,nick});
}

int process_send_msg(object me,string who,string msg)
{
	int fd = query_temp("im_fd", me);

	if(undefinedp(users[fd]) &&
	    !undefinedp(users[fd]["list"][who]))
		return tell(me, "[IM Message] �Y�ɰT���W���n�ͦW��S���o�ӤH�I\n");

	socket_write(fd,"PRIVMSG #bitlbee :"+ who + ":"+msg+"\r\n");
	{
		string *n = users[fd]["list"][who];
		
		// Unknown Bug ; Temporarily modified by clode
		if( !n || sizeof(n) != 5 ) return 1;
			
		tell(me,sprintf(HIW HBBLU"�A�ΧY�ɰT���ǵ� %s : %s\n"NOR,
			n[4]+"("+n[0]+"@"+n[1]+")\n["+who+"]",msg));
	}
	return 1;
}

int del_account(int fd,string protocol) 
{
	// �n�����u�A�~��R��
	socket_write(fd,"PRIVMSG #bitlbee :account off "+protocol+"\r\n");
	socket_write(fd,"PRIVMSG #bitlbee :account del "+protocol+"\r\n");
}

int register_account(int fd,string protocol,string account,string passwd)
{
	socket_write(fd,"PRIVMSG #bitlbee :account add "+protocol+" "+account+" "+passwd+"\r\n");
	socket_write(fd,"PRIVMSG #bitlbee :account on "+protocol+"\r\n");
}

mapping query_users(){ return users; }
string query_name()
{
	return "�����Y�ɳq�T�t��(IM_D)";
}
