#include <daemon.h>
#define monitor(x)	CHANNEL_D->channel_broadcast("nch", "SOCKET_OB: " + x)

int fd;
void rece_callback(mixed msg)
{
	if( stringp(msg) ) monitor(msg);
	SOCKET_D->socket_close(fd);
}
void stat_callback(string err)
{
	monitor(err);
}
void create()
{
	fd = SOCKET_D->socket_open("210.59.67.253", 4001, (: rece_callback :), (: stat_callback :));
	return;
}

string query_name()
{
	return "SOCKET_OB �t��(SOCKET_OB)";
}