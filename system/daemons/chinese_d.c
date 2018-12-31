/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chinese_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-1
 * Note   : ������F, �w�p�N�Ҧ��禡�g�J MudOS's Chinese Package
 * Update :
 *  o 2001-03-01 Clode �إߧ֨����(��30k���kram), �j�T�W�i�Ĳv
 *  o 2001-03-01 Clode �s�W�r���J�i�F�L����ƪ��Ʀr���
 *
 -----------------------------------------
 */

private string *number = ({ "�s","�@","�G","�T","�|","��","��","�C","�K","�E","�Q" });
private string *number2 = ({ "�s","��","�L","��","�v","��","��","�m","��","�h","��" });

// �̤j�ƭȳ��i�ۥѼW��
//string *unit = ({"�U","��","��","��","��","�V","��","��","��","��","��","��","��e�F","������","���ѥ�","���i��ĳ","�L�q","�j��",});
private string *unit = ({"�U","��","��",});

private mapping cache;

string chinese_number(mixed i)
{
	// �Y��J�ѼƬ� integer (�| overflow) 
	if( intp(i) )
	{
	    if( i < 0 )
		return "�t"+chinese_number(-i);
	    if( i < 11 )
	    	return number[i];
	    if( i < 1000 )
		return cache[i];
	    if( i < 10000 )
	    {
		if( !(i % 1000) ) return number[i/1000]+"�d";
		if( i%1000 < 100 ) return number[i/1000]+"�d�s"+cache[i%1000];
		return number[i/1000]+"�d"+cache[i%1000];
	    }
	    if( i < 100000000 ) 
	    {
		if( !(i % 10000) ) return chinese_number(i/10000)+"�U";
		if( i%10000 < 1000 ) return chinese_number(i/10000)+"�U�s"+cache[i%10000];
		return chinese_number(i/10000)+"�U"+chinese_number(i%10000);
	    }
	    if( i < 2147483648 )
	    {
		if( !(i % 100000000) ) return cache[i/100000000]+"��";
		if( i%100000000 < 10000000 ) return cache[i/100000000]+"���s"+chinese_number(i%100000000);
		return cache[i/100000000]+"��"+chinese_number(i%100000000);
	    }
	    else return "2147483647(overflow)";
	}
	// �Y��J�ѼƬ� string (�L����ƳB�z)
	else if( stringp(i) && i != "")
	{
	   	int j, k, *n=({}), usize = sizeof(unit);
		string *u=({""}), msg;
		
		if( i[0] == '-' ) return "�t"+chinese_number(i[1..]);
		
		// �N�Ʀr�̥|��Ʃ��
		while( (msg = i[<(j+=4)..<(j-3)])!="" )
		{			
			n += ({ to_int(msg) });
			if( j != 4 ) u += ({ unit[k++%usize] });
		}
		
		j = k = sizeof(n);
			
		while(j--)
			if( n[j] ) msg += (j+1<k ? (n[j+1] && n[j]>999 ? chinese_number(n[j]):"�s"+chinese_number(n[j])):chinese_number(n[j])) + u[j];
			else if( u[j] == unit[usize-1] ) msg += unit[usize-1];

		return msg;
	}
	else return "���~�ƭ�";
}

nomask string chinese_period(int t)
{
	int y, n, d, h, m, s;
	string time="";

	s = t %60;
	m = t /60%60;
	h = t /60/60%24;
	d = t /60/60/24%30;
	n = t /60/60/24/30%13;
	y = t /60/60/24/30/13;

	if(y) time = cache[y] + "�~�S";
	if(n) time += cache[n] + "�Ӥ�"		+ (y?"":"�S");
	if(d) time += cache[d] + "��"		+ (n||y?"":"�S");
	if(h) time += cache[h] + "�p��"		+ (d||n||y?"":"�S");
	if(m) time += cache[m] + "��"		+ (h||d||n||y?"":"�S");

	return time+cache[s]+"��";	
}

private mapping chinese =
([
"north"		:"�_��",
"south"		:"�n��",
"east"		:"�F��",
"west"		:"����",
"northwest"	:"��_��",
"northeast"	:"�F�_��",
"southwest"	:"��n��",
"southeast"	:"�F�n��",
"down"		:"�ӤU",
"up"		:"�ӤW",
]);

string to_chinese(string idx)
{
	return chinese[idx];
}

int chinese_to_number(mixed chinese)
{
	int i, len, num, idx;
	
	if( intp(chinese) ) return chinese;
	
	if( !stringp(chinese) )
		return 0;

	len = sizeof(chinese);
	
	for(i=0;i<len;i+=2)
	{
		idx = member_array(chinese[i..i+1], number);
		
		if( idx == -1 )
			idx = member_array(chinese[i..i+1], number2);
		
		if( idx == -1 )
			continue;
		
		if( idx == 10 )
		{
			if( i!=0 )
				continue;
			else if(i+2!=len)
				idx = 1;
		}

		num = num*10 + idx;
	}
	return num;
}

string initialize(int i)
{
	if( i < 11 ) return number[i];
	if( i < 20 ) return "�Q"+number[i%10];
	if( i < 100 ) return number[i/10]+"�Q"+(i%10 ? number[i%10] : "" );
	if( i < 1000 ) return number[i/100]+"��"+(i%100 ? ((i%100<10?"�s":0)||(i%100<20?"�@":""))+initialize(i%100) : "");
}
 
private void create()
{
	cache = allocate_mapping(0);
	
	/* �إ߼ƭȧ֨� */
	for(int i=0;i<=1000;i++)
		cache[i] = initialize(i);
}

string query_name()
{
	return "����t��(CHINESE_D)";
}
