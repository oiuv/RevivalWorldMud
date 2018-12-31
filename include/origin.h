/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : origin.h
 * Author : MudOS driver
 * Date   : 
 * Note   : INCLUDE file
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#define ORIGIN_BACKEND       		0x1 /* backwards compat */
#define ORIGIN_DRIVER        		0x1
#define ORIGIN_LOCAL         		0x2
#define ORIGIN_CALL_OTHER    		0x4
#define ORIGIN_SIMUL_EFUN    		0x8
#define ORIGIN_CALL_OUT      		0x10
#define ORIGIN_EFUN          		0x20
/* pseudo frames for call_other function pointers and efun pointer */
#define ORIGIN_FUNCTION_POINTER   	0x40
/* anonymous functions */
#define ORIGIN_FUNCTIONAL         	0x80
