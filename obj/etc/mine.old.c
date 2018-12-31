#include <ansi.h>
#include <daemon.h>
#include <inherit.h>
#include <secure.h>
#include <lock.h>

inherit STANDARD_OBJECT;

void do_join(object me, string arg);
void do_start(object me, string arg);
void do_show(object me, string arg);
void do_find(object me, string arg);
void do_finding(object me, int x, int y);
private void reset_mine();
private varargs void flying(object me, int i);

nosave mapping actions = 	
([
	"join"			:	(: do_join :),
	"start"			:	(: do_start :),
	"show"			:	(: do_show :),
	"goto"			:	(: do_find :),
	"reset"			:	(: reset_mine :),
]);

nosave mapping corresponding =
([
	"north"			:	"south",
	"south"			:	"north",
	"east"			:	"west",
	"west"			:	"east",
	"northwest"		:	"southeast",
	"northeast"		:	"southwest",
	"southwest"		:	"northeast",
	"southeast"		:	"northwest",
	"up"			:	"down",
	"down"			:	"up",
]);

nosave string help = @HELP
��a�p��Ԫ�-�ϥλ���

join			�[�J�C��
start			�}�l�C��
show			��ܧG�p��
goto <X�b> <Y�b>	�ȶ}�Ӥ��
reset			�����C��
HELP;

nosave array mine_map;
nosave int total_mine = 0, finded_mine = 0;
nosave int map_size = 0;
nosave int call_depth = 0;
nosave int player_turn = 0;
nosave object *players = allocate(0);

private int is_started()
{
	return pointerp(mine_map);
}
private int valid_coordinate(int x, int y)
{
	if( x < 0 || y < 0 || x >= sizeof(mine_map) || y >= sizeof(mine_map[0]) ) return 0;
	return 1;
}
private int valid_find(int x, int y)
{
	if( mine_map[x][y] > 0 )return 0;
	return 1;
}
private object query_player()
{
	return players[player_turn%sizeof(players)];
}
private int next_player()
{
	int i = ++player_turn;
	if( !sizeof(players) )
	{
		reset_mine();
		return -1;
	}
	msg(CYN"\n�i "HIC"�� " + (i+1) + " �B��$ME�C\n\n"NOR, query_player(), 0, 1);
	tell(query_player(), "\a");
	do_show(query_player(), 0);
	return i;
}
private void kick_player(object me, int x, int y)
{
	player_turn--;
	players -= ({ me });
	mine_map[x][y] = 10;
	me->faint();
	flying(me);
	delete_temp("status", me);
	set_temp("status", HIY + sizeof(players) + " �H�ð���"NOR);
	tell(me, HIY"\n�i "YEL"�C�������A�A��F�C\n\n"NOR);
}
private void reset_mine()
{
	foreach(object ob in (players - ({ 0 })))
		delete_temp("status", ob);
	set_temp("status", HIY"�ð����I��"NOR);
	mine_map = 0;
	player_turn = 0;
	players = allocate(0);
	msg(HIY"\n�i "YEL"�C�������C\n\n"NOR, this_object(), 0, 1);
}
private void leaver_filter()
{
	if( !is_started() ) return;
	players -= ({ 0 });
	if( !sizeof(players) ) reset_mine();
}
private void allocate_mine(int width, int height)
{
	int count;
	float ratio;
	
	total_mine = 0;
	// �إ߯x�}
	mine_map = allocate(width);
	for(int x=0; x<width; x++)
		mine_map[x] = allocate(height);

	ratio = (random(3)+1)/10.0;
	// �H���t�m���u
	map_size = width*height;
	count = width*height*ratio;
	while(count > 0)
	{
		int x = random(width);
		int y = random(height);
		if( mine_map[x][y] == -1 ) continue;
		mine_map[x][y] = -1;
		count--;
		total_mine++;	
	}
	return;
}
private int find_mine(int x, int y)
{
	array finding_queue = allocate(0);
	int mine_count;

	if( !valid_coordinate(x, y) ) return -2;
	if( mine_map[x][y] == -1 ) return 1;
	if( mine_map[x][y] > 0 ) return -1;

	finding_queue += ({ ({ x, y }) });
	while(sizeof(finding_queue))
	{
		array loc = finding_queue[0];
		x = loc[0];
		y = loc[1];
		finding_queue -= ({ loc });

		if( !valid_coordinate(x, y) ) continue;
		if( mine_map[x][y] == -1 ) continue;
		if( mine_map[x][y] > 0 ) continue;

		mine_count = 0;
		for(int dx=-1; dx<=1; dx++)
			for(int dy=-1; dy<=1; dy++)
			{
				if( dx == 0 && dx == dy ) continue;
				if( !valid_coordinate(x + dx, y + dy) ) continue;
				if( mine_map[x + dx][y + dy] == -1 || mine_map[x + dx][y + dy] == 10 )
				{
					//printf("%d %d has bomb\n", x + dx+1, y + dy+1); 
					mine_count++;
				}
			}
	
		mine_map[x][y] = 1 + mine_count;
		finded_mine++;
	
		if( mine_count == 0 ) // Explore
			for(int dx=-1; dx<=1; dx++)
				for(int dy=-1; dy<=1; dy++)
				{
					if( dx == 0 && dx == dy ) continue;
					//find_mine(x + dx, y + dy);
					finding_queue += ({ ({ x + dx, y + dy }) });
				}
	}
	return 0;
}

private varargs void flying(object me, int i)
{
	mixed exit;
	mapping exits;
	object env;
	string arg, my_idname, from;

	env = environment(me);
	my_idname = me->query_idname();

	if( !mapp(exits = query("exits", env)) || !sizeof(exits) ) return;

	if( i > 10 )
		return broadcast(env, my_idname+"����F�a�W�A�åB½�u�F�n�X��A���G�˪����M�C\n", 0, me);

	arg = keys(exits)[random(sizeof(keys(exits)))];
	exit = exits[arg];
	from = CHINESE_D->to_chinese(corresponding[arg]);
	if( functionp(exit) ) exit = evaluate(exit, me);

	// �Y�O�y�ХX�f
	if( arrayp(exit) )
	{
		if( (MAP_D->query_map_system(exit))->query_coor_data(exit, "lock") & LOCKED && !wizardp(me) )
			return broadcast(exit, my_idname+"���F�L�ӡA���W�F�� "+capitalize(arg)+" ��V�����C\n", 0, me);

		if( !wizardp(me) && (MAP_D->query_map_system(exit))->query_coor_data(exit, "lock") & WIZLOCKED )
			return broadcast(exit, my_idname+"���F�L�ӡA���W�F�� "+capitalize(arg)+" ��V������C\n", 0, me);
	}
	// �Y�O����W�٥X�f
	else if( stringp(exit) )
	{
		if( query("lock/"+arg, env) & LOCKED && !wizardp(me))					   
			return broadcast(exit, my_idname+"���F�L�ӡA���W�F�� "+capitalize(arg)+" ��V�����C\n", 0, me);

		if( !wizardp(me) && query("lock/"+arg, env) & WIZLOCKED )
			return broadcast(exit, my_idname+"���F�L�ӡA���W�F�� "+capitalize(arg)+" ��V������C\n", 0, me);
	}
	else return;

	broadcast(env, my_idname+"��"+(CHINESE_D->to_chinese(arg)||" "+capitalize(arg)+NOR" ��V")+"���h�C\n", 0, me);

	me->move(exit);

	if( from )
		broadcast(exit, my_idname+"�q"+from+"���F�L�ӡC\n", 0, me);
	else
		broadcast(exit, my_idname+"���F�L�ӡC\n", 0, me);

	flying(me, i++);
}

void do_join(object me, string arg) 
{
	if( sizeof(players) >= 10 )
		return tell(me, "�����x���y��w���A�еy��A�ӡC\n");

	if( pointerp(mine_map) )
		return tell(me, "�C���w�}�l�C\n");

	if( member_array(me, players) != -1 )
		return tell(me, "�z�w�g�[�J���C���F�C\n");

	msg("$ME�q�f�U���ǥX 2 �T�ҳѵL�X���N����i�F$YOU�C\n", me, this_object(), 1);

	players += ({ me });
	set_temp("status", HIY"�ð����I�� (" + sizeof(players) + " �H)"NOR);
}
void do_start(object me, string arg)
{
	int i, max_x, max_y;
	
	if( member_array(me, players) == -1 )
		return tell(me, "�z�å��ѥ[�C���A�Шϥ� join ���O�[�J�C���C\n");
	if( is_started() )
		return tell(me, "�C���w�g�}�l�A�Шϥ� goto ���O�ӴM��a�p�C\n");

	set_temp("status", HIY + sizeof(players) + " �H�ð���"NOR);
	msg(YEL"\n�i "HIY"��a�p�C���}�l�C\n\n"NOR, me, 0, 1);

	max_x = 12 + sizeof(players);
	max_y = 12 + sizeof(players);

	msg(CYN"�i "HIC"�H�����ͦa�� "+max_x+"x"+max_y+"...\n"NOR, me, 0, 1);
	allocate_mine(max_x, max_y);
	
	msg(CYN"�i "HIC"�ѻ�l�Y�X�C������ ...\n"NOR, me, 0, 1);

	leaver_filter();
	i = sizeof(players);
	msg(CYN"�i "NOR, me, 0, 1);
	while(i)
	{
		int j = random(i);
		object ob = players[j];

		set_temp("status", HIY"��"NOR YEL"�a�p"NOR, ob);
		msg(HIC + (sizeof(players)-i+1) + ". " + ob->query_idname() + HIC", "NOR, me, 0, 1);
		players -= ({ ob });
		players += ({ ob });
		i--;
	}
	
	msg(CYN"\n�i "HIC"�п�J help mine �d�߫��O�Ϊk�C\n"NOR, me, 0, 1);
	msg(HIY"\n�i "YEL"�C���ؼ�: �׶}�Ҧ��i�঳�a�p���a�I�C\n"NOR, me, 0, 1);
	msg(CYN"\n�i "HIC"�� " + (player_turn+1) + " �B��$ME�C\n\n"NOR, query_player(), 0, 1);
	do_show(query_player(), 0);
	tell(query_player(), "\a");
}
void do_show(object me, string arg)
{
	string msg = "�G�p�� (�@ " + total_mine + " �T���u, �Ѿl " + (map_size-finded_mine) + " �Ӧa�I):\n";
	int flag = 0;

	if( !is_started() )
		return tell(me, "�C���|���}�l�A�Шϥ� start ���O�}�l�C���C\n");

	if( wizardp(me) && arg == "mines" ) flag = 1;

	msg += "�ثe��: " + query_player()->query_idname() + "\n";
	msg += HIG"Y\\X";
	for(int i=0; i<sizeof(mine_map); i++)
		msg += sprintf("%s%2d"NOR, (i%2==0 ? NOR:HIG), i+1);
	msg += "\n"NOR;
	for(int y=0; y<sizeof(mine_map[0]); y++)
	{
		msg += sprintf(HIG"%2d "NOR, y+1);
		for(int x=0; x<sizeof(mine_map); x++)
		{
			string symbol;
			switch(mine_map[x][y]-1)
			{
			
			case -2:	if( flag ) { symbol = HIR"��"NOR; break; }
			case -1:	if( (x+y)%2 == 0 )
						symbol = BCYN + HIC"  "NOR;
					else	symbol = BBLU + HIC"  "NOR;
					break;
			case  0:	symbol = NOR"  "NOR;		break;
			case  1:	symbol = HIB"��"NOR;		break;
			case  2:	symbol = HIG"��"NOR;		break;
			case  3:	symbol = HIR"��"NOR;		break;
			case  4:	symbol = HIY"��"NOR;		break;
			case  5:	symbol = HIC"��"NOR;		break;
			case  6:	symbol = HIW"��"NOR;		break;
			case  7:	symbol = HIW"��"NOR;		break;
			case  8:	symbol = HIW"��"NOR;		break;
			case  9:	symbol = HIR"��"NOR;		break;
			default:	symbol = sprintf(HIW"%2d "NOR, mine_map[x][y]-1);
			}
			msg += sprintf("%2s", symbol);
		}
		msg += sprintf(HIG" %2d "NOR, y+1);
		msg += "\n";
	}
	msg += HIG"   ";
	for(int i=0; i<sizeof(mine_map); i++)
		msg += sprintf("%s%-2d"NOR, (i%2==0 ? NOR:HIG), i+1);
	msg += "\n"NOR;
	tell(me, msg);	
}
void do_find(object me, string arg)
{
	int x, y;

	if( !is_started() )
		return tell(me, "�C���|���}�l�A�Шϥ� start ���O�}�l�C���C\n");

	if( me != query_player() )
		return tell(me, "�{�b�٨S������A�C\n");

	if( !arg || sscanf(arg, "%d %d", x, y) != 2 )
		return tell(me, "���O�Ц�: find <��> <�C>\n");

	if( !valid_coordinate(x-1, y-1) )
		return tell(me, "�y�Фw�W�X�a�Ͻd��C\n");
	if( !valid_find(x-1, y-1) )
		return tell(me, "�o�Ӯy�Фw�g���L�F�C\n");

	msg("$ME�p���l�l���N�}�B��V " + sprintf("%d, %d", x, y) + " ���y�ФW ...\n", me, 0, 1);
	call_out((: do_finding :), 1, me, x, y);
	
}
void do_finding(object me, int x, int y)
{
	int res;

	leaver_filter();	// �ˬd���a�O�_�s�b
	call_depth = 0;		// ���^�`�׭p��

	res = find_mine(x-1, y-1);
	if( res < 0 ) return;
	if( res )
	{
		switch(random(2))
		{
		case 0: msg("$ME���}�U���G�S�����򲧪��C\n", me, 0, 1); break;
		case 1: msg(HIR"\n$ME���a�p�F�I�I\n\n"NOR, me, 0, 1); break;
		}
		msg("���M���T�u�I�v���@�n�A�u��$ME�Q�����@�ζ¬��ĤW���ݡC\n", me, 0, 1);

		kick_player(me, x-1, y-1);

		CHANNEL_D->channel_broadcast("news",
			sprintf("%s�@�}���a�p��U�A�u�I�v�@�n�����T�A�u���@�ζ¬��ĤW���ݡC\n", me->query_idname()));
		if( sizeof(players) == 1 )
		{
			msg(HIY"\n����$ME��a�p���!!\n\n"NOR, query_player(), 0, 1);
			reset_mine();
		}
		else next_player();
	}
	else
	{
		switch(random(2))
		{
		case 0: msg("$ME���}�U���G�S�����򲧪��C\n", me, 0, 1); break;
		case 1: msg("$ME���}�U���G�n�o�ͤ���ơA���S���򳣨S�o�͡C\n", me, 0, 1); break;
		}
		if( sizeof(players) == 1 && map_size-total_mine == finded_mine )
		{
			msg(HIY"\n����$ME��a�p���!!\n\n"NOR, me, 0, 1);
			reset_mine();
		}
		else next_player();
	}
	return;
}

void heart_beat()
{
	leaver_filter();
}
void create()
{
	set_idname("mine game",HIC"��a�p"NOR CYN"��Ԫ�"NOR);
	set("flag/no_amount", 1);
	set("long", help);
	set("unit", "�x");
	set("mass", -1);
	reset_mine();
	set_heart_beat(10);
}