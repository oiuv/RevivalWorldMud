/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : material.h
 * Author : Clode@RevivalWorld
 * Date   : 
 * Note   : INCLUDE file
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#define MATERIAL_CHINESE	([ \
METAL	:	"[1;37m����[0m",	\
STONE	:	"[0;1;30m���[0m",	\
WATER	:	"[1;36m�M��[0m", 	\
WOOD	:	"[0;33m���[0m", 	\
FUEL	:	"[1;34m�U��[0m", 	\
GOLD	:	"[1;33m��[0m", 	\
IRON	:	"[1;37m�K[0m",	\
])

/* �򥻭�� */
#define METAL		"metal"		//����
#define WATER		"water"		//��
#define WOOD		"wood"		//���
#define STONE		"stone"		//���
#define FUEL		"fuel"		//�ƥۿU��

#define GOLD		"gold"		//����
#define IRON		"iron"		//�K

#define MATERIAL(x)	("/obj/materials/"+x)

// �@�ŭ��
#define M_METAL		"metal"		//����
#define M_STONE		"stone"		//�ۧ�
#define M_WOOD		"wood"		//���
#define M_WATER		"water"		//��
#define M_FUEL		"fuel"		//�U��

// �G�ŭ��
// ����
#define M_GOLD		"gold"		//��
#define M_COPPER	"copper"	//��
#define M_SILVER	"silver"	//��
#define M_TIN		"tinmine"	//��
#define M_ALUMINUM	"aluminum"	//�T
#define M_IRON		"iron"		//�K
#define M_URANIUM	"uranium"	//�\
// �ۧ�
#define M_QUARTZ	"quartz"	//�ۭ^
#define M_SILICON	"silicon"	//����
#define M_POTTER	"potter"	//���g
#define M_MARBLE	"marble"	//�j�z��
#define M_GRANITE	"granite"	//��^��
#define M_SERPENTINE 	"serpentine"	//�D����
#define M_DIAMOND	"diamond"	//�p��
#define M_LIME		"lime"		//�ۦǩ�
// ���
#define M_FIR		"fir"		//����
#define M_JUNIPER	"juniper"	//�̤�
#define M_SANDAL	"sandal"	//�Ȥ�
#define M_TEAK		"teak"		//�c��
// �U��
#define M_OIL		"oil"		//��o
#define M_GAS		"ngas"		//�ѵM��
#define M_COAL		"coal"		//���q
// ��
#define M_ROWATER	"rowater"	//�f���z��
#define M_DIWATER	"diwater"	//�h���l��
#define M_DTWATER	"dtwater"	//�]�H��

// �T�ŭ��
#define M_STEEL		"steel"		//��
#define M_PLASTICS	"plastics"	//�콦
#define M_RUBBER	"rubber"	//��
#define M_GLASS		"glass"		//����
