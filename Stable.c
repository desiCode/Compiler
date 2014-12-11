/************************************************************************************************************************************
File name: stable.c
Compiler: Microsoft Visual Studio 2010 and GCC 
Author: Lovejot Singh Dew
Course: CST 8152 ñ Compilers Lab Section: 402 
Assignment: 3
Date: Nov 15, 2013 
Professor: Sv. Ranev
Version/History : The number tells you how many times the function has been edited !
Purpose: 

Function List:
STD st_create(int st_size);
int st_install(STD sym_table, char *lexeme , int line);
int st_lookup(STD sym_table, char * lexeme);
int st_update_type(STD sym_table, int vid_offset,char v_type);
int st_update_value(STD sym_table , int vid_offset);
char st_get_type(STD sym_table);
void st_destroy(STD sym_table);
int st_print(STD sym_tabe);
static void st_setsize(void);
static void st_incoffset(void);
int st_store(STD sym_table);
int st_sort(STD sym_table,char s_order);
static int compare_a(const void *a, const void *b)
static int compare_d(const void *a, const void *b)

********************************************************************************************************/
/*************************************includes*************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include "stable.h"
#include <stdlib.h>
#include <string.h>  /* string functions */

/*********************************************************************************************************/
 extern STD sym_table;
extern int scerrnum;
/*******************************************************************************************************************************************************
Purpose: The function creates a new empty sym_table then it allocates dynamic memory for an array of STVR with st_size provided as parameter.Then 
			it creates a self incrementing buffer that is initialized to plsBD.On successful operation is sets st_size to st_size else sets st_size to 0
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: malloc() b_create()
Parameters: st_size of type int is number of elements used to allocate dynamic memory to array
Return value: sym_table of type STD which symbol table descriptor
Algorithm: take size as parameter to create an array of that size , then create a self incrementing buffer check if both the functions have worked properly
		   and return created sym_table
********************************************************************************************************************************************************/
STD st_create(int st_size){
	
	STD sym_table;       /*local variable */
	sym_table.pstvr =(STVR*) malloc(st_size *sizeof(STVR));      /*allocating dynamic memory for an array*/
	if(sym_table.pstvr == NULL){						/*to check if the function worked successfully*/
		sym_table.st_size= 0;
		return sym_table;	
	}
	sym_table.plsBD= b_create(INT_CAP,INT_INC,'a');				/*creating a self incrementing buffer */
	/*sym_table.plsBD= b_create(3,5,'a');	works with reallocation also*/
	if(sym_table.plsBD!=NULL){									/*if the function is successful*/
	sym_table.st_offset=ZERO;										/*set offset to zero*/
	sym_table.st_size=st_size;						/*set symbol table size to size provided in parameter*/
	}
	else sym_table.st_size=ZERO;				/*if function failed set size to zero*/
	
	return sym_table;							/*return created sym_table*/
}
/*******************************************************************************************************************************************************
Purpose: The function installs the lexeme into the symbol table setting the various fields related to it.
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: st_lookup() strlen() b_get_chmemloc() b_getsize() b_addc()st_incoffset()
Parameters: lexeme : sym_table of type STD a symobl table descriptor
lexeme of type char * holds the string that is to be installed into the symbol table
line of type int : line number where the string was found in the file
Return value: offset the memory location where the lexeme is added in the sym_table array pstvr
lookup_offset ; if the lexeme is already in the sumbol table return the offset where was found first
FAIL_A : if the symbol table is full
Algorithm: the function first checks to see if there is space in the symbol table if yes then checks if the lexeme was not previously added 
			to symol table if no then adds the lexeme at the current st_offset and sets plex to memolocation of the lexeme in the buffer,
			oline to line provided in parameters and also set the status field bits using the bitwise operations increments st_offset of the
			global symbol table using the static funtion . then return the offset where the lexeme is added in the pstvr or if its found inthe lookup function
********************************************************************************************************************************************************/
int st_install(STD sym_table, char *lexeme , int line){

	short offset;		/* offset needed everytime a new lexeme is to be added to plsBD buffer*/
	 int len,			/**loop variable used to run the for loop*/
	 loop,		/*len used for length of the lexeme  */
	 look_offset;	/*offset used to store the offset returned by st_lookup() */
	char flag=0;
	
	if(sym_table.st_size <= ZERO)
		return F_FAIL;

	
	look_offset=st_lookup(sym_table,lexeme);	
	if(look_offset == -1){
		if(sym_table.st_size  == sym_table.st_offset){
		return FAIL_A;			/*return -1 if the symbol table is full*/
	}
		/* instal the lexeme into storage */
		len =strlen(lexeme);		
		sym_table.pstvr[sym_table.st_offset].plex = b_get_chmemloc(sym_table.plsBD,b_getsize(sym_table.plsBD));	/*storing the pointer the lexeme inserted by the symbol_table into the CA(plsBD)*/
		
		for(loop=0;loop<len;loop++){			/*loop to add char into plsBD buffer*/
			b_addc(sym_table.plsBD,lexeme[loop]);
			if(b_addc == NULL){return F_FAIL ;}
			if(b_get_r_flag(sym_table.plsBD)==1)
				flag=ONE;
		}
		b_addc(sym_table.plsBD,'\0');			/*adding string terminator to ca_head*/
		if(b_addc == NULL){return F_FAIL ;}
		if(b_get_r_flag(sym_table.plsBD)==1)
				flag=ONE;					/*if buffer reallocated set the flag*/

		if(flag== ONE){		/*check if flag in on which is 1 */
			offset=0,		/*used to calculated offset of the next string*/
				len=0;		/*length of the lexeme*/
			for(loop=0;loop<=sym_table.st_offset;loop++){	
				len=strlen(b_get_chmemloc(sym_table.plsBD,offset));		
				sym_table.pstvr[loop].plex = b_get_chmemloc(sym_table.plsBD,offset);		/*adding the lexeme memory location to plex*/
				offset =offset +(short) len+1;		/*calculating the offset of the next lexeme in the buffer plsBD*/
			}/*end of for*/
		}/*end of flag if*/

		sym_table.pstvr[sym_table.st_offset].o_line=line;		/*line number where the string was found in the file*/
		sym_table.pstvr[sym_table.st_offset].status_field |=DEF_VAL;	/*setting the bits to their default values 0xFFF8*/
			

		if(lexeme[len-1]=='#'){						/*to check if the lexeme is a SVID */
			sym_table.pstvr[sym_table.st_offset].status_field |=ST_SRT;		/*setting data type indicators to 11*/
			sym_table.pstvr[sym_table.st_offset].status_field |=ST_UFLG;	/*setting the update bit as strings cannot be updated to any other data type*/
			sym_table.pstvr[sym_table.st_offset].i_value.str_offset=-1;		/* setting i_value of the string*/
		}
		else if(lexeme[0]=='i' || lexeme[0]=='o'|| lexeme[0]=='d' || lexeme[0]=='n'){
		    sym_table.pstvr[sym_table.st_offset].status_field |=ST_INT;		/*setting the data type indicator to int i.e. using OR operation*/
			sym_table.pstvr[sym_table.st_offset].i_value.int_val=0;			/*setting i_value to int*/
		}
		else {	/*we have a float*/
			sym_table.pstvr[sym_table.st_offset].status_field |=ST_FLT;
			sym_table.pstvr[sym_table.st_offset].i_value.fpl_val=0.0f;
		}
		
		offset = sym_table.st_offset; 
		st_incoffset();		/*increasing the st_offset of the global symbol table*/
		
		return offset;		/*return offset where the lexeme is added*/
		
	}/*end of if lookup*/
	
	return look_offset;		/*offset return by the look up function*/
}
/*******************************************************************************************************************************************************
Purpose: the function gets called in install it takes a valid sym_table and a lexeme as parameter.Then goes throught the array pstvr in reverse order
		if the string is found in the array it return the offset of it else return FAIL_A(-1) to indicate it didn't find the string in it.
Author: Lovejot Singh Dew
History/Versions: 1
Called functions:strcmp()
Parameters:sym_table : a valid symbol table
lexeme : svid or avid that's needed to install in the sym_table.
Return value: FAIL_A if the lexeme was not found , returns index of lexeme found in the pstvr by loop or
			returns F_FAIL is sym_table was not valid
Algorithm:	check if the suplied sym_table is a valid table and also check to to see if its offset to greater than 0 as there is no need to look up for the first lexeme
			.run a loop reverse order in the array pstvr return the index of array if the lexeme match any lexemes in the table .The lexeme equality is checked using
			strcmp() function

********************************************************************************************************************************************************/
int st_lookup(STD sym_table, char * lexeme){
	int loop;		/*variable used to run the loop*/
	
	if(sym_table.st_size<=0)	/*to check for a valid sym_table*/
		return F_FAIL;			/*return -2 on failure*/

	if(sym_table.st_offset>0){	/*no need to lookup for the first lexeme*/
		for(loop=sym_table.st_offset-1;loop>=0;loop--){			/*loop in reverse order*/
			if(strcmp(sym_table.pstvr[loop].plex,lexeme)==0){			/*comparing strings to see the difference*/
				return loop;				/*return the index if the lexeme matches any other lexeme in the array pstvr*/
			}/*end of if*/
		}/*end of for*/
	}/*end of if*/
	
	return FAIL_A;	/*return -1 if nothing is found or if its the first lexeme being added in the array*/
}
/*******************************************************************************************************************************************************
Purpose: the function updates the date type of the lexeme pointed by vid_offset with the data type specified by v_type in the symbol table specified by sym_table
Author: Lovejot Singh Dew
History/Versions: 2
Called functions:none
Parameters:sym_table : a valid sym_table 
vid_offset : offset of lexeme in the array who's value needs to be updated
Return value: F_FAIL if invalid sym_table : vid_offset : on successful modification FAIL_A : if value could not be updated or wrong data type indicator  
Algorithm: check for a valid symbol table by checking its size greater than zero if a valid table.check the update bit of the lexeme at the offset provided by vid_offset
			if the bit is 0 update the value to data tyoe given by v_type and set the update bit to 1 if the bit cannot be updated return FAIL_A
********************************************************************************************************************************************************/
int st_update_type(STD sym_table, int vid_offset,char v_type){
	
	if(sym_table.st_size <=0  || vid_offset > sym_table.st_offset || vid_offset <0)/*a check for a vlid symtable*/
		return F_FAIL;

	if((sym_table.pstvr[vid_offset].status_field & CHK_UFLG)==0){		/*check if update bit is 0*/
		sym_table.pstvr[vid_offset].status_field &= DEF_VAL;	/*seting the data bits to 0*/		
		if(v_type =='I'){				/*change the data type to int*/
			sym_table.pstvr[vid_offset].status_field |=ST_INT;		/*updating them to int*/
			}
			else if(v_type=='F'){
				sym_table.pstvr[vid_offset].status_field |= ST_FLT;
			}
			sym_table.pstvr[vid_offset].status_field |= ST_UFLG;
			
			return vid_offset;
		}
		else return FAIL_A;		/*if the string cannot be updated*/
	
	
}
/*******************************************************************************************************************************************************
Purpose: the function updates the intial value of the lexeme in the symbol table specified by the vid_offset to i_value .
Author: Lovejot Singh Dew
History/Versions: 2
Called functions: st_get_type()
Parameters:sym_table : pointer to symbol table
vid_offset : offset of the lexeme in the array pstvr
i_value : the new value that needs to be updated 
Return value: on successful operation it return vid_offset
FAIL_A on failure 
Algorithm: check for valid symbol table , valid vid_offset call the function st_get_type to get the data type of the lexeme and update the ivalue accordingly adn return vid_offset
			on success 
********************************************************************************************************************************************************/
int st_update_value(STD sym_table , int vid_offset,InitialValue i_value){

	if(sym_table.st_size <=0|| vid_offset < 0 || vid_offset >= sym_table.st_offset)	/*check for validity  >= for sym_table.st_offset as the offset is always incremented
																					after installing a lexeme in the table*/ 
		return  FAIL_A;

	switch(st_get_type(sym_table,vid_offset)){		/*st_get_type returns a char that tells the data type of the lexeme or avid*/
		case 'I':
		case 'F':
		case 'S':
			sym_table.pstvr[vid_offset].i_value=i_value;
			break;
		default:
			return FAIL_A;
	}
	return vid_offset;
}
/*******************************************************************************************************************************************************
Purpose: the function return the data type of the lexeme present at the offset vid_offset in the array pstvr of symbol table
Author: Lovejot Singh Dew
History/Versions: 1
Called functions:none
Parameters:sym_table : pointer to symbol table
		  vid_offset : offset of the lexeme in the array pstvr
Return value: FAIL_A : on invalid sym_table or returns I for integer F for floating point and S for strings
Algorithm:  get the lexeme compare it to some hex value and determine what data type it is
********************************************************************************************************************************************************/
char st_get_type(STD sym_table,int vid_offset){
		if(sym_table.st_size <=0)
			return FAIL_A;	

		if((sym_table.pstvr[vid_offset].status_field &ST_INT )==TW_O){ /* AND the staus field with defines to check the data type*/
			return 'I';
		}
		else if((sym_table.pstvr[vid_offset].status_field & ST_FLT )== FOU_R){
			return 'F';
		}
		else if((sym_table.pstvr[vid_offset].status_field & ST_SRT )==SI_X)
			return 'S';
		return FAIL_A;
}

/*******************************************************************************************************************************************************
Purpose: the function destroys all teh dynamic memory allocated to the sym_table
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: free() b_destroy()
Parameters: sym_table a valid symbol table
Return value:	none
Algorithm: free the array pstvr destroy the buffer and set size of the sym_table to 0

********************************************************************************************************************************************************/
void st_destroy(STD sym_table){
	if(sym_table.st_size >0){
		free(sym_table.pstvr);
		b_destroy(sym_table.plsBD);
		sym_table.st_size=0;
	}
}
/*******************************************************************************************************************************************************
Purpose: the function prints lexemes in the symbol table with the line numbers they were first found in the file
Author: Lovejot Singh Dew
History/Versions: 1
Called functions:printf()
Parameters: sym_table
Return value: FAIL_A is invalid symbol table or returns a count of number of entries printed
Algorithm: 

********************************************************************************************************************************************************/
int st_print(STD sym_table){
	
	if(sym_table.st_size<=0)
		return FAIL_A;
	
	else {
	int loop ,		/*variable used to run the loop*/
		count=0;	/*number of entries printed*/

	printf("\nSymbol Table\n____________\n\n");
	printf("Line Number Variable Identifier\n");
	
	for(loop=0;loop<sym_table.st_offset;loop++){
		printf("%2d          %s\n",sym_table.pstvr[loop].o_line,sym_table.pstvr[loop].plex);
	++count;
	}
	return count;
	}
}
/*******************************************************************************************************************************************************
Purpose: the function is used to set the size of the global sym table to zero as other functions don't have the access to the global symbol table
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: none
Parameters:void
Return value: none
Algorithm: set size of the global sym_table to 0

********************************************************************************************************************************************************/
static void st_setsize(void){
	 sym_table.st_size=ZERO;
}
/*******************************************************************************************************************************************************
Purpose: increment the offset of the global symbol table
Author: Lovejot Singh Dew
History/Versions: 1
Called functions:none
Parameters:	void
Return value: none
Algorithm: increment the value of the global symbol table 

********************************************************************************************************************************************************/
static void st_incoffset(void){
			++sym_table.st_offset;
}
/*******************************************************************************************************************************************************
Purpose: the function is used to store all the data of the symbol table in a file when the symbol table is full
Author: Lovejot Singh Dew
History/Versions: 1
Called functions:fopen() 
Parameters:
Return value: 
Algorithm: 

********************************************************************************************************************************************************/
int st_store(STD sym_table){
	FILE * fi;
	int records=0,loop;
	
	if(sym_table.st_size<=0)
		return F_FAIL;
	
	fi=fopen(OUTFILE,"wt");		/*this will produce warning*/
	if(fi!=NULL){
		
		fprintf(fi,"%d",sym_table.st_size);			/*print the size of the sym_table*/
		for(loop=0;loop<sym_table.st_offset;loop++){
			
			fprintf(fi," %X ",sym_table.pstvr[loop].status_field);		/*print the status field in hex format*/
			fprintf(fi,"%d ",strlen(sym_table.pstvr[loop].plex));		/*print the length of the  lexeme*/
			fprintf(fi,"%s ",sym_table.pstvr[loop].plex);				/*print the lexeme*/
			fprintf(fi,"%d ",sym_table.pstvr[loop].o_line);				/*print the line number of the lexeme*/
			if(st_get_type(sym_table,loop) == 'F'){						/*print the i_value of the lexeme*/
			fprintf(fi,"%.2f",sym_table.pstvr[loop].i_value);
			}
			else 
				fprintf(fi,"%d",sym_table.pstvr[loop].i_value);
			++records;				/*increase the record count by 1*/
		}
		printf("Symbol Table stored.\n");
		fclose(fi);			/*close the file*/
		return records;			
	}
	else return FAIL_A;

}
/*******************************************************************************************************************************************************
Purpose: the functino sorts the array pstvr in ascending or decending order provided by the user .
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: qsort() compare_a()
Parameters: sym_table : a valid symbol table 
			s_order : char that is responsible for the sort function to sort accordingly
Return value: 0
Algorithm: ce

********************************************************************************************************************************************************/
int st_sort(STD sym_table,char s_order){

/*	int loop=0,
		loop2=0,					/*used for second bonus but the program crashes after printing garbage collecting .... don't know why
		len=0,								
		offset=0;
	char * lex;
	Buffer * temp=NULL;*/
	if(sym_table.st_size <=ZERO)
		return F_FAIL;

	if(s_order == 'A')			/*sort the array in acsending order*/
		qsort(sym_table.pstvr, sym_table.st_offset, sizeof(STVR), compare_a);
	else if(s_order == 'D')		/*sort the array in decending order*/
		qsort(sym_table.pstvr, sym_table.st_offset, sizeof(STVR), compare_d);
	
	/*						aranges the buffer pslBD in same order as by the sort function but crashes
	temp=b_create(b_getsize(sym_table.plsBD),INT_INC,'a');

	for(loop=0;loop< sym_table.st_offset;loop++){
		
		len=strlen(sym_table.pstvr[loop].plex);
		lex=sym_table.pstvr[loop].plex;
		for(loop2=0;loop2<len;loop2++){
			b_addc(temp,lex[loop2]);
		}
		b_addc(temp,'\0');
	}
	
		b_destroy(sym_table.plsBD);
		
		sym_table.plsBD=temp;

		for(loop=0;loop<sym_table.st_offset;loop++){
				len=strlen(b_get_chmemloc(sym_table.plsBD,offset)) +1;
				sym_table.pstvr[loop].plex = b_get_chmemloc(sym_table.plsBD,offset);
				offset =offset + len;
			}
		*/	 

	return ZERO;
}
/*******************************************************************************************************************************************************
The function was needed by the qsort() to sort the array into ascending order
Purpose: the function takes two strings and returns a difference btw the two strings
Author: Lovejot Singh Dew
History/Versions: 1
Called functions:strcmp()
Parameters:const void *  a : string 1
		const void *b : string 2
Return value: value returned by the strcmp()
Algorithm: 

********************************************************************************************************************************************************/
static int compare_a(const void *a, const void *b)
{
    return strcmp( ((const STVR*)a)->plex, ((const STVR*)b)->plex );
}
/*******************************************************************************************************************************************************
the function was need by the qsort() to sort the array into decending order
Purpose: the function takes two parameters and return the difference between the two

Author: Lovejot Singh Dew
History/Versions: 1
Called functions:strcmp()
Parameters:	const void *a : string 1
			const void *b : string 2
Return value: the functions returns the value return by the strcmp
Algorithm: 

********************************************************************************************************************************************************/
 static int compare_d(const void *a, const void *b)
{
    return strcmp( ((const STVR*)b)->plex, ((const STVR*)a)->plex );
}
/******************************************END OF FILE*****************************************************/
/******************************************END OF FILE*****************************************************/
/******************************************END OF FILE*****************************************************/
 
