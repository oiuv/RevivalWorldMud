/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : simul_efun.c
 * Author : Clode@RevivalWorld
 * Date   : 2000-12-28
 * Note   : �����~���禡����, �H�U Include ���Ǧ����n���p, �H�N�ܰʥi��y�� Mud �L�k�Ұ�
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <daemon.h>

#include "simul_efun/person.c"

#include "simul_efun/log.c"

#include "simul_efun/path.c"

#include "simul_efun/string.c"

#include "simul_efun/parse.c"

#include "simul_efun/file.c"

#include "simul_efun/object.c"

#include "simul_efun/zone.c"

#include "simul_efun/city.c"

#include "simul_efun/area.c"

#include "simul_efun/message.c"

#include "simul_efun/title_screen.c"

#include "simul_efun/socket.c"

#include "simul_efun/boolean.c"

#include "simul_efun/location.c"

#include "simul_efun/array.c"

#include "simul_efun/bit.c"

#include "simul_efun/math.c"

#include "simul_efun/secure.c"

#include "simul_efun/int.c"

#include "simul_efun/process.c"

private void create()
{
	printf("Simul Efun Object(/system/kernel/simul_efun.c) ���J���\\n");
}

string iconv_string(string from, string to, string text)
{
	printf("�� Efun �Q�h�ìO Crasher �ثe�Q���ꤤ, �Фť��N�ϥ�\n");
	return 0;
}