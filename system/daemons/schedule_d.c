#include <daemon.h>

#define EVENT_LOOP		0
#define EVENT_CUR_TIME		1
#define EVENT_MAX_TIME		2
#define EVENT_OBJECT		3
#define EVENT_FUNCTION		4
#define EVENT_ARGUMENT		5
#define monitor(x)              CHANNEL_D->channel_broadcast("nch", "SCHEDULE_D: " + x)

nosave int last_update_time;
nosave int handle_id;
nosave mapping events = allocate_mapping(0);
/*
nosave mixed events =
({
//	({ Do loop ,Default Value, Timetick, Filename, Function, Arg ... }),
//	({ 0, 0, 3, __FILE__, "update", "http://news.pchome.com.tw/include/online_news.html" }),
//	({ 0, 0, 3, this_object(), "update", "http://news.pchome.com.tw/include/online_news.html" }),
});
*/
int evaluate_event(int id)
{
	mixed event;
	
	if( undefinedp(event = events[id]) ) return 0;
	if( !event[EVENT_OBJECT] ) return 0;

	if( functionp(event[EVENT_OBJECT]) )
	{
		monitor(sprintf("������ %O", event[EVENT_OBJECT]));
		evaluate(event[EVENT_OBJECT]);
	}
	else
	{
		mixed args = ({ event[EVENT_FUNCTION] }) + event[EVENT_ARGUMENT..sizeof(event)-1];
		monitor(sprintf("���� %O->%s", event[EVENT_OBJECT], event[EVENT_FUNCTION]));
		//monitor(sprintf("call_other(%O, %O)", event[EVENT_OBJECT], args));
		call_other(event[EVENT_OBJECT], args);
	}
	return 1;
}
mixed query_events()
{
	return events;
}

varargs int set_event(int time, int loop, mixed ob, mixed func, mixed args...)
{
	mixed event;

	if( !ob ) error("�ѼƹL��");
	if( functionp(ob) )
		event = ({ loop, 0, time, ob });
	else
		event = ({ loop, 0, time, ob, func }) + args;

	events[handle_id] = event;

	if( event[EVENT_LOOP] ) evaluate_event(handle_id);
	return handle_id++;
}
void delete_event(int handle)
{
	if( !undefinedp(handle) )
		map_delete(events, handle);
}
mixed query_event(int handle)
{
	return events[handle];
}
void heart_beat()
{
	int i;

	if( !sizeof(events) ) return;
	if( !last_update_time ) last_update_time = time();

	i = time() - last_update_time;	// �O��C���߸����ɶ��t
	last_update_time = time();	// �O���̫�@���߸��ɶ�
	foreach(int id, mixed event in events)
	{
		if( !event[EVENT_OBJECT] )
		{	// �R���H�򥢪���Ψ禡���Ъ��ƥ�
			map_delete(events, id);
			continue;
		}
		if( event[EVENT_CUR_TIME] < event[EVENT_MAX_TIME] ) 
		{	// ���W�L�ƥ�`���ɶ�
			event[EVENT_CUR_TIME] += i;
		}
		else 
		{	// �w�W�L�ƥ�`���ɶ�
			evaluate_event(id);
			event[EVENT_CUR_TIME] = 0;
			if( !event[EVENT_LOOP] ) map_delete(events, id); // �����R��
		}
	}
}
void create()
{
	set_heart_beat(10);

	foreach(int id, mixed event in events)
	{
		if( !event[EVENT_LOOP] ) continue;
		evaluate_event(id);
	}
}
string query_name()
{
	return "�Ƶ{�t��(SCHEDULE_D)";
}
