/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : preload.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

#define FIRST_PRELOAD	"/system/daemons/system_d.c"

/* ��l�Ƨ@�~ Initialization */
/* �Y driver �ѼƬ� -e , flag ���D�s�� */
string *epilog(int flag)
{
	if( flag )
	{
		printf("�X�ʵ{�������w�����J�{�ǡC\n");
		log_file("system/preload","�X�ʵ{�������w�����J�{�ǡC\n");
		return allocate(0);
	}

	return ({ FIRST_PRELOAD });
}

/* �w�����J���� */
void preload(string file)
{
	mixed err;
	
	if( err = catch(load_object(file)) )
		log_file("system/preload",sprintf("���J %s �ɵo�Ϳ��~: %O\n", file, err));
}
