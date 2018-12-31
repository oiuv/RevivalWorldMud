/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : bit.c
 * Author : Cookys@RevivalWorld
 * Date   : 2003-2-12
 * Note   : included by Simul_Efun Object
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
string trans_bit_to_string(int bit, string *text)
{
	int size=sizeof(text),tmp;
	string result="";

	for(int i=0;i<size;i++)
	{
		int flag=bit%2;
		
		if(flag) 
		{
			tmp++;
			result+=text[i];
			if(tmp>0) result+="�A";
		}
		bit/=2;
		flag=0;
	}
	//result+="�C";
	return result;
}


array trans_bit_to_array(int bit,string *text)
{
	int size=sizeof(text),tmp;
	array result=({});
	
	for(int i=0;i<size;i++)
	{
		int flag=bit%2;
		
		if(flag) 
		{
			tmp++;
			result+=({text[i]});
		}
		bit/=2;
		flag=0;
	}
	return result;
}