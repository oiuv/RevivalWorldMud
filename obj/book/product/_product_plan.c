/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _product_plan.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-15
 * Note   : ���~�]�p��
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <skill.h>

inherit STANDARD_OBJECT;

#define COST_SOCIAL_EXP	100000

mapping actions;
	
void do_readplan(object me, string arg)
{
	int not_enough_skill;
	string product = query("product");
	string module = "/std/module/product/"+product+"/module.c";
	mapping product_info;
	
	if( !module || !load_object(module) ) return;
		
	product_info = module->query_product_info();

	foreach(string sk, int level in product_info["skill"])
	{
		if( me->query_skill_level(sk) < level )
		{
			not_enough_skill = 1;
			tell(me, pnoun(2, me)+"���u"+(SKILL(sk))->query_idname()+"�v�ޯ൥�Ť��� "+level+" �šA�ݤ���"+this_object()->query_idname()+"�C\n");
		}
	}
	
	if( not_enough_skill )
		return;

	if( !me->cost_social_exp(COST_SOCIAL_EXP) )
		return tell(me, pnoun(2, me)+"�����|�g��Ȥ��� "+COST_SOCIAL_EXP+"�A�L�k�i��ǲߡC\n");

 	msg("$ME�}�l�{�uŪ�_$YOU�A�u��$ME�ŵM�}�Ԧ������_���I�Y�A���G�q���Y�o��F�ܦh���F��C\n", me, this_object(), 1);

	msg("$ME�Ƿ|�F�u"+product_info["name"]+"("+capitalize(product_info["id"])+")�v���s�@��k�C\n", me, 0, 1);
	
	set("learned_products", (copy(query("learned_products", me))||allocate(0)) | ({ product }), me);
	me->save();

	msg("$YOU�b$ME�\Ū����K�Ƭ��H���C\n", me, this_object(), 1);

	destruct(this_object(), 1);
}

void create()
{
	string product = query("product");
	string module = "/std/module/product/"+product+"/module.c";
	mapping product_info;
	
	if( !module || !load_object(module) ) return;
		
	product_info = module->query_product_info();

	set_idname(HIW+product_info["id"]+NOR CYN" plan"+NOR, HIW+product_info["name"]+NOR CYN"���~�]�p��"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	actions = ([ "readplan" : (: do_readplan :) ]);

	if( this_object()->set_shadow_database() ) return;
	
	set("long", "�@�������}����"+product_info["name"]+"("+capitalize(product_info["id"])+")���~�]�p�ϡA�γ\�z��q���ǲߨ�@�ǯS�����~�Ͳ���k�C\n\n  ");
	set("unit", "�i");
	set("mass", 100);
	set("value", 10000);
	set("badsell", 1);
}
