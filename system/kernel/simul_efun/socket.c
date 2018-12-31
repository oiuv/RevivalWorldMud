/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : socket.c
 * Author : Sinji@RevivalWorld
 * Date   : 2001-07-03
 * Note   : included by Simul_Efun Object
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
string dump_socket_status()
{
        string ret = @END
Fd    State      Mode       Local Address          Remote Address            Owner
--  ---------  --------  ---------------------  ---------------------  ---------------------
END;
        foreach(array item in socket_status()) {
                ret += sprintf("%2d  %|9s  %|8s  %-21s  %-21s  %O\n",
                    item[0], item[1], item[2], item[3], item[4], item[5]);
        }
        return ret;
}

