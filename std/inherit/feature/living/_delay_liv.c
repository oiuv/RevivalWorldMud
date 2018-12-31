/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _delay_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-02
 * Note   : �ʧ@����
 * Update :
 *  o 2003-00-00 
 -----------------------------------------
 */

#include <delay.h>

/*
({
	second,
	message,
	finish message,
	function point,
})
*/

private nosave mapping delayinfo = allocate_mapping(0);

varargs int is_delaying(string key)
{
	if( !key )
		return sizeof(delayinfo);
	else
		return !undefinedp(delayinfo[key]);
}

varargs int start_delay(string key, int second, string msg, string fmsg, function fp)
{
	if( second < 1 )
		return 0;

	delayinfo[key] = ({ second, msg, fmsg, fp });
}

varargs void remove_delay(string key)
{
	if( !key )
		delayinfo = allocate_mapping(0);
	else
		map_delete(delayinfo, key);
}

array query_delay(string key)
{
	return delayinfo[key];
}

string query_delay_msg(string key)
{
	if( !key )
	{
		if( is_delaying() )
			return values(delayinfo)[0][MESSAGE]+"...���� "+values(delayinfo)[0][SECOND]+" ��\n";
		else
			return 0;
	}
	else
	{
		if( is_delaying(key) )
			return delayinfo[key][MESSAGE]+"...���� "+values(delayinfo)[0][SECOND]+" ��\n";
		else
			return 0;
	}
}

private void delay_decay()
{
	foreach( string key, array info in delayinfo )
	{
		if( --delayinfo[key][SECOND] <= 0 )
		{
			if( functionp(info[FUNCTION]) )
				catch( evaluate(info[FUNCTION]) );
			
			if( stringp(info[FMESSAGE]) )
				tell(this_object(), info[FMESSAGE]);

			map_delete(delayinfo, key);
		}
	}
}
