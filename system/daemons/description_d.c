/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : description_d.c
 * Author : Clode@RevivalWorld
 * Date	  : 2006-02-08
 * Note	  : �ԭz���F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <buff.h>

string *buff_list = ({
	BUFF_STR		,	HIY"�O"NOR YEL"�q"NOR,
	BUFF_PHY		,	HIY"��"NOR YEL"��"NOR,
	BUFF_INT		,	HIY"��"NOR YEL"��"NOR,
	BUFF_AGI		,	HIY"��"NOR YEL"��"NOR,
	BUFF_CHA		,	HIY"�y"NOR YEL"�O"NOR,
	BUFF_STAMINA_MAX	,	HIG"��O"NOR GRN"�̤j��"NOR,
	BUFF_HEALTH_MAX		,	HIG"�ͩR"NOR GRN"�̤j��"NOR,
	BUFF_ENERGY_MAX		,	HIG"�믫"NOR GRN"�̤j��"NOR,
	BUFF_STAMINA_REGEN	,	HIC"��O"NOR CYN"��_�t��/2s"NOR,
	BUFF_HEALTH_REGEN	,	HIC"�ͩR"NOR CYN"��_�t��/2s"NOR,
	BUFF_ENERGY_REGEN	,	HIC"�믫"NOR CYN"��_�t��/2s"NOR,
	BUFF_LOADING_MAX	,	HIM"�t��"NOR MAG"�W��/kg"NOR,
	BUFF_FOOD_MAX		,	HIM"����"NOR MAG"�W��"NOR,
	BUFF_DRINK_MAX		,	HIM"����"NOR MAG"�W��"NOR,
	BUFF_SOCIAL_EXP_BONUS	,	HIB"���|�g��"NOR BLU"�[��/%"NOR,
	BUFF_MOVING_STAMINA	,	HIR"������O"NOR RED"�ӶO"NOR,
	BUFF_ARMOR_SLASH	,	HIW"����"NOR WHT"���m"NOR,
	BUFF_ARMOR_GORE		,	HIW"���"NOR WHT"���m"NOR,
	BUFF_ARMOR_EXPLODE	,	HIW"�z��"NOR WHT"���m"NOR,
	BUFF_ARMOR_INJURY	,	HIW"����"NOR WHT"���m"NOR,
	BUFF_ARMOR_FIRE		,	HIW"�P��"NOR WHT"���"NOR,
	BUFF_ARMOR_ICE		,	HIW"�H�B"NOR WHT"���"NOR,
	BUFF_ARMOR_POISON	,	HIW"�@�r"NOR WHT"���"NOR,
	BUFF_ARMOR_HEART	,	HIW"���F"NOR WHT"���"NOR,
});

int buff_list_size;

string buff_description(object ob, string key, string chinese, int indent)
{
	int buff = query(key, ob);

	if( !buff ) return 0;
		
	if( buff > 0 )
		return sprintf(repeat_string(" ", indent)+NOR YEL"�D%-15s "NOR CYN"+"HIC"%d\n"NOR, chinese, buff);
	else
		return sprintf(repeat_string(" ", indent)+NOR YEL"�D%-15s "NOR RED"-"HIR"%d\n"NOR, chinese, -buff);
}

string query_object_buff_description(object ob, int indent)
{
	string description = "";
	
	for(int i=0;i<buff_list_size;i+=2)
		description += buff_description(ob, buff_list[i], buff_list[i+1], indent) || "";
	
	return description;
}

// ���~�ԭz
varargs string query_object_description(object ob, int need_quantity)
{
	string mylong = query("long", ob);
	string description = ob->short(need_quantity)+" �w\n";

	description += "������������������������������������������������������������������������������\n\n";	
	
	if( mylong )
		description += "  "+mylong+"\n";
		
	description += query_object_buff_description(ob, 2);

	description += "\n������������������������������������������������������������������������������\n";

	return description;
}

// �ͪ��ԭz
varargs string query_living_description(object ob, int need_quantity)
{
	string long = query("long", ob);
	string description = ob->short(need_quantity)+" �w\n";

	description += "������������������������������������������������������������������������������\n\n";
	
	description += "  "+pnoun(3, ob)+"���~�֬� "+query("age", ob)+" ���C\n\n"+(long?long+"\n":"");

	if( ob->is_module_npc() )
	{
		string boss = query("boss", ob);
		object bossob;
		
		if( stringp(boss) )
			bossob = find_player(boss);
		
		if( objectp(bossob) )
			description += "  �䨭�����������u�A���D�O "+bossob->query_idname()+"\n";
		else if( !boss )
			description += "  �䨭�����������u�A���L���D���\n";
		else
			description += "  �䨭�����������u�A���D�O "+capitalize(boss)+"\n";
	}

	description += "\n";
	
	foreach(object eq in ob->query_equipment_objects())
	{
		description += "  "+HIW"["NOR+eq->query_part_name()+HIW"] "NOR"�˳� " + eq->short()+"\n";
		description += query_object_buff_description(eq, 9)+"\n";
	}

	
	description += "\n������������������������������������������������������������������������������\n";
	
	return description;
}

void create()
{
	buff_list_size = sizeof(buff_list);
}

string query_name()
{
	return "�ԭz�t��(DESCRIPTION_D)";
}
