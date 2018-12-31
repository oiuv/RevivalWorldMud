/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : /include/map.h
 * Author : Clode@RevivalWorld
 * Date   : 
 * Note   : INCLUDE file, �w�q����ƥi�Y�p����x�s
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */
 
/* �@�i�a�Ϫ��j�p�W�d */
#define MAP_WIDTH 	100	//�e
#define MAP_LENGTH 	100	//��

/* �i���Ͻd��, ���P�e�ҥ������_��, �H�~���n�b�a�ϥ����� */
/* �s�W�B�׽d��]�w, �{�b�� 25:9 ��ҭ�n, �Y�L�ݭn���קK���, �_�h���B�׭p��|�����D */
#define CITY_VISION_WIDTH 	25	//�i���a�Ϥj�p-�e
#define CITY_VISION_LENGTH 	9	//�i���a�Ϥj�p-��
#define AREA_VISION_WIDTH 	31	//�i���a�Ϥj�p-�e
#define AREA_VISION_LENGTH 	9	//�i���a�Ϥj�p-��

#define	AGRICULTURE_REGION	(1<<0)		// �A�~��
#define	INDUSTRY_REGION		(1<<1)		// �u�~��
#define	COMMERCE_REGION		(1<<2)		// �ӷ~��
#define AREA_REGION		(1<<3)		// ����

#define GOVERNMENT		(1<<0)		// �F���ؿv
#define	ENTERPRISE		(1<<1)		// ���~���Ϋؿv
#define INDIVIDUAL		(1<<2)		// �p�H�ؿv

/* �a�Ϯy�и�Ʈw�]�w�� KEY ���w�q */
#define FLAGS		1	// ��ƺX��
#define TYPE		2	// �ت����A
#define ROOM		3	// �ж����A�θ��|
#define SPECIAL_EXIT	4	// �S��X�f
#define VALUE		6	// �g�a����

/* FLAGS �����줸�ˬd���ةw�q */
#define NO_MOVE		(1 << 0)	// ����L�k�q��X��
#define NO_VIEW         (1 << 1)  	// ����L�k�Q�ݨ�

/* �ت����A TYPE ���������A */
//
#define MOUNTAIN	1	//�s��
#define FOREST		2	//�˪L
#define RIVER		3	//�e�t
#define SEA		4	//���v
//
#define ROAD		21	//�D��
#define BRIDGE		22	//����
//
#define DOOR		41	//��
#define WALL		42	//���
//
#define FARM		61	//�A��
#define PASTURE		62	//����
#define FISHFARM	63	//�i�޳�
//
#define LIGHT		101	//���O
#define POOL		102	//����
#define PAVILION	103	//�D�F
#define TREE		104	//�j��
#define GRASS		105	//���
#define STATUE		106	//�J��

#define MAP_FOREST	0	// �˪L���A���a��
#define MAP_OCEAN	1	// ���v���A���a��

