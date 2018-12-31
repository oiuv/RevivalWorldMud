/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : /include/equipment.h
 * Author : Clode@RevivalWorld
 * Date   : 
 * Note   : INCLUDE file
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#define PART_ID			0	// ����N��
#define PART_AMOUNT		1	// ������i�˳ƪ��˳Ƽƶq
#define PART_USER_AMOUNT	2	// ���˳ƦP�ɥi�H�Q�h�֤H�˳�
#define PART_OUTSIDE		3	// ���˳ƬO�_������~���˳�
#define	PART_CONFLICT		4	// ���쪺�Ĭ���(�Ҧp "���u�˥�" ����M "�y�M"/"���k��" �P�ɸ˳�


// �˳Ƴ���w�q			({ �N��, 	�i�˳Ƽƶq, 	�i�P�ɸ˳ƤH��,	�O�_���~���˳�,	�˳Ƴ���Ĭ� })

#define EQ_FINGER		({ "finger", 	2, 		1, 		0,		({})		})
#define EQ_MOUNT		({ "mount",	1,		1, 		0,		({})		})
#define	EQ_BROOCH		({ "brooch",	1,		1, 		0,		({})		})
#define EQ_EYE			({ "eye",	1,		1,		0,		({})		})
#define EQ_NOSE			({ "nose",	1,		1,		0,		({})		})
#define EQ_HAND			({ "hand",	2,		1,		0,		({ "twohand" }) })
#define EQ_TWOHAND		({ "twohand",	1,		1,		0,		({ "hand" })	})
#define EQ_BODY			({ "body",	1,		1,		0,		({})		})
#define EQ_HEAD                 ({ "head",      1,              1,              0,              ({})            })

