/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : research.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-07
 * Note   : ���y��o����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <npc.h>
#include <skill.h>

inherit ROOM_ACTION_MOD;
#define PRODUCT_MODULE_PATH		"/std/module/product/"

#define RANDOM_THING			({ "�ަ�|", "������", "�Ϧ��l", "�����~", "�J����", "�H����", "�䨧�G", "�׶�", "�j�z�]�p�z", "�s�˰^�Y", "�J�_", "�̯��e", "������", "�H�J�ѽu", "�H�J��" })

// ���o�Ҧ��i�H�Ͳ������~ module ����}�C
private object *query_available_products()
{
	string product;
	object *modules, module;
	
	modules = allocate(0);
	
	foreach(product in get_dir(PRODUCT_MODULE_PATH))
	{
		catch( module = load_object(PRODUCT_MODULE_PATH + product +"/module") );
		
		if( !objectp(module) ) continue;
		
		modules += ({ module });
	}
	
	return modules;
}

void do_list_action(object me, string arg)
{
	int i;
	string file, msg;
	object env = environment(me);
	object master = env->query_master();
	object product;
	mapping product_info;
	string *productslist = query("productslist", master);
	
	if( !sizeof(productslist) )
		return tell(me, "�o�Ӭ�o���ߩ|���]�p�X����i�H�Ͳ������~�C\n");
	
	msg = env->query_room_name()+"�ثe�w�g��o���������~�̷ӳ̷s�o���Ǧp�U��G\n";
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += HIW"�s�� ���~�W��                           ���~����          �^�O�� ����          \n"NOR;
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	i = 0;
	foreach(file in productslist)
	{
		catch(product = load_object(file));
	
		if( !objectp(product) ) continue;
			
		product_info = product->query_product_info();
		
		msg += sprintf("%-4s %-34s %-18s%-7d%s\n", (++i)+".", product->query_idname(), product_info["name"]+"("+product_info["id"]+")", query("design/inlay", product), product_info["value"]);
	}
	
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	me->more(msg);
}

void do_design_action(object me, string arg)
{
	string value;
	mapping product_info;
	object *products, product;
	object env = environment(me);
	object master = env->query_master();

	if( !arg )
	{
		string msg;
		mapping design = query("design", master);
		
		if( !mapp(design) )
			return tell(me, "�ثe�S�����󲣫~��o��ơA�O�_�n��o���~�H(type, id, name, inlay, finish)\n");
			
		msg = "�ثe�̷s��o�������~���G\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += NOR"���~����(type) �G"NOR+(design["type"]||"���]�p")+"\n";
		msg += NOR"���~�^��(id)   �G"NOR+(design["id"]||"���]�p")+"\n";
		msg += NOR"���~����(name) �G"NOR+(design["name"]||"���]�p")+"\n";
		msg += NOR"�^�O���~(inlay)�G\n"NOR;
			
		if( mapp(design["inlay"]) )
		foreach(string file, int num in copy(design["inlay"]))
		{
			file = replace_string(file, "#", "/");
			
			if( !file_exists(file) ) continue;

			product = load_object(file);
			
			if( !objectp(product) ) continue;
			
			msg += "                 "+QUANTITY_D->obj(product, num, 1)+"\n";
		}
		else
			msg += "                  ���^�O���󪫫~\n";
		
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

		
		tell(me, msg);
		return;
	}

	if( query("owner", env) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"���O�o�����y��o���ߪ��֦��̡C\n");

	sscanf(arg, "%s %s", arg, value);
	
	switch(arg)
	{

		case "type":
		{
			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n��o���@�ز��~(plist)�H\n");

			products = query_available_products();
		
			if( !sizeof(products) )
				return tell(me, "�o�ؤu�t�ثe�L�k��o���󲣫~�C\n");
			
			foreach( product in products )
			{
				product_info = product->query_product_info();
						
				if( product_info["id"] == value )
				{
					if( query("technology", master) < product_info["technology"] )
						return tell(me, "��o���ߥثe�ֿn����ޭȤ����H�}�o���ز��~�C\n");

					if( member_array(value, query("learned_products", me)||allocate(0)) == -1 )
						return tell(me, pnoun(2, me)+"�����ճo�ӪF��ӫ��s�y�A�o����첣�~�]�p�Ϭ�s�@�U�C\n");

					foreach(string skill, int level in product_info["skill"])
						if( me->query_skill_level(skill) < level )
							return tell(me, pnoun(2, me)+"��"+(SKILL(skill))->query_name()+"���Ť��� "+level+" �L�k��o�������~�C\n");

					msg("$ME�ӶO�F "+product_info["technology"]+" �I����ޭȶi��u"+product_info["name"]+"("+product_info["id"]+")�v�������~���}�o�C\n", me, 0, 1);
					
					addn("technology", -product_info["technology"], master);
					
					set("design/type", value, master);

					master->save();
					return;
				}
			}
			
			return tell(me, "�L�k��o "+value+" �o�ز��~�A"+pnoun(2, me)+"�Q�n��o���@�ز��~(plist)�H\n");
			break;
		}
		case "name":
		{
			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n�����~������˪�����W�١H\n");

			value = kill_repeat_ansi(value+NOR);
			
			if( !is_chinese(value) )
				return tell(me, "�ХΡu����v�����~������W�١C\n");
			
			if( noansi_strlen(value) > 16 || noansi_strlen(value) < 2 )
				return tell(me, "����W�r�����b 1 �� 8 �Ӥ���r�����C\n");
			

			msg("$ME�N���~������W�ٳ]�p���u"+value+"�v�C\n", me, 0, 1);
			
			set("design/name", value, master);
			
			master->save();
			break;
		}
		case "id":
		{
			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n�����~������˪��^��W�١H\n");

			value = kill_repeat_ansi(lower_case(value)+NOR);

			if( !is_english(value) )
				return tell(me, "�ХΡu�^��v�����~���^��W�١C\n");
			
			if( noansi_strlen(value) > 16 || noansi_strlen(value) < 3 )
				return tell(me, "�^��W�٥����b 3 �� 16 �ӭ^��r�Ƥ����C\n");

			value = replace_string(value, " ", "-");

			msg("$ME�N���~���^��W�ٳ]�p���u"+value+"�v�C\n", me, 0, 1);
			
			set("design/id", value, master);
			
			master->save();
			break;
		}
		case "delete":
		{
			string *productslist = query("productslist", master)||allocate(0);
			int num;
			
			if( !value || !value[0] || !to_int(value) )
				return tell(me, "�п�J���T���s���C\n");
			
			num = to_int(value) - 1;
				
			if( num < 0 || num > sizeof(productslist)-1 )
				return tell(me, "�S���o�ӽs�������~�C\n");
			
			product = load_object(productslist[num]);
			
			productslist = productslist[0..num-1] + productslist[num+1..];
			
			set("productslist", productslist, master);
			
			msg("$ME�N�s���� "+value+" �����~�u$YOU�v�q��o���~�C���R���C\n", me, product, 1);
			break;
		}
		case "inlay":
		{
			object ob;
			
			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n�N���򪫫~�^�O�b���~���H\n");
			
			if( !objectp(ob = present(value, me)) )
				return tell(me, pnoun(2, me)+"�����W�S�� "+value+" �o�ت��~�C\n");
			
			if(me->is_equipping_object(ob) )
				return tell(me, pnoun(2, me)+"����������"+ob->query_idname()+"���˳ƪ��A�C\n");

			addn("design/inlay/"+replace_string(base_name(ob), "/", "#"), 1, master);

			msg("$ME�N$YOU�^�O�첣�~�]�p�ҫ��W�i����աA���~���������o�X"HIW"�_�S"NOR"��"WHT"���~"NOR"�C\n", me, ob, 1);	
			
			destruct(ob, 1);
			master->save();
			me->save();
			break;
		}
		case "finish":
		{
			string *productslist = query("productslist", master)||allocate(0);
			string type = query("design/type", master);
			string id = query("design/id", master);
			string name = query("design/name", master);
			mapping inlay = query("design/inlay", master);
			int inlay_num;
			
			
			if( !type )
				return tell(me, "���]�w�Ͳ����~�����C\n");

			if( !id )
				return tell(me, "���]�w���~�^��W�١C\n");

			if( !name )
				return tell(me, "���]�w���~����W�١C\n");
		
			product = load_object("/product/"+type+"/"+SERIAL_NUMBER_D->query_serial_number("product/"+type));
			
			product_info = product->query_product_info();
			
			product->set_idname(id, name);

			set("mass", product_info["mass"], product);
			set("value", product_info["value"], product);
			set("class", product_info["class"], product);
			set("design/owner", me->query_id(1), product);
			set("design/room", env->query_room_name(), product);
			set("design/time", time(), product);
			set("design/btype", env->query_building_type(), product);
			
			// �@�^�O�����]�w
			if( mapp(inlay) )
			{
				inlay_num = implode(values(inlay), (: $1+$2 :));
				product->query_module_file()->setup_inlay(product, inlay);
				set("design/inlay", inlay_num, product);
			}

			product->save();			
			productslist = ({ base_name(product) }) | productslist;
			
			delete("design", master);

			set("productslist", productslist, master);
			master->save();

			CHANNEL_D->channel_broadcast("city", me->query_idname()+"��"+query("short", env)+"�����̷s�u"+product_info["name"]+"("+product_info["id"]+")�v���~�]�p�éR�W���u"+product->query_idname()+"�v�A�^�O���~�� "HIW+inlay_num+NOR"�A���Ȭ� "HIY+NUMBER_D->number_symbol(product_info["value"])+NOR"�C", me);

			break;
		}
		default:
			return tell(me, "�L�k�}�o "+arg+" �o�ز��~�C\n");
			break;
	}
}

void heart_beat(object room)
{
	int technologylevel;
	int researchmanage;
	int technology;
	int userinside;
	int npcinside;

	object npc, ob;
	object *npcs = allocate(0);
	string *slaves = room->query_slave();

	if( !query_temp("research_heart_beat", room) )
		set_temp("research_heart_beat", random(1024), room);
		
	if( !(addn_temp("research_heart_beat", 1, room)%240) )
	{
		room->save();
		npcs->save();
	}

	foreach(ob in all_inventory(room))
	{
		if( ob->is_module_npc() && query("job/cur", ob) == ENGINEER )
		{
			npcs += ({ ob });
			++npcinside;
		}
		
		if( userp(ob) )
			userinside = 1;
	}
	
	if( npcinside )
	{
		foreach(npc in npcs)
		{
			technologylevel += npc->query_skill_level("technology");
			researchmanage += npc->query_skill_level("researchmanage");
		}
		
		technologylevel /= npcinside;
		researchmanage /= 5;

		npc = npcs[random(sizeof(npcs))];
	}

	set("technologylevel", technologylevel, room);
	set("researchmanage", researchmanage, room);

	if( researchmanage < sizeof(slaves) + 1 )
	{
		if( userinside )
			broadcast(room, "��o�޲z����O�L�k�伵�L�q����o���߳s��C\n");
		return;	
	}

	if( npcinside)
	{
		if( random(technologylevel) < 10 )
		{
			technology = to_int((-random(5)-1) * pow(sizeof(slaves)+1, 0.3));
			
			if( userinside )
			{
				if( !random(2) )
					msg("$ME���F�Ӥj����A�u����I�I�v�f���w��q���O�W��]�Ƶ��N�a�F�I(��ޭ� "HIR+technology+NOR")\n", npc, 0, 1);
				else
					msg("$ME��o�_�Q��u"HIY+RANDOM_THING[random(sizeof(RANDOM_THING))]+NOR"�v�q�q���ƾǭ�Ƹ̭ˡA�u���@�@"WHT"�·�"NOR"�G����ӹ���ǡI(��ޭ� "HIR+technology+NOR")\n", npc, 0, 1);
			}
		}
		else
		{
			technology = random(to_int(pow(technologylevel / 6 * (sizeof(slaves)+1), 0.5)));
			
			if( userinside )
			{
				if( !random(2) )
					msg("$ME�̴`�ۼзǪ�����{�ǡA�N�s�� "+sprintf(HIG"%c%c"NOR GRN"%d%d%d%d%d"NOR, range_random(65,90),range_random(65,90),random(10),random(10),random(10),random(10),random(10))+" ���궵���ئp�������C(��ޭ� "HIG"+"+technology+NOR")\n", npc, 0, 1);
				else
					msg("$ME�q���Ѥ�����g��A�N�쥻���~����s��ƭ��s��z��g�C(��ޭ� "HIG"+"+technology+NOR")\n", npc, 0, 1);
			}
		}
	}
	
	addn("technology", technology, room);
	addn("total_technology", technology, room);
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"laboratory"	:
	([
		"short"	: HIC"��o"NOR CYN"�����"NOR,
		"heartbeat":60,	// ��ڮɶ� 1 �����
		"job"	: ENGINEER,
		"master": 1,
		"help"	:
			([
"topics":
@HELP
    ���Ѳ��~���s�y�P��ެ�o�C
HELP,


"design":
@HELP
�]�p���s���~�����O�A�Ϊk�p�U�G[�޲z���O]
  design type gold		- �Ͳ���(gold)�������~�A�Ҧ����~�����i�� plist �d��
  design id gold wire		- �N���~���^��W�ٳ]�w�� gold wire (�i�ϥΦ�X)
  design name ���u		- �N���~������W�ٳ]�w�� ���u (�i�ϥΦ�X)
  design inlay '���~'		- �N�Y�ǯS���~�յ��^�O�첣�~�ҫ��W
  design delete 3		- �N��o���~�C���s���� 3 �����~��T�R��
  design finish			- �������~�]�p�A�o��s���~
HELP,

"list":
@HELP
�C�X�w�g��o���\�����~�C��A�Ϊk�p�U�G
  list		 		- �C�X�w�g��o���\�����~�C��
HELP,
			]),
		"action":
			([
				"design"		: (: do_design_action($1, $2) :),
				"list"			: (: do_list_action($1, $2) :),
			]),
	]),
]);

string look_room(object env)
{
	string msg = "";
	object master = env->query_master();
		
	msg += "��ޭ� "HIC+query("technology", master)+NOR CYN"/"+query("total_technology", master)+NOR" ";
	msg += "������o�O "HIY+query("technologylevel", master)+NOR+" ";
	msg += "��o�޲z "HIY+(sizeof(master->query_slave())+1)+"/"+query("researchmanage", master)+NOR+"\n";
	
	return msg;
}

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIC"���y"NOR CYN"��o����"NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,INDIVIDUAL

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,COMMERCE_REGION

	// �}�i�����O��
	,"9000000"

	// �ؿv���������ռаO
	,1

	// �c�a�^�m��
	,5
	
	// �̰��i�[�\�Ӽh
	,2
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,1
	
	// �ؿv���ɥN
	,2
});

