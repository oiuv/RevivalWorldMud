/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : news_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-03
 * Note   : �s�D���F
 * Update :
 *  o 2003-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>

#define DATA_PATH		"/data/daemon/news.o"
#define CHANGELOG		"/doc/help/news"
#define OVERDUE			7

#define TIME		0
#define MESSAGE		1

private array news;

/*
({
	({ time, msg }),
	...
})
*/
int save_data()
{
	return save_object(DATA_PATH);
}

int restore_data()
{
	return restore_object(DATA_PATH);
}

int add_news(string msg)
{
	if( !msg || !msg[0] ) return 0;
	
	foreach( array newsa in news )
	{
		if( newsa[MESSAGE] == msg )
			return 0;
	}

	news += ({ ({ time(), msg }) });
	
	return save_data();
}

int remove_news(int num)
{
	if( num >= sizeof(news) || num < 0 )
		return 0;

	news = news[0..num-1] + news[num+1..];
	
	return save_data();
}
	
array query_news()
{
	return copy(news);
}

void broadcast_latest_news()
{
	array newe;
	
	if( !sizeof(news) )	
		return;
	
	newe = news[<1];
	CHANNEL_D->channel_broadcast("news", HIG"�������i "+NOR GRN"("+TIME_D->replace_ctime(newe[TIME])[0..4]+") "+newe[MESSAGE]+NOR);
}

void broadcast_news()
{
	int time, num;
	array newe, broadcast_news;
	
	if( !sizeof(news) )	
		return;
	
	time = time();
	broadcast_news = allocate(0);

	// �L���s�D��s�J changelog
	foreach( newe in news )
	{
		if( time - newe[TIME] > OVERDUE*86400 )
		{
			write_file(CHANGELOG, read_file(CHANGELOG)+"("+TIME_D->replace_ctime(newe[TIME])[0..4]+") "+newe[MESSAGE]+"\n", 1);
			news[num] = 0;
		}
		else if( time - newe[TIME] < 3*86400 )
			broadcast_news += ({ newe });
		num++;
	}
	news -= ({ 0 });

	if( sizeof(broadcast_news) )
	{	
		newe = broadcast_news[random(sizeof(broadcast_news))];

		CHANNEL_D->channel_broadcast("news", NOR GRN"("+TIME_D->replace_ctime(newe[TIME])[0..4]+") "+newe[MESSAGE]+NOR);
	}
	//else
		//CHANNEL_D->channel_broadcast("news", NOR GRN"�O�W�s�D "+AUTONEWS_D->query_news()[0]+NOR);
}

int remove()
{
	return save_data();
}

private void create()
{
	if( !restore_data() )
	{
		news = allocate(0);
		save_data();
	}
}

string query_name()
{
	return "�C���s�D�t��(NEWS_D)";
}
