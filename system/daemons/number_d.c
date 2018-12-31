/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : number_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-8-14
 * Note   : �Ʀr���F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <type.h>

varargs string number_symbol(mixed arg, int mark)
{
	string number, result="";
	int i, j, k, l=0;

        switch(typeof(arg)) 
        {
        	case INT:
                	number = arg+"";
                	break;
        	case STRING:
                	number = arg;
                	break;
        	default:
               		error(sprintf("cash_number: Expected: string, Got: %O.\n", arg));
        }

	i = strlen(number);
	
	if( number[0] == '-' )
	{
		l=1; 
		i--;
	}

	if( i <= 3 ) return mark ? sprintf("%*'*'s", mark, number) : number;

	k = i % 3 + l;
	j = i / 3;
	
	if( k ) result += number[0..k-1]+( l && k==1 ? "" : ",");
	
	for(l=0;l<j;l++)
		result += ( l ? ",":"") + number[(k+l*3)..(k+(l+1)*3-1)];
	
	if( mark ) result = sprintf("%*'*'s", mark, result);
	
	return result;
}
string query_name()
{
	return "�Ʀr�t��(NUMBER_D)";
}
