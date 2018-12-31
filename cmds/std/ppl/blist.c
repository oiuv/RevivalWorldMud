/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : blist.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-14
 * Note   : ���a���O�{��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <roommodule.h>

inherit COMMAND;

string generate_building_list();

string help = @HELP
 �ؿv���C��                                                           [blist]
������������������������������������������������������������������������������
      �o�ӫ��O�i�H���A�ݨ�ثeRW�̶}�񪺫ؿv���A�H�έn�}�i�o�ؿv�һݪ��̧C��
  ��C���A���n���سy([1;33mbuild[m) �K�X������A�ӥB�����n���@���H�W�����C

      ��A���U[1;33mblist[m �ɡA�|�X�{�p�U�e���G
HELP + generate_building_list()+ @HELP

  �D�����B�W�١G
          �N�O�o�ؿv���D�n�\�ΡA�Y�O�ݤ����^��... �ݥL������W�٤]�j���i�H�F
      �ѥL�D�n����O�a�H

  �D�̤֩ж�����G
          ���]�A����b�����̤֮�ƭ���A�Ҧp[1;36m�p��[m�ө��n�F... �����̤֩ж�����
      �O�@�A���դ@�I�N�O�A�o�R�E��a�C�b�K���a�W�\�W[1m���[m�A�ѤU�������զ⪺�@
      ��A�~�i�H�}�i([1;33mopen[m)�A��[1;36m�p��[m�ө��C

          �H�ϥ��|�Ҫ��ܡG
        
        �����u�̤֩ж�����v�Һ⪺�ƶq
        ���������n���ӻ\����Ϊ��a�C

          �̤֩ж����� = 3

      �@��������    �� = 4    �j��̤֩ж�����C
        ��������
        ��������    �� = 12   ��®��ӻ\�K�X������a�C
      �@��������

             ��M�A�������ܤ]�i�H�\���G
                                                        ������
        ��������    ��������    ������        ������    ������
        ��������    ��������    ��������    ��������    ������    ����������
        ��������    ��������    ��������    ��������    ������    ����������
          ������    ������      ��������    ��������    ������    ����������

          �o�����M�ݭn�R���a�i�H�֤@��A���ƹ�W�����������٬O�򥿤�Τ@�ˡC

  �D�ݩʡG
          ²�檺�����N�O�A���ݪ������A�@�}�l���O��[36m�p�H[m�}�l�C�ӫ�i������ξ�
      �����Цa�����@�w�{�פ���i�H�}���q�][1;30m���~[m�^�A�Ϊ̬O�g�ѧ벼���|�Ӧ�����
      ���][35m�F��[m�^�C

          ���ثe�٥u��[36m�p�H[m�ݩʡA[1;30m���~[m�M[35m�F��[m�ݩʥ��b�}�o���A�ҥH�L�k�ԭz�C

  �D�ϰ�G
          �b�@�}�l���ɭԡA��a�w�g�Q�F�������n�F�U�إΦa�C�Ҧ����ؿv��������
      �ؿv�F���W���n���Φa���W�A���ǥu��Q�ؿv�b�Y�ǥΦa�W�C�p�G[1;33m�Ȧ�[m�A�u��}
      �i��[1;36;46m�ӷ~��[m���W�F[1;36m�p��[m�ө��h�O�T�إΦa���i�H�}�i�C�C�إΦa���a�������P�A
      �b�سy�ɶ��n�`�N�C

  �D�}�i��O�G
          ��A�w�g�N�H�W����@�@�F�����ɡA�b�}�i���e���̫�@�ӭ���A�N�O�}�i
      ��O�C���]�A�n�\���O�@��[1;36m�p��[m�ө��A�w�b�Φa�W�R�F��������ơA�ä@�@�سy
      �X�K�X������]�]�A���^�C�A���W�����n�������������A�~��[1;33mopen store[m�A�}�i
      �A��[1;36m�p��[m�ө��C

[33m    �������O: build, open
[m������������������������������������������������������������������������������
                                                            �������s���@��
HELP;



string generate_building_list()
{
        array data;
        string msg, buildingtype, buildingregion, *sortid;
        mapping building_table = BUILDING_D->query_building_table();
        string age;
	string *manual_sort = ({
		"cityhall",
		"recycle",
		"labor",
		"trading_post",
		"bank",
		"postoffice",
		"jail",
		"auction",
		"trainstation",
		"airport",
		"space",
		"nuclear",
		"prodigy",

		0,
		"enterprise",
		"stock",
		"university",
		0,
		"store",
		"warehouse",
		"house",
		"fence",
		"cabin",
		"expedition",
		"research",
		"resource",
		0,
		
		"metal",
		"stone",
		"water",
		"wood",
		"fuel",
		
		0,
		"food",
		"drink",
		"clothing",
		"instrument",
		"furniture",
		"hardware",
		"chemical",
		"machinery",
		"electrics",
		"transportation",
		"entertainment",
		"medicine",
		"adventure",
		"shortrange",
		"longrange",
		"armor",
		"heavyarmor",
		"magic",
	});

        sortid = manual_sort | keys(building_table);
       
       	msg = "�ؿv�C��A�ثe�@�i�سy "+sizeof(manual_sort-({0}))+" �ثؿv�G\n";
        msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
        msg +=        "�ؿv����       �W��   �̤֩ж� �� �W���� �c�a �Ӽh     ��O/�C�� �ӭ� ���� �ɥN �}\n";
        msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
        
        
        foreach( string id in sortid )
        {
        	if( !stringp(id) )
        	{
        		msg += "\n";
        		continue;
        	}

                data = building_table[id];
                
                if( !data || !data[ROOMMODULE_OPENCOST] ) continue;
                
                if( data[ROOMMODULE_BUILDINGTYPE] & GOVERNMENT )
                        buildingtype = MAG"�F"NOR;
                else if( data[ROOMMODULE_BUILDINGTYPE] & ENTERPRISE )
                        buildingtype = WHT"��"NOR;
                else if( data[ROOMMODULE_BUILDINGTYPE] & INDIVIDUAL )
                        buildingtype = CYN"��"NOR;
		
		switch(data[ROOMMODULE_AGE])
		{
			case 0: age = WHT"�v�e"NOR; break;
			case 1: age = HIG"�A"NOR GRN"�~"NOR; break;
			case 2: age = HIY"�u"NOR YEL"�~"NOR; break;
			case 3:	age = HIC"��"NOR CYN"��"NOR; break;
			case 4: age = HIB"�t"NOR BLU"�z"NOR; break;
			case 5: age = HIR"��"NOR RED"��"NOR; break;
		}

		if( data[ROOMMODULE_REGION] & AREA_REGION )
			buildingregion = HIW BGRN" ���� "NOR;
		else
                	buildingregion = ( data[ROOMMODULE_REGION] & AGRICULTURE_REGION ? HIG BGRN"�A"NOR : "  ")+
					 ( data[ROOMMODULE_REGION] & INDUSTRY_REGION ? HIY BYEL"�u"NOR : "  ")+
					 ( data[ROOMMODULE_REGION] & COMMERCE_REGION ? HIC BCYN"��"NOR : "  ");

                msg += sprintf("%-15s%-13s%2d %-3s%-7s%-5d%-4d%14s %|4s %|4s %4s %s\n", 
                        HIY+capitalize(id)+NOR, 
                        data[ROOMMODULE_SHORT],
                        data[ROOMMODULE_ROOMLIMIT],
                        buildingtype,
                        buildingregion,
                        data[ROOMMODULE_FLOURISH],
                        data[ROOMMODULE_MAXFLOOR],
                        HIW+NUMBER_D->number_symbol(data[ROOMMODULE_OPENCOST])+NOR,
                        NOR WHT+(data[ROOMMODULE_MAXLIMIT]||""),
                        NOR WHT+(data[ROOMMODULE_MAXBUILDINGLIMIT]||""),
                       	age,
                        data[ROOMMODULE_TESTING] ? HIR"��"NOR : HIG"��"NOR,
                        );
        }

        msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
        
        
        return msg;
}


private void do_command(object me, string arg)
{
	me->more(generate_building_list());
}
