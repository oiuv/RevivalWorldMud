/*
 * Project:     FTP Server
 * Author:      Cwlin
 * Version:     1.2.6
 * Last Update Time:
 *		2005/4/17 10:05 PM
 * Note:
 * 2003/7/22 09:02 PM	- Fixed -	List �� File Transfer ���i��]��Ʃ|��
 *					�����ǰe�Y������ǰe�������T���ɭP���_
 * 2005/4/17 10:05 PM	- Modify -	���T�ŧi�p�����
 *			- Add -		�s�W�۰ʧ��w�ϥ� port �ﶵ
 *			- Modify -	�ץ��ܼƩR�W	
 * 2006/6/30 03:06 AM	- Modify -	�ץ��{�ɤU�����_�ɪ����~
 * 
 ****************************
 * �y�{��:
 *      1. �إ߳s��(Socket create) ->		�����^��(Read Callback)
 *						�����^��(Close Callback)
 *
 *      2. �]�w�s����(Socket Bind)
 *
 *      3. ���ݳs��(Socket Listen) ->		���ݦ^��(Listen Callback)
 *      4. ���ݦ^��(Listen Callback) ->		�s������(Socket Accept)
 *
 *      5. �s������(Socket Accept) ->		�����^��(Read Callback)
 *						�w�ǳƼg�J�^��(Write Callback)
 *
 *      6. �w�ǳƼg�J�^��(Write Callback) ->	�s���g�J(Socket Write)
 *
 *      7. ��}(Resolve) ->			��}�^��(Resolve Callback)
 */
/* Includes */
#include <socket.h>
#include <socket_err.h>

/* MUD Feature Includes */
#include <daemon.h>
#include <secure.h>
#include <mudlib.h>

/* Defines */

/*
#define MUD_NAME		"Revival World"
*/
#define FTPD_VERSION		"1.2.6"
#define SEC_IN_YEAR		365 * 24 * 60 * 60

#define HOST_IP                 "59.124.167.216"
#define SOCKET_PORT             4121
#define MAX_PORT_INCRE		3	// Auto increase port when specific port are using
#define TRANSFER_MSG_DELAY	3	// The message delay when recevied data 

/* �۰ʰ����D�� IP */
#undef AUTO_DETECT_HOSTIP

#define MAX_BYTE_TRANSFER	102400	// Specific in MudOS options.h
#define ERROR_LOG		"ftp_d"

#define log_error(x)		log_file(ERROR_LOG, sprintf("%s", x))
#define user_exist(x)		(SECURE_D->is_wizard(x))
#define monitor(x)		CHANNEL_D->channel_broadcast("nch", "FTP_D: " + x)


/* Global variables */
private nosave int socket_fd;		// Port Server FD
private nosave int data_callback_time;
private nosave string host_ip;		// Host Ip
private nosave mapping sockets;		// Sockets
private nosave mapping data_sockets;	// Data Sockets
private nosave mapping resolves;	// Resolve() Keys

/* Prototypes */
private void close_connection(int fd);
private void close_data_connection(int fd);
private void socket_send(int fd, string arg);		// Sending normal message
private void socket_send_file(int fd, string path);	// Sending file
private void socket_send_list(int fd, string listing);	// Sending list

private void write_callback(int fd);
private void read_callback(int fd, string str);
private void close_callback(int fd);
private void listen_callback(int fd);
private void resolve_callback(string addr, string ip, int key);

private void ftp_login(int fd);
private void ftp_input(int fd, string str);

private void data_read_callback(int fd, mixed msg);
private void data_write_callback(int fd);
private void data_close_callback(int fd);
private void data_listen_callback(int fd);

string memory_expression(int m);

/* MUD Feature Functions */
/* ���o�b���K�� */
private string get_passwd(string user)
{
	return PASSWORD_D->query_password(user);
}
/* ���o�ثe�u�@�ؿ� */
private string get_pwd(string user)
{
	return wiz_home(user);
}
/* ���o���| */
private string get_path(string pwd, string file)
{
	if( pwd[<1] != '/' ) pwd += "/";
	return resolve_path(pwd, file);
}
/* ���O - �ؿ��M�� */
private string cmd_list(string pwd, string arg, string id)
{
	string path, last_mdfy, auth, res = "";
	int file_size;
	array file, files;

	if( pwd[<1] != '/' ) pwd += "/";
	path = resolve_path(pwd, arg);
	if( path[<1] != '/' ) path += "/";
	files = get_dir(path, -1);
	if( !sizeof(files) )
	{
		if( file_size(path) == -2 ) return "";
		return 0;
	}
	foreach(file in files)
	{
		/*
		File name = file[0];
		File size = file[1];
		Last modify time = file[2];
		*/
		file_size = file[1];
		if( file[0][<1] == '.' ) continue;
		if( file_size == -2 )
		{
			auth = "d";
			file_size = 4096;
		}
		else	auth = "-";
		if( !SECURE_D->permission(path + file[0], id, READ, 0) )
			auth += "-";
		else	auth += "r";
		if( !SECURE_D->permission(path + file[0], id, WRITE, 0) )
			auth += "-";
		else	auth += "w";
		if( !SECURE_D->permission(path + file[0], id, COMPILE, 0) )
			auth += "-";
		else if( file[0][<2..] == ".c" )
			auth += "x";
		else	auth += "-";
		auth += "------";

		if( file[2] + SEC_IN_YEAR < time() )
			last_mdfy = sprintf("%6s %5s", ctime(file[2])[4..9], ctime(file[2])[<4..<0]);
		else	last_mdfy = sprintf("%6s %5s", ctime(file[2])[4..9], ctime(file[2])[11..<9]);

		res += sprintf("%10s %4d %-8d %-8d %8d %12s %s\r\n",
		    auth, 1, 0, 0, file_size, last_mdfy, file[0]);
	}
	return res;
}
/* ���O - �ؿ��M��(²�u) */
private string cmd_nlist(string pwd, string arg)
{
	string path, res = "";
	array files;

	if( pwd[<1] != '/' ) pwd += "/";
	path = resolve_path(pwd, arg);
	if( path[<1] != '/' ) path += "/";
	files = get_dir(path);
	if( !sizeof(files) )
	{
		if( file_size(path) == -2 ) return "";
		return 0;
	}
	foreach(string file in files)
	{
		if( file[<1] == '.' ) continue;
		res += sprintf("%s\r\n", file);
	}
	return res;
}
/* ���O - ���u�@�ؿ� */
private string cmd_cwd(string pwd, string arg)
{
	string path;
	if( pwd[<1] != '/' ) pwd += "/";
	path = resolve_path(pwd, arg);
	if( path != "/" && path[<1] == '/' ) path = path[0..<2];
	if( file_size(path) != -2 ) return 0;
	return path;
}


/* Functions */
private void setup()
{
	int res, port;

	/* �إ� socket */
	socket_fd = socket_create(STREAM, (: read_callback :), (: close_callback :));
	if( socket_fd < 0 )
	{
		monitor("��l���~: socket_create: " + socket_error(socket_fd)); 
		log_error("setup: socket_create: " + socket_error(socket_fd) + ".\n");
		return;
	}

	/* �]�w socket �s���� */
	port = SOCKET_PORT;
	do
	{
		res = socket_bind(socket_fd, SOCKET_PORT);
		if( res < 0 )
		{
			monitor("��l���~: socket_bind: " + socket_error(res)); 
			log_error("setup: socket_bind: " + socket_error(res) + ".\n");
			if( port - SOCKET_PORT > MAX_PORT_INCRE )
			{
				socket_close(socket_fd);
				return;
			}

		}
	}
	while(res < 0 && port++);

	/* �}�l���� socket �s�� */
	res = socket_listen(socket_fd, (: listen_callback :));
	if( res < 0 )
	{
		socket_close(socket_fd);
		monitor("��l���~: socket_listen: " + socket_error(res)); 
		log_error("setup: socket_listen: " + socket_error(res) + ".\n");
	}
	monitor("�Ұʩ� " + host_ip + ":" + port); 
}


/* socket �ǰe��Ƨ����q�� */
private void write_callback(int fd) 
{
	monitor("write_callback: " + fd + ".");
}

/* socket ������Ƴq�� */
private void read_callback(int fd, string str)
{
	foreach(string cmd in explode(str, "\n"))
	ftp_input(fd, cmd);
}

/* socket �����q�� */
private void close_callback(int fd)
{
	if( undefinedp(sockets[fd]) ) return;
	if( !undefinedp(sockets[fd]["login"]) )
		monitor(sprintf("%s(%s) �_�u�C", sockets[fd]["user"], sockets[fd]["address"]));
	map_delete(sockets, fd);
}

/* socket �s���q�� */
private void listen_callback(int fd) 
{
	int res_fd, port, key;
	string addr;

	/* �����s�� */
	res_fd = socket_accept(fd, (: read_callback :), (: write_callback :));
	if( res_fd < 0 )
	{
		log_error("listen_callback: " + socket_error(res_fd) + ".\n");
		return;
	}

	sscanf(socket_address(res_fd), "%s %d", addr, port);

	sockets[res_fd] = ([
	    "address" : addr,		// IP ��m
	    "name" : addr,		// ����W��
	    "port" : port,		// ���ݰ�
	    "time" : time(),		// �s���ɶ�
	    "type" : "ascii",		// ��ƶǰe����
	    "rest" : 0,			// ��ƶǰe�_�l�줸
	    "write_status" : EESUCCESS 	// �s�����A
	]);

	/* ��}���o����W�� */
	key = resolve(addr, (: resolve_callback :));
	resolves[key] = res_fd;

	/* �n�J�{�� */
	ftp_login(res_fd);
}

/* ��}�q�� */
private void resolve_callback(string addr, string ip, int key)
{
	if( addr ) {
		int fd;
		fd = resolves[key];
		if( fd && sockets[fd] ) sockets[fd]["name"] = addr;
	}
	map_delete(resolves, key);
}

/* �ǰe��Ʀ� socket */
private void socket_send(int fd, string arg)
{
	int res;

	if( !undefinedp(sockets[fd]["pending"]) )
	{
		sockets[fd]["pending"] += arg;
		res = socket_write(fd, sockets[fd]["pending"]);
	}
	else	res = socket_write(fd, arg);

	sockets[fd]["write_status"] = res;
	switch(res) {
	case EESUCCESS:		/* ���\ */
	case EECALLBACK:	/* ���ݧ��� */
		if( !undefinedp(sockets[fd]["pending"]) )
			map_delete(sockets[fd], "pending");
		break;
	case EEALREADY:		/* �{�Ǥw�i�� */
	case EEWOULDBLOCK:	/* �{�ǰ��� */
	case EESEND:		/* �ǰe��ƿ��~ */
		if( undefinedp(sockets[fd]["pending"]) )
			sockets[fd]["pending"] = arg;
		call_out((: socket_send :), 1, fd, "");
		break;
	default:		/* ��L���~ */
		log_error("socket_send: " + socket_error(res) + ".\n");
		socket_close(fd);
		break;
	}
}

private void socket_send_file(int fd, string arg)
{
	int i, len, pos;
	string path;

	if( undefinedp(sockets[fd]) ) return;
	pos = sockets[fd]["rest"];
	path = get_path(sockets[fd]["pwd"], arg);
	if( undefinedp(sockets[fd]["data_fd"]) ) return;
	if( (len = file_size(path)) < 0 )
	{
		socket_send(fd, "550 Failed to open file.\n");
		return;
	}
	if( undefinedp(sockets[fd]["retr"]) )
	{
		sockets[fd]["retr"] = path;
		sockets[fd]["retr_len"] = len;
		data_sockets[sockets[fd]["data_fd"]] = fd;
		socket_send(fd, "150 Opening " + upper_case(sockets[fd]["type"]) + " mode data connection for " + arg + " (" + (len - sockets[fd]["rest"]) + " bytes).\n");
		monitor(sprintf("%s �}�l�U���ɮ� %s�C", sockets[fd]["user"], path));
	}
	while(pos <= len)
	{
		if( undefinedp(sockets[fd]["data_fd"]) ) break;
		i = socket_write(sockets[fd]["data_fd"], read_buffer(path, pos, MAX_BYTE_TRANSFER));
		if( i < 0 && i != EECALLBACK ) break;
		pos += ((len - pos) > MAX_BYTE_TRANSFER ? MAX_BYTE_TRANSFER: (len - pos));
	}
	if( i == EEWOULDBLOCK || i == EEALREADY  )
	{
		sockets[fd]["rest"] = pos;
		sockets[fd]["send_handle"] = call_out((: socket_send_file :), 1, fd, path);
		return;
	}
	if( pos == len )
	{
		monitor(sprintf("%s �U���ɮ� %s �����A�@ %s�C",
			sockets[fd]["user"],
			path,
			memory_expression(sockets[fd]["retr_len"])
		    ));
		socket_send(fd, "226 File send OK.\n");
		sockets[fd]["rest"] = 0;
		map_delete(data_sockets, fd);
		map_delete(sockets[fd], "retr");
		map_delete(sockets[fd], "retr_len");
		close_data_connection(fd);
		return;
	}
	return;
}

private void socket_send_list(int fd, string listing)
{
	int i;

	if( undefinedp(sockets[fd]["data_fd"]) ) return;
	if( undefinedp(sockets[fd]["list"]) )
	{
		sockets[fd]["list"] = 1; // Send list flag
		socket_send(fd, "150 Here comes the directory listing.\n");
	}
	i = socket_write(sockets[fd]["data_fd"], listing);
	if( i < 0 )
	{
		// monitor("socket_send_list: " + socket_error(i)); 
		sockets[fd]["send_handle"] = call_out((: socket_send_list :), 1, fd, listing);
		return;
	}
	close_data_connection(fd);
	socket_send(fd, "226 Directory send OK.\n");
	map_delete(sockets[fd], "list");
	return;
}

/* ���� socket �s�u */
private void close_connection(int fd)
{
	/* �Y�ǰe��Ƶ{�ǰe�|������, �~��s�u */
	if( sockets[fd]["write_status"] == EECALLBACK ) return;
	map_delete(sockets, fd);
	socket_close(fd);
}

private void close_data_connection(int fd)
{
	if( undefinedp(sockets[fd]) ) return;
	socket_close(sockets[fd]["data_fd"]);
	map_delete(sockets[fd], "data_conn");
	map_delete(sockets[fd], "data_fd");
}

#ifdef AUTO_DETECT_HOSTIP
void hostip_resolve_callback(string addr, string ip, int key)
{
	if( ip )
	{
		host_ip = ip;
		setup();
		return;
	}
	log_error("create: auto_detect_hostip: cannot detect host ip.\n");
}
#endif

void create()
{
	/* ��l���ܼ� */
	sockets = allocate_mapping(0);
	data_sockets = allocate_mapping(0);
	resolves = allocate_mapping(0);

#ifdef AUTO_DETECT_HOSTIP
	/* �۰ʰ����D���W�� */
	{
		string host = query_host_name();
		resolve(host, (: hostip_resolve_callback :));
	}
#else
	host_ip = HOST_IP;
	/* �Ұ� */
	setup();
#endif
}

/* �n�J�{�� */
private void ftp_login(int fd)
{
	socket_send(fd, "220 "MUD_NAME" FTP server (Version "FTPD_VERSION" (MudOS/LPC)) ready.\n");
}

/* ���O�B�z�{�� */
private void ftp_input(int fd, string str)
{
	string *command, verb, arg;

	/* �L�o���O�榡 */
	if( !str ) return;
	if( str[<1] == '\n' ) str = str[0..<2];
	if( str[<1] == '\r' ) str = str[0..<2];
	if( str == "" ) return;

	command = explode(str, " ");
	if( !sizeof(command) ) return;
	verb = command[0];
	verb = lower_case(verb);
	arg = implode(command[1..], " ");

	/* ���n�J�e����ϥΫ��O */
	if( undefinedp(sockets[fd]["login"]) && member_array(verb, ({ "user", "pass", "quit" })) == -1 )
	{
		socket_send(fd, "530 Please login with USER and PASS.\n");
		return;
	}

	/* �O�����O */
	sockets[fd]["lastcmd"] = str;

	switch(verb)
	{
	case "user":	/* �ϥΪ̱b�� */
		sockets[fd]["user"] = arg;
		socket_send(fd, "331 Please specify the password.\n");
		break;
	case "pass":	/* �ϥΪ̱K�X */
		if( undefinedp(sockets[fd]["user"]) )
		{
			socket_send(fd, "503 Login with USER first.\n");
			break;
		}
		{
			string pass = get_passwd(sockets[fd]["user"]);
			if( user_exist(sockets[fd]["user"]) && crypt(arg, pass) == pass )
			{
				string pwd = get_pwd(sockets[fd]["user"]);

				socket_send(fd, "230 Login successful.\n");
				monitor(sprintf("%s(%s) �n�J�C", sockets[fd]["user"], sockets[fd]["address"]));
				if( file_size(pwd) != -2 ) pwd = "/";
				sockets[fd]["pwd"] = pwd;
				sockets[fd]["login"] = 1;
				break;
			}
			socket_send(fd, "530 Login incorrect.\n");
		}
		break;
	case "type":	/* ��ƶǰe���� */
		arg = lower_case(arg);
		if( arg == "i" )
		{
			sockets[fd]["type"] = "binary";
			socket_send(fd, "200 Switching to Binary mode.\n");
			break;
		}
		if( arg == "a" )
		{
			sockets[fd]["type"] = "ascii";
			socket_send(fd, "200 Switching to ASCII mode.\n");
			break;
		}
		socket_send(fd, "500 Unrecognised TYPE command.\n" );
		break;
	case "rest":	/* ��ƶǰe�_�l�줸 */
		{
			int pos;
			sscanf(arg, "%d", pos);
			socket_send(fd, "350 Restart position accepted (" + pos + ").\n");
			sockets[fd]["rest"] = pos;
		}
		break;
	case "pwd":	/* �ثe�u�@�ؿ� */
		socket_send(fd, "257 \"" + sockets[fd]["pwd"] + "\"\n");
		break;
	case "abor":	/* ���_�ǿ�ʧ@ */
		socket_send(fd, "426 Transfer aborted. Data connection closed.\n");
		monitor(sprintf("%s ���_�ɮ׶ǿ�ʧ@�C", sockets[fd]["user"]));
		if( !undefinedp(sockets[fd]["send_handle"]) )
		{
			if( find_call_out(sockets[fd]["send_handle"]) )
				remove_call_out(sockets[fd]["send_handle"]);
			monitor(sprintf("%s �w�����ɮ׶ǿ�I�s�C", sockets[fd]["user"]));
			map_delete(sockets[fd], "send_handle");
		}
		sockets[fd]["rest"] = 0;

		map_delete(sockets[fd], "list");
		map_delete(sockets[fd], "retr");
		map_delete(sockets[fd], "retr_len");
		map_delete(sockets[fd], "stor");
		map_delete(sockets[fd], "stor_len");
		close_data_connection(fd);
		socket_send(fd, "226 Abort successful.\n");
		break;
	case "port":	/* �Ȥ�ݭn�D���A���s�W�Ȥ��*/
		{
			string *args, data_addr;
			int data_mode, data_port, data_fd, res;

			args = explode(arg, ",");
			if( sizeof(args) < 6 )
			{
				socket_send(fd, "500 Illegal PORT command.\n");
				break;
			}
			data_addr = implode(args[0..3], ".");
			data_port = (to_int(args[4]) << 8) + to_int(args[5]);
			if( sockets[fd]["type"] == "ascii" ) data_mode = STREAM;
			if( sockets[fd]["type"] == "binary" ) data_mode = STREAM_BINARY;
			data_fd = socket_create(data_mode, (: data_read_callback :), (: data_close_callback :));
			if( data_fd < 0 )
			{
				socket_send(fd, "425 Can't create data socket.\n");
				break;
			}
			res = socket_connect(data_fd,
			    data_addr + " " + data_port,
			    (: data_read_callback :), (: data_write_callback :));
			if( res < 0 )
			{
				socket_close(data_fd);
				socket_send(fd, "425 Can't build data connection.\n");
			}
			sockets[fd]["data_conn"] = "port";
			sockets[fd]["data_fd"] = data_fd;
			data_sockets[data_fd] = fd;
			socket_send(fd, "200 PORT command successful.\n");
		}
		break;
	case "pasv":	/* ���A���ݭn�D�Ȥ�ݳs�W���A���� */
		{       
			int data_mode, data_fd, port, res;
			string host;
			if( sockets[fd]["type"] == "ascii" ) data_mode = STREAM;
			if( sockets[fd]["type"] == "binary" ) data_mode = STREAM_BINARY;
			data_fd = socket_create(data_mode, (: data_read_callback :), (: data_close_callback :));

			if( data_fd < 0 )
			{
				socket_send(data_fd, "425 Can't create data socket.\n");
				break;
			}

			do
			{
				port = random((1 << 16) - (1 << 10)) + (1 << 10);
				res = socket_bind(data_fd, port);
			}
			while(res == EEADDRINUSE);

			if( res < 0 )
			{
				socket_close(socket_fd);
				socket_send(fd, "425 Can't bind socket port.\n");
				return;
			}
			res = socket_listen(data_fd, (: data_listen_callback :));
			if( res < 0 )
			{
				socket_close(socket_fd);
				socket_send(fd, "425 Can't bind listen socket.\n");
				return;
			}
			host = host_ip;
			host = replace_string(host, ".", ",");
			socket_send(fd, sprintf("227 Entering Passive Mode (%s,%d,%d)\n",
				host, (port >> 8), port - ((port >> 8) << 8)));
			sockets[fd]["data_conn"] = "pasv";
			data_sockets[data_fd] = fd;
		}
		break;
	case "list":	/* �ؿ��M�� */
		if( undefinedp(sockets[fd]) ) return;
		if( undefinedp(sockets[fd]["data_fd"]) )
		{
			if( undefinedp(sockets[fd]["data_conn"]) )
			{
				socket_send(fd, "425 Use PORT or PASV first.\n");
				break;
			}
			call_out((: ftp_input :), 1, fd, str);
			break;
		}
		{
			string list = cmd_list(sockets[fd]["pwd"], arg, sockets[fd]["user"]);
			if( !list )
			{
				socket_send(fd, "550 No files found.\n");
				close_data_connection(fd);
				break;
			}
			socket_send_list(fd, list);
		}
		break;
	case "nlst":	/* �ؿ��M��(²�u) */
		if( undefinedp(sockets[fd]) ) return;
		if( undefinedp(sockets[fd]["data_fd"]) )
		{
			if( undefinedp(sockets[fd]["data_conn"]) )
			{
				socket_send(fd, "425 Use PORT or PASV first.\n");
				break;
			}
			call_out((: ftp_input :), 1, fd, str);
			break;
		}
		{
			string list = cmd_nlist(sockets[fd]["pwd"], arg);
			if( !list )
			{
				socket_send(fd, "550 No files found.\n");
				close_data_connection(fd);
				break;
			}
			socket_send_list(fd, list);
		}
		break;
	case "cdup":	/* ���ܤu�@�ؿ��ܤW�@�h */
		arg = "..";
	case "cwd":	/* ���ܥثe�u�@�ؿ� */
		{
			string pwd = cmd_cwd(sockets[fd]["pwd"], arg);
			if( arg != "" && pwd )
			{
				sockets[fd]["pwd"] = pwd;
				socket_send(fd, "250 Directory successfully changed.\n");
				break;
			}
			socket_send(fd, "550 Failed to change directory.\n");
		}
		break;
	case "retr":	/* �����ɮ� */
		if( undefinedp(sockets[fd]) ) return;
		if( undefinedp(sockets[fd]["data_fd"]) )
		{
			if( undefinedp(sockets[fd]["data_conn"]) )
			{
				socket_send(fd, "425 Use PORT or PASV first.\n");
				break;
			}
			call_out((: ftp_input :), 1, fd, str);
			break;
		}
		socket_send_file(fd, arg);
		break;
	case "stor":	/* �ǰe�ɮ� */
		if( undefinedp(sockets[fd]) ) return;
		if( undefinedp(sockets[fd]["data_fd"]) )
		{
			if( undefinedp(sockets[fd]["data_conn"]) )
			{
				socket_send(fd, "425 Use PORT or PASV first.\n");
				break;
			}
			call_out((: ftp_input :), 1, fd, str);
			break;
		}
		{
			string path = get_path(sockets[fd]["pwd"], arg);
			if( file_size(path) != -1 && !sockets[fd]["rest"] && !write_file(path, "", 1) )
			{
				socket_send(fd, "553 Could not create file.\n");
				break;
			}
			sockets[fd]["stor"] = path;
			sockets[fd]["stor_len"] = 0;
			socket_send(fd, "150 Ok to send data.\n");
			monitor(sprintf("%s �}�l�W���ɮ� %s�C", sockets[fd]["user"], path));
			data_sockets[sockets[fd]["data_fd"]] = fd;
		}
		break;
	case "dele":	/* �R���ɮ� */
		{
			string path = get_path(sockets[fd]["pwd"], arg);
			if( rm(path) )
			{
				socket_send(fd, "250 Delete operation successful.\n");
				monitor(sprintf("%s �R���ɮ� %s�C", sockets[fd]["user"], path));
				break;
			}
			socket_send(fd, "550 Delete operation failed.\n");
		}
		break;
	case "mkd":	/* �إߥؿ� */
		{
			string path = get_path(sockets[fd]["pwd"], arg);
			if( mkdir(path) )
			{
				socket_send(fd, "257 \"" + path + "\" created.\n");
				monitor(sprintf("%s �إߥؿ� %s�C", sockets[fd]["user"], path));
				break;
			}
			socket_send(fd, "550 Create directory operation failed. \n");
		}
		break;
	case "rmd":	/* �R���ؿ� */
		{
			string path = get_path(sockets[fd]["pwd"], arg);
			if( rmdir(path) )
			{
				socket_send(fd, "250 Remove directory operation successful.\n");
				monitor(sprintf("%s �R���ؿ� %s�C", sockets[fd]["user"], path));
				break;
			}
			socket_send(fd, "250 Remove directory operation failed.\n");
		}
		break;
	case "rnfr":	/* ���W�ٱq */
		{
			string path = get_path(sockets[fd]["pwd"], arg);
			sockets[fd]["rnfr"] = path;
			socket_send(fd, "350 Ready for RNTO.\n");
		}
		break;
	case "rnto":	/* ���W�٦� */
		{
			string path = get_path(sockets[fd]["pwd"], arg);
			sockets[fd]["rnto"] = path;
			if( !rename(sockets[fd]["rnfr"], sockets[fd]["rnto"]) )
			{
				socket_send(fd, "250 Rename successful.\n");
				monitor(sprintf("%s ��� %s �ɮצW�٬� %s�C", sockets[fd]["user"], sockets[fd]["rnfr"], sockets[fd]["rnto"]));
				break;
			}
			socket_send(fd, "250 Rename failed.\n");
		}
		break;
	case "syst":	/* ��ܥثe���A�����t������ */
		socket_send(fd, "215 MUD Type: " + MUDLIB_NAME +"\n");
		break;
	case "quit":	/* ���} */
		socket_send(fd, "221 Goodbye.\n");
		if( !undefinedp(sockets[fd]["login"]) )
			monitor(sprintf("%s(%s) ���}�C", sockets[fd]["user"], sockets[fd]["address"]));

		close_data_connection(fd);
		close_connection(fd);
		break;
	default:	/* ��L���O */
		socket_send(fd, "500 Unknown command.\n");
		break;
	}
}

/* socket �ǰe��Ƨ����q�� */
private void data_write_callback(int fd)
{
	int percentage; 

	if( undefinedp(sockets[data_sockets[fd]]["retr"]) ) return;
	//if( !data_callback_time ) data_callback_time = time();
	//if( (time() - data_callback_time) < TRANSFER_MSG_DELAY ) return;
	data_callback_time = time();
	percentage = to_int(to_float(sockets[data_sockets[fd]]["rest"]) / sockets[data_sockets[fd]]["retr_len"] * 100);

	monitor(sprintf("%s �U���ɮ� %s �i�� %s/%s (%d%%)",
		sockets[data_sockets[fd]]["user"],
		sockets[data_sockets[fd]]["retr"],
		memory_expression(sockets[data_sockets[fd]]["rest"]),
		memory_expression(sockets[data_sockets[fd]]["retr_len"]),
		percentage,
	    ));
}

/* socket ��Ʊ����q�� */
private void data_read_callback(int fd, mixed msg)
{
	int size = (stringp(msg) ? strlen(msg):sizeof(msg));
	// If data mode are using Binary the msg type is array
	// If data mode are using Ascii the msg type is string

	if( undefinedp(sockets[data_sockets[fd]]["stor"]) ) return;
	if( size > MAX_BYTE_TRANSFER )
	{
		monitor(sprintf("%s �ǿ�e�׶W�L�g�J�����ɮ� %s �i��l���C",
			sockets[data_sockets[fd]]["user"],
			sockets[data_sockets[fd]]["stor"]));
	}
	write_buffer(sockets[data_sockets[fd]]["stor"], sockets[data_sockets[fd]]["rest"], msg);
	sockets[data_sockets[fd]]["rest"] += size; 	// �ɮפj�p (��Ǧ�m + �W�Ǥj�p)
	sockets[data_sockets[fd]]["stor_len"] += size; 	// �����W�Ǥj�p (�W�Ǥj�p)
}

/* socket �����q�� */
private void data_close_callback(int fd)
{
	if( undefinedp(data_sockets[fd]) ) return;
	if( undefinedp(sockets[data_sockets[fd]]["stor"]) ) return;

	socket_send(data_sockets[fd], "226 File receive OK.\n");
	monitor(sprintf("%s �W���ɮ� %s �����A�@ %s�C",
		sockets[data_sockets[fd]]["user"],
		sockets[data_sockets[fd]]["stor"],
		memory_expression(sockets[data_sockets[fd]]["stor_len"]),
	    ));

	close_data_connection(data_sockets[fd]);

	sockets[data_sockets[fd]]["rest"] = 0;

	map_delete(sockets[data_sockets[fd]], "stor");
	map_delete(sockets[data_sockets[fd]], "stor_len");
	map_delete(data_sockets, fd);
}

/* socket �s���q�� */
private void data_listen_callback(int fd)
{
	int res_fd;

	res_fd = socket_accept(fd, (: data_read_callback :), (: data_write_callback :));
	if( res_fd < 0 )
	{
		log_error("data_listen_callback: " + socket_error(res_fd) + ".\n");
		return;
	}
	sockets[data_sockets[fd]]["data_fd"] = res_fd;
	map_delete(data_sockets, fd);
	socket_close(fd);
}

mapping query_socket(int fd)
{
	return sockets[fd];
}

mapping query_sockets() 
{
	return sockets;
}

mapping query_resolves()
{
	return resolves;
}

string memory_expression(int m)
{ 
	float mem = to_float(m);
	if( mem<1024 ) return sprintf("%.1f Bytes", mem);
	if( mem<1024*1024 ) return sprintf("%.1f KB", (float)mem / 1024);
	return sprintf("%.2f MB", (float)mem / (1024*1024));
}
string query_name()
{
	return "�ɮ׶ǿ�t��(FTP_D)";
}
