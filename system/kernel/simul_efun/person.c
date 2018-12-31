/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : person.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-7-2
 * Note   : included by Simul_Efun Object
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <gender.h>
#include <pronoun.h>

// �H�٥N�W��
string pnoun(int pronoun_number, mixed arg)
{
	if( objectp(arg) ) 
		arg = query("gender", arg);
	
	switch(pronoun_number)
	{
		case FIRST_PERSONAL:	
			return FIRST_PERSONAL_PRONOUN;
		
		case SECOND_PERSONAL: 
			switch( arg )
			{
				case MALE_GENDER: 	return SECOND_PERSONAL_PRONOUN_MALE;
				case FEMALE_GENDER:	return SECOND_PERSONAL_PRONOUN_FEMALE;
				default:		return SECOND_PERSONAL_PRONOUN_DEFAULT;
			}

		case THIRD_PERSONAL:    
			if( undefinedp(arg) )
				return THIRD_PERSONAL_PRONOUN_ANIMAL;     
			switch( arg )
			{
				case MALE_GENDER: 	return THIRD_PERSONAL_PRONOUN_MALE;
				case FEMALE_GENDER:	return THIRD_PERSONAL_PRONOUN_FEMALE;
				default:		return THIRD_PERSONAL_PRONOUN_DEFAULT;
			}
		default:
			error("���~�H�٥N�W���C\n");
	}
}
