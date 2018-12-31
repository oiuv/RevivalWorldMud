/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : network_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-9-12
 * Note   : ������T���F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>

nomask string query_network_short(object user)
{
	string network_short;
	mapping login;
	
	if( !objectp(user) )
		return 0;

	login = query_temp("login", user);
	
	if( !mapp(login) ) login = allocate_mapping(0);
	
	network_short = sprintf("<%s/%s/%s/%s/%s/%s>",
		capitalize(SECURE_D->level_name(user->query_id(1))),
		query_ip_number(user)||login["ip"]||"-",
		query_ip_name(user)||login["ip_name"]||"-",
		(query_ip_port(user)||login["ip_port"]||"-")+"",
		login["terminaltype"]||"-",
		login["ident"]||"-",
		);

	network_short = replace_string(network_short, "\n", "");
	return network_short;
}

string dsize(string size)
{
	if( count(size, "<", "1024") )
		return size + " Bytes";
	
	if( count(size, "<", "1048576") )
		return count(size, "/", "1024")+" KBytes";
		
	if( count(size, "<", "1073741824") )
		return count(size, "/", "1048576")+" MBytes";
		
	if( count(size, "<", "1099511627776") )
		return count(size, "/", "1073741824")+" GBytes";

	if( count(size, "<", "1125899906842624") )
		return count(size, "/", "1099511627776")+" TBytes";
	
}

string query_network_packet_stats()
{
	mapping networkstats = network_stats();

	return sprintf("�w���� %s �ʥ](%.2f/sec)�A�w�ǰe %s �ʥ](%.2f/sec)", NUMBER_D->number_symbol(networkstats["incoming packets total"]), to_float(networkstats["incoming packets total"])/uptime(), NUMBER_D->number_symbol(networkstats["outgoing packets total"]), to_float(networkstats["outgoing packets total"])/uptime());

}


void create()
{

}
string query_name()
{
	return "������T�t��(NETWORK_D)";
}
