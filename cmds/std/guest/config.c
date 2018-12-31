/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : config.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-00-00
 * Note   : config ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
#include <runtime_config.h>

inherit COMMAND;

string help = @HELP
	�з� config ���O�C
HELP;

private void command(object me, string arg)
{
	string msg = "";
	
       	if( !is_command() ) return;
       	
	if( !arg )
	{
		msg += "__MUD_NAME__				: "+get_config(__MUD_NAME__)+"\n";
		msg += "__ADDR_SERVER_IP__			: "+get_config(__ADDR_SERVER_IP__)+"\n";
		msg += "__MUD_LIB_DIR__				: "+get_config(__MUD_LIB_DIR__)+"\n";
		msg += "__BIN_DIR__				: "+get_config(__BIN_DIR__)+"\n";
		msg += "__LOG_DIR__				: "+get_config(__LOG_DIR__)+"\n";
		msg += "__INCLUDE_DIRS__			: "+get_config(__INCLUDE_DIRS__)+"\n";
		msg += "__SAVE_BINARIES_DIR__			: "+get_config(__SAVE_BINARIES_DIR__)+"\n";
		msg += "__MASTER_FILE__				: "+get_config(__MASTER_FILE__)+"\n";
		msg += "__SIMUL_EFUN_FILE__			: "+get_config(__SIMUL_EFUN_FILE__)+"\n";
		msg += "__SWAP_FILE__				: "+get_config(__SWAP_FILE__)+"\n";
		msg += "__DEBUG_LOG_FILE__			: "+get_config(__DEBUG_LOG_FILE__)+"\n";
		msg += "__DEFAULT_ERROR_MESSAGE__		: "+get_config(__DEFAULT_ERROR_MESSAGE__)+"\n";
		msg += "__DEFAULT_FAIL_MESSAGE__		: "+get_config(__DEFAULT_FAIL_MESSAGE__)+"\n";
		msg += "__GLOBAL_INCLUDE_FILE__			: "+get_config(__GLOBAL_INCLUDE_FILE__)+"\n";
		msg += "__MUD_PORT__				: "+get_config(__MUD_PORT__)+"\n";
		msg += "__ADDR_SERVER_PORT__			: "+get_config(__ADDR_SERVER_PORT__)+"\n";
		msg += "__TIME_TO_CLEAN_UP__			: "+get_config(__TIME_TO_CLEAN_UP__)+"\n";
		msg += "__TIME_TO_RESET__			: "+get_config(__TIME_TO_RESET__)+"\n";
		msg += "__TIME_TO_SWAP__			: "+get_config(__TIME_TO_SWAP__)+"\n";
		msg += "__COMPILER_STACK_SIZE__			: "+get_config(__COMPILER_STACK_SIZE__)+"\n";
		msg += "__EVALUATOR_STACK_SIZE__		: "+get_config(__EVALUATOR_STACK_SIZE__)+"\n";
		msg += "__INHERIT_CHAIN_SIZE__			: "+get_config(__INHERIT_CHAIN_SIZE__)+"\n";
		msg += "__MAX_EVAL_COST__			: "+get_config(__MAX_EVAL_COST__)+"\n";
		msg += "__MAX_LOCAL_VARIABLES__			: "+get_config(__MAX_LOCAL_VARIABLES__)+"\n";
		msg += "__MAX_CALL_DEPTH__			: "+get_config(__MAX_CALL_DEPTH__)+"\n";
		msg += "__MAX_ARRAY_SIZE__			: "+get_config(__MAX_ARRAY_SIZE__)+"\n";
		msg += "__MAX_BUFFER_SIZE__			: "+get_config(__MAX_BUFFER_SIZE__)+"\n";
		msg += "__MAX_MAPPING_SIZE__			: "+get_config(__MAX_MAPPING_SIZE__)+"\n";
		msg += "__MAX_STRING_LENGTH__			: "+get_config(__MAX_STRING_LENGTH__)+"\n";
		msg += "__MAX_BITFIELD_BITS__			: "+get_config(__MAX_BITFIELD_BITS__)+"\n";
		msg += "__MAX_BYTE_TRANSFER__			: "+get_config(__MAX_BYTE_TRANSFER__)+"\n";
		msg += "__MAX_READ_FILE_SIZE__			: "+get_config(__MAX_READ_FILE_SIZE__)+"\n";
		msg += "__RESERVED_MEM_SIZE__			: "+get_config(__RESERVED_MEM_SIZE__)+"\n";
		msg += "__SHARED_STRING_HASH_TABLE_SIZE__	: "+get_config(__SHARED_STRING_HASH_TABLE_SIZE__)+"\n";
		msg += "__LIVING_HASH_TABLE_SIZE__		: "+get_config(__LIVING_HASH_TABLE_SIZE__)+"\n";
		
		tell(me, msg+"\n", CMDMSG);
	}
	else
		tell(me, msg, get_config(to_int(arg))+"\n", CMDMSG);
}
