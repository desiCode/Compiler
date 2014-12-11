/************************************************************************************************************************************
File name: table.h
Compiler: Microsoft Visual Studio 2010 and GCC 
Author: Lovejot Singh Dew
Course: CST 8152 ñ Compilers Lab Section: 402 
Assignment: 2
Date: Oct 25, 2013 
Professor: Sv. Ranev
Purpose: Declarations of the defines ,array of tables used in scanner.c

************************************************************************************************************************************/

#ifndef  TABLE_H_
#define  TABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

#define EOS 255
#define ES  12 /* Error state */
#define IS -1    /* Inavalid state */
#define TRUE 1	/*using like boolean*/
#define FALSE 0	
#define  PLT_MAX 32767
#define TABLE_COLUMNS 7
/*transition table - type of states defined in separate table*/
/*a digit represents the state of machine where the would go when in this special digit*/
int  st_table[ ][TABLE_COLUMNS] = {
/* State 0*/{1,6,4,4,IS,IS,IS},
/* State 1*/{1,1,1,1,2,3,2},			
/* State 2*/{IS,IS,IS,IS,IS,IS,IS},/**/
/* State 3*/{IS,IS,IS,IS,IS,IS,IS},
/* State 4*/{ES,4,4,4,7,5,5},
/* State 5*/{IS,IS,IS,IS,IS,IS,IS},
/* State 6*/{ES,9,9,ES,7,ES,5},
/* State 7*/{8,7,7,7,8,8,8},
/* State 8*/{IS,IS,IS,IS,IS,IS,IS},
/* State 9*/{ES,ES,9,ES,ES,11,11},
/* State 10*/{ES,ES,ES,ES,ES,11},
/* State 11*/{IS,ES,IS,IS,IS,IS,IS},
/* State 12*/{ES,ES,ES,ES,ES,ES,13},
/* State 13*/{IS,IS,IS,IS,IS,IS,IS}
};

/* Accepting state table definition */

#define ASWR     1  /* accepting state with retract */
#define ASNR     2  /* accepting state with no retract */
#define NOAS     0 /* not accepting state */


/*last column of the transit table that reports state of machine*/
	int as_table[ ] ={NOAS,NOAS,ASWR,ASNR,NOAS,ASWR,NOAS,NOAS,ASWR,NOAS,NOAS,ASWR,ASNR,ASWR};

/* Accepting action function declarations */
Token aa_func02(char *lexeme);		/* AVID /KW-ASWR */
Token aa_func03(char *lexeme);		/* SVID */
Token aa_func05(char *lexeme);		/* DIL */
Token aa_func08(char *lexeme);		/* FPL */
Token aa_func11(char *lexeme);		/* OIL */
Token aa_func12(char *lexeme);		/* ES*/
Token aa_func13(char *lexeme);		/* ES*/

typedef Token (*PTR_AAF)(char *lexeme);/*pointer to function takes strings and returns token* /

/* Accepting function (action) callback table (array) definition */
/* If you do not want to use the typedef, the equvalent declaration is:
 * Token (*aa_table[])(char lexeme[]) = {
 */

PTR_AAF aa_table[ ] ={NULL,NULL,aa_func02,aa_func03,NULL,aa_func05,NULL,NULL,aa_func08,NULL,NULL,aa_func11,aa_func12,aa_func13};
	/*NUll means not accepting state of the machine 

/* Keyword lookup table (.AND. and .OR. are not keywords) */
#define KWT_SIZE  8

char * kw_table []= {
                      "ELSE",
                      "IF",
                      "INPUT",
                      "OUTPUT",
                      "PLATYPUS",
                      "REPEAT",
                      "THEN",
                      "USING"   
                     };

#endif
                     
/*********************************************************************XXXX*****************************************************************************************/
