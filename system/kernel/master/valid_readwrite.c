/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : valid_readwrite.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

#include <daemon.h>
#include <secure.h>

/*
名稱：
        valid_read - 檢查一個人是否有權讀取一個檔案。
語法：
        int valid_read( string file, object user, string func );

        整數 valid_read( 字串 檔案, 物件 使用者, 字串 函式 );
用法：
        每次當一個物件嘗試讀取檔案時，驅動程式會呼叫主宰物件的 valid_read()
        以檢查是否允許讀取。此函式的參數為檔案名稱、嘗試讀取此檔的物件、呼
        叫的函式名稱。如果 valid_read() 傳回非零值，則准許讀取。
參考：
        valid_write
作者：
        Tim Hollebeek  Beek@ZorkMUD, Lima Bean, IdeaExchange, and elsewhere
翻譯：
        spock@muds.net          2000.May.21.    v22 版
*/
int valid_read(string file, object from, string func)
{
	return SECURE_D->valid_check(file, from, func, READ);
}

/*
名稱：
        valid_write - 檢查一個物件是否有權寫入一個檔案。
語法：
        int valid_write( string file, object ob, string func );

        整數 valid_write( 字串 檔案, 物件 物件, 字串 函式 );
用法：
        每次當一個物件嘗試寫入檔案時，驅動程式會呼叫主宰物件中的
        valid_write() 以檢查是否允許寫入。此函式的參數為檔案名稱、嘗試寫入
        此檔的物件、呼叫的函式名稱〈通常是被使用的外部函式〉。如果
        valid_write() 傳回非零值，則准許寫入。
參考：
        valid_read
作者：
        Tim Hollebeek  Beek@ZorkMUD, Lima Bean, IdeaExchange, and elsewhere
翻譯：
        spock@muds.net          2000.May.21.    v22 版
*/
int valid_write(string file, object from, string func)
{
	return SECURE_D->valid_check(file, from, func, WRITE);
}
