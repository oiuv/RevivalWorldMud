/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mj.h
 * Author : Clode@RevivalWorld
 * Date   : 
 * Note   : INCLUDE file
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#define DATA_PATH	"/data/object/mj112.o"
#define MJAI_FILE 	"/obj/etc/mj/mjai.c"

#define VERSION 	"1.13"

// �@��
#define BASEMONEY	10000
// �@�x
#define BONUSMONEY	4000

// Score Index
#define SCORE		0
#define ROUNDS		1
#define WINTIMES	2
#define GUNTIMES	3
#define BONUSES 	4
#define HUMYSELVES	5
#define LOSEBONUSES	6
#define LOSETIMES	7

// MJtile Index
#define CHINESE 	0
#define AMOUNT		1
#define COLOR1		2
#define COLOR2		3

// MJdata Index
#define PLAYING 	0	//�C����
#define TURN		1	//�����
#define DUMPED		2	//�����P
#define DUMPING 	3	//���P
#define MO		4	//�N�P
#define CHEAT		5	//�B�J
#define TILELIMIT	6	//�y���i��
#define ROUND		7	//�`��ƭp��
#define MROUND		8	//����ƭp��
#define WINNER		9	//�W��Ĺ�a
#define HUMYSELF	10	//�ۺN
#define HUFIRST 	11	//�ѭJ
#define NINEYOUNGS	12	//�E�\�E�M
#define LOSER		13	//��j
#define CMASTER 	14	//�s������
#define ROUNDWIND	15	//�魷
#define OPENDOOR	16	//�}��
#define FINISH		17	//�����@��
#define CFINISH 	18	//��������
#define KEYTILE 	19	//����P
#define LISTENING	20	//ť�P��
#define ONLOOKER	21	//���[��

// Pdata Index
#define PLAYER		0	//����
#define ID		1	//��ID
#define IDNAME		2	//�W��
#define TILE		3	//�D�P
#define OTILE		4	//���P
#define FLOWER		5	//��P
#define AUTOPON 	6	//�۰ʸI
#define AUTOGON 	7	//�۰ʺb
#define AUTOHU		8	//�۰ʭJ
#define LISTEN		9	//ť�P
#define TAILTILE	10	//�N��
#define LOOKSEA 	11	//�ݮ���
#define SECRETGON	12	//�t�b
#define MONEY		13	//�w�X
#define PLISTEN 	14	//�۰�ť�P
#define LISTENTYPE	15	//ť�P���A
#define AUTOOFF 	16	//�۰�����
#define FLOWERNUM	17	//��P��
#define LOOKALL 	18	//�ݥ���
#define AI		19	//AI���a
#define PDUMPED		20	//�ӤH���L���P

// Psort Index
#define MASTER		0	//���a��

#define TITLE_SCREEN_LENGTH	14
#define Dsort	({ "�F","�n","��","�_" })
#define Tsort	({ "1w","2w","3w","4w","5w","6w","7w","8w","9w","1s","2s","3s","4s","5s","6s","7s","8s","9s","1t","2t","3t","4t","5t","6t","7t","8t","9t","ea","so","we","no","ba","jo","fa","f1","f2","f3","f4","f5","f6","f7","f8", })
