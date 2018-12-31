mixed map;
int room_count;

int check_link(int y,int x)
{
	int count=0;

	if( map[y-1][x-1]=="1" ) count ++;
	if( map[y-2][x]=="1" ) count++;
	if( map[y-1][x+1]=="1" ) count ++;

	return count;
}
// area[RMAP][place][num][z][y][x]
void make_maze(int size)
{

	mixed *tmp;
	string str="";
	map=allocate(size);

	if( size > 200 ) return 0;
	for(int i=0;i<size;i++)
		map[i]=allocate(size);

	for(int i=0;i<size;i++)
	{
		map[0][i]=0+"";
		map[1][i]=1+"";
		map[size-1][i]=0+"";
		map[size-2][i]=1+"";
	}

	for(int i=2;i<size-2;i++)
	{
		map[i][0]=0+"";
		map[i][size-1]=0+"";
		map[i][1]=1+"";
		map[i][size-2]=1+"";

		for(int j=2;j<size-2;j++)
		{
			if( check_link(i,j)>1 ) // > �h�֥i�H��������
				map[i][j]=random(2)+"";
			else
				map[i][j]="1";
			/*
			if( map[i-1][j]== "1" && map[i-1][j-1]!="1" && map[i-1][j+1]!="1" )
				map[i][j]="1";
			else if( map[i-1][j]=="1" )
				map[i][j]=random(2)+"";
			else if( map[i][j-1]=="1" )
				map[i][j]=random(2)+"";
			else map[i][j]=random(2)+"";
			*/
		}
		room_count+=sizeof(filter_array(map[i],(: $1=="1" :)));
	}

	//return map;

	for(int i=0;i<size;i++)
		str+=implode(map[i],"##")+"\n";
	str = replace_string(replace_string(str,"0","�i"),"1","  ");

	tmp=explode(str,"\n");
	map=allocate(sizeof(tmp));

	for(int i=0;i<sizeof(tmp);i++)
		map[i]=explode(tmp[i],"##");
	//shout(sprintf("%O\n",map));
//	return map;
	//printf("%s\n",str);
	//write_file("/u/c/cookys/map/TEST-MAZE",str,1);

}

mixed create_area_data()
{
	return ({});
}


mixed create_extra_info()
{
	mixed data=([]);
	
	for(int i=0;i<10;i++)
	{
		data[i]=([]);
		//for(int j=0;j<10;j++)
		//{	
		//	data[i][j]=([]);
			//for(int k=0;k<10;k++)
		//		data[i][j]["cityroom"]=new("/obj/room/cityroom.c");
		//}
		//data[i]["cityroom"]=new("/obj/room/cityroom.c");
	}		
	return data;
}

int special_move()
{
	return 1;
}

int move(string icon)
{	
	//shout(sprintf("%O\n",icon=="  "));
	return icon == "  ";
}

mixed create_area_map()
{
	return ({({map})});
}

void create()
{
	make_maze(100);
	map=({({map})});
}