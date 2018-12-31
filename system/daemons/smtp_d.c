/*
 * Project:		SMTP Client
 * Author:		Cwlin
 * Version:		1.0.0
 * Last Update Time:	2005/1/07 09:17 PM
 * Description:		Using mail(string to, string data, string subject) to mail.
 * Refer to:		RFC 821 - Simple Mail Transfer Protocol
 *			http://www.faqs.org/rfcs/rfc821.html
 */

/* Includes */
#include <socket.h>
#include <socket_err.h>

/* MUD Feature Includes */
#include <daemon.h>

/* Client Settings */
#define HOST_NAME	"rw.twku.net"
#define SENDER_NAME	"���ͪ��@��"
#define SENDER_FROM	"rw@" + HOST_NAME

/* Server Settings */
#define SMTP_ADDR	"127.0.0.1"
#define SMTP_PORT	"25"
#define SMTP_SERVER	SMTP_ADDR + " " + SMTP_PORT

//#define DEBUG
#define monitor(x)	CHANNEL_D->channel_broadcast("sys", "SMTP_D: " + x)

/* MAIL DATA */
#define MAIL_TO		0
#define MAIL_SUBJECT	1
#define MAIL_DATA	2

/* SMTP Process */
#define STEP_HELO	0
#define STEP_MAIL	1
#define STEP_RCPT	2
#define STEP_DATA	3
#define STEP_INPUT	4
#define STEP_NEXT	5

/* Special Process */
#define STEP_QUIT	11

nosave int socket_fd;
nosave int socket_write_fd = -1;
nosave int is_process = 0;
nosave int step = 0;
nosave string socket_write_buffer;
nosave string *mails = allocate(0);
nosave string *current_mail = allocate(3);

varargs void mail(string to, string data, string subject);
void do_mail();
varargs void do_next(int connect);
void socket_close_callback(int fd);
void socket_read_callback(int fd, mixed msg);
void socket_write_callback(int fd);
void smtp_process(int step);
string smtp_error(int reply_code);

void create()
{
}

void mail(string to, string data, string subject)
{
	string *mail = allocate(3);

	if( !to || !data )
	{
		error("mail() �w������ӰѼơA�ѼƤ����C");
		return;
	}
	mail[MAIL_TO] = to;
	mail[MAIL_DATA] = data;
	mail[MAIL_SUBJECT] = subject;
	mails += ({ mail });
	if( !is_process ) do_next();
}
void do_mail()
{
	int err;

	socket_fd = socket_create(STREAM, (: socket_read_callback :), (: socket_close_callback :));

	if( socket_fd < 0 )
	{
		monitor("Socket create error: " + socket_error(socket_fd));
		return;
	}
	err = socket_connect(socket_fd, SMTP_SERVER, (: socket_read_callback :), (: socket_write_callback :));

	if( err != EESUCCESS )
	{
		monitor("Socket connect error: " + socket_error(err));
		return;
	}
	return;
}

void socket_close_callback(int fd)
{
	socket_fd = 0;
	socket_write_fd = 0;
	return;
}

void socket_send(int fd, mixed msg)
{
	int res;

#ifdef DEBUG
	monitor("�e�X�T��: " + msg);
#endif
	// �|������ socket write callback �e�A�N���ǰe���T���s�J buffer
	if( socket_write_fd == -1 )
	{
		if( !socket_write_buffer ) socket_write_buffer = "";
		socket_write_buffer += msg;
		return;
	}
	// �p�G buffer �����T���|���e�X�A�h�P�����T���P�ɰe�X
	if( socket_write_buffer )
	{
		socket_write_buffer += msg;
		res = socket_write(fd, socket_write_buffer);
	}
	else res = socket_write(fd, msg);

	switch(res) {
	case EESUCCESS:		/* ���\ */
	case EECALLBACK:	/* ���ݧ��� */
		socket_write_buffer = 0;
		break;
	case EEALREADY:		/* �{�Ǥw�i�� */
	case EEWOULDBLOCK:	/* �{�ǰ��� */
	case EESEND:		/* �ǰe��ƿ��~ */
		if( !socket_write_buffer ) socket_write_buffer = msg;
		call_out((: socket_send :), 1, fd, "");
		break;
	default:		/* ��L���~ */
		monitor("socket_send: " + socket_error(res));
		socket_close(fd);
		break;
	}
}

/* �Ǧ^�i�g�J�� FD */
void socket_write_callback(int fd)
{
	socket_write_fd = fd;
	// �p�G���� socket write callback ��
	// buffer �����T�����ǰe�h�ߧY�ǰe�A�òM�� buffer
	if( socket_write_buffer )
	{
		socket_write(fd, socket_write_buffer);
		socket_write_buffer = 0;
	}
}

void socket_read_callback(int fd, mixed msg)
{
	int code;


#ifdef DEBUG
	monitor("�����T��: " + msg);
#endif

	if( !msg ) msg = "";
	msg = explode(msg, "\n");
	for(int i=0; i<sizeof(msg); i++)
	{
		// ���R�w�����T�����^���X
		sscanf(msg[i], "%d %*s", code);
		if( !code ) return;

#ifdef DEBUG
		monitor("Reply code: " + code);
#endif

		switch(code)
		{
		case 220:
			// Service ready
			smtp_process(step=0);
			break;
		case 221:
			// Service closing transmission channel
			smtp_process(STEP_NEXT);
			break;
		case 250:
			// Requested mail action okay, completed
			if( step == STEP_INPUT ) // Input completed and delivered
			{
				if( sizeof(mails) != 0 )
					monitor("�ѤU " + sizeof(mails) + " �� Email ����");
				else
					monitor("�w�����Ҧ� Email �ǰe");
			}
		case 354:
			// Start mail input; end with <CRLF>.<CRLF>
			smtp_process(++step); // �U�@�B
			break;
		case 251:
			// User not local; will forward to <forward-path>
		case 214:
			// System message
			monitor("��T: " + msg[i]);
			break;
		case 450:
		case 451:
		case 452:
		case 550:
		case 551:
		case 552:
		case 553:
		case 554:
			monitor("����: " + smtp_error(code));
			smtp_process(STEP_NEXT);
			break;
		case 421:
		case 500:
		case 501:
		case 502:
		case 503:
		case 504:
			monitor("���~: " + smtp_error(code));
			smtp_process(STEP_NEXT);
		default:
			monitor("�������T��: " + msg[i]);
			break;
		}
	}
	return;
}

void smtp_process(int step)
{
	switch(step)
	{
	case STEP_HELO:
		socket_send(socket_write_fd, "HELO " + HOST_NAME + "\n");
		break;
	case STEP_MAIL:
		socket_send(socket_write_fd, "MAIL FROM: " + SENDER_FROM + "\n");
		break;
	case STEP_RCPT:
		socket_send(socket_write_fd, "RCPT TO: " + current_mail[MAIL_TO] + "\n");
		break;
	case STEP_DATA:
		socket_send(socket_write_fd, "DATA\n");
		break;
	case STEP_INPUT:
		{
			/* Date, Subject, To, Cc, From */
			string data = "";
			//data += "Date: " + ctime(time()) + "\n";
			data += "From: " + SENDER_NAME + " <" + SENDER_FROM + ">\n";
			data += "To: " + current_mail[MAIL_TO] + "\n";
			if( current_mail[MAIL_SUBJECT] )
				data += "Subject: " + current_mail[MAIL_SUBJECT] + "\n";
			data += "\n" + current_mail[MAIL_DATA] + "\n";
			socket_send(socket_write_fd, data + ".\n");
			break;
		}
	case STEP_QUIT:
		socket_send(socket_write_fd, "QUIT\n");
		break;
	case STEP_NEXT:
		//call_out((: do_next :), 1, 1);
		do_next();
		break;
	default:
		monitor("smtp_process: unknown process code " + step);
		break;
	}
	return;
}

varargs void do_next(int connect)
{
	string *mail;

	if( !sizeof(mails) )
	{
		is_process = 0;
		smtp_process(STEP_QUIT);
		return;
	}
	is_process = 1;
	mail = (array)mails[0];
	current_mail = mail;
	mails = mails[1..];
	monitor("�ǰe Email �� " + current_mail[MAIL_TO] + " �� ...");
	if( connect )
		smtp_process(step=1);
	else
		do_mail();
}

string smtp_error(int reply_code)
{
	string msg;

	switch(reply_code)
	{
	case 500:
		// 500 Syntax error, command unrecognized.
		// [This may include errors such as command line too long]
		msg = "�榡���~�A�L�k���ѫ��O"; break;
	case 501:
		// 501 Syntax error in parameters or arguments
		msg = "�ѼƮ榡���~"; break;
	case 502:
		// 502 Command not implemented
		msg = "Command not implemented"; break;
	case 503:
		// 503 Bad sequence of commands
		msg = "Bad sequence of commands"; break;
	case 504:
		// 504 Command parameter not implemented
		msg = "Command parameter not implemented"; break;
	case 550:
		// 550 Requested action not taken: mailbox unavailable
		// [E.g., mailbox not found, no access]
		msg = "�S���o�� E-mail ��m"; break;
	case 551:
		// 551 User not local; please try <forward-path>
		msg = "User not local; please try <forward-path>"; break;
	case 552:
		//  552 Requested mail action aborted: exceeded storage allocation
		msg = "Requested mail action aborted: exceeded storage allocation"; break;
	case 553:
		// 553 Requested action not taken: mailbox name not allowed
		// [E.g., mailbox syntax incorrect]
		msg = "E-mail �榡���~"; break;
	case 554:
		// 554 Transaction failed
		msg = "�ǰe����"; break;
	default:
		msg = "Unknown reply code.";
		break;
	}
	return msg;
}

string query_name()
{
	return "�H��ǿ��w�t��(SMTP_D)";
}
