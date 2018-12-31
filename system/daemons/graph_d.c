/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : graph_d.c
 * Author : Cookys@RevivalWorld
 * Date   : 2001-6-29
 * Note   : �����Ϻ��F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>

mixed color =
({
    ({"",""}), ({RED,BRED}),({GRN,BGRN}),({CYN,BCYN}),({BLU,BBLU}),({YEL,BYEL}),({MAG,BMAG}),({BWHT,WHT})
});

string *pic = ({ "�@","�j","�k","�l","�m","�n","�o","�p","�i" });

varargs string graph(int cur, int max, int fc, int bc, int length)
{
	string result="";
	int now_len,flag=0;
	float last_len,temp;

	if( !length ) length = 40;
	//if( max < cur ) return sprintf("%"+(length*2-18)+"Oops! There's something wrong with it!","");
	if( max < cur )
	{
		flag=1;
		cur=max;
	}
	
	if( cur < 0 ) cur = 0;

	cur*=1000;
	max*=1000;
	
	now_len = floor(cur/(max/to_float(length)));
	last_len = cur%( max/length );
	last_len ? temp = last_len/((max/length)/8.) : 0. ;
	result = sprintf(color[fc][1]+"%-"+now_len*2 +"s"," ");
	if( flag )
		result[<2..]=">>";
	if( now_len < length )
	{
		if( cur==max && temp>0 && temp <1 )
			result+=sprintf(color[fc][1]+"%-2s","");
		else
			result+=sprintf(NOR+color[fc][0]+color[bc][1]+pic[to_int(temp)]+NOR+color[bc][1]+"%-"+((length-now_len)*2-2)+"s","");
	}

	return result+NOR;
}

void create() 
{
	
}

string query_name()
{
	return "ø�Ϩt��(GRAPH_D)";
}
