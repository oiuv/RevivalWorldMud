// socket_d.c
#include <daemon.h>
#include <socket.h>
#include <socket_err.h>

#define monitor(x)	CHANNEL_D->channel_broadcast("nch", "SOCKET_D: " + x)
#define PROCESS_TIMEOUT		30

mapping sockets = allocate_mapping(0);

mapping query_sockets()
{
	return sockets;
}

private void connect_timeout(int fd)
{
	if( undefinedp(sockets[fd]) ) return;
	if( objectp(sockets[fd]["owner"]) )
		evaluate(sockets[fd]["stat_callback"], "Unable to connect to remote host: Connection timeout");
	socket_close(fd);
	return;
}

private void receive_data(int fd, string msg)
{ 
	if( !sockets[fd]["owner"] )
	{
		socket_close(fd);
		return;
	}
	evaluate(sockets[fd]["rece_callback"], msg);
	return;
}

private void write_data(int fd)
{
	sockets[fd]["write_fd"] = fd;
	
	if( !undefinedp(sockets[fd]["connect_handle"]) )
	{
		remove_call_out(sockets[fd]["connect_handle"]);
		map_delete(sockets[fd], "connect_handle");
	}

	// �p�G���� socket write callback ��
	// buffer �����T�����ǰe�h�ߧY�ǰe�A�òM�� buffer
	if( sockets[fd]["write_buffer"] )
	{
		socket_write(fd, sockets[fd]["write_buffer"]);
		sockets[fd]["write_buffer"] = 0;
	}
	return;
}


void socket_close(int fd)
{
	monitor("socket_close: " + fd);
	efun::socket_close(fd);

	if( !undefinedp(sockets[fd]["connect_handle"]) )
	{
		remove_call_out(sockets[fd]["connect_handle"]);
		map_delete(sockets[fd], "connect_handle");
	}
	map_delete(sockets, fd);
	return;
}

void socket_send(int fd, mixed msg)
{
	int res;

#ifdef DEBUG
	monitor("�e�X�T��: " + msg);
#endif

	if( !msg || msg == "" ) return; 
	// �|������ socket write callback �e�A�N���ǰe���T���s�J buffer
	if( !sockets[fd]["write_fd"] )
	{
		if( !sockets[fd]["write_buffer"] )
			sockets[fd]["write_buffer"] = msg;
		else
			sockets[fd]["write_buffer"] += msg;
		return;
	}
	
	// �p�G buffer �����T���|���e�X�A�h�P�����T���P�ɰe�X
	if( sockets[fd]["write_buffer"] )
	{
		sockets[fd]["write_buffer"] += msg;
		res = socket_write(fd, sockets[fd]["write_buffer"]);
	}
	else res = socket_write(fd, msg);

	switch(res) {

	case EESUCCESS:		/* ���\ */
	case EECALLBACK:	/* ���ݧ��� */
		sockets[fd]["write_buffer"] = 0;
		break;
	case EEALREADY:		/* �{�Ǥw�i�� */
	case EEWOULDBLOCK:	/* �{�ǰ��� */
	case EESEND:		/* �ǰe��ƿ��~ */
		if( !sockets[fd]["write_buffer"] )
			sockets[fd]["write_buffer"] = msg;

		call_out((: socket_send :), 1, fd, "");
		break;
	default:		/* ��L���~ */
		evaluate(sockets[fd]["stat_callback"], socket_error(res));
		socket_close(fd);
		break;
	}
}

int socket_open(string host, int port, function rece_callback, function stat_callback)
{
	int socket_fd, res;
	string addr;

	monitor("socket_open: " + sprintf("%s %d %O %O", host, port, rece_callback, stat_callback));

	socket_fd = socket_create(STREAM, (: receive_data :), (: socket_close :));
	if( socket_fd < 0 )
	{
		evaluate(stat_callback, "socket_create: " + socket_error(socket_fd));
		return -1;
	}

	addr = sprintf("%s %d", host, port);
	res = socket_connect(socket_fd, addr, (: receive_data :), (: write_data :));

	if( res != EESUCCESS )
	{
		evaluate(stat_callback, "socket_connect: " + socket_error(res));
		socket_close(socket_fd);
		return -1;
	}

	sockets[socket_fd] = allocate_mapping(0);
	sockets[socket_fd]["addr"] = host;
	sockets[socket_fd]["port"] = port;
	sockets[socket_fd]["owner"] = previous_object();
	sockets[socket_fd]["rece_callback"] = rece_callback;
	sockets[socket_fd]["stat_callback"] = stat_callback;
	sockets[socket_fd]["connect_handle"] = call_out((: connect_timeout :), PROCESS_TIMEOUT, socket_fd);
	return socket_fd;
}
string query_name()
{
	return "SOCKET �t��(SOCKET_D)";
}
